/*
    ! 이 헤더파일을 직접 포함하지 말 것.
*/
#pragma once


namespace fd::refl
{
    /*
        @brief 타입이름이 포함된 문자열을 획득합니다. 라이브러리 사용자는 이 함수를 직접 호출하지 말 것.
        @detail
         이 함수는 정의는 규칙을 위반하면서 정의했습니다. MSVC의 __FUNCSIG__ 매크로는 해당함수의 서명을 문자열로 획득하기 때문입니다.
        만약 어떤 namespace에 이 함수가 정의된다면 namespace가 포함된 더 길다란 문자열이 생성됩니다.
    */
    template<typename _Ty>
    constexpr auto __cdecl _rn_cl(int = 0) noexcept
    {
        /*
            clang __PRETTY_FUNCTION__는 다음과 같은 형식을 가진다.
            auto __cdecl fd::refl::_rn_cl(int) [_Ty = fd::ESomeType]
            auto __cdecl fd::refl::_rn_cl(int) [_Ty = std::basic_string_view<char, std::char_traits<char> >]
        */
        return __PRETTY_FUNCTION__;
    }

    template<typename _Ty>
    struct base_type_name_traits<_Ty, ClangCompilerTag>
    {
        static constexpr std::string_view rawName{ _rn_cl<_Ty>() };
        static constexpr std::string_view fixedPrefix{ "auto __cdecl fd::refl::_rn_cl(int) [_Ty = " };
        static constexpr std::string_view fixedSubfix{ "]" };

        static constexpr size_t kDynamicPrefixLen{ 0 };
    };
}
