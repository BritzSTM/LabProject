/**
    AlignMalloc.h의 Template 구현부
*/


namespace Core
{
    //forward declaration
    namespace _internal
    {
       intptr _MallocAlignedMemoryBase(const size_t& allocateSize, const EAlignmentSize& alignment) noexcept;
       void _FreeAlignedMemoryBase(intptr& ptrAddress) noexcept;

       template <typename _CAST_PTR_TYPE_>
       FLINE aligned_ptr<_CAST_PTR_TYPE_>
       _MallocAlignedMemoryTemplateImpl(const size_t& allocateSize, const EAlignmentSize& alignment);
    
       template<typename _CAST_PTR_TYPE_>
       FLINE void
       _FreeAlignedMemoryTemplateImpl(aligned_ptr<_CAST_PTR_TYPE_>& ptr);

       template<typename _CAST_PTR_TYPE_>
       FLINE aligned_ptr<_CAST_PTR_TYPE_>
       _aligned_ptr_castTemplateImpl(const _CAST_PTR_TYPE_& pPtr) noexcept;
    }

    /**
    정렬된 메모리 주소를 안정적으로 사용하기 위해 사용하는 포인터 자료형
    데이터에 접근하기 위해서는 사용하고자 하는 포인터형으로 캐스팅 후 이용해야 합니다.
    */
    template<typename _CAST_PTR_TYPE_>
    class aligned_ptr final
    {
    public:
        using raw_ptr_type = intptr;
        using pointer = _CAST_PTR_TYPE_;
        using const_pointer = const _CAST_PTR_TYPE_;
        using self_type = aligned_ptr<_CAST_PTR_TYPE_>;

    private:
        //해당 항목에 대해 성능검증을 해볼 필요성이 있음
        using set_type = concurrent_unordered_set<raw_ptr_type>;

        friend struct PtrTypeTraits<aligned_ptr<_CAST_PTR_TYPE_>>;

        template<typename _CAST_PTR_TYPE_>
        friend FLINE aligned_ptr<_CAST_PTR_TYPE_> 
        _internal::_MallocAlignedMemoryTemplateImpl(const size_t& allocateSize, const EAlignmentSize& alignment);

        template<typename _CAST_PTR_TYPE_>
        friend FLINE void 
        _internal::_FreeAlignedMemoryTemplateImpl(aligned_ptr<_CAST_PTR_TYPE_>& ptr);

        template<typename _CAST_PTR_TYPE_>
        friend FLINE aligned_ptr<_CAST_PTR_TYPE_>
        _internal::_aligned_ptr_castTemplateImpl(const _CAST_PTR_TYPE_& pPtr) noexcept;

    public:
        [[nodiscard]]explicit operator pointer() noexcept
        {
            return reinterpret_cast<pointer>(m_rawPtrAddress);
        }


        /**비교 연산자 포인터 타입 취급할 것이므로 nullptr 지원*/
        FLINE bool operator==(const std::nullptr_t& rhs) const noexcept;
        FLINE bool operator!=(const std::nullptr_t& rhs) const noexcept;

        FLINE bool operator==(const self_type& rhs) const noexcept;
        FLINE bool operator!=(const self_type& rhs) const noexcept;

#if defined(DEBUG) || defined(_DEBUG)
        //디버그 시에만 RawPtr값을 획득을 돕는 함수입니다.
        const raw_ptr_type DebugGetRawPtr() const noexcept
        {
            return m_rawPtrAddress;
        }
#endif

    private:
        explicit constexpr aligned_ptr(const intptr rawPtrAddress) noexcept
            : m_rawPtrAddress(rawPtrAddress)
        {

        }

        static void _RegisterAlignedRawPtrSet(const self_type& ptr) noexcept
        {
            s_alignedRawPtrSet.insert(ptr.m_rawPtrAddress);
        }

        static void _UnregisterAlignedRawPtrSet(const self_type& ptr) noexcept
        {
            s_alignedRawPtrSet.internal_erase(ptr.m_rawPtrAddress);
        }

        static bool _FindAlignedPtr(const raw_ptr_type& ptrAddress) noexcept
        {
            if (std::cend(s_alignedRawPtrSet) != s_alignedRawPtrSet.find(ptrAddress))
            {
                return true;
            }

            return false;
        }

    private:
        raw_ptr_type m_rawPtrAddress;
        static set_type s_alignedRawPtrSet;

        //_CAST_TYPE_은 무조건 포인터 타입만 인정
        static_assert(std::is_pointer<_CAST_PTR_TYPE_>::value, "_CAST_PTR_TYPE_ is not pointer type");
    };

    //aligned_ptr<void*>와 intptr의 크기는 동일함을 보장해야함. 이것이 통과되면 다른 형도 전부 통과
    static_assert(sizeof(aligned_ptr<void*>) == sizeof(intptr), "aligned_ptr<void*>, intptr size not equal");


    template<typename _CAST_PTR_TYPE_>
    struct PtrTypeTraits<aligned_ptr<_CAST_PTR_TYPE_>>
          : public PtrTypeTraitsNullptrBase<typename aligned_ptr<_CAST_PTR_TYPE_>::raw_ptr_type>
    {
        static constexpr aligned_ptr<_CAST_PTR_TYPE_> ErrorAddress = aligned_ptr<_CAST_PTR_TYPE_>(
              PtrTypeTraits<typename aligned_ptr<_CAST_PTR_TYPE_>::raw_ptr_type>::ErrorAddress);
    };

    template<typename _CAST_PTR_TYPE_>
    bool aligned_ptr<_CAST_PTR_TYPE_>::operator==(const std::nullptr_t& rhs) const noexcept
    {
        return (PtrTypeTraits<aligned_ptr<_CAST_PTR_TYPE_>>::NullptrAddrValue == (m_rawPtrAddress));
    }

    template<typename _CAST_PTR_TYPE_>
    bool aligned_ptr<_CAST_PTR_TYPE_>::operator!=(const std::nullptr_t& rhs) const noexcept
    {
        return !(operator==(rhs));
    }

    template<typename _CAST_PTR_TYPE_>
    bool aligned_ptr<_CAST_PTR_TYPE_>::operator==(const self_type& rhs) const noexcept
    {
        return (rhs.m_rawPtrAddress == m_rawPtrAddress);
    }

    template<typename _CAST_PTR_TYPE_>
    bool aligned_ptr<_CAST_PTR_TYPE_>::operator!=(const self_type& rhs) const noexcept
    {
        return !(operator==(rhs));
    }


    //사용 금지
    namespace _internal
    {
        FLINE bool _IsPossibleToMallocAlignedMemory(const size_t& allocateSize, const EAlignmentSize& alignment) noexcept
        {
            return (0 < allocateSize);
        }

        /**해제 가능한 주소를 가진 aligned_ptr인지 확인*/
        template<typename _CAST_PTR_TYPE_>
        FLINE bool _IsPossibleToFreeAlignedMemory(const aligned_ptr<_CAST_PTR_TYPE_>& ptr) noexcept
        {
            return ((ptr != nullptr) && (ptr != PtrTypeTraits<aligned_ptr<_CAST_PTR_TYPE_>>::ErrorAddress));
        }

        template <typename _CAST_PTR_TYPE_>
        FLINE aligned_ptr<_CAST_PTR_TYPE_>
        _MallocAlignedMemoryTemplateImpl(const size_t& allocateSize, const EAlignmentSize& alignment)
        {
            if (!_IsPossibleToMallocAlignedMemory(allocateSize, alignment))
            {
                throw std::invalid_argument("MallocAlignedMemory invalid argument");
            }

            const intptr &result = _MallocAlignedMemoryBase(allocateSize, alignment);
            if (result == PtrTypeTraits<intptr>::NullptrAddrValue)
            {
                throw std::bad_alloc();

            }

            return aligned_ptr<_CAST_PTR_TYPE_>(result);
        }

        template<typename _CAST_PTR_TYPE_>
        FLINE void _FreeAlignedMemoryTemplateImpl(aligned_ptr<_CAST_PTR_TYPE_>& ptr)
        {
            if (_IsPossibleToFreeAlignedMemory(ptr))
            {
                _FreeAlignedMemoryBase(ptr.m_rawPtrAddress);
                ptr.m_rawPtrAddress = PtrTypeTraits<aligned_ptr<_CAST_PTR_TYPE_>>::NullptrAddrValue;
            }
            else
            {
                throw bad_free_aligned_ptr();
            }
        }

        template<typename _CAST_PTR_TYPE_>
        FLINE aligned_ptr<_CAST_PTR_TYPE_>
        _aligned_ptr_castTemplateImpl(const _CAST_PTR_TYPE_& pPtr) noexcept
        {
            return aligned_ptr<_CAST_PTR_TYPE_>(
                reinterpret_cast<typename aligned_ptr<_CAST_PTR_TYPE_>::raw_ptr_type>(pPtr));
        }
    }

    template <typename _CAST_PTR_TYPE_>
    aligned_ptr<_CAST_PTR_TYPE_>
    MallocAlignedMemory(const size_t& allocateSize, const EAlignmentSize& alignment)
    {
        const auto &resultPtr = 
            _internal::_MallocAlignedMemoryTemplateImpl<_CAST_PTR_TYPE_>(allocateSize, alignment);

        aligned_ptr<_CAST_PTR_TYPE_>::_RegisterAlignedRawPtrSet(resultPtr);

        return resultPtr;
    }

    template <>
    aligned_ptr<void*>
    MallocAlignedMemory(const size_t& allocateSize, const EAlignmentSize& alignment)
    {
        //등록안하고 바로 반환
        return _internal::_MallocAlignedMemoryTemplateImpl<void*>(allocateSize, alignment);
    }

    template<typename _CAST_PTR_TYPE_>
    void FreeAlignedMemory(aligned_ptr<_CAST_PTR_TYPE_>& ptr)
    {
        _internal::_FreeAlignedMemoryTemplateImpl(ptr);
        aligned_ptr<_CAST_PTR_TYPE_>::_UnregisterAlignedPtrSet(ptr);
    }

    template <>
    void FreeAlignedMemory(aligned_ptr<void*>& ptr)
    {
        //등록 해제 절차가 없다.
        _internal::_FreeAlignedMemoryTemplateImpl(ptr);
    }

    template <typename _CAST_PTR_TYPE_>
    aligned_ptr<_CAST_PTR_TYPE_>
    aligned_ptr_cast(const _CAST_PTR_TYPE_& pPtr)
    {
        static_assert(std::is_pointer<_CAST_PTR_TYPE_>::value, 
            "_CAST_PTR_TYPE_ is not pointer type");
        
        //포인터 참조형임을 참고할것.
        if (aligned_ptr<_CAST_PTR_TYPE_>::_FindAlignedPtr(*pPtr))
        {
            return _internal::_aligned_ptr_castTemplateImpl<_CAST_PTR_TYPE_>(pPtr);
        }
        
        throw bad_cast_aligned_ptr();
    }

    template <>
    aligned_ptr<void*>
    aligned_ptr_cast(void* const &pPtr)
    {
        return _internal::_aligned_ptr_castTemplateImpl<void*>(pPtr);
    }

}
