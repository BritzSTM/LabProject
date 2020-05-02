#pragma once


#include "EnvironmentValue.h"
#include "Common/BaseTypes.h"
#include "Type/TypeTraits.h"


namespace Platform
{
	/*
	  Operating System를 전처리문으로 정의 합니다.
	  정의 되는 상수 값은 uint8값에 포함되어야 합니다.
	  또한 정의된 값들을 이용해 TARGET_OS 매크로 값을 EnvironmentValue.h에 정의해야 합니다.
	*/
#   define OS_NONE    0x00
#   define OS_WIN32   0x01
#   define OS_WIN64   0x02
#   define OS_LINUX32 0x04
#   define OS_LINUX64 0x08

	/**Operating System에 대해 열거형으로 정의 합니다.*/
	enum class ECOS_TYPE : uint8
	{
		NONE_ = OS_NONE,    //분류되지 않는 타입 입니다.
		WIN32_ = OS_WIN32,
		WIN64_ = OS_WIN64,
		LINUX32_ = OS_LINUX32,
		LINUX64_ = OS_LINUX64
	};

	/**Target Operating System을 반환 합니다.*/
	COMPILE_TIME_FUNCTION(ECOS_TYPE GetOSType())
	{
		switch (TARGET_OS)
		{
		case OS_WIN32:
			return ECOS_TYPE::WIN32_;

		case OS_WIN64:
			return ECOS_TYPE::WIN64_;

		case OS_LINUX32:
			return ECOS_TYPE::LINUX32_;

		case OS_LINUX64:
			return ECOS_TYPE::LINUX64_;

		default:
			return ECOS_TYPE::NONE_;
		}
	}


	extern "C"
	{
		/**ECOS_TYPE 열거형을 전달 받아 '\0'으로 끝나는 문자열 값을 반환 합니다.*/
		const char* GetOSTypeToChars(const ECOS_TYPE type) noexcept;
	}

	/**
	운영체제에 종류에 따른 tag 생성
	운영체제에 종속되는 클래스 제작을 할때 사용하면 전처리기를 사용 안하고
	클래스 인스턴스화를 도울 수 있다.
	*/
#if (TARGET_OS == OS_WIN64)
	struct OSWindowsTag : std::true_type {};
	struct OSLinuxTag : std::false_type {};

#elif (TARGET_OS == OS_LINUX64)
	struct OSWindowsTag : std::false_type {};
	struct OSLinuxTag : std::true_type {};

#else
	struct OSWindowsTag : std::false_type {};
	struct OSLinuxTag : std::false_type {};

#endif



#   undef OS_NONE
#   undef OS_WIN32
#   undef OS_WIN64
#   undef OS_LINUX32
#   undef OS_LINUX64
}
