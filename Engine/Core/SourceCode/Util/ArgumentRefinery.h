#pragma once


#include "CoreHelper.h"
#include "Container/string.h"

#include <xutility>


namespace Core
{

    struct SDelimiterInfo
    {
        const char* MainDelimiters;
        const char* SubDelminiters;
    };

   
    namespace _internal
    {
        class CArgumentRefineryBase
        {
        public:
            using SArgumentPair = std::pair<string, string>; //first = ArgumentType, second = Data for argument
            using VecContainer = std::vector<SArgumentPair>;

        protected:
            ~CArgumentRefineryBase()
            {

            }

            CArgumentRefineryBase(const SDelimiterInfo& info) noexcept
                : m_delimiterInfo(info)
            {

            }
            
        protected:
            //Delimiter정보를 획득합니다.
            ILINE const SDelimiterInfo& _GetDelimiterInfo() const noexcept
            {
                return m_delimiterInfo;
            }

            //format이 발견된다면 pszFormat의 길이가 반환됩니다. 발견되지 못하면 npos가 반환됩니다.
            static size_t _FindFormatNextPos(const std::string_view& pszFormat, const std::string_view& pszStr) noexcept
            {
                if (pszFormat.length() > pszStr.length())
                {
                    return std::string_view::npos;
                }

                size_t subCount = 0;
                for (const auto& elem : pszFormat)
                {
                    if (elem != pszStr[subCount++])
                    {
                        return std::string_view::npos;
                    }
                }

                return subCount;
            }

            /**
            _FUNC_에는 3가지의 전달인자가 전달됩니다.
            const char* filterType, subDelimiter, string_view& mixedStr
            _FUNC_는 3가지의 정보를 가지고 분리할 기준 위치를 반환해야 합니다.
            */
            template <size_t _ARR_SIZE_, typename _FUNC_>
            ILINE SArgumentPair __SplitArgumentBase(
                const std::array<const char*, _ARR_SIZE_>& arrFilterTypes,
                const string_view& mixedStr,
                const _FUNC_& findPosFunc) const
            {
                size_t findPos;
                for (const char* filterType : arrFilterTypes)
                {
                    findPos = findPosFunc(filterType, m_delimiterInfo.SubDelminiters, mixedStr);

                    if (findPos != string_view::npos)
                    {
                        //Argument, Others
                        return make_pair(string(mixedStr.substr(0, findPos)), string(mixedStr.substr(findPos)));
                    }
                }

                //Only Argument
                return make_pair(string(mixedStr), string(""));
            }

            /**
            arrArgumentTypes, SplittedStrToMainDelmiter, vecRes 인자가 전달됩니다.
            _FUNC_는 이를 이용해 추가적인 분리 작업후 vecRes에 저장해야 합니다.
            */
            template <size_t _ARR_SIZE_, typename _FUNC_>
            VecContainer _GetRefinedArgumentsBase(
                const std::array<const char*, _ARR_SIZE_>& arrArgumentTypes,
                const char* const pszDataToFilter,
                const _FUNC_ splitFunc) const
            {
                using namespace std;
                const size_t len = strlen(m_delimiterInfo.MainDelimiters);

                VecContainer vecRes;
                string_view sourceStr(pszDataToFilter);

                size_t findPos = sourceStr.find(m_delimiterInfo.MainDelimiters);
                while (findPos != string_view::npos)
                {
                    //2차 정제
                    string_view mixedStr = sourceStr.substr(0, findPos);
                    splitFunc(arrArgumentTypes, mixedStr, vecRes);

                    //다음 문자열 진행
                    sourceStr = sourceStr.substr(findPos);
                    findPos = sourceStr.find(m_delimiterInfo.MainDelimiters, len);
                }

                //남은 요소 정리
                splitFunc(arrArgumentTypes, sourceStr, vecRes);
                return vecRes;
            }

        private:
            const SDelimiterInfo m_delimiterInfo;

        };

    }

    /**
    Maindelimiter를 이용해 문자열을 Argument로 정제합니다.
    */
    class CArgumentRefineryForFormatter final
        : public _internal::CArgumentRefineryBase
    {
    public:
        ~CArgumentRefineryForFormatter()
        {

        }

        CArgumentRefineryForFormatter(const SDelimiterInfo& info) noexcept
            : CArgumentRefineryBase(info)
        {

        }

        /**
        전달된 데이터를 ArgumentTypes로 정제합니다.
        문자열 순서대로 정재된 결과값이 산출되며 SArgumentPair.first만 유효합니다.
        전달된 각각의 arrArgumentType 문자열 앞에는 MainDelimiterSimbol이 있어야 합니다.
        */
        template <size_t _ARR_SIZE_>
        VecContainer GetRefinedArgumentsButUnknownMixed(
            const std::array<const char*, _ARR_SIZE_>& arrArgumentTypes,
            const char* const pszDataToFilter) const
        {
            _IsAvailable();

            return _GetRefinedArgumentsBase(arrArgumentTypes, pszDataToFilter,
                [this](const auto& arrArgumentTypes, const auto& mixedStr, auto& resStorage)
            {
                SArgumentPair pair = _SplitArgumentAndUnknown(arrArgumentTypes, mixedStr);

                if (pair.first.length() > 0)
                {
                    resStorage.push_back(make_pair(pair.first, ""));
                }

                if (pair.second.length() > 0)
                {
                    resStorage.push_back(make_pair(pair.second, ""));
                }

            });
        }

    private:
        ILINE void _IsAvailable() const
        {
            if (_GetDelimiterInfo().MainDelimiters == nullptr)
            {
                throw std::logic_error("MainDelimiters is nullptr");
            }
        }

        template <size_t _ARR_SIZE_>
        SArgumentPair _SplitArgumentAndUnknown(
            const std::array<const char*, _ARR_SIZE_>& arrFilterTypes,
            const string_view& mixedStr) const
        {
            return __SplitArgumentBase(arrFilterTypes, mixedStr,
                [](const char* const filterType, const char* const subDelimiters, const string_view& mixedStr)
            {
                return _FindFormatNextPos(filterType, mixedStr);
            });
        }

    };


    class CArgumentRefineryForParser final
        : public _internal::CArgumentRefineryBase
    {
    public:
        ~CArgumentRefineryForParser()
        {

        }

        CArgumentRefineryForParser(const SDelimiterInfo& info) noexcept
            : CArgumentRefineryBase(info)
        {

        }

        /**
        Maindelimiter를 이용해 문자열을 Argument로 분리하고
        SubDelimiter를 이용해 Data를 분리합니다.
        */
        template <size_t _ARR_SIZE_>
        VecContainer GetRefinedArgumentsAndData(
            const std::array<const char*, _ARR_SIZE_>& arrArgumentTypes,
            const char* const pszDataToFilter) const
        {
            _IsAvailable();

            return _GetRefinedArgumentsBase(arrArgumentTypes, pszDataToFilter,
                [this](const auto& arrArgumentTypes, const auto& mixedStr, auto& resStorage)
            {
                SArgumentPair pair = _SplitArgumentAndData(arrArgumentTypes, mixedStr);

                if (pair.first.length() > 0)
                {
                    resStorage.push_back(pair);
                }
            });
        }

    private:
        ILINE void _IsAvailable() const
        {
            if (_GetDelimiterInfo().MainDelimiters == nullptr)
            {
                throw std::logic_error("MainDelimiters is nullptr");
            }

            if (_GetDelimiterInfo().SubDelminiters == nullptr)
            {
                throw std::logic_error("SubDelminiters is nullptr");
            }
        }

        template <size_t _ARR_SIZE_>
        SArgumentPair _SplitArgumentAndData(
            const std::array<const char*, _ARR_SIZE_>& arrFilterTypes,
            const string_view& mixedStr) const
        {
            SArgumentPair pair = __SplitArgumentBase(arrFilterTypes, mixedStr,
                [](const char* const filterType, const char* const subDelimiters, const string_view& mixedStr)
            {
                return mixedStr.find(subDelimiters);
            });

            //모든 Data 시작부분에 subDelimiter가 붙음 따라서 해당 부분을 제거한다. 데이터 뒷부분의 trim도 진행
            pair.second.erase(0, std::strlen(m_delimiterInfo.SubDelminiters));
            rtrim(pair.second);
            return pair;
        }

    };

}