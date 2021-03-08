#include "type_info.h"


namespace fd::refl::_internal_dummy
{
    void test()
    {
        static_assert(GetTypeFullName<ETypeCLASS>() == "fd::refl::ETypeCLASS");
    }
}
