#include <libdxfrw.h>
#include <optional>

#if !defined(__cpp_lib_optional) || __cpp_lib_optional < 201606L
#error "The dxfrw public target must propagate its C++17 requirement"
#endif

static_assert(sizeof(DRW_Variant::ActiveDwgLayerReference) > 0,
              "public C++17 API must be visible to downstream consumers");
