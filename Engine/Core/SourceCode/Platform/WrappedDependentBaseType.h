/**
@file WrappedDependentBaseType.h
@brief 운영체제 종속적인 자료형들을 자동으로 전처리 include 하기 위한 헤더파일
@date 2018-10-22  오후 22:55:56
*/


#pragma once


#include "CoreHelper.h"


#if TARGET_OS == OS_WIN64
#include "X64/Windows/WinDependentBaseType.h"


#else
COMPILE_ERROR("Not impl dependentBaseType header");


#endif


#define WINDOWS_API __stdcall


namespace Platform
{

    union alignas(8) WindowContext
    {
    public:
        ILINE WindowContext& operator=(const WindowContext& rhs) noexcept
        {
            GUARD_SELF_ASSIGNMENT;
            window_context_data = rhs.window_context_data;
            return *this;
        }

        ILINE bool operator==(const __int64& value) noexcept
        {
            return(window_context_data == value);          
        }

#if TARGET_OS == OS_WIN64
    public:
        ILINE explicit WindowContext(const ::HWND hWnd) noexcept
            : windows_native_handle(hWnd)
        {

        }

        ILINE WindowContext& operator=(const ::HWND& rhs) noexcept
        {
            windows_native_handle = rhs;
            return *this;
        }

        ILINE operator ::HWND() const noexcept
        {
            return windows_native_handle;
        }

        ILINE explicit operator unsigned __int64() const noexcept
        {
            return window_context_data;
        }

    private:
        ::HWND windows_native_handle;
        static_assert(sizeof(::HWND) <= sizeof(unsigned __int64), 
            "WindowContext Mapping Error");

#else
        COMPILE_ERROR("Not impl WindowContext Type");

#endif

    private:
        unsigned __int64  window_context_data;
          
    };

    static_assert(sizeof(WindowContext) == 8, "WindowContext Size is not 8 byte");

}