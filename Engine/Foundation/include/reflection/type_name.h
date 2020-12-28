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
#include "../base_types.h"

#include <array>
#include <string_view>


namespace fd::refl
{
    /*
        이 유형을 컴파일러 특성별(_CompilerTag)로 특수화 할 것. 
        아래 정의된 데이터 맴버는 반드시 제공되어야 함.
    */
    template<typename _Ty, typename _CompilerTag = CurrCompilerTag>
    struct base_type_name_traits
    {
        static_assert(false, "Unknown compiler tag.");

        static constexpr std::string_view rawName{ nullptr };
        static constexpr std::string_view fixedPrefix{ nullptr };
        static constexpr std::string_view fixedSubfix{ nullptr };

        static constexpr size_t kDynamicPrefixLen{ 0 }; /**< 고정길이 접두사 이후 동적인 접두사의 길이 */
    };
}


#if (FD_CURR_COMPILER == FD_MSVC_COMPILER)
#include "type_name_msvc.h"

#elif (FD_CURR_COMPILER == FD_CLANG_COMPILER)
#include "type_name_clang.h"

#else
#   error Not impl type_name

#endif


namespace fd::refl
{
    namespace _internal_type_name
    {
        template<typename _Ty>
        inline static constexpr size_t GetRawTypeNameStartPos() noexcept
        {
            using bt = base_type_name_traits<_Ty>;
            return bt::fixedPrefix.size() + bt::kDynamicPrefixLen;
        }

        template<typename _Ty>
        inline static constexpr size_t GetRawTypeNameEndPos() noexcept
        {
            using bt = base_type_name_traits<_Ty>;
            return bt::rawName.size() - bt::fixedSubfix.size();
        }

        template<typename _Ty>
        constexpr size_t CalNormalizedFullNameLen() noexcept
        {
            using bt = base_type_name_traits<_Ty>;

            size_t len{ 0 };
            for (size_t i{ GetRawTypeNameStartPos<_Ty>() }; i < GetRawTypeNameEndPos<_Ty>(); ++i)
            {
                if (bt::rawName[i] != ' ')
                {
                    ++len;
                }
            }

            return len;
        }

        template<typename _Ty>
        constexpr auto GetNormalizedFullName() noexcept
        {
            using namespace std;
            using bt = base_type_name_traits<_Ty>;

            constexpr size_t kArrSize{ CalNormalizedFullNameLen<_Ty>() };
            array<char, kArrSize + 1> arr{ };

            for (size_t tyIdx{ GetRawTypeNameStartPos<_Ty>() }, arrIdx{ 0 }; tyIdx < GetRawTypeNameEndPos<_Ty>(); ++tyIdx)
            {
                if (bt::rawName[tyIdx] != ' ')
                {
                    arr[arrIdx++] = bt::rawName[tyIdx];
                }
            }
            arr[kArrSize] = '\0';

            return arr;
        }
    }

    template<typename _Ty>
    struct type_name_traits
    {
        static constexpr auto normalizedFullName{ _internal_type_name::GetNormalizedFullName<_Ty>() }; /** Normalized name in std::array */

        /** 해당유형의 이름과 비교합니다. 컴파일 타임에 이름 비교시 이 함수를 이용할 것. */
        static constexpr bool compare(std::string_view name) noexcept
        {
            if ((normalizedFullName.size() - 1) != name.size())
            {
                return false;
            }

            for (size_t i{ 0 }; i < name.size(); ++i)
            {
                if (name[i] != normalizedFullName[i])
                {
                    return false;
                }
            }

            return true;
        }
    };
}
