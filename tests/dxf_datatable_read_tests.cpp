// Generated DXF-only DATATABLE projection controls; input files live in temp.
#include "lc3_compat_check.cpp"
#include "libdxfrw.h"
#include "intern/dxfwriter.h"

#include <chrono>
#include <filesystem>
#include <fstream>
#include <memory>
#include <vector>

namespace {
struct Reader final : LC3CompatCheck {
    void addDataTable(const DRW_DataTable& value) override { tables.push_back(value); }
    void addRawDxfObject(const DRW_RawDxfObject& value) override {
        if (value.name == "DATATABLE") raw.push_back(value);
    }
    std::vector<DRW_DataTable> tables;
    std::vector<DRW_RawDxfObject> raw;
};

bool writeInput(const std::filesystem::path& path, bool binary,
                unsigned referenceCount = 2u,
                bool unsupportedColumnType = false) {
    std::ofstream file(path, std::ios::binary | std::ios::trunc);
    if (!file) return false;
    if (binary) {
        const char sentinel[] = "AutoCAD Binary DXF\r\n\x1a";
        file.write(sentinel, sizeof(sentinel));
    }
    std::unique_ptr<dxfWriter> writer = binary
        ? std::unique_ptr<dxfWriter>(new dxfWriterBinary(&file))
        : std::unique_ptr<dxfWriter>(new dxfWriterAscii(&file));
    const auto str = [&](int code, const char* value) {
        return writer->writeString(code, value);
    };
    const auto integer = [&](int code, int value) {
        return code == 70 ? writer->writeInt16(code, value)
                          : writer->writeInt32(code, value);
    };
    if (!str(0, "SECTION") || !str(2, "HEADER")
        || !str(9, "$ACADVER") || !str(1, "AC1024") || !str(0, "ENDSEC")
        || !str(0, "SECTION") || !str(2, "OBJECTS")
        || !str(0, "DATATABLE") || !str(5, "115") || !str(330, "114")
        || !str(100, "AcDbDataTable") || !integer(70, 2)
        || !integer(90, 4) || !integer(91, 2)
        || !str(1, "TABLE_NAME") || !integer(92, unsupportedColumnType ? 77 : 2) || !str(2, "DOUBLE")
        || !writer->writeDouble(40, 1.25) || !writer->writeDouble(40, 2.5)
        || !integer(92, 1) || !str(2, "INTEGER")
        || !integer(93, 7) || !integer(93, 8)
        || !integer(92, 6) || !str(2, "TEXT") || !str(3, "alpha")
        || !str(3, "beta") || !integer(92, 1) || !str(2, "INTEGER_2")
        || !integer(93, -3) || !integer(93, 4)
        || (referenceCount >= 1u && !str(360, "116")))
        return false;
    if (referenceCount >= 2u && !str(360, "117")) return false;
    return str(0, "ENDSEC") && str(0, "EOF") && !writer->hasWriteError();
}

bool verify(const DRW_DataTable& table, bool expectHandles) {
    return table.tType == DRW::DATATABLE && table.handle == 0x115u
        && table.parentHandle == 0x114u && table.flags == 2
        && table.columnCount == 4 && table.rowCount == 2
        && table.tableName == "TABLE_NAME" && table.columns.size() == 4u
        && table.columns[0].type == 2 && table.columns[0].text == "DOUBLE"
        && table.columns[0].rows.size() == 2u
        && table.columns[0].rows[0].dataDouble == 1.25
        && table.columns[0].rows[1].dataDouble == 2.5
        && table.columns[1].type == 1 && table.columns[1].text == "INTEGER"
        && table.columns[1].rows.size() == 2u
        && table.columns[1].rows[0].dataLong == 7
        && table.columns[1].rows[1].dataLong == 8
        && table.columns[2].type == 6 && table.columns[2].text == "TEXT"
        && table.columns[2].rows.size() == 2u
        && table.columns[2].rows[0].dataString == "alpha"
        && table.columns[2].rows[1].dataString == "beta"
        && table.columns[3].type == 1 && table.columns[3].text == "INTEGER_2"
        && table.columns[3].rows.size() == 2u
        && table.columns[3].rows[0].dataLong == -3
        && table.columns[3].rows[1].dataLong == 4
        && table.rowHandles.size() == (expectHandles ? 2u : 1u)
        && table.rowHandles[0] == 0x116u
        && (!expectHandles || table.rowHandles[1] == 0x117u);
}
}

int main() {
    const auto path = std::filesystem::temp_directory_path()
        / ("dxf-datatable-" + std::to_string(
            std::chrono::steady_clock::now().time_since_epoch().count()) + ".dxf");
    struct Cleanup {
        std::filesystem::path path;
        ~Cleanup() { std::error_code ec; std::filesystem::remove(path, ec); }
    } cleanup{path};

    for (const bool binary : {false, true}) {
        dxfRW reader(path.string().c_str());
        if (!writeInput(path, binary)) return 1;
        Reader result;
        if (!reader.read(&result, false) || result.tables.size() != 1u
            || result.raw.size() != 1u || !verify(result.tables.front(), true))
            return 2;

        // Incomplete row references remain available through raw-object
        // preservation but do not publish an incomplete typed graph node.
        if (!writeInput(path, binary, 1u)) return 3;
        Reader incomplete;
        if (!reader.read(&incomplete, false) || !incomplete.tables.empty()
            || incomplete.raw.size() != 1u)
            return 4;

        if (!writeInput(path, binary, 0u)) return 5;
        Reader missingReferences;
        if (!reader.read(&missingReferences, false)
            || !missingReferences.tables.empty()
            || missingReferences.raw.size() != 1u)
            return 6;

        if (!writeInput(path, binary, 2u, true)) return 7;
        Reader unknownColumn;
        if (!reader.read(&unknownColumn, false) || !unknownColumn.tables.empty()
            || unknownColumn.raw.size() != 1u)
            return 8;
    }
    return 0;
}
