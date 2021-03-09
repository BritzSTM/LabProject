#pragma once

#include <mutex>


namespace fd::refl
{
    namespace _internal_type_storage
    {
        // 이건 fd 컴파일할 때 한정으로 가능한 것.
        std::once_flag s_typeStorageInitFlag;

    }
}
