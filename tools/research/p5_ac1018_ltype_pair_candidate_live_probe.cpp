// Optional caller-supplied raw bodies and a live typed callback. This is not a
// reader-private source/CRC ticket and does not prove graph or output closure.
#include "dx_iface.h"
#include "libdwgr.h"
#include "intern/dwgobjectframe.h"
#include "intern/dwgutil.h"
#include "intern/dwg_fixed_handles.h"
#include "intern/dwg_ltype_control_byblock_ac1018_candidate.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
#include <algorithm>
#include <cstdlib>
#include <cstdint>
#include <vector>

class GeneratedSource final:public dx_iface {
public:
 explicit GeneratedSource(dwgRW& writer):writer_(writer){}
 void writeHeader(DRW_Header& header) override {header.vars.clear();}
 void collectDwgAppIds() override {}
 void writeBlocks() override {} void writeBlockRecords() override {} void writeEntities() override {}
 void writeLTypes() override {
  DRW_LType byblock;byblock.handle=0x14u;byblock.name="ByBlock";byblock.flags=0x40;byblock.length=0.;byblock.alignment=65;
  DRW_LType bylayer;bylayer.handle=0x15u;bylayer.name="ByLayer";bylayer.flags=0x40;bylayer.length=0.;bylayer.alignment=65;
  DRW_LType continuous;continuous.handle=0x16u;continuous.name="Continuous";continuous.desc="Solid line";continuous.flags=0;continuous.length=0.;continuous.alignment=65;
  generated=writer_.addLType(&byblock)&&writer_.addLType(&bylayer)&&writer_.addLType(&continuous);
  byBlockHandle=byblock.handle;byLayerHandle=bylayer.handle;continuousHandle=continuous.handle;
 }
 void writeLayers() override {} void writeTextstyles() override {} void writeVports() override {}
 void writeViews() override {} void writeUCSs() override {} void writeDimstyles() override {} void writeAppId() override {}
 bool generated{false};
 std::uint32_t byBlockHandle{0u},byLayerHandle{0u},continuousHandle{0u};
private:dwgRW& writer_;
};
class Probe final:public dx_iface {
public:
 void reset(){model.reset();duplicate=false;tickets=0;ticketValid=false;rowTickets=0;rowTicketValid=false;controlFrame.reset();byBlockFrame.reset();byBlockFrameModel.reset();}
 void addLType(const DRW_LType& v) override {dx_iface::addLType(v);if(v.name=="BYBLOCK"||v.name=="ByBlock"){if(model)duplicate=true;model=v;}}
 std::optional<DRW_LType> model;bool duplicate=false;unsigned tickets=0;bool ticketValid=false;unsigned rowTickets=0;bool rowTicketValid=false;
 std::optional<DRW_UnsupportedObject> controlFrame,byBlockFrame;std::optional<DRW_LType> byBlockFrameModel;
 bool wantsVerifiedTableCandidate(std::uint32_t h) const noexcept override {return h==5u||h==15u||h==16u||h==17u||h==0x14u||h==0x15u;}
 void verifiedTableCandidate(const DwgVerifiedTableCandidateTicket& t) noexcept override {
  if(t.kind()==DwgVerifiedTableCandidateKind::LTypeControl&&t.version()==DRW::AC1018
      &&t.handle()==5u&&t.frame().body().size()<=4096u){++tickets;ticketValid=true;
    DRW_UnsupportedObject raw;raw.m_version=t.version();raw.m_handle=t.handle();raw.m_objectType=56;
    raw.m_objectOffset=t.offset();raw.m_objectSize=static_cast<std::uint32_t>(t.frame().body().size());
    raw.m_bodyBitSize=t.frame().bodyBitSize();raw.m_rawBytes=t.frame().body();controlFrame=std::move(raw);
  }
  if(t.kind()==DwgVerifiedTableCandidateKind::LTypeRecord&&t.version()==DRW::AC1018
      &&(t.handle()==15u||t.handle()==16u||t.handle()==17u||t.handle()==0x14u||t.handle()==0x15u||t.handle()==0x16u)
      &&t.frame().body().size()<=4096u&&t.ltypeModel()){
    ++rowTickets;
    const std::string expected=(t.handle()==15u||t.handle()==0x14u)?"BYBLOCK":(t.handle()==16u||t.handle()==0x15u)?"BYLAYER":"CONTINUOUS";
    if(t.ltypeModel()->handle==t.handle()&&t.ltypeModel()->name==expected)rowTicketValid=true;
    if(t.handle()==15u||t.handle()==0x14u){byBlockFrameModel=*t.ltypeModel();DRW_UnsupportedObject raw;raw.m_version=t.version();raw.m_handle=t.handle();raw.m_objectType=57;
      raw.m_objectSize=static_cast<std::uint32_t>(t.frame().body().size());raw.m_bodyBitSize=t.frame().bodyBitSize();raw.m_rawBytes=t.frame().body();byBlockFrame=std::move(raw);}
  }
 }
};
std::uint32_t get32(const std::uint8_t* p){return static_cast<std::uint32_t>(p[0])|static_cast<std::uint32_t>(p[1])<<8u
 |static_cast<std::uint32_t>(p[2])<<16u|static_cast<std::uint32_t>(p[3])<<24u;}
void put32(std::uint8_t* p,std::uint32_t n){for(unsigned i=0;i<4u;++i)p[i]=static_cast<std::uint8_t>(n>>(i*8u));}
bool corruptObjectFrameCrc(const std::filesystem::path& path,const DRW_UnsupportedObject& source,
                           std::vector<std::uint8_t>& original){
 std::ifstream input(path,std::ios::binary);std::vector<char> chars((std::istreambuf_iterator<char>(input)),{});if(!input||source.m_rawBytes.empty())return false;
 original.reserve(chars.size());for(const auto c:chars)original.push_back(static_cast<std::uint8_t>(c));
 const auto hit=std::search(original.cbegin(),original.cend(),source.m_rawBytes.cbegin(),source.m_rawBytes.cend());
 if(hit==original.cend()||std::search(std::next(hit),original.cend(),source.m_rawBytes.cbegin(),source.m_rawBytes.cend())!=original.cend())return false;
 const auto bodyAt=static_cast<std::size_t>(hit-original.cbegin());dwgBuffer reader(original.data(),original.size(),nullptr);DwgObjectFrame frame;
 std::size_t frameAt=0u;bool foundFrame=false;for(const auto prefix:{2u,4u})if(bodyAt>=prefix
   &&frame.readAt(reader,DRW::AC1018,bodyAt-prefix)&&frame.body()==source.m_rawBytes
   &&reader.getPosition()==bodyAt+source.m_rawBytes.size()+2u){frameAt=bodyAt-prefix;foundFrame=true;break;}
 if(!foundFrame)return false;
 std::size_t pageAt=0u;std::vector<std::uint8_t> header(32u);
 for(std::size_t p=0x100u;p+32u<=original.size();p+=0x20u){std::copy_n(original.data()+p,32u,header.data());dwgCompressor::decrypt18Hdr(header.data(),32u,p);
  const auto size=get32(header.data()+8u);if(get32(header.data())!=0x4163043bu||size!=get32(header.data()+12u)
    ||size>original.size()-(p+32u)||bodyAt<p+32u||bodyAt+source.m_rawBytes.size()+2u>p+32u+size)continue;
  const auto dataCrc=dwgUtil::checksum18(0u,original.data()+p+32u,size);const auto headerCrc=get32(header.data()+24u);put32(header.data()+24u,0u);
  if(dataCrc!=get32(header.data()+28u)||dwgUtil::checksum18(dataCrc,header.data(),32u)!=headerCrc)continue;
  if(pageAt!=0u)return false;pageAt=p;}
 if(!pageAt)return false;auto damaged=original;damaged[bodyAt+source.m_rawBytes.size()]^=1u;
 std::copy_n(damaged.data()+pageAt,32u,header.data());dwgCompressor::decrypt18Hdr(header.data(),32u,pageAt);
 const auto size=get32(header.data()+8u);const auto dataCrc=dwgUtil::checksum18(0u,damaged.data()+pageAt+32u,size);
 put32(header.data()+28u,dataCrc);put32(header.data()+24u,0u);put32(header.data()+24u,dwgUtil::checksum18(dataCrc,header.data(),32u));
 dwgCompressor::decrypt18Hdr(header.data(),32u,pageAt);std::copy(header.begin(),header.end(),damaged.begin()+static_cast<std::ptrdiff_t>(pageAt));
 dwgBuffer verify(damaged.data(),damaged.size(),nullptr);if(frame.readAt(verify,DRW::AC1018,frameAt))return false;
 std::ofstream output(path,std::ios::binary|std::ios::trunc);output.write(reinterpret_cast<const char*>(damaged.data()),static_cast<std::streamsize>(damaged.size()));return static_cast<bool>(output);
}
DRW_UnsupportedObject body(const char* path,std::uint32_t handle,int type) {
 std::ifstream in(path,std::ios::binary);DRW_UnsupportedObject r;r.m_version=DRW::AC1018;r.m_handle=handle;r.m_objectType=type;
 r.m_rawBytes.assign(std::istreambuf_iterator<char>(in),{});r.m_objectSize=static_cast<std::uint32_t>(r.m_rawBytes.size());return r;
}
int main(int argc,char**argv){
 if(argc==7&&std::string(argv[1])=="--inspect-resource"){
  const auto parseHandle=[](const char* text,std::uint32_t& value){
   char* end=nullptr;const auto parsed=std::strtoul(text,&end,0);
   if(text==end||*end!='\0'||parsed==0u||parsed>0xfffffffful)return false;
   value=static_cast<std::uint32_t>(parsed);return true;
  };
  std::uint32_t control=0u,ordinary=0u,byBlock=0u,byLayer=0u;
  if(!parseHandle(argv[3],control)||!parseHandle(argv[4],ordinary)
      ||!parseHandle(argv[5],byBlock)||!parseHandle(argv[6],byLayer))return 2;
  dx_iface reader;dx_data data;
  const bool selected=reader.enableVerifiedLTypeResourceSource(
      control,ordinary,byBlock,byLayer);
  const bool imported=selected&&reader.fileImport(argv[2],&data,false);
  const bool bound=imported&&dx_iface::hasUnchangedVerifiedLTypeResourceSource(
      data,control,ordinary,byBlock,byLayer);
  std::cout<<"selected="<<selected<<" imported="<<imported
      <<" operationSucceeded="<<data.dwgReadOutcome.operationSucceeded
      <<" sourceBound="<<bound<<" generation="<<data.importGeneration<<'\n';
  return bound?0:1;
 }
 if(argc==3&&std::string(argv[1])=="--self-test"){
  const std::filesystem::path path(argv[2]);if(std::filesystem::exists(path))return 3;
  struct Cleanup{std::filesystem::path p;~Cleanup(){std::error_code e;std::filesystem::remove(p,e);}} cleanup{path};
  dx_iface selectedLtype,selectedXrecord,duplicateLtype;
  const bool ltypeSelection=selectedLtype.enableVerifiedLTypeResourceSource(5u,0x16u,0x14u,0x15u)
      &&!selectedLtype.enableVerifiedXRecordSource(0xbcu);
  const bool xrecordSelection=selectedXrecord.enableVerifiedXRecordSource(0xbcu)
      &&!selectedXrecord.enableVerifiedLTypeResourceSource(5u,0x16u,0x14u,0x15u);
  const bool duplicateSelection=!duplicateLtype.enableVerifiedLTypeResourceSource(5u,0x16u,0x16u,0x15u);
  dwgRW writer(path.string().c_str());GeneratedSource generated(writer);
  if(!writer.write(&generated,DRW::AC1018,true)||!generated.generated)return 4;
  std::ifstream source(path,std::ios::binary);
  const std::vector<char> original((std::istreambuf_iterator<char>(source)),{});
  if(!source||original.size()<64u)return 5;
  dx_iface ltypeReader;dx_data ltypeData;
  const bool ltypeSelected=ltypeReader.enableVerifiedLTypeResourceSource(
      DRW::DwgLtypeControlHandle,generated.continuousHandle,
      generated.byBlockHandle,generated.byLayerHandle);
  const bool ltypeImported=ltypeSelected&&ltypeReader.fileImport(path.string(),&ltypeData,false);
  bool ltypeSourceBound=ltypeImported&&dx_iface::hasUnchangedVerifiedLTypeResourceSource(
      ltypeData,DRW::DwgLtypeControlHandle,generated.continuousHandle,
      generated.byBlockHandle,generated.byLayerHandle);
  bool ltypeMutationRefused=false,ltypeRestoreAccepted=false;
  if(ltypeSourceBound) {
   const auto row=std::find_if(ltypeData.lineTypes.begin(),ltypeData.lineTypes.end(),
       [&generated](const DRW_LType& value){return value.handle==generated.continuousHandle;});
   if(row==ltypeData.lineTypes.end())ltypeSourceBound=false;
   else {const auto description=row->desc;row->desc+="_MUTATED";
    ltypeMutationRefused=!dx_iface::hasUnchangedVerifiedLTypeResourceSource(ltypeData,
        DRW::DwgLtypeControlHandle,generated.continuousHandle,
        generated.byBlockHandle,generated.byLayerHandle);
    row->desc=description;ltypeRestoreAccepted=dx_iface::hasUnchangedVerifiedLTypeResourceSource(ltypeData,
        DRW::DwgLtypeControlHandle,generated.continuousHandle,
        generated.byBlockHandle,generated.byLayerHandle);}
  }
  auto data=std::make_unique<dx_data>();Probe probe;
  {std::ofstream broken(path,std::ios::binary|std::ios::trunc);broken.write(original.data(),20);if(!broken)return 6;}
  const bool failedRead=probe.fileImport(path.string(),data.get(),false);
  const bool leaked=probe.tickets!=0u||probe.rowTickets!=0u;
  {std::ofstream restored(path,std::ios::binary|std::ios::trunc);restored.write(original.data(),static_cast<std::streamsize>(original.size()));if(!restored)return 7;}
  const bool read=probe.fileImport(path.string(),data.get(),false);
  const bool ok=ltypeSelection&&xrecordSelection&&duplicateSelection
      &&ltypeSourceBound&&ltypeMutationRefused&&ltypeRestoreAccepted
      &&!failedRead&&!leaked&&read&&probe.tickets==1u&&probe.ticketValid
      &&probe.rowTickets==3u&&probe.rowTicketValid;
  if(!ok)return 8;
  std::vector<std::uint8_t> originalBytes;if(!probe.controlFrame||!corruptObjectFrameCrc(path,*probe.controlFrame,originalBytes))return 9;
  probe.reset();auto damaged=std::make_unique<dx_data>();const bool damagedRead=probe.fileImport(path.string(),damaged.get(),false);
  bool controlDiagnostic=false;const auto& integrity=damaged->dwgReadOutcome.observations.integrity;
  for(std::size_t i=0;i<integrity.retained;++i){const auto& row=integrity.rows[i];
   if(row.kind==DwgIntegrityCheckKind::ObjectFrameBounds&&row.hasLogicalHandle&&row.logicalHandle==5u)controlDiagnostic=true;
   if(row.kind==DwgIntegrityCheckKind::DataPageChecksum||row.kind==DwgIntegrityCheckKind::DataPageCrc)return 10;}
  const bool leakedCrcTickets=probe.tickets!=0u||probe.rowTickets!=0u;
  {std::ofstream restored(path,std::ios::binary|std::ios::trunc);restored.write(reinterpret_cast<const char*>(originalBytes.data()),static_cast<std::streamsize>(originalBytes.size()));if(!restored)return 11;}
  probe.reset();auto retryData=std::make_unique<dx_data>();const bool crcRetryRead=probe.fileImport(path.string(),retryData.get(),false);
  const bool crcRetry=crcRetryRead&&probe.tickets==1u&&probe.ticketValid&&probe.rowTickets==3u&&probe.rowTicketValid;
  const auto byBlockHandle=probe.byBlockFrame?probe.byBlockFrame->m_handle:0u;
  std::vector<std::uint8_t> rowOriginalBytes;
  if(!probe.byBlockFrame||!corruptObjectFrameCrc(path,*probe.byBlockFrame,rowOriginalBytes))return 13;
  probe.reset();auto rowDamagedData=std::make_unique<dx_data>();
  const bool rowDamagedRead=probe.fileImport(path.string(),rowDamagedData.get(),false);
  bool rowControlDiagnostic=false;const auto& rowIntegrity=rowDamagedData->dwgReadOutcome.observations.integrity;
  for(std::size_t i=0;i<rowIntegrity.retained;++i){const auto& row=rowIntegrity.rows[i];
   if(row.kind==DwgIntegrityCheckKind::ObjectFrameBounds&&row.hasLogicalHandle&&row.logicalHandle==byBlockHandle)rowControlDiagnostic=true;
   if(row.kind==DwgIntegrityCheckKind::DataPageChecksum||row.kind==DwgIntegrityCheckKind::DataPageCrc)return 14;}
  const bool rowCrcLeakedTickets=probe.tickets!=0u||probe.rowTickets!=0u;
  {std::ofstream restored(path,std::ios::binary|std::ios::trunc);restored.write(reinterpret_cast<const char*>(rowOriginalBytes.data()),static_cast<std::streamsize>(rowOriginalBytes.size()));if(!restored)return 15;}
  probe.reset();auto rowRetryData=std::make_unique<dx_data>();
  const bool rowRetryRead=probe.fileImport(path.string(),rowRetryData.get(),false);
  const bool rowRetry=rowRetryRead&&probe.tickets==1u&&probe.ticketValid&&probe.rowTickets==3u&&probe.rowTicketValid;
  std::cout<<"generated=1 ltypeSelected="<<ltypeSelected<<" ltypeImported="<<ltypeImported
    <<" ltypeSourceBound="<<ltypeSourceBound<<" ltypeMutationRefused="<<ltypeMutationRefused
    <<" ltypeRestoreAccepted="<<ltypeRestoreAccepted<<" failedRead="<<failedRead<<" leakedTickets="<<leaked
    <<" retryRead="<<read<<" crcRead="<<damagedRead<<" crcDiagnostic="<<controlDiagnostic
    <<" crcLeakedTickets="<<leakedCrcTickets<<" crcRetry="<<crcRetry
    <<" rowCrcRead="<<rowDamagedRead<<" rowCrcDiagnostic="<<rowControlDiagnostic
    <<" rowCrcLeakedTickets="<<rowCrcLeakedTickets<<" rowCrcRetry="<<rowRetry<<"\n";
  return !damagedRead&&controlDiagnostic&&!leakedCrcTickets&&crcRetry
      &&!rowDamagedRead&&rowControlDiagnostic&&!rowCrcLeakedTickets&&rowRetry?0:16;
 }
 if(argc==3&&std::string(argv[1])=="--emit-dwg"){
  std::ifstream existing(argv[2],std::ios::binary);if(existing)return 3;
  dwgRW writer(argv[2]);GeneratedSource source(writer);
  return writer.write(&source,DRW::AC1018,true)&&source.generated?0:4;
 }
 if(argc!=2&&argc!=4)return 2;auto data=std::make_unique<dx_data>();auto p=std::make_unique<Probe>();
 const bool read=p->fileImport(argv[1],data.get(),false);
 if(argc==2){DwgLTypeControlByBlockAc1018Candidate::Result joined;
  const bool bound=p->controlFrame&&p->byBlockFrameModel
    &&DwgLTypeControlByBlockAc1018Candidate::inspect(*p->controlFrame,*p->byBlockFrame,
        *p->byBlockFrameModel,joined);
  std::cout<<"read="<<read<<" controlTickets="<<p->tickets<<" controlValid="<<p->ticketValid
  <<" rowTickets="<<p->rowTickets<<" rowValid="<<p->rowTicketValid<<" joined="<<bound<<"\n";
  return !(read&&p->tickets==1u&&p->ticketValid&&p->rowTickets==3u&&p->rowTicketValid);}
 const auto control=body(argv[2],5u,56);const auto row=body(argv[3],0x14u,57);
 DwgLTypeControlByBlockAc1018Candidate::Result r;const bool ok=read&&!p->duplicate&&p->model
  &&DwgLTypeControlByBlockAc1018Candidate::inspect(control,row,*p->model,r);
 std::cout<<"read="<<read<<" matched="<<ok<<" control="<<r.control<<" byblock="<<r.byBlock
          <<" ordinary="<<r.ordinary.target<<" bylayer="<<r.byLayerPhantom.target<<'\n';return !ok;}
