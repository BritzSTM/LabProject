#include "stdafx.h"
#include "MemoryCoreSystem.h"


#include <memory>


namespace Core
{

    bool CCoreMemorySystem::InitSystem()
    {
        //긴급 메모리 
        InitEmergencyMemoryFromEnvValue();

        //New Handleing
        InitMemoryHandlerFromEnvValue();

        return true;
    }

    void CCoreMemorySystem::ReleaseSystem()
    {
        //기존 Handler를 유지한다.
    }

    void CCoreMemorySystem::InitEmergencyMemoryFromEnvValue()
    {
        _GetEmergencyMemoryBase(g_kSystemEmergencyMemorySize);
    }
    
    CEmergencyMemory& CCoreMemorySystem::_GetEmergencyMemoryBase(const size_t& secureSize)
    {
        static auto emergencyMemory = std::make_unique<CEmergencyMemory>(secureSize);
        return (*emergencyMemory.get());
    }

    void CCoreMemorySystem::InitMemoryHandlerFromEnvValue()
    {
        //사용여부 env 값 확인
        if (g_kSystemMemoryHandlerEnabled)
        {
            CMemoryHandlerSystem::InitSystem(
                EMemHandlerType::Enum::EmergencyMemoryHandler, &GetEmergencyMemory());
        }
    }
}