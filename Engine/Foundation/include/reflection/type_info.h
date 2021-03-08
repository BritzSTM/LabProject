#pragma once
#ifndef __GG__
#define __GG__
#include "lang_type.h"
#include "type_name.h"
#include "../span.h"
#include "../hash.h"
#include "../lang_util.h"

#include <exception>
#include <tuple>
#include <optional>
#include <type_traits>
#include <initializer_list>


namespace fd::refl
{
    /** 선언구조에 대한 정보 */
    struct SDeclType final
    {
        ETypeQualifier qualifier;
        ETypeCLASS typeCLASS;
    };

    namespace _internal_type_info
    {
        template<typename _Ty, size_t... _ns>
        constexpr auto GetDeclTypesImpl(std::index_sequence<_ns...> seq) noexcept
        {
            const std::array<SDeclType, seq.size()> decls{ {{ GetTypeQualifier<seed_traits<_Ty>::level_t<_ns>>(), GetTypeCLASS<seed_traits<_Ty>::level_t<_ns>>() }...} };

            return decls;
        }

        template<typename _Ty>
        inline constexpr auto GetDeclTypes() noexcept
        {
            return GetDeclTypesImpl<_Ty>(std::make_index_sequence<seed_traits<_Ty>::depth + 1>());
        }

        // GetDeclTypes으로 획득한 배열을 정적 스토리지로 올리기 위한 구조체
        template<typename _Ty>
        struct SDeclTypeStorage
        {
            static constexpr auto declTypes{ GetDeclTypes<_Ty>() };
        };
    }

    ///** 선언구조 스택을 획득 */
    template<typename _Ty>
    constexpr span<const SDeclType> GetDeclStack() noexcept
    {
        return { _internal_type_info::SDeclTypeStorage<_Ty>::declTypes };
    }

    //class CDeclQuery;

    ///** CDeclQuery의 예외객체 */
    //class QueryException
    //    : public std::logic_error
    //{
    //public:
    //    virtual ~QueryException() = default;

    //    QueryException(const CDeclQuery& errObj, const char* const pszMsg = "");
    //    QueryException(const QueryException&) = default;
    //    QueryException(QueryException&&) = default;

    //private:
    //    const CDeclQuery m_errQuery;
    //};

    ///** 선언구조에 대한 질의 */
    //class CDeclQuery
    //{
    //    /* 메소드들이 일정의 질의문이기 때문에 작명규칙을 따르지 않는다. */
    //private:
    //    using pos_type = typename span<const SDeclType>::size_type;

    //public:
    //    ~CDeclQuery() = default;

    //    CDeclQuery(span<const SDeclType> declTypes);

    //    CDeclQuery(const CDeclQuery&) = default;
    //    CDeclQuery(CDeclQuery&&) noexcept = default;

    //    CDeclQuery& operator=(const CDeclQuery&) = default;
    //    CDeclQuery& operator=(CDeclQuery&&) = default;


    //    // Query Methods

    //    /** 호출한 질의 객체의 상태를 유지 */
    //    [[nodiscard]] CDeclQuery Is() noexcept;

    //    /** 다음 선언에 대한 질의로 상태 변경. 불가능하다면 예외가 발생 */
    //    CDeclQuery& Next();
    //    CDeclQuery& Next(std::nothrow_t) noexcept;

    //    /** 최종 유형에 대한 질의 시작 */
    //    CDeclQuery& Seed() noexcept;

    //    /** 현재질의 깊이 */
    //    size_t GetCurrentDepth() const noexcept;

    //    /** 타입선언 깊이 */
    //    size_t GetDeclDepth() const noexcept;

    //    /** 동등한 선언 구조인지 비교 */
    //    bool operator==(const CDeclQuery& rhs) noexcept;

    //    /** 유효성 확인 */
    //    operator bool() const noexcept
    //    {
    //        return m_isCorrectQuery;
    //    }

    //    /** 질의 내용에 따른 객체 상태 초기화 */
    //    void Clear() noexcept
    //    {
    //        using std::swap;

    //        CDeclQuery cls{ m_declTypes };
    //        swap(*this, cls);
    //    }


    //    CDeclQuery& Reference() noexcept;

    //    // 총 8개
    //    CDeclQuery& Ptr() noexcept;
    //    CDeclQuery& Ptr(const ETypeCLASS type) noexcept;
    //    CDeclQuery& ConstPtr() noexcept;
    //    CDeclQuery& ConstPtr(const ETypeCLASS type) noexcept;

    //    // 총 16개의 인터페이스
    //    // 8개는 Type을 잘못넣을 가능성. 16개는 너무 많아서 문제

    //    //CDeclQuery& ObjPtr() noexcept;
    //    //CDeclQuery& FuncPtr() noexcept;
    //    //CDeclQuery& MemberFuncPtr() noexcept;
    //    //CDeclQuery& MemberDataPtr() noexcept;

    //    //CDeclQuery& ConstObjPtr() noexcept;
    //    //CDeclQuery& ConstFuncPtr() noexcept;
    //    //CDeclQuery& ConstMemberFuncPtr() noexcept;
    //    //CDeclQuery& ConstMemberDataPtr() noexcept;



    //    CDeclQuery& Class() noexcept;
    //    CDeclQuery& Struct() noexcept;
    //    CDeclQuery& Union() noexcept;
    //    CDeclQuery& Enum() noexcept;
    //    CDeclQuery& EnumClass() noexcept;
    //    CDeclQuery& Array(const size_t dim = kArrDimMax) noexcept;
    //    CDeclQuery& Fundamental() noexcept;

    //    CDeclQuery& Reference(const ETypeQualifier qf) noexcept;

    //    CDeclQuery& ObjPtr(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& FuncPtr(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& MemberFuncPtr(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& MemberDataPtr(const ETypeQualifier qf) noexcept;

    //    CDeclQuery& ConstObjPtr(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& ConstFuncPtr(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& ConstMemberFuncPtr(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& ConstMemberDataPtr(const ETypeQualifier qf) noexcept;

    //    CDeclQuery& Class(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& Struct(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& Union(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& Enum(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& EnumClass(const ETypeQualifier qf) noexcept;
    //    CDeclQuery& Array(const ETypeQualifier qf, const size_t dim = kArrDimMax) noexcept;
    //    CDeclQuery& Fundamental(const ETypeQualifier qf) noexcept;

    //private:
    //    size_t CalArrayDim(const pos_type cursor) noexcept; // 해당 위치에서 배열 차원을 계산함

    //    bool BeginQuery() noexcept;
    //    void EndQuery() noexcept;

    //    // 일반적인 한정자를 무시하는 쿼리
    //    template<ETypeCLASS... _MatchTypes>
    //    void QueryIgnoringQF() noexcept;

    //    // 일반적인 한정자를 포함하는 쿼리
    //    template<ETypeCLASS... _MatchTypes>
    //    void QueryQF(const ETypeQualifier qf) noexcept;

    //public:
    //    static constexpr size_t kArrDimMax = ~size_t(0); /** 배열의 최대 차원 수를 의미합니다 */

    //private:
    //    span<const SDeclType> m_declTypes;
    //    pos_type m_currCusor;
    //    bool m_isCorrectQuery;
    //};

    /*
        TypeMeta를 나타내는 구조체들은 비상속 위임 interface를 사용함.
        필수 interface는 반드시 해당 타입이 지원해야 함.
        
        - 필수 interface
          static const char* GetTypeName(const SomeMeta& v) noexcept;
          static size_t GetTypeSize(const SomeMeta& v) noexcept;

        - 현재 확장 interface
          static const char* GetTypeHelp(const SomeMeta& v);
    */

    struct STypeMeta
    {
        static const char* GetTypeName(const STypeMeta& v) noexcept;
        static size_t GetTypeSize(const STypeMeta& v) noexcept;

        const char* pszName;
        size_t size;
    };

    const char* STypeMeta::GetTypeName(const STypeMeta& v) noexcept
    {
        return v.pszName;
    }

    size_t STypeMeta::GetTypeSize(const STypeMeta& v) noexcept
    {
        return v.size;
    }

    struct STypeDetailMeta
        : public STypeMeta
    {
        static const char* GetTypeHelp(const STypeDetailMeta& v) noexcept;

        const char* pszHelp;
    };

    const char* STypeDetailMeta::GetTypeHelp(const STypeDetailMeta& v) noexcept
    {
        return v.pszHelp;
    }

    struct ITypeMeta
    {
        FD_PURE_INTERFACE_PROP(ITypeMeta);

        virtual std::string_view GetName() const noexcept = 0;
        virtual size_t GetSize() const noexcept = 0;
        virtual std::string_view GetHelp() const = 0;
    };

    struct SEmptyExtendMeta { };

    struct SFieldTypeMeta
    {
        static const char* GetFieldName(const SFieldTypeMeta& v) noexcept;
        static size_t GetFieldOffset(const SFieldTypeMeta& v) noexcept;

        const char* pszName;
        size_t offset;
    };

    const char* SFieldTypeMeta::GetFieldName(const SFieldTypeMeta& v) noexcept
    {
        return v.pszName;
    }

    size_t SFieldTypeMeta::GetFieldOffset(const SFieldTypeMeta& v) noexcept
    {
        return v.offset;
    }

    struct STypeFieldDetailMeta
        : public SFieldTypeMeta
    {
        static const char* GetFieldTypeHelp(const STypeFieldDetailMeta& v) noexcept;

        const char* pszHelp;
    };

    const char* STypeFieldDetailMeta::GetFieldTypeHelp(const STypeFieldDetailMeta& v) noexcept
    {
        return v.pszHelp;
    }

    struct IFieldTypeMeta
    {
        FD_PURE_INTERFACE_PROP(IFieldTypeMeta);

        virtual std::string_view GetName() const noexcept = 0;
        virtual size_t GetOffset() const noexcept = 0;
        virtual std::string_view GetHelp() const noexcept = 0;
    };

    namespace _internal_type_info
    {
        /**
            복합구조 내부에서 SFieldType들을 수출하기 위한 proxy 인터페이스.
            _Ty는 내부에 CExporter_Gen 클래스를 보유해야함
        */
        template<typename _Ty>
        struct FieldProxyForGenerator final
        {
        private:
            struct Supported {};
            struct NotSupoorted {};

            template<typename _Ty>
            static Supported checkSupport(typename std::decay_t<typename _Ty::CExporter_Gen>*);

            template<typename _Ty>
            static NotSupoorted checkSupport(...);

        public:
            static constexpr bool IsSupport{ std::is_same_v<decltype(checkSupport<_Ty>(nullptr)), Supported> };
            // ->span<const SFieldType>
            template<bool _support = IsSupport>
            static auto ExportFields() noexcept
            {
                if constexpr (_support)
                    return ExporterType::ExportFields();

                return {};
            }
        };
    }

    /** 해당 타입 맴버의 위치를 반환합니다. */
#define FD_REFL_OFFSETOF(_TY_NAME_, _MEMBER_NAME_) (fd::size_t)(&((_TY_NAME_*)0)->_MEMBER_NAME_)

    /** 복합 자료형에 필드 수출업자를 선언합니다. */
#define FD_REFL_DECL_FIELD_EXPORTER(_CLASS_NAME_) \
    private:\
        friend struct _internal_type_info::FieldProxyForGenerator<_CLASS_NAME_>; \
        class CExporter_Gen // 이 클래스는 생성기를 통해 반드시 구현되어야 함.

    /** 생성된 meta 정보의 유형입니다 */
    enum class EMetaType : uint8
    {
        Compiler,
        Generator,
        Custom
    };

    struct IType;

    class ReflectionException
        : public std::logic_error
    {
    public:
        virtual ~ReflectionException();

        ReflectionException(gsl::not_null<const IType*> pErrorData, std::string_view msg);
        ReflectionException(const ReflectionException&) = default;
        ReflectionException(ReflectionException&&) noexcept = default;

        ReflectionException& operator=(const ReflectionException&) = default;
        ReflectionException& operator=(ReflectionException&&) noexcept = default;

        const IType* GetErrorData() const noexcept;
        
    private:
        const IType* m_pErrorData;
    };

    ReflectionException::~ReflectionException()
    {

    }

    ReflectionException::ReflectionException(gsl::not_null<const IType*> pErrorData, std::string_view msg)
        : logic_error(msg.data())
        , m_pErrorData{ pErrorData }
    {

    }

    const IType* ReflectionException::GetErrorData() const noexcept
    {
        return m_pErrorData;
    }

    struct IType
    {
        FD_PURE_INTERFACE_PROP(IType);

        virtual EMetaType GetMetaType() const noexcept = 0;
        //virtual CDeclQuery Is() const noexcept = 0;

        //virtual size_t GetFieldCount() const noexcept = 0;
        //virtual IFieldTypeMeta* GetField(const size_t idx) const = 0;
        //virtual IFieldTypeMeta* GetField(const size_t idx, std::nothrow_t) const noexcept = 0;
    };


    template<EMetaType _type, typename _TypeMeta, typename _ExtendMeta>
    class CType
        : private _TypeMeta
        //, private _ExtendMeta
        , public IType
    {
    public:
        virtual EMetaType GetMetaType() const noexcept;


        // Impl ITypeMeta

        virtual std::string_view GetName() const noexcept;
        virtual size_t GetSize() const noexcept;
        virtual std::string_view GetHelp() const;

    private:


    };

    template<EMetaType _type, typename _TypeMeta, typename _ExtendMeta>
    EMetaType CType<_type, _TypeMeta, _ExtendMeta>::GetMetaType() const noexcept
    {
        return _type;
    }

    template<EMetaType _type, typename _TypeMeta, typename _ExtendMeta>
    std::string_view CType<_type, _TypeMeta, _ExtendMeta>::GetName() const noexcept
    {
        return _TypeMeta::GetTypeName(*this);
    }

    template<EMetaType _type, typename _TypeMeta, typename _ExtendMeta>
    size_t CType<_type, _TypeMeta, _ExtendMeta>::GetSize() const noexcept
    {
        return _TypeMeta::GetTypeSize(*this);
    }

    template<EMetaType _type, typename _TypeMeta, typename _ExtendMeta>
    std::string_view CType<_type, _TypeMeta, _ExtendMeta>::GetHelp() const
    {
        // 호출 실패할 경우 아예 함수가 없는 경우라 컴파일 불가
        //return _TypeMeta::GetTypeHelp(*this);

        return "";
    }



    ///** @brief 타입정보를 획득합니다 */
    //template<typename _Ty>
    //IType const* GetType();

    ///** @brief 변수에서 타입정보를 획득합니다 */
    //template<typename _Ty>
    //IType const* GetType(_Ty&);

    ///** @brief 타입정보를 획득합니다. 타입정보 조회 불가능이라도 예외가 발생하지 않습니다 */
    //template<typename _Ty>
    //IType const* GetType(std::nothrow_t) noexcept;

    ///** @brief 변수에서 타입정보를 획득합니다. 타입정보 조회 불가능이라도 예외가 발생하지 않습니다 */
    //template<typename _Ty>
    //IType const* GetType(_Ty&, std::nothrow_t) noexcept;


    ///** @brief 런타임에 타입정보를 획득합니다. main 함수 진입이후에 호출이 유효함 */
    //IType const* GetType(std::string_view);

    ///** @brief 변수에서 타입정보를 획득합니다. main 함수 진입이후에 호출이 유효함. 예외를 발생하지 않음 */
    //IType const* GetType(std::string_view, std::nothrow_t) noexcept;
}
#endif
