/**
@file APIObjectPtr.h
@brief 
DLL로 Exports된 객채들의 해제 인터페이스 및 
RAII를 위한 템플릿 객체를 제공합니다.
@date 2018-10-12  오후 23:47:14
*/


#pragma once


#include "CoreHelper.h"
#include <memory>
#include <functional>


namespace Core
{
	/**
	CreateAPIObject 정적 메소드로 생성된 객체를 제거하기 위한 인터페이스 입니다.
	*/
	struct IAPIObjectRelease
	{
		virtual ~IAPIObjectRelease() {}

		/**API로 할당된 객체를 제거하는 인터페이스 입니다.*/
		virtual void APIObjectRelease() = 0;
	};

	/**
	std::shared_ptr을 기반으로 하며 할당과 소멸자만 생성함수로 지정해 준다.
	해당 APIObjectPtr을 사용하려면 제공되는 객체는 IAPIObjectRelease를 상속하고 
	CreateAPIObject를 정적으로 구현해야 한다.
	*/
	template<typename _TYPE>
	using APIObjectPtr = std::shared_ptr<_TYPE>;

	/**
	APIObjectPtr을 생성합니다.
	*/
	template<typename _TYPE, typename... _TYPES>
	[[nodiscard]] FLINE APIObjectPtr<_TYPE>
	MakeAPIPtr(_TYPES&&... Args)
	{
		const auto rawPtr = _TYPE::CreateAPIObject(std::forward<_TYPES>(Args)...);
		std::shared_ptr<_TYPE> pResultPtr(rawPtr, std::mem_fn(&_TYPE::APIObjectRelease));

		return pResultPtr;
	}
}