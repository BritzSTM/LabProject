/**
@file mutex
@brief
tbb, std mutex의 wrapping
tbb의 RAII과 std RAII는 차이가 있음. Core에 있는 mutex는 tbb라는걸 명심
@date 2018-10-14  오전 00:46:55
*/

#pragma once


#include <tbb/mutex.h>
#include <tbb/compat/condition_variable>


#include <mutex>


namespace Core
{
	using tbb::mutex;
	using namespace tbb::interface5;
}