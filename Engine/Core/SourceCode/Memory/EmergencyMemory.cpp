#include "stdafx.h"
#include "EmergencyMemory.h"
#include "Thread/Mutex.h"


namespace Core
{
    CEmergencyMemory::~CEmergencyMemory()
    {

    }

    CEmergencyMemory::CEmergencyMemory(const size_t& secureSize)
        : m_memBlock(secureSize)
    {
        
    }

    size_t CEmergencyMemory::SecureHeapMemory() noexcept
    {
        if (IsSecureAvailable())
        {
            const auto result = m_memBlock.GetMemorySize();
            m_memBlock.Deallocate();
            return result;
        }
        
        return size_t(0);
    }

    size_t CEmergencyMemory::GetSecuredSize() noexcept
    {
        return m_memBlock.GetMemorySize();
    }
}