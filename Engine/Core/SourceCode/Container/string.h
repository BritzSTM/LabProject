/**
@file string.h
@brief Core에서 사용할 문자열 관련된 것을 정의 합니다.
@remarks
다음과 같은 매크로에 따라 사용할 컨테이너를 정의 합니다.
SELECTED_STRING_CONTAINER = STL_STRING
SELECTED_STRING_CONTAINER = CORE_STRING

@date 2018-10-11  오후 19:04:24
*/
#pragma once


//user
#include "../CoreHelper.h"
#include "../EnvironmentValue.h"
#include "../Common/BaseTypes.h"
#include "../Type/TypeTraits.h"
#include "../Hash/Hash.h"
#include "../Util/TextLiteral.h"

//std
#include <string_view>
#include <sstream>
#include <cassert>


namespace Core
{
	namespace Container
	{
		namespace String
		{

#if   SELECTED_STRING_CONTAINER == STL_STRING
			/*  STL 기본 문자열을 사용합니다.*/
			using string = std::string;
			using wstring = std::wstring;
            using string_view = std::string_view;


#elif SELECTED_STRING_CONTAINER == CORE_STRING
			COMPILE_ERROR("Not impl CoreString");
#else
			COMPILE_ERROR("SELECTED_STRING_CONTAINER not defined");
#endif


			const char* const EmptyString = "";


			/**
			@brief 
			문자열을 Compile Time에 HashValue로 변환 합니다.
		    비교 발생시 문자열 비교가 아닌 HashValue로 비교를 진행합니다.
			@date 2018-10-11  오후 19:15:38
			*/
			struct SCompileTimeHashFromString
			{
				template <size_t N>
				constexpr SCompileTimeHashFromString(const char(&strArr)[N])
					: pszString(strArr)
					, HashValue(Hash::EncodeHash(strArr))
				{

				}

				COMPILE_TIME_FUNCTION(operator Hash::HashValue())
				{
					return HashValue;
				}

				template<typename _TYPE>
				COMPILE_TIME_FUNCTION(bool operator==(const _TYPE& rhs) const)
				{
					return HashValue == rhs;
				}

				template<typename _TYPE>
				COMPILE_TIME_FUNCTION(bool operator!=(const _TYPE& rhs) const)
				{
					return !(operator==(rhs));
				}
				
				template<>
				COMPILE_TIME_FUNCTION(bool operator==<SCompileTimeHashFromString>(
					const SCompileTimeHashFromString& rhs) const)
				{
					return HashValue == rhs.HashValue;
				}

				template<>
				COMPILE_TIME_FUNCTION(bool operator!=<SCompileTimeHashFromString>(
					const SCompileTimeHashFromString& rhs) const)
				{
					return !(operator==(rhs));
				}

				COMPILE_TIME_FUNCTION(bool operator<(const SCompileTimeHashFromString& rhs) const)
				{
					return HashValue < rhs.HashValue;
				}

				const char* pszString;
				Hash::HashValue HashValue;
			};


			static_assert(std::is_trivially_copyable<SCompileTimeHashFromString>::value, "CompileTimeHashFromString is not copyable");


#define SWITCH_STRING(_VALUE_) switch(_VALUE_.HashValue)

/**switch 문에서 문자열 Hash 사용하는 Macro */
#define CASE_STRING(_STR_) \
  __pragma( warning( push ) ) __pragma( warning( disable: 4307 ) ) \
case SCompileTimeHashFromString(_STR_).HashValue __pragma( warning( pop ) )





			/**
			@brief 
			문자열을 RealTime에 HashValue로 변환 합니다.
			비교 발생시 문자열 비교가 아닌 HashValue로 비교를 진행합니다.
			@date 2018-10-12  오후 13:23:41
			*/
			struct SHashFromString
			{
				SHashFromString(const char* pszStr)
					: pszString(pszStr)
					, HashValue(Hash::EncodeHash(pszStr))
				{

				}

				operator Hash::HashValue() const noexcept
				{
					return HashValue;
				}

				template<typename _TYPE>
				bool operator==(const _TYPE& rhs) const noexcept
				{
					return HashValue == rhs;
				}

				template<typename _TYPE>
				bool operator!=(const _TYPE& rhs) const noexcept
				{
					return !(operator==(rhs));
				}

				template<>
				bool operator==(const SHashFromString& rhs) const noexcept
				{
					return HashValue == rhs.HashValue;
				}

				template<>
				bool operator!=(const SHashFromString& rhs) const noexcept
				{
					return !(operator==(rhs));
				}

				bool operator<(const SHashFromString& rhs) const noexcept
				{
					return (HashValue < rhs.HashValue);
				}

				const char* pszString;
				Hash::HashValue HashValue;
			};

			static_assert(std::is_trivially_copyable<SHashFromString>::value, "SHashFromString is not copyable");

			FLINE bool operator==(const SCompileTimeHashFromString& lhs, const SHashFromString& rhs)
			{
				return lhs.HashValue == rhs.HashValue;
			}

			FLINE bool operator==(const SHashFromString& lhs, const SCompileTimeHashFromString& rhs)
			{
				return lhs.HashValue == rhs.HashValue;
			}

			FLINE bool operator!=(const SCompileTimeHashFromString& lhs, const SHashFromString& rhs)
			{
				return !(operator==(lhs, rhs));
			}

			FLINE bool operator!=(const SHashFromString& lhs, const SCompileTimeHashFromString& rhs)
			{
				return !(operator==(lhs, rhs));
			}


			/**
			@brief 객체를 문자열로 표현하는 인터페이스
			@date 2018-10-12  오후 13:47:28
			*/
			struct IToString
			{
				DISALLOW_CREATE_INSTANCE(IToString);
				virtual ~IToString() {}

				virtual string ToString() const = 0;
			};


			/**
			@brief 
			유니코드 문자열에 대한 연산을 하는 클래스. 
			모든 메소드는 정적이다.
			@date 2018-10-12  오후 13:48:57
			*/
			class API Unicode final
			{
			public:
				static string ToUTF8(const wstring& str, uint32 codePage = UTF8_CODE_PAGE);

			private:
				Unicode() = delete;
				Unicode(const Unicode&) = delete;
				~Unicode() = delete;
			};


			/**
			@brief
			UTF8 문자열에 대한 연산을 하는 클래스.
			모든 메소드는 정적이다.
			@date 2018-10-12  오후 13:49:01
			*/
			class API UTF8 final
			{
			public:
				static wstring ToUNICODE(const string& str, uint32 codePage = UTF8_CODE_PAGE);

			private:
				UTF8() = delete;
				UTF8(const UTF8&) = delete;
				~UTF8() = delete;
			};
			

			////////////////////
			//Utility Functions
			////////////////////


			FLINE const char* AppendsCharsToString()
			{
				return EmptyString;
			}

			/*
			Chars 문자열들을 모아 string형으로 합쳐서 반환한다.
			많은 문자열 수는 부하가 크지만 적당한 1~4는 크지 않을 것이다.
			*/
			template<typename ARG_TYPE, typename..._ARGS_TYPE>
			ILINE string AppendsCharsToString(ARG_TYPE& arg, _ARGS_TYPE&&...args)
			{
				assert(arg != nullptr && TEXTL("AppendsCharsToString arg is nullptr"));

				std::stringstream stream;
				stream << arg;
				stream << AppendsCharsToString(std::forward<_ARGS_TYPE>(args)...);
				return stream.str();
			}


            //trim
            static ConstexprString(s_kpszDefaultTrimChars, " \t\f\v\n\r");

            ILINE string& LTrim(string& str, const char* const pszTrimChars = s_kpszDefaultTrimChars)
            {
                size_t findPos = str.find_first_not_of(pszTrimChars);
                
                if (findPos != string::npos)
                {
                    str.erase(0, findPos);
                }

                return str;
            }

            ILINE string& RTrim(string& str, const char* const pszTrimChars = s_kpszDefaultTrimChars)
            {
                size_t findPos = str.find_last_not_of(pszTrimChars);
               
                if (findPos != std::string::npos)
                {
                    str.erase(findPos + 1);
                }

                return str;
            }

            ILINE string& Trim(string& str, const char* const pszTrimChars = s_kpszDefaultTrimChars)
            {
                LTrim(str, pszTrimChars);
                RTrim(str, pszTrimChars);
                return str;
            }

			std::vector<string> SplitStringToVector(const string srcStr, const string delimiter);
			ILINE std::vector<string> SplitStringToVector(const char* const pszString, const char* const pszDelimiter)
			{
				return SplitStringToVector(string(pszString), string(pszDelimiter));
			}

            /**stringstream을 지원하는 문자열의 vector집합을 하나의 문자열로 결합 합니다.*/
            template<typename _STR_TYPE_>
            string VectorToString(const std::vector<_STR_TYPE_>& vecSrc, const char* const pszDelimiter)
            {
                const char* const kpszFilteredDelimiter = 
                    (pszDelimiter == nullptr) ? (EmptyString) : (pszDelimiter);

                std::stringstream s;
                for (const auto& elem : vecSrc)
                {
                    s << elem;
                    s << kpszFilteredDelimiter;
                }

                return s.str();
            }
			
		}
	}
}

/**
  to_string bool type specialization
*/
namespace std
{
	FLINE std::string to_string(bool value)
	{
		return (value) ? ("true") : ("false");
	}
}

/**
std hash 특수화
*/
namespace std
{
	template <typename _Kty>
	struct hash;

	template<>
	struct hash<Core::Container::String::SCompileTimeHashFromString>
	{
		COMPILE_TIME_FUNCTION(std::size_t operator()(
			const Core::Container::String::SCompileTimeHashFromString& _Keyval) const)
		{
			return _Keyval.HashValue;
		}
	};

	template<>
	struct hash<Core::Container::String::SHashFromString>
	{
		COMPILE_TIME_FUNCTION(std::size_t operator()(
			const Core::Container::String::SHashFromString& _Keyval) const)
		{
			return _Keyval.HashValue;
		}
	};
}


/**
tbb hash 특수화
*/
namespace tbb
{
	template <typename Key>
	class tbb_hash;

	template <>
	class tbb_hash<Core::Container::String::SCompileTimeHashFromString>
	{
	public:
		COMPILE_TIME_FUNCTION(std::size_t operator()(
			const Core::Container::String::SCompileTimeHashFromString& _Keyval) const)
		{
			return _Keyval.HashValue;
		}
	};

	template <>
	class tbb_hash<Core::Container::String::SHashFromString>
	{
	public:
		COMPILE_TIME_FUNCTION(std::size_t operator()(
			const Core::Container::String::SHashFromString& _Keyval) const)
		{
			return _Keyval.HashValue;
		}
	};
	
}


//해당파일을 포함하면 네임스페이스 선언을 안하고 사용 가능하다.
using namespace Core::Container::String;


//Not Moveing Header File
#include "FixString.h"