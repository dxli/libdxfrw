#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <limits>
#include <vector>
#include <utility>

#include "intern/dwgbuffer.h"
#include "intern/dwgbufferw.h"
#include "intern/dwg_fixed_handles.h"
#include "intern/dwgreader.h"

namespace {
void require(bool ok, const char* text) {
    if (!ok) { std::cerr << text << '\n'; std::exit(1); }
}
void put(dwgBufferW& w, unsigned code, std::uint64_t value) {
    dwgHandle h;
    h.code = static_cast<std::uint8_t>(code);
    h.ref = static_cast<std::uint32_t>(value);
    h.ref64 = value;
    w.putHandle(h);
}
struct Frame {
    std::vector<std::uint8_t> bytes;
    std::size_t reactorBit;
};
Frame frame(bool dictionary, bool aligned, unsigned count, unsigned reactorCode = DRW::DwgSoftPointer,
            std::uint64_t reactorRef = 0x40,
            unsigned declaredCount = std::numeric_limits<unsigned>::max()) {
    dwgBufferW w;
    w.putObjType(DRW::AC1021, DRW::DwgBlockControlObjectType);
    w.putRawLong32(0);
    put(w, 0, 0xf0);
    w.putBitShort(0);
    w.putBitLong(declaredCount == std::numeric_limits<unsigned>::max()
                     ? count : declaredCount);
    w.putBit(dictionary ? 0 : 1);
    w.putBitLong(1);
    w.putBit(0);
    if (aligned) w.alignToByte();
    auto boundary = w.bitCount();
    put(w, DRW::DwgSoftPointer, 0);
    auto reactorBit = w.bitCount();
    for (unsigned i = 0; i < count; ++i) put(w, reactorCode, reactorRef + i);
    if (dictionary) put(w, DRW::DwgHardOwnership, 0x42);
    put(w, DRW::DwgSoftOwnership, 0x43);
    put(w, DRW::DwgHardOwnership, DRW::DwgModelSpaceBlockRecordHandle);
    put(w, DRW::DwgHardOwnership, DRW::DwgPaperSpaceBlockRecordHandle);
    w.alignToByte();
    w.patchRawLong32AtBit(10u, static_cast<std::uint32_t>(boundary));
    return {w.data(), static_cast<std::size_t>(reactorBit)};
}
bool parse(DRW_ObjControl& control, const Frame& f) {
    dwgBuffer b(f.bytes.data(), f.bytes.size());
    return control.parseDwg(DRW::AC1021, &b, 0);
}
void assertReset(const DRW_ObjControl& c) {
    require(c.reactorCount() == 0 && c.reactorHandles.empty() && c.orderedHandleSlots.empty()
            && c.handlesList.empty() && c.phantomHandles.empty() && c.xDictHandle == 0,
            "failure did not reset control");
}
void replaceByte(Frame& f, std::size_t bit, std::uint8_t value) {
    for (unsigned i = 0; i < 8; ++i) {
        const auto at = bit + i;
        const auto mask = static_cast<std::uint8_t>(1u << (7u - (at % 8u)));
        if ((value & (0x80u >> i)) != 0) f.bytes.at(at / 8u) |= mask;
        else f.bytes.at(at / 8u) &= static_cast<std::uint8_t>(~mask);
    }
}
}
int main() {
    DRW_ObjControl c;
    for (bool dictionary : {false, true}) for (bool aligned : {false, true}) {
        auto good = frame(dictionary, aligned, 2);
        require(parse(c, good), "valid reactor frame rejected");
        require(c.reactorCount() == 2 && c.reactorHandles == std::vector<std::uint32_t>({0x40, 0x41}), "reactor order lost");
        require(c.xDictHandle == (dictionary ? 0x42u : 0u), "dictionary role lost");
        require(c.orderedHandleSlots == std::vector<std::uint32_t>({0x43, DRW::DwgModelSpaceBlockRecordHandle, DRW::DwgPaperSpaceBlockRecordHandle}), "entry order lost");
        require(!parse(c, frame(dictionary, aligned, 0, DRW::DwgSoftPointer,
                                0x40, 1000000)),
                "oversized reactor count accepted");
        assertReset(c);
        require(parse(c, good), "retry after oversized count failed");
        auto bad = good;
        replaceByte(bad, bad.reactorBit, 0x4f);
        require(!parse(c, bad), "oversized reactor handle accepted");
        assertReset(c);
        bad = good;
        bad.bytes.pop_back();
        require(!parse(c, bad), "truncated frame accepted");
        assertReset(c);
        require(!parse(c, frame(dictionary, aligned, 2, DRW::DwgHardOwnership)), "wrong reactor role accepted");
        assertReset(c);
        require(!parse(c, frame(dictionary, aligned, 2, DRW::DwgSoftPointer, UINT64_C(0x100000040))), "wide reactor reference truncated");
        assertReset(c);
        require(parse(c, good), "same-control retry failed");
    }
    for (const auto relative : std::vector<std::pair<unsigned, std::uint64_t>>{
             {6, 0}, {8, 0}, {10, 0x10}, {12, 0x10}}) {
        require(parse(c, frame(false, false, 1, relative.first, relative.second)),
                "documented relative soft-pointer reactor rejected");
        const std::uint32_t expected = relative.first == 6 ? 0xf1u
            : relative.first == 8 ? 0xefu : relative.first == 10 ? 0x100u : 0xe0u;
        require(c.reactorHandles == std::vector<std::uint32_t>({expected}),
                "relative reactor resolved incorrectly");
    }
    require(parse(c, frame(false, false, 0)), "zero-reactor frame rejected");
    require(c.reactorHandles.empty() && c.reactorCount() == 0, "zero-reactor state dirty");
    return 0;
}
