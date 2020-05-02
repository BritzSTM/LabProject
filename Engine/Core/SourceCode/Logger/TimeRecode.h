/**
@file TimeRecode.h
@brief CTimeStamp + Msg의 구조를 가지는 Recode를 정의 합니다.
@date 2018-10-15  오후 20:20:27
*/


#pragma once


#include "ILogger.h"
#include "Time/Time.h"
#include "Type\TypeTraits.h"


#include <sstream>


namespace Core
{
	namespace Log
	{
		/**
		@brief  CTimeStamp + Msg의 구조를 가지는 Recode를 정의 합니다.
		@date 2018-10-15  오후 21:46:09
		@version 1.0v
		*/
		class TimeRecode final
			: public IRecode
		{

		public:
			virtual ~TimeRecode() {}

			/**
			지정된 시간과 메시지, Split으로 레코드를 생성합니다
			*/
			TimeRecode(
				const Time::CTimeStamp& time,
				const string& msg,
				const char* pTimeAndRecodeSplit,
				const char* pRecodeSplit);

			/**
			지정된 시간, 메시지로 레코드를 생성합니다
			*/
			TimeRecode(const Time::CTimeStamp& time, const string& msg);

			/**
			현재 시간, 메시지로 레코드를 생성합니다
			*/
			TimeRecode(const string& msg);

			/**
			현재시간으로 비어있는 레코드를 생성합니다
			*/
			TimeRecode();

			/**
			기본 복제 생성자
			*/
			TimeRecode(const TimeRecode& src) = default;

			/**
			이동 생성자
			*/
			TimeRecode(TimeRecode&& src) noexcept;




			//Operators

			/**
			복제 대입연산자.
			*/
			TimeRecode& operator= (const TimeRecode& rhs);

			/**
			이동 대입연산자
			*/
			TimeRecode& operator= (TimeRecode&& rhs) noexcept;

			/**
			비교 연산자. message만 비교한다.
			시간 비교가 필요한 경우는 TimeTag만 획득하여 비교한다.
			*/
			bool operator==(const TimeRecode& rhs);


			//IRecode Interfacce Methods

			/**
			레코드를 추가로 기록합니다.
			*/
			virtual void WriteRecode(string message) override;

			/**
			레코드를 초기화 합니다. (TimeTag 제외)
			*/
			FLINE virtual void ClearRecode() override
			{
				m_message.clear();
			}

			/**
			TimeTag가 포함된 레코드를 출력합니다.
			*/
			virtual string ToString() const override;

			/**
			TimeTag가 없는 레코드를 출력합니다. true,
			false의 값에는 의미가 없습니다.
			*/
			virtual string ToString(bool);

			/**
			원하는 형식의 TimeTag로 레코드를 출력합니다.
			*/
			
			template <typename _TimeTag>
			string ToString(_TimeTag)
			{
				using namespace Time;

				static_assert(
					std::is_base_of<
					Tag::TimeTag, _TimeTag>::value,
					"Is Not Time Tag");

				std::stringstream s;
				s << m_timeTag.ToString(_TimeTag{});
				
				if(m_pTimeAndRecodeSplit != nullptr)
					s << m_pTimeAndRecodeSplit;

				s << m_message;

				return s.str();
			}

			//TimeRecode Methods

			/**
			TimeRecode의 TimeTag를 획득합니다. 획득 후 수정이 가능합니다.
			*/
			virtual Time::CTimeStamp& GetTime() noexcept
			{
				return m_timeTag;
			}

			/**
			Recode의 Split을 설정합니다.
			*/
			void SetSplit(char *c) noexcept
			{
				m_pRecodeSplit = c;
			}

		private:
			Time::CTimeStamp m_timeTag;
			const char*      m_pTimeAndRecodeSplit; //TimeTag와 RecodeMessage의 구분자
			const char*      m_pRecodeSplit;        //추가로 들어온 메시지들에 대한 구분자
			string           m_message;
		};
	}
}