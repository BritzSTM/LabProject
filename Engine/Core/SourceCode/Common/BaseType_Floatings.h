/*
  fp16 Type
  모든 소프트웨어 연산은 fp16에서 fp32로 변환 후 연산을 진행하며
  다시 fp32 => fp16으로 변환합니다.

  fp16 구조 참조
  https://en.wikipedia.org/wiki/Half-precision_floating-point_format

  요약 Format
  Sign bit: 1 bit | Exponent width: 5 bits | Significand precision: 11 bits (10 explicitly stored)

  Nan State
  0 11111 0000000000 = infinity
  1 11111 0000000000 = −infinity

  LAST UPDATE : 2018/ 07/ 24/ PM 20:00
*/

#pragma once

#include "CoreHelper.h"
#include "BaseType_Integers.h"

#include "Type\TypeTraits.h"
#include <functional>

//Visual Studio 전용이므로 차후에 바꿔야함
#pragma warning(push)
#pragma warning(disable: 4455)

namespace Core
{
	namespace BaseTypes
	{


		/*

		  기본으로 사용될 실수 원자 타입에 대한 정적 크기 확인

		*/
		static_assert(sizeof(float) == 4, "float type size isn't 4byte!");
		static_assert(sizeof(double) == 8, "double type size isn't 8byte!");





		/*

		  실수형 타입 정의

		*/
		using fp32 = float;
		using fp64 = double;





		/*
		  fp16의 기반이 되는 Raw type 입니다.
		  이와 같이 union으로 선언해 랩핑한 이유는
		  잘못된 대입으로 인한 형변환을 막기 위해서 입니다.

		  이 타입은 POD이며 2바이트 기준으로 정렬됩니다.
		*/
		using  _FP16RawType = union alignas(2) {
			unsigned __int16 encoded;          //raw data
		};


		//Check _FP16RawType
		static_assert((sizeof(_FP16RawType) == 2), "_FP16RawType type size isn't 2byte!");
		static_assert((std::is_pod<_FP16RawType>::value == true), "_FP16RawType type must be pod type");





		/*
		  fp16에 대한 통제를 담당하는 제어 클래스
		*/
		class FP16Control
		{
			// enums and typedefs and nested classes
		public:

			/*
			  _FP16RawType Convert Concepts
			*/
			using FP16RawToFP32Converter = std::function<fp32(const _FP16RawType)>;  //_FP16RawType => fp32 함수포인터Type
			using FP32ToFP16RawConverter = std::function<_FP16RawType(const fp32)>;  //fp32 => _FP16RawType 함수포인터Type

			/*
			  _FP16RawType State Concepts
			*/
			using FP16RawIsNaNStater = std::function<bool(const _FP16RawType)>;      //NaN State

			/*
			  _FP16RawType Operator Concepts
			*/
			using FP16RawAddOperator = std::function<_FP16RawType(const _FP16RawType, const _FP16RawType)>; //덧셈
			using FP16RawSubOperator = std::function<_FP16RawType(const _FP16RawType, const _FP16RawType)>; //뺄셈
			using FP16RawMulOperator = std::function<_FP16RawType(const _FP16RawType, const _FP16RawType)>; //곱셈
			using FP16RawDivOperator = std::function<_FP16RawType(const _FP16RawType, const _FP16RawType)>; //나눗셈

		private:

			/*
			  fp32 state
			*/
			struct _fp32State
			{
				union Bits
				{
					float   f;
					int32  si;
					uint32 ui;
				};

				static constexpr int32 shift = 13;
				static constexpr int32 shiftSign = 16;

				static constexpr int32 infN = 0x7F800000;          // flt32 infinity
				static constexpr int32 maxN = 0x477FE000;          // max flt16 normal as a flt32
				static constexpr int32 minN = 0x38800000;          // min flt16 normal as a flt32
				static constexpr int32 signN = 0x80000000;         // flt32 sign bit

				static constexpr int32 infC = infN >> shift;
				static constexpr int32 nanN = (infC + 1) << shift; // minimum flt16 nan as a flt32
				static constexpr int32 maxC = maxN >> shift;
				static constexpr int32 minC = minN >> shift;
				static constexpr int32 signC = signN >> shiftSign; // flt16 sign bit

				static constexpr int32 mulN = 0x52000000; // (1 << 23) / minN
				static constexpr int32 mulC = 0x33800000; // minN / (1 << (23 - shift))

				static constexpr int32 subC = 0x003FF;    // max flt32 subnormal down shifted
				static constexpr int32 norC = 0x00400;    // min flt32 normal down shifted

				static constexpr int32 maxD = infC - maxC - 1;
				static constexpr int32 minD = minC - subC - 1;
			};

			/*
			  fp16 state
			*/
			struct _fp16State
			{

			};

			// Methods
		public:

			/*
			  변환
			*/
			/*
			  소프트웨어적으로 _FP16RawType => FP32 타입으로 변환하는 함수입니다.
			  예외가 발생하지 않는 함수입니다.
			  더 이상 사용되지 않습니다.
			*/
			[[deprecated("Do not use v1 decode func")]]
			FLINE static constexpr fp32 ConvertFP16RawToFP32_Soft(const _FP16RawType) noexcept;

			/*
			  소프트웨어적으로 _FP16RawType => FP32 타입으로 변환하는 함수입니다.
			  예외가 발생하지 않는 함수입니다.
			*/
			FLINE static constexpr fp32 ConvertFP16RawToFP32_Softv2(const _FP16RawType) noexcept;

			/*
			  소프트웨어적으로 FP32 => _FP16RawType 타입으로 변환하는 함수입니다.
			  예외가 발생하지 않는 함수입니다.
			  더 이상 사용되지 않습니다.
			*/
			[[deprecated("Do not use v1 encode func")]]
			FLINE static constexpr _FP16RawType ConvertFP32ToFP16Raw_Soft(const fp32) noexcept;

			/*
			  소프트웨어적으로 FP32 => _FP16RawType 타입으로 변환하는 함수입니다.
			  예외가 발생하지 않는 함수입니다.
			*/
			FLINE static constexpr _FP16RawType ConvertFP32ToFP16Raw_Softv2(const fp32) noexcept;

			/*
			  FP16 타입에 대한 변환 함수의 상태를 소프트웨어 변환으로 초기화 합니다.
			  예외가 발생하지 않는 함수입니다.
			*/
			static void API InitializeFP16Converter() noexcept;

			/*
			  FP16 타입에 대한 변환 함수들을 지정합니다.
			  empty 함수 객체는 InvalidArgumentNullptrException 예외를 발생 시킵니다.
			*/
			static void API SetFP16Converter(const FP16RawToFP32Converter, const FP32ToFP16RawConverter);

			/*
			  FP16 타입에 지정되어 있는 변환 함수들을 획득합니다.
			  nullptr 전달인자는 InvalidArgumentNullptrException 예외를 발생 시킵니다.
			*/
			void API GetFP16Converter(FP16RawToFP32Converter*, FP32ToFP16RawConverter*);

			/*
			  FP16RawType을 fp32 타입으로 변환합니다.
			*/
			FLINE static fp32 API ConvertFP16RawToFP32(const _FP16RawType) noexcept;

			/*
			  fp32 타입을 _FP16RawType으로 변환합니다.
			*/
			FLINE static _FP16RawType API ConvertFP32ToFP16Raw(const fp32) noexcept;

			/*
			  상태
			*/
			/*
			  _FP16RawType QuietNan을 반환합니다.
			*/
			FLINE static constexpr _FP16RawType _Get_FP16RawTypeQuietNaN() noexcept
			{
				//‭0 11111 1000000000‬
				return _FP16RawType{ 0x7E00 };
			}

			/*
			  _FP16RawType QuietNan을 반환합니다.
			*/
			FLINE static constexpr _FP16RawType _Get_FP16RawTypeSignalingNaN() noexcept
			{
				//‭0 11111 1000000000‬
				return _FP16RawType{ 0x7E01 };
			}

			/*
			  _FP16RawType Positive infinity 반환합니다.
			*/
			FLINE static constexpr _FP16RawType _Get_FP16RawTypeInfinity() noexcept
			{
				/*
				Infinity State
				0 11111 0000000000 = infinity
				1 11111 0000000000 = −infinity
				*/
				return _FP16RawType{ 0x7C00 };
			}

			/*
			  FP16 타입에 대한 상태 함수의 상태를 소프트웨어 변환으로 초기화 합니다.
			  예외가 발생하지 않는 함수입니다.
			*/
			static void API InitializeFP16Stater() noexcept;

			/*
			  FP16 타입에 대한 상태 함수의 상태를 소프트웨어 변환으로 초기화 합니다.
			  empty 함수 객체는 InvalidArgumentNullptrException 예외를 발생 시킵니다.
			*/
			static void API SetFP16Stater(const FP16RawIsNaNStater);

			/*
			  FP16 타입에 대한 상태 함수의 상태를 소프트웨어 변환으로 초기화 합니다.
			  nullptr 전달인자는 InvalidArgumentNullptrException 예외를 발생 시킵니다.
			*/
			static void API GetFP16Stater(FP16RawIsNaNStater*);

			/*
			  소프트웨어적으로 _FP16RawType타입이 Nan인지 확인하는 함수 입니다.
			  예외가 발생하지 않는 함수입니다.
			*/
			ILINE static bool IsNaN_FP16RawType_Soft(const _FP16RawType hf) noexcept;

			/*
			  _FP16RawType타입이 Nan인지 확인하는 함수 입니다.
			  예외가 발생하지 않는 함수입니다.
			*/
			FLINE static bool IsNaN_FP16RawType(const _FP16RawType hf) noexcept
			{
				static FP16RawIsNaNStater& targetProc = _InitializeNaNStater();
				return targetProc(hf);
			}


			/*
			  연산
			*/
			FLINE static _FP16RawType AddFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				//fp32로 변환 후 연산
				return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) + ConvertFP16RawToFP32(rhs));
			}

			FLINE static _FP16RawType SubFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				//fp32로 변환 후 연산
				return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) - ConvertFP16RawToFP32(rhs));
			}

			FLINE static _FP16RawType MulFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				//fp32로 변환 후 연산
				return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) * ConvertFP16RawToFP32(rhs));
			}

			/*
			  나누기 연산
			  0으로 나눌 경우 예외 발생
			*/
			FLINE static _FP16RawType DivFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) / ConvertFP16RawToFP32(rhs));
			}

			/*
			  _FP16RawType 덧셈 연산
			*/
			FLINE static _FP16RawType AddFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				static FP16RawAddOperator& targetPrco = _InitializeAddOperator();
				return targetPrco(lhs, rhs);
			}
			FLINE static _FP16RawType SubFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				static FP16RawSubOperator& targetPrco = _InitializeSubOperator();
				return targetPrco(lhs, rhs);
			}
			FLINE static _FP16RawType MulFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				static FP16RawMulOperator& targetPrco = _InitializeMulOperator();
				return targetPrco(lhs, rhs);
			}
			FLINE static _FP16RawType DivFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
			{
				static FP16RawDivOperator& targetPrco = _InitializeDivOperator();
				return targetPrco(lhs, rhs);
			}



		private:
			/*
			  초기화 함수 집합
			*/

			FLINE static FP16RawToFP32Converter& _InitializeFP16RawToFP32Converter() noexcept;
			FLINE static FP32ToFP16RawConverter& _InitializeFP32ToFP16RawConverter() noexcept;

			FLINE static FP16RawIsNaNStater&     _InitializeNaNStater() noexcept;

			FLINE static FP16RawAddOperator&     _InitializeAddOperator() noexcept;
			FLINE static FP16RawSubOperator&     _InitializeSubOperator() noexcept;
			FLINE static FP16RawMulOperator&     _InitializeMulOperator() noexcept;
			FLINE static FP16RawDivOperator&     _InitializeDivOperator() noexcept;


			// Members
		private:

			static FP16RawToFP32Converter s_pFP16RawToFP32;
			static FP32ToFP16RawConverter s_pFP32ToFP16Raw;

			static FP16RawIsNaNStater s_pFP16RawIsNaN;

			static FP16RawAddOperator s_pFP16RawAdd;
			static FP16RawSubOperator s_pFP16RawSub;
			static FP16RawMulOperator s_pFP16RawMul;
			static FP16RawDivOperator s_pFP16RawDiv;

		};

		constexpr fp32 FP16Control::ConvertFP16RawToFP32_Soft(const _FP16RawType hf) noexcept
		{
			//         sing 1bit      ,   exponent 5bits                      ,   Significand 11bits
			fp32 f = static_cast<fp32>(((hf.encoded & 0x8000) << 16) | (((hf.encoded & 0x7c00) + 0x1C000) << 13) | ((hf.encoded & 0x03FF) << 13));

			return static_cast<fp32>(f);
		}

		constexpr _FP16RawType FP16Control::ConvertFP32ToFP16Raw_Soft(const fp32 f) noexcept
		{
			unsigned __int32 x = *((unsigned __int32*)&f);

			return  _FP16RawType{ ((x >> 16) & 0x8000) | ((((x & 0x7f800000) - 0x38000000) >> 13) & 0x7c00) | ((x >> 13) & 0x03ff) };
		}

		constexpr fp32 FP16Control::ConvertFP16RawToFP32_Softv2(const _FP16RawType hf) noexcept
		{
			/*
			  https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
			*/
			using fs = _fp32State;

			fs::Bits v{ 0.f };

			v.ui = hf.encoded;
			int32 sign = v.si & fs::signC;
			v.si ^= sign;
			sign <<= fs::shiftSign;
			v.si ^= ((v.si + fs::minD) ^ v.si) & -(v.si > fs::subC);
			v.si ^= ((v.si + fs::maxD) ^ v.si) & -(v.si > fs::maxC);

			fs::Bits s{ 0.f };
			s.si = fs::mulC;
			s.f *= v.si;
			int32 mask = -(fs::norC > v.si);
			v.si <<= fs::shift;
			v.si ^= (s.si ^ v.si) & mask;
			v.si |= sign;

			return v.f;
		}

		constexpr _FP16RawType FP16Control::ConvertFP32ToFP16Raw_Softv2(const fp32 f) noexcept
		{
			/*
			  https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
			*/
			using fs = _fp32State;

			fs::Bits v{ f }, s{ 0.f };

			//v.f = value;
			uint32 sign = v.si & fs::signN;
			v.si ^= sign;
			sign >>= fs::shiftSign; // logical shift
			s.si = fs::mulN;
			s.si = static_cast<int32>(s.f * v.f);       // correct subnormals
			v.si ^= (s.si ^ v.si) & -(fs::minN > v.si);
			v.si ^= (fs::infN ^ v.si) & -((fs::infN > v.si) & (v.si > fs::maxN));
			v.si ^= (fs::nanN ^ v.si) & -((fs::nanN > v.si) & (v.si > fs::infN));
			v.ui >>= fs::shift;     // logical shift
			v.si ^= ((v.si - fs::maxD) ^ v.si) & -(v.si > fs::maxC);
			v.si ^= ((v.si - fs::minD) ^ v.si) & -(v.si > fs::subC);

			return _FP16RawType{ static_cast<uint16>(v.ui | sign) };
		}

		fp32 API FP16Control::ConvertFP16RawToFP32(const _FP16RawType fp16Raw) noexcept
		{
			static FP16RawToFP32Converter& target = _InitializeFP16RawToFP32Converter();

			return target(fp16Raw);
		}

		_FP16RawType API FP16Control::ConvertFP32ToFP16Raw(const fp32 f) noexcept
		{
			static FP32ToFP16RawConverter& target = _InitializeFP32ToFP16RawConverter();

			return target(f);
		}


		bool FP16Control::IsNaN_FP16RawType_Soft(const _FP16RawType hf) noexcept
		{
			/*
			Nan State
			0 11111 0000000000 = qnan
			1 11111 0000000001 = snan

			두개의 상태를 다 확인해야함.
			*/
			using uint16 = unsigned __int16;
			using uint32 = unsigned __int32;

			constexpr uint16 mask1 = _Get_FP16RawTypeQuietNaN().encoded;
			constexpr uint16 mask2 = _Get_FP16RawTypeSignalingNaN().encoded;

			union alignas(4) Board
			{
				struct
				{
					uint16 m1;
					uint16 m2;
				};
				uint32 m32;
			};

			constexpr Board masks{ mask1, mask2 };  //setup +, - mask
			Board target{ hf.encoded   , hf.encoded };

			target.m32 = (target.m32 & masks.m32);


			if ((target.m1 == masks.m1) || (target.m1 == masks.m2))
			{
				return true;
			}
			else
			{
				return false;
			}
		}


		FP16Control::FP16RawToFP32Converter& FP16Control::_InitializeFP16RawToFP32Converter() noexcept
		{
			if (s_pFP16RawToFP32 == nullptr)
				s_pFP16RawToFP32 = ConvertFP16RawToFP32_Softv2;

			return s_pFP16RawToFP32;
		}

		FP16Control::FP32ToFP16RawConverter& FP16Control::_InitializeFP32ToFP16RawConverter() noexcept
		{
			if (s_pFP32ToFP16Raw == nullptr)
				s_pFP32ToFP16Raw = ConvertFP32ToFP16Raw_Softv2;

			return s_pFP32ToFP16Raw;
		}


		FP16Control::FP16RawIsNaNStater& FP16Control::_InitializeNaNStater() noexcept
		{
			if (s_pFP16RawIsNaN == nullptr)
				s_pFP16RawIsNaN = IsNaN_FP16RawType_Soft;

			return s_pFP16RawIsNaN;
		}


		FP16Control::FP16RawAddOperator& FP16Control::_InitializeAddOperator() noexcept
		{

			s_pFP16RawAdd = AddFP16Raw_Soft;
			return s_pFP16RawAdd;
		}

		FP16Control::FP16RawSubOperator& FP16Control::_InitializeSubOperator() noexcept
		{
			s_pFP16RawSub = SubFP16Raw_Soft;
			return s_pFP16RawSub;
		}

		FP16Control::FP16RawMulOperator& FP16Control::_InitializeMulOperator() noexcept
		{
			s_pFP16RawMul = MulFP16Raw_Soft;
			return s_pFP16RawMul;
		}

		FP16Control::FP16RawDivOperator& FP16Control::_InitializeDivOperator() noexcept
		{
			s_pFP16RawDiv = DivFP16Raw_Soft;
			return s_pFP16RawDiv;
		}





		/*
		  Half-precision floating-point(2 Byte)
		  이 타입은 POD이며 2바이트 기준으로 정렬됩니다.
		*/
		union alignas(2) fp16
		{
		public:

			//fp16() : m_fp16RawData() { }
			//fp16(const float &f) : m_fp16RawData(FP16Control::ConvertFP32ToFP16Raw(f)) { }

			/*
			  형 변환
			*/
			FLINE operator float() const noexcept
			{
				return static_cast<float>(FP16Control::ConvertFP16RawToFP32(m_fp16RawData));
			}

			//FLINE operator int() const noexcept
			//{
			//	return static_cast<int>(FP16Control::ConvertFP16RawToFP32(m_fp16RawData));
			//}

			/*
			  대입
			*/
			FLINE fp16& operator=(const float& f) noexcept
			{
				m_fp16RawData = FP16Control::ConvertFP32ToFP16Raw(f);
				return *this;
			}

			FLINE fp16& operator=(const int& i) noexcept
			{
				m_fp16RawData = FP16Control::ConvertFP32ToFP16Raw(static_cast<float>(i));
				return *this;
			}

			/*
			  연산자
			*/
			friend constexpr fp16 operator""hf(long double d) noexcept;


			FLINE fp16& operator/(const fp16& rhs)
			{
				m_fp16RawData = FP16Control::DivFP16Raw(this->m_fp16RawData, rhs.m_fp16RawData);
				return *this;
			}

		private:
			_FP16RawType m_fp16RawData;
		};


		/*
		  fp16(half-floating) literal
		  컴파일 타임 변환을 보장해야함
		*/
		FLINE constexpr fp16 operator""hf(long double d) noexcept
		{
			fp16 f{};
			f.m_fp16RawData = FP16Control::ConvertFP32ToFP16Raw_Softv2(static_cast<float>(d));
			return f;
		}


		using hfloat = fp16;  //fp16 another name

		//Check FP16 Type
		static_assert((sizeof(fp16) == 2), "fp16 type size isn't 2byte!");
		static_assert((std::is_pod<fp16>::value == true), "fp16 type must be pod type");
		static_assert((std::is_same<fp16, hfloat>::value == true), "fp16 and hfloat type is not same");

	}
}

/*

union alignas(2) FP16
{
public:

	//형 변환
	FLINE operator float() const
	{
		//raw.to32...? | 내가 직접 함수 포인터 호출...
		//float으로 변환 후 int 형 cast 
		;
		return static_cast<float>(_FP16Control::to1(this->m_rawData));
	}

	FLINE operator int() const
	{
		//float으로 변환 후 int 형 cast 
		return static_cast<int>(0);
	}


	FLINE FP16& operator=(const float &f)
	{
		//처리후 이걸 반환
		return *this;
	}

	FLINE void operator=(float f)
	{
		
	}

private:
	_FP16RawType m_rawData;
};

//Check _FP16RawType
static_assert((sizeof(FP16) == 2), "_FP16RawType type size isn't 2byte!");
static_assert((std::is_pod<FP16>::value == true), "_FP16RawType type must be pod type");

*/

/*
/*
fp16 Type을 위한 생성된 inl 파일
반드시 BaseTypes에만 포함되어야 합니다.

fp16의 추가 연산에 대한것을 구현한 파일.
std 기본 네임스페이스에 있는 함수들을 구현한 것.

LAST UPDATE : 2018/ 07/ 24/ PM 20:00
*/


/*
fp16 부동소수점 필요 함수

std 네임스페이스의 함수를 이용
*/
/*
//namespace std
//{
//	/*
//	FP16의 상태가 NaN인지 확인 합니다.
//	예외가 발생하지 않는 함수입니다.
//	*/
//	FLINE bool isnan(const GYP::Engine::Core::BaseTypes::fp16 hf) noexcept
//	{
//		return GYP::Engine::Core::BaseTypes::FP16IsNan(hf);
//	}
//
//	FLINE GYP::Engine::Core::BaseTypes::fp16 nanhf() noexcept
//	{
//		return GYP::Engine::Core::BaseTypes::GetFP16QuietNan();
//	}
//}
//*/


	/*
	한계 상태 정의
	std base로 작성
	numeric_limits<fp16>
	*/
	/*
	template<> class numeric_limits<fp16>
	: public std::_Num_float_base
	{     // limits for type fp16
	public:
	typedef fp16 _Ty;

	_NODISCARD static constexpr _Ty(min)() _noexcept
	{	// return minimum value
	return (_FLT_MIN);
	}

	_NODISCARD static constexpr _Ty(max)() _noexcept
	{	// return maximum value
	return (_FLT_MAX);
	}

	_NODISCARD static constexpr _Ty lowest() _noexcept
	{	// return most negative value
	return (-(max)());
	}

	_NODISCARD static constexpr _Ty epsilon() _noexcept
	{	// return smallest effective increment from 1.0
	return (_FLT_EPSILON);
	}

	_NODISCARD static constexpr _Ty round_error() _noexcept
	{	// return largest rounding error
	return (0.5F);
	}

	_NODISCARD static constexpr _Ty denorm_min() _noexcept
	{	// return minimum denormalized value
	return (_FLT_TRUE_MIN);
	}

	_NODISCARD static constexpr _Ty infinity() _noexcept
	{	// return positive infinity
	return (__builtin_huge_valf());
	}

	_NODISCARD static constexpr _Ty quiet_NaN() _noexcept
	{	// return non-signaling NaN
	return (__builtin_nanf("0"));
	}

	_NODISCARD static constexpr _Ty signaling_NaN() _noexcept
	{	// return signaling NaN
	return (__builtin_nansf("1"));
	}

	_STCONS(int, digits, FLT_MANT_DIG);
	_STCONS(int, digits10, FLT_DIG);

	_STCONS(int, max_digits10, 2 + FLT_MANT_DIG * 301L / 1000);

	_STCONS(int, max_exponent, (int)FLT_MAX_EXP);
	_STCONS(int, max_exponent10, (int)FLT_MAX_10_EXP);
	_STCONS(int, min_exponent, (int)FLT_MIN_EXP);
	_STCONS(int, min_exponent10, (int)FLT_MIN_10_EXP);
	};
	*/

#pragma warning(pop)


