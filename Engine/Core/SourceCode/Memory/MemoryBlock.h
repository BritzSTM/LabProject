//테스트 필요
#pragma once


//user
#include "EnvironmentValue.h"
#include "Common/BaseTypes.h"
#include "Memory/IObjectSizer.h"


//std
#include <memory>


namespace Core
{

    //테스트용 함수 사용금지
    template <typename _MEM_BLOCK_TYPE_>
    ILINE size_t GetMemblockAllocatedSize(const _MEM_BLOCK_TYPE_& block) noexcept
    {
        return (block.GetAllocatedSize());
    }

    template<template<typename _T_ = byte> class _ALLOCATOR_ = std::allocator>
    class CMemBlockHeap final
    {
    private:
        using self_type = CMemBlockHeap<_ALLOCATOR_>;
        
    public:
        ~CMemBlockHeap()
        {
            _DeallocateMemory();
        }

        /**
        전달받은 size만큼 메모리를 할당하여 보유합니다.
        메모리 할당 실패시 bad_alloc 예외가 발생합니다.
        지연 생성여부를 결정할 수 있습니다.
        지연생성을 설정한 경우 Allocate 메소드를 호출해야 합니다.
        */
        CMemBlockHeap(const size_t& allocateSize, const bool isDeferred = false)
            : m_allocatedSize(allocateSize)
            , m_pMem((isDeferred) ? (nullptr) : (_AllocateMemory(allocateSize)))
            , m_hasOwnershipOfMem(true)
        {
            
        }

        /**
        이미 할당된 메모리 주소, 사용가능한 크기를 전달 받습니다.
        예외가 발생하지 않습니다.
        */
        CMemBlockHeap(const void* pMem, const size_t& memSize) noexcept
            : m_allocatedSize(memSize)
            , m_pMem(pMem)
            , m_hasOwnershipOfMem(false)
        {

        }
       
        /**
        CMemblockHeap을 복제합니다. 
        복제된 객체는 메모리에 대한 소유권을 가지고 있습니다.
        copySize가 원본 객체의 메모리보다 클 경우 out_of_range 예외가 발생합니다.
        */
        CMemBlockHeap(const self_type& src, const size_t& copySize = self_type::npos)
        {
            _DeepCopyMembers(src, copySize);
        }

        /**
        src로 부터 데이터를 이동합니다.
        이동이 완료된 src는 초기화 상태가 됩니다.
        */
        CMemBlockHeap(self_type&& src) noexcept
            : m_allocatedSize(src.m_allocatedSize)
            , m_pMem(src.m_pMem)
            , m_hasOwnershipOfMem(src.m_hasOwnershipOfMem)
        {
            src._ClearMembers();
        }

        //CMemBlockHeap의 전체 객체 크기를 획득합니다.
        ILINE size_t GetObjectSize() const noexcept
        {
            return (sizeof(self_type) + GetMemorySize());
        }

        //CMemBlockHeap이 관리하는 메모리의 크기를 반환합니다.
        size_t GetMemorySize() const noexcept
        {
            //지연할당인 경우 체크
            if (m_pMem == nullptr)
            {
                return size_t(0);
            }

            return m_allocatedSize;
        }

        //CMemBlockHeap이 관리하는 메모리의 첫 주소를 확득합니다.
        ILINE byte* GetMemAddress() const noexcept
        {
            return m_pMem;
        }

        /**
        지연생성이 설정됬을 때 호출하면 메모리 할당이 진행됩니다.
        할당된 상태라면 false를 반환합니다.
        */
        bool Allocate()
        {
            if (m_pMem == nullptr && m_hasOwnershipOfMem)
            {
                m_pMem = _AllocateMemory(m_allocatedSize);
                return true;
            }

            return false;
        }

        void Deallocate() noexcept
        {
            _DeallocateMemory();
            m_pMem = nullptr;
        }

        /**
        복제함수. 
        복제될 크기를 전달받습니다. 복제된 객체는 메모리 ownership을 가집니다.
        복제될 크기를 전달하지 않으면 전체를 복제합니다
        copySize가 원본 객체의 메모리보다 클 경우 out_of_range 예외가 발생합니다.
        */
        template<template<typename _U_ = byte> class _U_ALLOCATOR_ = std::allocator>
        CMemBlockHeap<_U_ALLOCATOR_>
        CopyToAnotherBlockHeap(const size_t& copySize = self_type::npos) const
        {
            const size_t filteredCopySize = _FilterCopySize(m_allocatedSize, copySize);

            //소스객체의 메모리가 비할당 상태일 수 있음. 따라서 지연생성
            CMemBlockHeap<_U_ALLOCATOR_> result(filteredCopySize, true);

            if (m_pMem != nullptr)
            {
                result.Allocate();
                std::memcpy(result.GetMemAddress(), m_pMem, filteredCopySize);
            }

            return std::move(result);
        }
        
        //Assign. 얕은 복제를 진행합니다.
        self_type& operator=(const self_type& rhs) noexcept
        {
            GUARD_SELF_ASSIGNMENT;
            _ShallowcopyMembers(rhs);
        }

        bool operator==(const self_type& rhs) const noexcept
        {
            if (m_allocatedSize == rhs.m_allocatedSize)
            {
                if (m_pMem == rhs.m_pMem)
                {
                    if (m_hasOwnershipOfMem == rhs.m_hasOwnershipOfMem)
                    {
                        return true;
                    }
                }
            }

            return false;
        }

        bool operator!=(const self_type& rhs) const noexcept
        {
            return (!operator==(rhs));
        }

    private:
        //모든 Member를 초기화 합니다. 할당된 메모리는 해제하지 않습니다.
        ILINE void _ClearMembers() noexcept
        {
            m_allocatedSize = 0;
            m_pMem = nullptr;
            m_hasOwnershipOfMem = false;
        }

        ILINE void _ShallowcopyMembers(const self_type& src)
        {
            m_allocatedSize = src.m_allocatedSize;
            m_pMem = src.m_pMem;
            m_hasOwnershipOfMem = src.m_hasOwnershipOfMem;
        }

        ILINE void _DeepCopyMembers(const self_type& src, const size_t& copySize)
        {
            m_allocatedSize = _FilterCopySize(src.m_allocatedSize, copySize);

            //원본객체가 복제가능한 상태이면 복제 개시
            if (src.m_pMem != nullptr)
            {
                m_pMem = _AllocateMemory(m_allocatedSize);
                std::memcpy(m_pMem, src.m_pMem, m_allocatedSize);
            }
        }

        size_t _FilterCopySize(const size_t& srcSize, const size_t& copySize) const
        {
            if (copySize == self_type::npos)
            {
                return srcSize;
            }
            
            if (srcSize >= copySize)
            {
                return copySize;
            }
            
            throw std::out_of_range("copySize is more then srcSize.");
        }

        ILINE byte* _AllocateMemory(const size_t& allocateSize)
        {
            _ALLOCATOR_<> allocator;
            return allocator.allocate(allocateSize);
        }

        ILINE void _DeallocateMemory() noexcept
        {
            if (m_hasOwnershipOfMem)
            {
                if (m_pMem != nullptr)
                {
                    _ALLOCATOR_<> allocator;
                    allocator.deallocate(m_pMem, m_allocatedSize);
                }
            }
        }

    public:
        static constexpr size_t npos = (~size_t(0));

    private:
        size_t m_allocatedSize;
        byte* m_pMem;
        bool m_hasOwnershipOfMem;
    };

}