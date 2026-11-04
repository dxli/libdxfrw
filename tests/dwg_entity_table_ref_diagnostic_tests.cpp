// SPDX-License-Identifier: GPL-2.0-or-later
#include "intern/dwgreader27.h"
#include "intern/dwgsafety.h"
#include <iostream>
#include <atomic>
#include <cstdlib>
#include <new>
#include <memory>
#include <utility>

static std::atomic<int> failAfter{-1};
void* operator new(std::size_t n) {if(failAfter>=0 && failAfter--==0)throw std::bad_alloc();if(void* p=std::malloc(n?n:1))return p;throw std::bad_alloc();}
void operator delete(void* p) noexcept {std::free(p);}
void operator delete(void* p,std::size_t) noexcept {std::free(p);}

struct Reader final : dwgReader27 {
 explicit Reader(DRW::Version v=DRW::AC1027) : dwgReader27(std::unique_ptr<dwgBuffer>{},nullptr) {version=v;}
 using dwgReader::parseAttribs;
 void addRow(DwgIntegritySeverity severity,const char* section=nullptr) {
  recordIntegrityDiagnostic(severity,DwgIntegrityAddressSpace::None,DwgIntegrityPhase::SectionParser,
    DwgIntegrityCheckKind::EntityLayerReference,-1,-1,section);
 }
};
template<class T> struct Entity : T {
 void setup(std::uint32_t layer,std::uint32_t ltype,unsigned flags) {
  this->handle=0x44;this->layerH.ref=layer;this->layerH.ref64=layer;
  this->lTypeH.ref=ltype;this->lTypeH.ref64=ltype;this->ltFlags=static_cast<std::uint8_t>(flags);
 }
 void wideLayer(std::uint64_t value) {this->layerH.ref64=value;this->layerH.ref=static_cast<std::uint32_t>(value);}
 void wideLtype(std::uint64_t value) {this->lTypeH.ref64=value;this->lTypeH.ref=static_cast<std::uint32_t>(value);}
 void relative() {this->layerH.code=7;this->lTypeH.code=7;}
};
static bool checkRow(const Reader& r,DwgIntegrityCheckKind kind,std::uint64_t target,DRW::Version version) {
 if(r.m_integrityDiagnostics.size()!=1u)return false;const auto& d=r.m_integrityDiagnostics.front();
 return d.kind==kind && d.severity==DwgIntegritySeverity::Error && d.phase==DwgIntegrityPhase::SectionParser
     && d.version==version && d.hasLogicalHandle && d.logicalHandle==0x44
     && d.hasExpected && d.expected==target && d.hasObserved && d.observed==0;
}
template<class T> bool controls(DRW::Version version) {
 Reader r(version);auto* layer=new DRW_Layer;layer->name="ValidLayer";r.layermap.emplace(0x11,layer);
 auto* ltype=new DRW_LType;ltype->name="ValidType";r.ltypemap.emplace(0x22,ltype);
 Entity<T> e;e.setup(0x11,0x22,3);r.parseAttribs(&e);
 if(!r.m_integrityDiagnostics.empty()||e.layer!="ValidLayer"||e.lineType!="ValidType")return false;
 e.setup(0x7e,0x22,3);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLayerReference,0x7e,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0x22,0x22,3);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLayerReference,0x22,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0x11,0x7e,3);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLineTypeReference,0x7e,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0x11,0x11,3);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLineTypeReference,0x11,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0x11,0x22,3);e.wideLayer(0x100000011ull);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLayerReference,0x100000011ull,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0,0x22,3);e.wideLayer(0x100000000ull);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLayerReference,0x100000000ull,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0x11,0x22,3);e.wideLtype(0x100000022ull);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLineTypeReference,0x100000022ull,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0x11,0,3);e.wideLtype(0x100000000ull);r.parseAttribs(&e);if(!checkRow(r,DwgIntegrityCheckKind::EntityLineTypeReference,0x100000000ull,version))return false;r.m_integrityDiagnostics.clear();
 e.setup(0x11,0x22,3);e.relative();r.parseAttribs(&e);if(!r.m_integrityDiagnostics.empty())return false;
 for(unsigned flag : {0u,1u,2u}) { e.setup(0x11,0x7e,flag);e.wideLtype(0x100000022ull);r.parseAttribs(&e);if(!r.m_integrityDiagnostics.empty())return false; }
 e.setup(0,0,3);r.parseAttribs(&e);return r.m_integrityDiagnostics.empty();
}
int main(){
 for(const auto version : {DRW::AC1015,DRW::AC1018,DRW::AC1021,DRW::AC1024,DRW::AC1027,DRW::AC1032}) {
 if(!controls<DRW_Point>(version) || !controls<DRW_Line>(version) || !controls<DRW_Arc>(version) || !controls<DRW_Circle>(version)
    || !controls<DRW_Ellipse>(version) || !controls<DRW_Solid>(version) || !controls<DRW_Trace>(version) || !controls<DRW_3Dface>(version)
    || !controls<DRW_Block>(version) || !controls<DRW_Insert>(version) || !controls<DRW_Attrib>(version) || !controls<DRW_Attdef>(version)
    || !controls<DRW_Vertex>(version) || !controls<DRW_SeqEnd>(version) || !controls<DRW_Polyline>(version)
    || !controls<DRW_LWPolyline>(version) || !controls<DRW_Spline>(version) || !controls<DRW_Hatch>(version)
    || !controls<DRW_Text>(version) || !controls<DRW_MText>(version) || !controls<DRW_DimLinear>(version) || !controls<DRW_DimRadial>(version))return 1;
 }

 Reader reader;Entity<DRW_Line> line;line.setup(0x7e,0,0);
 for(std::size_t i=0;i<dwgSafety::MaxIntegrityDiagnostics;++i)reader.addRow(DwgIntegritySeverity::Warning);
 reader.parseAttribs(&line);
 if(reader.m_integrityDiagnostics.size()!=dwgSafety::MaxIntegrityDiagnostics || reader.m_integrityDiagnosticsDropped!=1u)return 2;
 Reader vectorFailure;failAfter=0;vectorFailure.parseAttribs(&line);failAfter=-1;
 if(!vectorFailure.m_integrityDiagnostics.empty() || vectorFailure.m_integrityDiagnosticsDropped!=1u)return 3;
 Reader constructionFailure;failAfter=0;
 constructionFailure.addRow(DwgIntegritySeverity::Error,"an intentionally long section name that cannot fit small string storage");failAfter=-1;
 if(!constructionFailure.m_integrityDiagnostics.empty() || constructionFailure.m_integrityDiagnosticsDropped!=1u)return 4;
 return 0;
}
