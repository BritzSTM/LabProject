#pragma once


#include "../CoreHelper.h"

#include <boost/interprocess/sync/named_semaphore.hpp>


namespace Core
{

    class CAsyncWait final
    {
        DISALLOW_COPY_COTR_AND_ASSIGN(CAsyncWait);

    public:
        ~CAsyncWait()
        {

        }

        CAsyncWait()
            : m_sema(1)
            , m_isComplete(false)
        {

        }

        ILINE bool IsComplete() const noexcept
        {
            return m_isComplete;
        }

        ILINE void Execute() noexcept
        {
            m_sema.wait();
        }

        ILINE void Complete() noexcept
        {
            m_sema.post();
            m_isComplete = true;
        }
        
        ILINE void WaitAsync() noexcept
        {
            m_sema.wait();
            m_sema.post();
        }     

    private:
        boost::interprocess::interprocess_semaphore m_sema;
        bool m_isComplete;
    };

}