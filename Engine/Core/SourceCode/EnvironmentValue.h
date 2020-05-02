/**
@file  EnvironmentValue.h
@brief 중요 매크로 환경 변수들이 선언되어 있습니다.
@date 2018-10-11  오후 14:20:43
*/
#pragma once


//user
#include "CoreHelper.h"


/*
   Target Architecture에 대해 정의 합니다. 
   반드시 정의 되어 있어야 합니다.

   Architecture 상수 매크로 값은 Platform\Common\ArchType.h에 정의되어 있습니다.
*/
#if defined(_M_X64)
/**Target  Architecture*/
#   define TARGET_ARCH ARCH_X64
#else
    COMPILE_ERROR("Not defined ArchitectureType");
#endif


/*
   Target Operating System에 대해 정의 합니다.
   반드시 정의 되어 있어야 합니다.

   Operating System 상수 매크로 값은 Platform\Common\OSType.h에 정의되어 있습니다.
*/
#if defined(_WIN64) || defined(WIN64)
/**목표 OS 설정*/
#   define TARGET_OS OS_WIN64
/**운영체제에 따른 유니코드 코드페이지 windows->CP_UTF8*/
#   define UTF8_CODE_PAGE 65001

/**운영체제에 따른 기본 마우스 커서 ID*/
#   define SELECTED_MOUSE_DEFAULT_CURSOR 32512

#else
    COMPILE_ERROR("Not defined OSType");
#endif







/*
  Container Env
*/



#define CONTAINER_TYPE_STL 0
#define CONTAINER_TYPE_TBB 1

#ifndef USING_STL_CONTAINER
#    define SELECTED_CONTAINER_TYPE CONTAINER_TYPE_TBB
#else
#    define SELECTED_CONTAINER_TYPE CONTAINER_TYPE_STL
#endif


/*
   사용할 문자열 컨테이너를 선택하기 위한 매크로 정의
   문자열 컨테이너만 특별히 따로 분리
*/
#define CORE_STRING 0
#define STL_STRING  1

#ifdef USING_CORE_STRING
#   define SELECTED_STRING_CONTAINER CORE_STRING
#else
#   define SELECTED_STRING_CONTAINER STL_STRING
#endif


/*
   사용할 HashValue Size
*/
#ifdef USING_CORE_HASH_32BIT
#   define SELECTED_CORE_HASH_SIZE HASH32BIT
#else
#   define SELECTED_CORE_HASH_SIZE HASH64BIT
#endif



/**
Container들의 예외 발생 활성화 여부 설정 전처리
기본적으로 정의가 안된 경우 SELECTED_CONTAINER_EXCEPTION 모드를 사용
*/
#if !defined(SELECTED_CONTAINER_EXCEPTION) || !defined(SELECTED_CONTAINER_EXCEPTION_NOT_THROW)
#   if defined(DEBUG) || defined(_DEBUG)
#      define SELECTED_CONTAINER_EXCEPTION
#   else
#      define SELECTED_CONTAINER_EXCEPTION_NOT_THROW
#   endif
#endif




/*
   필수 매크로 환경 변수 값들이 정의되어 있는지 확인하는 영역 
*/

#ifndef TARGET_ARCH
    COMPILE_ERROR("TARGET_ARCH is not defined");
#endif

#ifndef TARGET_OS
    COMPILE_ERROR("TARGET_OS is not defined");
#endif

#ifndef UTF8_CODE_PAGE
    COMPILE_ERROR("UTF8_CODE_PAGEH is not defined");
#endif

#ifndef SELECTED_MOUSE_DEFAULT_CURSOR
	COMPILE_ERROR("SELECTED_MOUSE_DEFAULT_CURSOR is not defined");
#endif

#ifndef UNICODE
    COMPILE_ERROR("UNICODE is not defined");
#endif

#ifndef SELECTED_STRING_CONTAINER
	COMPILE_ERROR("SELECTED_STRING_CONTAINER is not defined");
#endif

#ifndef SELECTED_CORE_HASH_SIZE
	COMPILE_ERROR("SELECTED_CORE_HASH_SIZE is not defined");
#endif

#ifndef SELECTED_CONTAINER_TYPE
	COMPILE_ERROR("SELECTED_CONTAINER_TYPE is not defined");
#endif

/**링크 경고를 출력하지 않기 위한 더미 함수*/
void __DisableLinkDummy();


//#define USE_SHORT_CALL_STACK_DISPLAY
//USING_CORE_HASH_32BIT


namespace Core
{
    //메모리 부족시 대응하기 위한 예약영역 크기
    constexpr size_t g_kSystemEmergencyMemorySize = 4096;

    //독자 메모리 Handler 사용여부
    constexpr bool g_kSystemMemoryHandlerEnabled = true;

    //메모리 핸들러 설정
    constexpr const char* g_kMemoryHandlerType = "EmergencyMemoryHandler";
}
