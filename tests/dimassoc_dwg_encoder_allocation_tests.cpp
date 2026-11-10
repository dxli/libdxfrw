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
#include <utility>
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

class BlockRepresentationProbe final : public DRW_BlockRepresentationData {
public:
    using DRW_BlockRepresentationData::encodeDwg;
};

class ImageDefinitionReactorProbe final : public DRW_ImageDefinitionReactor {
public:
    using DRW_ImageDefinitionReactor::encodeDwg;
};

class ImageDefinitionProbe final : public DRW_ImageDef {
public:
    using DRW_ImageDef::encodeDwg;
};

class PlotSettingsProbe final : public DRW_PlotSettings {
public:
    using DRW_PlotSettings::encodeDwg;
};

class LayoutProbe final : public DRW_Layout {
public:
    using DRW_Layout::encodeDwg;
};

class DictionaryProbe final : public DRW_Dictionary {
public:
    using DRW_Dictionary::encodeDwg;
};

class DictionaryVarProbe final : public DRW_DictionaryVar {
public:
    using DRW_DictionaryVar::encodeDwg;
};

class DictionaryWithDefaultProbe final : public DRW_DictionaryWithDefault {
public:
    using DRW_DictionaryWithDefault::encodeDwg;
};

class XRecordProbe final : public DRW_XRecord {
public:
    using DRW_XRecord::encodeDwg;
};

class ScaleProbe final : public DRW_Scale {
public:
    using DRW_Scale::encodeDwg;
};

class GroupProbe final : public DRW_Group {
public:
    using DRW_Group::encodeDwg;
};

class RasterVariablesProbe final : public DRW_RasterVariables {
public:
    using DRW_RasterVariables::encodeDwg;
};

class WipeoutVariablesProbe final : public DRW_WipeoutVariables {
public:
    using DRW_WipeoutVariables::encodeDwg;
};

class SortEntsTableProbe final : public DRW_SortEntsTable {
public:
    using DRW_SortEntsTable::encodeDwg;
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

RasterVariablesProbe makeRasterVariables() {
    RasterVariablesProbe variables;
    variables.handle = 0xA500u;
    variables.parentHandle = 0x20u;
    variables.reactorHandles = {0x21u, 0x22u};
    variables.setDwgCommonObjectState(2, 1u, false);
    variables.xDictHandle = 0x23u;
    variables.m_classVersion = 1;
    variables.m_imageFrame = 1;
    variables.m_imageQuality = 0;
    variables.m_units = 8;
    return variables;
}

bool sweepRasterVariables(DRW::Version version, bool aliasHandles) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const RasterVariablesProbe variables = makeRasterVariables();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* const cleanHandleOutput = aliasHandles ? &cleanBody
                                                       : &cleanHandles;
    if (!variables.encodeDwg(version, &cleanBody, &cleanStrings,
                             cleanHandleOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* const handleOutput = aliasHandles ? &body : &handles;
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = variables.encodeDwg(version, &body, &strings,
                                                handleOutput);
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
        dwgBufferW* const failedHandleOutput = aliasHandles ? &failedBody
                                                            : &failedHandles;
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = variables.encodeDwg(
            version, &failedBody, &failedStrings, failedHandleOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!variables.encodeDwg(version, &failedBody, &failedStrings,
                                 failedHandleOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "RASTERVARIABLES DWG encoder version="
              << static_cast<int>(version)
              << (aliasHandles ? " body-aliased" : " separate-handles")
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

WipeoutVariablesProbe makeWipeoutVariables() {
    WipeoutVariablesProbe variables;
    variables.handle = 0xA600u;
    variables.parentHandle = 0x20u;
    variables.reactorHandles = {0x21u, 0x22u};
    variables.setDwgCommonObjectState(2, 1u, false);
    variables.xDictHandle = 0x23u;
    variables.m_classVersion = 0;
    variables.m_displayFrame = 1u;
    return variables;
}

bool sweepWipeoutVariables(DRW::Version version, bool aliasHandles) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const WipeoutVariablesProbe variables = makeWipeoutVariables();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* const cleanHandleOutput = aliasHandles ? &cleanBody
                                                       : &cleanHandles;
    if (!variables.encodeDwg(version, &cleanBody, &cleanStrings,
                             cleanHandleOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* const handleOutput = aliasHandles ? &body : &handles;
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = variables.encodeDwg(version, &body, &strings,
                                                handleOutput);
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
        dwgBufferW* const failedHandleOutput = aliasHandles ? &failedBody
                                                            : &failedHandles;
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = variables.encodeDwg(
            version, &failedBody, &failedStrings, failedHandleOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!variables.encodeDwg(version, &failedBody, &failedStrings,
                                 failedHandleOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "WIPEOUTVARIABLES DWG encoder version="
              << static_cast<int>(version)
              << (aliasHandles ? " body-aliased" : " separate-handles")
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

SortEntsTableProbe makeSortEntsTable() {
    SortEntsTableProbe table;
    table.handle = 0xA700u;
    table.parentHandle = 0x20u;
    table.reactorHandles = {0x21u, 0x22u};
    table.setDwgCommonObjectState(2, 1u, false);
    table.xDictHandle = 0x23u;
    table.m_blockOwnerHandle = 0x30u;
    table.m_sortHandles = {0x101u, 0x102u, 0x103u};
    table.m_entityHandles = {0x201u, 0x202u, 0x203u};
    return table;
}

bool sweepSortEntsTable(DRW::Version version, bool aliasHandles) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const SortEntsTableProbe table = makeSortEntsTable();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* const cleanHandleOutput = aliasHandles ? &cleanBody
                                                       : &cleanHandles;
    if (!table.encodeDwg(version, &cleanBody, &cleanStrings,
                         cleanHandleOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* const handleOutput = aliasHandles ? &body : &handles;
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = table.encodeDwg(version, &body, &strings,
                                            handleOutput);
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
        dwgBufferW* const failedHandleOutput = aliasHandles ? &failedBody
                                                            : &failedHandles;
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = table.encodeDwg(
            version, &failedBody, &failedStrings, failedHandleOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!table.encodeDwg(version, &failedBody, &failedStrings,
                             failedHandleOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "SORTENTSTABLE DWG encoder version="
              << static_cast<int>(version)
              << (aliasHandles ? " body-aliased" : " separate-handles")
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
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

BlockRepresentationProbe makeBlockRepresentation() {
    BlockRepresentationProbe record;
    record.handle = 0xC100u;
    record.parentHandle = 0x20u;
    record.reactorHandles = {0x21u};
    record.xDictHandle = 0x22u;
    record.m_flag = 3u;
    record.m_blockHandle = 0x40u;
    return record;
}

bool sweepBlockRepresentation(bool aliasHandles) {
    DRW_TextCodec codec;
    codec.setVersion(DRW::AC1024, false);
    const BlockRepresentationProbe record = makeBlockRepresentation();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* const cleanHandleOutput = aliasHandles ? nullptr : &cleanHandles;
    if (!record.encodeDwg(DRW::AC1024, &cleanBody, &cleanStrings,
                          cleanHandleOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* const handleOutput = aliasHandles ? nullptr : &handles;
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = record.encodeDwg(
        DRW::AC1024, &body, &strings, handleOutput);
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
        const bool encoded = record.encodeDwg(
            DRW::AC1024, &failedBody, &failedStrings, failedHandleOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!record.encodeDwg(DRW::AC1024, &failedBody, &failedStrings,
                              failedHandleOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "BlockRepresentationData DWG encoder "
              << (aliasHandles ? "body-aliased" : "separate")
              << " handles: allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

ImageDefinitionReactorProbe makeImageDefinitionReactor() {
    ImageDefinitionReactorProbe record;
    record.handle = 0xD100u;
    record.parentHandle = 0x20u;
    record.reactorHandles = {0x21u, 0x22u};
    record.xDictHandle = 0x23u;
    record.m_classVersion = 2;
    return record;
}

bool sweepImageDefinitionReactor(DRW::Version version, bool aliasHandles) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const ImageDefinitionReactorProbe record = makeImageDefinitionReactor();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* const cleanHandleOutput = aliasHandles ? nullptr : &cleanHandles;
    if (!record.encodeDwg(version, &cleanBody, &cleanStrings,
                          cleanHandleOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* const handleOutput = aliasHandles ? nullptr : &handles;
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = record.encodeDwg(
        version, &body, &strings, handleOutput);
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
        const bool encoded = record.encodeDwg(
            version, &failedBody, &failedStrings, failedHandleOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!record.encodeDwg(version, &failedBody, &failedStrings,
                              failedHandleOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "IMAGEDEF_REACTOR DWG encoder version="
              << static_cast<int>(version) << ' '
              << (aliasHandles ? "body-aliased" : "separate")
              << " handles: allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

ImageDefinitionProbe makeImageDefinition() {
    ImageDefinitionProbe definition;
    definition.handle = 0xE100u;
    definition.parentHandle = 0x20u;
    definition.reactorHandles = {0x21u, 0x22u};
    definition.xDictHandle = 0x23u;
    definition.setDwgCommonObjectState(2, 1u, false);
    definition.imgVersion = 2;
    definition.name = "images/transaction-probe.png";
    definition.u = 640.0;
    definition.v = 480.0;
    definition.loaded = 1;
    definition.resolution = 5;
    definition.up = 0.125;
    definition.vp = 0.125;
    return definition;
}

enum class ImageDefinitionStreams {
    R13InlineSeparateArguments,
    R2004DetachedHandles,
    Separate,
    SharedStringAndHandle,
    AllBody,
    BodyStringDetachedHandles,
    SeparateStringBodyHandles
};

bool sweepImageDefinition(ImageDefinitionStreams mode) {
    const DRW::Version version = mode == ImageDefinitionStreams::R13InlineSeparateArguments
        ? DRW::AC1012
        : mode == ImageDefinitionStreams::R2004DetachedHandles
            ? DRW::AC1018 : DRW::AC1021;
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const ImageDefinitionProbe definition = makeImageDefinition();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    const auto outputs = [mode](dwgBufferW& body, dwgBufferW& strings,
                                dwgBufferW& handles) {
        dwgBufferW* stringOutput = nullptr;
        dwgBufferW* handleOutput = nullptr;
        switch (mode) {
        case ImageDefinitionStreams::R13InlineSeparateArguments:
        case ImageDefinitionStreams::R2004DetachedHandles:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::Separate:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SharedStringAndHandle:
            stringOutput = &strings;
            handleOutput = &strings;
            break;
        case ImageDefinitionStreams::AllBody:
            break;
        case ImageDefinitionStreams::BodyStringDetachedHandles:
            stringOutput = &body;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SeparateStringBodyHandles:
            stringOutput = &strings;
            handleOutput = &body;
            break;
        }
        return std::pair<dwgBufferW*, dwgBufferW*>(stringOutput, handleOutput);
    };
    const auto cleanOutputs = outputs(cleanBody, cleanStrings, cleanHandles);
    if (!definition.encodeDwg(version, &cleanBody, cleanOutputs.first,
                              cleanOutputs.second))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    const auto output = outputs(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = definition.encodeDwg(
        version, &body, output.first, output.second);
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
        const auto failedOutput = outputs(failedBody, failedStrings, failedHandles);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = definition.encodeDwg(
            version, &failedBody, failedOutput.first, failedOutput.second);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!definition.encodeDwg(version, &failedBody, failedOutput.first,
                                  failedOutput.second)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    static const char* const names[] = {
        "AC1012-inline", "AC1018-detached-handles", "separate",
        "shared-string-handle", "all-body", "body-string-detached-handles",
        "separate-string-body-handles"
    };
    std::cout << "IMAGEDEF DWG encoder version=" << static_cast<int>(version)
              << ' ' << names[static_cast<std::size_t>(mode)]
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

PlotSettingsProbe makePlotSettings() {
    PlotSettingsProbe settings;
    settings.handle = 0xF100u;
    settings.parentHandle = 0x20u;
    settings.reactorHandles = {0x21u, 0x22u};
    settings.xDictHandle = 0x23u;
    settings.setDwgCommonObjectState(2, 1u, false);
    settings.pageSetupName = "layout setup";
    settings.printerConfig = "printer driver.pc3";
    settings.plotLayoutFlags = 3;
    settings.marginLeft = 0.25;
    settings.marginBottom = 0.5;
    settings.marginRight = 0.75;
    settings.marginTop = 1.0;
    settings.paperWidth = 210.0;
    settings.paperHeight = 297.0;
    settings.paperSize = "ISO A4";
    settings.plotOriginX = 1.5;
    settings.plotOriginY = -2.5;
    settings.paperUnits = 1;
    settings.plotRotation = 2;
    settings.plotType = 5;
    settings.windowMinX = -10.0;
    settings.windowMinY = -20.0;
    settings.windowMaxX = 30.0;
    settings.windowMaxY = 40.0;
    settings.plotViewName = "legacy plot view";
    settings.realWorldUnits = 2.0;
    settings.drawingUnits = 3.0;
    settings.currentStyleSheet = "monochrome.ctb";
    settings.scaleType = 4;
    settings.scaleFactor = 1.25;
    settings.paperImageOriginX = 0.125;
    settings.paperImageOriginY = 0.25;
    settings.shadePlotMode = 2;
    settings.shadePlotResLevel = 4;
    settings.shadePlotCustomDPI = 300;
    settings.plotViewHandle.ref = 0x34u;
    settings.shadePlotHandle.ref = 0x35u;
    return settings;
}

bool sweepPlotSettings(ImageDefinitionStreams mode) {
    const DRW::Version version = mode == ImageDefinitionStreams::R13InlineSeparateArguments
        ? DRW::AC1012
        : mode == ImageDefinitionStreams::R2004DetachedHandles
            ? DRW::AC1018 : DRW::AC1021;
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const PlotSettingsProbe settings = makePlotSettings();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    const auto outputs = [mode](dwgBufferW& body, dwgBufferW& strings,
                                dwgBufferW& handles) {
        dwgBufferW* stringOutput = nullptr;
        dwgBufferW* handleOutput = nullptr;
        switch (mode) {
        case ImageDefinitionStreams::R13InlineSeparateArguments:
        case ImageDefinitionStreams::R2004DetachedHandles:
        case ImageDefinitionStreams::Separate:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SharedStringAndHandle:
            stringOutput = &strings;
            handleOutput = &strings;
            break;
        case ImageDefinitionStreams::AllBody:
            break;
        case ImageDefinitionStreams::BodyStringDetachedHandles:
            stringOutput = &body;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SeparateStringBodyHandles:
            stringOutput = &strings;
            handleOutput = &body;
            break;
        }
        return std::pair<dwgBufferW*, dwgBufferW*>(stringOutput, handleOutput);
    };
    const auto cleanOutputs = outputs(cleanBody, cleanStrings, cleanHandles);
    if (!settings.encodeDwg(version, &cleanBody, cleanOutputs.first,
                            cleanOutputs.second))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    const auto output = outputs(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = settings.encodeDwg(
        version, &body, output.first, output.second);
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
        const auto failedOutput = outputs(failedBody, failedStrings, failedHandles);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = settings.encodeDwg(
            version, &failedBody, failedOutput.first, failedOutput.second);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!settings.encodeDwg(version, &failedBody, failedOutput.first,
                                failedOutput.second)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    static const char* const names[] = {
        "AC1012-inline", "AC1018-detached-handles", "separate",
        "shared-string-handle", "all-body", "body-string-detached-handles",
        "separate-string-body-handles"
    };
    std::cout << "PLOTSETTINGS DWG encoder version="
              << static_cast<int>(version) << ' '
              << names[static_cast<std::size_t>(mode)]
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

LayoutProbe makeLayout() {
    LayoutProbe layout;
    layout.handle = 0xF180u;
    layout.parentHandle = 0x20u;
    layout.reactorHandles = {0x21u, 0x22u};
    layout.xDictHandle = 0x23u;
    layout.setDwgCommonObjectState(2, 1u, false);
    layout.pageSetupName = "layout setup";
    layout.printerConfig = "printer driver.pc3";
    layout.plotLayoutFlags = 3;
    layout.marginLeft = 0.25;
    layout.marginBottom = 0.5;
    layout.marginRight = 0.75;
    layout.marginTop = 1.0;
    layout.paperWidth = 210.0;
    layout.paperHeight = 297.0;
    layout.paperSize = "ISO A4";
    layout.plotOriginX = 1.5;
    layout.plotOriginY = -2.5;
    layout.paperUnits = 1;
    layout.plotRotation = 2;
    layout.plotType = 5;
    layout.windowMinX = -10.0;
    layout.windowMinY = -20.0;
    layout.windowMaxX = 30.0;
    layout.windowMaxY = 40.0;
    layout.plotViewName = "legacy plot view";
    layout.realWorldUnits = 2.0;
    layout.drawingUnits = 3.0;
    layout.currentStyleSheet = "monochrome.ctb";
    layout.scaleType = 4;
    layout.scaleFactor = 1.25;
    layout.paperImageOriginX = 0.125;
    layout.paperImageOriginY = 0.25;
    layout.shadePlotMode = 2;
    layout.shadePlotResLevel = 4;
    layout.shadePlotCustomDPI = 300;
    layout.name = "MODEL";
    layout.tabOrder = 2;
    layout.layoutFlags = 1;
    layout.ucsOrigin = DRW_Coord{1.0, 2.0, 3.0};
    layout.limMinX = -100.0;
    layout.limMinY = -200.0;
    layout.limMaxX = 100.0;
    layout.limMaxY = 200.0;
    layout.insPoint = DRW_Coord{4.0, 5.0, 6.0};
    layout.ucsXAxis = DRW_Coord{1.0, 0.0, 0.0};
    layout.ucsYAxis = DRW_Coord{0.0, 1.0, 0.0};
    layout.elevation = 7.0;
    layout.orthoViewType = 3;
    layout.extMin = DRW_Coord{-10.0, -20.0, -30.0};
    layout.extMax = DRW_Coord{10.0, 20.0, 30.0};
    layout.viewportCount = 2;
    layout.viewportHandles = {0x31u, 0x32u};
    layout.plotViewHandle.ref = 0x34u;
    layout.shadePlotHandle.ref = 0x35u;
    layout.paperSpaceBlockRecordHandle.ref = 0x36u;
    layout.lastActiveViewportHandle.ref = 0x37u;
    layout.baseUcsHandle.ref = 0x38u;
    layout.namedUcsHandle.ref = 0x39u;
    return layout;
}

bool sweepLayout(ImageDefinitionStreams mode) {
    const DRW::Version version = mode == ImageDefinitionStreams::R13InlineSeparateArguments
        ? DRW::AC1012
        : mode == ImageDefinitionStreams::R2004DetachedHandles
            ? DRW::AC1018 : DRW::AC1021;
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const LayoutProbe layout = makeLayout();
    const auto outputs = [mode](dwgBufferW& body, dwgBufferW& strings,
                                dwgBufferW& handles) {
        dwgBufferW* stringOutput = nullptr;
        dwgBufferW* handleOutput = nullptr;
        switch (mode) {
        case ImageDefinitionStreams::R13InlineSeparateArguments:
        case ImageDefinitionStreams::R2004DetachedHandles:
        case ImageDefinitionStreams::Separate:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SharedStringAndHandle:
            stringOutput = &strings;
            handleOutput = &strings;
            break;
        case ImageDefinitionStreams::AllBody:
            break;
        case ImageDefinitionStreams::BodyStringDetachedHandles:
            stringOutput = &body;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SeparateStringBodyHandles:
            stringOutput = &strings;
            handleOutput = &body;
            break;
        }
        return std::pair<dwgBufferW*, dwgBufferW*>(stringOutput, handleOutput);
    };

    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    const auto cleanOutput = outputs(cleanBody, cleanStrings, cleanHandles);
    if (!layout.encodeDwg(version, &cleanBody, cleanOutput.first,
                          cleanOutput.second))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    const auto output = outputs(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = layout.encodeDwg(
        version, &body, output.first, output.second);
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
        const auto failedOutput = outputs(
            failedBody, failedStrings, failedHandles);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = layout.encodeDwg(
            version, &failedBody, failedOutput.first, failedOutput.second);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!layout.encodeDwg(version, &failedBody, failedOutput.first,
                              failedOutput.second)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "LAYOUT DWG encoder version=" << static_cast<int>(version)
              << " stream mode=" << static_cast<int>(mode)
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

DictionaryProbe makeDictionary() {
    DictionaryProbe dictionary;
    dictionary.handle = 0xF1A0u;
    dictionary.parentHandle = 0x20u;
    dictionary.reactorHandles = {0x21u, 0x22u};
    dictionary.xDictHandle = 0x23u;
    dictionary.cloning = 2;
    dictionary.hardOwner = 1;
    dictionary.m_entries = {
        {"ACAD_GROUP", 0x31u},
        {"ACAD_MLINESTYLE", 0x32u},
        {"ACAD_PLOTSETTINGS", 0x33u}
    };
    return dictionary;
}

bool sweepDictionary(DRW::Version version, ImageDefinitionStreams mode) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const DictionaryProbe dictionary = makeDictionary();
    const auto outputs = [mode](dwgBufferW& body, dwgBufferW& strings,
                                dwgBufferW& handles) {
        dwgBufferW* stringOutput = nullptr;
        dwgBufferW* handleOutput = nullptr;
        switch (mode) {
        case ImageDefinitionStreams::R13InlineSeparateArguments:
        case ImageDefinitionStreams::R2004DetachedHandles:
        case ImageDefinitionStreams::Separate:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SharedStringAndHandle:
            stringOutput = &strings;
            handleOutput = &strings;
            break;
        case ImageDefinitionStreams::AllBody:
            break;
        case ImageDefinitionStreams::BodyStringDetachedHandles:
            stringOutput = &body;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SeparateStringBodyHandles:
            stringOutput = &strings;
            handleOutput = &body;
            break;
        }
        return std::pair<dwgBufferW*, dwgBufferW*>(stringOutput, handleOutput);
    };

    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    const auto cleanOutput = outputs(cleanBody, cleanStrings, cleanHandles);
    if (!dictionary.encodeDwg(version, &cleanBody, cleanOutput.first,
                              cleanOutput.second))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    const auto output = outputs(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = dictionary.encodeDwg(
        version, &body, output.first, output.second);
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
        const auto failedOutput = outputs(
            failedBody, failedStrings, failedHandles);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = dictionary.encodeDwg(
            version, &failedBody, failedOutput.first, failedOutput.second);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!dictionary.encodeDwg(version, &failedBody, failedOutput.first,
                                  failedOutput.second)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "DICTIONARY DWG encoder version="
              << static_cast<int>(version)
              << " stream mode=" << static_cast<int>(mode)
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

bool sweepDictionaryWithDefault(DRW::Version version,
                                ImageDefinitionStreams mode) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    DictionaryWithDefaultProbe dictionary;
    dictionary.handle = 0xF1A8u;
    dictionary.parentHandle = 0x20u;
    dictionary.reactorHandles = {0x21u, 0x22u};
    dictionary.xDictHandle = 0x23u;
    dictionary.cloning = 2;
    dictionary.hardOwner = 1;
    dictionary.m_defaultEntryHandle = 0x34u;
    dictionary.m_entries = {
        {"ACAD_MATERIAL", 0x31u},
        {"ACAD_PLOTSTYLENAME", 0x32u}
    };
    const auto outputs = [mode](dwgBufferW& body, dwgBufferW& strings,
                                dwgBufferW& handles) {
        dwgBufferW* stringOutput = nullptr;
        dwgBufferW* handleOutput = nullptr;
        switch (mode) {
        case ImageDefinitionStreams::R13InlineSeparateArguments:
        case ImageDefinitionStreams::R2004DetachedHandles:
        case ImageDefinitionStreams::Separate:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SharedStringAndHandle:
            stringOutput = &strings;
            handleOutput = &strings;
            break;
        case ImageDefinitionStreams::AllBody:
            break;
        case ImageDefinitionStreams::BodyStringDetachedHandles:
            stringOutput = &body;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SeparateStringBodyHandles:
            stringOutput = &strings;
            handleOutput = &body;
            break;
        }
        return std::pair<dwgBufferW*, dwgBufferW*>(stringOutput, handleOutput);
    };

    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    const auto cleanOutput = outputs(cleanBody, cleanStrings, cleanHandles);
    if (!dictionary.encodeDwg(version, &cleanBody, cleanOutput.first,
                              cleanOutput.second))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    const auto output = outputs(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = dictionary.encodeDwg(
        version, &body, output.first, output.second);
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
        const auto failedOutput = outputs(
            failedBody, failedStrings, failedHandles);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = dictionary.encodeDwg(
            version, &failedBody, failedOutput.first, failedOutput.second);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!dictionary.encodeDwg(version, &failedBody, failedOutput.first,
                                  failedOutput.second)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "DICTIONARYWDFLT DWG encoder version="
              << static_cast<int>(version)
              << " stream mode=" << static_cast<int>(mode)
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

bool sweepDictionaryVar(DRW::Version version, ImageDefinitionStreams mode) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    DictionaryVarProbe variable;
    variable.handle = 0xF1B0u;
    variable.parentHandle = 0x20u;
    variable.reactorHandles = {0x21u, 0x22u};
    variable.xDictHandle = 0x23u;
    variable.m_schema = 3;
    variable.m_value = "ACAD_VARIABLE_VALUE_WITH_HEAP_BACKED_TEXT";
    const auto outputs = [mode](dwgBufferW& body, dwgBufferW& strings,
                                dwgBufferW& handles) {
        dwgBufferW* stringOutput = nullptr;
        dwgBufferW* handleOutput = nullptr;
        switch (mode) {
        case ImageDefinitionStreams::R13InlineSeparateArguments:
        case ImageDefinitionStreams::R2004DetachedHandles:
        case ImageDefinitionStreams::Separate:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SharedStringAndHandle:
            stringOutput = &strings;
            handleOutput = &strings;
            break;
        case ImageDefinitionStreams::AllBody:
            break;
        case ImageDefinitionStreams::BodyStringDetachedHandles:
            stringOutput = &body;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SeparateStringBodyHandles:
            stringOutput = &strings;
            handleOutput = &body;
            break;
        }
        return std::pair<dwgBufferW*, dwgBufferW*>(stringOutput, handleOutput);
    };

    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    const auto cleanOutput = outputs(cleanBody, cleanStrings, cleanHandles);
    if (!variable.encodeDwg(version, &cleanBody, cleanOutput.first,
                            cleanOutput.second))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    const auto output = outputs(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = variable.encodeDwg(
        version, &body, output.first, output.second);
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
        const auto failedOutput = outputs(
            failedBody, failedStrings, failedHandles);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = variable.encodeDwg(
            version, &failedBody, failedOutput.first, failedOutput.second);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!variable.encodeDwg(version, &failedBody, failedOutput.first,
                                failedOutput.second)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "DICTIONARYVAR DWG encoder version="
              << static_cast<int>(version)
              << " stream mode=" << static_cast<int>(mode)
              << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

bool sweepXRecord(DRW::Version version, bool separateHandles) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    XRecordProbe record;
    record.handle = 0xF1C0u;
    record.parentHandle = 0x20u;
    record.reactorHandles = {0x21u, 0x22u};
    record.xDictHandle = 0x23u;
    record.m_cloning = 2;
    record.m_values.emplace_back(1, UTF8STRING(
        "A long XRECORD string value that allocates while being encoded"));
    record.m_values.emplace_back(40, 1.25);
    record.m_values.emplace_back(10, DRW_Coord{1.0, 2.0, 3.0});
    record.m_values.emplace_back(310, std::vector<std::uint8_t>(300, 0xA5u));
    record.m_handleValues.emplace_back(0, 0x41u);
    record.m_handleValues.emplace_back(331, 0x42u);

    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    if (!record.encodeDwg(version, &cleanBody, &cleanStrings,
                          separateHandles ? &cleanHandles : nullptr))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = record.encodeDwg(
        version, &body, &strings,
        separateHandles ? &handles : nullptr);
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
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = record.encodeDwg(
            version, &failedBody, &failedStrings,
            separateHandles ? &failedHandles : nullptr);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!record.encodeDwg(version, &failedBody, &failedStrings,
                              separateHandles ? &failedHandles : nullptr)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    std::cout << "XRECORD DWG encoder version="
              << static_cast<int>(version) << ' '
              << (separateHandles ? "detached" : "body-aliased")
              << " handles: allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

ScaleProbe makeScale() {
    ScaleProbe scale;
    scale.handle = 0xF200u;
    scale.name = "detail scale";
    scale.flag = 1u;
    scale.paperUnits = 1.0;
    scale.drawingUnits = 24.0;
    scale.isUnitScale = false;
    return scale;
}

bool sweepScale(DRW::Version version, bool separateStringStream,
                bool passBodyAsStringStream) {
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const ScaleProbe scale = makeScale();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    dwgBufferW* cleanStringOutput = separateStringStream
        ? &cleanStrings : (passBodyAsStringStream ? &cleanBody : nullptr);
    if (!scale.encodeDwg(version, &cleanBody, cleanStringOutput))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    dwgBufferW* stringOutput = separateStringStream
        ? &strings : (passBodyAsStringStream ? &body : nullptr);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = scale.encodeDwg(version, &body, stringOutput);
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
        dwgBufferW* failedStringOutput = separateStringStream
            ? &failedStrings
            : (passBodyAsStringStream ? &failedBody : nullptr);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = scale.encodeDwg(
            version, &failedBody, failedStringOutput);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!scale.encodeDwg(version, &failedBody, failedStringOutput)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    const char* const mode = separateStringStream ? "separate-string"
        : passBodyAsStringStream ? "explicit-body-alias" : "inline-string";
    std::cout << "SCALE DWG encoder version=" << static_cast<int>(version)
              << ' ' << mode << ": allocation offsets=" << allocations
              << ", rollback/retry=PASS\n";
    return true;
}

GroupProbe makeGroup() {
    GroupProbe group;
    group.handle = 0xF300u;
    group.parentHandle = 0x20u;
    group.reactorHandles = {0x21u};
    group.xDictHandle = 0x22u;
    group.setDwgCommonObjectState(1, 1u, false);
    group.m_description = "three selected entities";
    group.m_isUnnamed = false;
    group.m_selectable = true;
    group.m_entityHandles = {0x40u, 0x44u, 0x48u};
    return group;
}

bool sweepGroup(ImageDefinitionStreams mode) {
    const DRW::Version version = mode == ImageDefinitionStreams::R13InlineSeparateArguments
        ? DRW::AC1012
        : mode == ImageDefinitionStreams::R2004DetachedHandles
            ? DRW::AC1018 : DRW::AC1021;
    DRW_TextCodec codec;
    codec.setVersion(version, false);
    const GroupProbe group = makeGroup();
    dwgBufferW cleanBody(&codec), cleanStrings(&codec), cleanHandles(&codec);
    seed(cleanBody, cleanStrings, cleanHandles);
    const auto outputs = [mode](dwgBufferW& body, dwgBufferW& strings,
                                dwgBufferW& handles) {
        dwgBufferW* stringOutput = nullptr;
        dwgBufferW* handleOutput = nullptr;
        switch (mode) {
        case ImageDefinitionStreams::R13InlineSeparateArguments:
        case ImageDefinitionStreams::R2004DetachedHandles:
        case ImageDefinitionStreams::Separate:
            stringOutput = &strings;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SharedStringAndHandle:
            stringOutput = &strings;
            handleOutput = &strings;
            break;
        case ImageDefinitionStreams::AllBody:
            break;
        case ImageDefinitionStreams::BodyStringDetachedHandles:
            stringOutput = &body;
            handleOutput = &handles;
            break;
        case ImageDefinitionStreams::SeparateStringBodyHandles:
            stringOutput = &strings;
            handleOutput = &body;
            break;
        }
        return std::pair<dwgBufferW*, dwgBufferW*>(stringOutput, handleOutput);
    };
    const auto cleanOutputs = outputs(cleanBody, cleanStrings, cleanHandles);
    if (!group.encodeDwg(version, &cleanBody, cleanOutputs.first,
                         cleanOutputs.second))
        return false;

    dwgBufferW body(&codec), strings(&codec), handles(&codec);
    seed(body, strings, handles);
    const auto output = outputs(body, strings, handles);
    allocationsBeforeFailure.store(1000000, std::memory_order_relaxed);
    const unsigned long beforeCalibration =
        failedAllocations.load(std::memory_order_relaxed);
    const bool calibrated = group.encodeDwg(
        version, &body, output.first, output.second);
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
        const auto failedOutput = outputs(failedBody, failedStrings, failedHandles);
        const dwgBufferW originalBody = failedBody;
        const dwgBufferW originalStrings = failedStrings;
        const dwgBufferW originalHandles = failedHandles;
        const unsigned long beforeFailure =
            failedAllocations.load(std::memory_order_relaxed);
        allocationsBeforeFailure.store(offset, std::memory_order_relaxed);
        const bool encoded = group.encodeDwg(
            version, &failedBody, failedOutput.first, failedOutput.second);
        allocationsBeforeFailure.store(-1, std::memory_order_relaxed);
        if (encoded
            || failedAllocations.load(std::memory_order_relaxed)
                != beforeFailure + 1u
            || !same(failedBody, originalBody)
            || !same(failedStrings, originalStrings)
            || !same(failedHandles, originalHandles))
            return false;
        if (!group.encodeDwg(version, &failedBody, failedOutput.first,
                             failedOutput.second)
            || !same(failedBody, cleanBody)
            || !same(failedStrings, cleanStrings)
            || !same(failedHandles, cleanHandles))
            return false;
    }
    static const char* const names[] = {
        "AC1012-inline", "AC1018-detached-handles", "separate",
        "shared-string-handle", "all-body", "body-string-detached-handles",
        "separate-string-body-handles"
    };
    std::cout << "GROUP DWG encoder version=" << static_cast<int>(version)
              << ' ' << names[static_cast<std::size_t>(mode)]
              << ": allocation offsets=" << allocations
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
        || !sweepEvaluationGraph(true)
        || !sweepBlockRepresentation(false)
        || !sweepBlockRepresentation(true)
        || !sweepImageDefinitionReactor(DRW::AC1012, false)
        || !sweepImageDefinitionReactor(DRW::AC1024, false)
        || !sweepImageDefinitionReactor(DRW::AC1024, true)
        || !sweepImageDefinition(ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepImageDefinition(ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepImageDefinition(ImageDefinitionStreams::Separate)
        || !sweepImageDefinition(ImageDefinitionStreams::SharedStringAndHandle)
        || !sweepImageDefinition(ImageDefinitionStreams::AllBody)
        || !sweepImageDefinition(ImageDefinitionStreams::BodyStringDetachedHandles)
        || !sweepImageDefinition(ImageDefinitionStreams::SeparateStringBodyHandles)
        || !sweepPlotSettings(ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepPlotSettings(ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepPlotSettings(ImageDefinitionStreams::Separate)
        || !sweepPlotSettings(ImageDefinitionStreams::SharedStringAndHandle)
        || !sweepPlotSettings(ImageDefinitionStreams::AllBody)
        || !sweepPlotSettings(ImageDefinitionStreams::BodyStringDetachedHandles)
        || !sweepPlotSettings(ImageDefinitionStreams::SeparateStringBodyHandles)
        || !sweepLayout(ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepLayout(ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepLayout(ImageDefinitionStreams::Separate)
        || !sweepLayout(ImageDefinitionStreams::SharedStringAndHandle)
        || !sweepLayout(ImageDefinitionStreams::AllBody)
        || !sweepLayout(ImageDefinitionStreams::BodyStringDetachedHandles)
        || !sweepLayout(ImageDefinitionStreams::SeparateStringBodyHandles)
        || !sweepDictionary(DRW::AC1012,
                ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepDictionary(DRW::AC1014,
                ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepDictionary(DRW::AC1015,
                ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepDictionary(DRW::AC1018,
                ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepDictionary(DRW::AC1021, ImageDefinitionStreams::Separate)
        || !sweepDictionary(DRW::AC1021,
                ImageDefinitionStreams::SharedStringAndHandle)
        || !sweepDictionary(DRW::AC1021, ImageDefinitionStreams::AllBody)
        || !sweepDictionary(DRW::AC1021,
                ImageDefinitionStreams::BodyStringDetachedHandles)
        || !sweepDictionary(DRW::AC1021,
                ImageDefinitionStreams::SeparateStringBodyHandles)
        || !sweepDictionaryWithDefault(DRW::AC1012,
                ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepDictionaryWithDefault(DRW::AC1015,
                ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepDictionaryWithDefault(DRW::AC1018,
                ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepDictionaryWithDefault(DRW::AC1021,
                ImageDefinitionStreams::Separate)
        || !sweepDictionaryWithDefault(DRW::AC1021,
                ImageDefinitionStreams::SharedStringAndHandle)
        || !sweepDictionaryWithDefault(DRW::AC1021,
                ImageDefinitionStreams::AllBody)
        || !sweepDictionaryWithDefault(DRW::AC1021,
                ImageDefinitionStreams::BodyStringDetachedHandles)
        || !sweepDictionaryWithDefault(DRW::AC1021,
                ImageDefinitionStreams::SeparateStringBodyHandles)
        || !sweepDictionaryVar(DRW::AC1012,
                ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepDictionaryVar(DRW::AC1018,
                ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepDictionaryVar(DRW::AC1021, ImageDefinitionStreams::Separate)
        || !sweepDictionaryVar(DRW::AC1021,
                ImageDefinitionStreams::SharedStringAndHandle)
        || !sweepDictionaryVar(DRW::AC1021, ImageDefinitionStreams::AllBody)
        || !sweepDictionaryVar(DRW::AC1021,
                ImageDefinitionStreams::BodyStringDetachedHandles)
        || !sweepDictionaryVar(DRW::AC1021,
                ImageDefinitionStreams::SeparateStringBodyHandles)
        || !sweepXRecord(DRW::AC1012, false)
        || !sweepXRecord(DRW::AC1018, true)
        || !sweepXRecord(DRW::AC1021, true)
        || !sweepXRecord(DRW::AC1021, false)
        || !sweepScale(DRW::AC1012, false, false)
        || !sweepScale(DRW::AC1021, true, false)
        || !sweepScale(DRW::AC1021, false, true)
        || !sweepRasterVariables(DRW::AC1012, false)
        || !sweepRasterVariables(DRW::AC1018, false)
        || !sweepRasterVariables(DRW::AC1021, true)
        || !sweepWipeoutVariables(DRW::AC1012, false)
        || !sweepWipeoutVariables(DRW::AC1018, false)
        || !sweepWipeoutVariables(DRW::AC1021, true)
        || !sweepSortEntsTable(DRW::AC1012, false)
        || !sweepSortEntsTable(DRW::AC1018, false)
        || !sweepSortEntsTable(DRW::AC1021, true)
        || !sweepGroup(ImageDefinitionStreams::R13InlineSeparateArguments)
        || !sweepGroup(ImageDefinitionStreams::R2004DetachedHandles)
        || !sweepGroup(ImageDefinitionStreams::Separate)
        || !sweepGroup(ImageDefinitionStreams::SharedStringAndHandle)
        || !sweepGroup(ImageDefinitionStreams::AllBody)
        || !sweepGroup(ImageDefinitionStreams::BodyStringDetachedHandles)
        || !sweepGroup(ImageDefinitionStreams::SeparateStringBodyHandles)) {
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
