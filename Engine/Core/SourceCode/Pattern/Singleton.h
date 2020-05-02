/**
@file Singleton.h
@brief 싱글톤 패턴 템플릿
@remarks
인스턴스 생성을 std::once_call을 사용하였음.

@date 2018-08-23  오후 18:47:49
*/
#pragma once


#include "CoreHelper.h"
#include "Common\BaseTypes.h"


#include <memory>
#include "Thread/Mutex.h"


namespace Core
{
	namespace Pattern
	{
		/**
		@brief Singleton 패턴 템플릿 클래스
		@remarks 
		@date 2018-10-15  오후 21:48:27
		@version 1.0v
		*/
		template <typename T>
		class TSingleton
		{
		public:
			using self_type = TSingleton<T>;
			using object_type = T;

			using pointer = object_type* ;
			using const_pointer = const pointer;

			using references = object_type& ;
			using const_references = const references;

		public:
			virtual ~TSingleton() {}

			template <typename ...Args>
			static pointer GetInstance(Args&&...values)
			{
				//static object_type instance;
				std::call_once(
					TSingleton<T>::s_onceFlag,
					[&values...]()
				{
					self_type::s_instance.reset(new T(std::forward<Args>(values)...));
					//self_type::s_instance = std::make_shared<T>(std::forward<Args>(values)...);
				});

				return &(*self_type::s_instance);
			}

			static auto GetRefCount() noexcept
			{
				return self_type::s_instance.use_count();
			}

		private:
			TSingleton() = delete;
			TSingleton(const TSingleton&) = delete;
			TSingleton(TSingleton&&) noexcept = delete;

			self_type& operator=(const self_type&) = delete;

		private:
			static std::shared_ptr<object_type> s_instance;
			static std::once_flag s_onceFlag;
		};

		template <typename T>
		std::shared_ptr<T> TSingleton<T>::s_instance = nullptr;

		template <typename T>
		std::once_flag TSingleton<T>::s_onceFlag;

	}
}
