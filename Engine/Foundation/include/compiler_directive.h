/**
   @brief 공통적인 컴파일러 명령을 정의하기 위한 파일 
*/
#pragma once


#define FD_UNKNOWN_COMPILER 0x00
#define FD_MSVC_COMPILER    0x01
#define FD_CLANG_COMPILER   0x02

#if defined(__clang__)
#   define FD_CURR_COMPILER FD_CLANG_COMPILER

#elif defined(_MSC_VER)
#   define FD_CURR_COMPILER FD_MSVC_COMPILER

#else
#   define FD_CURR_COMPILER FD_UNKNOWN_COMPILER

#endif
