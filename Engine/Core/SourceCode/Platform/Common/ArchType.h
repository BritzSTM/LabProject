#pragma once


#include "EnvironmentValue.h"
#include "Common/BaseTypes.h"


namespace Platform
{

	/*
	  Architecture를 전처리문으로 정의 합니다.
	  정의 되는 상수 값은 uint8값에 포함되어야 합니다.
	  또한 정의된 값들을 이용해 TARGET_ARCH 매크로 값을 EnvironmentValue.h에 정의해야 합니다.
	*/
#   define ARCH_NONE  0x00
#   define ARCH_X32   0x01
#   define ARCH_X64   0x02
#   define ARCH_ARMv8 0x04


	/**Architecture에 대해 열거형으로 정의 합니다.*/
	enum class ECARCH_TYPE : uint8
	{
		NONE  = ARCH_NONE,  //분류되지 않는 타입 입니다.
		X32   = ARCH_X32,
		X64   = ARCH_X64,
		ARMv8 = ARCH_ARMv8
	};

	/**Target Architecture를 반환 합니다.*/
	COMPILE_TIME_FUNCTION(ECARCH_TYPE GetArchitectureType())
	{
		switch (TARGET_ARCH)
		{
		case ARCH_X32:
			return ECARCH_TYPE::X32;

		case ARCH_X64:
			return ECARCH_TYPE::X64;

		case ARCH_ARMv8:
			return ECARCH_TYPE::ARMv8;

		default:
			return ECARCH_TYPE::NONE;
		}
	}

	extern "C"
	{
		/**
		@brief ECARCH_TYPE을 전달 받아 해당 열거형을 문자열 값으로 표현 합니다.
		@param type
		@return '\0'으로 끝나는 문자열 값
		@exception 없음
		@version 1.0v
		*/
		const char* GetArchitectureTypeToChars(const ECARCH_TYPE type) noexcept;
	}
}
