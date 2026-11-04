#include "dx_iface.h"
#include "dx_data.h"
#include "libdwgr.h"
#include "lc3_compat_check.cpp"
#include <algorithm>
#include <cstdint>
#include <chrono>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <string>
#include <functional>
class DxIfaceStandardDwgCaptureTestAccess {
public:
 static DRW_UnsupportedObject& frame(dx_data& data, unsigned handle) { return data.m_opaqueDwgSourceFrames.at({data.importGeneration,handle}); }
 static DRW_UnsupportedObject& standard(dx_data& data, unsigned handle) { return data.m_standardDwgObjectFrames.at({data.importGeneration,handle}).raw; }
 static std::size_t qualified(const dx_data& data) { return data.m_opaqueDwgOutputQualifiedFrameIdentities.size(); }
 static std::optional<DRW_DwgAc1027LayerSource>& layerSource(dx_data& data) { return data.m_ac1027DefaultLayerSource; }
 static bool& layerDuplicate(dx_data& data) { return data.m_ac1027DefaultLayerDuplicate; }
 static std::uint64_t& layerGeneration(dx_data& data) { return data.m_ac1027DefaultLayerGeneration; }
 static auto& layerPublication(dx_data& data) { return data.m_ac1027LayerPublication; }
 static std::optional<dwgHandle>& headerMaterial(dx_data& data) { return data.m_ac1027HeaderMaterialSource; }
 static bool& headerDuplicate(dx_data& data) { return data.m_ac1027HeaderMaterialDuplicate; }
 static std::uint64_t& headerGeneration(dx_data& data) { return data.m_ac1027HeaderMaterialGeneration; }
};
static void flip(DRW_UnsupportedObject& frame,unsigned delta) { const auto bit=frame.m_rawBytes.size()*8u-frame.m_bodyBitSize+delta; frame.m_rawBytes.at(bit/8)^=static_cast<unsigned char>(1u<<(7u-bit%8)); }
static std::string contents(const std::string& p){std::ifstream in(p,std::ios::binary);return std::string(std::istreambuf_iterator<char>(in),{});}
class Provider final : public LC3CompatCheck, public DRW_DwgDefaultMaterialProvider, public DRW_DwgDefaultPlotStyleProvider {
public:
 explicit Provider(dwgRW& w): writer(w) {}
 bool wantsDwgDefaultMaterialGraph() const override { return true; }
 bool wantsDwgDefaultPlotStyleGraph() const override { return true; }
 bool dwgDefaultMaterialOpaqueReferencesAbsent(std::uint32_t a,std::uint32_t b,std::uint32_t c,std::uint32_t d) const override { return a && b && c && d; }
 void writeEntities() override { DRW_Line line;line.basePoint={1.,2.,3.};line.secPoint={4.,5.,6.};ok=writer.writeLine(&line); }
 bool finalizeDwgWrite() override { return ok; }
 dwgRW& writer; bool ok=false;
};
int main(){
 const auto base=std::filesystem::temp_directory_path() / ("libdxfrw-ac1027-strict-" + std::to_string(std::chrono::steady_clock::now().time_since_epoch().count()));
 std::filesystem::create_directories(base);
 const std::string input=(base/"line.dwg").string(); dwgRW dwg(input.c_str());Provider provider(dwg);if(!dwg.write(&provider,DRW::AC1027,true)||!provider.ok)return 2;
 dx_data data; dx_iface reader; if(!reader.fileImport(input,&data,false) || data.materials.size()!=3 || data.dictionaryWithDefaults.size()!=1 || data.placeholders.size()!=1)return 3;
 auto& mat=data.materials.front(); auto& matRaw=DxIfaceStandardDwgCaptureTestAccess::frame(data,mat.handle);
 auto materialDictIt=std::find_if(data.dictionaries.begin(),data.dictionaries.end(),[&](const DRW_Dictionary& d){return d.handle==mat.parentHandle;});
 if(materialDictIt==data.dictionaries.end())return 26;
 auto& materialDict=*materialDictIt;
 auto& materialDictRaw=DxIfaceStandardDwgCaptureTestAccess::standard(data,materialDict.handle);
 auto& dict=data.dictionaryWithDefaults.front(); auto& dictRaw=DxIfaceStandardDwgCaptureTestAccess::frame(data,dict.handle);
 auto& place=data.placeholders.front(); auto& placeRaw=DxIfaceStandardDwgCaptureTestAccess::frame(data,place.handle);
 // Public callbacks invoked after the read lease cannot create or alter source evidence.
 const auto priorHeader=DxIfaceStandardDwgCaptureTestAccess::headerMaterial(data);
 const auto priorLayer=DxIfaceStandardDwgCaptureTestAccess::layerSource(data);
 if(!priorHeader || !priorLayer)return 53;
 dwgHandle forged=*priorHeader;forged.ref=1u;reader.addDwgHeaderMaterialSource(DRW::AC1027,forged);
 auto forgedLayer=*priorLayer;forgedLayer.m_raw.m_handle=1u;reader.addDwgAc1027DefaultLayerSource(forgedLayer);
 if(DxIfaceStandardDwgCaptureTestAccess::headerDuplicate(data)
     || DxIfaceStandardDwgCaptureTestAccess::layerDuplicate(data)
     || DxIfaceStandardDwgCaptureTestAccess::headerMaterial(data)->ref!=priorHeader->ref
     || DxIfaceStandardDwgCaptureTestAccess::layerSource(data)->m_raw.m_handle!=priorLayer->m_raw.m_handle)return 54;
 for(bool bin: {false,true}) {
  dx_iface writer; const std::string path=(base / (bin?"binary.dxf":"ascii.dxf")).string();
  auto write=[&](){return writer.fileExport(path,DRW::AC1027,bin,&data,false,true,dx_dxfSourceTextPolicy::RequireValidSource,DRW::DxfTextWritePolicy::RequireLossless,dx_dxfProxyGraphicsPolicy::FailIfNotPreserved,dx_dwgReadCompletenessPolicy::RequireComplete);};
  const auto attempt=[&](const char* label,const std::function<void()>& mutate,const std::function<void()>& restore){
   {std::ofstream out(path,std::ios::binary|std::ios::trunc);out<<"sentinel";} mutate(); const bool accepted=write(); const bool kept=contents(path)=="sentinel";restore();const bool retry=write();
   std::cerr << (bin?"binary ":"ascii ") << label << " accepted="<<accepted<<" kept="<<kept<<" retry="<<retry<<'\n'; return !accepted && kept && retry;
  };
  if(!write() || DxIfaceStandardDwgCaptureTestAccess::qualified(data)!=8u)return 10;
  {std::ofstream out(path,std::ios::binary|std::ios::trunc);out<<"sentinel";}
  if(writer.fileExport(path,DRW::AC1024,bin,&data,false,true,dx_dxfSourceTextPolicy::RequireValidSource,DRW::DxfTextWritePolicy::RequireLossless,dx_dxfProxyGraphicsPolicy::FailIfNotPreserved,dx_dwgReadCompletenessPolicy::RequireComplete) || contents(path)!="sentinel" || !write())return 27;
  for (auto& item:data.materials) {
   auto& wire=DxIfaceStandardDwgCaptureTestAccess::frame(data,item.handle);
   const auto flags=item.flags;
   if(!attempt("material common flags",[&]{item.flags^=1;},[&]{item.flags=flags;}))return 11;
   if(!attempt("material owner role all",[&]{flip(wire,1);},[&]{flip(wire,1);}))return 12;
   if(!attempt("material reactor role all",[&]{flip(wire,17);},[&]{flip(wire,17);}))return 13;
  }
  if(!attempt("material dictionary entry role",[&]{flip(materialDictRaw,16+2);},[&]{flip(materialDictRaw,16+2);}))return 14;
  if(!attempt("material dictionary name",[&]{materialDict.m_entries[0].m_name="ByWrong";},[&]{materialDict.m_entries[0].m_name="ByBlock";}))return 15;
  if(!attempt("plot entry role",[&]{flip(dictRaw,16+2);},[&]{flip(dictRaw,16+2);}))return 16;
  auto savedClasses=data.dwgClassCoverage.m_entries;
  if(!attempt("missing class row",[&]{data.dwgClassCoverage.m_entries.clear();},[&]{data.dwgClassCoverage.m_entries=savedClasses;}))return 17;
  if(!attempt("incomplete opaque capture",[&]{data.dwgReadOutcome.opaqueSourceCaptureFailed=true;},[&]{data.dwgReadOutcome.opaqueSourceCaptureFailed=false;}))return 18;
  auto savedNod=data.namedObjectDictionary.m_entries;
  if(!attempt("missing NOD key",[&]{data.namedObjectDictionary.m_entries.clear();},[&]{data.namedObjectDictionary.m_entries=savedNod;}))return 19;
  const auto plotKey=std::find_if(savedNod.begin(),savedNod.end(),[](const DRW_Dictionary::Entry& e){return e.m_name=="ACAD_PLOTSTYLENAME";});
  const auto materialKey=std::find_if(savedNod.begin(),savedNod.end(),[](const DRW_Dictionary::Entry& e){return e.m_name=="ACAD_MATERIAL";});
  if(plotKey==savedNod.end() || materialKey==savedNod.end())return 20;
  if(!attempt("plot NOD alias",[&]{auto alias=*plotKey;alias.m_name="PLOT_ALIAS";data.namedObjectDictionary.m_entries.push_back(alias);},[&]{data.namedObjectDictionary.m_entries=savedNod;}))return 21;
  if(!attempt("material NOD alias",[&]{auto alias=*materialKey;alias.m_name="MATERIAL_ALIAS";data.namedObjectDictionary.m_entries.push_back(alias);},[&]{data.namedObjectDictionary.m_entries=savedNod;}))return 22;
  if(!attempt("duplicate plot NOD key",[&]{data.namedObjectDictionary.m_entries.push_back(*plotKey);},[&]{data.namedObjectDictionary.m_entries=savedNod;}))return 23;
  if(!attempt("material class count",[&]{for(auto& c:data.dwgClassCoverage.m_entries)if(c.m_recordName=="MATERIAL")++c.m_instanceCount;},[&]{data.dwgClassCoverage.m_entries=savedClasses;}))return 24;
  if(!attempt("plot class row",[&]{for(auto& c:data.dwgClassCoverage.m_entries)if(c.m_recordName=="ACDBDICTIONARYWDFLT")c.m_classNumber++;},[&]{data.dwgClassCoverage.m_entries=savedClasses;}))return 25;
  auto& layer=data.layers.front(); const auto layerPlot=layer.plotStyleHandle; const auto layerMaterial=layer.materialHandle;
  if(!attempt("layer plot source role",[&]{layer.plotStyleHandle.code=4;},[&]{layer.plotStyleHandle=layerPlot;}))return 28;
  if(!attempt("layer plot source target",[&]{layer.plotStyleHandle.ref=1;},[&]{layer.plotStyleHandle=layerPlot;}))return 29;
  if(!attempt("layer material source role",[&]{layer.materialHandle.code=4;},[&]{layer.materialHandle=layerMaterial;}))return 30;
  if(!attempt("layer material source target",[&]{layer.materialHandle.ref=1;},[&]{layer.materialHandle=layerMaterial;}))return 31;
  const auto materialText=layer.handleMaterialS; const auto plotText=layer.handlePlotS;
  if(!attempt("layer material source string",[&]{layer.handleMaterialS="1";},[&]{layer.handleMaterialS=materialText;}))return 32;
  if(!attempt("layer plot source string",[&]{layer.handlePlotS="1";},[&]{layer.handlePlotS=plotText;}))return 33;
  auto& layerEvidence=DxIfaceStandardDwgCaptureTestAccess::layerSource(data);const auto savedLayerEvidence=layerEvidence;
  if(!savedLayerEvidence)return 40;
  if(!attempt("layer receipt missing",[&]{layerEvidence.reset();},[&]{layerEvidence=savedLayerEvidence;}))return 41;
  if(!attempt("layer receipt duplicate",[&]{DxIfaceStandardDwgCaptureTestAccess::layerDuplicate(data)=true;},[&]{DxIfaceStandardDwgCaptureTestAccess::layerDuplicate(data)=false;}))return 42;
  if(!attempt("layer receipt generation",[&]{++DxIfaceStandardDwgCaptureTestAccess::layerGeneration(data);},[&]{--DxIfaceStandardDwgCaptureTestAccess::layerGeneration(data);}))return 43;
  auto& publication=DxIfaceStandardDwgCaptureTestAccess::layerPublication(data);const auto savedPublication=publication;
  if(!attempt("layer publication missing",[&]{publication.valid=false;},[&]{publication=savedPublication;}))return 44;
  if(!attempt("layer publication duplicate",[&]{publication.duplicate=true;},[&]{publication=savedPublication;}))return 45;
  if(!attempt("layer publication offset",[&]{++publication.offset;},[&]{publication=savedPublication;}))return 46;
  if(!attempt("layer publication ordinal",[&]{++publication.ordinal;},[&]{publication=savedPublication;}))return 47;
  if(!attempt("layer receipt handle",[&]{++layerEvidence->m_raw.m_handle;},[&]{layerEvidence=savedLayerEvidence;}))return 48;
  if(!attempt("layer unknown tail role",[&]{flip(layerEvidence->m_raw,16+8+16+16+16+3);},[&]{layerEvidence=savedLayerEvidence;}))return 49;
  if(!attempt("layer unknown relative nonnull",[&]{flip(layerEvidence->m_raw,16+8+16+16+16+2);flip(layerEvidence->m_raw,16+8+16+16+16+3);},[&]{layerEvidence=savedLayerEvidence;}))return 55;
  const auto oldUnknown=layer.unknownHandle;
  if(!attempt("layer unknown typed",[&]{layer.unknownHandle.ref=1u;},[&]{layer.unknownHandle=oldUnknown;}))return 56;
  if(!attempt("layer xref tail role",[&]{flip(layerEvidence->m_raw,16+3);},[&]{layerEvidence=savedLayerEvidence;}))return 50;
  if(!attempt("layer xref flags bits",[&]{layerEvidence->m_raw.m_rawBytes.at(4)^=0x40;},[&]{layerEvidence=savedLayerEvidence;}))return 51;
  if(!attempt("layer discarded flags bits",[&]{layerEvidence->m_raw.m_rawBytes.at(5)^=0x01;},[&]{layerEvidence=savedLayerEvidence;}))return 52;
  auto& header=DxIfaceStandardDwgCaptureTestAccess::headerMaterial(data); const auto savedHeader=header;
  if(!savedHeader)return 34;
  if(!attempt("header missing",[&]{header.reset();},[&]{header=savedHeader;}))return 35;
  if(!attempt("header role",[&]{header->code=4;},[&]{header=savedHeader;}))return 36;
  if(!attempt("header target",[&]{header->ref=1;},[&]{header=savedHeader;}))return 37;
  if(!attempt("header duplicate",[&]{DxIfaceStandardDwgCaptureTestAccess::headerDuplicate(data)=true;},[&]{DxIfaceStandardDwgCaptureTestAccess::headerDuplicate(data)=false;}))return 38;
  if(!attempt("header generation",[&]{++DxIfaceStandardDwgCaptureTestAccess::headerGeneration(data);},[&]{--DxIfaceStandardDwgCaptureTestAccess::headerGeneration(data);}))return 39;
  if(!attempt("material typed",[&]{mat.m_diffuseColorFactor=.25;},[&]{mat.m_diffuseColorFactor=1.;}))return 4;
  if(!attempt("material body",[&]{matRaw.m_rawBytes[10]^=1;},[&]{matRaw.m_rawBytes[10]^=1;}))return 5;
  if(!attempt("material owner role",[&]{flip(matRaw,1);},[&]{flip(matRaw,1);}))return 6;
  if(!attempt("plot default role",[&]{flip(dictRaw,16+16+3);},[&]{flip(dictRaw,16+16+3);}))return 7;
  if(!attempt("placeholder owner role",[&]{flip(placeRaw,3);},[&]{flip(placeRaw,3);}))return 8;
  if(!attempt("plot scalar",[&]{dict.cloning=2;},[&]{dict.cloning=1;}))return 9;
 }
 std::filesystem::remove_all(base);
}
