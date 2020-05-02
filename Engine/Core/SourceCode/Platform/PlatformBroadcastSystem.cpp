#include "stdafx.h"
#include "PlatformBroadcastSystem.h"


namespace Platform
{

    void CMouseEventBroadcastingStation::Update()
    {
        SMouseEvents popEvent;
        while (m_eventQueue.try_pop(popEvent))
        {
            _NotifyParallely(popEvent);
        }
    }

    /**즉시 전파를 시작하며 IsNotifiyChain의 값이 참인경우 하위 항목에게도 즉시 통지를 합니다.*/
    void CMouseEventBroadcastingStation::NotifiyImmediately(const SMouseEvents& event, bool IsNotifiyChain)
    {
        using namespace Core;

        CLambdaTask delegator([event, this]()
        {
            parallel_for(
                blocked_range<size_t>(0, m_immediatelyListenerList.Size(), s_kListenerGrain),
                CMouseEventImmediatelyBroadcaster(event, &m_immediatelyListenerList));
        });

        if (IsNotifiyChain)
        {
            CLambdaTask delegator([event, this]()
            {
                Notify(event, true);
            });
        }

    }

    void CKeyboardEventBroadcastingStation::Update()
    {
        SKeyboardEvents popEvent;
        while (m_eventQueue.try_pop(popEvent))
        {
            _NotifyParallely(popEvent);
        }
    }

    void CKeyboardEventBroadcastingStation::NotifiyImmediately(const SKeyboardEvents& event, bool IsNotifiyChain)
    {
        using namespace Core;

        CLambdaTask delegator([event, this]()
        {
            parallel_for(
                blocked_range<size_t>(0, m_immediatelyListenerList.Size(), s_kListenerGrain),
                CKeyboardEventImmediatelyBroadcaster(event, &m_immediatelyListenerList));
        });

        if (IsNotifiyChain)
        {
            CLambdaTask delegator([event, this]()
            {
                Notify(event, true);
            });
        }

    }


    void CWindowEventBroadcastingStation::Update()
    {
        SWindowEventsPair popEvent;
        while (m_eventQueue.try_pop(popEvent))
        {
            _NotifyParallely(popEvent.first, popEvent.second);
        }
    }

    void CWindowEventBroadcastingStation::NotifiyImmediately(const SWindowEvents& event, IWindow* const pWindow, bool IsNotifiyChain)
    {
        using namespace Core;

        CLambdaTask delegator([event, pWindow, this]()
        {
            parallel_for(
                blocked_range<size_t>(0, m_immediatelyListenerList.Size(), s_kListenerGrain),
                CWindowEventImmediatelyBroadcaster(event, pWindow, &m_immediatelyListenerList));
        });

        if (IsNotifiyChain)
        {
            CLambdaTask delegator([event, pWindow, this]()
            {
                Notify(event, pWindow);
            });
        }

    }


    std::unique_ptr<CKeyboardEventBroadcastingStation> SBroadcastingStation::pKeyboard;
    std::unique_ptr<CMouseEventBroadcastingStation> SBroadcastingStation::pMouse;
    std::unique_ptr<CWindowEventBroadcastingStation> SBroadcastingStation::pWindow;

}