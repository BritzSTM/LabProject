#include "stdafx.h"
#include "AlignMalloc.h"


namespace Core
{
    namespace _internal
    {

        intptr _MallocAlignedMemoryBase(const size_t& allocateSize, const EAlignmentSize& alignment) noexcept
        {
            const size_t alignmentValue = static_cast<size_t>(alignment);
            const size_t totalSize = allocateSize + alignmentValue;

            byte *pAllocatedMemory = ::new(std::nothrow)(byte[totalSize]);
            if (pAllocatedMemory == nullptr)
            {
                return PtrTypeTraits<intptr>::NullptrAddrValue;
            }
            
            //주소보정
            const intptr rawPtrAddress = static_cast<size_t>(*pAllocatedMemory);
            const size_t mask = alignmentValue - 1;
            const size_t missDistance = rawPtrAddress & mask;
            const size_t movingDistance = alignmentValue - missDistance;
            const intptr biasAddress = rawPtrAddress + movingDistance;

            //free시에 필요한 이동거리를 baisAddress -1 위치에 기록
            pAllocatedMemory = reinterpret_cast<byte*>(biasAddress - 1);
            (*pAllocatedMemory) = static_cast<byte>(movingDistance);

            return biasAddress;
        }

        /**정렬되기전 주소를 획득하는 함수*/
        ILINE intptr _GetAddressBeforeAlignment(const intptr& address) noexcept
        {
            //조정된 주소의 이동거리는 1바이트 전에 저장되어 있다.
            const byte *pDiff = reinterpret_cast<byte*>(address - 1);
            return (address - static_cast<intptr>(*pDiff));
        }

        void _FreeAlignedMemoryBase(intptr& ptrAddress) noexcept
        {
            const intptr realRawAddr = _GetAddressBeforeAlignment(ptrAddress);
            byte *pTarget = reinterpret_cast<byte*>(realRawAddr);
            delete pTarget;
        }

    }
}
