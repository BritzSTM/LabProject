#pragma once


#include "../CoreHelper.h"
#include "Thread/Thread.h"


namespace Platform
{

    class CPlatformSystem final
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(CPlatformSystem);

    public:
        /**System Update를 독립적 Thread로 할지 결정할 수 있습니다.*/
        static bool InitSystem(bool useThread = true);
        static void ReleaseSystem();

        static bool IsThreadUpdate() noexcept;

        /**System이 ThreadMode가 아닐 때 호출 하십시오.*/
        static void Update();
               
    private:
        static void _UpdateWithThread();
        static void _ProceedWorker();
        //아직 세분화하지 못한 작업들을 진행한다.
        static void _ProceedMisc();

    private:
        static bool s_IsRelease;
        static Core::CThread* s_pUpdateThread;
    };

}