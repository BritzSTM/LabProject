#pragma once


#include "../CoreHelper.h"
#include "../Common/BaseTypes.h"
#include "../Common/EnumExtension.h"


namespace Platform
{

    DEFINE_EXT_ENUM
    (
        enum, EMouseEventType, ENUM_SIZE_SET(int16),
        E_NAME_V(None, -1)
        E_NAME(LeftButtonDown)
        E_NAME(LeftButtonUp)
        E_NAME(LeftButtonDoubleClick)
        E_NAME(RightButtonDown)
        E_NAME(RightButtonUp)
        E_NAME(RightButtonDoubleClick)
        E_NAME(WhellRotate)
        E_NAME(WhellButtonDown)
        E_NAME(WhellButtonUp)
        E_NAME(WhellButtonDoubleClick)
        E_NAME(Movement)
        E_NAME(LeaveGUI)
        E_NAME(CaptureChange)
    );

    
    //이벤트 타입, 위치,
    struct SMouseEvents
    {
        EMouseEventType Type; //2byte
        int16 WhellDelta;     //2byte
        
        //4byte
        union                 
        {
            uint32 _data;
            struct SPos
            {
                int16 X;
                int16 Y;
            }Pos;
        };
    };

    static_assert(sizeof(SMouseEvents) == 8, "SMouseEvents size is not 8byte");


    using MouseEventHandler = void(*)(const SMouseEvents&);

    struct IMouseEventListener
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(IMouseEventListener);

    public:
        virtual ~IMouseEventListener() {}

        virtual void OnMouseEvent(const SMouseEvents& event) = 0;

    };

    struct IMouseEventImmediatelyListener
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(IMouseEventImmediatelyListener);

    public:
        virtual ~IMouseEventImmediatelyListener() {}

        virtual void OnMouseEventImmediately(const SMouseEvents& event) = 0;

    };

}