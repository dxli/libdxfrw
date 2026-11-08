// SPDX-License-Identifier: GPL-2.0-or-later
// Generated AC1018 TABLE body/handle codec allocation rollback and retry.
#include "drw_entities.h"
#include "intern/dwg_fixed_handles.h"
#include "intern/dwgbufferw.h"

#include <cstdlib>
#include <iostream>
#include <new>
#include <string>
#include <vector>

namespace {
bool trackAllocations = false;
long allocationsBeforeFailure = -1;
std::size_t allocationAttempts = 0;
std::size_t failedAllocations = 0;

class AllocationFailure final {
public:
    explicit AllocationFailure(long offset) {
        allocationAttempts = 0;
        allocationsBeforeFailure = offset;
        trackAllocations = true;
    }
    ~AllocationFailure() {
        trackAllocations = false;
        allocationsBeforeFailure = -1;
    }
};

class TableProbe final : public DRW_Table {
public:
    using DRW_Table::parseDwg;

    bool encode(dwgBufferW& body, dwgBufferW& handles) {
        oType = 562u; // generated dispatch value; not a production class ordinal
        return encodeDwg(DRW::AC1018, &body, 0u, nullptr, &handles);
    }
};

TableProbe makeTable() {
    TableProbe table;
    table.handle = 0xA500u;
    table.setDwgLayerHandle(DRW::DwgLayer0Handle);
    table.blockRecH.ref = 0xA600u;
    table.m_tableStyleHandle = 0xA700u;
    table.basePoint = DRW_Coord{4.0, 5.0, 0.0};
    table.extPoint = DRW_Coord{0.0, 0.0, 1.0};
    table.m_horizontalDirection = DRW_Coord{1.0, 0.0, 0.0};

    DRW_TableColumn column;
    column.m_width = 8.0;
    table.m_content.m_columns.push_back(column);

    DRW_TableRow row;
    row.m_height = 3.0;
    DRW_TableCell cell;
    cell.m_type = 1;
    cell.m_mergedWidth = 1;
    cell.m_mergedHeight = 1;
    DRW_TableCellContent content;
    content.m_type = 1;
    content.m_text = "generated allocation retry";
    cell.m_contents.push_back(content);
    row.m_cells.push_back(cell);
    table.m_content.m_rows.push_back(row);
    return table;
}

bool seed(dwgBufferW& body, dwgBufferW& handles) {
    body.putRawChar8(0xA5u);
    handles.putRawChar8(0x5Au);
    return body.isGood() && handles.isGood();
}

bool run() {
    TableProbe calibration = makeTable();
    dwgBufferW calibrationBody;
    dwgBufferW calibrationHandles;
    if (!seed(calibrationBody, calibrationHandles))
        return false;
    bool calibrationResult = false;
    {
        AllocationFailure measure(-1);
        calibrationResult = calibration.encode(
            calibrationBody, calibrationHandles);
    }
    const std::size_t allocationCount = allocationAttempts;
    if (!calibrationResult || allocationCount == 0u)
        return false;

    std::size_t exercised = 0u;
    for (std::size_t offset = 0u; offset < allocationCount; ++offset) {
        TableProbe table = makeTable();
        dwgBufferW body;
        dwgBufferW handles;
        if (!seed(body, handles))
            return false;
        const std::vector<std::uint8_t> bodyBefore = body.data();
        const std::vector<std::uint8_t> handlesBefore = handles.data();
        const std::size_t bodyBitsBefore = body.bitCount();
        const std::size_t handleBitsBefore = handles.bitCount();
        const std::size_t failuresBefore = failedAllocations;
        bool encoded = true;
        {
            AllocationFailure failure(static_cast<long>(offset));
            encoded = table.encode(body, handles);
            if (failedAllocations != failuresBefore + 1u
                || allocationAttempts <= offset)
                return false;
        }
        if (encoded || body.data() != bodyBefore
            || handles.data() != handlesBefore
            || body.bitCount() != bodyBitsBefore
            || handles.bitCount() != handleBitsBefore)
            return false;
        ++exercised;

        // Retry this same table and the same output buffers after each injected
        // failure. The seeded prefixes must be preserved on successful append.
        if (!table.encode(body, handles)
            || body.data().empty() || body.data().front() != 0xA5u
            || handles.data().empty() || handles.data().front() != 0x5Au)
            return false;
    }

    std::cout << "AC1018 TABLE direct codec: allocations=" << allocationCount
              << ", injected failures=" << exercised
              << ", destination rollback and same-object retry=PASS\n";
    return exercised == allocationCount;
}
} // namespace

void* operator new(std::size_t size) {
    if (trackAllocations) {
        ++allocationAttempts;
        if (allocationsBeforeFailure == 0) {
            ++failedAllocations;
            throw std::bad_alloc();
        }
        if (allocationsBeforeFailure > 0)
            --allocationsBeforeFailure;
    }
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
    const std::size_t before = failedAllocations;
    if (!run() || failedAllocations == before) {
        std::cerr << "AC1018 TABLE allocation rollback/retry failed\n";
        return 1;
    }
    return 0;
}
