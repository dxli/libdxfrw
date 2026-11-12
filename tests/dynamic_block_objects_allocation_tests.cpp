// SPDX-License-Identifier: GPL-2.0-or-later
// Generated dynamic-block OBJECTS record allocation rollback control.
#include "libdxfrw.h"
#include "intern/dxfreader.h"
#include "intern/dxfwriter.h"

#include <cstdlib>
#include <iostream>
#include <memory>
#include <new>
#include <sstream>
#include <string>
#include <vector>

class DxfDynamicBlockTestAccess {
public:
    static void attachWriter(dxfRW& codec, DRW::Version version,
                             bool binary, std::ostream& output) {
        codec.version = version;
        codec.binFile = binary;
        codec.writer = binary
            ? std::unique_ptr<dxfWriter>(
                  std::make_unique<dxfWriterBinary>(&output))
            : std::unique_ptr<dxfWriter>(
                  std::make_unique<dxfWriterAscii>(&output));
    }

    static bool writeEnd(dxfRW& codec) {
        return codec.writer != nullptr
            && codec.writer->writeString(0, "WIRE_END");
    }
};

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0u;

struct AllocationFailure {
    explicit AllocationFailure(long count) {
        allocationsBeforeFailure = count;
    }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

DRW_DynamicBlockObject makePurgePreventer() {
    DRW_DynamicBlockObject object(
        "ACDB_DYNAMICBLOCKPURGEPREVENTER_VERSION");
    object.handle = 0xE702u;
    object.parentHandle = 0xE701u;
    object.reactorHandles.push_back(0xE701u);
    object.m_purgeFlag = 1u;
    object.m_purgeBlockHandle = 0xE700u;
    object.m_bodyFullyDecoded = true;
    return object;
}

bool wireReadback(const std::string& bytes, bool binary) {
    std::istringstream input(bytes);
    std::unique_ptr<dxfReader> reader = binary
        ? std::unique_ptr<dxfReader>(std::make_unique<dxfReaderBinary>(&input))
        : std::unique_ptr<dxfReader>(std::make_unique<dxfReaderAscii>(&input));
    int code = 0;
    if (!reader->readRec(&code) || code != 0
        || reader->getString() != "ACDB_DYNAMICBLOCKPURGEPREVENTER_VERSION")
        return false;
    bool ended = false;
    bool hasHandle = false;
    bool hasSubclass = false;
    bool hasFlag = false;
    std::vector<std::uint32_t> owners;
    while (reader->readRec(&code)) {
        if (code == 0) {
            ended = reader->getString() == "WIRE_END";
            break;
        }
        switch (code) {
        case 5:
            hasHandle = reader->getHandleString() == 0xE702u;
            break;
        case 102:
            {
                const std::string marker = reader->getString();
                if (marker != "{ACAD_REACTORS" && marker != "}")
                    return false;
            }
            break;
        case 330:
            owners.push_back(reader->getHandleString());
            break;
        case 100:
            hasSubclass = reader->getString()
                == "AcDbDynamicBlockPurgePreventer";
            break;
        case 70:
            hasFlag = reader->getInt32() == 1;
            break;
        default:
            return false;
        }
    }
    return ended && hasHandle && hasSubclass && hasFlag
        && owners == std::vector<std::uint32_t>{0xE701u, 0xE701u};
}

bool sourceUnchanged(const DRW_DynamicBlockObject& object) {
    return object.handle == 0xE702u
        && static_cast<std::uint32_t>(object.parentHandle) == 0xE701u
        && object.reactorHandles == std::vector<std::uint32_t>{0xE701u}
        && object.m_purgeFlag == 1u
        && object.m_purgeBlockHandle == 0xE700u
        && object.m_bodyFullyDecoded;
}

bool allocationSweep(bool binary) {
    constexpr long calibrationLimit = 1000000;
    const auto writeRecord = [](dxfRW& codec,
                                DRW_DynamicBlockObject& object) {
        return codec.writeDynamicBlockObject(&object);
    };

    std::ostringstream cleanBytes;
    dxfRW cleanCodec("");
    DxfDynamicBlockTestAccess::attachWriter(
        cleanCodec, DRW::AC1024, binary, cleanBytes);
    DRW_DynamicBlockObject cleanObject = makePurgePreventer();
    long allocationCount = 0;
    bool cleanWrite = false;
    {
        AllocationFailure failure(calibrationLimit);
        cleanWrite = writeRecord(cleanCodec, cleanObject);
        allocationCount = calibrationLimit - allocationsBeforeFailure;
    }
    if (!cleanWrite || !sourceUnchanged(cleanObject)
        || cleanCodec.getWritingContext()
            ->sourceHandleToWrittenObjectMap.count(cleanObject.handle) != 1u
        || cleanCodec.getWritingContext()
            ->sourceHandleToWrittenObjectMap.at(cleanObject.handle)
            != cleanCodec.remapHandle(cleanObject.handle)
        || !DxfDynamicBlockTestAccess::writeEnd(cleanCodec)) {
        std::cerr << "clean PurgePreventer record failed (binary="
                  << binary << ") write=" << cleanWrite
                  << " source=" << sourceUnchanged(cleanObject)
                  << " handleBindings="
                  << cleanCodec.getWritingContext()
                         ->sourceHandleToWrittenObjectMap.size()
                  << '\n';
        return false;
    }
    const std::string expectedBytes = cleanBytes.str();
    const bool cleanReadback = wireReadback(expectedBytes, binary);
    if (allocationCount <= 0 || !cleanReadback) {
        std::cerr << "PurgePreventer clean readback/calibration failed binary="
                  << binary << " allocationCount=" << allocationCount
                  << " bytes=" << expectedBytes.size()
                  << " readback=" << cleanReadback << '\n';
        return false;
    }

    for (long offset = 0; offset < allocationCount; ++offset) {
        std::ostringstream failedBytes;
        dxfRW failedCodec("");
        DxfDynamicBlockTestAccess::attachWriter(
            failedCodec, DRW::AC1024, binary, failedBytes);
        DRW_DynamicBlockObject object = makePurgePreventer();
        const std::size_t failuresBefore = failedAllocations;
        bool wrote = false;
        try {
            AllocationFailure failure(offset);
            wrote = writeRecord(failedCodec, object);
        } catch (const std::bad_alloc&) {
            allocationsBeforeFailure = -1;
        }
        allocationsBeforeFailure = -1;
        const DRW_WritingContext* context = failedCodec.getWritingContext();
        if (wrote || failedAllocations == failuresBefore
            || !sourceUnchanged(object) || context == nullptr
            || context->sourceHandleToWrittenObjectMap.count(object.handle)
                != 0u) {
            std::cerr << "PurgePreventer allocation rollback failed at "
                      << (binary ? "Binary" : "ASCII")
                      << " offset=" << offset << " wrote=" << wrote
                      << " failed=" << (failedAllocations != failuresBefore)
                      << " identityAbsent="
                      << (context != nullptr
                          && context->sourceHandleToWrittenObjectMap.count(
                                 object.handle) == 0u)
                      << '\n';
            return false;
        }
    }
    std::cout << (binary ? "Binary" : "ASCII")
              << " PurgePreventer allocation offsets=" << allocationCount
              << "; source identities roll back on every refusal\n";
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
    if (void* value = std::malloc(size == 0u ? 1u : size))
        return value;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) { return ::operator new(size); }
void operator delete(void* value) noexcept { std::free(value); }
void operator delete[](void* value) noexcept { std::free(value); }
void operator delete(void* value, std::size_t) noexcept { std::free(value); }
void operator delete[](void* value, std::size_t) noexcept { std::free(value); }

int main() {
    return allocationSweep(false) && allocationSweep(true) ? 0 : 1;
}
