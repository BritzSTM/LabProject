#include "stdafx.h"
#include "TimeRecode.h"


namespace Core
{
	namespace Log
	{

		TimeRecode::TimeRecode(
			const Time::CTimeStamp& time,
			const string& msg,
			const char* pTimeAndRecodeSplit,
			const char* pRecodeSplit)
			       : m_timeTag(time)
			       , m_message(msg)
			       , m_pTimeAndRecodeSplit(pTimeAndRecodeSplit)
			       , m_pRecodeSplit(pRecodeSplit)
		{
			
		}

		TimeRecode::TimeRecode(const Time::CTimeStamp& time, const string& msg)
			       : TimeRecode(time, msg, " ", " ")
		{

		}

		TimeRecode::TimeRecode(const string& msg)
			       : TimeRecode(Time::CTimeStamp::Now(), msg)
		{

		}

		TimeRecode::TimeRecode()
			       : TimeRecode(Time::CTimeStamp::Now(),"")
		{

		}

		TimeRecode::TimeRecode(TimeRecode&& src) noexcept
			: m_timeTag(std::move(src.m_timeTag))
			, m_message(std::move(src.m_message))
			, m_pTimeAndRecodeSplit(src.m_pTimeAndRecodeSplit)
			, m_pRecodeSplit(src.m_pTimeAndRecodeSplit)
		{

		}


		//Operators


		TimeRecode& TimeRecode::operator= (const TimeRecode& rhs)
		{
			GUARD_SELF_ASSIGNMENT;

			m_timeTag = rhs.m_timeTag;
			m_message = rhs.m_message;
			m_pTimeAndRecodeSplit = rhs.m_pTimeAndRecodeSplit;
			m_pRecodeSplit = rhs.m_pRecodeSplit;

			return *this;
		}

		TimeRecode& TimeRecode::operator= (TimeRecode&& rhs) noexcept
		{
			m_timeTag = std::move(rhs.m_timeTag);
			m_message = std::move(rhs.m_message);
			m_pTimeAndRecodeSplit = rhs.m_pTimeAndRecodeSplit;
			m_pRecodeSplit = rhs.m_pRecodeSplit;

			return *this;
		}

		bool TimeRecode::operator==(const TimeRecode& rhs)
		{
			return (m_message == rhs.m_message);
		}


		//IRecode Interfacce Methods

		void TimeRecode::WriteRecode(string message)
		{
			if (m_pRecodeSplit != nullptr)
				m_message.append(m_pRecodeSplit);

			m_message.append(message);
		}

		string TimeRecode::ToString() const
		{
			std::stringstream s;
			
			s << m_timeTag.ToString();

			if(m_pTimeAndRecodeSplit != nullptr)
				s << m_pTimeAndRecodeSplit;                //TimeTag split
			
			s << m_message;

			return s.str();
		}
		
		string TimeRecode::ToString(bool)
		{
			return m_message;
		}
		
	}
}