//테스트 필요
#pragma once

#include "AlignMalloc.h"


namespace Core
{

    /**
    std allocator를 지원하기 위한 AlignMalloc의 확장
    생성된 첫 객체를 기준으로 정렬되므로
    첫 객체 이후의 대한 메모리 주소 정렬은 보장하지 않는다.
    */
    template<typename T, const EAlignmentSize _T_ALIGNMENT_>
    class alignment_allocator
    {
    public:
        using value_type = T;
        using pointer = typename value_type*;
        using const_pointer = typename const value_type*;
        using reference = typename value_type&;
        using const_reference = typename const value_type&;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        
        template<typename U, const EAlignmentSize _U_ALIGNMENT_>
        struct rebind 
        {
            using other = alignment_allocator<U, _U_ALIGNMENT_>;
        };

    public:
        alignment_allocator() noexcept {}
        alignment_allocator(const alignment_allocator&) noexcept {}
        template<typename U, const EAlignmentSize _U_ALIGNMENT_>
        alignment_allocator(const alignment_allocator<U, _U_ALIGNMENT_>&) noexcept {}


        pointer address(reference x) const noexcept
        {
            return &x;
        }

        const_pointer address(const_reference x) const noexcept
        {
            return &x;
        }

        /**
        n은 object의 수
        hint는 정렬의 기준 값.
        */
        pointer allocate(const size_type&n, const EAlignmentSize& hint = _T_ALIGNMENT_)
        {
            return MallocAlignedMemory<void*>(n, hint);
        }

        /**
        메모리 해제. 
        p인자만 유효하게 사용된다. 
        p인자는 메모리 할당시 전달 받았던 주소를 반드시 전달 받아야 한다.
        */
        void deallocate(pointer p, size_type)
        {
            FreeAlignedMemory(aligned_ptr_cast<void*>(p));
        }

        constexpr size_type max_size() const noexcept
        {
            return ((~size_type(0) - static_cast<size_type>(_T_ALIGNMENT_)) / sizeof(value_type));
        }

        template<typename U, typename... Args>
        void construct(U *p, Args&&... args)
        {
            ::new((void *)p) U(std::forward<Args>(args)...);
        }

        void destroy(pointer p) const
        { 
            p->~value_type(); 
        }
    };

    template<const EAlignmentSize _T_ALIGNMENT_>
    class alignment_allocator<void, _T_ALIGNMENT_> 
    {
    public:
        using value_type = void;
        using pointer = void*;
        using const_pointer = const void*;
        
        template<typename U, const EAlignmentSize _U_ALIGNMENT_>
        struct rebind
        {
            using other = alignment_allocator<U, _U_ALIGNMENT_>;
        };
    };

    template<typename T, const EAlignmentSize T_ALIGNMENT,
        typename U, const EAlignmentSize U_ALIGNMENT>
    ILINE bool operator==(const alignment_allocator<T, T_ALIGNMENT>&, const alignment_allocator<U, U_ALIGNMENT>&)
    { 
        return true;
    }

    template<typename T, const EAlignmentSize T_ALIGNMENT,
        typename U, const EAlignmentSize U_ALIGNMENT>
    ILINE bool operator!=(const alignment_allocator<T, T_ALIGNMENT>&, const alignment_allocator<U, U_ALIGNMENT>&)
    {
        return false;
    }

}