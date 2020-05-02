/**
@file BaseTypes.h
@brief 기본 원시 데이터 형에 대해 정의 합니다.
@date 2018-10-11  오후 18:52:30
*/

#pragma once


#include "EnvironmentValue.h"
#include "BaseType_Integers.h"
#include "BaseType_Floatings.h"
#include "Platform/WrappedDependentBaseType.h"


namespace Core
{
	namespace BaseTypes
	{
		//Byte형 정의
		using byte = unsigned char;
	}
}


/*
  유일하게 size_t를 사용하기 위해서 포함시키는 헤더파일
  다른 타입은 사용하지 않기
*/
#include <cstdint>
using namespace Core::BaseTypes;

/**상수형 문자열을 정의 합니다.*/
#define ConstexprString(NAME, VALUE) constexpr const char* NAME = VALUE