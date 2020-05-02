#pragma once


#include "../CoreHelper.h"


namespace Platform
{
    class CPlatformSystem;

    class CPlatformSystemMode final
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(CPlatformSystemMode);
    private:
        friend CPlatformSystem;

    public:
        FLINE static bool IsThreadMode() noexcept
        {
            return s_isThreadMode;
        }

    private:
        static bool s_isThreadMode;
    };

    /**Platform Update Mode 상태에 따른 처리를 돕는 Macro.
반드시 CPlatformWorkerQueueSystem, CWaitAsync가 포함되어야 함*/
#define PLATFORM_EXECUTE_FUNC(_WAITER_, _WORKER_)   \
if (Platform::CPlatformSystemMode::IsThreadMode()){ \
    CPlatformWorkerQueueSystem::Push(&_WORKER_);    \
    _WAITER_.WaitAsync();              }            \
else{ _WORKER_.Execute(); }                         \

}