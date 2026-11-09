// SPDX-License-Identifier: GPL-2.0-or-later
// Generated FIELD, path-object, and PARTIAL_VIEWING_INDEX allocation control.
#include "dx_iface.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <string>
#include <vector>

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0;

struct AllocationFailure {
    explicit AllocationFailure(long count) { allocationsBeforeFailure = count; }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

class IndexExport final : public dx_iface {
public:
    explicit IndexExport(dxfRW& codec) : codec(codec) {
        cData = &storage;
        currentBlock = storage.mBlock;
        dxfW = &codec;
        line.handle = 0xD7B0u;
        line.parentHandle = DRW::DxfModelSpaceBlockRecordHandle;
        line.basePoint = {1.0, 2.0, 3.0};
        line.secPoint = {4.0, 5.0, 6.0};
        field.handle = 0xD7B3u;
        field.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
        field.m_evaluatorId = "AcExpr";
        field.m_fieldCode = "1+1";
        field.m_valueString = "2";
        field.m_valueStringLength = 1;
        fieldList.handle = 0xD7B4u;
        fieldList.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
        fieldList.m_fieldHandles.push_back(field.handle);
        motionPath.handle = 0xD7B5u;
        motionPath.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
        motionPath.m_classVersion = 1;
        motionPath.m_frames = 120;
        motionPath.m_frameRate = 30;
        motionPath.m_cornerDeceleration = true;
        curvePath.handle = 0xD7B6u;
        curvePath.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
        curvePath.m_classVersion = 1;
        pointPath.handle = 0xD7B7u;
        pointPath.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
        pointPath.m_classVersion = 1;
        pointPath.m_point = {9.0, 8.0, 7.0};
        index.handle = 0xD7B2u;
        index.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
        index.m_entries.push_back({{0.0, 1.0, 2.0}, {7.0, 8.0, 9.0}, line.handle});
    }

    void writeEntities() override {
        lineWritten = codec.writeLine(&line);
        if (!lineWritten)
            (void)codec.writeRawDxfObject(nullptr);
    }

    void writeObjects() override {
        if (failAt < 0) {
            fieldWritten = codec.writeField(&field);
            fieldListWritten = fieldWritten
                && codec.writeFieldList(&fieldList);
            pathsWritten = fieldListWritten
                && codec.writeMotionPath(&motionPath)
                && codec.writeCurvePath(&curvePath)
                && codec.writePointPath(&pointPath);
            indexWritten = pathsWritten
                && codec.writePartialViewingIndex(&index);
            if (!indexWritten)
                (void)codec.writeRawDxfObject(nullptr);
            return;
        }
        const std::size_t before = failedAllocations;
        AllocationFailure failure(failAt);
        try {
            fieldWritten = codec.writeField(&field);
            fieldListWritten = fieldWritten
                && codec.writeFieldList(&fieldList);
            pathsWritten = fieldListWritten
                && codec.writeMotionPath(&motionPath)
                && codec.writeCurvePath(&curvePath)
                && codec.writePointPath(&pointPath);
            indexWritten = pathsWritten
                && codec.writePartialViewingIndex(&index);
        } catch (const std::bad_alloc&) {
            reached = failedAllocations != before
                ? static_cast<long>(failedAllocations - before)
                : failAt - allocationsBeforeFailure;
            throw;
        }
        reached = failedAllocations != before
            ? static_cast<long>(failedAllocations - before)
            : failAt - allocationsBeforeFailure;
        if (!indexWritten)
            (void)codec.writeRawDxfObject(nullptr);
    }

    bool finalizeDxfWrite() override {
        return lineWritten && fieldWritten && fieldListWritten && pathsWritten
            && indexWritten;
    }

    dxfRW& codec;
    dx_data storage;
    DRW_Line line;
    DRW_Field field;
    DRW_FieldList fieldList;
    DRW_MotionPath motionPath;
    DRW_CurvePath curvePath;
    DRW_PointPath pointPath;
    DRW_PartialViewingIndex index;
    long failAt = -1;
    long reached = 0;
    bool lineWritten = false;
    bool fieldWritten = false;
    bool fieldListWritten = false;
    bool pathsWritten = false;
    bool indexWritten = false;
};

bool readAll(const std::filesystem::path& path, std::string& value) {
    std::ifstream input(path, std::ios::binary);
    if (!input)
        return false;
    value.assign(std::istreambuf_iterator<char>{input},
                 std::istreambuf_iterator<char>{});
    return !input.bad();
}

bool lineMatches(const DRW_Line& actual, const DRW_Line& expected) {
    return actual.parentHandle == expected.parentHandle
        && actual.basePoint.x == expected.basePoint.x
        && actual.basePoint.y == expected.basePoint.y
        && actual.basePoint.z == expected.basePoint.z
        && actual.secPoint.x == expected.secPoint.x
        && actual.secPoint.y == expected.secPoint.y
        && actual.secPoint.z == expected.secPoint.z;
}

bool indexMatches(const DRW_PartialViewingIndex& actual,
                  const DRW_PartialViewingIndex& expected) {
    if (actual.handle != expected.handle
        || actual.parentHandle != expected.parentHandle
        || actual.m_entries.size() != expected.m_entries.size())
        return false;
    for (std::size_t i = 0; i < expected.m_entries.size(); ++i) {
        const auto& left = actual.m_entries[i];
        const auto& right = expected.m_entries[i];
        if (left.objectHandle != right.objectHandle
            || left.extentsMin.x != right.extentsMin.x
            || left.extentsMin.y != right.extentsMin.y
            || left.extentsMin.z != right.extentsMin.z
            || left.extentsMax.x != right.extentsMax.x
            || left.extentsMax.y != right.extentsMax.y
            || left.extentsMax.z != right.extentsMax.z)
            return false;
    }
    return true;
}

bool readbackMatches(const std::filesystem::path& path,
                     const DRW_Line& expectedLine,
                     const DRW_PartialViewingIndex& expectedIndex) {
    dx_data data;
    class IndexReadback final : public dx_iface {
    public:
        void addLine(const DRW_Line& value) override { lines.push_back(value); }
        void addField(const DRW_Field& value) override { fields.push_back(value); }
        void addFieldList(const DRW_FieldList& value) override {
            fieldLists.push_back(value);
        }
        void addMotionPath(const DRW_MotionPath& value) override {
            motionPaths.push_back(value);
        }
        void addCurvePath(const DRW_CurvePath& value) override {
            curvePaths.push_back(value);
        }
        void addPointPath(const DRW_PointPath& value) override {
            pointPaths.push_back(value);
        }
        void addPartialViewingIndex(
                const DRW_PartialViewingIndex& value) override {
            indexes.push_back(value);
        }
        std::vector<DRW_Line> lines;
        std::vector<DRW_Field> fields;
        std::vector<DRW_FieldList> fieldLists;
        std::vector<DRW_MotionPath> motionPaths;
        std::vector<DRW_CurvePath> curvePaths;
        std::vector<DRW_PointPath> pointPaths;
        std::vector<DRW_PartialViewingIndex> indexes;
    } reader;
    if (!reader.fileImport(path.string(), &data, false)
        || data.sourceFormat != dx_source_format::DXF
        || reader.lines.size() != 1u || reader.fields.size() != 1u
        || reader.fieldLists.size() != 1u
        || reader.motionPaths.size() != 1u || reader.curvePaths.size() != 1u
        || reader.pointPaths.size() != 1u
        || reader.indexes.size() != 1u
        || !lineMatches(reader.lines.front(), expectedLine))
        return false;
    const DRW_Field& actualField = reader.fields.front();
    if (actualField.handle != 0xD7B3u
        || actualField.parentHandle != DRW::DwgNamedObjectsDictionaryHandle
        || actualField.m_evaluatorId != "AcExpr"
        || actualField.m_fieldCode != "1+1"
        || actualField.m_valueString != "2"
        || actualField.m_valueStringLength != 1)
        return false;
    const DRW_FieldList& actualFieldList = reader.fieldLists.front();
    if (actualFieldList.handle != 0xD7B4u
        || actualFieldList.parentHandle != DRW::DwgNamedObjectsDictionaryHandle
        || actualFieldList.m_fieldHandles != std::vector<std::uint32_t>{0xD7B3u})
        return false;
    const DRW_MotionPath& actualMotionPath = reader.motionPaths.front();
    if (actualMotionPath.handle != 0xD7B5u
        || actualMotionPath.m_classVersion != 1
        || actualMotionPath.m_frames != 120 || actualMotionPath.m_frameRate != 30
        || !actualMotionPath.m_cornerDeceleration)
        return false;
    const DRW_CurvePath& actualCurvePath = reader.curvePaths.front();
    if (actualCurvePath.handle != 0xD7B6u
        || actualCurvePath.m_classVersion != 1
        || actualCurvePath.m_entityHandle != 0u)
        return false;
    const DRW_PointPath& actualPointPath = reader.pointPaths.front();
    if (actualPointPath.handle != 0xD7B7u
        || actualPointPath.m_classVersion != 1
        || actualPointPath.m_point.x != 9.0 || actualPointPath.m_point.y != 8.0
        || actualPointPath.m_point.z != 7.0)
        return false;
    DRW_PartialViewingIndex expectedOutput = expectedIndex;
    expectedOutput.m_entries.front().objectHandle = reader.lines.front().handle;
    return indexMatches(reader.indexes.front(), expectedOutput);
}

bool run(const std::filesystem::path& directory) {
    const auto fail = [](const char* message) {
        std::cerr << "FIELD/path/PARTIAL_VIEWING_INDEX allocation retry: "
                  << message << '\n';
        return false;
    };
    if (!std::filesystem::create_directories(directory)
        && !std::filesystem::is_directory(directory))
        return fail("could not create scratch directory");

    const auto output = directory / "field-fieldlist-partial-viewing-index.dxf";
    const std::string sentinel = "prior FIELD/FIELDLIST/PARTIAL_VIEWING_INDEX destination\n";
    for (const bool binary : {false, true}) {
        long allocationCount = 0;
        std::string expectedBytes;
        {
            const std::string outputName = output.string();
            dxfRW codec(outputName.c_str());
            IndexExport exporter(codec);
            codec.setDxfTextWritePolicy(DRW::DxfTextWritePolicy::RequireLossless);
            codec.setDxfWriteRecordInventoryEnabled(true);
            exporter.failAt = 1000000;
            const bool exported = codec.write(&exporter, DRW::AC1027, binary);
            const bool readback = exported && readbackMatches(
                output, exporter.line, exporter.index);
            if (!exported || !exporter.lineWritten || !exporter.fieldWritten
                || !exporter.fieldListWritten || !exporter.pathsWritten
                || !exporter.indexWritten
                || !readback) {
                std::cerr << "calibration codecWrite=" << exported
                          << " lineWrite=" << exporter.lineWritten
                          << " fieldWrite=" << exporter.fieldWritten
                          << " fieldListWrite=" << exporter.fieldListWritten
                          << " pathsWrite=" << exporter.pathsWritten
                          << " indexWrite=" << exporter.indexWritten
                          << " readback=" << readback
                          << " binary=" << binary << '\n';
                return fail("allocation calibration output did not read back");
            }
            allocationCount = exporter.reached;
            if (!readAll(output, expectedBytes))
                return fail("could not retain calibration output bytes");
        }
        if (allocationCount <= 0)
            return fail("calibration observed no OBJECTS callback allocations");

        for (long offset = 0; offset < allocationCount; ++offset) {
            {
                std::ofstream prior(output, std::ios::binary | std::ios::trunc);
                prior << sentinel;
                if (!prior)
                    return fail("could not initialize destination sentinel");
            }
            const std::string outputName = output.string();
            dxfRW codec(outputName.c_str());
            IndexExport exporter(codec);
            codec.setDxfTextWritePolicy(DRW::DxfTextWritePolicy::RequireLossless);
            codec.setDxfWriteRecordInventoryEnabled(true);
            const DRW_Line sourceLine = exporter.line;
            const DRW_Field sourceField = exporter.field;
            const DRW_FieldList sourceFieldList = exporter.fieldList;
            const DRW_MotionPath sourceMotionPath = exporter.motionPath;
            const DRW_CurvePath sourceCurvePath = exporter.curvePath;
            const DRW_PointPath sourcePointPath = exporter.pointPath;
            const DRW_PartialViewingIndex sourceIndex = exporter.index;
            exporter.failAt = offset;
            exporter.reached = 0;
            exporter.lineWritten = false;
            exporter.fieldWritten = false;
            exporter.fieldListWritten = false;
            exporter.pathsWritten = false;
            exporter.indexWritten = false;
            const bool wrote = codec.write(&exporter, DRW::AC1027, binary);
            exporter.failAt = -1;
            std::string current;
            if (wrote || exporter.reached != 1
                || !readAll(output, current) || current != sentinel)
                return fail("injected failure changed destination or missed allocation");
            if (!lineMatches(exporter.line, sourceLine)
                || exporter.field.handle != sourceField.handle
                || exporter.field.parentHandle != sourceField.parentHandle
                || exporter.field.m_evaluatorId != sourceField.m_evaluatorId
                || exporter.field.m_fieldCode != sourceField.m_fieldCode
                || exporter.field.m_valueString != sourceField.m_valueString
                || exporter.field.m_valueStringLength != sourceField.m_valueStringLength
                || exporter.fieldList.handle != sourceFieldList.handle
                || exporter.fieldList.parentHandle != sourceFieldList.parentHandle
                || exporter.fieldList.m_fieldHandles != sourceFieldList.m_fieldHandles
                || exporter.motionPath.handle != sourceMotionPath.handle
                || exporter.motionPath.parentHandle != sourceMotionPath.parentHandle
                || exporter.motionPath.m_frames != sourceMotionPath.m_frames
                || exporter.motionPath.m_frameRate != sourceMotionPath.m_frameRate
                || exporter.motionPath.m_cornerDeceleration != sourceMotionPath.m_cornerDeceleration
                || exporter.curvePath.handle != sourceCurvePath.handle
                || exporter.curvePath.parentHandle != sourceCurvePath.parentHandle
                || exporter.curvePath.m_entityHandle != sourceCurvePath.m_entityHandle
                || exporter.pointPath.handle != sourcePointPath.handle
                || exporter.pointPath.parentHandle != sourcePointPath.parentHandle
                || exporter.pointPath.m_point.x != sourcePointPath.m_point.x
                || exporter.pointPath.m_point.y != sourcePointPath.m_point.y
                || exporter.pointPath.m_point.z != sourcePointPath.m_point.z
                || !indexMatches(exporter.index, sourceIndex))
                return fail("injected failure changed entity, FIELD/path, or PVI state");
            // Entity serialization assigns the emitted handle to the caller's
            // entity before the later OBJECTS callback fails. Restore its source
            // identity so this retry exercises the same source graph.
            if (exporter.line.handle == sourceLine.handle)
                return fail("expected prior ENTITIES callback mutation was absent");
            exporter.line.handle = sourceLine.handle;
            const bool retry = codec.write(&exporter, DRW::AC1027, binary);
            std::string retryBytes;
            if (!retry || !exporter.lineWritten || !exporter.fieldWritten
                || !exporter.fieldListWritten || !exporter.pathsWritten
                || !exporter.indexWritten
                || !readAll(output, retryBytes) || retryBytes != expectedBytes
                || !readbackMatches(output, sourceLine, sourceIndex)
                || !lineMatches(exporter.line, sourceLine)
                || !indexMatches(exporter.index, sourceIndex))
                return fail("same-codec retry failed exact bytes or graph readback");
        }
        std::cout << "FIELD/path/PARTIAL_VIEWING_INDEX OBJECTS "
                  << (binary ? "Binary" : "ASCII")
                  << ": allocation offsets=" << allocationCount
                  << ", rollback/retry/readback=PASS\n";
        std::error_code ignored;
        std::filesystem::remove(output, ignored);
    }
    return true;
}
} // namespace

void* operator new(std::size_t size) {
    if (allocationsBeforeFailure == 0) {
        ++failedAllocations;
        allocationsBeforeFailure = -1;
        throw std::bad_alloc();
    }
    if (allocationsBeforeFailure > 0)
        --allocationsBeforeFailure;
    if (void* value = std::malloc(size == 0 ? 1 : size))
        return value;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* value) noexcept { std::free(value); }
void operator delete[](void* value) noexcept { std::free(value); }
void operator delete(void* value, std::size_t) noexcept { std::free(value); }
void operator delete[](void* value, std::size_t) noexcept { std::free(value); }

int main(int argc, char** argv) {
    if (argc != 2) {
        std::cerr << "usage: partial_viewing_index_objects_allocation_tests SCRATCH_DIR\n";
        return 2;
    }
    const std::size_t before = failedAllocations;
    if (!run(argv[1]))
        return 1;
    if (failedAllocations == before) {
        std::cerr << "allocation injector did not reach operator new\n";
        return 1;
    }
    return 0;
}
