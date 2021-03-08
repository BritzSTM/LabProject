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

        template<typename _Ty, size_t _depth>
        struct get_decl_type_depth_basis
        {
            constexpr static size_t depth{ _depth };
        };

        template<typename _Ty, size_t _depth, typename _RemovedCvTy = std::remove_cv_t<_Ty>>
        struct get_decl_type_depth_impl
            : std::conditional_t<std::is_reference_v<_RemovedCvTy>, get_decl_type_depth_impl<std::remove_reference_t<_RemovedCvTy>, _depth + 1>,
            std::conditional_t<std::is_pointer_v<_RemovedCvTy>, get_decl_type_depth_impl<std::remove_pointer_t<_RemovedCvTy>, _depth + 1>,
            std::conditional_t<std::is_array_v<_RemovedCvTy>, get_decl_type_depth_impl<std::remove_pointer_t<std::decay_t<_RemovedCvTy>>, _depth + 1>, get_decl_type_depth_basis<_RemovedCvTy, _depth>>>>
        {

        };

        template<typename _Ty, size_t _curr, size_t _depth, typename _RemovedCvTy = std::remove_cv_t<_Ty>>
        struct decl_type_level_impl
            : std::conditional_t<(_curr == _depth), identity<_Ty>,
            std::conditional_t<std::is_reference_v<_RemovedCvTy>, decl_type_level_impl<std::remove_reference_t<_RemovedCvTy>, _curr + 1, _depth>,
            std::conditional_t<std::is_pointer_v<_RemovedCvTy>, decl_type_level_impl<std::remove_pointer_t<_RemovedCvTy>, _curr + 1, _depth>,
            std::conditional_t<std::is_array_v<_RemovedCvTy>, decl_type_level_impl<std::remove_pointer_t<std::decay_t<_RemovedCvTy>>, _curr + 1, _depth>, identity<_Ty>>>>>
        {

        };
    }

    /** 선언타입의 선언구조 깊이를 획득합니다. */
    template<typename _Ty>
    struct get_decl_type_depth
        : _internal_type_traits_ext::get_decl_type_depth_impl<_Ty, 0> { };

    /** 전달된 타입과 깊이를 이용해 해당 깊이에 맞는 타입을 반환합니다. */
    template<typename _Ty, size_t _depth>
    struct decl_type_level : _internal_type_traits_ext::decl_type_level_impl<_Ty, 0, _depth> { };

    /** seed type에 관련된 특성을 추론 */
    template<typename _Ty>
    struct seed_traits final
    {
        static constexpr size_t depth{ get_decl_type_depth<_Ty>::depth }; // seed type에 도달하기 위한 깊이

        using type = typename decl_type_level<_Ty, depth>::type;; // Seed type

        // 해당 레벨에 대한 타입. 최상층은 0을 의미한다
        template<size_t _depth>
        using level_t = typename decl_type_level<_Ty, _depth>::type;
    };

    /** seed_traits helper. 즉시 seed type을 반환 */
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
            static constexpr std::size_t args_count = sizeof...(_Args);

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


    // Array

    /** 배열의 차원을 획득합니다. 0일 경우 배열이 아닙니다. 최대 8차원까지 지원합니다. */
    template<typename _Ty>
    struct get_array_dim
    {
        static constexpr size_t dim = 0;
    };

    template<typename _Ty, size_t _a>
    struct get_array_dim<_Ty[_a]>
    {
        static constexpr size_t dim = 1;
    };

    template<typename _Ty, size_t _a, size_t _b>
    struct get_array_dim<_Ty[_a][_b]>
    {
        static constexpr size_t dim = 2;
    };

    template<typename _Ty, size_t _a, size_t _b, size_t _c>
    struct get_array_dim < _Ty[_a][_b][_c]>
    {
        static constexpr size_t dim = 3;
    };

    template<typename _Ty, size_t _a, size_t _b, size_t _c, size_t _d>
    struct get_array_dim<_Ty[_a][_b][_c][_d]>
    {
        static constexpr size_t dim = 4;
    };

    template<typename _Ty, size_t _a, size_t _b, size_t _c, size_t _d, size_t _e>
    struct get_array_dim<_Ty[_a][_b][_c][_d][_e]>
    {
        static constexpr size_t dim = 5;
    };

    template<typename _Ty, size_t _a, size_t _b, size_t _c, size_t _d, size_t _e, size_t _f>
    struct get_array_dim<_Ty[_a][_b][_c][_d][_e][_f]>
    {
        static constexpr size_t dim = 6;
    };

    template<typename _Ty, size_t _a, size_t _b, size_t _c, size_t _d, size_t _e, size_t _f, size_t _g>
    struct get_array_dim<_Ty[_a][_b][_c][_d][_e][_f][_g]>
    {
        static constexpr size_t dim = 7;
    };

    template<typename _Ty, size_t _a, size_t _b, size_t _c, size_t _d, size_t _e, size_t _f, size_t _g, size_t _h>
    struct get_array_dim<_Ty[_a][_b][_c][_d][_e][_f][_g][_h]>
    {
        static constexpr size_t dim = 8;
    };

    /** get_array_dim helper */
    template<typename _Ty>
    inline constexpr size_t get_array_dim_v = get_array_dim<_Ty>::dim;
}
