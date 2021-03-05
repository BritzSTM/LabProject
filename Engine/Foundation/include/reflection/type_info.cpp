#include "type_info.h"


namespace fd::refl
{
    QueryException::QueryException(const CDeclQuery& errObj, const char* const pszMsg)
        : logic_error(pszMsg)
        , m_errQuery{ errObj }
    {

    }

    /*
        CDeclQuery 구현 규칙

        1. 객체는 항상 유효한 declStack을 보유해야함
        2. 해당 질의에 실패하면 전체 질의는 실패 상태가 됨
        3. 모든 질의는 유요한 상태에서 진행되어야 함

        문제 질의를 하면 상태가 넘어가
        q.pointer() -> 넘어간 상태 하지만 true 라면 확인가능 하지만 false라면? 뒤로가기 있어야함


        q.is().pointer() -> 안넘어가고 확인만 하면됨. 문제는 끝에서 회복시켜야함
        -> is가 나오면 새 객체를 복제하여 전송하면 해결됨
    */

    CDeclQuery::CDeclQuery(span<const SDeclType> declTypes)
        : m_declTypes{ declTypes }
        , m_currCusor{ 0 }
        , m_isCorrectQuery{ true }
    {
        Expects(m_declTypes.size() > 0);
    }

    CDeclQuery CDeclQuery::Is() noexcept
    {
        return *this;
    }

    CDeclQuery& CDeclQuery::Next()
    {
        if (m_currCusor + 1 >= m_declTypes.size())
            throw QueryException(*this, "Next query failed.");

        ++m_currCusor;
        return *this;
    }

    CDeclQuery& CDeclQuery::Seed() noexcept
    {
        m_currCusor = (m_declTypes.size() - 1);
        return *this;
    }

    size_t CDeclQuery::GetCurrentDepth() const noexcept
    {
        return m_currCusor;
    }

    size_t CDeclQuery::GetDeclDepth() const noexcept
    {
        return m_declTypes.size();
    }


    CDeclQuery& CDeclQuery::Reference() noexcept
    {
        QueryIgnoringQF<ETypeCLASS::LValObjectRef, ETypeCLASS::LValFunctionRef, ETypeCLASS::RValObjectRef, ETypeCLASS::RValFunctionRef>();
        return *this;
    }

    // 객체 포인터를 제외한 나머지는 해당 포인터를 이용해 역참조 해서 실행하거나 값을 획득해야함
    CDeclQuery& CDeclQuery::Pointer() noexcept
    {
        ETypeCLASS::ObjectPtr, ETypeCLASS::MemberFunctionPtr, ETypeCLASS::MemberDataPtr, ETypeCLASS::FunctionPtr;
        //ETypeCLASS::Nullptr,
    }

    CDeclQuery& CDeclQuery::ConstPointer() noexcept
    {
        //현재 위치에서 다음 객체가 상수 인지 확인하면 된다.
        // c++ 해석 const int* 라면 상수 객체를 가르키고 있는 포인터라 해석되기 때문
        // 실제 declStack으로도 [non, ptr][const, int] 해석이 뜸

        m_declTypes[m_currCusor].typeCLASS == ETypeCLASS::ObjectPtr &&
        m_declTypes[m_currCusor + 1].qualifier == ETypeQualifier::Const || ETypeQualifier::ConstVolatile
    }

    CDeclQuery& CDeclQuery::Class() noexcept
    {
        QueryIgnoringQF<ETypeCLASS::Class>();
        return *this;
    }

    CDeclQuery& CDeclQuery::Struct() noexcept
    {
        QueryIgnoringQF<ETypeCLASS::Struct>();
        return *this;
    }

    CDeclQuery& CDeclQuery::Union() noexcept
    {
        QueryIgnoringQF<ETypeCLASS::Union>();
        return *this;
    }

    CDeclQuery& CDeclQuery::Enum() noexcept
    {
        QueryIgnoringQF<ETypeCLASS::Enum>();
        return *this;
    }

    CDeclQuery& CDeclQuery::EnumClass() noexcept
    {
        QueryIgnoringQF<ETypeCLASS::EnumClass>();
        return *this;
    }

    CDeclQuery& CDeclQuery::Fundamental() noexcept
    {
        QueryIgnoringQF<ETypeCLASS::Void, ETypeCLASS::Nullptr, ETypeCLASS::Bool, ETypeCLASS::Char, ETypeCLASS::SChar, ETypeCLASS::UChar,
            ETypeCLASS::Char16, ETypeCLASS::Char32, ETypeCLASS::Int16, ETypeCLASS::UInt16, ETypeCLASS::Int32, ETypeCLASS::UInt32,
            ETypeCLASS::Int64, ETypeCLASS::UInt64, ETypeCLASS::Float, ETypeCLASS::Double>();

        return *this;
    }

    CDeclQuery& CDeclQuery::Array(const size_t dim) noexcept
    {
        Expects(dim > 0);

        if (BeginQuery())
        {
            const auto caledDim{ CalArrayDim(m_currCusor) };

            if (caledDim > 0)
            {
                if (dim != kArrDimMax && dim != caledDim)
                    m_isCorrectQuery = false;
                else
                    m_currCusor += caledDim;    // 인정된 차원만큼 커서를 점프
            }
            else
            {
                m_isCorrectQuery = false;
                EndQuery();
            }
        }

        return *this;
    }

    CDeclQuery& CDeclQuery::Array(const ETypeQualifier qf, const size_t dim) noexcept
    {
        Expects(dim > 0);

        if (BeginQuery())
        {
            if (m_declTypes[m_currCusor].qualifier != qf)
                m_isCorrectQuery = false;

            if (m_isCorrectQuery)
            {
                const auto caledDim{ CalArrayDim(m_currCusor) };

                if (caledDim > 0)
                {
                    if (dim != kArrDimMax && dim != caledDim)
                        m_isCorrectQuery = false;
                    else
                        m_currCusor += caledDim;    // 인정된 차원만큼 커서를 점프
                }
                else
                {
                    m_isCorrectQuery = false;
                    EndQuery();
                }
            }
        }

        return *this;
    }



    bool CDeclQuery::operator==(const CDeclQuery& rhs) noexcept
    {
        return m_declTypes == rhs.m_declTypes;
    }

    size_t CDeclQuery::CalArrayDim(const pos_type cursor) noexcept
    {
        Expects(cursor < m_declTypes.size());

        size_t dim{ 0 };
        for (auto i{ cursor }; i < m_declTypes.size(); ++i)
        {
            if (m_declTypes[i].typeCLASS == ETypeCLASS::Array)
                ++dim;
            else
                break;
        }

        return dim;
    }

    bool CDeclQuery::BeginQuery() noexcept
    {
        if (m_isCorrectQuery)
        {
            if (m_currCusor > m_declTypes.size())
                m_isCorrectQuery = false;
        }

        return m_isCorrectQuery;
    }

    void CDeclQuery::EndQuery() noexcept
    {
        ++m_currCusor;
    }

    template<ETypeCLASS... _MatchTypes>
    void CDeclQuery::QueryIgnoringQF() noexcept
    {
        if (BeginQuery())
        {
            if ((m_declTypes[m_currCusor].typeCLASS != _MatchTypes) || ...)
                m_isCorrectQuery = false;

            EndQuery();
        }
    }

    template<ETypeCLASS... _MatchTypes>
    void CDeclQuery::QueryQF(const ETypeQualifier qf) noexcept
    {
        if (BeginQuery())
        {
            if (m_declTypes[m_currCusor].qualifier != qf)
                m_isCorrectQuery = false;

            if (m_isCorrectQuery)
            {
                if ((m_declTypes[m_currCusor].typeCLASS != _MatchTypes) || ...)
                    m_isCorrectQuery = false;
            }

            EndQuery();
        }
    }

    void ddwh() {}

    bool testtest1()
    {
        struct DJ
        {
            void ok(){};
        };

        using FM = decltype(&DJ::ok);

        auto& fms{ ddwh };

        using T1 = volatile int const*;
        using T2 = T1 const*;
        int n;
        int const* const a[10]{};
        int* b[10][20];
        const T2 c[10]{};
        int* d[10][20][30][40];

        constexpr auto t{ GetTypeCLASS< decl_type_level<decltype(d), 3>::type>() };


        CDeclQuery q{ nullptr };

        q.Is().Seed().FuncPtr();

        // const int* 상수 객체애 대한 포인터임
        //원인 const int* 에서 포인터가 제거되면 int가 되어야 하지만 안그럼 const int
        constexpr auto d{ _internal_type_info::SDeclTypeStorage<decltype(fms)>::declTypes };
    }
}
