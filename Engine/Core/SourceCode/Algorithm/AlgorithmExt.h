/**
@file AlgorithmExt.h
@brief 공통으로 사용될 수 있는 템플릿 알고리즘 모음
@remarks 
@date 2018-08-20  오전 11:57:37
@section License

*/
#pragma once


#include "CoreHelper.h"
#include "Common\BaseTypes.h"
#include "Type/TypeTraits.h"

#include <algorithm>


namespace stdext
{

	/**
	@fn FLINE auto InsertLoop(_Iter it, const _IndexType count, _InsertFunc pFunc) -> bool
	@brief 
	반복자를 시작으로 count까지 반복자 데이터를 pFunc를 호출하여 데이터를 삽입합니다. 
	단 pFunc가 false를 반환 하면 함수도 종료됩니다.
	@remarks 
	@param it 시작 반복자 
	@param count 삽입 갯수
	@param pFunc 삽입 대행 객체 반환 값으로는 bool
	@return True 객체를 삽입할 때 pFunc로부터 false가 반환 되지 않음 | False 객체를 삽입할 때 pFunc로부터 flase가 반환 됨
	@todo  _InsertFunc 객체가 참조형, 일반일 때의 퍼포먼스 테스트를 한번 해보는 것이 좋을듯 함 
	*/
	template <typename _Iter, typename _InsertFunc, typename _IndexType>
	FLINE auto InsertLoop(_Iter it, const _IndexType count, _InsertFunc pFunc) -> bool
	{
		for (auto i = 0; i < count; ++i)
		{
			if (pFunc(*(it++)) == false)
				return false;
		}
		
		return true;
	}

	template <typename _Iter, typename _InsertFunc, typename _IndexType>
	FLINE auto MoveLoop(_Iter it, const _IndexType count, _InsertFunc pFunc) -> bool
	{
		using rawType = decltype(*it); //this type is const int&. because *it is lvalue
		using type = typename std::remove_const<std::remove_reference<rawType>::type>::type;//const int& -> const int -> int

		for (auto i = 0; i < count; ++i)
		{
			if (pFunc(std::move(const_cast<type&>(*(it++)))) == false)
				return false;
		}

		return true;
	}

    //Overload된 함수를 위한 Macro
#define WrapFnToLambda(_FUNC_) \
    ([] (auto&&... args) -> decltype(auto) {return _FUNC_(std::forward<decltype(args)>(args)...);})

}