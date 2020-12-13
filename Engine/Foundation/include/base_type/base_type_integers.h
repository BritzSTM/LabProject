/**
    @file BaseType_Integers.h
    @brief Platform 이식성을 높이기 위해 기본 원자타입을 정의
    @details
    시대가 발전함에 따라 언어에서 제공하는 기본 자료형의 크기의 의미가 변해왔다.
    과거 int형의 경우 CPU가 빨리 처리할 수 있는 최대 크기의 의미도 포함했기 때문에
    CPU의 bit에 따라 int형이 16bit = 2byte, 32bit = 4byte로 변해왔다.
    그리고 32bit CPU의 시대가 오래감에 따라 64bit 시대에도 호환성을 위해 int형의 경우 4byte를 유지했다
    그러나 long형의 경우 초기 언어 정의에서 int형보다 더 큰 크기를 제공할 수 있을 수 있다라는 의미로 인해
    platform에 따라 호환성 유지정책이 틀려 long형이 의미하는 크기가 다르다.
    따라서 long형을 사용하는 것은 지양하되 호환성 있는 코드를 작성하기 위해 정규화된 자료형들을 정의한다
*/
#pragma once

#include "../platform/target_arch.h"


#if (FD_TARGET_ARCH == FD_ARCH_X64)
#include "base_type_x64_common.h"

#else
#    error unknown arch type. not include fixed integer types.

#endif


namespace fd
{
    namespace base_type
    {
        /*
            최종 정수원자 타입 크기 확인.
        */
        static_assert(sizeof(int8)  == 1, "int8 type size isn't 1byte!");
        static_assert(sizeof(int16) == 2, "int16 type size isn't 2byte!");
        static_assert(sizeof(int32) == 4, "int32 type size isn't 4byte!");
        static_assert(sizeof(int64) == 8, "int64 type size isn't 8byte!");

        static_assert(sizeof(uint8)  == 1, "uint8 type size isn't 1byte!");
        static_assert(sizeof(uint16) == 2, "uint16 type size isn't 2byte!");
        static_assert(sizeof(uint32) == 4, "uint32 type size isn't 4byte!");
        static_assert(sizeof(uint64) == 8, "uint64 type size isn't 8byte!");
    }
}
