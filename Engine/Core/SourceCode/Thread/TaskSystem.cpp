#include "stdafx.h"
#include "TaskSystem.h"


#include <tbb/task_scheduler_init.h>


namespace Core
{
	namespace Thread
	{
		int32 CTaskSystem::gs_threadCount = 0;

		tbb::task_scheduler_init& CTaskSystem::_GetTaskSchedulerInit()
		{
			//지연상태로 반환
			static tbb::task_scheduler_init taskInit(tbb::task_scheduler_init::deferred);
			return taskInit;
		}

		Container::ConcurrencyListenerList<ITaskSystemInitializedListener>&
		CTaskSystem::_GetInitializedListenerList()
		{
			static Container::ConcurrencyListenerList<ITaskSystemInitializedListener> list;
			return list;
		}

		Container::ConcurrencyListenerList<ITaskSystemTerminatedListener>&
		CTaskSystem::_GetTerminatedListenerList()
		{
			static Container::ConcurrencyListenerList<ITaskSystemTerminatedListener> list;
			return list;
		}

		void CTaskSystem::Initialize(const int32 threadCount) noexcept
		{
			auto& scheduler = _GetTaskSchedulerInit(); 
			scheduler.initialize(threadCount);

			//기본 코어 수
			if (threadCount == kAutoThreadCount)
			{
				gs_threadCount = scheduler.default_num_threads();
			}
			else
			{
				gs_threadCount = threadCount;
			}

			//Listener 통지
			auto& list = _GetInitializedListenerList();
			list.Notify([](const auto& tag)
			{
				tag.pListener->OnInitializedTaskSystem(gs_threadCount);
			});
		}

		void CTaskSystem::Terminate() noexcept
		{
			_GetTaskSchedulerInit().terminate();

			//Listener 통지
			auto& list = _GetTerminatedListenerList();
			list.Notify([](const auto& tag)
			{
				tag.pListener->OnTerminatedTaskSystem();
			});
		}

		bool CTaskSystem::IsActivated() noexcept
		{
			return _GetTaskSchedulerInit().is_active();
		}

		int32 CTaskSystem::GetThreadCount() noexcept
		{
			return gs_threadCount;
		}

		bool CTaskSystem::AddOnInitializedListener(const ITaskSystemInitializedListener* pListener, const char* pszName)
		{
			if (pListener == nullptr)
			{
				return false;
			}

			auto& list = _GetInitializedListenerList();
			return list.Add(pListener, pszName);
		}

		void CTaskSystem::RemoveOnInitializedListener(const ITaskSystemInitializedListener* pListener)
		{
			if (pListener == nullptr)
			{
				return;
			}

			auto& list = _GetInitializedListenerList();
			list.Remove(pListener);
		}

		bool CTaskSystem::AddOnTerminatedListener(const ITaskSystemTerminatedListener* pListener, const char* pszName)
		{
			if (pListener == nullptr)
			{
				return false;
			}

			auto& list = _GetTerminatedListenerList();
			return list.Add(pListener, pszName);
		}

		void CTaskSystem::RemoveTerminatedListener(const ITaskSystemTerminatedListener* pListener)
		{
			if (pListener == nullptr)
			{
				return;
			}

			auto& list = _GetTerminatedListenerList();
			list.Remove(pListener);
		}
	}
}