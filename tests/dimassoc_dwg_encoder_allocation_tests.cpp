// SPDX-License-Identifier: GPL-2.0-or-later
// Generated DWG object encoder allocation rollback and retry controls.
#include "drw_objects.h"
#include "intern/dwgbufferw.h"
#include "intern/drw_textcodec.h"

#include <atomic>
#include <cstdlib>
#include <iostream>
#include <new>
#include <string>
#include <vector>

namespace {
std::atomic<long> allocationsBeforeFailure{-1};
std::atomic<unsigned long> failedAllocations{0};

class DimensionAssociationProbe final : public DRW_DimensionAssociation {
public:
    using DRW_DimensionAssociation::encodeDwg;
};

class EvaluationGraphProbe final : public DRW_EvaluationGraph {
public:
    using DRW_EvaluationGraph::encodeDwg;
};

DimensionAssociationProbe makeAssociation() {
    DimensionAssociationProbe association;
    association.handle = 0xA100u;
    association.parentHandle = 0x20u;
    association.reactorHandles = {0x21u};
    association.m_dimensionHandle = 0x30u;
    association.m_associativityFlags = 1u;
    association.m_isTransSpace = true;
    association.m_rotatedDimensionType = 4u;
    DRW_DimensionAssociationOsnapRef ref;
    ref.m_className = "AcDbOsnapPointRef";
    ref.m_objectOsnapType = 13u;
    ref.m_objectHandle = 0x40u;
    ref.m_objectHandles = {0x40u, 0x44u};
    ref.m_hasSubentType = true;
    ref.m_subentType = 2;
    ref.m_hasGsMarker = true;
    ref.m_gsMarker = 9;
    ref.m_hasXrefObjectHandleString = true;
    ref.m_xrefObjectHandleString = "nested/xref/path";
    ref.m_hasNearParameter = true;
    ref.m_nearParameter = 0.25;
    ref.m_hasOsnapPoint = true;
    ref.m_osnapPoint = DRW_Coord{1.25, -2.5, 3.75};
    ref.m_hasLastPointRef = true;
    ref.m_lastPointRef = 1;
    association.m_osnapRefs.push_back(std::move(ref));
    return association;
}

bool same(const dwgBufferW& lhs, const dwgBufferW& rhs) {
    if (lhs.data() != rhs.data() || lhs.bitPos() != rhs.bitPos()
        || lhs.isGood() != rhs.isGood()
        || lhs.handleOccurrences().size() != rhs.handleOccurrences().size())
        return false;
    for (std::size_t i = 0; i < lhs.handleOccurrences().size(); ++i) {
        const auto& left = lhs.handleOccurrences()[i];
        const auto& right = rhs.handleOccurrences()[i];
        if (left.code != right.code || left.reference != right.reference
            || left.startBit != right.startBit || left.endBit != right.endBit)
            return false;
    }
    return true;
}

void seed(dwgBufferW& body, dwgBufferW& strings, dwgBufferW& handles) {
    body.putRawChar8(0xA5u);
    body.putBit(1u);
    strings.putRawChar8(0x5Au);
    strings.putBit(0u);
    handles.putRawChar8(0xC3u);
    handles.putBit(1u);
}

enum class StreamAlias {
    Separate,
    SharedStringAndHandle,
    AllBody
};

bool sweep(StreamAlias alias) {
    DRW_TextCodec codec;
    codec.setVersion(DRW::AC1024, false);
    const DimensionAssociationProbe association = makeAssociation();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* const cleanStringOutput = alias == StreamAlias::AllBody
        ? nullptr : &cleanStrings;
    dwgBufferW* const cleanHandleOutput = alias == StreamAlias::AllBody
        ? nullptr : alias == StreamAlias::SharedStringAndHandle
            ? &cleanStrings : &cleanHandles;
    if (!association.encodeDwg(DRW::AC1024, &cleanBody, cleanStringOutput,
                               cleanHandleOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* const stringOutput = alias == StreamAlias::AllBody
        ? nullptr : &strings;
    dwgBufferW* const handleOutput = alias == StreamAlias::AllBody
        ? nullptr : alias == StreamAlias::SharedStringAndHandle
            ? &strings : &handles;

    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = association.encodeDwg(
        DRW::AC1024, &body, stringOutput, handleOutput);
    const long allocations = 1000000
        - allocationsBeforeFailure.load(std::memory_order_relaxed);
    allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
    if (!calibrated || allocations <= 0
        || failedAllocations.load(std::memory_order_relaxed)
            != beforeCalibration)
        return false;

    for (long offset = 0; offset < allocations; ++offset) {
        dwgBufferW failedBody(&codec), failedStrings(&codec), failedHandles(&codec);
        seed(failedBody, failedStrings, failedHandles);
        dwgBufferW* const failedStringOutput = alias == StreamAlias::AllBody
            ? nullptr : &failedStrings;
        dwgBufferW* const failedHandleOutput = alias == StreamAlias::AllBody
            ? nullptr : alias == StreamAlias::SharedStringAndHandle
                ? &failedStrings : &failedHandles;
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = association.encodeDwg(
            DRW::AC1024, &failedBody, failedStringOutput,
            failedHandleOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!association.encodeDwg(DRW::AC1024, &failedBody,
                failedStringOutput, failedHandleOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    const char* aliasName = alias == StreamAlias::AllBody ? "all-body"
        : alias == StreamAlias::SharedStringAndHandle
            ? "shared-string-handle" : "separate";
    std::cout << "DIMASSOC DWG encoder " << aliasName
              << " streams: allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

EvaluationGraphProbe makeEvaluationGraph() {
    EvaluationGraphProbe graph;
    graph.handle = 0xB100u;
    graph.parentHandle = 0x20u;
    graph.reactorHandles = {0x21u, 0x22u};
    graph.xDictHandle = 0x23u;
    graph.m_value96 = 17;
    graph.m_value97 = 29;
    graph.m_nodes = {
        {1, 3, 2, 0x40u, 5, 7, 11, 13},
        {2, 9, 0, 0x44u, 17, 19, 23, 31}
    };
    graph.m_edges = {
        {1, 2, 3, 4, 5, 6, 7, 8, 9, 10},
        {11, 12, 13, 14, 15, 16, 17, 18, 19, 20}
    };
    return graph;
}

bool sweepEvaluationGraph(bool aliasHandles) {
    DRW_TextCodec codec;
    codec.setVersion(DRW::AC1021, false);
    const EvaluationGraphProbe graph = makeEvaluationGraph();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* const cleanHandleOutput = aliasHandles ? nullptr : &cleanHandles;
    if (!graph.encodeDwg(DRW::AC1021, &cleanBody, &cleanStrings,
                         cleanHandleOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* const handleOutput = aliasHandles ? nullptr : &handles;
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = graph.encodeDwg(
        DRW::AC1021, &body, &strings, handleOutput);
    const long allocations = 1000000
        - allocationsBeforeFailure.load(std::memory_order_relaxed);
    allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
    if (!calibrated || allocations <= 0
        || failedAllocations.load(std::memory_order_relaxed)
            != beforeCalibration)
        return false;

    for (long offset = 0; offset < allocations; ++offset) {
        dwgBufferW failedBody(&codec), failedStrings(&codec), failedHandles(&codec);
        seed(failedBody, failedStrings, failedHandles);
        dwgBufferW* const failedHandleOutput = aliasHandles
            ? nullptr : &failedHandles;
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = graph.encodeDwg(
            DRW::AC1021, &failedBody, &failedStrings, failedHandleOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!graph.encodeDwg(DRW::AC1021, &failedBody, &failedStrings,
                             failedHandleOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "EvaluationGraph DWG encoder "
              << (aliasHandles ? "body-aliased" : "separate")
              << " handles: allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}
} // namespace

void* operator new(std::size_t size) {
    long remaining = allocationsBeforeFailure.load(std::memory_order_relaxed);
    if (remaining == 0) {
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        failedAllocations.fetch_add(1, std::memory_order_relaxed);
        throw std::bad_alloc();
    }
    if (remaining > 0)
        allocationsBeforeFailure.fetch_sub(1, std::memory_order_relaxed);
    if (void* memory = std::malloc(size == 0 ? 1 : size))
        return memory;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory, std::size_t) noexcept { std::free(memory); }
void operator delete[](void* memory, std::size_t) noexcept { std::free(memory); }

int main() {
    if (!sweep(StreamAlias::Separate)
        || !sweep(StreamAlias::SharedStringAndHandle)
        || !sweep(StreamAlias::AllBody)
        || !sweepEvaluationGraph(false)
        || !sweepEvaluationGraph(true)) {
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        std::cerr << "DIMASSOC DWG encoder allocation transaction failed\n";
        return 1;
    }
    if (failedAllocations.load(std::memory_order_relaxed) == 0u) {
        std::cerr << "allocation fault injector did not reach operator new\n";
        return 1;
    }
    return 0;
}
