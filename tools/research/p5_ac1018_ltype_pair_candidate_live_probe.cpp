// Optional caller-supplied raw bodies and a live typed callback. This is not a
// reader-private source/CRC ticket and does not prove graph or output closure.
#include "dx_iface.h"
#include "libdwgr.h"
#include "intern/dwg_ltype_control_byblock_ac1018_candidate.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>

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
 }
 void writeLayers() override {} void writeTextstyles() override {} void writeVports() override {}
 void writeViews() override {} void writeUCSs() override {} void writeDimstyles() override {} void writeAppId() override {}
 bool generated{false};
private:dwgRW& writer_;
};
class Probe final:public dx_iface {
public:
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
DRW_UnsupportedObject body(const char* path,std::uint32_t handle,int type) {
 std::ifstream in(path,std::ios::binary);DRW_UnsupportedObject r;r.m_version=DRW::AC1018;r.m_handle=handle;r.m_objectType=type;
 r.m_rawBytes.assign(std::istreambuf_iterator<char>(in),{});r.m_objectSize=static_cast<std::uint32_t>(r.m_rawBytes.size());return r;
}
int main(int argc,char**argv){
 if(argc==3&&std::string(argv[1])=="--self-test"){
  const std::filesystem::path path(argv[2]);if(std::filesystem::exists(path))return 3;
  struct Cleanup{std::filesystem::path p;~Cleanup(){std::error_code e;std::filesystem::remove(p,e);}} cleanup{path};
  dwgRW writer(path.string().c_str());GeneratedSource source(writer);
  if(!writer.write(&source,DRW::AC1018,true)||!source.generated)return 4;
  auto data=std::make_unique<dx_data>();Probe probe;
  const bool read=probe.fileImport(path.string(),data.get(),false);
  const bool ok=read&&probe.tickets==1u&&probe.ticketValid&&probe.rowTickets==3u&&probe.rowTicketValid;
  std::cout<<"generated=1 read="<<read<<" controlTickets="<<probe.tickets
    <<" rowTickets="<<probe.rowTickets<<" typedNames="<<probe.rowTicketValid<<"\n";
  return ok?0:5;
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
