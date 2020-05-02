/**
@file ComponentBaseType.h
@brief Component System의 구성을 위한 기본 타입 정의 및 Help 함수 구현
@date 2018-10-16  오후 20:58:04
*/


#pragma once


#include "EnvironmentValue.h"
#include "Container/string.h"
#include "Common/Exception.h"


#if   SELECTED_CONTAINER_TYPE == CONTAINER_TYPE_STL
#include <unordered_map>
#elif SELECTED_CONTAINER_TYPE == CONTAINER_TYPE_TBB
#include "Container/ConcurrencyUnorderedMap.h"
#else
    COMPILE_ERROR("Not Imple Container Type");
#endif


namespace Core
{
	//Forward declaration
	struct IComponent;


	/**
	ComponentNameInfo이름 정보를 보관합니다.
	*/
	using SComponentNameInfo = SCompileTimeHashFromString;


	/**
	Component생성을 위한 Parameters들의 정보를 담는 구조체
	*/
	struct SComponentCreateParameters
	{

	};


	/**
	Component 생성을 하는 함수들을 가르킬 수 있는 함수 포인터 타입
	*/
	using ComponentCreateFuncType = IComponent* (*)(const SComponentCreateParameters&);


	/**
	해당 타입의 컴포넌트를 생성하는 템플릿 함수
	*/
	template<typename _TYPE>
	IComponent* _CreateComponent(const SComponentCreateParameters& parameters)
	{
		return new _TYPE(parameters);
	}


	//concurrent_unordered_map을 이용한 ComponentRegistry Type 정의
#if   SELECTED_CONTAINER_TYPE == CONTAINER_TYPE_STL
	using ComponentRegistry = std::unordered_map<SComponentNameInfo, ComponentCreateFuncType>;
#elif SELECTED_CONTAINER_TYPE == CONTAINER_TYPE_TBB
	using ComponentRegistry = Container::concurrent_unordered_map<SComponentNameInfo, ComponentCreateFuncType>;
#else
	COMPILE_ERROR("Not Imple Container Type");
#endif


	/**
	ComponentRegistry를 획득합니다
	*/
	FLINE ComponentRegistry& _GetComponentRegistry() noexcept
	{
		static ComponentRegistry reg;
		return reg;
	}


	/**
	@brief 	Component를 ComponentRegistry에 등록시키는 대행 객체
	@date 2018-10-16  오후 21:29:44
	@version 1.0v
	*/
	template<typename _COMPONENT_TYPE>
	class _CComponentRegister
	{
	public:
		/**전달받은 SComponentNameInfo로 ComponentRegistry에 Compoent를 등록합니다*/
		static _CComponentRegister& Register(const SComponentNameInfo& infoKey)
		{
			//단 한번만 등록할 수 있도록 함
			static _CComponentRegister registeredCompoent(infoKey);
			return registeredCompoent;
		}

	private:
		_CComponentRegister(const SComponentNameInfo& infoKey)
		{
			auto& componentRegistry = _GetComponentRegistry();
			ComponentCreateFuncType pCreateFunc = _CreateComponent<_COMPONENT_TYPE>;
			
			const auto cEndIter = componentRegistry.cend();
			const auto cResIter = componentRegistry.find(infoKey);
			
			if (cEndIter == cResIter)
			{
				auto resInsert = componentRegistry.insert(std::make_pair(infoKey, pCreateFunc));
				
				//삽입 실패 예외 발생
				if (resInsert.second == false)
				{
					throw std::RuntimeErrorException("ComponentRegistry insert failed");
				}
			}
			else
			{
				//중복객체 존제 예외 발생
				throw std::RuntimeErrorException("ComponentRegistry insert duplication");
			}
		}

		//일반적으로 생성 목하도록 삭제
		_CComponentRegister() = delete;
		_CComponentRegister(const _CComponentRegister&) = delete;
		_CComponentRegister& operator=(const _CComponentRegister&) = delete;
	};

}

/**컴포넌트 타입으로 Who 정보를 생성합니다. 반드시 Class 내부에 정의 해야 합니다.*/
#define MAKE_COMPONENT_WHO(COMPONENT_TYPE)                      \
public:                                                         \
virtual Core::SComponentNameInfo Who() const noexcept override  \
{                                                               \
	return __m_info;                                            \
}                                                               \
static Core::SComponentNameInfo WhoType() noexcept              \
{                                                               \
	return __m_info;                                            \
}                                                               \
private:                                                        \
    static const Core::SComponentNameInfo __m_info                     


/**
클래스 타입정보 이름을 전달 받아 컴포넌트를 등록합니다. 
항상 CPP파일에서 사용해야 합니다. 
또한 Name은 컴포넌트 생성시 요구하는 이름을 정의 하는 것 입니다.
*/
#define REGISTER_COMPONENT(TYPE, NAME)                          \
const Core::SComponentNameInfo TYPE::__m_info = TEXTL(#TYPE);   \
namespace Core                                                  \
{                                                               \
namespace _ComponentReg                                         \
{                                                               \
namespace                                                       \
{                                                               \
template<typename _COMPONENT_TYPE>                              \
    struct Registration;                                        \
                                                                \
    template<>                                                  \
    struct Registration<TYPE>                                   \
    {                                                           \
        static const _CComponentRegister<TYPE>& reg;            \
    };                                                          \
                                                                \
   const _CComponentRegister<TYPE>& Registration<TYPE>::reg =   \
                     _CComponentRegister<TYPE>::Register(NAME); \
}                                                               \
}                                                               \
}