/**
@file RingLogger.h
@brief 링큐를 사용하여 일정 량만 기록을 보유하는 Logger
@remarks
@date 2018-08-24  오후 13:53:28
*/


#pragma once


#include "ILogger.h"
#include "Container/RingQueue.h"
#include "Container/ConcurrencyListenerList.h"


#include <list>
#include <algorithm>


namespace Core
{
	namespace Log
	{

		/**
		@brief
		고정형 RingQueue를 사용하는 ThreadSafe Logger
		@date 2018-10-14  오전 00:49:49
		@version 1.01v
		*/
		template<typename _RecodeType, size_t _Capacity>
		class RingLogger
			: public ILogger
		{
		public:
			virtual ~RingLogger()
			{

			}

			RingLogger()
				: m_ringData()
				, m_onFullLoggerListenerList()
				, m_onNewRecodeListenerList()
				, m_onFullLoggerHandler(nullptr)
				, m_onNewRecodeHandler(nullptr)
			{

			}


			//새로운 레코드 삽입
			virtual void InsertRecode(const char* pszMsg) override
			{
				lock_guard lock(m_mutex);
				m_ringData.enqueue(_RecodeType(pszMsg));

				_HandlingOnNewRecode();
				_HandlingOnFullLogger();
			}

			//결과 리포트
			virtual std::vector<string> GetReoprt()
			{
				lock_guard lock(m_mutex);
				std::vector<string> resReport(m_ringData.size());

				auto cIter = std::cbegin(m_ringData);
				for (auto i = 0; i < m_ringData.size(); ++i)
				{
					const auto& recode = *cIter;
					resReport.push_back(recode.ToString());
					++cIter;
				}
				return std::move(resReport);
			}

			//덮어 쓰기 유무. Not ThreadSafe Func
			virtual void SetOverWrite(bool b) noexcept
			{
				m_ringData.SetOverWriting(b);
			}

			virtual void AddOnNewRecodeListener(const Core::ILoggerEventOnNewRecodeListener *pListener, const char *pszName)
			{
				m_onNewRecodeListenerList.Add(pListener, pszName);
			}
			virtual void RemoveOnNewRecodeListener(const Core::ILoggerEventOnNewRecodeListener *pListener)
			{
				m_onNewRecodeListenerList.Remove(pListener);
			}
			virtual void AddOnFullLoggerListener(const Core::ILoggerEventOnFullLoggerListener *pListener, const char *pszName)
			{
				m_onFullLoggerListenerList.Add(pListener, pszName);
			}
			virtual void RemoveOnFullLoggerListener(const Core::ILoggerEventOnFullLoggerListener *pListener)
			{
				m_onFullLoggerListenerList.Remove(pListener);
			}

			//RegisterHandler Not ThreadSafe Func
			virtual void RegisterFullLoggerHandler(const OnFullLoggerHandler& handler) override
			{
				m_onFullLoggerHandler = handler;
			}
			//RegisterHandler Not ThreadSafe Func
			virtual void RegisterOnNewRecodeHandler(const OnNewRecodeHandler& handler) override
			{
				m_onNewRecodeHandler = handler;
			}


		private:
			//새로운 Recode가 삽입되고 나서 처리할 항목들
			FLINE void _HandlingOnNewRecode()
			{
				//New Recode
				if (m_onNewRecodeHandler != nullptr)
				{
					m_onNewRecodeHandler(this);
				}

				//New Recode Listener
				m_onNewRecodeListenerList.Notify([this](const auto& tag)
				{
					tag.pListener->OnNewRecode(&m_ringData.rear());
				});
			}

			//새로운 Recode가 삽입되고 처리될 FullState
			FLINE void _HandlingOnFullLogger()
			{
				//Full State
				if (m_ringData.IsFull() == true)
				{
					if (m_onFullLoggerHandler != nullptr)
					{
						m_onFullLoggerHandler(this);
					}

					//FullState Listener
					m_onFullLoggerListenerList.Notify([this](const auto& tag)
					{
						tag.pListener->OnFullLogger(this);
					});
				}
			}

			//더이상 사용되지 않음. Listsner 제거를 위한 템플릿 함수
			template<typename _LISTENER_TYPE, typename LISTENER_LIST_TYPE>
			[[deprecated]]FLINE void _RemoveHandling(const _LISTENER_TYPE* pListener, LISTENER_LIST_TYPE& listenerList)
			{
				lock_guard lock(m_mutex);

				listenerList.remove_if(
					[&pListener](const LISTENER_LIST_TYPE::value_type& tag)
				{
					return tag.pListener == pListener;
				});
			}

		private:
			mutex m_mutex;

			//Data
			Container::RingQueue <_RecodeType, _Capacity> m_ringData;

			//Listener
			Core::Container::ConcurrencyListenerList<ILoggerEventOnFullLoggerListener> m_onFullLoggerListenerList;
			Core::Container::ConcurrencyListenerList<ILoggerEventOnNewRecodeListener> m_onNewRecodeListenerList;

			//Handler
			OnFullLoggerHandler  m_onFullLoggerHandler;
			OnNewRecodeHandler   m_onNewRecodeHandler;
		};
	}
}