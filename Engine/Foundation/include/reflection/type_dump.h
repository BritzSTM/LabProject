#pragma once

#include <string>


namespace fd::refl
{
    //추가된 저수준 인터페이스들

    template<typename _OutStemIt, typename _Ty, typename _Fn>
    void to_string_to(_OutStemIt dest, const _Ty v, _Fn&& f)
    {
        // 타입 정보를 획득

        // 타입정보에서 하위 맴버들을 순회하면서 fd 타입 수준이면 테이터를 획득

        // 함수 f를 통해 포맷된 값을 출력으로 보내기
    }

    template<typename _OutStemIt, typename _Ty, typename _Fn>
    void to_string_to(_OutStemIt dest, const _Ty* const v, _Fn&& f);

    template<typename _OutStemIt, typename _Ty, typename _Fn>
    void to_string_to(_OutStemIt dest, const _Ty v, _Fn&& f, std::nothrow_t) noexcept;

    template<typename _OutStemIt, typename _Ty, typename _Fn>
    void to_string_to(_OutStemIt dest, const _Ty* const v, _Fn&& f, std::nothrow_t) noexcept;


    //저수준 인터페이스들을 랩핑한 인터페이스

    template<typename _Ty>
    std::string to_string(const _Ty v);

    template<typename _Ty, typename _Fn>
    std::string to_string(const _Ty v, _Fn&& f);

    template<typename _Ty>
    std::string to_string(const _Ty* const v);

    template<typename _Ty, typename _Fn>
    std::string to_string(const _Ty* const v, _Fn&& f);

    template<typename _Ty>
    std::string to_string(const _Ty v, std::nothrow_t) noexcept;

    template<typename _Ty, typename _Fn>
    std::string to_string(const _Ty v, _Fn&& f, std::nothrow_t) noexcept;

    template<typename _Ty>
    std::string to_string(const _Ty* const v, std::nothrow_t) noexcept;

    template<typename _Ty, typename _Fn>
    std::string to_string(const _Ty* const v, _Fn&& f, std::nothrow_t) noexcept;
}
