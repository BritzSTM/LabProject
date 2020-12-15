#pragma once

#include <string_view>


namespace fd::refl
{
    namespace _internal_type_name_clang
    {
        /*
            __PRETTY_FUNCTION__는 다음과 같은 형식을 가진다.
            std::string_view GetTypeFullName() [_Ty = fd::ESomeType]
            std::string_view GetTypeFullName() [_Ty = std::basic_string_view<char, std::char_traits<char> >]
        */

        constexpr std::string_view ExtractTypeFullNameImpl(std::string_view fnSig) noexcept
        {
            using namespace std;

            if (size_t leftABPos{ fnSig.find("[_Ty = ") }; leftABPos != fnSig.npos)
            {
                // 7은 [_Ty = 의 길이
                fnSig.remove_prefix(leftABPos + 7);
            }
            else
            {
                return {};
            }

            // 마지막 기호( ']' )만 제거하면 된다.
            fnSig.remove_suffix(1);

            return fnSig;
        }

        template<typename _Ty>
        constexpr std::string_view ExtractTypeFullName() noexcept
        {
            return ExtractTypeFullNameImpl(__PRETTY_FUNCTION__);
        }
    }

    template<typename _Ty>
    constexpr std::string_view GetTypeFullName() noexcept
    {
        return _internal_type_name_clang::ExtractTypeFullName<_Ty>();
    }
}
