//테스트 필요
#pragma once


#include "Common/BaseTypes.h"
#include "MemoryBlock.h"


namespace Core
{

    /**
    메모리를 힙에 할당 해두고 있다가 긴급상황때 메모리를 해제하여
    메모리를 사용가능하도록 하게 합니다.
    */
    class CEmergencyMemory final
    {
    public:
        ~CEmergencyMemory() noexcept;

        /**
        EmergencyMemorySystem을 초기화 합니다.
        메모리 확보에 실패하여 bad_alloc 예외가 발생할 수 있습니다.
        */
        CEmergencyMemory(const size_t& secureSize);

        /**확보가능 여부를 반환 합니다.*/
        ILINE bool IsSecureAvailable() const noexcept
        {
            return (m_memBlock.GetMemAddress() != nullptr);
        }

        /**반환된 크기 만큼 HeapMemory를 확보 합니다.*/
        size_t SecureHeapMemory() noexcept;

        /**확보가능한 Heap크기를 반환합니다.*/
        size_t GetSecuredSize() noexcept;

    private:   
        CMemBlockHeap<> m_memBlock;
    };

}