/**
    @brief Architecture를 정의합니다. 현재 이 라이브러리는 64bit만 지원합니다. 
*/
#pragma once


#define FD_ARCH_NONE   0x00
#define FD_ARCH_X64    0x01
#define FD_ARCH_ARM64  0x02


#if defined(__amd64) || defined(__amd64__) || defined(_M_X64)
#   define FD_TARGET_ARCH FD_ARCH_X64

#elif defined(__aarch64__) || defined(_M_ARM64)
#   define FD_TARGET_ARCH FD_ARCH_ARM64

#else
#   define FD_TARGET_ARCH FD_ARCH_NONE 

#endif
