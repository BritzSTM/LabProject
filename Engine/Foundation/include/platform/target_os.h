#pragma once

#include "target_arch.h"


#define FD_TARGET_OS_NONE    0x00
#define FD_TARGET_OS_WIN64   0x01
#define FD_TARGET_OS_LINUX64 0x02


#if ( defined(WIN64) || defined(_WIN64) )
#   define FD_TARGET_OS FD_TARGET_OS_WIN64

#elif ( defined(__linux__) && (( FD_TARGET_ARCH == FD_ARCH_X64 ) || (FD_TARGET_ARCH == FD_ARCH_ARM64)) )
#   define FD_TARGET_OS FD_TARGET_OS_LINUX64

#else
#   error "FD_TARGET_OS is Not impl" 

#endif
