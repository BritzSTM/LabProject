#pragma once


#include "../Common/Exception.h"
#include "../Util/TextLiteral.h"
#include "AsyncWait.h"

#include <tbb/task.h>


namespace Core
{
    
    //tbb impl

    using CTask = tbb::task;
    using CEmptyTask = tbb::empty_task;
    using CTaskList = tbb::task_list;

    
    namespace _internal
    {

        //메모리를 절약하기 위해 타입을 나눔 _CLambdaTaskCore, _CLambdaTaskCoreForWaiter

        template<typename _LAMBDA_FUNC_>
        class _CLambdaTaskCore
            : public CTask
        {
        public:
            virtual ~_CLambdaTaskCore() {}

            _CLambdaTaskCore(_LAMBDA_FUNC_&& lambda)
                : m_fn(std::move(lambda))
            {

            }

            virtual CTask* execute() override
            {
                m_fn();
                return nullptr;
            }

        private:
            _LAMBDA_FUNC_ m_fn;

        };

        //waiter는 nullptr을 불허합니다. 사용시 확인
        template<typename _LAMBDA_FUNC_>
        class _CLambdaTaskCoreForWaiter
            : public CTask
        {
        public:
            virtual ~_CLambdaTaskCoreForWaiter() {}

            _CLambdaTaskCoreForWaiter(CAsyncWait* const pWaiter, _LAMBDA_FUNC_&& lambda)
                : m_pWaiter(pWaiter)
                , m_fn(std::move(lambda))     
            {
                m_pWaiter->Execute();
            }

            virtual CTask* execute() override
            {
                m_fn();
                m_pWaiter->Complete();
                return nullptr;
            }

        private:
            _LAMBDA_FUNC_ m_fn;
            CAsyncWait* const m_pWaiter;
        };
    }


    /**
    람다 함수를 task에 위임시켜 즉시 실행합니다.
    비동기적 실행입니다.
    람다 함수가 참조하는 객체들에 대한 생명주기를 주의할 것.
    */
    template<typename _LAMBDA_FUNC_>
    class CLambdaTask
    {
    public:
        virtual ~CLambdaTask() {}

        CLambdaTask(_LAMBDA_FUNC_&& lambda)
        {
            _internal::_CLambdaTaskCore< _LAMBDA_FUNC_> *pCore =
                new(CTask::allocate_root())_internal::_CLambdaTaskCore<_LAMBDA_FUNC_>(std::move(lambda));
           
            CTask::enqueue(*pCore);
        }

        CLambdaTask(CAsyncWait* const pWaiter, _LAMBDA_FUNC_&& lambda)
        {
            _CheckWaiter(pWaiter);

            _internal::_CLambdaTaskCoreForWaiter< _LAMBDA_FUNC_> *pCore =
                new(CTask::allocate_root())
                _internal::_CLambdaTaskCoreForWaiter<_LAMBDA_FUNC_>(pWaiter, std::move(lambda));

            CTask::enqueue(*pCore);
        }
    private:
        ILINE void _CheckWaiter(CAsyncWait* const pWaiter) const
        {
            if (pWaiter == nullptr)
            {
                THROWER(InvalidArgumentNullptrException, EF_TMSG("CAsyncWait가 nullptr 입니다."));
            }
        }

    };

    /**
    람다 함수가 참조하는 객체들에 대한 생명주기를 주의할 것.
    */
    template<typename _LAMBDA_FUNC_>
    class CLambdaSyncTask
        : public CTask
    {
    public:
        virtual ~CLambdaSyncTask() {}

        CLambdaSyncTask(_LAMBDA_FUNC_&& lambda)
            : m_fn(std::move(lambda))
        {

        }

        virtual CTask* execute() override
        {
            m_fn();
            return nullptr;
        }

    private:
        _LAMBDA_FUNC_ m_fn;

    };

}