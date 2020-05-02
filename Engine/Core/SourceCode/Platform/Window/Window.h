#pragma once


#include "CoreHelper.h"
#include "Common/OSType.h"
#include "_WindowDetail.h"


namespace Platform
{
   
    //OsType에 따른 impl 처리
#if (TARGET_OS == OS_WIN64)
    using Window = _internal::_WindowsWindow;

#else
    COMPILE_ERROR("Not Selected Window Type");

#endif

}