/**
@file Event.h
@brief 기본 이벤트를 지원하기 위한 기능을 제공합니다.
@date 2018-10-12  오후 21:54:35
*/


#pragma once


#include "../CoreHelper.h"


/**
이벤트 핸들을 정의 합니다. 
SIGNATURE는 함수 포인터 형을 제공해야 합니다.
ex void() -> void(*)()
*/
#define CreateEventHandler(NAME, SIGNATURE) using NAME = SIGNATURE

/**
Instance 클래스에 전달받은 Listener의 Tag구조체, Add, Remove Method를 추가합니다.
사용하지 말것 가독성 떨어짐
*/
#define CreateEventListenerMethods(EVENT_NAME, RETURN_TYPE, LISTENER_TYPE)\
virtual RETURN_TYPE Add##EVENT_NAME##Listener(const LISTENER_TYPE* pListener, const char* pszName) = 0;\
virtual RETURN_TYPE Remove##EVENT_NAME##Listener(const LISTENER_TYPE* pListener) = 0


/**
static 클래스에 전달받은 Listener의 Tag구조체, Add, Remove Method를 추가합니다.
사용하지 말것 가독성 떨어짐
*/
#define CreateEventListenerStaticMethods(EVENT_NAME, RETURN_TYPE, LISTENER_TYPE)\
static RETURN_TYPE Add##EVENT_NAME##Listener(const LISTENER_TYPE* pListener, const char* pszName);\
static RETURN_TYPE Remove##EVENT_NAME##Listener(const LISTENER_TYPE* pListener)
