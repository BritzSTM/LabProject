/**항상 맨처음 초기화 되어야 하는 시스템*/
#pragma once

/**모든 new 연산을 조정성있는 연산으로 변경하기 위해 포함시킴*/
#include <tbb/tbbmalloc_proxy.h>

//user
#include "CoreHelper.h"
#include "Common/BaseType_Integers.h"
#include "Memory/EmergencyMemory.h"
#include "Memory/MemoryHandlerSystem.h"


namespace Core
{
    /**
    항상 이 시스템은 최우선적으로 초기화 되어야 합니다.
    */
    class CCoreMemorySystem final
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(CCoreMemorySystem);

    public:
        static bool InitSystem();
        static void ReleaseSystem();

        FLINE static CEmergencyMemory& GetEmergencyMemory() noexcept
        {
            return _GetEmergencyMemoryBase(0);
        }

    private:
        static void InitEmergencyMemoryFromEnvValue();
        static CEmergencyMemory& _GetEmergencyMemoryBase(const size_t& secureSize);

        static void InitMemoryHandlerFromEnvValue();
    };

}