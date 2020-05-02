#pragma once


#include "../CoreHelper.h"
#include "../Common/Exception.h"
#include "../Type/TypeTraits.h"
#include "../Util/TextLiteral.h"
#include "../Util/TupleUtil.h"
#include "../Thread/Task.h"
#include "../Thread/ParallelFor.h"
#include "../Thread/BlockedRange.h"
#include "../Container/ConcurrencyListenerList.h"
#include "../Container/ConcurrencyQueue.h"

#include <cassert>
#include <atomic>
#include <xutility>


namespace Core
{

    //TDeferredBroadcastingStation의 Update가 완료되기전 다시 Update에 진입하는 경우 발생하는 예외
    DEFINE_EXCEPTION_TYPE
    (
        Core, Exception, MultipleUpdatesException, NO_DEFINE_DETAILS
    );


    /**Tuple Test */
    template<
        typename _BroadcasterType,
        template<typename _IN_EVENT_LISTENER_TYPE> class _ListenerListContainerType = Core::Container::ConcurrencyListenerList,
        template<typename _IN_EVENT_DATA_TYPE> class _EventQueueContainerType = Core::Container::concurrent_queue>
    class Tu final
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(Tu);

    public:
        using BroadcasterType = _BroadcasterType;
        using DataTupleType = typename BroadcasterType::DataTupleType;
        using ListenerType = typename BroadcasterType::ListenerType;
        
        using ListenerListType = _ListenerListContainerType<ListenerType>;
        using EventQueueType = _EventQueueContainerType<DataTupleType>;

    public:
        ~Tu() {}

        Tu(const size_t listenerGrainSize = s_kDefaultListenerGrain)
            : m_listenerGrainSize(listenerGrainSize)
            , m_updating(false)
        {
            //validation check
            _CheckGrainSize(m_listenerGrainSize);

        }

        ILINE size_t GetDeferredGrainSize() const
        {
            return  m_listenerGrainSize;

        }

        ILINE size_t SetDeferredGrainSize(const size_t grainSize)
        {
            _CheckGrainSize(grainSize);
            m_listenerGrainSize = grainSize;

        }

        ILINE bool AddListener(const ListenerType* pListener, const char* const pszName)
        {
            return m_listenerList.Add(pListener, pszName);

        }

        ILINE void RemoveListener(const ListenerType* pListener)
        {
            m_listenerList.Remove(pListener);

        }


#define _EnableIFDataTupleEmptyType_ \
        typename = std::enable_if_t<std::is_same<DataTupleType, stdext::empty_tuple>::value>

#define _EnableIFDataTupleNotEmptyType_ \
        typename = std::enable_if_t<!std::is_same<DataTupleType, stdext::empty_tuple>::value>

#define _EnableIFNotEqualDataTuple_ \
            typename = std::enable_if_t<\
            !std::is_same<DataTupleType, typename ArgType>::value>

        /**지연통지. 메시지 큐에 데이터를 저장합니다*/
        //template<_EnableIFDataTupleEmptyType_>
        void Notify()
        {
            m_eventQueue.push(stdext::empty_tuple());

        }
        
        /**지연통지. 메시지 큐에 데이터를 저장합니다*/
        //template<_EnableIFDataTupleNotEmptyType_>
        void Notify(const DataTupleType& data)
        {
            m_eventQueue.push(data);
            //static_assert(false, "ref");

        }
        
        /**지연통지. 메시지 큐에 데이터를 저장합니다*/
       /* void Notify(DataTupleType&& data)
        {
            m_eventQueue.emplace(std::forward<DataTupleType>(data));
            static_assert(false, "move");

        }*/

        ///**지연통지. 메시지 큐에 데이터를 저장합니다*/
        //template<typename ArgType, typename... ArgTypes, 
        //    _EnableIFDataTupleNotEmptyType_, _EnableIFNotEqualDataTuple_(ArgType)>
        //void Notify(const ArgType& arg, ArgTypes&&... args)
        //{
        //    static_assert(((sizeof...(ArgTypes) + 1) == s_kNotifyArgCount), 
        //        "notify func arg count not match");
        //    static_assert(false, "false1");
        //}

        /**지연통지. 메시지 큐에 데이터를 저장합니다*/
    /*    template<typename ArgType, typename... ArgTypes>
        auto Notify(ArgType&& arg, ArgTypes&&... args) -> _EnableIFDataTupleNotEmptyType_
        {
            static_assert(((sizeof...(ArgTypes) + 1) == s_kNotifyArgCount),
                "notify func arg count not match");
            static_assert(false, "false2");
        }*/

#undef _EnableIFDataTupleEmptyType_
#undef _EnableIFDataTupleNotEmptyType_
#undef _EnableIFNotEqualDataTuple_

        ILINE void ImmediatelyNotify(DataTupleType&& event)
        {
            //업데이트 여부를 따지지 않고 바로 통지를 시작한다. 긴급 메시지 성격을 가지기 때문
            //_NotifyParallely(event);

        }

        ILINE void ImmediatelyNotify(DataTupleType& event)
        {
            //업데이트 여부를 따지지 않고 바로 통지를 시작한다. 긴급 메시지 성격을 가지기 때문
            //_NotifyParallely(event);

        }

        /**isImmediate값이 참이라면 Msg대기열을 무시하고 바로 통지를 시작합니다.*/
        ILINE void ImmediatelyNotify(const DataTupleType& event)
        {
            //업데이트 여부를 따지지 않고 바로 통지를 시작한다. 긴급 메시지 성격을 가지기 때문
           // _NotifyParallely(event);

        }


        /**
        등록된 이벤트들을 순차적으로 통지합니다.
        내부적으로 Task로 처리됩니다.
        Thraed를 이용해 다중 업데이트 금지
        */
        void Update()
        {
            _CheckMulitUpdates();
            _EnterUpdate();
            DataTupleType popEvent;
            while (m_eventQueue.try_pop(popEvent))
            {
                _NotifyParallely(popEvent);
            }
            _EscapeUpdate();

        }

    private:
        FLINE void _CheckGrainSize(const size_t& size) const
        {
            if (size == 0)
            {
                THROWER(Core::GrainSizeException, EF_TMSG("GrainSize는 0이 될 수 없습니다."));
            }

        }

        FLINE void _CheckMulitUpdates() const
        {
            if (m_updating == true)
            {
                THROWER(MultipleUpdatesException, EF_TMSG("Update함수가 완료되기 전에 다시 Update함수가 호출 되었습니다."));
            }

        }

        //실제 업데이트가 이루어 지는 곳
        FLINE void _NotifyParallely(DataTupleType& event) //event의 const 여부 util에 추가
        {
            using namespace Core;
           
            parallel_for(
                blocked_range<size_t>(0, m_listenerList.Size(), m_listenerGrainSize),
                stdext::expand_left_tuple_to_create_obj<BroadcasterType>(
                    event, &m_listenerList));

        }

        FLINE void _EnterUpdate() noexcept
        {
            m_updating = true;

        }

        FLINE void _EscapeUpdate() noexcept
        {
            m_updating = false;

        }

    public:
        static constexpr int32 s_kDefaultListenerGrain = 64;
        static constexpr int32 s_kNotifyArgCount = std::tuple_size<DataTupleType>::value;

    private:
        size_t m_listenerGrainSize;
        EventQueueType m_eventQueue;
        ListenerListType m_listenerList;
        std::atomic_bool m_updating;

    };


     /**Task 기반의 단 하나의 데이터를 broadcast하기 위한 템플릿 클래스*/
    template<typename _EVENT_DATA_TYPE_, class _EVENT_LISTENER_TYPE_,
        class _EVENT_BROADCASTER_TYPE_,
        template<typename _IN__EVENT_LISTENER_TYPE_> class _EVENT_LISTENER_LIST_TYPE_ = Core::Container::ConcurrencyListenerList,
        template<typename _IN_EVENT_DATA_TYPE_> class _EVENT_QUEUE_TYPE_ = Core::Container::concurrent_queue>
    class TDeferredBroadcastingStation final
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(TDeferredBroadcastingStation);

    public:
        using EventDataType = _EVENT_DATA_TYPE_;
        using EventListenerType = _EVENT_LISTENER_TYPE_;
        using EventBroadcasterType = _EVENT_BROADCASTER_TYPE_;
        using EventListenerListType = _EVENT_LISTENER_LIST_TYPE_<EventListenerType>;
        using EventQueueType = _EVENT_QUEUE_TYPE_<EventDataType>;


    public:
        ~TDeferredBroadcastingStation() {}

        TDeferredBroadcastingStation(const size_t listenerGrainSize = s_kDefaultListenerGrain)
            : m_listenerGrainSize(listenerGrainSize)
            , m_updating(false)
        {
            //validation check
            _CheckGrainSize(m_listenerGrainSize);
        }

        ILINE size_t GetDeferredGrainSize() const
        {
            return  m_listenerGrainSize;
        }

        ILINE size_t SetDeferredGrainSize(const size_t grainSize)
        {
            _CheckGrainSize(grainSize);
            m_listenerGrainSize = grainSize;
        }

        ILINE bool AddListener(const EventListenerType* pListener, const char* const pszName)
        {
            return m_listenerList.Add(pListener, pszName);
        }

        ILINE void RemoveListener(const EventListenerType* pListener)
        {
            m_listenerList.Remove(pListener);
        }

        /**isImmediate값이 참이라면 Msg대기열을 무시하고 바로 통지를 시작합니다.*/
        ILINE void Notify(const EventDataType& event, const bool isImmediate = false)
        {
            if (isImmediate)
            {
                //업데이트 여부를 따지지 않고 바로 통지를 시작한다. 긴급 메시지 성격을 가지기 때문
                _NotifyParallely(event);
            }
            else
            {
                m_eventQueue.push(event);
            }
        }

        /**
        등록된 이벤트들을 순차적으로 통지합니다.
        내부적으로 Task로 처리됩니다.
        Thraed를 이용해 다중 업데이트 금지
        */
        void Update()
        {
            _CheckMulitUpdates();
            _EnterUpdate();
            EventDataType popEvent;
            while (m_eventQueue.try_pop(popEvent))
            {
                _NotifyParallely(popEvent);
            }
            _EscapeUpdate();
        }

    private:
        FLINE void _CheckGrainSize(const size_t& size) const
        {
            if (size == 0)
            {
                THROWER(Core::GrainSizeException, EF_TMSG("GrainSize는 0이 될 수 없습니다."));
            }
        }

        FLINE void _CheckMulitUpdates() const
        {
            if (m_updating == true)
            {
                THROWER(MultipleUpdatesException, EF_TMSG("Update함수가 완료되기 전에 다시 Update함수가 호출 되었습니다."));
            }
        }

        //실제 업데이트가 이루어 지는 곳
        FLINE void _NotifyParallely(const EventDataType& event)
        {
            using namespace Core;
            parallel_for(
                blocked_range<size_t>(0, m_listenerList.Size(), m_listenerGrainSize),
                EventBroadcasterType(event, &m_listenerList));
        }

        FLINE void _EnterUpdate() noexcept
        {
            m_updating = true;
        }

        FLINE void _EscapeUpdate() noexcept
        {
            m_updating = false;
        }

    public:
        static constexpr int32 s_kDefaultListenerGrain = 64;

    private:
        size_t m_listenerGrainSize;
        EventQueueType m_eventQueue;
        EventListenerListType m_listenerList;
        std::atomic_bool m_updating;

    };
    


    /**Task 기반의 단 하나의 데이터를 즉시 broadcast하기 위한 템플릿 클래스*/
    template<typename _EVENT_DATA_TYPE_, class _EVENT_LISTENER_TYPE_, class _EVENT_BROADCASTER_TYPE_,
        template<typename _IN__EVENT_LISTENER_TYPE_> class _EVENT_LISTENER_LIST_TYPE_ = Core::Container::ConcurrencyListenerList>
    class TImmediateBroadcastingStation final
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(TImmediateBroadcastingStation);

    public:
        using EventDataType = _EVENT_DATA_TYPE_;
        using EventListenerType = _EVENT_LISTENER_TYPE_;
        using EventBroadcasterType = _EVENT_BROADCASTER_TYPE_;
        using EventListenerListType = _EVENT_LISTENER_LIST_TYPE_<EventListenerType>;

    public:
        ~TImmediateBroadcastingStation() {}

        TImmediateBroadcastingStation(const size_t listenerGrainSize = s_kDefaultListenerGrain)
            : m_listenerGrainSize(listenerGrainSize)
        {
            //validation check
            _CheckGrainSize(m_listenerGrainSize);
        }

        ILINE size_t GetImmediateGrainSize() const
        {
            return  m_listenerGrainSize;
        }

        ILINE size_t SetImmediateGrainSize(const size_t grainSize)
        {
            _CheckGrainSize(grainSize);
            m_listenerGrainSize = grainSize;
        }

        ILINE bool AddImmediatelyListener(const EventListenerType* pListener, const char* const pszName)
        {
            return m_immediatelyListenerList.Add(pListener, pszName);
        }

        ILINE void RemoveImmediatelyListener(const EventListenerType* pListener)
        {
            m_immediatelyListenerList.Remove(pListener);
        }

        void NotifyImmediately(const EventDataType& event)
        {
            using namespace Core;
            parallel_for(
                blocked_range<size_t>(0, m_immediatelyListenerList.Size(), m_listenerGrainSize),
                EventBroadcasterType(event, &m_immediatelyListenerList));
        }

        /**Task에 위임되어 즉시 전파를 시작하며 비동기 함수 입니다. waiter는 nullptr전달이 가능합니다.*/
        ILINE void NotifyImmediatelyAsync(const EventDataType& event, Core::CAsyncWait* const pWaiter)
        {
            using namespace Core;

            if (pWaiter != nullptr)
            {
                CLambdaTask delegator(pWaiter, [event, this]()
                {
                    this->NotifyImmediately(event);
                });
            }
            else
            {
                CLambdaTask delegator([event, this]()
                {
                    this->NotifyImmediately(event);
                });
            }
        }

    public:
        static constexpr int32 s_kDefaultListenerGrain = 64;

    private:
        FLINE void _CheckGrainSize(const size_t& size) const
        {
            if (size == 0)
            {
                THROWER(Core::GrainSizeException, EF_TMSG("GrainSize는 0이 될 수 없습니다."));
            }
        }

    private:
        size_t m_listenerGrainSize;
        EventListenerListType  m_immediatelyListenerList;

    };


    //SubData를 포함한BroadcastingStation
    /**task기반의 data, subdata를 broadcast하기 위한 템플릿 클래스*/
    template<typename _EVENT_DATA_TYPE_, typename _SUB_DATA_TYPE_,
        class _EVENT_LISTENER_TYPE_, class _EVENT_BROADCASTER_TYPE_,
        template<typename _IN__EVENT_LISTENER_TYPE_> class _EVENT_LISTENER_LIST_TYPE_ = Core::Container::ConcurrencyListenerList,
        template<typename _IN_EVENT_DATA_TYPE_> class _EVENT_QUEUE_TYPE_ = Core::Container::concurrent_queue>
    class TDeferredPairBroadcastingStation final
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(TDeferredPairBroadcastingStation);

    public:
        using EventDataType = _EVENT_DATA_TYPE_;
        using SubDataType = _SUB_DATA_TYPE_;
        using EventDataPairType = std::pair<EventDataType, SubDataType>;

        using EventListenerType = _EVENT_LISTENER_TYPE_;
        using EventBroadcasterType = _EVENT_BROADCASTER_TYPE_;
        using EventListenerListType = _EVENT_LISTENER_LIST_TYPE_<EventListenerType>;
        using EventQueueType = _EVENT_QUEUE_TYPE_<EventDataPairType>;


    public:
        ~TDeferredPairBroadcastingStation() {}

        TDeferredPairBroadcastingStation(const size_t listenerGrainSize = s_kDefaultListenerGrain)
            : m_listenerGrainSize(listenerGrainSize)
            , m_updating(false)
        {
            //validation check
            _CheckGrainSize(m_listenerGrainSize);
        }

        ILINE size_t GetDeferredGrainSize() const
        {
            return m_listenerGrainSize;
        }

        ILINE size_t SetDeferredGrainSize(const size_t grainSize)
        {
            _CheckGrainSize(grainSize);
            m_listenerGrainSize = grainSize;
        }

        ILINE bool AddListener(const EventListenerType* pListener, const char* const pszName)
        {
            return m_listenerList.Add(pListener, pszName);
        }

        ILINE void RemoveListener(const EventListenerType* pListener)
        {
            m_listenerList.Remove(pListener);
        }

        /**isImmediate값이 참이라면 Msg대기열을 무시하고 바로 통지를 시작합니다.*/
        ILINE void Notify(const EventDataType& event, const SubDataType& subData, 
            const bool isImmediate = false)
        {
            if (isImmediate)
            {
                //업데이트 여부를 따지지 않고 바로 통지를 시작한다. 긴급 메시지 성격을 가지기 때문
                _NotifyParallely(event, subData);
            }
            else
            {
                m_eventQueue.push(std::make_pair(event,subData));
            }
        }

        /**
        등록된 이벤트들을 순차적으로 통지합니다.
        내부적으로 Task로 처리됩니다.
        Thraed를 이용해 다중 업데이트 금지
        */
        void Update()
        {
            _CheckMulitUpdates();
            _EnterUpdate();
            EventDataPairType popEventPair;
            while (m_eventQueue.try_pop(popEventPair))
            {
                _NotifyParallely(popEventPair.first, popEventPair.second);
            }
            _EscapeUpdate();
        }

    private:
        FLINE void _CheckGrainSize(const size_t& size) const
        {
            if (size == 0)
            {
                THROWER(Core::GrainSizeException, EF_TMSG("GrainSize는 0이 될 수 없습니다."));
            }
        }

        FLINE void _CheckMulitUpdates() const
        {
            if (m_updating == true)
            {
                THROWER(MultipleUpdatesException, EF_TMSG("Update함수가 완료되기 전에 다시 Update함수가 호출 되었습니다."));
            }
        }

        //실제 업데이트가 이루어 지는 곳
        FLINE void _NotifyParallely(const EventDataType& event, const SubDataType& subData)
        {
            using namespace Core;
            parallel_for(
                blocked_range<size_t>(0, m_listenerList.Size(), m_listenerGrainSize),
                EventBroadcasterType(event, subData, &m_listenerList));
        }

        FLINE void _EnterUpdate() noexcept
        {
            m_updating = true;
        }

        FLINE void _EscapeUpdate() noexcept
        {
            m_updating = false;
        }

    public:
        static constexpr int32 s_kDefaultListenerGrain = 64;

    private:
        size_t m_listenerGrainSize;
        EventQueueType m_eventQueue;
        EventListenerListType m_listenerList;
        std::atomic_bool m_updating;

    };


    /**task기반의 data, subdata를 즉시 broadcast하기 위한 템플릿 클래스*/
    template<typename _EVENT_DATA_TYPE_, typename _SUB_DATA_TYPE_,
        class _EVENT_LISTENER_TYPE_, class _EVENT_BROADCASTER_TYPE_,
        template<typename _IN__EVENT_LISTENER_TYPE_> class _EVENT_LISTENER_LIST_TYPE_ = Core::Container::ConcurrencyListenerList>
    class TImmediatePairBroadcastingStation final
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(TImmediatePairBroadcastingStation);

    public:
        using EventDataType = _EVENT_DATA_TYPE_;
        using SubDataType = _SUB_DATA_TYPE_;

        using EventListenerType = _EVENT_LISTENER_TYPE_;
        using EventBroadcasterType = _EVENT_BROADCASTER_TYPE_;
        using EventListenerListType = _EVENT_LISTENER_LIST_TYPE_<EventListenerType>;

    public:
        ~TImmediatePairBroadcastingStation() {}

        TImmediatePairBroadcastingStation(const size_t listenerGrainSize = s_kDefaultListenerGrain)
            : m_listenerGrainSize(listenerGrainSize)
        {
            //validation check
            _CheckGrainSize(m_listenerGrainSize);
        }

        ILINE size_t GetImmediateGrainSize() const
        {
            return  m_listenerGrainSize;
        }

        ILINE size_t SetImmediateGrainSize(const size_t grainSize)
        {
            _CheckGrainSize(grainSize);
            m_listenerGrainSize = grainSize;
        }

        ILINE bool AddImmediatelyListener(const EventListenerType* pListener, const char* const pszName)
        {
            return m_immediatelyListenerList.Add(pListener, pszName);
        }

        ILINE void RemoveImmediatelyListener(const EventListenerType* pListener)
        {
            m_immediatelyListenerList.Remove(pListener);
        }

        void NotifyImmediately(const EventDataType& event, const SubDataType& subData)
        {
            using namespace Core;
            parallel_for(
                blocked_range<size_t>(0, m_immediatelyListenerList.Size(), m_listenerGrainSize),
                EventBroadcasterType(event, subData, &m_immediatelyListenerList));
        }

        /**Task에 위임되어 즉시 전파를 시작하며 비동기 함수 입니다. waiter는 nullptr전달이 가능합니다.*/
        ILINE void NotifyImmediatelyAsync(const EventDataType& event, const SubDataType& subData,
            Core::CAsyncWait* const pWaiter)
        {
            using namespace Core;

            if (pWaiter != nullptr)
            {
                CLambdaTask delegator(pWaiter, [event, subData, this]()
                {
                    this->NotifyImmediately(event, subData);
                });
            }
            else
            {
                CLambdaTask delegator([event, subData, this]()
                {
                    this->NotifyImmediately(event, subData);
                });
            }
        }

    public:
        static constexpr int32 s_kDefaultListenerGrain = 64;

    private:
        FLINE void _CheckGrainSize(const size_t& size) const
        {
            if (size == 0)
            {
                THROWER(Core::GrainSizeException, EF_TMSG("GrainSize는 0이 될 수 없습니다."));
            }
        }

    private:
        size_t m_listenerGrainSize;
        EventListenerListType  m_immediatelyListenerList;

    };


    //지연, 즉시 타입의 방송 시스템 템플릿 구조체
    template <typename _DEFERRED_TYPE_, typename _IMMEDIATED_TYPE_>
    struct TMultiBroadcastingStation
    {
        _DEFERRED_TYPE_ Deferred;
        _IMMEDIATED_TYPE_ Immediate;


        //타입 유효성 확인
        /*/
        static_assert(stdext::is_base_of_template<
            TDeferredBroadcastingStation, _DEFERRED_TYPE_>::value || 
            stdext::is_base_of_template<
            TDeferredPairBroadcastingStation, _DEFERRED_TYPE_>::value,
            "_DEFERRED_TYPE_ is not base of TDeferredEventBroadcastingStation");

        static_assert(stdext::is_base_of_template<
            TImmediateBroadcastingStation, IMMEDIATED_TYPE_>::value ||
            stdext::is_base_of_template<
            TImmediatePairBroadcastingStation, IMMEDIATED_TYPE_>::value,
            "_IMMEDIATED_TYPE_ is not base of TImmediateEventBroadcastingStation");*/
    };

}