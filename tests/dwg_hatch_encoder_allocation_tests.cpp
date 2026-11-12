// SPDX-License-Identifier: GPL-2.0-or-later
// Generated HATCH DWG encoder allocation rollback and retry controls.
#include "drw_entities.h"
#include "intern/dwgbufferw.h"

#include <atomic>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <new>
#include <vector>

namespace {
std::atomic<long> allocationsBeforeFailure{-1};
std::atomic<unsigned long> failedAllocations{0};

bool failAllocation() noexcept {
    const long remaining = allocationsBeforeFailure.load(
        std::memory_order_relaxed);
    if (remaining < 0)
        return false;
    if (remaining == 0) {
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        failedAllocations.fetch_add(1u, std::memory_order_relaxed);
        return true;
    }
    allocationsBeforeFailure.fetch_sub(1, std::memory_order_relaxed);
    return false;
}

class HatchProbe final : public DRW_Hatch {
public:
    using DRW_Hatch::encodeDwg;
    std::int16_t rawType() const { return oType; }
};

struct Buffers {
    dwgBufferW body;
    dwgBufferW strings;
    dwgBufferW handles;
};

enum class AliasLayout {
    Separate,
    SharedStringAndHandle,
    BodyAndString,
    BodyAndHandles,
    AllBody
};

void seed(Buffers& buffers) {
    buffers.body.putRawChar8(0xa5u);
    buffers.body.putBit(1u);
    buffers.strings.putRawChar8(0x5au);
    buffers.strings.putBit(0u);
    buffers.handles.putRawChar8(0x3cu);
    buffers.handles.putBit(1u);
}

void route(AliasLayout layout, Buffers& buffers,
           dwgBufferW*& strings, dwgBufferW*& handles) {
    strings = &buffers.strings;
    handles = &buffers.handles;
    switch (layout) {
    case AliasLayout::Separate:
        break;
    case AliasLayout::SharedStringAndHandle:
        handles = strings;
        break;
    case AliasLayout::BodyAndString:
        strings = &buffers.body;
        break;
    case AliasLayout::BodyAndHandles:
        handles = &buffers.body;
        break;
    case AliasLayout::AllBody:
        strings = &buffers.body;
        handles = &buffers.body;
        break;
    }
}

HatchProbe makeHatch() {
    HatchProbe hatch;
    hatch.handle = 0x40u;
    hatch.name = "ANSI31";
    hatch.solid = false;
    hatch.associative = true;
    hatch.angle = 17.0;
    hatch.scale = 0.75;
    hatch.pixelSize = 2.5;
    hatch.seedPoints = {{1.0, 2.0, 0.0}, {3.0, 4.0, 0.0}};

    DRW_Hatch::PatternLine pattern;
    pattern.angle = 30.0;
    pattern.baseX = 0.25;
    pattern.baseY = 0.5;
    pattern.offsetX = 0.75;
    pattern.offsetY = 1.0;
    pattern.dashList = {1.0, -0.25, 0.5};
    hatch.patternLines.push_back(pattern);

    auto loop = std::make_shared<DRW_HatchLoop>(4);
    auto line = std::make_shared<DRW_Line>();
    line->basePoint = {0.0, 0.0, 0.0};
    line->secPoint = {10.0, 0.0, 0.0};
    loop->objlist.push_back(line);
    loop->m_boundaryHandles = {0x60u};
    loop->update();
    hatch.appendLoop(loop);
    return hatch;
}

bool sameBuffers(const Buffers& left, const Buffers& right) {
    return left.body.data() == right.body.data()
        && left.body.bitPos() == right.body.bitPos()
        && left.strings.data() == right.strings.data()
        && left.strings.bitPos() == right.strings.bitPos()
        && left.handles.data() == right.handles.data()
        && left.handles.bitPos() == right.handles.bitPos();
}

bool encode(DRW::Version version, HatchProbe& hatch, AliasLayout layout,
            Buffers& buffers) {
    dwgBufferW* strings = nullptr;
    dwgBufferW* handles = nullptr;
    route(layout, buffers, strings, handles);
    return hatch.encodeDwg(version, &buffers.body, 0u, strings, handles);
}

bool sweep(DRW::Version version, AliasLayout layout,
           unsigned long& reachedFailures) {
    HatchProbe clean = makeHatch();
    Buffers cleanBuffers;
    seed(cleanBuffers);
    if (!encode(version, clean, layout, cleanBuffers)
        || clean.rawType() != 78) {
        std::cerr << "clean HATCH encoding failed for version "
                  << static_cast<int>(version) << '\n';
        return false;
    }

    bool reachedSuccess = false;
    for (long failIndex = 0; failIndex < 128; ++failIndex) {
        HatchProbe hatch = makeHatch();
        Buffers buffers;
        seed(buffers);
        const Buffers before = buffers;
        const auto oldType = hatch.rawType();
        const auto oldHandle = hatch.handle;
        const auto failuresBefore = failedAllocations.load(
            std::memory_order_relaxed);

        allocationsBeforeFailure.store(failIndex,
                                       std::memory_order_relaxed);
        const bool encoded = encode(version, hatch, layout, buffers);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);

        if (encoded) {
            if (!sameBuffers(buffers, cleanBuffers)
                || hatch.rawType() != clean.rawType()
                || hatch.handle != clean.handle) {
                std::cerr << "HATCH output differs after allocation sweep\n";
                return false;
            }
            reachedFailures = static_cast<unsigned long>(failIndex);
            reachedSuccess = true;
            break;
        }

        if (failedAllocations.load(std::memory_order_relaxed)
                == failuresBefore
            || !sameBuffers(buffers, before)
            || hatch.rawType() != oldType || hatch.handle != oldHandle) {
            std::cerr << "HATCH allocation failure did not roll back\n";
            return false;
        }

        // Same object, same buffers, corrected retry must reproduce clean bytes.
        if (!encode(version, hatch, layout, buffers)
            || !sameBuffers(buffers, cleanBuffers)
            || hatch.rawType() != clean.rawType()
            || hatch.handle != clean.handle) {
            std::cerr << "HATCH same-object retry failed\n";
            return false;
        }
    }
    if (!reachedSuccess) {
        std::cerr << "HATCH allocation sweep did not reach clean success\n";
        return false;
    }
    return true;
}
} // namespace

void* operator new(std::size_t size) {
    if (failAllocation())
        throw std::bad_alloc();
    if (void* memory = std::malloc(size == 0u ? 1u : size))
        return memory;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) {
    if (failAllocation())
        throw std::bad_alloc();
    if (void* memory = std::malloc(size == 0u ? 1u : size))
        return memory;
    throw std::bad_alloc();
}

void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory, std::size_t) noexcept { std::free(memory); }
void operator delete[](void* memory, std::size_t) noexcept { std::free(memory); }

int main() {
    const DRW::Version versions[] = {DRW::AC1015, DRW::AC1018,
        DRW::AC1021, DRW::AC1024, DRW::AC1027, DRW::AC1032};
    const AliasLayout layouts[] = {AliasLayout::Separate,
        AliasLayout::SharedStringAndHandle, AliasLayout::BodyAndString,
        AliasLayout::BodyAndHandles, AliasLayout::AllBody};
    unsigned long totalReachedFailures = 0u;
    for (const auto version : versions) {
        for (const auto layout : layouts) {
            unsigned long reachedFailures = 0u;
            if (!sweep(version, layout, reachedFailures))
                return 1;
            totalReachedFailures += reachedFailures;
        }
    }
    if (failedAllocations.load(std::memory_order_relaxed) == 0u
        || totalReachedFailures == 0u) {
        std::cerr << "HATCH allocation injector did not fire\n";
        return 1;
    }
    std::cout << "HATCH DWG encode allocation rollback/retry passed across "
              << sizeof(versions) / sizeof(versions[0]) << " versions and "
              << sizeof(layouts) / sizeof(layouts[0])
                     * sizeof(versions) / sizeof(versions[0])
              << " version/layout profiles; " << totalReachedFailures
              << " allocation offsets exercised\n";
    return 0;
}
