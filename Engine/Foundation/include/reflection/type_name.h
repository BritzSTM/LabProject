/*
    @brief 타입이름을 컴파일 시간에 획득하기 위한 인터페이스를 제공합니다.
    @detail
    1. 반드시 타입이름들은 정규화된 이름으로 제공할 것. 다음과 같은 형식으로 나타남

    MSVC
    [Front IDE] namespace::ABC *&, [Compiled] struct namespace::ABC*&

    Clang
    [Front, Compiled] namespace::ABC *&

*/
#pragma once

#include "../compiler_directive.h"


#if (FD_CURR_COMPILER == FD_MSVC_COMPILER)
#include "type_name_msvc.h"

#elif (FD_CURR_COMPILER == FD_CLANG_COMPILER)
#include "type_name_clang.h"

#else
#   error Not impl type_name

#endif

namespace fd::refl
{
    template<typename _Ty>
    struct type_name_traits
    {

    };
}
