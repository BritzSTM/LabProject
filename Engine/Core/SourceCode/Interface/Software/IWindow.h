/**
@file IWindow.h
@brief WindowGUI interface를 정의 합니다.
@date 2018-10-22  오후 22:18:27
*/


#pragma once


#include "Common/BaseTypes.h"
#include "Platform/WrappedDependentBaseType.h"
#include "Event/Event.h"
#include "Container/string.h"
#include "Math/Point.h"
#include "Interface/Hardware/IMouse.h"


namespace Core
{

	ConstexprString(kDefaultTitle, "Undefined");

	/**
	창 크기를 나타내는 구조체
	8byte
	*/
	struct SWindowSize
	{
		uint32 Width;
		uint32 Height;
	};

	/**
	2차원 스크린상에서 창의 위치를 나타내기 위한 구조체
	8byte
	*/
	using SWindowTopLeftPosition = Math::SPoint2D<int32>;


	/**
	스크린 상태 대한 정의
	*/
	enum EWindowState
	{
		FullScreen, /**전체화면*/
		Window,     /**윈도우 */
		Minimum,    /**최소화 */
		MinimumBackGround  /**최소화, 동작 상태*/
	};


	/**
	창 스타일 정의
	*/
	enum EWindowStyle
	{
		NormalGame,
		NormalApp
	};

	
	static LRESULT CALLBACK DefaultWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
	{
		//if (message == Destroy)
		//{
		//	PostQuitMessage(0);
		//}
		
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	

	struct IWindow
	{
		//Handler type def
		CreateEventHandler(WindowMessageEventHandler, LRESULT (*)(HWND, UINT, WPARAM, LPARAM));


		//Methods
		virtual ~IWindow() {}


		//StateChage
		virtual bool Show() = 0;
		virtual bool Hide() = 0;


		//Set
		virtual void SetSize(const uint32 w, const uint32 h) = 0;
		virtual void SetTitleBar(const char* title) = 0;
		virtual void SetPos(const int32 topLeftX, const int32 topLeftY) = 0;
		virtual void SetStyle(const EWindowStyle style) = 0;
		virtual void SetState(const EWindowState state) = 0;
		virtual void SetMouse(const IMouse* pMouse) = 0;


		//Get
		virtual SWindowTopLeftPosition GetPos() const noexcept = 0;
		virtual string GetTitleName() const noexcept = 0;
		virtual SWindowSize GetSize() const noexcept = 0;
		virtual EWindowStyle GetStyle() const noexcept = 0;
		virtual EWindowState GetState() const noexcept = 0;
		virtual HWND GetHWND() const noexcept = 0;
		virtual IMouse* GetMouse() const noexcept = 0;


		virtual bool IsCreated() const noexcept = 0;
		virtual bool IsShow() const noexcept = 0;


		//Events
		virtual void SetMessageHandler(const WindowMessageEventHandler handler = DefaultWndProc) = 0;
		virtual WindowMessageEventHandler GetMessageHandler() const noexcept = 0;


		//창을 다시그리는 명령어
		virtual void Update() = 0;
	};


}