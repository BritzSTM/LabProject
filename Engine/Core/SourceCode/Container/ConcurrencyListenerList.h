#pragma once


#include "../CoreHelper.h"
#include "../Type/TypeTraits.h"
#include "../Thread/CriticalSection.h"
#include "../Thread/Mutex.h"
#include "../Common/Exception.h"
#include "../Util/TextLiteral.h"

#include <list>
#include <vector>
#include <algorithm>


namespace Core
{
	namespace Container
	{
		/**
		리스너에 이름 태그를 달기위한 자료구조
		Listener pointer, Listener pszName 
		*/
		template <typename _LISTENER_TYPE>
		struct SListenerTag final
		{
			_LISTENER_TYPE* const pListener;
			const char* pszName;
		};

		/**
		@brief Listener add, remove, notify를 ThreadSafe하게 하기 위한 템플릿 클래스
		@remarks 
		템플릿 인자 _SYN_TYPE는 동기화를 위한 객체 선택가능(cs(기본), mutex)
		_CONTAINER인자는 내부에서 Listener를 관리 하는 타입
		통지 함수는 lambda를 사용해 통지
		*/
		template<
			typename _LISTENER_TYPE, 
			typename _SYN_TYPE = Core::CriticalSection, 
			template<typename _ELEM> class _CONTAINER = std::list>
		class ConcurrencyListenerList
		{
			//type alias
		public:
			using ListenerType  = _LISTENER_TYPE;
			using ListenerTagType = SListenerTag<ListenerType>;
			using ContainerType = _CONTAINER<ListenerTagType>;
			using SyncType      = _SYN_TYPE;

		private:
			using LockType = typename std::lock_guard<_SYN_TYPE>;

		public:
			ILINE ~ConcurrencyListenerList() {}

			ILINE ConcurrencyListenerList()
				: m_listenerSet()
				, m_isNotifying(false)
			{
				
			}
			
			/**전달된 Listener가 Set에 존재 유무*/
			ILINE bool IsContain(const ListenerType* pListener)
			{
				if (pListener == nullptr)
				{
					return false;
				}

				LockType lock(m_syncObject);
				const auto cendIter = std::cend(m_listenerSet);
				auto resIter = _Find(pListener);

				return !(resIter == cendIter);
			}

			/**현재 통지 중인지 확인 합니다*/
			ILINE bool IsNotifying() const noexcept
			{
				return m_isNotifying;
			}

			/**Listener의 수를 반환 합니다*/
			ILINE typename ContainerType::size_type Size() const noexcept
			{
				return m_listenerSet.size();
			}

			/**ListenerList를 초기화 합니다*/
			ILINE void Clear() noexcept
			{
				LockType lock(m_syncObject);
				m_listenerSet.clear();
			}

			/**새로운 Listener를 추가 합니다*/
			ILINE bool Add(const ListenerType* pListener, const char* pszName)
			{
				if (pListener == nullptr)
				{
					return false;
				}

				if (IsContain(pListener) == true)
				{
					return false;
				}

				LockType lock(m_syncObject);
				ListenerTagType tag{ const_cast<ListenerType* const>(pListener), pszName };
				m_listenerSet.push_back(tag);
				return true;
			}

			/**Listener를 제거 합니다*/
			ILINE void Remove(const ListenerType* pListener)
			{
				if (pListener == nullptr)
				{
					return;
				}

				LockType lock(m_syncObject);
				const auto cendIter = std::cend(m_listenerSet);
				const auto resIter = _Find(pListener);
				if (cendIter != resIter)
				{
					m_listenerSet.erase(resIter);
				}
			}

			/**
			모든 Listener에게 통지하는 함수
			Lambda식을 통해 확장가능합니다.
			단 Lambda 전달 인자는 항상 ListenerList의 const Tagtype& 입니다.
			*/
			template<typename _LAMBDA>
			ILINE void Notify(_LAMBDA func)
			{
				LockType lock(m_syncObject);
				m_isNotifying = true;
				for (const auto& tag : m_listenerSet)
				{
					func(tag);
				}
				m_isNotifying = false;
			}

            //std::advance 기반이므로 반복해서 호출하지 마시오.
            ILINE auto operator[](const size_t& index) const
            {
                _CheckOutOfRange(index);
                auto iter = std::begin(m_listenerSet);
                std::advance(iter, index);
                return iter;
            }

        private:
            FLINE void _CheckOutOfRange(const size_t& index) const
            {
                if ((index > (Size() - 1)) || (index < 0))
                {
                    THROWER(OutOfRangeException, EF_TMSG("ConcurrencyListenerList index Out Of Range"));
                }
            }

            FLINE auto _Find(const ListenerType* pListener) const
            {
                auto resIter = std::find_if(
                    std::cbegin(m_listenerSet),
                    std::cend(m_listenerSet),
                    [&pListener](const auto& tag)
                {
                    return tag.pListener == pListener;
                });

                return resIter;
            }

		private:
			ContainerType m_listenerSet;
			SyncType      m_syncObject;
			bool          m_isNotifying;

		};

/**Listener변수의 이름을 넣으면 TagType이 반환 됩니다*/
#define GetTagType(LISTENER_LIST_VALUE_NAME) decltype(LISTENER_LIST_VALUE_NAME)::ListenerTagType
	}
}