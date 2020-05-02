/**
@file TypeTraits.h
@brief type_traits.h를 확장한 것
*/
#pragma once


#include "../CoreHelper.h"

#include <tuple>
#include <type_traits>


namespace stdext
{


	template < template <typename...> class base, typename derived>
	struct is_base_of_template_impl
	{
		template<typename... Ts>
		static constexpr std::true_type  test(const base<Ts...> *);
		static constexpr std::false_type test(...);
		using type = decltype(test(std::declval<derived*>()));
	};

	/**
	@brief template type check를 위한 함수. 컴파일 타임 보장됨
	@see https://stackoverflow.com/questions/34672441/stdis-base-of-for-template-classes
	*/
	template < template <typename...> class base, typename derived>
	using is_base_of_template = typename is_base_of_template_impl<base, derived>::type;

    
    //Function traits

    template<typename _FUNC_>
    struct function_traits;

    /**함수의 타입값을 이용해 정보를 획득합니다. function_traits<decltype(func)>*/
    template<typename _RETURN_TYPE_, typename... _ARG_TYPES_>
    struct function_traits<_RETURN_TYPE_(_ARG_TYPES_...)>
    {
        /**함수의 반환 값*/
        using return_type = _RETURN_TYPE_;

        /**전달인자의 갯수*/
        static constexpr std::size_t args_count = sizeof...(_ARG_TYPES_);

        template <std::size_t _ARG_INDEX_>
        struct argument_traits
        {
            static_assert(_ARG_INDEX_ < args_count, "invalid index parameter");

            using type = typename std::tuple_element<_ARG_INDEX_, std::tuple<_ARG_TYPES_...>>::type;
        };

    };

    //특수화
    //function ref
    template<typename _RETURN_TYPE_, typename... _ARG_TYPES_>
    struct function_traits<_RETURN_TYPE_(&)(_ARG_TYPES_...)>
        : public function_traits<_RETURN_TYPE_(_ARG_TYPES_...)>
    {

    };

    //function pointer 
    template<typename _RETURN_TYPE_, typename... _ARG_TYPES_>
    struct function_traits<_RETURN_TYPE_(*)(_ARG_TYPES_...)>
        : public function_traits<_RETURN_TYPE_(_ARG_TYPES_...)>
    {

    };

    //member function pointer
    template<typename _CLASS_TYPE_, typename _RETURN_TYPE_, typename... _ARG_TYPES_>
    struct function_traits<_RETURN_TYPE_(_CLASS_TYPE_::*)(_ARG_TYPES_...)>
        : public function_traits<_RETURN_TYPE_(_CLASS_TYPE_&, _ARG_TYPES_...)>
    {

    };


    //const member function pointer
    template<typename _CLASS_TYPE_, typename _RETURN_TYPE_, typename... _ARG_TYPES_>
    struct function_traits<_RETURN_TYPE_(_CLASS_TYPE_::*)(_ARG_TYPES_...) const>
        : public function_traits<_RETURN_TYPE_(_CLASS_TYPE_&, _ARG_TYPES_...)>
    {

    };

    //member object pointer
    template<typename _CLASS_TYPE_, typename _RETURN_TYPE_>
    struct function_traits<_RETURN_TYPE_(_CLASS_TYPE_::*)>
        : public function_traits<_RETURN_TYPE_(_CLASS_TYPE_&)>
    {

    };


    //sfinae return type

    /**
    전달되는 함수의 반환값으로 함수의 반환 값을 결정해야 될때 유용한 매크로 함수
    sfinaer기반의 결정. 반환 값이 void일 때 사용
    auto name(_arg_func_, someSig) -> decide_return_type_void(_arg_func_)
    */
#define decide_return_type_void(_func_) \
typename std::enable_if<\
    (std::is_same<typename stdext::function_traits<decltype(_func_)>::return_type, void>::value),\
        typename stdext::function_traits<decltype(_func_)>::return_type>::type

    /**
    전달되는 함수의 반환값으로 함수의 반환 값을 결정해야 될때 유용한 매크로 함수
    sfinaer기반의 결정. 반환 값이 void가 아닐때 사용
    auto name(_arg_func_, someSig) -> decide_return_type_not_void(_arg_func_)
    */
#define decide_return_type_not_void(_func_) \
typename std::enable_if<\
    !(std::is_same<typename stdext::function_traits<decltype(_func_)>::return_type, void>::value),\
        typename stdext::function_traits<decltype(_func_)>::return_type>::type
    
}