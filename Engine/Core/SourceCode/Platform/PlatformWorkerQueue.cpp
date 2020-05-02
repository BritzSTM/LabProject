#include "stdafx.h"
#include "PlatformWorkerQueue.h"

namespace Platform
{

    Core::Container::concurrent_queue<IPlatformWorker*> 
        CPlatformWorkerQueueSystem::s_workerQueue;

}