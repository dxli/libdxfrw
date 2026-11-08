// SPDX-License-Identifier: GPL-2.0-or-later
// Generated TABLECONTENT OBJECTS allocation-failure rollback and retry.
#include "lc3_compat_check.cpp"
#include "dx_iface.h"

#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <new>
#include <string>

namespace {
long allocationsBeforeFailure = -1;
std::size_t failedAllocations = 0;

struct AllocationFailure {
    explicit AllocationFailure(long count) { allocationsBeforeFailure = count; }
    ~AllocationFailure() { allocationsBeforeFailure = -1; }
};

class TableContentExport final : public LC3CompatCheck {
public:
    explicit TableContentExport(dxfRW& output) : output(output) {
        content.handle = 0x7100u;
        content.m_parseComplete = true;
        content.m_semanticContentComplete = true;
        content.m_content.m_name = "generated allocation table";
        content.m_content.m_description = "single-cell rollback control";
        DRW_TableColumn column;
        column.m_name = "Value";
        content.m_content.m_columns.push_back(column);
        DRW_TableCellContent value;
        value.m_type = 1;
        value.m_text = "allocation retry";
        value.m_value.m_dataType = 4;
        value.m_value.m_value.addString(1, value.m_text);
        DRW_TableCell cell;
        cell.m_contents.push_back(value);
        DRW_TableRow row;
        row.m_cells.push_back(cell);
        content.m_content.m_rows.push_back(row);
    }

    dxfRW& output;
    DRW_TableContentObject content;
    long failAt = -1;
    long reached = 0;
    bool emitted = false;

    void writeObjects() override {
        const auto emit = [this]() {
            emitted = output.writeTableContent(&content);
        };
        if (failAt < 0) {
            emit();
            return;
        }
        const auto before = failedAllocations;
        try {
            AllocationFailure failure(failAt);
            emit();
            reached = failedAllocations != before
                ? static_cast<long>(failedAllocations - before)
                : failAt - allocationsBeforeFailure;
        } catch (const std::bad_alloc&) {
            reached = failedAllocations != before
                ? static_cast<long>(failedAllocations - before)
                : failAt - allocationsBeforeFailure;
            throw;
        }
    }

    bool finalizeDxfWrite() override { return emitted; }
};

bool readAll(const std::filesystem::path& path, std::string& value) {
    std::ifstream input(path, std::ios::binary);
    if (!input)
        return false;
    value.assign(std::istreambuf_iterator<char>(input),
                 std::istreambuf_iterator<char>());
    return !input.bad();
}

bool write(TableContentExport& exporter, dxfRW& codec, bool binary) {
    return codec.write(&exporter, DRW::AC1027, binary);
}

bool verify(const std::filesystem::path& path) {
    dx_data data;
    dx_iface reader;
    const bool imported = reader.fileImport(path.string(), &data, false);
    if (!imported || data.tableContents.size() != 1u)
        return false;
    const DRW_TableContentObject& actual = data.tableContents.front();
    if (actual.handle != 0x7100u || !actual.m_parseComplete
        || actual.m_content.m_name != "generated allocation table"
        || actual.m_content.m_description != "single-cell rollback control"
        || actual.m_content.m_columns.size() != 1u
        || actual.m_content.m_columns.front().m_name != "Value"
        || actual.m_content.m_rows.size() != 1u
        || actual.m_content.m_rows.front().m_cells.size() != 1u)
        return false;
    const auto& contents = actual.m_content.m_rows.front().m_cells.front().m_contents;
    return contents.size() == 1u && contents.front().m_type == 1
        && contents.front().m_text == "allocation retry"
        && contents.front().m_value.m_dataType == 4
        && contents.front().m_value.m_value.type() == DRW_Variant::STRING
        && contents.front().m_value.m_value.c_str()
            == std::string("allocation retry");
}

bool sourceUnchanged(const TableContentExport& actual,
                     const TableContentExport& expected) {
    const auto& a = actual.content;
    const auto& b = expected.content;
    return a.handle == b.handle && a.m_parseComplete == b.m_parseComplete
        && a.m_content.m_name == b.m_content.m_name
        && a.m_content.m_description == b.m_content.m_description
        && a.m_content.m_columns.size() == b.m_content.m_columns.size()
        && a.m_content.m_columns.front().m_name
            == b.m_content.m_columns.front().m_name
        && a.m_content.m_rows.size() == b.m_content.m_rows.size()
        && a.m_content.m_rows.front().m_cells.front().m_contents.front().m_text
            == b.m_content.m_rows.front().m_cells.front().m_contents.front().m_text;
}

bool run(const std::filesystem::path& directory) {
    const auto fail = [](const char* message) {
        std::cerr << "TABLECONTENT OBJECTS allocation retry: " << message << '\n';
        return false;
    };
    if (!std::filesystem::create_directories(directory)
        && !std::filesystem::is_directory(directory))
        return fail("could not create scratch directory");
    const auto output = directory / "tablecontent-objects.dxf";
    const std::string sentinel = "prior TABLECONTENT destination bytes\n";
    dxfRW codec(output.string().c_str());
    TableContentExport exporter(codec);
    TableContentExport expected(codec);

    for (const bool binary : {false, true}) {
        if (!write(exporter, codec, binary))
            return fail("warm-up export was refused");
        exporter.failAt = 1000000;
        if (!write(exporter, codec, binary))
            return fail("allocation calibration export was refused");
        if (!verify(output))
            return fail("calibration output did not read back typed content");
        const long allocationCount = exporter.reached;
        if (allocationCount <= 0)
            return fail("calibration observed no callback allocations");
        for (long offset = 0; offset < allocationCount; ++offset) {
            {
                std::ofstream prior(output, std::ios::binary | std::ios::trunc);
                prior << sentinel;
                if (!prior)
                    return fail("could not initialize destination sentinel");
            }
            exporter.failAt = offset;
            exporter.reached = 0;
            exporter.emitted = false;
            const bool wrote = write(exporter, codec, binary);
            exporter.failAt = -1;
            std::string current;
            if (wrote || exporter.reached < 1 || !readAll(output, current)
                || current != sentinel)
                return fail("injected failure changed destination or was missed");
            if (!sourceUnchanged(exporter, expected))
                return fail("injected failure changed the caller object");
            if (!write(exporter, codec, binary) || !verify(output))
                return fail("same-writer retry failed typed readback");
            if (!sourceUnchanged(exporter, expected))
                return fail("same-writer retry changed the caller object");
        }
        std::cout << "TABLECONTENT OBJECTS "
                  << (binary ? "Binary" : "ASCII")
                  << ": allocation offsets=" << allocationCount
                  << ", rollback/retry/readback=PASS\n";
    }
    std::error_code ignored;
    std::filesystem::remove(output, ignored);
    return true;
}
} // namespace

void* operator new(std::size_t size) {
    if (allocationsBeforeFailure == 0) {
        ++failedAllocations;
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
        std::cerr << "usage: tablecontent_objects_allocation_tests <scratch-dir>\n";
        return 2;
    }
    return run(argv[1]) ? 0 : 1;
}
