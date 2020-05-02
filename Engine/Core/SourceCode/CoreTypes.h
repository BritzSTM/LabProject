

#pragma once


#include "Common/BaseTypes.h"


namespace Core
{

#if TARGET_OS == OS_WIN64
	/**
	SystemEvent를 정의합니다.
	필요한 이벤트만 Wrapping 하거나 정의 함
	*/
	enum ESystemEvent : UINT
	{
		WindowCreated        = WM_CREATE,
		WindowMove           = WM_MOVE,
		WindowResize         = WM_SIZE,
		WindowActivate       = WM_ACTIVATE,
		WindowSetFocus       = WM_SETFOCUS,
		WindowKillFocus      = WM_KILLFOCUS,
		WindowPosChange      = WM_WINDOWPOSCHANGING,
		InputLanguageChange  = WM_INPUTLANGCHANGE,
		KeyDown              = WM_KEYDOWN,
		KeyUP                = WM_KEYUP,
		CriticalError        = 0xFFFF - 2,
		ReleaseSystem        = 0xFFFF - 1,
		LostDevice           = 0xFFFF
	};

	enum ESystemActivateEvent : UINT
	{

	};

#endif



}