/**
@file MemoryUtil.h
@brief Memory관련 유틸리티 함수를 구현합니다.
@date 2018-10-12  오후 23:18:16
*/
#pragma once


//user
#include "EnvironmentValue.h"

namespace Core
{
	extern "C"
	{
		/**MemoryByte ordering이 BigEndiand인지 확인합니다.*/
		API bool IsBigEndian() noexcept;

		/**MemoryByte ordering이 LittleEndiand인지 확인합니다.*/
		API bool IsLittleEndian() noexcept;
	}


}