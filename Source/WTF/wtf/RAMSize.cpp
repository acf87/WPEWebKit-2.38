/*
 * Copyright (C) 2012-2019 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include <wtf/RAMSize.h>
#include <wtf/text/StringToIntegerConversion.h>
#include <wtf/text/WTFString.h>

#include <mutex>

#if OS(WINDOWS)
#include <windows.h>
#elif USE(SYSTEM_MALLOC)
#if OS(LINUX) || OS(FREEBSD)
#include <sys/sysinfo.h>
#elif OS(UNIX)
#include <unistd.h>
#endif // OS(LINUX) || OS(FREEBSD) || OS(UNIX)
#else
#include <bmalloc/bmalloc.h>
#endif

namespace WTF {

#if OS(WINDOWS)
static constexpr size_t ramSizeGuess = 512 * MB;
#endif

static size_t customRAMSize()
{
    // Syntax: Case insensitive, unit multipliers (M=Mb, K=Kb, <empty>=bytes).
    // Example: WPE_RAM_SIZE='500M'

    size_t customSize = 0;

    String s = String::fromLatin1(getenv("WPE_RAM_SIZE"));
    if (!s.isEmpty()) {
        String value = s.stripWhiteSpace().convertToLowercaseWithoutLocale();
        size_t units = 1;
        if (value.endsWith('k'))
            units = KB;
        else if (value.endsWith('m'))
            units = MB;
        if (units != 1)
            value = value.substring(0, value.length() - 1);
        customSize = parseInteger<uint64_t>(value).value_or(0) * units;
    }

    return customSize;
}

static size_t computeRAMSize()
{
    size_t custom = customRAMSize();
    if (custom)
        return custom;

#if OS(WINDOWS)
    MEMORYSTATUSEX status;
    status.dwLength = sizeof(status);
    bool result = GlobalMemoryStatusEx(&status);
    if (!result)
        return ramSizeGuess;
    return status.ullTotalPhys;
#elif USE(SYSTEM_MALLOC)
#if OS(LINUX) || OS(FREEBSD)
    struct sysinfo si;
    sysinfo(&si);
    return si.totalram * si.mem_unit;
#elif OS(UNIX)
    long pages = sysconf(_SC_PHYS_PAGES);
    long pageSize = sysconf(_SC_PAGE_SIZE);
    return pages * pageSize;
#else
#error "Missing a platform specific way of determining the available RAM"
#endif // OS(LINUX) || OS(FREEBSD) || OS(UNIX)
#else
    return bmalloc::api::availableMemory();
#endif
}

size_t ramSize()
{
    static size_t ramSize;
    static std::once_flag onceFlag;
    std::call_once(onceFlag, [] {
        ramSize = computeRAMSize();
    });
    return ramSize;
}

} // namespace WTF
