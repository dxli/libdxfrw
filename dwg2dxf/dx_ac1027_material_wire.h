// SPDX-License-Identifier: GPL-2.0-or-later
#ifndef DX_AC1027_MATERIAL_WIRE_H
#define DX_AC1027_MATERIAL_WIRE_H
#include "dx_raster_dwg_wire.h"
#include "drw_objects.h"
namespace dx_ac1027_material_wire {
inline bool matches(const DRW_UnsupportedObject& raw,
                    const DRW_Material& typed) noexcept {
    using namespace dx_raster_wire::detail;
    if (raw.m_version != DRW::AC1027 || raw.m_isEntity || !raw.m_isCustomClass
        || raw.m_objectType < 500 || raw.m_recordName != "MATERIAL"
        || raw.m_className != "AcDbMaterial" || raw.m_handle == 0u
        || raw.m_handle != typed.handle || raw.m_parentHandle != typed.parentHandle
        || raw.m_numReactors != 1 || raw.m_reactorHandles.size() != 1u
        || raw.m_reactorHandles != typed.reactorHandles || raw.m_xDictFlag != 1u
        || raw.m_hasDataStorage || raw.m_rawBytes.empty()
        || raw.m_rawBytes.size() > 1024u || raw.m_bodyBitSize == 0u
        || !typed.extData.empty() || !typed.appData.empty()
        || typed.xDictHandle != 0u || typed.hasDataStorageBinaryData()
        || typed.extensionDictionaryFlag() != 1u)
        return false;
    const auto total = static_cast<std::uint32_t>(raw.m_rawBytes.size() * 8u);
    if (raw.m_bodyBitSize >= total) return false;
    const auto end = total - raw.m_bodyBitSize;
    Bits body{raw.m_rawBytes.data(), 0u, end};
    const auto tag = body.take(2u);
    const auto type = static_cast<std::uint32_t>(tag == 0u ? body.take(8u)
        : tag == 1u ? body.take(8u) + 0x1f0u : body.little(2u));
    if (!body.good || type != static_cast<std::uint32_t>(raw.m_objectType)) return false;
    const auto self = handle(body, 0u, 0u, true);
    if (!body.good || self.value != raw.m_handle || body.shortValue() != 0u
        || body.longValue() != 1u || body.take(1u) != 1u
        || body.take(1u) != 0u || !body.good) return false;
    Bits handles{raw.m_rawBytes.data(), end, total};
    const auto owner = handle(handles, 4u, raw.m_handle);
    const auto reactor = handle(handles, 4u, raw.m_handle);
    if (!handles.good || owner.role != 4u || reactor.role != 4u
        || owner.value != raw.m_parentHandle || reactor.value != raw.m_parentHandle)
        return false;
    const auto tail = handles.end - handles.pos;
    return tail < 8u && handles.take(tail) == 0u && handles.good;
}
// Exact source-witnessed ACAD_MATERIAL dictionary, including all three
// soft-owner item tokens and the independently bounded UTF-16 name stream.
inline bool dictionary(const DRW_UnsupportedObject& raw, const DRW_Dictionary& typed,
                       std::uint32_t owner) noexcept {
    using namespace dx_raster_wire::detail;
    if (raw.m_version != DRW::AC1027 || raw.m_objectType != 42 || raw.m_isEntity
        || raw.m_isCustomClass || raw.m_handle != typed.handle || raw.m_parentHandle != owner
        || raw.m_numReactors != 0 || !raw.m_reactorHandles.empty() || raw.m_xDictFlag != 1u
        || raw.m_hasDataStorage || raw.m_rawBytes.empty() || raw.m_rawBytes.size() > 1024u
        || raw.m_bodyBitSize == 0u || typed.m_entries.size() != 3u
        || typed.m_entries[0].m_name != "ByBlock" || typed.m_entries[1].m_name != "ByLayer"
        || typed.m_entries[2].m_name != "Global") return false;
    const auto total = static_cast<std::uint32_t>(raw.m_rawBytes.size() * 8u);
    if (raw.m_bodyBitSize >= total) return false;
    const auto end = total - raw.m_bodyBitSize;
    Bits body{raw.m_rawBytes.data(), 0u, end};
    const auto tag = body.take(2u);
    const auto type = static_cast<std::uint32_t>(tag == 0u ? body.take(8u)
        : tag == 1u ? body.take(8u) + 0x1f0u : body.little(2u));
    const auto self = handle(body, 0u, 0u, true);
    if (!body.good || type != 42u || self.value != raw.m_handle
        || body.shortValue() != 0u || body.longValue() != 0u
        || body.take(1u) != 1u || body.take(1u) != 0u
        || body.longValue() != 3u || body.shortValue() != 1u
        || body.take(8u) != 0u || !body.good || end < 17u) return false;
    Bits footer{raw.m_rawBytes.data(), end - 17u, end};
    const auto size = static_cast<std::uint32_t>(footer.little(2u));
    if (footer.take(1u) != 1u || !footer.good || (size != 350u && size != 403u)
        || size > end - 17u || end - 17u - size != body.pos) return false;
    Bits text{raw.m_rawBytes.data(), body.pos, end - 17u};
    for (const auto& entry : typed.m_entries) {
        const auto length = text.shortValue();
        if (length != entry.m_name.size() + (size == 403u ? 1u : 0u)) return false;
        for (const char c : entry.m_name)
            if (text.little(2u) != static_cast<unsigned char>(c)) return false;
        if (size == 403u && text.little(2u) != 0u) return false;
    }
    const auto textTail = text.end - text.pos;
    if ((size == 350u ? textTail != 0u : textTail != 5u)
        || text.take(textTail) != 0u || !text.good) return false;
    Bits handles{raw.m_rawBytes.data(), end, total};
    const auto ownerToken = handle(handles, 4u, raw.m_handle);
    if (!handles.good || ownerToken.role != 4u || ownerToken.value != owner) return false;
    for (const auto& entry : typed.m_entries) {
        const auto token = handle(handles, 2u, raw.m_handle);
        if (!handles.good || token.role != 2u || token.value != entry.m_handle) return false;
    }
    const auto tail = handles.end - handles.pos;
    return tail < 8u && handles.take(tail) == 0u && handles.good;
}
} // namespace dx_ac1027_material_wire
#endif
