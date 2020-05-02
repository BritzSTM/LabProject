#pragma once


#include "../CoreHelper.h"
#include "../Common/BaseTypes.h"
#include "../Common/EnumExtension.h"
#include "WindowBaseTypes.h"
#include "IWindow.h"


namespace Platform
{

    DEFINE_EXT_ENUM
    (
        enum class, EWindowEventTypes, ENUM_SIZE_SET(int16),
        E_NAME_V(None, -1)
        E_NAME(Create)       //창 인스턴스가 생성
        E_NAME(Activate)     //모든 내부리소스 이용 가능한 상태
        E_NAME(Close)        //닫기 직전 리소스 정리중
        E_NAME(Destroy)      //완전 파괴
        E_NAME(GetFocus)
        E_NAME(LostFocus)
        E_NAME(Moving)
        E_NAME(Movement)
        E_NAME(Sizing)
        E_NAME(Sized)
        E_NAME(Show)
        E_NAME(ShowFull)
        E_NAME(Hide)
        E_NAME(ChangeInputType) //외부 입력기의 변깅 ex)언어 변경
        E_NAME(Paint)
        E_NAME(Capture)
        E_NAME(LostDevice)
    );

    
    struct alignas(8) SWindowEvents
    {

        EWindowEventTypes Type;
        char pad[2];            
        union
        {
            int32 _data;
            SWindowSize Size;
            SWindowPos Pos;
        }Value;
        
    };

    static_assert(sizeof(SWindowEvents) == 8, "SWindowEvents is not 8byte");


    struct IWindowEventListener
    {
        DISALLOW_CREATE_INSTANCE(IWindowEventListener);

    public:
        virtual ~IWindowEventListener() {}

        virtual void OnWindowEvent(const SWindowEvents& event, IWindow* const pWIndow) = 0;
    };

    struct IWindowEventImmediatelyListener
    {
        DISALLOW_CREATE_INSTANCE(IWindowEventImmediatelyListener);

    public:
        virtual ~IWindowEventImmediatelyListener() {}

        virtual void OnWindowEventImmediately(const SWindowEvents& event, IWindow* const pWIndow) = 0;
    };

}