/*
 * Copyright (C) 2017 Igalia S.L.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#pragma once

#if ENABLE(REMOTE_INSPECTOR)

#include <WebCore/Timer.h>
#include <wtf/HashMap.h>
#include <wtf/Vector.h>
#include <wtf/glib/GRefPtr.h>
#include <wtf/glib/SocketConnection.h>
#include <wtf/text/WTFString.h>

typedef struct _GCancellable GCancellable;

namespace WebKit {

class RemoteInspectorClient;
class RemoteInspectorProxy;
class RemoteWebInspectorUIProxy;

class RemoteInspectorObserver {
public:
    virtual ~RemoteInspectorObserver() { }
    virtual void targetListChanged(RemoteInspectorClient&) = 0;
    virtual void connectionClosed(RemoteInspectorClient&) = 0;
};

class RemoteInspectorClient {
    WTF_MAKE_FAST_ALLOCATED();
public:
    RemoteInspectorClient(String&& hostAndPort, RemoteInspectorObserver&);
    ~RemoteInspectorClient();

    const String& hostAndPort() const { return m_hostAndPort; }
    const String& backendCommandsURL() const { return m_backendCommandsURL; }

    enum class InspectorType { UI, HTTP };
    GString* buildTargetListPage(InspectorType) const;
    enum class ShouldEscapeSingleQuote : bool { No, Yes };
    void appendTargertList(GString*, InspectorType, ShouldEscapeSingleQuote) const;
    void inspect(uint64_t connectionID, uint64_t targetID, const String& targetType, InspectorType = InspectorType::UI);
    void sendMessageToBackend(uint64_t connectionID, uint64_t targetID, const String&);
    void closeFromFrontend(uint64_t connectionID, uint64_t targetID);

private:
    static const SocketConnection::MessageHandlers& messageHandlers();
    void setupConnection(Ref<SocketConnection>&&);
    void connectionDidClose();

    struct Target {
        uint64_t id;
        CString type;
        CString name;
        CString url;
    };

    void setBackendCommands(const char*);
    void setTargetList(uint64_t connectionID, Vector<Target>&&);
    void sendMessageToFrontend(uint64_t connectionID, uint64_t targetID, const char*);
    void setupInspectorClientTimerFired();

    String m_hostAndPort;
    String m_backendCommandsURL;
    RemoteInspectorObserver& m_observer;
    RefPtr<SocketConnection> m_socketConnection;
    GRefPtr<GCancellable> m_cancellable;
    HashMap<uint64_t, Vector<Target>> m_targets;
    HashMap<std::pair<uint64_t, uint64_t>, std::unique_ptr<RemoteInspectorProxy>> m_inspectorProxyMap;
    WebCore::Timer m_setupInspectorClientTimer;
};

} // namespace WebKit

#endif // ENABLE(REMOTE_INSPECTOR)