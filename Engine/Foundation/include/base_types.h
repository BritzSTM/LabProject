/**
    @file base_types.h
    @brief 정규화된 정수, 실수 자료형을 제공함과 동시에 CPU 정보처리 단위 자료형을 정의합니다.
    @details
     x86 CPU 정보처리 단위는 byte, word, dword, qword가 있지만 word의 순수한 의미는
    CPU가 빠르게 처리할 수 있는 최대 데이터 크기를 의미한다.
    따라서 x86, arm에서 의미하는 word가 의미하는 크기가 달라진다.
     8bit 시대부터 발전해온 x86의 경우 16bit 시대에서 word의 크기가 2byte가 되었으며
    32bit 시대에서는 word가 4byte가 되어야 했지만 호환성을 위해 2byte로 계승되어 왔다.
    하지만 arm의 경우 32bit에서 word는 4byte를 가진다.
     그렇기에 호환성 극대화 하기 위해서는 추상화 단계에서 CPU 정보처리 단위의 사용을 지양해야 하며
    하위 platform 종속성을 가진 함수들에서만 사용하도록 노력해야 한다.
     이 코드에서 정보처리의 단위는 명확한 크기를 나타내기 위해 word에 크기 접미사를 붙여 나타내기로 한다.
    따라서 half_word, dword, qword의 단위는 정의하지 않는다.
*/
#pragma once

#include "base_type/base_type_integers.h"
#include "base_type/base_type_floatings.h


namespace fd
{
    /**
        @namespace base_type
        @brief
        정규화된 기본 자료형을 제공하는 영역. Lib 사용시 BaseTypes namespace를 직접 참조하지 말고
        base_types를 포함하여 namespace fd에서 참조할 것
    */
    namespace base_type
    {
        /*
            CPU 정보처리 정규화 단위 정의
        */
        using byte   = uint8;
        using word16 = uint16;
        using word32 = uint32;
        using word64 = uint64;

        // 자주 사용되는 자연수 최대 크기 
        using size_t = uint64;
    }

    // level 0 상승
    using namespace base_type;
}
