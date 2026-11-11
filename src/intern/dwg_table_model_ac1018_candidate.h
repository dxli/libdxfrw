/* Bounded deep-snapshot eligibility for one parsed AC1018 TABLE profile.
 * This admits source observation only; it cannot authorize DWG/DXF output.
 */
#ifndef DWG_TABLE_MODEL_AC1018_CANDIDATE_H
#define DWG_TABLE_MODEL_AC1018_CANDIDATE_H
#include "drw_entities.h"
#include "dxfattdef.h"
#include "dxfentitiesauth.h"
#include <cstddef>
#include <typeinfo>
namespace DwgTableModelAc1018Candidate {
// A conservative fixed charge covers container/control-block overhead and all
// individually bounded allocations below. It is charged before any deep copy.
constexpr std::size_t ModelReservation=256u*1024u;
inline bool bounded(const DRW_Table& t) noexcept {
    const auto small=[](const std::string& s){return s.size()<=256u;};
    const auto color=[&](const DRW_DwgCmColor& c){return small(c.colorName)&&small(c.bookName);};
    if(typeid(t)!=typeid(DRW_Table) || t.eType!=DRW::TABLE || !t.handle
        || !t.m_hasSemanticContent || !t.m_semanticContentComplete
        || t.proxyGraphics.size()!=2104u || t.numProxyGraph!=2104
        || !small(t.name) || !small(t.layer) || !small(t.lineType)
        || !small(t.colorName) || !small(t.proxyGraphicsProvenance().sourceCodePage) || !t.extData.empty() || !t.appData.empty()
        || !t.reactorHandles.empty() || t.hasDataStorageRecord || t.hasDataStorageBinaryData()
        || !t.dataStorageHandleKey.empty() || !t.dataStorageData.empty() || !t.dataStoragePayloadMarkerSection.empty()
        || !t.pendingAppIdResolutions.empty() || !t.pendingLayerRefResolutions.empty()
        || !dxfAttdef::Decoder::hasNoDxfAccountingLease(t) || !dxfAttdef::Decoder::hasNoEntityNativeWriteBindings(t)
        || !t.attlist.empty() || t.seqEndEntity || !t.attribHandles.empty()
        || t.m_content.m_columns.size()!=3u || t.m_content.m_rows.size()!=3u
        || t.m_content.m_mergedRanges.size()>9u || t.m_content.m_subrecordRanges.size()>32u
        || !t.m_content.m_fieldHandles.empty() || !small(t.m_content.m_name) || !small(t.m_content.m_description))return false;
    for(const auto& name:t.m_tableRowTextStyleNames)if(!small(name))return false;
    for(const auto& value:t.m_tableRowTextColors)if(!color(value))return false;
    for(const auto& value:t.m_tableRowFillColors)if(!color(value))return false;
    for(const auto& value:t.m_tableBorderColors)if(!color(value))return false;
    for(const auto& column:t.m_content.m_columns)if(!small(column.m_name))return false;
    for(const auto& range:t.m_content.m_subrecordRanges)if(!small(range.m_name))return false;
    for(const auto& row:t.m_content.m_rows) {
        if(row.m_cells.size()!=3u)return false;
        for(const auto& cell:row.m_cells) {
            if(!small(cell.m_toolTip) || !small(cell.m_textStyleName) || !cell.m_attributes.empty()
                || cell.m_contents.size()!=1u)return false;
            const auto& content=cell.m_contents.front();const auto& value=content.m_value;
            std::size_t variantBytes=0u;
            if(!small(content.m_text) || !small(value.m_formatString) || !small(value.m_valueString)
                || !value.m_rawData.empty() || value.m_value.type()!=DRW_Variant::STRING
                || !dxfEntitiesAuth::VariantAccess::canonical(value.m_value)
                || !small(*value.m_value.content.s)
                || !value.m_value.estimateRetainedPayloadBytes(1024u,variantBytes)
                || value.m_value.dwgCmColorValue()!=nullptr || value.m_value.isLayerRef()
                || value.m_value.rawDwgLayerReference()!=0u)return false;
        }
    }
    return true;
}
}
#endif
