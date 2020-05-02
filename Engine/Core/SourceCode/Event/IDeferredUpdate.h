#pragma once


#include "../CoreHelper.h"


namespace Core
{

    //지연된 업데이트를 사용하는 객체들을 위한 인터페이스
    struct IDeferredUpdate
    {
        DISALLOW_CREATE_INSTANCE(IDeferredUpdate);

    public:
        virtual ~IDeferredUpdate() {}

        //업데이트를 진행합니다
        virtual void Update() = 0;

    };

}