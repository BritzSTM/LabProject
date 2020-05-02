#pragma once


#include "Thread/AsyncWait.h"


namespace Platform
{

    struct IPlatformWorker
    {
        virtual void Execute() = 0;
        virtual void ToComplete() noexcept = 0;
        virtual bool IsAsync() noexcept = 0;
    };


    template<typename _LAMBDA_>
    class CPlatformWorker
        : public IPlatformWorker
    {
    public:
        virtual ~CPlatformWorker() {}

        CPlatformWorker(Core::CAsyncWait* const pWaiter, _LAMBDA_ && fn)
            : m_pWaiter(pWaiter)
            , m_fn(fn)
        {
            m_pWaiter->Execute();
        }

        virtual void Execute() override
        {
            m_fn();
        }

        virtual void ToComplete() noexcept override
        {
            if (m_pWaiter != nullptr)
            {
                m_pWaiter->ToComplete();
            }
        }

        virtual bool IsAsync() noexcept override
        {
            return (m_pWaiter != nullptr);
        }

    private:        
        Core::CAsyncWait* const m_pWaiter;
        _LAMBDA_ m_fn;      

    };

}