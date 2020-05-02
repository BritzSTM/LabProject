//테스트 필요
#pragma once


#include "CoreHelper.h"
#include "Common/BaseTypes.h"
#include "Type/TypeTraits.h"

#include "Container/ConcurrentUnorderedSet.h"


namespace Core
{

    /**
    할당된 정렬된 메모리 해제에 실패할 경우 발생하는 예외 입니다.
    */
    class bad_free_aligned_ptr
        : public std::exception
    {
        //vc 규격 호출임
    public:
        bad_free_aligned_ptr() noexcept
#if defined(MS_COMPILER)
            : std::exception("bad free", 1)
#else
            : std::exception("bad free")
#endif
        {

        }
    };
    
    /**
    aligned_ptr cast에 실패한 경우 발생하는 예외 입니다.
    */
    class bad_cast_aligned_ptr
        : public std::exception
    {
    public:
        bad_cast_aligned_ptr() noexcept
#if defined(MS_COMPILER)
            : std::exception("bad cast aligned_ptr", 1)
#else
            : std::exception("bad cast aligned_ptr")
#endif
        {

        }
    };


    /**정렬크기 열거형*/
    enum class EAlignmentSize : uint32
    {
        _2 = 2,
        _4 = 4,
        _8 = 8,
        _16 = 16,
        _32 = 32,
        _64 = 64,
        _128 = 128,
        _256 = 256
    };

    static_assert(sizeof(EAlignmentSize) == sizeof(uint32), "EAlignmentSize, uint32 size is not equal ");
    

    //forward declaration 
    template<typename _CAST_PTR_TYPE_>
    class aligned_ptr;
    
    /**
    힙에 메모리 주소가 정렬된 메모리를 획득합니다.
    기존 요청한 크기보다 약간의 추가 영역을 추가적으로 할당합니다.
    실패시 bad_alloc, invalid_argument 예외가 발생합니다.
    */
    template <typename _CAST_PTR_TYPE_>
    [[nodiscard]]aligned_ptr<_CAST_PTR_TYPE_>
    MallocAlignedMemory(const size_t& allocateSize, const EAlignmentSize& alignment);

    /**
    힙에 메모리 주소가 정렬된 메모리를 획득합니다.
    void* 형으로 cast가 가능하며 사용시 주소관리를 철저하게 해야한다.
    기존 요청한 크기보다 약간의 추가 영역을 추가적으로 할당합니다.
    실패시 bad_alloc, invalid_argument 예외가 발생합니다.
    */
    template <>
    [[nodiscard]]aligned_ptr<void*>
    MallocAlignedMemory(const size_t& allocateSize, const EAlignmentSize& alignment);

    /**
    할당된 정렬된 메모리를 해제 합니다.
    실패시 bad_free_aligned_ptr예외를 반환 합니다.
    */
    template <typename _CAST_PTR_TYPE_>
    void FreeAlignedMemory(aligned_ptr<_CAST_PTR_TYPE_>& ptr);

    /**
    할당된 정렬된 메모리를 해제 합니다.
    실패시 bad_free_aligned_ptr예외를 반환 합니다.
    */
    template <>
    void FreeAlignedMemory(aligned_ptr<void*>& ptr);

    /**
    전달받은 pointer 타입을 aligned_ptr 타입으로 형변환 합니다.
    실패시 bad_cast_aligned_ptr 예외가 발생합니다.
    */
    template <typename _CAST_PTR_TYPE_>
    aligned_ptr<_CAST_PTR_TYPE_>
    aligned_ptr_cast(const _CAST_PTR_TYPE_& pPtr);

    /**
    전달받은 pointer 타입을 aligned_ptr 타입으로 형변환 합니다.
    void* 형의 경우 실패가 일어나지 않습니다.
    */
    template <>
    aligned_ptr<void*> aligned_ptr_cast(void* const &pPtr);

}

//실제 템플릿 함수 구현부. 이동금지
#include "_AlignMalloc.hpp"