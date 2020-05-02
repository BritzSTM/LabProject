/**
@file Time.h
@brief 시간을 표현하기 위한 클래스들의 구현
@date 2018-10-13  오후 20:18:21
*/


#pragma once


#include "Platform\PlatformInfo.h"
#include "Container\string.h"
#include "Type\TypeTraits.h"


#include <chrono>
#include <tbb/tick_count.h>


namespace Core
{
	namespace Time
	{

		//std리터럴 사용
		using namespace std::chrono_literals;


		/**TimeStamp에서 사용하는 문자열format을 위한 tag 구조체*/
		namespace Tag
		{
			struct TimeTag { };
			struct MM_dd_yyyy_tt_HH_mm_ss : public TimeTag { };
			struct MM_dd_tt_HH_mm_ss : public TimeTag { };
			struct tt_HH_mm_ss : public TimeTag { };
		}


		/**
		@brief 
		시간에 대해 표현하는 객체입니다. 
		ToString()을통해 값의 확인이 가능합니다.
		@date 2018-10-13  오후 20:23:14
		@version 1.0v
		*/
		class CTimeStamp final
			: public IToString
		{
			//현재는 윈도우64 bit만 구현
			static_assert(GetOSType() == ECOS_TYPE::WIN64_, "Not impl CTimeStamp");
		public:
			using Tick      = time_t;
			using Clock     = std::chrono::system_clock;
			using TimePoint = std::chrono::time_point<Clock>;

			
			//rename
			using Hours        = std::chrono::hours;
			using Minutes      = std::chrono::minutes;
			using Seconds      = std::chrono::seconds;
			using Milliseconds = std::chrono::milliseconds;

			//cast rename
			template<class _ToDuration, class _Duration>
			FLINE static constexpr auto DurationCast(const _Duration &dur)
			{
				return std::chrono::duration_cast<_ToDuration>(dur);
			}
		

		public:
			CTimeStamp(TimePoint Tick, const char* pSplict = "/", const char*pBorders = "[]");
			CTimeStamp();
			CTimeStamp(const CTimeStamp&) = default;

			virtual ~CTimeStamp() {}

			/**
			IToString interface
			*/
			FLINE virtual string ToString() const override
			{
				return ToString(Tag::TimeTag{});
			}

			template <typename TAG>
			FLINE string ToString(TAG) const
			{
				static_assert(
					std::is_base_of<Tag::TimeTag, TAG>::value ,
					"Is not TimeTag");

				return _GetString(TAG{});
			}

			/**
			현재 시간으로 생성된 TimeStamp를 반환
			*/
			static CTimeStamp Now() noexcept;

			/**
			milliseconds 단위차를 반환함
			*/
			auto operator-(const CTimeStamp& rhs)
			{
				using namespace std::chrono;

				return duration_cast<milliseconds>(m_tick - rhs.m_tick);
			}

			/**
			일정 시간을 더해 반환됨
			*/
			template <typename Duration>
			auto operator+(const Duration& rhs)
			{
				using namespace std::chrono;

				static_assert(
					stdext::is_base_of_template<duration, Duration>::value,
					"rhs is not duration template");

				m_tick = m_tick + rhs;
				return *this;
			}
		
		private:
			// 1900+
			FLINE static constexpr int _GetYearReviseValue() noexcept
			{
				return 1900;
			}

			//숫자 형식 포멧 00
			FLINE static std::ostream& _NumFormatting(std::ostream& o) noexcept
			{
				o.fill('0');
				o.width(2);
				return o;
			}

			//실제 Tag 구현 함수
			string _GetString(Tag::TimeTag) const;
			string _GetString(Tag::MM_dd_yyyy_tt_HH_mm_ss) const;
			string _GetString(Tag::MM_dd_tt_HH_mm_ss) const;
			string _GetString(Tag::tt_HH_mm_ss) const;

		private:
			TimePoint m_tick;
			const char* m_pRecodeSplit;
			const char* m_pBorders;
		};

		/**
		TBB의 정밀카운터 tbb::tick_count rename
		*/
		using CTickCounter = tbb::tick_count;

	}
}