/*
 * Copyright (C) 2020 Igalia S.L
 * Copyright (C) 2020 Metrological Group B.V.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "NicosiaImageBufferPipe.h"

#include "ImageBuffer.h"
#include "NativeImage.h"
#include "NicosiaPlatformLayer.h"
#include "TextureMapperPlatformLayerBuffer.h"
#include "TextureMapperPlatformLayerProxyGL.h"

#if USE(CAIRO)
#include <cairo.h>
#endif

#if USE(NICOSIA)

namespace Nicosia {

using namespace WebCore;

NicosiaImageBufferPipeSource::NicosiaImageBufferPipeSource()
{
    m_nicosiaLayer = Nicosia::ContentLayer::create(Nicosia::ContentLayerTextureMapperImpl::createFactory(*this));
}

NicosiaImageBufferPipeSource::~NicosiaImageBufferPipeSource()
{
    downcast<Nicosia::ContentLayerTextureMapperImpl>(m_nicosiaLayer->impl()).invalidateClient();
}

void NicosiaImageBufferPipeSource::handle(RefPtr<ImageBuffer>&& buffer)
{
    if (!buffer)
        return;

    Locker locker { m_imageBufferLock };

    if (!m_imageBuffer) {
        auto proxyOperation = [this] (TextureMapperPlatformLayerProxy& proxy) mutable {
            return downcast<TextureMapperPlatformLayerProxyGL>(proxy).scheduleUpdateOnCompositorThread([this] () mutable {
                auto& proxy = downcast<Nicosia::ContentLayerTextureMapperImpl>(m_nicosiaLayer->impl()).proxy();
                Locker locker { proxy.lock() };

                if (!proxy.isActive())
                    return;

                auto texture = BitmapTextureGL::create(TextureMapperContextAttributes::get());

                {
                    Locker locker { m_imageBufferLock };

                    if (!m_imageBuffer)
                        return;

                    auto nativeImage = ImageBuffer::sinkIntoNativeImage(WTFMove(m_imageBuffer));
                    if (!nativeImage)
                        return;

                    auto size = nativeImage->size();

                    texture->reset(size, nativeImage->hasAlpha() ? BitmapTexture::SupportsAlpha : BitmapTexture::NoFlag);
#if USE(CAIRO)
                    auto* surface = nativeImage->platformImage().get();
                    auto* imageData = cairo_image_surface_get_data(surface);
                    texture->updateContents(imageData, IntRect(IntPoint(), size), IntPoint(), cairo_image_surface_get_stride(surface));
#else
                    notImplemented();
#endif
                }

                auto layerBuffer = makeUnique<TextureMapperPlatformLayerBuffer>(WTFMove(texture));
                layerBuffer->setExtraFlags(TextureMapperGL::ShouldBlend);
                downcast<TextureMapperPlatformLayerProxyGL>(proxy).pushNextBuffer(WTFMove(layerBuffer));
            });
        };
        proxyOperation(downcast<Nicosia::ContentLayerTextureMapperImpl>(m_nicosiaLayer->impl()).proxy());
    }

    m_imageBuffer = WTFMove(buffer);
}

void NicosiaImageBufferPipeSource::swapBuffers()
{
    if (!m_context)
        return;

    if (m_context->layerComposited())
        return;

    m_context->prepareTexture();
    IntSize textureSize(m_context->m_currentWidth, m_context->m_currentHeight);

    TextureMapperGL::Flags flags = TextureMapperGL::ShouldFlipTexture;
    if (m_context->contextAttributes().alpha)
        flags |= TextureMapperGL::ShouldBlend;

    {
        auto& proxy = downcast<Nicosia::ContentLayerTextureMapperImpl>(m_nicosiaLayer->impl()).proxy();
        Locker locker { proxy.lock() };
        ASSERT(is<TextureMapperPlatformLayerProxyGL>(proxy));
        downcast<TextureMapperPlatformLayerProxyGL>(proxy).pushNextBuffer(makeUnique<TextureMapperPlatformLayerBuffer>(m_context->m_compositorTexture, textureSize, flags, m_context->m_internalColorFormat));
    }

    m_context->markLayerComposited();
}

void NicosiaImageBufferPipeSource::setGraphicsContextGL(GraphicsContextGL* context)
{
    m_context = static_cast<GraphicsContextGLOpenGL*>(context);
}

void NicosiaImageBufferPipeSource::swapBuffersIfNeeded()
{
}

NicosiaImageBufferPipe::NicosiaImageBufferPipe()
    : m_source(adoptRef(*new NicosiaImageBufferPipeSource))
    , m_layerContentsDisplayDelegate(NicosiaImageBufferPipeSourceDisplayDelegate::create(m_source->platformLayer()))
{
}

RefPtr<ImageBufferPipe::Source> NicosiaImageBufferPipe::source() const
{
    return m_source.ptr();
}

RefPtr<GraphicsLayerContentsDisplayDelegate> NicosiaImageBufferPipe::layerContentsDisplayDelegate()
{
    return m_layerContentsDisplayDelegate.ptr();
}

} // namespace Nicosia

namespace WebCore {

RefPtr<ImageBufferPipe> ImageBufferPipe::create()
{
    return adoptRef(new Nicosia::NicosiaImageBufferPipe);
}

}

#endif // USE(NICOSIA)
