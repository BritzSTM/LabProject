#pragma once


#include "../CoreHelper.h"
#include "Container/ConcurrencyQueue.h"


namespace Platform
{

    struct IPlatformWorker;

   
    class CPlatformWorkerQueueSystem final
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(CPlatformWorkerQueueSystem);

    public:
        //ThreadSafe
        FLINE static void Push(const IPlatformWorker* const pWorker) noexcept
        {
            s_workerQueue.push(const_cast<IPlatformWorker*>(pWorker));
        }

        //ThreadSafe. but is not async;
        [[nodiscard]] static FLINE IPlatformWorker* const Pop() noexcept
        {
            IPlatformWorker* pRes;
            while (!s_workerQueue.try_pop(pRes));
            return pRes;
        }

        //ThreadSafe. È¹µæ ½ÇÆÐ½Ã nullptr ¹ÝÈ¯
        [[nodiscard]] static FLINE IPlatformWorker* const PopAsync() noexcept
        {
            IPlatformWorker* pRes;

            if (s_workerQueue.try_pop(pRes))
            {
                return pRes;
            }

            return nullptr;
        }
    private:
        static Core::Container::concurrent_queue<IPlatformWorker*> s_workerQueue;
    };

}