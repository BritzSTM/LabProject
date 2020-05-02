
#pragma once

#include "EnvironmentValue.h"
#include "CoreHelper.h"

DISABLE_HEADER_FILE;

/**
환경 값에 따라 Container의 예외 던지기 여부를 결정하는 CONTAINER_THROW(X) Macro 함수를 정의
SELECTED_CONTAINER_EXCEPTION 모드는 예외를 발생
ELECTED_CONTAINER_EXCEPTION_NOT_THROW 모드는 false를 반환

만들어지는 Container 성격의 Object는 특별한 경우가 아니면 모두 사용을 강제할 것
*/
#if defined(SELECTED_CONTAINER_EXCEPTION)
#   define CON_EXCEPTION_THROWER(EXCEPTION_TYPE, TEXT_MESSAGE) \
throw THROW_OBJ;\
static_assert(std::is_base_of<std::exception, THROW_OBJ>::value,\
 TEXTL("Exception object is not based on std::exception"))

#elif defined(SELECTED_CONTAINER_EXCEPTION_NOT_THROW)
#   define CONTAINER_THROW(THROW_OBJ) return false;

#else
    COMPILE_ERROR(TEXTL("Container Exception Mode is not defined"));
#endif