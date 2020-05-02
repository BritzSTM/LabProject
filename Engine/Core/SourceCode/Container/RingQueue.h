/**
@file RingQueue.h
@brief 고정 배열을 가진 Template RingQueue 정의
@date 2018-08-19  오후 17:20:34
@bug Iterator 생성방식에 문제 있음
*/

#pragma once

#include "CoreHelper.h"
#include "Common\BaseTypes.h"
#include "Common\Exception.h"

#include <initializer_list>
#include <functional>
#include <iterator>
#include <memory>

#include "Algorithm\AlgorithmExt.h"


namespace Core
{
	namespace Container
	{

		/*
		  RingQueue 반복자 구현
		*/
		/**< Const ring queue iterator */
		template <typename _RingQueue>
		class const_ring_queue_iterator
		{
		public:
			using iterator_category = std::bidirectional_iterator_tag;
			using value_type = typename _RingQueue::value_type;
			using pointer = typename _RingQueue::pointer;
			using const_pointer = typename _RingQueue::const_pointer;
			using reference = typename _RingQueue::reference;
			using const_reference = typename _RingQueue::const_reference;

			using difference_type = typename _RingQueue::difference_type;
			using index_type = typename _RingQueue::index_type;

			using self_type = const_ring_queue_iterator<_RingQueue>;

#ifdef MS_COMPILER
			/*
			  MS 컴파일러인 경우 추가
			  MS컴파일러는 STL 함수를 사용시에도 buf overflow 방지를 위해
			  크기 확인이 안되는 함수들, 반복자들을 deprecated 처리했기 때문에
			  오류 or 경고를 출력함.
			*/
			//using _Unchecked_type = iterator;
#endif

		public:
			const_ring_queue_iterator();
			const_ring_queue_iterator(const self_type&) = default;
			explicit const_ring_queue_iterator(const _RingQueue&);

			~const_ring_queue_iterator() = default;

			const value_type& operator*() const;
			const value_type* operator->() const;

			self_type& operator++();
			self_type operator++(int);

			self_type& operator--();
			self_type operator--(int);

			bool operator==(const self_type& rhs) const;
			bool operator!=(const self_type& rhs) const;

			self_type& operator=(const self_type& rhs); //this is default operator

		protected:
			FLINE void _Increase() noexcept;
			FLINE void _Decrease() noexcept;

		protected:
			const _RingQueue* m_pTargetQueue;
			index_type m_index;

		};

		template <typename _RingQueue>
		const_ring_queue_iterator<_RingQueue>::const_ring_queue_iterator()
			: m_pTargetQueue(nullptr)
			, m_index(-1)
		{

		}

		template <typename _RingQueue>
		const_ring_queue_iterator<_RingQueue>::const_ring_queue_iterator(const _RingQueue& ringQueue)
			: m_pTargetQueue(&ringQueue)
			, m_index(ringQueue.m_frontPos)
		{
			//첫 항목에 접근 가능한 index
		}




		template <typename _RingQueue>
		const typename const_ring_queue_iterator<_RingQueue>::value_type&
			const_ring_queue_iterator<_RingQueue>::operator*() const
		{
			return m_pTargetQueue->m_pRingData
				[(m_index + 1) % m_pTargetQueue->_GetRealMaxSize()];
		}

		template <typename _RingQueue>
		const typename const_ring_queue_iterator<_RingQueue>::value_type*
			const_ring_queue_iterator<_RingQueue>::operator->() const
		{
			return &(m_pTargetQueue->m_pRingData
				[(m_index + 1) % m_pTargetQueue->_GetRealMaxSize()]);
		}

		template <typename _RingQueue>
		typename const_ring_queue_iterator<_RingQueue>::self_type&
			const_ring_queue_iterator<_RingQueue>::operator++()
		{
			_Increase();
			return *this;
		}

		template <typename _RingQueue>
		typename const_ring_queue_iterator<_RingQueue>::self_type
			const_ring_queue_iterator<_RingQueue>::operator++(int)
		{
			auto prevIt = *this;
			_Increase();
			return prevIt;
		}

		template <typename _RingQueue>
		typename const_ring_queue_iterator<_RingQueue>::self_type&
			const_ring_queue_iterator<_RingQueue>::operator--()
		{
			_Decrease();
			return *this;
		}

		template <typename _RingQueue>
		typename const_ring_queue_iterator<_RingQueue>::self_type
			const_ring_queue_iterator<_RingQueue>::operator--(int)
		{
			auto prevIt = *this;
			_Decrease();
			return prevIt;
		}

		template <typename _RingQueue>
		bool
			const_ring_queue_iterator<_RingQueue>::operator==(const self_type& rhs) const
		{
			return ((m_pTargetQueue == rhs.m_pTargetQueue) &&
				(m_index == rhs.m_index));
		}

		template <typename _RingQueue>
		bool
			const_ring_queue_iterator<_RingQueue>::operator!=(const self_type& rhs) const
		{
			return !(*this == rhs);
		}

		template <typename _RingQueue>
		typename const_ring_queue_iterator<_RingQueue>::self_type&
			const_ring_queue_iterator<_RingQueue>::operator=(const self_type& rhs)
		{
			//self check
			GUARD_SELF_ASSIGNMENT;

			m_pTargetQueue = rhs.m_pTargetQueue;
			m_index = rhs.m_index;

			return *this;
		}

		template <typename _RingQueue>
		void
			const_ring_queue_iterator<_RingQueue>::_Increase() noexcept
		{
			m_index = (++m_index) % (m_pTargetQueue->_GetRealMaxSize());// +m_front;
		}

		template <typename _RingQueue>
		void
			const_ring_queue_iterator<_RingQueue>::_Decrease() noexcept
		{
			--m_index;

			if (m_index < 0)
				m_index = (m_pTargetQueue->_GetRealMaxSize() - 1);
		}	 	  	  	   	





		/**< RingQueue iterator */
		template <typename _RingQueue>
		class ring_queue_iterator :
			public const_ring_queue_iterator<_RingQueue>
		{
		public:
			using value_type = typename const_ring_queue_iterator<_RingQueue>::value_type;
			using index_type = typename const_ring_queue_iterator<_RingQueue>::index_type;
			using base_type = const_ring_queue_iterator<_RingQueue>;
			using self_type = ring_queue_iterator<_RingQueue>;

		public:
			ring_queue_iterator();
			ring_queue_iterator(const self_type&) = default;
			explicit ring_queue_iterator(const _RingQueue&);

			~ring_queue_iterator() = default;

			value_type& operator*() const;
			value_type* operator->() const;

			self_type& operator++();
			self_type  operator++(int);

			self_type& operator--();
			self_type  operator--(int);

		};


		template <typename _RingQueue>
		ring_queue_iterator<_RingQueue>::ring_queue_iterator()
			: const_ring_queue_iterator<_RingQueue>()
		{

		}

		template <typename _RingQueue>
		ring_queue_iterator<_RingQueue>::ring_queue_iterator(const _RingQueue& src)
			: const_ring_queue_iterator<_RingQueue>(src)
		{

		}

		template <typename _RingQueue>
		typename ring_queue_iterator<_RingQueue>::value_type&
			ring_queue_iterator<_RingQueue>::operator*() const
		{
			return const_cast<value_type&>(base_type::operator*());
		}

		template <typename _RingQueue>
		typename ring_queue_iterator<_RingQueue>::value_type*
			ring_queue_iterator<_RingQueue>::operator->() const
		{
			return const_cast<value_type*>(base_type::operator->());
		}

		template <typename _RingQueue>
		typename ring_queue_iterator<_RingQueue>::self_type&
			ring_queue_iterator<_RingQueue>::operator++()
		{
			this->_Increase();
			return *this;
		}

		template <typename _RingQueue>
		typename ring_queue_iterator<_RingQueue>::self_type
			ring_queue_iterator<_RingQueue>::operator++(int)
		{
			auto prevIt = *this;
			this->_Increase();
			return prevIt;
		}

		template <typename _RingQueue>
		typename ring_queue_iterator<_RingQueue>::self_type&
			ring_queue_iterator<_RingQueue>::operator--()
		{
			this->_Decrease();
			return *this;
		}

		template <typename _RingQueue>
		typename ring_queue_iterator<_RingQueue>::self_type
			ring_queue_iterator<_RingQueue>::operator--(int)
		{
			auto prevIt = *this;
			this->_Decrease();
			return prevIt;;
		}






		/**< Const reverse ring queue iterator */
		template <typename _RingQueue>
		class const_reverse_ring_queue_iterator :
			public const_ring_queue_iterator<_RingQueue>
		{
		public:
			using value_type = typename const_ring_queue_iterator<_RingQueue>::value_type;
			using index_type = typename const_ring_queue_iterator<_RingQueue>::index_type;
			using base_type = const_ring_queue_iterator<_RingQueue>;
			using self_type = const_reverse_ring_queue_iterator<_RingQueue>;

		public:
			const_reverse_ring_queue_iterator();
			const_reverse_ring_queue_iterator(const self_type&) = default;
			explicit const_reverse_ring_queue_iterator(const _RingQueue&);

			~const_reverse_ring_queue_iterator() = default;

			//* ->는 기존것으로도 충분함

			self_type& operator++();
			self_type operator++(int);

			self_type& operator--();
			self_type operator--(int);

			//대입 연산자도 기존것으로 충분함

		};




		template <typename _RingQueue>
		const_reverse_ring_queue_iterator<_RingQueue>::const_reverse_ring_queue_iterator()
			: const_ring_queue_iterator<_RingQueue>()
		{

		}

		template <typename _RingQueue>
		const_reverse_ring_queue_iterator<_RingQueue>::const_reverse_ring_queue_iterator(const _RingQueue& src)
			: const_ring_queue_iterator<_RingQueue>(src)
		{
			//front 상태로 초기화됨
			//역방향 시작임을 명시할 것. -2를 하여 마지막 객체에 index 위치
			this->_Decrease();
			this->_Decrease();
		}

		template <typename _RingQueue>
		typename const_reverse_ring_queue_iterator<_RingQueue>::self_type&
			const_reverse_ring_queue_iterator<_RingQueue>::operator++()
		{
			this->_Decrease();
			return *this;
		}

		template <typename _RingQueue>
		typename const_reverse_ring_queue_iterator<_RingQueue>::self_type
			const_reverse_ring_queue_iterator<_RingQueue>::operator++(int)
		{
			auto prevIt = *this;
			this->_Decrease();
			return prevIt;
		}

		template <typename _RingQueue>
		typename const_reverse_ring_queue_iterator<_RingQueue>::self_type&
			const_reverse_ring_queue_iterator<_RingQueue>::operator--()
		{
			this->_Increase();
			return *this;
		}

		template <typename _RingQueue>
		typename const_reverse_ring_queue_iterator<_RingQueue>::self_type
			const_reverse_ring_queue_iterator<_RingQueue>::operator--(int)
		{
			auto prevIt = *this;
			this->_Increase();
			return prevIt;
		}



		/**< Reverse ring queue iterator */
		template <typename _RingQueue>
		class reverse_ring_queue_iterator :
			public const_reverse_ring_queue_iterator<_RingQueue>
		{
		public:
			using value_type = typename const_reverse_ring_queue_iterator<_RingQueue>::value_type;
			using index_type = typename const_reverse_ring_queue_iterator<_RingQueue>::index_type;
			using base_type = const_reverse_ring_queue_iterator<_RingQueue>;
			using self_type = reverse_ring_queue_iterator<_RingQueue>;

		public:
			reverse_ring_queue_iterator();
			reverse_ring_queue_iterator(const self_type&) = default;
			explicit reverse_ring_queue_iterator(const _RingQueue&);

			~reverse_ring_queue_iterator() = default;

			value_type& operator*() const;
			value_type* operator->() const;

			self_type& operator++();
			self_type operator++(int);

			self_type& operator--();
			self_type operator--(int);

		};




		template <typename _RingQueue>
		reverse_ring_queue_iterator<_RingQueue>::reverse_ring_queue_iterator()
			: const_reverse_ring_queue_iterator()
		{

		}

		template <typename _RingQueue>
		reverse_ring_queue_iterator<_RingQueue>::reverse_ring_queue_iterator(const _RingQueue& src)
			: const_reverse_ring_queue_iterator(src)
		{

		}

		template <typename _RingQueue>
		typename reverse_ring_queue_iterator<_RingQueue>::value_type&
			reverse_ring_queue_iterator<_RingQueue>::operator*() const
		{
			return const_cast<value_type&>(base_type::operator*());
		}

		template <typename _RingQueue>
		typename reverse_ring_queue_iterator<_RingQueue>::value_type*
			reverse_ring_queue_iterator<_RingQueue>::operator->() const
		{
			return const_cast<value_type*>(base_type::operator->());
		}

		template <typename _RingQueue>
		typename reverse_ring_queue_iterator<_RingQueue>::self_type&
			reverse_ring_queue_iterator<_RingQueue>::operator++()
		{
			this->_Decrease();
			return *this;
		}

		template <typename _RingQueue>
		typename reverse_ring_queue_iterator<_RingQueue>::self_type
			reverse_ring_queue_iterator<_RingQueue>::operator++(int)
		{
			auto prevIt = *this;
			this->_Decrease();
			return prevIt;
		}

		template <typename _RingQueue>
		typename reverse_ring_queue_iterator<_RingQueue>::self_type&
			reverse_ring_queue_iterator<_RingQueue>::operator--()
		{
			this->_Increase();
			return *this;
		}

		template <typename _RingQueue>
		typename reverse_ring_queue_iterator<_RingQueue>::self_type
			reverse_ring_queue_iterator<_RingQueue>::operator--(int)
		{
			auto prevIt = *this;
			this->_Increase();
			return prevIt;
		}


		/**
		@enum ERingQueueCopyPolicy
		@brief RingQueue에서 사용하는 복제 정책
		@remarks
		Example
		Format => (적용방법 | 이동 or 복제될 소스의 진행 방향 | 목적지의 진행 방향)

		ClearSrcFrontToDestFront => 목적지의 큐를 초기화 한 후
									소스 큐의 맨 앞 부분 객체부터 차례대로
									목적지의 앞 부분에 순차적으로 복사한다.

		@date 2018-08-19  오후 17:39:13
		@version 1.0v
		*/
		enum class ERingQueuePolicy
		{
			ClearSrcFrontToDestFront,
			ClearSrcFrontToDestRear,
			ClearSrcRearToDestFront,
			ClearSrcRearToDestRear,
			None,                        /**<This is pivot, so.. Is not working */
			AppendSrcFrontToDestFront,
			AppendSrcFrontToDestRear,
			AppendSrcRearToDestFront,
			AppendSrcRearToDestRear,
			DefaultPolicy = ClearSrcFrontToDestRear
		};


		/**
		@class RingQueue
		@brief STL 표준을 지원하고 고정 배열을 가진 RingQueue Template
		@remarks
		 Size값은 최소 0보다 큰 값이 지정되어야 합니다.
		 복사 정책으로는 GYP::Engine::Core::Container::ERingQueueCopyPolicy를 사용합니다.
		@see GYP::Engine::Core::Container::ERingQueueCopyPolicy
		@date 2018-08-19  오후 17:30:19
		@version 1.0v
		*/
		template <typename T, size_t Size, class Allocator = std::allocator<T>>
		class RingQueue
		{
			static_assert(Size > 0, "RingQueue Size Size must be greater than 0");

			//Type alias
		public:

			using self_type = RingQueue<T, Size, Allocator>;

			/**
			@name STL 표준 alias
			@{
			*/
			using value_type = T;
			using reference = value_type & ;
			using const_reference = const reference;
			using pointer = value_type * ;
			using const_pointer = const pointer;

			using size_type = size_t;
			using index_type = int64;
			using difference_type = ptrdiff_t;

			using allocator_type = Allocator;

			//iterator
			using const_iterator = const_ring_queue_iterator<self_type>;
			using iterator = ring_queue_iterator<self_type>;

			using const_reverse_iterator = const_reverse_ring_queue_iterator<self_type>;
			using reverse_iterator = reverse_ring_queue_iterator<self_type>;
			/*@}*/

			friend class const_ring_queue_iterator<self_type>;
			friend class ring_queue_iterator<self_type>;

			friend class const_reverse_ring_queue_iterator<self_type>;
			friend class reverse_ring_queue_iterator<self_type>;

			//Methods
		public:
			/**
			@name Constructor
			@remarks
			OverWritingMode 설정에 따라서 큐가 꽉 찼을 때를 무시하고 계속 데이터를 삽입할 수 있습니다.
			@{*/

			/**
			생성자
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			*/
			RingQueue(bool overWritingMode);

			/**
			기본 생성자 OverWritingMode = false
			*/
			RingQueue();

			/**
			초기화 리스트 생성자
			@param initList 큐에 삽입될 객체 초기화 리스트
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			@param policy 큐에 initList 객체를 삽입하는 정책
			*/
			RingQueue(const std::initializer_list<value_type>& initList,
				bool overWritingMode, ERingQueuePolicy policy);

			/**
			초기화 리스트 생성자 Policy = ERingQueueCopyPolicy::DefaultPolicy
			@param initList 큐에 삽입될 객체 초기화 리스트
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			*/
			RingQueue(const std::initializer_list<value_type>& initList, bool overWritingMode);

			/**
			초기화 리스트 생성자 OverWritingMode = false, Policy = ERingQueueCopyPolicy::DefaultPolicy
			@param initList 큐에 삽입될 객체 초기화 리스트
			*/
			RingQueue(const std::initializer_list<value_type>& initList);

			/**
			복제 생성자
			@param src 복제할 큐의 원본
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			@param policy 큐에 src 객체를 복제 삽입하는 정책
			*/
			RingQueue(const self_type& src, bool overWritingMode, ERingQueuePolicy policy);

			/**
			복제 생성자 Policy = ERingQueueCopyPolicy::DefaultPolicy
			@param src 복제할 큐의 원본
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			*/
			RingQueue(const self_type& src, bool overWritingMode);

			/**
			복제 생성자  OverWritingMode = src.IsOverWrting(), Policy = ERingQueueCopyPolicy::DefaultPolicy
			@param src 복제할 큐의 원본
			*/
			RingQueue(const self_type& src);

			/**
			복제 생성자
			@param src 복제할 큐의 원본
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			@param policy 큐에 src 객체를 복제 삽입하는 정책
			*/
			template <size_t SrcSize, class SrcAllocator>
			RingQueue(const RingQueue<T, SrcSize, SrcAllocator>& src,
				bool overWritingMode, ERingQueuePolicy policy);

			/**
			복제 생성자 Policy = ERingQueueCopyPolicy::DefaultPolicy
			@param src 복제할 큐의 원본
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			*/
			template <size_t SrcSize, class SrcAllocator>
			RingQueue(const RingQueue<T, SrcSize, SrcAllocator>& src, bool overWritingMode);

			/**
			복제 생성자  OverWritingMode = src.IsOverWrting(), Policy = ERingQueueCopyPolicy::DefaultPolicy
			@param src 복제할 큐의 원본
			*/
			template <size_t SrcSize, class SrcAllocator>
			RingQueue(const RingQueue<T, SrcSize, SrcAllocator>& src);

			/**
			이동생성자
			@param src 이동대상이 되는 원본 큐
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			*/
			RingQueue(self_type&& src, bool overWritingMode) noexcept;

			/**
			이동생성자
			@param src 이동대상이 되는 원본 큐 OverWritingMode = src.IsOverWrting()
			*/
			RingQueue(self_type&& src) noexcept;

			/**
			이동 생성자
			@param src 이동할 큐의 원본
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			@param policy 큐에 src 객체를 삽입하는 정책
			*/
			template <size_t SrcSize, class SrcAllocator>
			RingQueue(RingQueue<T, SrcSize, SrcAllocator>&& src,
				bool overWritingMode, ERingQueuePolicy policy) noexcept;


			/**
			이동 생성자 Policy = ERingQueueCopyPolicy::DefaultPolicy
			@param src 이동할 큐의 원본
			@param overWritingMode 큐가 꽉 찼을 때 덮어 쓰기 유무
			*/
			template <size_t SrcSize, class SrcAllocator>
			RingQueue(RingQueue<T, SrcSize, SrcAllocator>&& src, bool overWritingMode) noexcept;

			/**
			이동 생성자 Policy = OverWritingMode = src.IsOverWrting(), ERingQueueCopyPolicy::DefaultPolicy
			@param src 이동할 큐의 원본
			*/
			template <size_t SrcSize, class SrcAllocator>
			RingQueue(RingQueue<T, SrcSize, SrcAllocator>&& src) noexcept;
			/*@}*/

			/**
			@name Destructor
			@{*/
			virtual ~RingQueue();
			/*@}*/



			/**
			@name Capacity
			@{
			*/

			/**
			@brief 큐에서 보관중인 객체들의 수를 반환합니다
			@return 보관중인 객체의 수
			*/
			FLINE size_type size()     const noexcept;

			/**
			@brief 큐에 가용가능한 최대 크기를 반환 합니다
			@return 큐에서 보관가능한 최대 객체의 수
			*/
			FLINE size_type max_size() const noexcept;

			/**
			@brief 큐가 비어 있는지 확인합니다
			@return true 비어있음, false 큐에 객체가 있음
			*/
			FLINE bool      empty()    const noexcept;

			/**
			@brief 큐를 비웁니다. (소유한 모든 객체의 소멸자가 호출 됩니다).
			*/
			FLINE void      clear()          noexcept;

			/**
			@brief 큐가 사용 가능한 상태인지 확인 합니다.
			@return
			True 사용가능한 상태
			False 큐의 데이터 보관 영역의 메모리가 해제된 상태 regeneration() 호출 시 재사용 가능
			*/
			FLINE bool      isAvailable() const noexcept;

			/**
			@brief 큐가 사용 불가능 상태일 때 호출하면 다시 사용가능 하도록 메모리를 재할당 합니다.
			@return True 재할당에 성공 | False 재할당에 실패(이미 할당 되있는경우 포함)
			*/
			FLINE bool      regeneration();
			/*@}*/


			/**목표 큐와 스왑합니다*/
			FLINE void swap(RingQueue&);
			//Copy
			template <typename Ty, size_t SrcSize, class SrcAllocator,
				size_t DestSize, class DestAllocator>
				static void CopyRingQueue(
					const RingQueue<Ty, SrcSize, SrcAllocator>& src,
					const size_t count,
					RingQueue<Ty, DestSize, DestAllocator>& dest,
					const bool isOverWriting,
					const ERingQueuePolicy policy);


			/**
			@name Operator
			@{
			*/
			self_type& operator=(const self_type& rhs);     //복제 대입 연산자
			template <size_t SrcSize, class SrcAllocator>   //복제 대입 다른크기
			self_type& operator=(const RingQueue<T, SrcSize, SrcAllocator>&);

			self_type& operator=(self_type&&) noexcept;     //이동 대입 연산자
			template <size_t SrcSize, class SrcAllocator>   //이동 대입 다른크기
			self_type& operator=(RingQueue<T, SrcSize, SrcAllocator>&&) noexcept;

			self_type& operator=(std::initializer_list<value_type>&); //초기화 대입 연산자
			/*@}*/

			/**
			@name Interface
			@{
			*/
			reference       front();
			const_reference front() const;

			reference       rear();
			const_reference rear() const;

			template<class... _Valty>
			bool emplaceFront(_Valty&&...);
			bool enqueueFront(value_type&&);
			bool enqueueFront(const value_type&);
			void dequeueFront();

			template<class... _Valty>
			bool emplaceRear(_Valty&&...);
			bool enqueueRear(value_type&&);
			bool enqueueRear(const value_type&);
			void dequeueRear();

			FLINE bool enqueue(value_type&&);
			FLINE bool enqueue(const value_type&); //enqueueback
			FLINE void dequeue();             //dequeueback
		   /*@}*/

			FLINE bool IsInitialState() const noexcept;
			FLINE bool IsFull() const noexcept;
			FLINE bool IsOverWriting() const noexcept;
			FLINE void SetOverWriting(bool)  noexcept;


			/**
			@name Iterator
			@{*/
			//정방향
			iterator begin();
			iterator end();
			const_iterator begin() const;
			const_iterator end() const;
			const_iterator cbegin() const;
			const_iterator cend() const;

			//역방향
			reverse_iterator rbegin();
			reverse_iterator rend();
			const_reverse_iterator rbegin() const;
			const_reverse_iterator rend() const;
			const_reverse_iterator crbegin() const;
			const_reverse_iterator crend() const;
			/*@}*/

		private:
			//상태 함수
			FLINE constexpr size_type _GetRealMaxSize() const noexcept;

			template <typename _T, size_t _Size, class _Allocator>
			FLINE void _CopyState(const RingQueue<_T, _Size, _Allocator>&) noexcept;


			//제어 함수
			FLINE void _IncreaseFrontPos() noexcept;
			FLINE void _IncreaseRearPos()  noexcept;

			FLINE void _DecreaseFrontPos() noexcept;
			FLINE void _DecreaseRearPos()  noexcept;

			FLINE void _ClearState() noexcept;

			//실제 데이터 영역 메모리 해제
			FLINE bool _DeallocateRingData() noexcept;

			//복제함수
			template <typename Ty, typename TargetSrc,
				size_t DestSize, class DestAllocator>
				FLINE static void _CopyRingQueueRaw(
					const TargetSrc& src,
					const size_t count,
					RingQueue<Ty, DestSize, DestAllocator>& dest,
					const bool isOverWriting,
					const ERingQueuePolicy policy);

			//이동함수
			template <typename Ty, typename TargetSrc,
				size_t DestSize, class DestAllocator>
				FLINE static void _MoveRingQueueRaw(
					const TargetSrc& src,
					const size_t count,
					RingQueue<Ty, DestSize, DestAllocator>& dest,
					const bool isOverWriting,
					const ERingQueuePolicy policy);

			friend class RingQueue;

			//Members
		private:
			allocator_type m_allocator;

			index_type  m_frontPos; //기록될 위치
			index_type  m_rearPos;  //기록될 위치
			size_type   m_currentElementCount;
			pointer     m_pRingData;

			bool m_isOverWriting;

		};

		/*
  반복자 구현
*/

/*
  const_ring_queue_iterator 반복자 구현
*/





















/*
  Template RingQueue 구현
*/

//기본 생성자

		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(bool overWritingMode)
			: m_allocator()
			, m_frontPos(-1)
			, m_rearPos(-1)
			, m_currentElementCount(0)
			, m_pRingData(m_allocator.allocate(_GetRealMaxSize()))
			, m_isOverWriting(overWritingMode)
		{

		}


		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue()
			: RingQueue(false)

		{

		}

		//초기화 리스트 생성자

		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			const std::initializer_list<value_type>& initList,
			bool overWritingMode,
			ERingQueuePolicy policy)
			: RingQueue(overWritingMode)
		{
			_CopyRingQueueRaw(initList, initList.size(), *this,
				overWritingMode, policy);
		}

		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			const std::initializer_list<value_type>& initList,
			bool overWritingMode)
			: RingQueue(
				initList,
				overWritingMode,
				ERingQueuePolicy::DefaultPolicy)
		{

		}


		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			const std::initializer_list<value_type>& initList)
			: RingQueue(
				initList,
				false,
				ERingQueuePolicy::DefaultPolicy)
		{

		}

		//복제생성자 동일 크기

		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			const self_type& src,
			bool overWritingMode,
			ERingQueuePolicy policy)
			: m_allocator(src.m_allocator)
			, m_frontPos(-1)
			, m_rearPos(-1)
			, m_currentElementCount(0)
			, m_pRingData(m_allocator.allocate(_GetRealMaxSize()))
			, m_isOverWriting(overWritingMode)
		{
			/*
			//ms 비표준 stdext::checked_array_iterator<int *>(b, 3)
			//사용말고 반복자 호출로 해결
			//배열 전체를 복제함
			std::copy_n(src.cbegin()
			, src._GetRealMaxSize()
			, this->begin());
			*/

			_CopyRingQueueRaw(src, src.size(), *this,
				overWritingMode, policy);
		}


		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			const self_type& src,
			bool overWritingMode)
			: RingQueue(
				src,
				overWritingMode,
				ERingQueuePolicy::DefaultPolicy)
		{

		}

		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(const self_type& src)
			: RingQueue(
				src,
				src.IsOverWriting(),
				ERingQueuePolicy::DefaultPolicy)
		{

		}

		//복제생성자 다른 크기

		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			const RingQueue<T, SrcSize, SrcAllocator>& src,
			bool overWritingMode,
			ERingQueuePolicy policy)
			: RingQueue(overWritingMode)
		{
			/*
			//확대 내용물 전부 복사
			if (Size >= SrcSize)
			{
			std::copy_n(src.cbegin()
			, src._GetRealMaxSize()
			, this->end());

			//fornt가 한바퀴 돌기 바로 직전 일 수 있음
			if (src.m_frontPos == (src._GetRealMaxSize() - 1))
			{
			m_frontPos = (_GetRealMaxSize() - 1);
			}
			else
			{
			m_frontPos = src.m_frontPos;
			}

			m_rearPos = SrcSize;
			m_currentElementCount = src.m_currentElementCount;
			}
			//축소
			else
			{
			auto srcIt = src.crbegin();

			for (auto i = 0; i < Size; ++i)
			{
			enqueueFront(*(srcIt++));
			}

			//함수를 사용해서 삽입했으므로 따로 m_front, m_rear 값을 조정 안함.
			}
			*/

			//초기상태의 큐에 복제
			CopyRingQueue(src, src.size(), *this, overWritingMode, policy);

		}

		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			const RingQueue<T, SrcSize, SrcAllocator>& src,
			bool overWritingMode)
			: RingQueue(
				src,
				overWritingMode,
				ERingQueuePolicy::DefaultPolicy)
		{

		}


		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		RingQueue<T, Size, Allocator>::RingQueue(const RingQueue<T, SrcSize, SrcAllocator>& src)
			: RingQueue(
				src,
				src.IsOverWriting(),
				ERingQueuePolicy::DefaultPolicy)
		{

		}

		//이동생성자

		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(RingQueue&& src, bool overWritingMode) noexcept
			: m_allocator(std::move(src.m_allocator))
			, m_frontPos(src.m_frontPos)
			, m_rearPos(src.m_rearPos)
			, m_currentElementCount(src.m_currentElementCount)
			, m_pRingData(src.m_pRingData)
			, m_isOverWriting(overWritingMode)
		{
			//메모리 주소를 옳김
			//src의 메모리 초기화, 상태 초기화
			src.m_pRingData = nullptr;
			src._ClearState();
		}

		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::RingQueue(RingQueue&& src) noexcept
			: RingQueue(std::move(src), src.IsOverWriting())
		{

		}

		//다른 크기 이동생성자

		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			RingQueue<T, SrcSize, SrcAllocator>&& src,
			bool overWritingMode,
			ERingQueuePolicy policy) noexcept
			: RingQueue(overWritingMode)
		{
			/*
			//확대
			if (Size >= SrcSize)
			{
				for (auto srcIt = src.begin(); srcIt != src.end(); ++srcIt)
				{
					enqueue(std::move(*srcIt));
				}
			}
			//축소
			else
			{
				auto srcIt = src.rbegin();

				for (auto i = 0; i < Size; ++i)
				{
					enqueueFront(std::move(*(srcIt++)));
				}
			}

			//원본 소멸자를 호출하여 메모리 해제
			src.~RingQueue();
			*/

			//초기상태의 큐에 데이터 이동
			//단 이동이 될려면 이동 생성자를 객체들이 지원해야 함
			_MoveRingQueueRaw(src, src.size(), *this, overWritingMode, policy);

			src._DeallocateRingData();
			src.m_pRingData = nullptr;
			src._ClearState();
		}
		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		RingQueue<T, Size, Allocator>::RingQueue(
			RingQueue<T, SrcSize, SrcAllocator>&& src,
			bool overWritingMode) noexcept
			: RingQueue(
				std::move(src),
				overWritingMode,
				ERingQueuePolicy::DefaultPolicy)
		{

		}

		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		RingQueue<T, Size, Allocator>::RingQueue(RingQueue<T, SrcSize, SrcAllocator>&& src) noexcept
			: RingQueue(
				std::move(src),
				src.IsOverWriting(),
				ERingQueuePolicy::DefaultPolicy)
		{

		}

		//소멸자
		template <typename T, size_t Size, class Allocator>
		RingQueue<T, Size, Allocator>::~RingQueue()
		{
			_DeallocateRingData();
		}





		/*
		  상태 함수 구현
		*/

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::swap(RingQueue& target)
		{
			std::swap(*this, target);
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::size_type
			RingQueue<T, Size, Allocator>::size() const noexcept
		{
			return m_currentElementCount;
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::size_type
			RingQueue<T, Size, Allocator>::max_size() const noexcept
		{
			return (_GetRealMaxSize() - 1);
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::empty() const noexcept
		{
			return (size() == 0);
		}

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::clear() noexcept
		{
			//비어 있을 때 까지 큐를 비움
			while (empty() == false)
			{
				dequeue();
			}

			//Clear state
			_ClearState();
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::IsOverWriting() const noexcept
		{
			return m_isOverWriting;
		}

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::SetOverWriting(bool b) noexcept
		{
			m_isOverWriting = b;
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::IsFull() const noexcept
		{
			return (max_size() == size());
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::IsInitialState() const noexcept
		{
			return ((m_frontPos == -1) && (m_rearPos == -1));
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::isAvailable() const noexcept
		{
			return (m_pRingData != nullptr);
		}


		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::regeneration()
		{
			if (isAvailable() == false)
			{
				m_pRingData = m_allocator.allocate(_GetRealMaxSize());

				return true;
			}

			return false;
		}

		template <typename T, size_t Size, class Allocator>
		typename constexpr RingQueue<T, Size, Allocator>::size_type
			RingQueue<T, Size, Allocator>::_GetRealMaxSize() const noexcept
		{
			return Size + 1;
		}

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::_ClearState() noexcept
		{
			m_frontPos = -1;
			m_rearPos = -1;
			m_currentElementCount = 0;
		}

		template <typename T, size_t Size, class Allocator>
		template <typename _T, size_t _Size, class _Allocator>
		void
			RingQueue<T, Size, Allocator>::_CopyState(const RingQueue<_T, _Size, _Allocator>& src) noexcept
		{
			m_frontPos = src.m_frontPos;
			m_rearPos = src.m_rearPos;
			m_currentElementCount = src.m_currentElementCount;
			m_isOverWriting = src.m_isOverWriting;

		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::_DeallocateRingData() noexcept
		{
			if (m_pRingData != nullptr)
			{
				m_allocator.deallocate(m_pRingData, _GetRealMaxSize());
				m_pRingData = nullptr;
				return true;
			}

			return false;
		}


		template <typename T, size_t Size, class Allocator>
		template <typename Ty, size_t SrcSize, class SrcAllocator,
			size_t DestSize, class DestAllocator>
			void
			RingQueue<T, Size, Allocator>::CopyRingQueue(
				const RingQueue<Ty, SrcSize, SrcAllocator>& src,
				const size_t count,
				RingQueue<Ty, DestSize, DestAllocator>& dest,
				const bool isOverWriting,
				const ERingQueuePolicy policy)
		{
			using EPolicy = ERingQueuePolicy;

			//크기 확인
			if (src.size() < count)
			{
				throw std::InvalidArgumentException("Count value is over then src size");
			}

			//clear policy 처리
			if (policy < EPolicy::None)
			{
				dest.clear();
			}

			_CopyRingQueueRaw(src, count, dest, isOverWriting, policy);

			//기본 소스 앞부분부터 차례대로 dest뒷부분에 붙인다.
			//copy_n(std::cbegin(src), count, std::begin(dest));
			//pInsertFunc = std::bind(static_cast<bool(dest_type::*)(const Ty&)>(&dest_type::enqueueRear),
			//	std::ref(dest), std::placeholders::_1);
		}

		//private

		template <typename T, size_t Size, class Allocator>
		template <typename Ty, typename TargetSrc,
			size_t DestSize, class DestAllocator>
			void
			RingQueue<T, Size, Allocator>::_CopyRingQueueRaw(
				const TargetSrc& src,
				const size_t count,
				RingQueue<Ty, DestSize, DestAllocator>& dest,
				const bool isOverWriting,
				const ERingQueuePolicy policy)
		{
			static_assert(std::is_same<TargetSrc::value_type, Ty>(), "Type is not same");

			using EPolicy = ERingQueuePolicy;
			using InsertFunc = std::function<bool(const Ty&)>;
			using dest_type = RingQueue<Ty, DestSize, DestAllocator>;

			dest.SetOverWriting(isOverWriting);

			//추가 클리어 이런건 함수 앞부분에서만 차이가 나기 때문. 방향만 감지하여 작업
			switch (policy)
			{
			case EPolicy::ClearSrcFrontToDestFront:
			case EPolicy::AppendSrcFrontToDestFront:

				stdext::InsertLoop(std::cbegin(src), count,
					[&dest](const Ty& value) -> bool
				{ return dest.enqueueFront(value); });

				break;

			case EPolicy::ClearSrcFrontToDestRear:
			case EPolicy::AppendSrcFrontToDestRear:

				stdext::InsertLoop(std::cbegin(src), count,
					[&dest](const Ty& value) -> bool
				{ return dest.enqueueRear(value); });

				break;

			case EPolicy::ClearSrcRearToDestFront:
			case EPolicy::AppendSrcRearToDestFront:

				stdext::InsertLoop(std::crbegin(src), count,
					[&dest](const Ty& value) -> bool
				{ return dest.enqueueFront(value); });

				break;

			case EPolicy::ClearSrcRearToDestRear:
			case EPolicy::AppendSrcRearToDestRear:

				//copy_n(std::crbegin(src), count, std::begin(dest));
				stdext::InsertLoop(std::crbegin(src), count,
					[&dest](const Ty& value)-> bool
				{ return dest.enqueueRear(value); });

				break;

			default:
				throw std::InvalidArgumentException("Policy argument error");
				break;
			}
		}

		template <typename T, size_t Size, class Allocator>
		template <typename Ty, typename TargetSrc,
			size_t DestSize, class DestAllocator>
			void
			RingQueue<T, Size, Allocator>::_MoveRingQueueRaw(
				const TargetSrc& src,
				const size_t count,
				RingQueue<Ty, DestSize, DestAllocator>& dest,
				const bool isOverWriting,
				const ERingQueuePolicy policy)
		{
			static_assert(std::is_same<TargetSrc::value_type, Ty>(), "Type is not same");

			using EPolicy = ERingQueuePolicy;
			using InsertFunc = std::function<bool(const Ty&)>;
			using dest_type = RingQueue<Ty, DestSize, DestAllocator>;

			dest.SetOverWriting(isOverWriting);

			//추가 클리어 이런건 함수 앞부분에서만 차이가 나기 때문. 방향만 감지하여 작업
			switch (policy)
			{
			case EPolicy::ClearSrcFrontToDestFront:
			case EPolicy::AppendSrcFrontToDestFront:

				stdext::MoveLoop(std::begin(src), count,
					[&dest](Ty&& value) -> bool
				{ return dest.enqueueFront(std::forward<Ty>(value)); });

				break;

			case EPolicy::ClearSrcFrontToDestRear:
			case EPolicy::AppendSrcFrontToDestRear:

				stdext::MoveLoop(std::begin(src), count,
					[&dest](Ty&& value) -> bool
				{ return dest.enqueueRear(std::forward<Ty>(value)); });

				break;

			case EPolicy::ClearSrcRearToDestFront:
			case EPolicy::AppendSrcRearToDestFront:

				stdext::MoveLoop(std::rbegin(src), count,
					[&dest](Ty&& value) -> bool
				{ return dest.enqueueFront(std::forward<Ty>(value)); });

				break;

			case EPolicy::ClearSrcRearToDestRear:
			case EPolicy::AppendSrcRearToDestRear:

				stdext::MoveLoop(std::rbegin(src), count,
					[&dest](Ty&& value)-> bool
				{ return dest.enqueueRear(std::forward<Ty>(value)); });

				break;

			default:
				//throw std::InvalidArgumentException("Policy argument error");
				break;
			}
		}



		/*
		  Operator
		*/

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::self_type&
			RingQueue<T, Size, Allocator>::operator=(const self_type& rhs)
		{
			GUARD_SELF_ASSIGNMENT;

			CopyRingQueue(rhs, rhs.size(), *this,
				IsOverWriting(), ERingQueuePolicy::ClearSrcFrontToDestRear);
			return *this;
		}

		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		typename RingQueue<T, Size, Allocator>::self_type&
			RingQueue<T, Size, Allocator>::operator=(const RingQueue<T, SrcSize, SrcAllocator>& rhs)
		{
			CopyRingQueue(rhs, rhs.size(), *this,
				IsOverWriting(), ERingQueuePolicy::ClearSrcFrontToDestRear);
			return *this;
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::self_type&
			RingQueue<T, Size, Allocator>::operator=(RingQueue&& rhs) noexcept
		{
			GUARD_SELF_ASSIGNMENT;

			clear();
			_MoveRingQueueRaw(rhs, rhs.size(), *this,
				IsOverWriting(), ERingQueuePolicy::ClearSrcFrontToDestRear);

			rhs._DeallocateRingData();
			rhs.m_pRingData = nullptr;
			rhs._ClearState();

			return *this;
		}

		template <typename T, size_t Size, class Allocator>
		template <size_t SrcSize, class SrcAllocator>
		typename RingQueue<T, Size, Allocator>::self_type&
			RingQueue<T, Size, Allocator>::operator=(RingQueue<T, SrcSize, SrcAllocator>&& rhs) noexcept
		{
			clear();
			_MoveRingQueueRaw(rhs, rhs.size, *this,
				IsOverWriting(), ERingQueuePolicy::ClearSrcFrontToDestRear);

			rhs._DeallocateRingData();
			rhs.m_pRingData = nullptr;
			rhs._ClearState();

			return *this;
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::self_type&
			RingQueue<T, Size, Allocator>::operator=(std::initializer_list<value_type>& initList)
		{
			CopyRingQueueRaw(initList, initList.size(), *this,
				IsOverWriting(), ERingQueuePolicy::ClearSrcFrontToDestRear);

			return *this;
		}





		/*
		  RingQueue interface 구현
		*/
		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::reference
			RingQueue<T, Size, Allocator>::front()
		{
#ifdef CONTAINER_USE_EXCEPTION
			if (empty() == true)
			{
				throw EmptyException();
			}
#endif

			const decltype(m_frontPos) realPos = (m_frontPos + 1) % _GetRealMaxSize();

			return m_pRingData[realPos];
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_reference
			RingQueue<T, Size, Allocator>::front() const
		{
#ifdef CONTAINER_USE_EXCEPTION
			if (empty() == true)
			{
				throw EmptyException();
			}
#endif
			const decltype(m_frontPos) realPos = (m_frontPos + 1) % _GetRealMaxSize();

			return m_pRingData[realPos];
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::reference
			RingQueue<T, Size, Allocator>::rear()
		{
#ifdef CONTAINER_USE_EXCEPTION
			if (empty() == true)
			{
				throw EmptyException();
			}
#endif

			decltype(m_rearPos) realPos = m_rearPos - 1;

			if (realPos < 0)
				realPos = _GetRealMaxSize() - 1;

			return m_pRingData[realPos];
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_reference
			RingQueue<T, Size, Allocator>::rear() const
		{
#ifdef CONTAINER_USE_EXCEPTION
			if (empty() == true)
			{
				throw EmptyException();
			}
#endif

			decltype(m_rearPos) realPos = m_rearPos - 1;

			if (realPos < 0)
				realPos = _GetRealMaxSize() - 1;

			return m_pRingData[realPos];
		}






		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::dequeueRear()
		{
#ifdef CONTAINER_USE_EXCEPTION
			if (empty() == true)
			{
				throw EmptyException("RingQueue is empty");
			}
#endif

			std::allocator_traits<allocator_type>::destroy(m_allocator, &rear());
			//m_allocator.destroy(&rear()); c++14

			_DecreaseRearPos();
			--m_currentElementCount;
		}

		template <typename T, size_t Size, class Allocator>
		template<class... _Valty>
		bool
			RingQueue<T, Size, Allocator>::emplaceFront(_Valty&&... value)
		{
			//초기화 상태
			if (IsInitialState() == true)
			{
				_DecreaseFrontPos();
				_IncreaseRearPos();

				std::allocator_traits<allocator_type>::construct(
					m_allocator, &m_pRingData[m_frontPos],
					std::forward<_Valty>(value)...);

				//m_allocator.construct(&m_pRingData[m_frontPos], std::forward<_Valty>(value)...);
				_DecreaseFrontPos();
				++m_currentElementCount;

				return true;
			}
			//꽉찬 상태
			else if (IsFull() == true)
			{
				//덮어 쓰기 유무 확인
				if (IsOverWriting() == true)
				{
					std::allocator_traits<allocator_type>::construct(
						m_allocator, &m_pRingData[m_frontPos],
						std::forward<_Valty>(value)...);

					//m_allocator.construct(&m_pRingData[m_frontPos], std::forward<_Valty>(value)...);
					_DecreaseFrontPos();
					_DecreaseRearPos();
					return true;
				}
				else
				{
					return false;
				}
			}
			//일반 상태
			else
			{
				std::allocator_traits<allocator_type>::construct(
					m_allocator, &m_pRingData[m_frontPos],
					std::forward<_Valty>(value)...);

				//m_allocator.construct(&m_pRingData[m_frontPos], std::forward<_Valty>(value)...);
				_DecreaseFrontPos();
				++m_currentElementCount;

				return true;
			}
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::enqueueFront(value_type&& value)
		{
			return emplaceFront(std::move(value));
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::enqueueFront(const value_type& value)
		{
			return emplaceFront(value);
		}


		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::dequeueFront()
		{
#ifdef CONTAINER_USE_EXCEPTION
			if (empty() == true)
			{
				throw EmptyException("RingQueue is empty");
			}
#endif

			std::allocator_traits<allocator_type>::destroy(m_allocator, &front());
			//m_allocator.destroy(&front());

			_IncreaseFrontPos();
			--m_currentElementCount;
		}

		template <typename T, size_t Size, class Allocator>
		template<class... _Valty>
		bool
			RingQueue<T, Size, Allocator>::emplaceRear(_Valty&& ...value)
		{
			//초기화 상태인가
			if (IsInitialState() == true)
			{
				_IncreaseRearPos();
				_DecreaseFrontPos();

				std::allocator_traits<allocator_type>::construct(
					m_allocator, &m_pRingData[m_rearPos],
					std::forward<_Valty>(value)...);
				//m_allocator.construct(&m_pRingData[m_rearPos], std::forward<_Valty>(value)...);
				_IncreaseRearPos();
				++m_currentElementCount;

				return true;
			}
			//풀상태인가
			else if (IsFull() == true)
			{
				//오버라이팅 상태
				if (IsOverWriting() == true)
				{
					std::allocator_traits<allocator_type>::construct(
						m_allocator, &m_pRingData[m_rearPos],
						std::forward<_Valty>(value)...);

					//m_allocator.construct(&m_pRingData[m_rearPos], std::forward<_Valty>(value)...);
					_IncreaseRearPos();
					_IncreaseFrontPos();
					//_IncreaseFrontPos(); //공백을 건너 뛰어야 함
					return true;
				}
				else
				{
					return false;
				}
			}
			//일반상태
			else
			{
				std::allocator_traits<allocator_type>::construct(
					m_allocator, &m_pRingData[m_rearPos],
					std::forward<_Valty>(value)...);

				//m_allocator.construct(&m_pRingData[m_rearPos], std::forward<_Valty>(value)...);
				_IncreaseRearPos();
				++m_currentElementCount;
				return true;
			}
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::enqueueRear(value_type&& value)
		{
			return emplaceRear(std::move(value));
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::enqueueRear(const value_type& value)
		{
			return emplaceRear(value);
		}



		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::enqueue(value_type&& value)
		{
			return emplaceRear(std::move(value));
		}

		template <typename T, size_t Size, class Allocator>
		bool
			RingQueue<T, Size, Allocator>::enqueue(const value_type& value)
		{
			return emplaceRear(value);
		}

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::dequeue()
		{
			dequeueFront();
		}





		/*
		  제어 함수 구현
		*/


		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::_IncreaseFrontPos() noexcept
		{
			m_frontPos = (++m_frontPos) % _GetRealMaxSize();
		}

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::_IncreaseRearPos() noexcept
		{
			m_rearPos = (++m_rearPos) % _GetRealMaxSize();
		}

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::_DecreaseFrontPos() noexcept
		{
			--m_frontPos;

			if (m_frontPos < 0)
				m_frontPos = _GetRealMaxSize() - 1;
		}

		template <typename T, size_t Size, class Allocator>
		void
			RingQueue<T, Size, Allocator>::_DecreaseRearPos() noexcept
		{
			--m_rearPos;

			if (m_rearPos < 0)
				m_rearPos = _GetRealMaxSize() - 1;
		}





		/*
		  반복자 반환 구현
		*/

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::iterator
			RingQueue<T, Size, Allocator>::begin()
		{
			if (empty() == true)
			{
				return end();
			}
			else
			{
				return ring_queue_iterator<self_type>(*this);
			}
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::iterator
			RingQueue<T, Size, Allocator>::end()
		{
			iterator it(*this);
			--it;
			return it;
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_iterator
			RingQueue<T, Size, Allocator>::begin() const
		{
			return const_cast<self_type*>(this)->begin();
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_iterator
			RingQueue<T, Size, Allocator>::end() const
		{
			return const_cast<self_type*>(this)->end();
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_iterator
			RingQueue<T, Size, Allocator>::cbegin() const
		{
			return begin();
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_iterator
			RingQueue<T, Size, Allocator>::cend() const
		{
			return end();
		}






		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::reverse_iterator
			RingQueue<T, Size, Allocator>::rbegin()
		{
			if (empty() == true)
			{
				return rend();
			}
			else
			{
				return reverse_iterator(*this);
			}
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::reverse_iterator
			RingQueue<T, Size, Allocator>::rend()
		{
			reverse_iterator it(*this);
			--it;
			return it;
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_reverse_iterator
			RingQueue<T, Size, Allocator>::rbegin() const
		{
			return const_cast<self_type*>(this)->rbegin();
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_reverse_iterator
			RingQueue<T, Size, Allocator>::rend() const
		{
			return const_cast<self_type*>(this)->rend();
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_reverse_iterator
			RingQueue<T, Size, Allocator>::crbegin() const
		{
			return rbegin();
		}

		template <typename T, size_t Size, class Allocator>
		typename RingQueue<T, Size, Allocator>::const_reverse_iterator
			RingQueue<T, Size, Allocator>::crend() const
		{
			return rend();
		}

	}
}
