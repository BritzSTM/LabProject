/**
@file  EngineFramework.h
@brief EngineFramework를 구성함
@date 2018-10-22  오후 21:15:50
*/


#pragma once


#include "CoreHelper.h"



namespace Engine
{
	//전방선언
	namespace Window
	{
		struct IWindow;
	}


	struct API IEngineFramework
	{
		virtual ~IEngineFramework() {}

		virtual bool InitializCore(const char* pCmd) = 0;
		virtual bool InitializSystem() = 0;
		virtual int  Start() = 0;
		virtual void Stop() = 0;

		virtual LRESULT OnWindowMessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) = 0;


	};


	class CEngineFramework final 
		: public IEngineFramework
	{
	public:
		CEngineFramework() = delete;
		CEngineFramework(HINSTANCE hInstace);

		virtual ~CEngineFramework();

		bool InitializCore(const char*    pCmd);
		bool InitializSystem();
		int Start();             /**Engine Framework를 시작합니다.*/
		void Stop();

	private:
		void Release();
		FLINE int ServiceLoop();

		/**윈도우를 생성하며 이벤트 메시지를 설정할 수 있는 곳입니다.*/
		bool _CreateWindow();
		LRESULT OnWindowMessageHandle(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
		static LRESULT _GameWindowHandler(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	private:
		const HINSTANCE m_hInstance;
		bool m_initializedCoreSystem;   /**코어 처기화 유무*/
		bool m_initializedEngineSystem; /**시스템이 초기화된 상태인가*/
		bool m_isRunning;               /**동작 모드인가*/
		bool m_isReady;                 /**모든것들이 준비된 상태인가*/
		Engine::Time::CTimer m_GameTimer;

		//std::unique_ptr<Core::IWindow> m_pWindow;

	};












}