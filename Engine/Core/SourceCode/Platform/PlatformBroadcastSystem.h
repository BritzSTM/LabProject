/**
공통적인 코드가 있으나 템플릿 기반으로 확장하지 않은 이유는
추후 개별로 MsgType마다 확장할 수 있기 때문.
인라인 함수의 일반함수화도 고려해봐야함.
*/
#pragma once

#include "../CoreHelper.h"
#include "../Common/EnumExtension.h"
#include "../Thread/Task.h"
#include "../Thread/ParallelFor.h"
#include "../Thread/BlockedRange.h"
#include "../Container/ConcurrencyListenerList.h"
#include "../Container/ConcurrencyQueue.h"
#include "Input/KeyboardEventTypes.h"
#include "Input/MouseEventTypes.h"
#include "Window/WindowEventTypes.h"

#include <xutility>


namespace Platform
{

    DEFINE_EXT_ENUM
    (
        enum, EPlatformMsgType, ENUM_SIZE_DEFAULT,
        E_NAME(Keyboard)
        E_NAME(Mouse)
        E_NAME(Window)
    );


    //Mouse

    using CMouseEventListenerList =
        Core::Container::ConcurrencyListenerList<IMouseEventListener>;

    using CMouseEventImmediatelyListenerList =
        Core::Container::ConcurrencyListenerList<IMouseEventImmediatelyListener>;


    class CMouseEventBroadcaster final
    {
    public:
        ~CMouseEventBroadcaster() {}

        CMouseEventBroadcaster(
            const SMouseEvents& eventData, const CMouseEventListenerList* const pList)
            : m_eventData(eventData)
            , m_pListenerList(pList)
        {

        }

        void operator()(const Core::blocked_range<size_t>& range) const
        {
            auto iter = (*m_pListenerList)[range.begin()];
            for (auto i = range.begin(); i != range.end(); ++i, ++iter)
            {             
                (*iter).pListener->OnMouseEvent(m_eventData);
            }
        }

    private:
        const CMouseEventListenerList* const m_pListenerList;
        const SMouseEvents m_eventData;

    };

    class CMouseEventImmediatelyBroadcaster final
    {
    public:
        ~CMouseEventImmediatelyBroadcaster() {}

        CMouseEventImmediatelyBroadcaster(
            const SMouseEvents& eventData, const CMouseEventImmediatelyListenerList* const pList)
            : m_eventData(eventData)
            , m_pListenerList(pList)
        {

        }

        void operator()(const Core::blocked_range<size_t>& range) const
        {
            auto iter = (*m_pListenerList)[range.begin()];

            for (auto i = range.begin(); i != range.end(); ++i, ++iter)
            {
                (*iter).pListener->OnMouseEventImmediately(m_eventData);
            }
        }

    private:
        const CMouseEventImmediatelyListenerList* const m_pListenerList;
        const SMouseEvents m_eventData;

    };


    class CMouseEventBroadcastingStation
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(CMouseEventBroadcastingStation);

    public:
        ~CMouseEventBroadcastingStation() {}
        CMouseEventBroadcastingStation() {}

        ILINE bool AddListener(const IMouseEventListener* pListener, const char* const pszName)
        {
            return m_listenerList.Add(pListener, pszName);
        }

        ILINE void RemoveListener(const IMouseEventListener* pListener)
        {
            m_listenerList.Remove(pListener);
        }

        ILINE bool AddImmediatelyListener(const IMouseEventImmediatelyListener* pListener, const char* const pszName)
        {
            m_immediatelyListenerList.Add(pListener, pszName);
        }

        ILINE void RemoveImmediatelyListener(const IMouseEventImmediatelyListener* pListener)
        {
            m_immediatelyListenerList.Remove(pListener);
        }

        /**isImmediate값이 참이라면 Msg대기열을 무시하고 바로 통지를 시작합니다.*/
        ILINE void Notify(const SMouseEvents& event, const bool isImmediate = false)
        {
            if (isImmediate)
            {
                _NotifyParallely(event);
            }
            else
            {
                m_eventQueue.push(event);
            }
        }

        /**Not ThreadSafe. 다중 업데이트 금지*/
        void Update();

        /**즉시 전파를 시작하며 IsNotifiyChain의 값이 참인경우 하위 항목에게도 즉시 통지를 합니다.*/
        void NotifiyImmediately(const SMouseEvents& event, bool IsNotifiyChain = false);

    private:
        FLINE void _NotifyParallely(const SMouseEvents& event) const
        {
            using namespace Core;

            parallel_for(
                blocked_range<size_t>(0, m_listenerList.Size(), s_kListenerGrain),
                CMouseEventBroadcaster(event, &m_listenerList));
        }

    public:
        //이벤트의 순서를 보장해야 하므로 Listener의 뭉치를 가지고 Task를 전달.
        static constexpr int32 s_kListenerGrain = 15;

    private:
        Core::Container::concurrent_queue<SMouseEvents> m_eventQueue;
        CMouseEventListenerList m_listenerList;
        CMouseEventImmediatelyListenerList m_immediatelyListenerList;

    };


    //Keyboard

    using CKeyboardEventListenerList =
        Core::Container::ConcurrencyListenerList<IKeyboardEventListener>;

    using CKeyboardEventImmediatelyListenerList =
        Core::Container::ConcurrencyListenerList<IKeyboardEventImmediatelyListener>;

    class CKeyboardEventBroadcaster final
    {
    public:
        ~CKeyboardEventBroadcaster() {}

        CKeyboardEventBroadcaster(
            const SKeyboardEvents& eventData, const CKeyboardEventListenerList* const pList)
            : m_eventData(eventData)
            , m_pListenerList(pList)
        {

        }

        void operator()(const Core::blocked_range<size_t>& range) const
        {
            auto iter = (*m_pListenerList)[range.begin()];
            for (auto i = range.begin(); i != range.end(); ++i, ++iter)
            {
                (*iter).pListener->OnKeyboardEvent(m_eventData);
            }
        }

    private:
        const CKeyboardEventListenerList* const m_pListenerList;
        const SKeyboardEvents m_eventData;

    };

    class CKeyboardEventImmediatelyBroadcaster final
    {
    public:
        ~CKeyboardEventImmediatelyBroadcaster() {}

        CKeyboardEventImmediatelyBroadcaster(
            const SKeyboardEvents& eventData, const CKeyboardEventImmediatelyListenerList* const pList)
            : m_eventData(eventData)
            , m_pListenerList(pList)
        {

        }

        void operator()(const Core::blocked_range<size_t>& range) const
        {
            auto iter = (*m_pListenerList)[range.begin()];

            for (auto i = range.begin(); i != range.end(); ++i, ++iter)
            {
                (*iter).pListener->OnKeyboardEventImmediately(m_eventData);
            }
        }

    private:
        const CKeyboardEventImmediatelyListenerList* const m_pListenerList;
        const SKeyboardEvents m_eventData;

    };

    class CKeyboardEventBroadcastingStation
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(CKeyboardEventBroadcastingStation);

    public:
        ~CKeyboardEventBroadcastingStation() {}
        CKeyboardEventBroadcastingStation() {}

        ILINE bool AddListener(const IKeyboardEventListener* pListener, const char* const pszName)
        {
            return m_listenerList.Add(pListener, pszName);
        }

        ILINE void RemoveListener(const IKeyboardEventListener* pListener)
        {
            m_listenerList.Remove(pListener);
        }

        ILINE bool AddImmediatelyListener(const IKeyboardEventImmediatelyListener* pListener, const char* const pszName)
        {
            m_immediatelyListenerList.Add(pListener, pszName);
        }

        ILINE void RemoveImmediatelyListener(const IKeyboardEventImmediatelyListener* pListener)
        {
            m_immediatelyListenerList.Remove(pListener);
        }

        /**isImmediate값이 참이라면 Msg대기열을 무시하고 바로 통지를 시작합니다.*/
        ILINE void Notify(const SKeyboardEvents& event, const bool isImmediate = false)
        {
            if (isImmediate)
            {
                _NotifyParallely(event);
            }
            else
            {
                m_eventQueue.push(event);
            }
        }

        /**Not ThreadSafe. 다중 업데이트 금지*/
        void Update();

        /**즉시 전파를 시작하며 IsNotifiyChain의 값이 참인경우 하위 항목에게도 즉시 통지를 합니다.*/
        void NotifiyImmediately(const SKeyboardEvents& event, bool IsNotifiyChain = false);

    private:
        FLINE void _NotifyParallely(const SKeyboardEvents& event) const
        {
            using namespace Core;

            parallel_for(
                blocked_range<size_t>(0, m_listenerList.Size(), s_kListenerGrain),
                CKeyboardEventBroadcaster(event, &m_listenerList));
        }

    public:
        //이벤트의 순서를 보장해야 하므로 Listener의 뭉치를 가지고 Task를 전달.
        static constexpr int32 s_kListenerGrain = 15;

    private:
        Core::Container::concurrent_queue<SKeyboardEvents> m_eventQueue;
        CKeyboardEventListenerList m_listenerList;
        CKeyboardEventImmediatelyListenerList m_immediatelyListenerList;

    };


    //Window

    using CWindowEventListenerList =
        Core::Container::ConcurrencyListenerList<IWindowEventListener>;

    using CWindowEventImmediatelyListenerList =
        Core::Container::ConcurrencyListenerList<IWindowEventImmediatelyListener>;

    class CWindowEventBroadcaster final
    {
    public:
        ~CWindowEventBroadcaster() {}

        CWindowEventBroadcaster(
            const SWindowEvents& eventData, IWindow* const pWindow, const CWindowEventListenerList* const pList)
            : m_eventData(eventData)
            , m_pWindow(pWindow)
            , m_pListenerList(pList)
        {

        }

        void operator()(const Core::blocked_range<size_t>& range) const
        {
            auto iter = (*m_pListenerList)[range.begin()];
            for (auto i = range.begin(); i != range.end(); ++i, ++iter)
            {
                (*iter).pListener->OnWindowEvent(m_eventData, m_pWindow);
            }
        }

    private:
        const CWindowEventListenerList* const m_pListenerList;
        IWindow* const m_pWindow;
        const SWindowEvents m_eventData;

    };

    class CWindowEventImmediatelyBroadcaster final
    {
    public:
        ~CWindowEventImmediatelyBroadcaster() {}

        CWindowEventImmediatelyBroadcaster(
            const SWindowEvents& eventData, IWindow* const pWindow, const CWindowEventImmediatelyListenerList* const pList)
            : m_eventData(eventData)
            , m_pWindow(pWindow)
            , m_pListenerList(pList)
        {

        }

        void operator()(const Core::blocked_range<size_t>& range) const
        {
            auto iter = (*m_pListenerList)[range.begin()];

            for (auto i = range.begin(); i != range.end(); ++i, ++iter)
            {
                (*iter).pListener->OnWindowEventImmediately(m_eventData, m_pWindow);
            }
        }

    private:
        const CWindowEventImmediatelyListenerList* const m_pListenerList;
        IWindow* const m_pWindow;
        const SWindowEvents m_eventData;

    };

    class CWindowEventBroadcastingStation
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(CWindowEventBroadcastingStation);
    
    public:
        using SWindowEventsPair = std::pair<SWindowEvents, IWindow*>;

    public:
        ~CWindowEventBroadcastingStation() {}
        CWindowEventBroadcastingStation() {}

        ILINE bool AddListener(const IWindowEventListener* pListener, const char* const pszName)
        {
            return m_listenerList.Add(pListener, pszName);
        }

        ILINE void RemoveListener(const IWindowEventListener* pListener)
        {
            m_listenerList.Remove(pListener);
        }

        ILINE bool AddImmediatelyListener(const IWindowEventImmediatelyListener* pListener, const char* const pszName)
        {
            m_immediatelyListenerList.Add(pListener, pszName);
        }

        ILINE void RemoveImmediatelyListener(const IWindowEventImmediatelyListener* pListener)
        {
            m_immediatelyListenerList.Remove(pListener);
        }

        /**isImmediate값이 참이라면 Msg대기열을 무시하고 바로 통지를 시작합니다.*/
        ILINE void Notify(const SWindowEvents& event, IWindow* const pWindow, const bool isImmediate = false)
        {
            if (isImmediate)
            {
                _NotifyParallely(event, pWindow);
            }
            else
            {
                m_eventQueue.push(std::make_pair(event, pWindow));
            }
        }

        /**Not ThreadSafe. 다중 업데이트 금지*/
        void Update();

        /**즉시 전파를 시작하며 IsNotifiyChain의 값이 참인경우 하위 항목에게도 즉시 통지를 합니다.*/
        void NotifiyImmediately(const SWindowEvents& event, IWindow* const pWindow, bool IsNotifiyChain = false);

    private:
        FLINE void _NotifyParallely(const SWindowEvents& event, IWindow* const pWindow) const
        {
            using namespace Core;

            parallel_for(
                blocked_range<size_t>(0, m_listenerList.Size(), s_kListenerGrain),
                CWindowEventBroadcaster(event, pWindow, &m_listenerList));
        }

    public:
        //이벤트의 순서를 보장해야 하므로 Listener의 뭉치를 가지고 Task를 전달.
        static constexpr int32 s_kListenerGrain = 15;

    private:
        Core::Container::concurrent_queue<SWindowEventsPair> m_eventQueue;
        CWindowEventListenerList m_listenerList;
        CWindowEventImmediatelyListenerList m_immediatelyListenerList;

    };


    struct SBroadcastingStation
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(SBroadcastingStation);

    public:
        static std::unique_ptr<CKeyboardEventBroadcastingStation> pKeyboard;
        static std::unique_ptr<CMouseEventBroadcastingStation> pMouse;
        static std::unique_ptr<CWindowEventBroadcastingStation> pWindow;

    };




    //immediately, normal,
    class CPlatformBroadcastSystem final
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(CPlatformBroadcastSystem);
        
    public:
        static bool InitSystem()
        {
            SBroadcastingStation::pKeyboard = std::make_unique<CKeyboardEventBroadcastingStation>();
            SBroadcastingStation::pMouse = std::make_unique<CMouseEventBroadcastingStation>();
            SBroadcastingStation::pWindow = std::make_unique<CWindowEventBroadcastingStation>();
            return true;
        }

        static void ReleaseSystem()
        {
            
        }
        
    };


}