/**
@file CoreHelper.h
@brief Core에서 코드 작성에 도움을 주는 함수들을 정의 합니다.
@date 2018-08-19  오후 18:39:44
*/


#pragma once





/*
    컴파일러 타입 선언
*/
#define MS_COMPILER  0x00000001 /**Visual Studio Compiler*/
#define GCC_COMPILER 0x00000002 /**GCC Compiler*/


/**
    사용할 컴파일러에 따른 지원 컴파일러 매크로 값 정의
    컴파일러는 반드시 c++17 이상을 지원해야 합니다.
*/
#if defined(_MSC_VER)
    //VS2017 이상만 지원
#   if _MSC_VER >= 1915
#      define SELECTED_COMPILER MS_COMPILER      /**선택된 컴파일러 버전*/
#   else
#      error "This MSC Version is not support."
#endif

#elif defined(__GNUC__)
    //GCC
#   error "Gcc Version is not implement"
#else
#   error "Not defined compiler type."
#endif


#ifndef SELECTED_COMPILER
#   error "SELECTED_COMPILER is not defined"
#endif


/**
    Compiler 타입에 따른 컴파일 도움 매크로 함수 구현
    COMPILE_INFO    => 컴파일시 문자열을 컴파일 출력창에 정보를 출력합니다.
    COMPILE_WARNING => 컴파일시 경고를 출력합니다.
    COMPILE_ERROR   => 컴파일시 에러를 발생시키고 출력합니다.
*/
#define __STRINGIZE(x) #x
#define STRINGIZE(x)  __STRINGIZE(x)
#define __MESSAGE_FORMAT __FILE__ "(" STRINGIZE(__LINE__) ") Message : "
#define __MESSAGE(text) __MESSAGE_FORMAT STRINGIZE(text)
#define __MESSAGE_NOT_FORMAT(text) STRINGIZE(text)


/**
   컴파일러 종류에 따른 에러 처리 매크로
*/
#if SELECTED_COMPILER == MS_COMPILER
#   define COMPILE_INFO(text) __pragma( message("Compile Info : " __MESSAGE_NOT_FORMAT(text)) ) /**컴파일 정보를 발생시킵니다.*/
#   define COMPILE_WARNING(text) __pragma( message("COMPILE_WARNING : " __MESSAGE(text)) ) /**컴파일 경고를 발생시킵니다.*/
#   define COMPILE_ERROR(text) static_assert(false, "COMPILE_ERROR : " __MESSAGE(text))    /**컴파일 경고를 발생시킵니다.*/
#elif SELECTED_COMPILER == GCC_COMPILER
#   error "Gcc Version is not implement" 
#else
#   error "Not defined support compiler type."
#endif


/*
    인라인 매크로
    ILINE => inline
    FLINE => forceinline
*/
//컴파일러 특화
#if   SELECTED_COMPILER == MS_COMPILER
#   define ILINE __inline      /**인라인 매크로*/
#   define FLINE __forceinline /**강제 인라인 매크로*/
#elif SELECTED_COMPILER == GCC_COMPILER
#   error "Gcc Version is not implement" 
#else
#   error "Not defined support compiler type."
#endif


/**
  API 함수를 정의할 매크로를 정의 합니다.
  CORE_EXPORTS가 정의 되어 있다면 API 매크로는 동적라이브러리 출력모드입니다.
*/
#ifndef CORE_EXPORTS
#    define TRADE_MODE 0
#else
#    define TRADE_MODE 1
#endif

//DLL EXPROTS는 USING_DLL 매크로가 정의 되어 있어야 합니다.
#if TRADE_MODE == 1 && defined(USING_DLL) //수출상태
//컴파일러 특화
#   if SELECTED_COMPILER == MS_COMPILER
#   define API __declspec(dllexport)
#   else
    COMPILE_ERROR("Gcc Version is not implement");
#   endif
#elif defined(USING_DLL)                  //수입상태
//컴파일러 특화
#   if SELECTED_COMPILER == MS_COMPILER
#   define API __declspec(dllimport)
#   else
    COMPILE_ERROR("Gcc Version is not implement");
#   endif
#else //정적 라이브러리 모드
#define API
#endif


#ifndef API
    COMPILE_ERROR("API is not defined");
#endif


/**
    코딩을 하면서 자주 사용되는 코드 SafeGuard
    코드 작성시 반드시 기존 알려진 변수 작명 규칙을 사용해야 한다.
    매크로 명의 시작은 항상 GUARD_로 시작해야 한다.

    ex)
    operator=의 전달 인자 작명 rhs
    복제 생성자 전달 객체 인자 작명 src 
*/
#define GUARD_SELF_ASSIGNMENT if(this == &rhs) return *this


/**
    해당 함수의 반환값을 컴파일 타임으로 확정 짓습니다. 해당 함수는 예외를 발생 시킬 수 없습니다.
*/
#define COMPILE_TIME_FUNCTION(FunctionPrototype) FLINE constexpr FunctionPrototype noexcept


///////////////
//TEST DEFINED
///////////////

#define SAFE_DELETE(_TARGET_PTR_) if(_TARGET_PTR_ != nullptr) {delete _TARGET_PTR_;}

#define __DISALLOW_CREATE_INSTANCE_RAW(CLASS_NAME)\
CLASS_NAME() = default;\
CLASS_NAME(const CLASS_NAME&) = default;\
CLASS_NAME& operator=(const CLASS_NAME&) = default

/**
	해당 매크로를 정의한 클래스는 인스턴스 생성을 비허용 합니다.
*/
#define DISALLOW_CREATE_INSTANCE(CLASS_NAME)\
protected:\
__DISALLOW_CREATE_INSTANCE_RAW(CLASS_NAME)

/**
	해당 매크로를 정의한 클래스는 인스턴스 생성을 자식까지 비허용 합니다.
*/
#define DISALLOW_CREATE_INSTANCE_PERMANENTLY(CLASS_NAME)\
private:\
__DISALLOW_CREATE_INSTANCE_RAW(CLASS_NAME)


/**
	해당 매크로를 정의한 클래스는 복제 생성자를 비허용 합니다.
*/
#define DISALLOW_COPY_COTR(CLASS_NAME)\
private:\
CLASS_NAME(const CLASS_NAME&) = delete

/**
	해당 매크로를 정의한 클래스는 복제 생성자, 대입연산자를 비허용 합니다.
*/
#define DISALLOW_COPY_COTR_AND_ASSIGN(CLASS_NAME)\
DISALLOW_COPY_COTR(CLASS_NAME);\
CLASS_NAME& operator=(const CLASS_NAME&) = delete


#define DISABLE_HEADER_FILE static_assert(false, __FILE__##" file is Disabled")