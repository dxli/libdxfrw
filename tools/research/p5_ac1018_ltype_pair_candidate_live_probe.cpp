// Optional caller-supplied raw bodies and a live typed callback. This is not a
// reader-private source/CRC ticket and does not prove graph or output closure.
#include "dx_iface.h"
#include "intern/dwg_ltype_control_byblock_ac1018_candidate.h"
#include <fstream>
#include <iostream>
#include <iterator>
#include <memory>
#include <optional>
class Probe final:public dx_iface {
public:
 void addLType(const DRW_LType& v) override {dx_iface::addLType(v);if(v.handle==0x14u){if(model)duplicate=true;model=v;}}
 std::optional<DRW_LType> model;bool duplicate=false;
};
DRW_UnsupportedObject body(const char* path,std::uint32_t handle,int type) {
 std::ifstream in(path,std::ios::binary);DRW_UnsupportedObject r;r.m_version=DRW::AC1018;r.m_handle=handle;r.m_objectType=type;
 r.m_rawBytes.assign(std::istreambuf_iterator<char>(in),{});r.m_objectSize=static_cast<std::uint32_t>(r.m_rawBytes.size());return r;
}
int main(int argc,char**argv){if(argc!=4)return 2;auto data=std::make_unique<dx_data>();auto p=std::make_unique<Probe>();
 const bool read=p->fileImport(argv[1],data.get(),false);const auto control=body(argv[2],5u,56);const auto row=body(argv[3],0x14u,57);
 DwgLTypeControlByBlockAc1018Candidate::Result r;const bool ok=read&&!p->duplicate&&p->model
  &&DwgLTypeControlByBlockAc1018Candidate::inspect(control,row,*p->model,r);
 std::cout<<"read="<<read<<" matched="<<ok<<" control="<<r.control<<" byblock="<<r.byBlock
          <<" ordinary="<<r.ordinary.target<<" bylayer="<<r.byLayerPhantom.target<<'\n';return !ok;}
