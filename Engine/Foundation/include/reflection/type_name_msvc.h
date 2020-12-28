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
    constexpr auto __cdecl _rn_vs(int = 0) noexcept
    {
        return __FUNCSIG__;
    }

    namespace _internal_type_name_msvc
    {
        constexpr size_t GetDynamicPrefixLen(std::string_view rawName, std::string_view fixedPrefix) noexcept
        {
            using namespace std;

            // 공백이 포함된 동적 접두사들
            constexpr array<string_view, 4> dyPrefixs{ "enum ", "class ", "struct ", "union " };

            size_t dyLen{ 0 };
            for (const auto& dy : dyPrefixs)
            {
                bool found{ true };

                //fixedPrefix 이후에 dy를 찾는다.
                for (size_t dyIdx{ 0 }, rawIdx{ fixedPrefix.size() };
                    dyIdx < dy.size(); ++dyIdx, ++rawIdx)
                {
                    if (dy[dyIdx] != rawName[rawIdx])
                    {
                        found = false;
                        break;
                    }
                }            

                if (found)
                {
                    dyLen = dy.size();
                    break;
                }
            }

            return dyLen;
        }
    }

    template<typename _Ty>
    struct base_type_name_traits<_Ty, MsvcCompilerTag>
    {
        static constexpr std::string_view rawName{ _rn_vs<_Ty>() };
        static constexpr std::string_view fixedPrefix{ "auto __cdecl fd::refl::_rn_vs<" };
        static constexpr std::string_view fixedSubfix{ ">(int) noexcept" };

        static constexpr size_t kDynamicPrefixLen{ _internal_type_name_msvc::GetDynamicPrefixLen(rawName, fixedPrefix) };
    };
}
