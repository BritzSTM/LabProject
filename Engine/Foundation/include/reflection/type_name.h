/*
    @brief 타입이름을 컴파일 시간에 획득하기 위한 인터페이스를 제공합니다.
    @detail
    1. 반드시 타입이름들은 정규화된 이름으로 제공할 것. 다음과 같은 형식으로 나타남

    MSVC
    [Front IDE] namespace::ABC *&, [Compiled] struct namespace::ABC*&

    Clang
    [Front, Compiled] namespace::ABC *&

    2. 호환성을 위해 저수준으로 작성한 것이 많아 못생긴 것이 많음

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
        static constexpr std::string_view rawName{ "ERR_SIMBOL" };
        static constexpr std::string_view fixedPrefix{ "" };
        static constexpr std::string_view fixedSubfix{ "" };

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
        // base_type_name_traits help function
        
        template<typename _Ty, typename _Bt = base_type_name_traits<_Ty>>
        inline static constexpr size_t GetRawTypeNameStartPos() noexcept
        {
            return _Bt::fixedPrefix.size() + _Bt::kDynamicPrefixLen;
        }

        template<typename _Ty, typename _Bt = base_type_name_traits<_Ty>>
        inline static constexpr size_t GetRawTypeNameEndPos() noexcept
        {
            return _Bt::rawName.size() - _Bt::fixedSubfix.size();
        }

        template<typename _Ty, typename _Bt = base_type_name_traits<_Ty>>
        constexpr size_t CalNormalizedFullNameLen() noexcept
        {
            size_t len{ 0 };
            for (size_t i{ GetRawTypeNameStartPos<_Ty>() }; i < GetRawTypeNameEndPos<_Ty>(); ++i)
            {
                if (_Bt::rawName[i] != ' ')
                {
                    ++len;
                }
            }

            return len;
        }

        template<typename _Ty, typename _Bt = base_type_name_traits<_Ty>>
        constexpr auto NormalizeFullTypeName() noexcept
        {
            constexpr size_t kArrSize{ CalNormalizedFullNameLen<_Ty>() };
            std::array<char, kArrSize + 1> arr{ };

            for (size_t tyIdx{ GetRawTypeNameStartPos<_Ty>() }, arrIdx{ 0 }; tyIdx < GetRawTypeNameEndPos<_Ty>(); ++tyIdx)
            {
                if (_Bt::rawName[tyIdx] != ' ')
                {
                    arr[arrIdx++] = _Bt::rawName[tyIdx];
                }
            }
            arr[kArrSize] = '\0';

            return arr;
        }


        template<typename _Ty>
        struct FullTypeNameStorage
        {
            static constexpr auto fullTypeName{ NormalizeFullTypeName<_Ty>() }; /** Normalized full type name in std::array */
        };

        // full_type_name_storage help function

        /** namespace ::심볼의 마지막 부분의 위치를 반환합니다. 미확인시 npos 반환 */
        template<typename _Ty, typename _Fs = FullTypeNameStorage<_Ty>>
        constexpr size_t GetLastNsSimbolPos() noexcept
        {
            for (size_t i{ _Fs::fullTypeName.size() - 1 }; i != 1; --i)
            {
                if (_Fs::fullTypeName[i] == ':' && _Fs::fullTypeName[i - 1] == ':')
                {
                    return i;
                }
            }

            return std::string_view::npos;
        }

        template<typename _Ty, typename _Fs = FullTypeNameStorage<_Ty>>
        inline constexpr size_t GetNamespaceLen() noexcept
        {
            constexpr size_t pos{ GetLastNsSimbolPos<_Ty, _Fs>() };

            return (pos == std::string_view::npos) ? 0 : (pos - 1);
        }

        template<typename _Ty, typename _Fs = FullTypeNameStorage<_Ty>>
        constexpr auto ExtractNamespaceName() noexcept
        {
            constexpr size_t kLen{ GetNamespaceLen<_Ty>() };

            std::array<char, kLen + 1> arr{};
            for (size_t i{ 0 }; i < kLen; ++i)
            {
                arr[i] = _Fs::fullTypeName[i];
            }
            arr[kLen] = '\0';

            return arr;
        }

        template<typename _Ty, typename _Fs = FullTypeNameStorage<_Ty>>
        constexpr auto ExtractTypeName() noexcept
        {
            constexpr size_t kTyPos{ GetNamespaceLen<_Ty, _Fs>() ? (GetNamespaceLen<_Ty, _Fs>() + 2) : 0 };
            constexpr size_t kLen{ _Fs::fullTypeName.size() - kTyPos };

            std::array<char, kLen> arr{};
            for (size_t i{ 0 }; i < kLen; ++i)
            {
                arr[i] = _Fs::fullTypeName[kTyPos + i];
            }            
            arr[kLen - 1] = '\0';

            return arr;
        }

        template<size_t _Size>
        constexpr bool CompareName(const std::array<char, _Size>& name1, std::string_view name2) noexcept
        {
            if ((name1.size() - 1) != name2.length())
            {
                return false;
            }

            for (size_t i{ 0 }; i < name2.length(); ++i)
            {
                if (name1[i] != name2[i])
                {
                    return false;
                }
            }

            return true;
        }

        template<typename _Ty>
        struct type_name_traits
            : FullTypeNameStorage<_Ty>
        {
            static constexpr auto namespaceName{ ExtractNamespaceName<_Ty>() }; /** Normalized namespace name in std::array */
            static constexpr auto typeName{ ExtractTypeName<_Ty>() }; /** Normalized type name in std::array */

            /** 해당유형의 전체이름과 비교합니다. 컴파일 타임 비교시 이 함수를 이용할 것. */
            inline static constexpr bool CompareFullTypeName(std::string_view name) noexcept
            {
                using FullTypeNameStorage<_Ty>::fullTypeName;
                return CompareName(fullTypeName, name);
            }

            /** 해당유형의 네임스페이스 이름과 비교합니다. 컴파일 타임 비교시 이 함수를 이용할 것. */
            inline static constexpr bool CompareNamespaceName(std::string_view name) noexcept
            {
                return CompareName(namespaceName, name);
            }

            /** 해당유형의 이름과 비교합니다. 컴파일 타임 비교시 이 함수를 이용할 것. */
            inline static constexpr bool CompareTypeName(std::string_view name) noexcept
            {
                return CompareName(typeName, name);
            }
        };
    }

    template<typename _Ty>
    using type_name_traits = _internal_type_name::type_name_traits<_Ty>;
}
