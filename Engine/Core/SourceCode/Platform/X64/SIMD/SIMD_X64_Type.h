/*
  Architecture에 따른 SIMD 명령어를 규격화 합니다.
  다음과 같은 타입을 반드시 정의해야 합니다.
  simd128, simd128i, simd128d | 128bit => 16byte
  simd256, simd256i, simd256d | 256bit => 32byte

  simd16, simd16i, simd16d
  simd32, simd32i, simd32d
  LAST UPDATE 2018/ 07/ 21/ AM12:49
*/

#pragma once

#include "CoreHelper.h"
#include "Common/BaseTypes.h"

#include <cassert>

#include <intrin.h>
#include <immintrin.h>


template <typename T>
constexpr std::size_t size_of(T const&) {
	return sizeof(T);
}


template <typename Ty>
union alignas(8) __simd64
{
	typedef Ty _Type;
	_Type  m64;       //int[2], short[4]

	//__simd64 size must be 8Byte
	static_assert(sizeof(_Type) == 8, "__simd64 Ty size must be 8Byte");

	fp16 f16[4];
	fp32 f32[2];
	struct
	{
		float fh;
		float fl;
	};

	int    i32[2];
	struct
	{
		int ih;
		int il;
	};

	double f64;
};

using simd64 = __simd64<__m64>;

template <typename Ty, size_t len>
union alignas(16) __simd128       //=>16byte 4*4
{
	__m128 m128;
	Ty     m[len];

	const Ty& operator[](int index) const
	{
		assert(index >= 0 && index < len);
		return m[index];
	}
	Ty& operator[](int index)
	{
		assert(index >= 0 && index < len);
		return m[index];
	}
};

using simd128f       = __simd128<float, 4>;
using simd128fx128f  = simd128f[4];
using simd4f         = simd128f;
using simd4fx4f      = simd128fx128f;

using simd128i       = __simd128<int  , 4>;
using simd128ix128i  = simd128i[4];
using simd4i         = simd128i;
using simd4ix4i      = simd128ix128i;

using simd128d       = __simd128<double, 2>;
using simd128dx128d  = simd128d[2];
using simd2d         = simd128d;
using simd2dx2d      = simd128dx128d;






template <typename Ty, size_t len>
union alignas(32) __simd256       // =>32byte 4*4
{
	__m256 m256;
	Ty     m[len];

	const Ty& operator[](int index) const
	{
		assert(index >= 0 && index < len);
		return m[index];
	}
	Ty& operator[](int index)
	{
		assert(index >= 0 && index < len);
		return m[index];
	}
};


using simd256f      = __simd256<float, 8>;
using simd256fx256f = simd256f[4];

template <typename Ty>
union alignas(32) __simd256_4x4     // =>32byte 4*4
{
	__simd256<Ty, 8> m[2];

	const Ty& operator[](int index) const
	{
		assert(index >= 0 && index < 4);
		return m[index*0.5];
	}
	Ty& operator[](int index)
	{
		assert(index >= 0 && index < 4);
		return m[index*0.5];
	}
};
// [3][0] => [2][5]
using simdxx        = simd256f[2];
using simd256 = __m256;
using simd256i = __simd256<int, 8>;
using simd256d = __simd256<double, 4>;


/*
  f16c 명령을(SIMD에 포함된 명령) 이용한 FP16 타입의 고속변환 함수
  f16c 명령어 타입을 지원하는지 확인해야 한다.
*/

/*
  fp16 타입을 fp32으로 변환 합니다.
*/
FLINE fp32 FP16ToFP32(const _FP16RawType fValue)
{
	__m128i V1 = _mm_cvtsi32_si128(static_cast<uint32>(fValue.encoded));
	__m128  V2 = _mm_cvtph_ps(V1);

	return _mm_cvtss_f32(V2);
}

/*
  fp32 타입을 fp16으로 변환 합니다.
*/
FLINE _FP16RawType FP32ToFP16(const fp32 fValue)
{
	__m128  V1 = _mm_set_ss(fValue);
	__m128i V2 = _mm_cvtps_ph(V1, 0);

	return _FP16RawType{ static_cast<decltype(_FP16RawType::encoded)>(_mm_cvtsi128_si32(V2)) };
}
