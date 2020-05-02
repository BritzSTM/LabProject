/**
@file CriticalSection.h
@brief CriticalSection객체에 대해 정의 합니다.
@date 2018-10-13  오전 09:45:59
*/


#pragma once


#include "Platform/Common/OSType.h"


//OS Type에 따른 정의
#if (TARGET_OS == OS_WIN64) || (TARGET_OS == OS_WIN32)
#include <synchapi.h>

namespace Core
{
	/**
	@brief CriticalSection
	@date 2018-10-15  오전 11:06:52
	@version 1.1
	@todo 기본버전의 복제생성자, 대입연산자 삭제완료
	*/
	class API CriticalSection final
	{
	public:
		ILINE ~CriticalSection() noexcept
		{
			DeleteCriticalSection(&m_cs);
		}

		ILINE CriticalSection()
		{
			InitializeCriticalSection(&m_cs);
		}

		CriticalSection(const CriticalSection&) = delete;
		CriticalSection& operator=(const CriticalSection&) = delete;

		ILINE void Enter()
		{
			EnterCriticalSection(&m_cs);
		}

		ILINE void Leave()
		{
			LeaveCriticalSection(&m_cs);
		}

		ILINE bool TryEnter()
		{
			return TryEnterCriticalSection(&m_cs);
		}

	private:
		CRITICAL_SECTION m_cs;
	};
}

#else
    COMPILE_ERROR("NOT IMPL CRITICAL_SECTION");
#endif


namespace std
{
	template<class _Mutex>
	class lock_guard;

	/**
	@brief lock_guard로 CriticalSection을 사용할 수 있도록 하기 위한 특수화
	@date 2018-10-15  오전 11:06:52
	@version 1.01v
	*/
	template<>
	class lock_guard<Core::CriticalSection>
	{
	public:
		using mutex_type = Core::CriticalSection;

	public:
		~lock_guard() noexcept
		{
			m_cs.Leave();
		}

		explicit lock_guard(Core::CriticalSection& cs)
			: m_cs(cs)
		{
			m_cs.Enter();
		}

		/*
		not used
		lock_guard(Core::CriticalSection& cs, adopt_lock_t)
			: m_cs(cs)
		{	

		}
		*/
		lock_guard(const lock_guard&) = delete;
		lock_guard& operator=(const lock_guard&) = delete;

	private:
		Core::CriticalSection& m_cs;

	};
}