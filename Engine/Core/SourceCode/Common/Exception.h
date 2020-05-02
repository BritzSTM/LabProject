/**
@file Exception.h
@brief
  예외처리를 위한 예외 객체 구현

  작명규칙
  1. 각단어는 대문자로 시작
  2. 띄어쓰기 표기 '_' 금지
  3. 항상 마지막에는 Exception을 붙인다.
  4. 클래스를 나타내는 C를 붙이지 않는다.

  사용가능 매크로 값
  USE_SHROT_CALL_STACK_DISPLAY
*/
#pragma once


#include "../Common/BaseTypes.h"
#include "../Common/EnumExtension.h"

#include <exception>
#include <stdexcept>


//*VS 자동완성기능 오류로 인한 static_assert 제어. 차후 기능이 개선되어 오류로 판단하지 않는다면 제거한다.*/
#define NOT_USE_EXCEPTION_BASE_CHECKER


namespace Core
{

    /**
    예외의 위험 레벨을 나타내는 수치 입니다
    Negligible 시스템 운행에 영향이 거의 없어 수용 가능
    Normal 단기적으로 수용가능. 차후에 처리가 필요함
    Critical 즉시 처리가 필요한 레벨
    */
    DEFINE_EXT_ENUM
    (
        enum class, EExceptionLevel, ENUM_SIZE_DEFAULT,
        E_NAME(Negligible)
        E_NAME(Normal)
        E_NAME(Critical)
    );


    /**기본 예외객체 입니다*/
    class Exception
        : public std::exception
    {
    protected:
        static ConstexprString(s_kpszUnknownException, "Unknown Exception");
        static ConstexprString(s_kpszUnknownWhere, "Unknown Where");

    public:
        virtual ~Exception() {}

        Exception(
            const char* const pszMessage,
            const char* const pszWhere,
            const EExceptionLevel level,
            const intptr subDataAddress) noexcept
            : exception(pszMessage)
            , m_Level(level)
            , m_pszWhere(pszWhere)
            , m_subDataAddress(subDataAddress)
        {

        }

        Exception() noexcept
            : Exception(
                s_kpszUnknownException,
                s_kpszUnknownWhere,
                EExceptionLevel::Enum::Normal,
                PtrTypeTraits<intptr>::ErrorAddress)
        {

        }

        /**컴파일 빌드 호환성을 위해 남겨둔 생성자. 더 이상 사용금지*/
        [[deprecated("Do not used ctor")]]Exception(const char* const pszMessage) noexcept
            : Exception(
                pszMessage,
                s_kpszUnknownWhere,
                EExceptionLevel::Enum::Normal,
                PtrTypeTraits<intptr>::ErrorAddress)
        {

        }

        /**예외의 위험 레벨 획득*/
        virtual const EExceptionLevel GetLevel() const noexcept
        {
            return m_Level;
        }

        /**사용자가 획득 가능한 데이터의 주소*/
        virtual const intptr GetSubDataAddress() const noexcept
        {
            return m_subDataAddress;
        }

        /**예외가 발생한 지점*/
        virtual const char* GetWhere() const noexcept
        {
            return m_pszWhere;
        }

    private:
        const EExceptionLevel m_Level;
        const char* const m_pszWhere;
        const intptr m_subDataAddress;
    };



#if defined(NOT_USE_EXCEPTION_BASE_CHECKER)
#   define __EXCEPTION_BASE_CHECKER(_EXCEPTION_TYPE_)
#else
#   define __EXCEPTION_BASE_CHECKER(_EXCEPTION_TYPE_)\
      static_assert(std::is_base_of<Core::Exception,##_EXCEPTION_TYPE_>::value, "Exception object is not based on Core::Exception")
#endif

#define NO_DEFINE_DETAILS

    /** 예외 객체를 정의 합니다.Exception객체 상속여부를 정적으로 검사합니다.
    DETAILS를 정의하지 않을 때에는 NO_DEFINE_DETAILS 인자전달*/
#define DEFINE_EXCEPTION_TYPE(_PARENT_NAMESPACE_, _PARENT_TYPE_, _EXCEPTION_NAME_, _DETAILS_)\
class _EXCEPTION_NAME_                                                                                                       \
	: public _PARENT_NAMESPACE_::_PARENT_TYPE_                                                                               \
{                                                                                                                            \
__EXCEPTION_BASE_CHECKER(_EXCEPTION_NAME_);                                                                                  \
public:                                                                                                                      \
    virtual ~_EXCEPTION_NAME_(){};                                                                                           \
                                                                                                                             \
    _EXCEPTION_NAME_(const char* const pszMessage, const char* const pszWhere, const Core::EExceptionLevel level, const intptr subDataAddress) noexcept        \
        : _PARENT_TYPE_(pszMessage, pszWhere,Core::EExceptionLevel::Enum::Normal, PtrTypeTraits<intptr>::ErrorAddress) {}             \
	_EXCEPTION_NAME_() noexcept                                                                                              \
    	: _PARENT_TYPE_(#_EXCEPTION_NAME_, s_kpszUnknownWhere, Core::EExceptionLevel::Enum::Normal, PtrTypeTraits<intptr>::ErrorAddress) {}      \
    _EXCEPTION_NAME_(const char* const pszMessage) noexcept                                                                  \
    	: _PARENT_TYPE_(pszMessage, s_kpszUnknownWhere, Core::EExceptionLevel::Enum::Normal, PtrTypeTraits<intptr>::ErrorAddress) {}             \
                                                                                                                             \
	_DETAILS_                                                                                                                \
}

#define _FILE_LINE_INFO_STR __FILE__ " (" STRINGIZE(__LINE__) ")"

    /**예외처리 문자열에 붙는 서식정보*/
#define _EXCEPTION_WHERE_FORMAT __FUNCSIG__ " [" _FILE_LINE_INFO_STR "]"

/**에러메시지 문자열을 작성합니다. 메시지 마지막에 오류서식정보가 붙습니다.*/
#define EF_MSG(_TEXT_MESSAGE_) _TEXT_MESSAGE_ __FUNCSIG__

/**사용하려면 TEXTL이 사용가능해야 합니다.*/
#define EF_TMSG(_TEXT_MESSAGE_) EF_MSG(TEXTL(_TEXT_MESSAGE_))

/**예외를 던지는 Macro. 예외의 위험레벨이 Normal로 설정됩니다. 던지는 예외 타입에 대해 Core::Exception 상속 정적 검사를 진행함*/
#define THROWER(_EXCEPTION_TYPE_, _TEXT_MESSAGE_)\
__EXCEPTION_BASE_CHECKER(_EXCEPTION_TYPE_);\
throw _EXCEPTION_TYPE_(_TEXT_MESSAGE_, _EXCEPTION_WHERE_FORMAT, Core::EExceptionLevel::Enum::Normal, Core::BaseTypes::PtrTypeTraits<intptr>::ErrorAddress)

/**예외를 던지는 Macro. 던지는 예외 타입에 대해 Core::Exception 상속 정적 검사를 진행함*/
#define THROWER_DETAIL(_EXCEPTION_TYPE_, _EXCEPTION_LEVEL_, _TEXT_MESSAGE_, _OBJECT_ADDR_)\
__EXCEPTION_BASE_CHECKER(_EXCEPTION_TYPE_);\
throw _EXCEPTION_TYPE_(_TEXT_MESSAGE_, _EXCEPTION_WHERE_FORMAT, _EXCEPTION_LEVEL_, _OBJECT_ADDR_)




    //기본예외 정의 영역

    DEFINE_EXCEPTION_TYPE
    (
        Core, Exception, LogicErrorException, NO_DEFINE_DETAILS
    );

    DEFINE_EXCEPTION_TYPE
    (
        Core, Exception, RuntimeErrorException, NO_DEFINE_DETAILS
    );

    DEFINE_EXCEPTION_TYPE
    (
        Core, LogicErrorException, InvalidArgumentException, NO_DEFINE_DETAILS
    );

    DEFINE_EXCEPTION_TYPE
    (
        Core, InvalidArgumentException, InvalidArgumentNullptrException, NO_DEFINE_DETAILS
    );

    DEFINE_EXCEPTION_TYPE
    (
        Core, LogicErrorException, EmptyException, NO_DEFINE_DETAILS
    );

    DEFINE_EXCEPTION_TYPE
    (
        Core, LogicErrorException, NotImplementedException, NO_DEFINE_DETAILS
    );

    DEFINE_EXCEPTION_TYPE
    (
        Core, RuntimeErrorException, InaccessibleAreaException, NO_DEFINE_DETAILS
    );

    DEFINE_EXCEPTION_TYPE
    (
        Core, LogicErrorException, OutOfRangeException, NO_DEFINE_DETAILS
    );

}