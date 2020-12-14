#pragma once

#include <type_traits>


namespace fd
{
    // enum

    template <typename _Ty, bool b = std::is_enum_v<_Ty>>
    struct is_scoped_enum : std::false_type {};

    template <typename _Ty>
    struct is_scoped_enum<_Ty, true>
        : std::integral_constant<bool, !std::is_convertible_v<_Ty, std::underlying_type_t<_Ty>>> {};

    template<typename _Ty>
    inline constexpr bool is_scoped_enum_v = is_scoped_enum<_Ty>::value;


    //function ptr

    template <typename _Ty, bool b = std::is_pointer_v<_Ty>>
    struct is_function_ptr : std::false_type {};

    template <typename _Ty>
    struct is_function_ptr<_Ty, true>
        : std::integral_constant<bool, std::is_function_v<std::remove_pointer_t<_Ty>>> {};

    template<typename _Ty>
    inline constexpr bool is_function_ptr_v{ is_function_ptr<_Ty>::value };


    // type

    template<typename _Ty>
    struct remove_cvref
    {
        using type = std::remove_cv_t<std::remove_reference_t<_Ty>>;
    };

    template<typename _Ty>
    using remove_cvref_t = typename remove_cvref<_Ty>::type;

    namespace _internal_type_traits_ext
    {
        template<typename _Ty>
        struct identity
        {
            using type = _Ty;
        };
    }

    /** 순수한 자료형을 획득한다. */
    template<typename _Ty>
    struct seed_traits
        : std::conditional_t<
        std::is_pointer_v<_Ty>,
        seed_traits<std::remove_pointer_t<_Ty>>,
        std::conditional_t<std::is_array_v<_Ty>, seed_traits<std::decay_t<_Ty>>, _internal_type_traits_ext::identity<remove_cvref_t<_Ty>>>
        >
    {

    };

    /** seed_traits helper */
    template<typename _Ty>
    using seed_traits_t = typename seed_traits<_Ty>::type;
}
