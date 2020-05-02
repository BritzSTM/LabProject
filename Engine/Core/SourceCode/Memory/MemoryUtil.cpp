/**
@file MemoryUtil.cpp
@brief MemoryUtil.h 구현 
@date 2018-10-12  오후 23:21:41
*/
#include "stdafx.h"
#include "MemoryUtil.h"
#include "Common/BaseTypes.h"


namespace Core
{

	bool IsBigEndian() noexcept
	{
		const int32 n = 0x1234'5678;
		const char* p = (const char*)&n;

		return (*p == 0x12);
	}

	bool IsLittleEndian() noexcept
	{
		return !(IsBigEndian());
	}

}
