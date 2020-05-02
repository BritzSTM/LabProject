#include "stdafx.h"
#include "PlatformSystem.h"
#include "PlatformSystemMode.h"
#include "PlatformWorker.h"
#include "PlatformWorkerQueue.h"
#include "PlatformMsgPump.h"


namespace Platform
{
    
    bool CPlatformSystem::s_IsRelease = false;
    Core::CThread* CPlatformSystem::s_pUpdateThread = nullptr;

    bool CPlatformSystem::InitSystem(bool useThread)
    {
        s_IsRelease = false;

        if (useThread)
        {
            CPlatformSystemMode::s_isThreadMode = true;
            s_pUpdateThread = new Core::CThread(_UpdateWithThread);
        }
        else
        {
            CPlatformSystemMode::s_isThreadMode = false;
        }

        CPlatformBroadcastSystem::InitSystem();

        return true;
    }

    void CPlatformSystem::ReleaseSystem()
    {
        s_IsRelease = true;
        
        if (s_pUpdateThread != nullptr)
        {
            s_pUpdateThread->join();
        }

        CPlatformBroadcastSystem::ReleaseSystem();

    }

    bool CPlatformSystem::IsThreadUpdate() noexcept
    {
        return CPlatformSystemMode::IsThreadMode();
    }

    void CPlatformSystem::Update()
    {
        PumpPlatformMessage();
        _ProceedWorker();
        _ProceedMisc();
    }

    void CPlatformSystem::_UpdateWithThread()
    {
        while (!s_IsRelease)
        {
            PumpPlatformMessage();
            _ProceedWorker();
            _ProceedMisc();
        }
    }

    void CPlatformSystem::_ProceedWorker()
    {
        IPlatformWorker *pWorker = nullptr;

        while ((pWorker = CPlatformWorkerQueueSystem::PopAsync()) != nullptr)
        {
            pWorker->Execute();
            pWorker->ToComplete();
        }
    }

    void CPlatformSystem::_ProceedMisc()
    {
        //something else
        SBroadcastingStation::pKeyboard->Update();
        SBroadcastingStation::pMouse->Update();
        SBroadcastingStation::pWindow->Update();
    }

}