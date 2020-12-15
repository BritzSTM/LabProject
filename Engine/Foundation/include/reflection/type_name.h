#pragma once

#include "../compiler_directive.h"


#if (FD_CURR_COMPILER == FD_MSVC_COMPILER)
#include "type_name_msvc.h"

#elif (FD_CURR_COMPILER == FD_CLANG_COMPILER)
#include "type_name_clang.h"

#else
#   error Not impl type_name

#endif
