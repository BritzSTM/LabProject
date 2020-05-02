/**
@file EnviromentValue.cpp
@brief 
환경변수들을 단 한번만 컴파일 출력 정보창에 
출력하기 위해 만들어진 파일
@date 2018-10-11  오후 17:16:31
*/


#include "stdafx.h"
#include "EnvironmentValue.h"

#pragma warning(disable:4221)
COMPILE_INFO(CompilerType : SELECTED_COMPILER);
COMPILE_INFO(TargetArch :TARGET_ARCH);
COMPILE_INFO(TargetOS : TARGET_OS);
COMPILE_INFO(UTF8 CodePage : UTF8_CODE_PAGE);
COMPILE_INFO(MouseDefaultCursorID : SELECTED_MOUSE_DEFAULT_CURSOR);

void __DisableLinkDummy()
{

}