



#pragma once


#include "Container/string.h"
#include "ComponentBaseTypes.h"


namespace Core
{
	/**
	@brief Compoent의 기본적인 인터페이스 입니다.
	@date 2018-10-16  오후 22:48:35
	@version 0.7v
	*/
	struct IComponent
	{
		virtual ~IComponent() {}
		
		/**Component Name을 반환 합니다*/
		virtual SComponentNameInfo Who() const noexcept = 0;

		/**Component이름 param을 이용해 생성합니다.*/
		static IComponent* Create(
			const SComponentNameInfo& info,
			const SComponentCreateParameters& ㅔarameters);
	};

	template<typename _CAST_COMPONENT_TYPE>
	struct c_c
	{
		static _CAST_COMPONENT_TYPE* Cast(IComponent* pComponent) noexcept
		{
			if (_CAST_COMPONENT_TYPE::WhoType() == pComponent->Who())
			{
				return static_cast<_CAST_COMPONENT_TYPE*>(pComponent);
			}
			else
			{
				return nullptr;
			}
		}
	};
	
#define component_cast(X, Y) (X->Who() == Y::WhoType()) ? (static_cast<Y*>(X)) : (nullptr)

	/**
	Test용 Component1
	*/
	class CDummyComponent1
		: public IComponent
	{
		MAKE_COMPONENT_WHO(CDummyComponent1);

	public:
		virtual ~CDummyComponent1() {}

		CDummyComponent1(const SComponentCreateParameters& param)
		{
			
		}
	};

	/** 
	Test용 Component2
	*/
	class CDummyComponent2
		: public IComponent
	{
		MAKE_COMPONENT_WHO(CDummyComponent2);

	public:
		virtual ~CDummyComponent2() {}

		CDummyComponent2(const SComponentCreateParameters& param)
		{

		}
	};
}