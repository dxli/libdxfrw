#include <chrono>
#include <cstdlib>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <memory>
#include <new>
#include <sstream>
#include <string>
#include <vector>

#include "drw_base.h"
#include "drw_header.h"
#include "libdxfrw.h"
#include "dx_iface.h"
#include "intern/dxfreader.h"
#include "intern/dxfwriter.h"

namespace {
long xdataAllocationFailureOffset = -1;
bool xdataAllocationFailed = false;
}

void* operator new(std::size_t size) {
    if (xdataAllocationFailureOffset >= 0
        && xdataAllocationFailureOffset-- == 0) {
        xdataAllocationFailed = true;
        throw std::bad_alloc();
    }
    if (void* memory = std::malloc(size == 0 ? 1 : size))
        return memory;
    throw std::bad_alloc();
}

void* operator new[](std::size_t size) {
    if (xdataAllocationFailureOffset >= 0
        && xdataAllocationFailureOffset-- == 0) {
        xdataAllocationFailed = true;
        throw std::bad_alloc();
    }
    if (void* memory = std::malloc(size == 0 ? 1 : size))
        return memory;
    throw std::bad_alloc();
}

void operator delete(void* memory) noexcept { std::free(memory); }
void operator delete[](void* memory) noexcept { std::free(memory); }
void operator delete(void* memory, std::size_t) noexcept { std::free(memory); }
void operator delete[](void* memory, std::size_t) noexcept { std::free(memory); }

class DxfXdataInt16TestAccess {
public:
    static void setup(dxfRW& codec, bool binary, std::ostringstream& output) {
        codec.version = DRW::AC1027;
        codec.binFile = binary;
        codec.writer = binary
            ? std::unique_ptr<dxfWriter>(std::make_unique<dxfWriterBinary>(&output))
            : std::unique_ptr<dxfWriter>(std::make_unique<dxfWriterAscii>(&output));
    }
    static bool writeXdata(dxfRW& codec, const std::vector<DRW_Variant*>& values) {
        return codec.writeExtData(values);
    }
    static bool preflightXdata(dxfRW& codec,
                               const std::vector<DRW_Variant*>& values) {
        return codec.preflightDxfXdataAppIds(values);
    }
    static void addRequiredAppId(dxfRW& codec, const std::string& value) {
        codec.m_requiredDxfXdataAppIds.insert(value);
    }
    static bool hasRequiredAppId(const dxfRW& codec, const std::string& value) {
        return codec.m_requiredDxfXdataAppIds.count(value) == 1u;
    }
    static std::size_t requiredAppIdCount(const dxfRW& codec) {
        return codec.m_requiredDxfXdataAppIds.size();
    }
};

namespace {

bool fail(const char* label) {
    std::cerr << "XDATA group 1070: " << label << '\n';
    return false;
}

bool directTypedControls() {
    for (const bool binary : {false, true}) {
        for (const int value : {-32769, -32768, -1, 0, 32767, 32768, 40000, 65535, 65536}) {
            std::ostringstream output;
            dxfRW codec("");
            DxfXdataInt16TestAccess::setup(codec, binary, output);
            DRW_Variant app(1001, std::string("ACAD"));
            DRW_Variant number(1070, value);
            const bool wrote = DxfXdataInt16TestAccess::writeXdata(
                codec, std::vector<DRW_Variant*>{&app, &number});
            if (value < -32768 || value > 65535 || (binary && value > 32767)) {
                if (wrote || !output.str().empty() || number.i_val() != value)
                    return fail("lossy Binary XDATA was not refused before the first group");
                continue;
            }
            if (!wrote) return fail("representable XDATA was refused");
            std::istringstream input(output.str());
            std::unique_ptr<dxfReader> reader = binary
                ? std::unique_ptr<dxfReader>(std::make_unique<dxfReaderBinary>(&input))
                : std::unique_ptr<dxfReader>(std::make_unique<dxfReaderAscii>(&input));
            int code = 0;
            if (!reader->readRec(&code) || code != 1001
                || !reader->readRec(&code) || code != 1070
                || reader->getInt32() != value)
                return fail("representable XDATA did not read back numerically");
        }
    }
    // The untyped writer is an explicit bit-pattern route. 40000 and -25536
    // are the same 16 bits; Binary readers interpret those bits as signed.
    std::ostringstream rawBytes;
    dxfWriterBinary rawWriter(&rawBytes);
    if (!rawWriter.writeInt16(1070, 40000))
        return fail("low-level raw-bit writer was narrowed");
    std::istringstream rawInput(rawBytes.str());
    dxfReaderBinary rawReader(&rawInput);
    int code = 0;
    if (!rawReader.readRec(&code) || code != 1070
        || rawReader.getInt32() != -25536)
        return fail("low-level raw-bit interpretation changed");
    std::ostringstream opaqueBytes;
    dxfRW opaqueCodec("");
    DxfXdataInt16TestAccess::setup(opaqueCodec, true, opaqueBytes);
    DRW_RawDxfObject opaque;
    opaque.name = "LOCAL_RAW_XDATA";
    opaque.m_version = DRW::AC1027;
    opaque.groups.emplace_back(5, std::string("A91"));
    opaque.groups.emplace_back(1001, std::string("ACAD"));
    opaque.groups.emplace_back(1070, 40000);
    if (!opaqueCodec.writeRawDxfObject(&opaque))
        return fail("opaque raw-object bit-pattern output was narrowed");
    std::istringstream opaqueInput(opaqueBytes.str());
    dxfReaderBinary opaqueReader(&opaqueInput);
    bool found = false;
    while (opaqueReader.readRec(&code)) {
        if (code == 1070) {
            if (opaqueReader.getInt32() != -25536)
                return fail("opaque raw-object bit pattern changed");
            found = true;
        }
    }
    if (!found) return fail("opaque raw-object 1070 was omitted");
    return true;
}

bool appIdAllocationRollbackControls() {
    unsigned reached = 0u;
    bool completed = false;
    DRW_Variant first(1001, std::string("APP_ONE"));
    DRW_Variant firstValue(1000, std::string("one"));
    DRW_Variant second(1001, std::string("APP_TWO"));
    DRW_Variant secondValue(1000, std::string("two"));
    const std::vector<DRW_Variant*> values{
        &first, &firstValue, &second, &secondValue};
    for (long offset = 0; offset < 128; ++offset) {
        std::ostringstream output;
        dxfRW codec("");
        DxfXdataInt16TestAccess::setup(codec, false, output);
        DxfXdataInt16TestAccess::addRequiredAppId(codec, "EXISTING");
        xdataAllocationFailed = false;
        xdataAllocationFailureOffset = offset;
        bool accepted = false;
        try {
            accepted = DxfXdataInt16TestAccess::preflightXdata(codec, values);
        } catch (...) {
            xdataAllocationFailureOffset = -1;
            return fail("APPID admission leaked an allocation exception");
        }
        const bool injected = xdataAllocationFailed;
        xdataAllocationFailureOffset = -1;
        if (accepted) {
            if (DxfXdataInt16TestAccess::requiredAppIdCount(codec) != 3u
                || !DxfXdataInt16TestAccess::hasRequiredAppId(codec, "EXISTING")
                || !DxfXdataInt16TestAccess::hasRequiredAppId(codec, "APP_ONE")
                || !DxfXdataInt16TestAccess::hasRequiredAppId(codec, "APP_TWO"))
                return fail("successful APPID admission lost a requirement");
            completed = true;
            break;
        }
        if (!injected)
            return fail("APPID admission refused without injected allocation failure");
        ++reached;
        if (DxfXdataInt16TestAccess::requiredAppIdCount(codec) != 1u
            || !DxfXdataInt16TestAccess::hasRequiredAppId(codec, "EXISTING")
            || DxfXdataInt16TestAccess::hasRequiredAppId(codec, "APP_ONE")
            || DxfXdataInt16TestAccess::hasRequiredAppId(codec, "APP_TWO"))
            return fail("allocation failure left a partial APPID requirement set");
        std::ostringstream retryOutput;
        dxfRW retryCodec("");
        DxfXdataInt16TestAccess::setup(retryCodec, false, retryOutput);
        DxfXdataInt16TestAccess::addRequiredAppId(retryCodec, "EXISTING");
        if (!DxfXdataInt16TestAccess::preflightXdata(retryCodec, values)
            || DxfXdataInt16TestAccess::requiredAppIdCount(retryCodec) != 3u)
            return fail("fresh APPID admission retry did not recover");
    }
    if (!completed || reached == 0u)
        return fail("APPID allocation sweep did not reach failure and success");
    std::cout << "XDATA APPID allocation failures rolled back=" << reached
              << " before successful admission\n";
    return true;
}

class SelectedWriter final : public dx_iface {
public:
    SelectedWriter(dxfRW& writer, bool xrecord, int value)
        : xrecord(xrecord) {
        dxfW = &writer;
        cData = &storage;
        currentBlock = storage.mBlock;
        line.basePoint = DRW_Coord(1, 2, 3);
        line.secPoint = DRW_Coord(4, 5, 6);
        line.extData.push_back(std::make_shared<DRW_Variant>(1001, std::string("ACAD")));
        line.extData.push_back(std::make_shared<DRW_Variant>(1070, value));
        record.handle = 0xA91u;
        record.parentHandle = 0xCu;
        record.m_values.emplace_back(1070, value);
    }
    void writeEntities() override {
        if (!xrecord) wrote = dxfW->writeLine(&line);
    }
    void writeObjects() override {
        if (xrecord) wrote = dxfW->writeXRecordDwgCompatibility(&record);
    }
    bool finalizeDxfWrite() override { return wrote; }
    void setValue(int value) {
        line.extData.back()->addInt(1070, value);
        record.m_values.front().addInt(1070, value);
        wrote = false;
    }
    dx_data storage;
    DRW_Line line;
    DRW_XRecord record;
    bool xrecord{false};
    bool wrote{false};
};

class SelectedReader final : public dx_iface {
public:
    void addLine(const DRW_Line& value) override {
        if (!value.extData.empty()) {
            for (const auto& item : value.extData)
                if (item && item->code() == 1070 && item->type() == DRW_Variant::INTEGER) {
                    number = item->i_val();
                    ++count;
                }
        }
    }
    void addXRecord(const DRW_XRecord& value) override {
        const auto& values = value.m_dataEntries.empty() ? value.m_values : value.m_dataEntries;
        for (const auto& item : values)
            if (item.code() == 1070 && item.type() == DRW_Variant::INTEGER) {
                number = item.i_val();
                ++count;
            }
    }
    int number{0};
    unsigned count{0};
};

bool wholeFileControls() {
    const auto nonce = std::chrono::steady_clock::now().time_since_epoch().count();
    for (const bool xrecord : {false, true}) {
        for (const bool binary : {false, true}) {
            const auto path = std::filesystem::temp_directory_path()
                / ("libdxfrw-xdata-1070-" + std::to_string(nonce)
                   + (xrecord ? "-object" : "-entity")
                   + (binary ? "-binary.dxf" : "-ascii.dxf"));
            struct Cleanup { std::filesystem::path path; ~Cleanup() {
                std::error_code ignored; std::filesystem::remove(path, ignored);
            }} cleanup{path};
            const std::string sentinel = "PRIOR_DESTINATION";
            { std::ofstream out(path, std::ios::binary); out << sentinel; }
            dxfRW writer(path.string().c_str());
            SelectedWriter selected(writer, xrecord, 40000);
            const bool first = writer.write(&selected, DRW::AC1027, binary);
            int expected = 40000;
            if (binary) {
                std::ifstream old(path, std::ios::binary);
                const std::string preserved{std::istreambuf_iterator<char>{old},
                                            std::istreambuf_iterator<char>{}};
                if (first || preserved != sentinel
                    || selected.line.extData.back()->i_val() != 40000
                    || selected.record.m_values.front().i_val() != 40000)
                    return fail("Binary output failure did not preserve destination and caller state");
                expected = 32767;
                selected.setValue(expected);
                if (!writer.write(&selected, DRW::AC1027, binary))
                    return fail("same-codec corrected retry failed");
            } else {
                if (!first)
                    return fail("ASCII unsigned 1070 was refused");
                std::ifstream before(path, std::ios::binary);
                const std::string prior{std::istreambuf_iterator<char>{before},
                                        std::istreambuf_iterator<char>{}};
                selected.setValue(65536);
                const bool rejected = !writer.write(&selected, DRW::AC1027, false);
                std::ifstream after(path, std::ios::binary);
                const std::string preserved{std::istreambuf_iterator<char>{after},
                                            std::istreambuf_iterator<char>{}};
                if (!rejected || preserved != prior
                    || selected.line.extData.back()->i_val() != 65536
                    || selected.record.m_values.front().i_val() != 65536)
                    return fail("ASCII out-of-range 1070 did not roll back");
                selected.setValue(expected);
                if (!writer.write(&selected, DRW::AC1027, false))
                    return fail("ASCII same-codec corrected retry failed");
            }
            dx_data data;
            SelectedReader reader;
            if (!reader.fileImport(path.string(), &data, false)
                || reader.count != 1u || reader.number != expected)
                return fail("whole-file typed readback lost the numeric value");
        }
    }
    return true;
}

} // namespace

int main() {
    if (!directTypedControls() || !wholeFileControls()) return 1;
    if (!appIdAllocationRollbackControls()) return 1;
    std::cout << "Typed XDATA 1070 Binary exact numeric policy: PASS\n";
    return 0;
}
