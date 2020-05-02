/**
@file Time.cpp
@brief 시간을 표현하기 위한 클래스들의 구현
@date 2018-10-13  오후 20:18:21
*/
#include "stdafx.h"
#include "Time.h"

#include <ctime>
#include <iomanip>
#include <sstream>


namespace Core
{
	namespace Time
	{
		CTimeStamp::CTimeStamp(TimePoint Tick, const char* pSplit, const char*pBorders)
			       : m_tick(Tick)
			       , m_pRecodeSplit(pSplit)
			       , m_pBorders(pBorders)
		{

		}

		CTimeStamp::CTimeStamp()
			: CTimeStamp(Clock::from_time_t(0))
		{

		}

		CTimeStamp CTimeStamp::Now() noexcept
		{
			return Clock::now();
		}

		string CTimeStamp::_GetString(Tag::TimeTag) const
		{
			return _GetString(Tag::MM_dd_yyyy_tt_HH_mm_ss{});
		}

		string CTimeStamp::_GetString(Tag::MM_dd_yyyy_tt_HH_mm_ss) const
		{
			std::stringstream s;
			
			//get time values
			const Tick tt = Clock::to_time_t(m_tick);
			tm tm;
			localtime_s(&tm, &tt);
			
			s << m_pBorders[0];

			//MM
			s << _NumFormatting << tm.tm_mon; s << m_pRecodeSplit[0];

			//dd
			s << _NumFormatting << tm.tm_mday; s << m_pRecodeSplit[0];

			//yyyy
			s << (tm.tm_year+_GetYearReviseValue()); s.put(' ');

			//tt
			s << ((tm.tm_hour < 12) ? ("AM") : ("PM")); s.put(' ');
			
			//HH
			s << _NumFormatting << tm.tm_hour; s << ":";

			//mm
			s << _NumFormatting << tm.tm_min; s << ":";
			
			//s
			s << _NumFormatting << tm.tm_sec;

			s << m_pBorders[1];
			return s.str();
		}

		string CTimeStamp::_GetString(Tag::MM_dd_tt_HH_mm_ss) const
		{
			std::stringstream s;

			//get time values
			const Tick tt = Clock::to_time_t(m_tick);
			tm tm;
			localtime_s(&tm, &tt);

			s << m_pBorders[0];

			//MM
			s << _NumFormatting << tm.tm_mon; s << m_pRecodeSplit[0];

			//dd
			s << _NumFormatting << tm.tm_mday; s.put(' ');

			//tt
			s << ((tm.tm_hour < 12) ? ("AM") : ("PM")); s.put(' ');

			//HH
			s << _NumFormatting << tm.tm_hour; s << ":";

			//mm
			s << _NumFormatting << tm.tm_min; s << ":";

			//s
			s << _NumFormatting << tm.tm_sec;

			s << m_pBorders[1];
			return s.str();
		}

		string CTimeStamp::_GetString(Tag::tt_HH_mm_ss) const
		{
			std::stringstream s;

			//get time values
			const Tick tt = Clock::to_time_t(m_tick);
			tm tm;
			localtime_s(&tm, &tt);

			s << m_pBorders[0];

			//tt
			s << ((tm.tm_hour < 12) ? ("AM") : ("PM")); s.put(' ');

			//HH
			s << _NumFormatting << tm.tm_hour; s << ":";

			//mm
			s << _NumFormatting << tm.tm_min; s << ":";

			//s
			s << _NumFormatting << tm.tm_sec;

			s << m_pBorders[1];
			return s.str();
		}
	}
}