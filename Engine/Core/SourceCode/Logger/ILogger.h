/**
@file ILogger.h
@brief Logger의 interface를 정의합니다.
@date 2018-10-12  오후 22:15:45
*/


#pragma once


#include "Common/BaseTypes.h"
#include "Container/string.h"
#include "Event/Event.h"


namespace Core
{
	/**
	Recode의 인터페이스를 정의 합니다.
	*/
	struct IRecode
		: public IToString
	{
		virtual ~IRecode() {}

		/**Recode 메시지를 기록합니다.*/
		virtual void WriteRecode(string message) = 0;

		/**Recode 메시지를 비웁니다.*/
		virtual void ClearRecode() = 0;

		/**Recode를 문자열 정보로 반환합니다.*/
		virtual string ToString() const override = 0;
	};


	//전방선언
	struct ILogger;

	/**
	Logger에 새로운 레코드의 입력에 대한 Listener interface
	*/
	struct ILoggerEventOnNewRecodeListener
	{
		virtual ~ILoggerEventOnNewRecodeListener() {}

		/**새로운 레코드 등장*/
		virtual void OnNewRecode(const IRecode* pRecode) = 0;
	};


	/**
	Logger가 full state때 발생하는 이벤트에 대한 Listener interface
	*/
	struct ILoggerEventOnFullLoggerListener
	{
		virtual ~ILoggerEventOnFullLoggerListener() {}

		/**로거가 가득 참*/
		virtual void OnFullLogger(ILogger* pLogger) = 0;
	};


	/**
	@brief Logger의 기본 인터페이스. ThreadSafe 함을 보장해야 함.
	@remarks
	순수하게 메시지에 대한 기록 확인을 위한 인터페이스
	파일 or 다른 스트림에 대한 출력 인터페이스는 구현사항이 아니다
	@date 2018-08-24  오후 12:45:22
	@version
	*/
	struct ILogger
	{
		CreateEventHandler(OnFullLoggerHandler, void(*)(ILogger*));
		CreateEventHandler(OnNewRecodeHandler, void(*)(ILogger*));


		virtual ~ILogger() {}

		/**Message를 삽입합니다.*/
		virtual void InsertRecode(const char* pszMsg) = 0;

		/**모든 Log내용을 문자열Vector로 반환함*/
		[[nodiscard]]virtual std::vector<string> GetReoprt() = 0;


		//Event Listener Methods
		CreateEventListenerMethods(OnNewRecode, void, ILoggerEventOnNewRecodeListener);
		CreateEventListenerMethods(OnFullLogger, void, ILoggerEventOnFullLoggerListener);


		//Handler
		virtual void RegisterFullLoggerHandler(const OnFullLoggerHandler& handler) = 0;
		virtual void RegisterOnNewRecodeHandler(const OnNewRecodeHandler& handler) = 0;
	};

}