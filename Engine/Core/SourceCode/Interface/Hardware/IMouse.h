/**
@file IMouse.h
@brief Mouse Interface, EventType, Listener를 정의 합니다.
@date 2018-10-22  오후 21:34:26
*/


#pragma once


#include "CoreHelper.h"
#include "EnvironmentValue.h"
#include "Common/BaseTypes.h"
#include "Math/Point.h"


namespace Core
{
	/**
	마우스 좌표를 나타내기 위한 구조체 입니다.
	부호있는 32bit x, y값을 가지고 있습니다.
	*/
	using SMousePos = Math::SPoint2D<int32>;


	/**
	마우스 이벤트를 나타내는 열거형 입니다.
	*/
	enum EMouseEvent
	{
		MOVE,
		LBUTTONDOWN,
		LBUTTONUP,
		LBUTTONDOUBLECLICK,
		RBUTTONDOWN,
		RBUTTONUP,
		RBUTTONDOUBLECLICK,
		MBUTTONDOWN,
		MBUTTONUP,
		MBUTTONDOUBLECLICK,
		WHEEL
	};


	/**
	MouseEventListener 입니다.
	*/
	struct IMouseEventListener
	{
		virtual ~IMouseEventListener() {}

		/**이벤트가 일어난 좌표 값, 이벤트 타입, 휠 간격을 전달 받습니다.*/
		virtual void OnMouseEvent(int32 x, int32 y, EMouseEvent eventType, int32 wheelDelta = 0) = 0;
	};


	struct IMouse
	{
		/**마우스 DefaultCurosrID 상수*/
		static constexpr uint32 kDefaultCursorID = SELECTED_MOUSE_DEFAULT_CURSOR;

		virtual ~IMouse() {}

		//set
		/**마우스 위치를 조정 합니다.*/
		virtual void SetPos(const int32 x, const int32 y) = 0;
		/**마우스 커서모양을 변경합니다.*/
		virtual void SetMouseCursor(const uint32 id = kDefaultCursorID) = 0;    
		 

		//get
		virtual int32 GetPosX()    const noexcept = 0;
		virtual int32 GetPosY()    const noexcept = 0;
		virtual SMousePos GetPos() const noexcept = 0;
		virtual uint32 GetMouseCursor() const noexcept = 0;

		//Event
		virtual void OnMouseEvent(const int32 x, const int32 y, const EMouseEvent eventType, const int32 wheelDelta = 0) = 0;
		
		virtual bool RegisterMouseEventListener(const IMouseEventListener* pListener, const char* pszName) = 0;
		virtual bool RemoveMouseEventListener(const IMouseEventListener* pListener) = 0;
	};
}