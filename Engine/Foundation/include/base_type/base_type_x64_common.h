/**
    @brief x64의 공통된 고정크기 기본 자료형 정의.
    이 파일을 직접 포함하지 말고 상위 파일인 base_type_integers.h를 포함해서 사용할 것

    @detail
    현재 x64 고정길이 옵션 자료형의 경우 c++11 이상 지원하는 컴파일러를 사용하면 대부분 기본 자료형으로 제공됨
*/
#pragma once

#include <cstdint>


namespace fd
{
    namespace base_type
    {
        using int8 = std::int8_t;
        using int16 = std::int16_t;
        using int32 = std::int32_t;
        using int64 = std::int64_t;

        using uint8 = std::uint8_t;
        using uint16 = std::int16_t;
        using uint32 = std::int32_t;
        using uint64 = std::int64_t;
    }
}
