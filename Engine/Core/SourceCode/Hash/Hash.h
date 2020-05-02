/**
@file Hash.h
@brief
Hash관련 구현한 파일
@remarks 
http://www.chrissavoie.com/articles/research/hash2 해쉬알고리즘 성능표
@date 2018-10-11  오후 18:27:46
*/
#pragma once


//user
#include "../CoreHelper.h"
#include "../EnvironmentValue.h"
#include "../Common/BaseTypes.h"

//std
#include <iosfwd>


#pragma warning(push)
#pragma warning(disable : 4307)
namespace Core
{
	namespace Hash
	{
		/*사용할 HashValue 크기*/
#define HASH32BIT 32
#define HASH64BIT 64

		/**32Bit HashValue Type 정의*/
		using HashValue32 = uint32;

		/**64Bit HashValue Type 정의*/
		struct HashValue64
		{
			union
			{
				uint64      Value;	/**64Bit Hash value*/
				HashValue32 HValue;	/**32bit 상위 값*/
				HashValue32 LValue;	/**32bit 하위 값*/
			};

			template<typename _TYPE>
			COMPILE_TIME_FUNCTION(bool operator==(const _TYPE& rhs) const)
			{
				return Value == rhs;
			}

			template<typename _TYPE>
			COMPILE_TIME_FUNCTION(bool operator!=(const _TYPE& rhs) const)
			{
				return !(operator==(rhs));
			}

			template<>
			COMPILE_TIME_FUNCTION(bool operator==<HashValue64>(const HashValue64& rhs) const)
			{
				return Value == rhs.Value;
			}

			template<>
			COMPILE_TIME_FUNCTION(bool operator!=<HashValue64>(const HashValue64& rhs) const)
			{
				return !(operator==(rhs));
			}


			COMPILE_TIME_FUNCTION(bool operator<(const HashValue64& rhs) const)
			{
				return Value < rhs.Value;
			}

			COMPILE_TIME_FUNCTION(operator std::size_t() const)
			{
				return static_cast<std::size_t>(Value);
			}
		};
		

		static_assert(std::is_pod<HashValue64>::value, "HashValue64 is not Pod Type");


		template <typename Type>
		struct GetHashValueType;

		template <>
		struct GetHashValueType<HashValue32>
		{
			using Type = uint32;
		};

		template <>
		struct GetHashValueType<HashValue64>
		{
			using Type = uint64;
		};

		/*
		  환경변수에 따른 HashValue Size 결정
		*/
#if SELECTED_CORE_HASH_SIZE == HASH32BIT
		using HashValue = HashValue32;
#elif SELECTED_CORE_HASH_SIZE == HASH64BIT
		using HashValue = HashValue64;
#else
    COMPILE_ERROR("SELECTED_CORE_HASH_SIZE not defined");
#endif


		template<typename _HASH_SiZE>
		COMPILE_TIME_FUNCTION(_HASH_SiZE _EncodeHashImpl(const char *pszStr))
		{
			size_t strLen = std::char_traits<char>::length(pszStr);

			_HASH_SiZE hashValue = 0;
			for (size_t i = 0; i < strLen; ++i)
			{
				hashValue = 65599 * hashValue + pszStr[i];
			}

			return hashValue ^ (hashValue >> 16);
		}

		extern "C"
		{
			/**
			@brief 컴파일 시간에 문자열 정보를 HashValue32로 변환 합니다.
			@param pszStr 끝값이 '\0' 문자열
			@return HashValue
			@version 1.0v
			*/
			COMPILE_TIME_FUNCTION(HashValue32 EncodeHash32(const char *pszStr))
			{
				return HashValue32{ _EncodeHashImpl<GetHashValueType<HashValue32>::Type>(pszStr) };
			}

			/**
			@brief 컴파일 시간에 문자열 정보를 HashValue64로 변환 합니다.
			@param pszStr 끝값이 '\0' 문자열
			@return HashValue
			@version 1.0v
			*/
			COMPILE_TIME_FUNCTION(HashValue64 EncodeHash64(const char *pszStr))
			{
				return HashValue64{ _EncodeHashImpl<GetHashValueType<HashValue64>::Type>(pszStr) };
			}

			/**
			@brief 컴파일 시간에 문자열 정보를 HashValue로 변환 합니다.
			@param pszStr 끝값이 '\0' 문자열
			@return HashValue
			@version 1.0v
			*/
			COMPILE_TIME_FUNCTION(HashValue EncodeHash(const char *pszStr))
			{
				return HashValue{ _EncodeHashImpl<GetHashValueType<HashValue>::Type>(pszStr) };
			}
		}


		/**
		@brief HashValue를 반환 하는 인터페이스
		@date 2018-10-11  오후 19:21:00
		@version 1.0v
		*/
		struct API IToHashValue
		{
			virtual ~IToHashValue() {}
			virtual HashValue32 ToHashValue() const = 0;
		};

	}
}
#pragma warning(pop)
