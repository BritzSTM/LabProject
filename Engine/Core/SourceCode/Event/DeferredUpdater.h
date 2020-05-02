#pragma once


#include "IDeferredUpdate.h"
#include "DBroadcaster.h"
#include "../Container/ConcurrencyListenerList.h"


namespace Core
{
    using lt = Container::ConcurrencyListenerList<IDeferredUpdate>;

    DEFINE_BROADCASTER
    (
        ddd, Update, DEFINE_NO_ARG, lt
    );

    template<typename ...TYPES_>
    class TPACK
    {

    };


    /**Task 기반의 단 하나의 데이터를 broadcast하기 위한 템플릿 클래스*/
    template<
        typename _EVENT_DATA_TYPES_,
        typename _EVENT_LISTENER_TYPE_>
        class TTEST final
    {

    };


    using TestPack = TPACK<double, int, int, int>;

    void d()
    {
        TTEST<TestPack, IDeferredUpdate> t;
        //valid(1.0, 1, 2, 3);
    }

    //업데이트 신호룰 주는 클래스
    class CDeferredUpdater 
        : public IDeferredUpdate
    {
    public:
        virtual void Update();



    private:
        Container::ConcurrencyListenerList<IDeferredUpdate> m_main;
        Container::ConcurrencyListenerList<CDeferredUpdater*> m_subUpdater;

    };

}