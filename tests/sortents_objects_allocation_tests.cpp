// SPDX-License-Identifier: GPL-2.0-or-later
// Generated SORTENTSTABLE OBJECTS allocation rollback/retry control.
#include "lc3_compat_check.cpp"
#include "libdxfrw.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <string>
#include <utility>
#include <vector>

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0u;

struct AllocationFailure {
    explicit AllocationFailure(long count) {
        allocationsBeforeFailure = count;
    }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

DRW_SortEntsTable makeSortTable() {
    DRW_SortEntsTable table;
    table.handle = 0xD720u;
    table.parentHandle = DRW::DwgNamedObjectsDictionaryHandle;
    table.m_blockOwnerHandle = DRW::DxfModelSpaceBlockRecordHandle;
    table.m_entityHandles = {0xD731u, 0xD732u, 0xD733u, 0xD734u};
    table.m_sortHandles = {0xA101u, 0xA202u, 0xA303u, 0xA404u};
    table.extData.push_back(new DRW_Variant(1001, std::string("ACAD")));
    table.extData.push_back(new DRW_Variant(
        1000, std::string("sortents-allocation-retry")));
    return table;
}

class Export final : public LC3CompatCheck {
public:
    explicit Export(dxfRW& writer) : writer(writer), table(makeSortTable()) {}

    void writeObjects() override {
        if (failAt < 0) {
            accepted = writer.writeSortEntsTable(&table);
        } else {
            const std::size_t before = failedAllocations;
            AllocationFailure failure(failAt);
            try {
                accepted = writer.writeSortEntsTable(&table);
            } catch (const std::bad_alloc&) {
                accepted = false;
            }
            reached = failedAllocations != before
                ? static_cast<long>(failedAllocations - before)
                : failAt - allocationsBeforeFailure;
        }
        if (!accepted)
            (void)writer.writeRawDxfObject(nullptr);
    }

    void writeEntities() override {
        for (std::size_t i = 0; i < table.m_entityHandles.size(); ++i) {
            DRW_Line line;
            line.handle = table.m_entityHandles[i];
            line.parentHandle = DRW::DxfModelSpaceBlockRecordHandle;
            line.basePoint = DRW_Coord(static_cast<double>(i), 1.0, 0.0);
            line.secPoint = DRW_Coord(static_cast<double>(i) + 1.0, 2.0, 0.0);
            if (!writer.writeLine(&line))
                accepted = false;
        }
    }

    bool finalizeDxfWrite() override { return accepted; }

    dxfRW& writer;
    DRW_SortEntsTable table;
    long failAt = -1;
    long reached = 0;
    bool accepted = false;
};

class Reader final : public LC3CompatCheck {
public:
    void addLine(const DRW_Line& line) override {
        lines.emplace_back(line.handle, line.parentHandle);
    }
    void addSortEntsTable(const DRW_SortEntsTable& table) override {
        tables.push_back(table);
    }
    std::vector<std::pair<std::uint32_t, std::uint32_t>> lines;
    std::vector<DRW_SortEntsTable> tables;
};

bool readBytes(const std::filesystem::path& path, std::string& bytes) {
    std::ifstream input(path, std::ios::binary);
    if (!input)
        return false;
    bytes.assign(std::istreambuf_iterator<char>{input},
                 std::istreambuf_iterator<char>{});
    return !input.bad();
}

bool readbackMatches(const std::filesystem::path& path) {
    dxfRW readerCodec(path.string().c_str());
    Reader reader;
    const bool read = readerCodec.read(&reader, false);
    if (!read || reader.tables.size() != 1u || reader.lines.size() != 4u) {
        std::cerr << "SORTENTSTABLE readback census failed: read=" << read
                  << " tables=" << reader.tables.size()
                  << " lines=" << reader.lines.size() << '\n';
        return false;
    }
    const DRW_SortEntsTable& table = reader.tables.front();
    const DRW_SortEntsTable expected = makeSortTable();
    if (table.handle != expected.handle
        || table.parentHandle != expected.parentHandle
        || table.m_blockOwnerHandle != DRW::DxfModelSpaceBlockRecordHandle
        || table.m_entityHandles.size() != expected.m_entityHandles.size()
        || table.m_sortHandles != expected.m_sortHandles
        || table.extData.size() != expected.extData.size()) {
        std::cerr << "SORTENTSTABLE field mismatch: handle=" << table.handle
                  << " parent=" << table.parentHandle
                  << " owner=" << table.m_blockOwnerHandle
                  << " pairs=" << table.m_entityHandles.size() << "/"
                  << table.m_sortHandles.size()
                  << " EED=" << table.extData.size()
                  << " entityCountMatch="
                  << (table.m_entityHandles.size()
                      == expected.m_entityHandles.size())
                  << " sortMatch="
                  << (table.m_sortHandles == expected.m_sortHandles)
                  << " parentMatch="
                  << (table.parentHandle == expected.parentHandle) << '\n';
        return false;
    }
    for (std::size_t i = 0; i < expected.m_entityHandles.size(); ++i) {
        if (table.m_entityHandles[i] != reader.lines[i].first
            || reader.lines[i].second != DRW::DxfModelSpaceBlockRecordHandle) {
            std::cerr << "SORTENTSTABLE target line mismatch at " << i
                      << ": handle=" << reader.lines[i].first
                      << " tableTarget=" << table.m_entityHandles[i]
                      << " owner=" << reader.lines[i].second << '\n';
            return false;
        }
    }
    for (std::size_t i = 0; i < expected.extData.size(); ++i) {
        if (!table.extData[i] || !expected.extData[i]
            || !table.extData[i]->content.s || !expected.extData[i]->content.s
            || table.extData[i]->code() != expected.extData[i]->code()
            || *table.extData[i]->content.s != *expected.extData[i]->content.s)
            return false;
    }
    return true;
}

bool sourceMatches(const DRW_SortEntsTable& actual) {
    const DRW_SortEntsTable expected = makeSortTable();
    if (actual.handle != expected.handle
        || actual.parentHandle != expected.parentHandle
        || actual.m_blockOwnerHandle != expected.m_blockOwnerHandle
        || actual.m_entityHandles != expected.m_entityHandles
        || actual.m_sortHandles != expected.m_sortHandles
        || actual.extData.size() != expected.extData.size())
        return false;
    for (std::size_t i = 0; i < expected.extData.size(); ++i) {
        if (!actual.extData[i] || !expected.extData[i]
            || !actual.extData[i]->content.s || !expected.extData[i]->content.s
            || actual.extData[i]->code() != expected.extData[i]->code()
            || *actual.extData[i]->content.s != *expected.extData[i]->content.s)
            return false;
    }
    return true;
}

bool allocationSweep(const std::filesystem::path& directory) {
    const auto path = directory / "sortents-allocation.dxf";
    const std::string prior = "prior SORTENTSTABLE destination\n";
    for (const bool binary : {false, true}) {
        long allocationCount = 0;
        std::string expectedBytes;
        {
            dxfRW writer(path.string().c_str());
            Export exporter(writer);
            exporter.failAt = 1000000;
            const bool written = writer.write(&exporter, DRW::AC1027, binary);
            if (!written || !exporter.accepted || !sourceMatches(exporter.table)
                || !readbackMatches(path) || !readBytes(path, expectedBytes)) {
                std::cerr << "clean SORTENTSTABLE write failed: write=" << written
                          << " accepted=" << exporter.accepted
                          << " source=" << sourceMatches(exporter.table)
                          << " readback=" << readbackMatches(path) << '\n';
                return false;
            }
            allocationCount = exporter.reached;
        }
        if (allocationCount <= 0)
            return false;

        for (long offset = 0; offset < allocationCount; ++offset) {
            {
                std::ofstream output(path, std::ios::binary | std::ios::trunc);
                output << prior;
                if (!output)
                    return false;
            }
            dxfRW writer(path.string().c_str());
            Export exporter(writer);
            exporter.failAt = offset;
            const bool written = writer.write(&exporter, DRW::AC1027, binary);
            exporter.failAt = -1;
            std::string current;
            if (written || exporter.accepted || exporter.reached != 1
                || !readBytes(path, current) || current != prior
                || !sourceMatches(exporter.table))
                return false;

            const bool retried = writer.write(&exporter, DRW::AC1027, binary);
            std::string retryBytes;
            if (!retried || !exporter.accepted
                || !sourceMatches(exporter.table)
                || !readBytes(path, retryBytes)
                || retryBytes != expectedBytes || !readbackMatches(path))
                return false;
        }
        std::cout << (binary ? "Binary" : "ASCII")
                  << " SORTENTSTABLE writer allocation offsets="
                  << allocationCount << '\n';
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
    const auto directory = std::filesystem::temp_directory_path()
        / ("sortents-objects-allocation-"
           + std::to_string(static_cast<long long>(
                 std::filesystem::file_time_type::clock::now()
                     .time_since_epoch().count())));
    if (!std::filesystem::create_directories(directory)
        && !std::filesystem::is_directory(directory))
        return 1;
    struct Cleanup {
        std::filesystem::path path;
        ~Cleanup() {
            std::error_code error;
            std::filesystem::remove_all(path, error);
        }
    } cleanup{directory};
    return allocationSweep(directory) ? 0 : 2;
}
