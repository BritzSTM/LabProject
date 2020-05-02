#pragma once


#include "Common/BaseTypes.h"
#include "Event/EventHelper.h"
#include "Container/ConcurrencyListenerList.h"

#include <type_traits>


//forward
namespace tbb
{
	class task_scheduler_init;
}


namespace Core
{
	namespace Thread
	{

		/**TaskSystem이 초기화 완료 되었음을 통지 받는 Listener*/
		struct ITaskSystemInitializedListener
		{
            DISALLOW_CREATE_INSTANCE(ITaskSystemInitializedListener);

        public:
			virtual ~ITaskSystemInitializedListener() {}

			virtual void OnInitializedTaskSystem(const int32& coreCount) = 0;
		};

		/**TaskSystem이 파괴 완료 되었음을 통지 받는 Listener*/
		struct ITaskSystemTerminatedListener
		{
            DISALLOW_CREATE_INSTANCE(ITaskSystemTerminatedListener);

        public:
			virtual ~ITaskSystemTerminatedListener() {}

			virtual void OnTerminatedTaskSystem() = 0;
		};

		/**
		@brief TaskSystem의 초기화를 담당하는 정적 Methods 집합 입니다.
		@date 2018-10-16  오전 01:38:33
		@version 1.0v
		*/
		class CTaskSystem final
		{
			DISALLOW_CREATE_INSTANCE_PERMANENTLY(CTaskSystem);

		public:
			static constexpr int32 kAutoThreadCount = -1;
			
		public:
			/**초기화를 진행합니다*/
			static void Initialize(const int32 threadCount = kAutoThreadCount) noexcept;
			/**시스템을 완전히 파괴 합니다.*/
			static void Terminate() noexcept;
			/**시스템의 활동 여부를 확인합니다.*/
			static bool IsActivated() noexcept;
			/**생성된 Thread수를 획득 합니다.*/
			static int32 GetThreadCount() noexcept;
			

			//ListenerMethods
			static bool AddOnInitializedListener(const ITaskSystemInitializedListener* pListener, const char* pszName);
			static void RemoveOnInitializedListener(const ITaskSystemInitializedListener* pListener);

			static bool AddOnTerminatedListener(const ITaskSystemTerminatedListener* pListener, const char* pszName);
			static void RemoveTerminatedListener(const ITaskSystemTerminatedListener* pListener);

		private:
			static tbb::task_scheduler_init& _GetTaskSchedulerInit();
			static Container::ConcurrencyListenerList<ITaskSystemInitializedListener>& _GetInitializedListenerList();
			static Container::ConcurrencyListenerList<ITaskSystemTerminatedListener>& _GetTerminatedListenerList();
			static int32 gs_threadCount;
		};

	}
}