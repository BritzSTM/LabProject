/**
Tuple을 효율적으로 사용하기 위한 함수들의 집합
stdext로 확장되어 사용된다.
*/
#pragma once


#include "../CoreHelper.h"
#include "../Type/TypeTraits.h"

#include <tuple>


namespace stdext
{

    namespace _internal
    {

        template<typename _FN_, typename _TUPLE_TYPE_, size_t... _SEQ_, typename... _SUB_DATA_TYPES_>
        FLINE auto _ExpandLeftTupleToFuncImpl(
            _FN_& func, _TUPLE_TYPE_&& tupleData, std::index_sequence<_SEQ_...>, _SUB_DATA_TYPES_&&... subDatas)
            -> decide_return_type_not_void(func)
        {
            using namespace std;

            return func(
                get<_SEQ_>(forward<_TUPLE_TYPE_>(tupleData))...,
                forward<_SUB_DATA_TYPES_>(subDatas)...
            );

        }

        template<typename _FN_, typename _TUPLE_TYPE_, size_t... _SEQ_, typename... _SUB_DATA_TYPES_>
        FLINE auto _ExpandLeftTupleToFuncImpl(
            _FN_& func, _TUPLE_TYPE_&& tupleData, std::index_sequence<_SEQ_...>, _SUB_DATA_TYPES_&&... subDatas)
            -> decide_return_type_void(func)
        {
            using namespace std;

            func(
                get<_SEQ_>(forward<_TUPLE_TYPE_>(tupleData))...,
                forward<_SUB_DATA_TYPES_>(subDatas)...
            );

        }

        template< typename _OBJ_TYPE_, typename _TUPLE_TYPE_, size_t... _SEQ_, typename... _SUB_DATA_TYPES_>
        FLINE _OBJ_TYPE_
        _ExpandLeftTupleToCreateObjImpl(_TUPLE_TYPE_&& tupleData, std::index_sequence<_SEQ_...>, _SUB_DATA_TYPES_&&... subDatas)
        {
            using namespace std;

            return _OBJ_TYPE_(
                get<_SEQ_>(forward<_TUPLE_TYPE_>(tupleData))..., 
                forward<_SUB_DATA_TYPES_>(subDatas)...
            );

        }

    }


#define __ForwardExpandLeftTupleToFuncImpl__ \
stdext::_internal::_ExpandLeftTupleToFuncImpl(\
        func,\
        std::forward<std::tuple<TupleTypes...>>(tupleData), std::index_sequence_for<TupleTypes...>(),\
        std::forward<SubDataTypes>(subDatas)...)\

    /**tuple을 입력받은 함수에 전달인자로 나열하여 호출 합니다*/
    template<typename Func, typename... TupleTypes, typename... SubDataTypes>
    auto expand_left_tuple_to_func(Func& func, std::tuple<TupleTypes...>& tupleData, SubDataTypes&&... subDatas)
        -> decide_return_type_not_void(func)
    {
        return __ForwardExpandLeftTupleToFuncImpl__;

    }

    /**tuple을 입력받은 함수에 전달인자로 나열하여 호출 합니다*/
    template<typename Func, typename... TupleTypes, typename... SubDataTypes>
    auto expand_left_tuple_to_func(Func& func, std::tuple<TupleTypes...>& tupleData, SubDataTypes&&... subDatas)
        -> decide_return_type_void(func)
    {
        __ForwardExpandLeftTupleToFuncImpl__;
        return;

    }

    /**tuple을 입력받은 함수에 전달인자로 나열하여 호출 합니다*/
    template<typename Func, typename... TupleTypes, typename... SubDataTypes>
    auto expand_left_tuple_to_func(Func& func, std::tuple<TupleTypes...>&& tupleData, SubDataTypes&&... subDatas)
        -> decide_return_type_not_void(func)
    {
        return __ForwardExpandLeftTupleToFuncImpl__;

    }

    /**tuple을 입력받은 함수에 전달인자로 나열하여 호출 합니다*/
    template<typename Func, typename... TupleTypes, typename... SubDataTypes>
    auto expand_left_tuple_to_func(Func& func, std::tuple<TupleTypes...>&& tupleData, SubDataTypes&&... subDatas)
        -> decide_return_type_void(func)
    {
        __ForwardExpandLeftTupleToFuncImpl__;
        return;

    }

#undef __ForwardExpandLeftTupleToFuncImpl__

#define __ForwardExpandLeftTupleToCreateObjImpl__ \
    stdext::_internal::_ExpandLeftTupleToCreateObjImpl<T>(\
        std::forward<std::tuple<TupleTypes...>>(tupleData), std::index_sequence_for<TupleTypes...>(),\
        std::forward<SubDataTypes>(subDatas)...)

    /**
    Tuple과 Subdatas를 이용해 객체를 생성합니다.
    함수를 호출하기전 생성할 자료형이 결정되어야 합니다.
    expand_left_tuple_to_create_obj<SomeType>(arugments)
    */
    template<typename T, typename... TupleTypes, typename... SubDataTypes>
    T expand_left_tuple_to_create_obj(std::tuple<TupleTypes...>& tupleData, SubDataTypes&&... subDatas)
    {
        return __ForwardExpandLeftTupleToCreateObjImpl__;

    }

    /**
    Tuple과 Subdatas를 이용해 객체를 생성합니다.
    함수를 호출하기전 생성할 자료형이 결정되어야 합니다.
    expand_left_tuple_to_create_obj<SomeType>(arugments)
    */
    template<typename T, typename... TupleTypes, typename... SubDataTypes>
    T expand_left_tuple_to_create_obj(std::tuple<TupleTypes...>&& tupleData, SubDataTypes&&... subDatas)
    {
        return __ForwardExpandLeftTupleToCreateObjImpl__;

    }
       
#undef __ForwardExpandLeftTupleToCreateObjImpl__

}