#include "stdafx.h"
#include "IComponent.h"

namespace Core
{
	IComponent* IComponent::Create(
		const SComponentNameInfo& info, 
		const SComponentCreateParameters& parameters)
	{
		auto& registry = _GetComponentRegistry();

		ComponentCreateFuncType pCreate = registry[info];

		if (pCreate != nullptr)
		{
			IComponent* pComponent = pCreate(parameters);
			return pComponent;
		}

		return nullptr;
	}
}

//Component Reg Macro ÇÊ¿ä
REGISTER_COMPONENT(Core::CDummyComponent1, TEXTL("DummyComponent1"))
REGISTER_COMPONENT(Core::CDummyComponent2, TEXTL("DummyComponent2"))