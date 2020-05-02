#include "stdafx.h"
#include "StackTracer.h"

#include <filesystem>


namespace Core
{
   
    std::vector<string> CStackTracer::GetStackFrameListToVector(const char* const pszFormat)
    {
        using namespace std;

        //0이 StackFrame 최상단을 의미함.
        const CStackTraceCore traceCore;
        const auto vecResource = _GetFormattingResource(pszFormat);
        size_t startIndex = _FindStackFrameStartIndex(__FUNCTION__, traceCore);
        vector<string> resList;
        stringstream strBuf;

        for (; startIndex < traceCore.size(); ++startIndex)
        {
            strBuf.str("");

            for (const auto& formatType : vecResource)
            {
                strBuf << _GetStackFrameValueFromFormat(
                    formatType.first.c_str(), traceCore, startIndex);
            }

            resList.push_back(strBuf.str());
        }

        return std::move(resList);
    }

    CArgumentRefineryForFormatter::VecContainer 
    CStackTracer::_GetFormattingResource(const char* const pszFormat)
    {
        CArgumentRefineryForFormatter refinery({ kpszArgumentTypeSimbol, nullptr });
        
        return std::move(refinery.GetRefinedArgumentsButUnknownMixed(s_argFormatTypes, pszFormat));
    }

    string CStackTracer::_GetStackFrameValueFromFormat(
        const SHashFromString& format,
        const CStackTraceCore& stackTraceCore,
        const size_t& index) noexcept
    {
        using namespace std::filesystem;

        static bool securedMem = false;
        const auto& frame = stackTraceCore[index];

        //Mem 재확보시 재진입 구역
    SECURED_MEM_START:
        try
        {
            SWITCH_STRING(format)
            {
                CASE_STRING("%FUNC_NAME") :
                    return frame.name();
                break;

                CASE_STRING("%STACK_ADDR") :
                    return std::to_string(frame.address());
                break;

                CASE_STRING("%STACK_ORDER") :
                    return std::to_string(index);
                break;

                CASE_STRING("%FILE_PATH") :
                    return (path(frame.source_file()).parent_path().u8string());
                break;

                CASE_STRING("%FILE_NAME") :
                    return (path(frame.source_file()).filename().u8string());
                break;

                CASE_STRING("%FILE_LINE") :
                    return std::to_string(frame.source_line());
                break;

                default:
                    //일치하지 않는다면 format문자열 그대로 나감
                    return format.pszString;
                break;
            }
        }
        //메모리 부족
        catch (std::bad_alloc&)
        {
            if (!securedMem)
            {
                securedMem = true;
                goto SECURED_MEM_START;
            }
        }
        //기타 예외
        catch (std::exception& e)
        {
            return _GetFormattingExceptionString(format, e);
        }       
        
        //비접근 영역
        return "[! InaccessibleAreaException (GetStackFrameValueFromFormat) !]";
    }

    size_t CStackTracer::_FindStackFrameStartIndex(
        const char* const pszCallerFuncFullName, 
        const CStackTraceCore& stackTraceCore)
    {
        size_t findIndex = 0;

        for (const auto& frame : stackTraceCore)
        {
            if (frame.name() == pszCallerFuncFullName)
            {
                break;
            }
            else
            {
                ++findIndex;
            }
        }

        //Caller를 제외해야 되기 때문에 + 1
        return findIndex + 1;
    }


}