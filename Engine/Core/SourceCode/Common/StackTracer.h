/**
테스트가 필요하며 테스트시 메모리 점유율 확인이 필요함. 
호출시 boost의 함수 호출시 메모리 점유율이 극적으로 늘어나는 현상이 발견됨
*/
#pragma once


#include "CoreHelper.h"
#include "Common/BaseTypes.h"
#include "Container/string.h"
#include "Memory/MemoryBlock.h"
#include "Util/ArgumentRefinery.h"

#include <boost/stacktrace.hpp>


namespace Core
{

    /**
    사용 가능한 Format 인자
    %FUNC_NAME      : Function name
    %STACK_ADDR     : Stackframe address
    %STACK_ORDER    : Stackframe elemet order number
    %FILE_PATH      : File abs path
    %FILE_NAME      : File name
    %FILE_LINE      : File line number

    CStackTracer에서 제공하는 모든 Method는 예외가 발생하지 않습니다.(아직은 아님)
    */
    class CStackTracer final
    {
        DISALLOW_CREATE_INSTANCE_PERMANENTLY(CStackTracer);

    public:
        using CStackFrame = boost::stacktrace::frame;
        using CStackTraceCore = boost::stacktrace::stacktrace;

    public:
        /**호출한 함수를 포함한 StackFrameList를 Vector로 반환합니다.*/
        static std::vector<string> GetStackFrameListToVector(const char* const pszFormat = kpszDefaultPrintFormat);

        /**
        호출한 함수를 포함한 StackFrameList를 문자열로 반환합니다.
        각 요소의 구분을 위한 Delimiter를 전달 받습니다.
        */
        ILINE static string GetStackFrameList(
            const char* const pszFormat = kpszDefaultPrintFormat,
            const char* const pszDelimiterSimbol = kpszDefaultDelimiters)
        {
            return VectorToString(GetStackFrameListToVector(pszFormat), pszDelimiterSimbol);
        }
        
    private:
        static CArgumentRefineryForFormatter::VecContainer
            _GetFormattingResource(const char* const pszFormat);

        static string _GetStackFrameValueFromFormat(
            const SHashFromString& format, 
            const CStackTraceCore& stackTraceCore,
            const size_t& index) noexcept;

        static size_t _FindStackFrameStartIndex(
            const char* const pszCallerFuncFullName, 
            const CStackTraceCore& stackTraceCore);

        static ILINE string _GetFormattingExceptionString(
            const SHashFromString& format,
            const std::exception& e)
        {
            std::stringstream buf;

            buf << '[! ';
            buf << e.what();
            buf << " FormatType : ";
            buf << format.pszString;
            buf << ' !]';

            return buf.str();
        }

    public:
        static ConstexprString(kpszArgumentTypeSimbol, "%");
        static constexpr std::array<const char*, 6> s_argFormatTypes =
        {
            "%FUNC_NAME","%STACK_ORDER", "%STACK_ADDR",
            "%FILE_PATH", "%FILE_NAME", "%FILE_LINE"
        };

        static ConstexprString(kpszDefaultDelimiters, "\n\r");
        static ConstexprString(kpszDefaultPrintFormat, "[%STACK_ORDER] %FUNC_NAME %FILE_PATH%FILE_NAME(%FILE_LINE)");
        static ConstexprString(kpszDefaultPrintShortFormat, "[%STACK_ORDER] %FUNC_NAME %FILE_NAME(%FILE_LINE)");
        static ConstexprString(kpszDefaultNotFoundFileString, "[Not Found File]");
                
    };

}