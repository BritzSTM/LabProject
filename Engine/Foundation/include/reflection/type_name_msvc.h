#pragma once

#include <string_view>
#include <array>


namespace fd::refl
{
    namespace _internal_type_name_msvc
    {
        /*
            컴파일 완료시 __FUNCSIG__는 다음과 같은 형식을 지닌다.

            class std::basic_string_view<char, std::char_traits<char>> __cdecl GetFullName<enum fd::ABType>(void) noexcept
            class std::basic_string_view<char, std::char_traits<char>> __cdecl GetFullName<class std::basic_string_view<char, std::char_traits<char>>>(void) noexcept

            컴파일 이전이라면 intellisense엔진은 enum같은 형식을 생략하고 함수 전달인자 void를 생략
        */
        constexpr std::string_view ExtractTypeFullNameImpl(std::string_view fnSig) noexcept
        {
            using namespace std;

            if (size_t leftPos{ fnSig.find("ExtractTypeFullName<") }; leftPos != fnSig.npos)
            {
                fnSig.remove_prefix(leftPos + 20);

                // msvc는 컴파일 후 일부 유형이 포함된다. 컴파일러별 공통적으로 포맷된 문자열 제공을 위해 제거한다
                constexpr array<const char*, 4> pszTypes{ "enum ", "class ", "struct ", "union " };
                for (auto i{ 0 }; i < pszTypes.size(); ++i)
                {
                    if (fnSig.find(pszTypes[i]) != fnSig.npos)
                    {
                        fnSig.remove_prefix(char_traits<char>::length(pszTypes[i]));
                        break;
                    }
                }
            }
            else
            {
                return {};
            }

            if (size_t rightPos{ fnSig.rfind(">() noexcept") }; rightPos != fnSig.npos)
            {
                fnSig.remove_suffix(12);
            }
            else if (size_t rightPos{ fnSig.rfind(">(void) noexcept") }; rightPos != fnSig.npos)
            {
                fnSig.remove_suffix(16);
            }
            else
            {
                return {};
            }

            return fnSig;
        }

        template<typename _Ty>
        constexpr std::string_view ExtractTypeFullName() noexcept
        {
            return ExtractTypeFullNameImpl(__FUNCSIG__);
        }
    }

    template<typename _Ty>
    constexpr std::string_view GetTypeFullName() noexcept
    {
        return _internal_type_name_msvc::ExtractTypeFullName<_Ty>();
    }
}
