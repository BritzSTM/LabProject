#pragma once

#include <type_traits>
#include <tuple>


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


    // type

    template<typename _Ty>
    struct remove_cvref
    {
        using type = std::remove_cv_t<std::remove_reference_t<_Ty>>;
    };

    template<typename _Ty>
    using remove_cvref_t = typename remove_cvref<_Ty>::type;


    // const pointer

    template<typename _Ty>
    struct is_const_pointer : std::false_type {};

    template<typename _Ty>
    struct is_const_pointer<const _Ty*> : std::true_type {};

    template<typename _Ty>
    struct is_const_pointer<const _Ty* const> : std::true_type {};

    template<typename _Ty>
    inline constexpr bool is_const_pointer_v = is_const_pointer<_Ty>::value;


    namespace _internal_type_traits_ext
    {
        template<typename _Ty>
        struct identity
        {
            using type = _Ty;
        };
    }

    /** 순수한 자료형을 획득한다. */
    template<typename _Ty, typename _RemovedCvrefTy = remove_cvref_t<_Ty>>
    struct seed_traits
        : std::conditional_t<
        std::is_pointer_v<_RemovedCvrefTy>,
        seed_traits<std::remove_pointer_t<_RemovedCvrefTy>>,
        std::conditional_t<std::is_array_v<_RemovedCvrefTy>, seed_traits<std::decay_t<_RemovedCvrefTy>>, _internal_type_traits_ext::identity<remove_cvref_t<_RemovedCvrefTy>>>
        >
    {

    };

    /** seed_traits helper */
    template<typename _Ty>
    using seed_traits_t = typename seed_traits<_Ty>::type;


    //function ptr

    template <typename _Ty, bool b = std::is_pointer_v<_Ty>>
    struct is_function_ptr : std::false_type {};

    template <typename _Ty>
    struct is_function_ptr<_Ty, true>
        : std::integral_constant<bool, std::is_function_v<std::remove_pointer_t<_Ty>>> {};

    template<typename _Ty>
    inline constexpr bool is_function_ptr_v{ is_function_ptr<_Ty>::value };


    // function

    namespace _internal_type_traits_ext
    {
        template<typename _Ty>
        struct function_args_traits;

        template<typename _Res, typename... _Args>
        struct function_args_traits<_Res(_Args...)>
        {
            static constexpr std::size_t arg_count = sizeof...(_Args);

            template <std::size_t _argIndex>
            struct argument_traits
            {
                static_assert(_argIndex < args_count, "invalid index parameter");

                using type = typename std::tuple_element<_argIndex, std::tuple<_Args...>>::type;
            };
        };
    }

    template<typename _Ty>
    struct function_traits;

    template<typename _Res, typename... _Args>
    struct function_traits<_Res(_Args...)>
        : _internal_type_traits_ext::function_args_traits<_Res(_Args...)>
    {
        using return_type = _Res;
    };
}
