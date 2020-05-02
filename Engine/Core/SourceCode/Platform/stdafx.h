/**
@file stdafx.h 
자주 사용하지만 자주 변경되지는 않는 
표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
들어 있는 포함 파일입니다.
@date 2018-10-13  오후 20:28:35
*/


#pragma once


///////////////////////////////
// Platform에 독립된 헤더 파일
///////////////////////////////

//STL
#include <array>
#include <vector>
#include <list>
#include <string>
#include <algorithm>

//Stream
#include <iomanip>
#include <fstream>
#include <ostream>
#include <sstream>

//Time
#include <ctime>
#include <chrono>

//Etc
#include <memory>
#include <random>
#include <filesystem>



//컴파일 환경에 따른 미리컴파일된 헤더 포함
#if defined(_WIN64) || defined(WIN64)

//Windows 64Bit
#include"X64\Windows\stdafx_win.h"

#elif defined(_LINUX64) || defined(LINUX64)
//Linux 64Bit

#else
//Error Platform

#endif

#include "CoreHelper.h"
