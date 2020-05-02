#pragma once

#include "../CoreHelper.h"
#include "../Common/BaseTypes.h"
#include "../Common/EnumExtension.h"


namespace Platform
{

    DEFINE_EXT_ENUM
    (
        enum, EKeyboardEventTypes, ENUM_SIZE_SET(int16),
        E_NAME_V(None, -1)
        E_NAME(KeyDown)
        E_NAME(KeyUp)
    );

    struct alignas(8) SKeyboardEvents
    {
        EKeyboardEventTypes Type;
        char pad[2];
        wchar_t KeyValue;
    };

    static_assert(sizeof(SKeyboardEvents) <= 8, "SKeyboardEvents type size error");


    struct IKeyboardEventListener
    {
        DISALLOW_CREATE_INSTANCE(IKeyboardEventListener);
    
    public:
        virtual ~IKeyboardEventListener() {}

        virtual void OnKeyboardEvent(const SKeyboardEvents& event) = 0;
    };

    struct IKeyboardEventImmediatelyListener
    {
        DISALLOW_CREATE_INSTANCE(IKeyboardEventImmediatelyListener);

    public:
        virtual ~IKeyboardEventImmediatelyListener() {}

        virtual void OnKeyboardEventImmediately(const SKeyboardEvents& event) = 0;
    };

}