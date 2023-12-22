/*
 * Copyright (C) 2013 Cable Television Laboratories, Inc.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 * notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 * notice, this list of conditions and the following disclaimer in the
 * documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
 * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"

#if ENABLE(VIDEO) && USE(GSTREAMER)

#include "TrackPrivateBaseGStreamer.h"

#include "GStreamerCommon.h"
#include "TrackPrivateBase.h"
#include <gst/tag/tag.h>
#include <wtf/text/CString.h>

GST_DEBUG_CATEGORY_EXTERN(webkit_media_player_debug);
#define GST_CAT_DEFAULT webkit_media_player_debug

namespace WebCore {

AtomString TrackPrivateBaseGStreamer::generateUniquePlaybin2StreamID(TrackType trackType, unsigned index)
{
    auto prefix = [trackType]() -> char {
        switch (trackType) {
        case TrackPrivateBaseGStreamer::TrackType::Audio:
            return 'A';
        case TrackPrivateBaseGStreamer::TrackType::Video:
            return 'V';
        case TrackPrivateBaseGStreamer::TrackType::Text:
            return 'T';
        default:
            ASSERT_NOT_REACHED();
            return 'U';
        }
    }();

    return makeAtomString(prefix, index);
}

static GRefPtr<GstPad> findBestUpstreamPad(GRefPtr<GstPad> pad)
{
    GRefPtr<GstPad> sinkPad = pad;
    GRefPtr<GstPad> peerSrcPad;

    peerSrcPad = adoptGRef(gst_pad_get_peer(sinkPad.get()));
    // Some tag events with language tags don't reach the webkittextcombiner pads on time.
    // It's better to listen for them in the earlier upstream ghost pads.
    if (GST_IS_GHOST_PAD(peerSrcPad.get()))
        sinkPad = adoptGRef(gst_ghost_pad_get_target(GST_GHOST_PAD(peerSrcPad.get())));
    return sinkPad;
}

TrackPrivateBaseGStreamer::TrackPrivateBaseGStreamer(TrackType type, TrackPrivateBase* owner, unsigned index, GRefPtr<GstPad>&& pad, bool shouldHandleStreamStartEvent)
    : m_notifier(MainThreadNotifier<MainThreadNotification>::create())
    , m_index(index)
    , m_type(type)
    , m_owner(owner)
    , m_shouldHandleStreamStartEvent(shouldHandleStreamStartEvent)
{
    setPad(WTFMove(pad));
    ASSERT(m_pad);

    m_id = AtomString(trackIdFromPadStreamStartOrUniqueID(type, index, m_pad));

    // We can't call notifyTrackOfTagsChanged() directly, because we need tagsChanged() to setup m_tags.
    tagsChanged();
}

TrackPrivateBaseGStreamer::TrackPrivateBaseGStreamer(TrackType type, TrackPrivateBase* owner, unsigned index, GstStream* stream)
    : m_notifier(MainThreadNotifier<MainThreadNotification>::create())
    , m_index(index)
    , m_stream(stream)
    , m_type(type)
    , m_owner(owner)
{
    ASSERT(m_stream);
    m_id = AtomString::fromLatin1(gst_stream_get_stream_id(m_stream.get()));

    g_signal_connect_swapped(m_stream.get(), "notify::tags", G_CALLBACK(+[](TrackPrivateBaseGStreamer* track) {
        track->tagsChanged();
    }), this);

    // We can't call notifyTrackOfTagsChanged() directly, because we need tagsChanged() to setup m_tags.
    tagsChanged();
}

void TrackPrivateBaseGStreamer::setPad(GRefPtr<GstPad>&& pad)
{
    if (m_bestUpstreamPad && m_eventProbe)
        gst_pad_remove_probe(m_bestUpstreamPad.get(), m_eventProbe);

    m_pad = WTFMove(pad);
    m_bestUpstreamPad = findBestUpstreamPad(m_pad);
    m_id = AtomString(trackIdFromPadStreamStartOrUniqueID(m_type, m_index, m_pad));

    if (!m_bestUpstreamPad)
        return;

    m_eventProbe = gst_pad_add_probe(m_bestUpstreamPad.get(), GST_PAD_PROBE_TYPE_EVENT_DOWNSTREAM, [] (GstPad*, GstPadProbeInfo* info, gpointer userData) -> GstPadProbeReturn {
        auto* track = static_cast<TrackPrivateBaseGStreamer*>(userData);
        switch (GST_EVENT_TYPE(gst_pad_probe_info_get_event(info))) {
        case GST_EVENT_TAG:
            track->tagsChanged();
            break;
        case GST_EVENT_STREAM_START:
            if (track->m_shouldHandleStreamStartEvent)
                track->streamChanged();
            break;
        default:
            break;
        }
        return GST_PAD_PROBE_OK;
    }, this, nullptr);
}

TrackPrivateBaseGStreamer::~TrackPrivateBaseGStreamer()
{
    disconnect();
    m_notifier->invalidate();
}

void TrackPrivateBaseGStreamer::disconnect()
{
    if (m_stream) {
        g_signal_handlers_disconnect_matched(m_stream.get(), G_SIGNAL_MATCH_DATA, 0, 0, nullptr, nullptr, this);
        ASSERT(!g_signal_handler_find(m_stream.get(), G_SIGNAL_MATCH_UNBLOCKED, 0, 0, nullptr, nullptr, nullptr));
    }

    m_tags.clear();

    m_notifier->cancelPendingNotifications();

    if (m_bestUpstreamPad && m_eventProbe) {
        gst_pad_remove_probe(m_bestUpstreamPad.get(), m_eventProbe);
        m_eventProbe = 0;
        m_bestUpstreamPad.clear();
    }

    if (m_pad) {
        g_signal_handlers_disconnect_matched(m_pad.get(), G_SIGNAL_MATCH_DATA, 0, 0, nullptr, nullptr, this);
        ASSERT(!g_signal_handler_find(m_pad.get(), G_SIGNAL_MATCH_UNBLOCKED, 0, 0, nullptr, nullptr, nullptr));
        m_pad.clear();
    }
}

void TrackPrivateBaseGStreamer::tagsChanged()
{
    GRefPtr<GstTagList> tags;
    if (m_bestUpstreamPad) {
        GRefPtr<GstEvent> tagEvent;
        guint i = 0;
        // Prefer the tag event having a language tag, if available.
        do {
            tagEvent = adoptGRef(gst_pad_get_sticky_event(m_bestUpstreamPad.get(), GST_EVENT_TAG, i));
            if (tagEvent) {
                GstTagList* tagsFromEvent = nullptr;
                gst_event_parse_tag(tagEvent.get(), &tagsFromEvent);
                tags = adoptGRef(gst_tag_list_copy(tagsFromEvent));
                String language;
                if (getTag(tags.get(), GST_TAG_LANGUAGE_CODE, language))
                    break;
            }
            i++;
        } while (tagEvent);
    } else if (m_stream)
        tags = adoptGRef(gst_stream_get_tags(m_stream.get()));

    if (!tags)
        tags = adoptGRef(gst_tag_list_new_empty());

    GST_DEBUG("Inspecting track at index %d with tags: %" GST_PTR_FORMAT, m_index, tags.get());
    {
        Locker locker { m_tagMutex };
        m_tags.swap(tags);
    }

    m_notifier->notify(MainThreadNotification::TagsChanged, [this] {
        notifyTrackOfTagsChanged();
    });
}

bool TrackPrivateBaseGStreamer::getLanguageCode(GstTagList* tags, AtomString& value)
{
    String language;
    if (getTag(tags, GST_TAG_LANGUAGE_CODE, language)) {
        AtomString convertedLanguage = AtomString::fromLatin1(gst_tag_get_language_code_iso_639_1(language.utf8().data()));
        GST_DEBUG("Converted track %d's language code to %s.", m_index, convertedLanguage.string().utf8().data());
        if (convertedLanguage != value) {
            value = WTFMove(convertedLanguage);
            return true;
        }
    }
    return false;
}

template<class StringType>
bool TrackPrivateBaseGStreamer::getTag(GstTagList* tags, const gchar* tagName, StringType& value)
{
    GUniqueOutPtr<gchar> tagValue;
    if (gst_tag_list_get_string(tags, tagName, &tagValue.outPtr())) {
        GST_DEBUG("Track %d got %s %s.", m_index, tagName, tagValue.get());
        value = StringType { String::fromLatin1(tagValue.get()) };
        return true;
    }
    return false;
}

void TrackPrivateBaseGStreamer::notifyTrackOfTagsChanged()
{
    TrackPrivateBaseClient* client = m_owner->client();

    GRefPtr<GstTagList> tags;
    {
        Locker locker { m_tagMutex };
        tags.swap(m_tags);
    }

    if (!tags)
        return;

    if (getTag(tags.get(), GST_TAG_TITLE, m_label) && client)
        client->labelChanged(m_label);

    AtomString language;
    if (!getLanguageCode(tags.get(), language))
        return;

    if (language == m_language)
        return;

    m_language = language;
    if (client)
        client->languageChanged(m_language);
}

void TrackPrivateBaseGStreamer::notifyTrackOfStreamChanged()
{
    if (!m_pad)
        return;

    GUniquePtr<char> streamId(gst_pad_get_stream_id(m_pad.get()));
    if (!streamId)
        return;

    GST_INFO("Track %d got stream start for stream %s.", m_index, streamId.get());
    m_id = AtomString::fromLatin1(streamId.get());
}

void TrackPrivateBaseGStreamer::streamChanged()
{
    m_notifier->notify(MainThreadNotification::StreamChanged, [this] {
        notifyTrackOfStreamChanged();
    });
}

void TrackPrivateBaseGStreamer::installUpdateConfigurationHandlers()
{
    if (m_pad) {
        g_signal_connect_swapped(m_pad.get(), "notify::caps", G_CALLBACK(+[](TrackPrivateBaseGStreamer* track) {
            track->m_taskQueue.enqueueTask([track]() {
                if (!track->m_pad)
                    return;
                auto caps = adoptGRef(gst_pad_get_current_caps(track->m_pad.get()));
                if (!caps)
                    return;
                track->updateConfigurationFromCaps();
            });
        }), this);
        g_signal_connect_swapped(m_pad.get(), "notify::tags", G_CALLBACK(+[](TrackPrivateBaseGStreamer* track) {
            track->m_taskQueue.enqueueTask([track]() {
                if (!track->m_pad)
                    return;
                track->updateConfigurationFromTags();
            });
        }), this);
    } else if (m_stream) {
        g_signal_connect_swapped(m_stream.get(), "notify::caps", G_CALLBACK(+[](TrackPrivateBaseGStreamer* track) {
            track->m_taskQueue.enqueueTask([track]() {
                track->updateConfigurationFromCaps();
            });
        }), this);
        g_signal_connect_swapped(m_stream.get(), "notify::tags", G_CALLBACK(+[](TrackPrivateBaseGStreamer* track) {
            if (isMainThread())
                track->updateConfigurationFromTags();
            else
                track->m_taskQueue.enqueueTask([track]() {
                    track->updateConfigurationFromTags();
                });
        }), this);
    }
}

String TrackPrivateBaseGStreamer::trackIdFromPadStreamStartOrUniqueID(TrackType type, unsigned index, const GRefPtr<GstPad>& pad)
{
    String streamId = nullString();
    if (!pad)
        return generateUniquePlaybin2StreamID(type, index);

    auto streamStart = adoptGRef(gst_pad_get_sticky_event(pad.get(), GST_EVENT_STREAM_START, 0));
    if (!streamStart)
        return generateUniquePlaybin2StreamID(type, index);

    const gchar* streamIdAsCharacters;
    gst_event_parse_stream_start(streamStart.get(), &streamIdAsCharacters);

    if (!streamIdAsCharacters)
        return generateUniquePlaybin2StreamID(type, index);

    StringView streamIdView = StringView::fromLatin1(streamIdAsCharacters);
    size_t position = streamIdView.find('/');
    if (position == notFound || position + 1 == streamIdView.length())
        return generateUniquePlaybin2StreamID(type, index);

    return streamIdView.substring(position + 1).toString();
}

GRefPtr<GstTagList> TrackPrivateBaseGStreamer::getAllTags(const GRefPtr<GstPad>& pad)
{
    auto allTags = adoptGRef(gst_tag_list_new_empty());
    GstTagList* taglist = nullptr;
    for (guint i = 0;; i++) {
        GRefPtr<GstEvent> tagsEvent = adoptGRef(gst_pad_get_sticky_event(pad.get(), GST_EVENT_TAG, i));
        if (!tagsEvent)
            break;
        gst_event_parse_tag(tagsEvent.get(), &taglist);
        allTags = adoptGRef(gst_tag_list_merge(allTags.get(), taglist, GST_TAG_MERGE_APPEND));
    }
    return allTags;
}

} // namespace WebCore

#endif // ENABLE(VIDEO) && USE(GSTREAMER)