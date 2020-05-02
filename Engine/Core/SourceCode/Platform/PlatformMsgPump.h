/**
현재까지는 이 헤더파일, 구현부에서만
운영체제 종속 형식을 사용할 수 있습니다.
다른 파일에서는 절대 사용하지 마십시오.
*/
#pragma once



#include "Common/OSType.h"
#include "WrappedDependentBaseType.h"
#include "Thread/Task.h"
#include "PlatformBroadcastSystem.h"


namespace Platform
{
         
    namespace _internal
    {
        
        struct alignas(8) _WindowsMsgPack
        {
            ::HWND Hwnd;
            ::UINT Message;
            ::WPARAM Wparam;
            ::LPARAM Lparam;
        };

        void _WindowsPumpMessage() noexcept;


        void _ProceedWindowsKeyboardMsg(_WindowsMsgPack& msgPack, bool& isOutput);
        void _ProceedWindowsMouseMsg(_WindowsMsgPack& msgPack, bool& isOutput);
        void _ProceedWindowsWindowMsg(_WindowsMsgPack& msgPack, bool& isOutput);
        Core::CTask* _SpwanMsgTask(_WindowsMsgPack* const pPack, bool* const pResPack);
        LRESULT WINDOWS_API _ProceedWindowMsgSource(
            HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

    }
    
    /**
    정의사항 기록

    1. PumpPlatformMessage는 반드시 void(*)()형을 따라야 합니다.
    */
#if TARGET_OS == OS_WIN64
    static const auto& PumpPlatformMessage = _internal::_WindowsPumpMessage;

#else
    COMPILE_ERROR("Not impl PumpPlatformMessage");

#endif

}