#pragma once


#include "EnvironmentValue.h"
#include "ArchType.h"
#include "OSType.h"

#include <functional>


namespace Platform
{
	namespace SIMD
	{

		using ESIMD_FEATURE_LEVEL = uint16;

		/**
		사용가능한 simd 명령어 집합에 대해 열거형으로 정의 합니다.
		*/
		enum ESIMD_FEATURE_TYPE : ESIMD_FEATURE_LEVEL
		{
			NONE = 0x0000,  //분류되지 않는 타입입니다.
			SSE = 0x0001,
			SSE2 = 0x0002,
			SSE3 = 0x0004,
			SSSE3 = 0x0008,
			SSE4_1 = 0x0010,
			SSE4_2 = 0x0020,
			AVX = 0x0040,  //X64 256bit
			AVX2 = 0x0080,  //X64 256bit int 확장
			FP16 = 0x0100,  //FP16 <=> FP32 고속 명령
			MOVBE = 0x0200,  //MSB LSB 고속 스위칭 명령
			NEON = 0x0400,  //ARM
			SOFTWARE = 0x8000,

			TYPE_COUNT = 14 //ESIMD_FEATURE_TYPE의 수
		};

		/*
		  X64 Architecture 명령어 집합은 Platform\X64\X64_ISA.asm으로 작성되어 링크된다.
		*/
#       if (TARGET_ARCH == ARCH_X64)

//#include "X64\SIMD\SIMD_X64_Type.h"
		namespace
		{
			extern "C"
			{
				bool __IsSupportSSE_ASM();
				bool __IsSupportSSE2_ASM();
				bool __IsSupportSSE3_ASM();
				bool __IsSupportSSSE3_ASM();
				bool __IsSupportSSE4_1_ASM();
				bool __IsSupportSSE4_2_ASM();
				bool __IsSupportFP16_ASM();
				bool __IsSupportMOVBE_ASM();
				bool __IsSupportAVX_ASM();
				bool __IsSupportAVX2_ASM();
			}
		}
#       endif

		/*
		  ARMv8 Architecture 명령어 집합은 Platform\ARM\ARM_ISA.asm으로 작성되어 링크된다.
		*/
#       if (TARGET_ARCH == ARCH_ARMv8)
		COMPILE_ERROR("Not implemented")
#       endif

		extern "C"
		{
			/**
			  CPU의 SSE명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportSSE()
			{
#  		    if (TARGET_ARCH == ARCH_X64)
				return __IsSupportSSE_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 SSE2명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportSSE2()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportSSE2_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 SSE3명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportSSE3()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportSSE3_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 SSSE3명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportSSSE3()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportSSSE3_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 SSE4.1명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportSSE4_1()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportSSE4_1_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 SSE4.2명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportSSE4_2()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportSSE4_2_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 FP16명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportFP16()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportFP16_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 MOVBE명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportMOVBE()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportMOVBE_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 AVX명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportAVX()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportAVX_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 AVX2명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportAVX2()
			{
#           if (TARGET_ARCH == ARCH_X64)
				return __IsSupportAVX2_ASM();
#           else
				return false;
#           endif
			}

			/**
			  CPU의 NEON명령어 지원 여부를 반환 합니다.
			*/
			FLINE bool IsSupportNEON()
			{
#           if (TARGET_ARCH == ARCH_ARMv8)
				COMPILE_ERROR("Not implemented")
#           else
				return false;
#           endif
			}

			/**
			  SIMD명령어를 사용하지 않는 Software 구현에 대한 지원 여부를 반환 합니다.
			  Software 구현을 미사용 하려면 NOT_USE_SIMD_SOFTWARE를 정의 하세요.
			*/
			FLINE bool IsSupportSoftware()
			{
#           if defined(NOT_USE_SIMD_SOFTWARE)
				return false;
#           else
				return true;
#           endif
			}
		}

		extern "C"
		{
			/**
			  CPU가 지원하는 SIMD명령어 특성 레벨을 반환 합니다.
			*/
			API ESIMD_FEATURE_LEVEL GetSIMDFeatureLevel() noexcept;

			/**
			ESIMD_FEATURE_LEVEL을 전달 받아
			outList에 문자열 정보를 저장합니다.
			만약 ppszStringArr값이 nullptr이 전달 된다면
			ppszStringArrLength에는 지원하는 갯수가 전달됩니다.
			*/
			API void GetSIMDFeatureLevelToChars(
				const ESIMD_FEATURE_LEVEL level, 
				const char** ppszStringArr,
				size_t& ppszStringArrLength);
		}

	
		/*
		  64, 128, 256 bit의 simd type들을 정의 합니다.
		  각 타입들은 메모리상에서 정렬된 상태로 존재 해야 합니다.
		*/

		/*
		미완
		fp16 operation Bind
		@see 아직 정리 안됨
		*/
		static bool _InitFP16()
		{
			//if (IsSupportFP16())
			//{
			//	_sg_pFP16ToFP32 = &FP16ToFP32;
			//	_sg_pFP32ToFP16 = &FP32ToFP16;

				//초기화 타임 문제 발생. 언제 연산자 bind가 진행 되나??
			//	fp16 a, b;
			//	a = a + b;

			//	return true;
			//}
			//else
			//{
			return false;
			//}
		}

		static bool _initFP16 = _InitFP16();


		/*
		  Macro Arch를 전부 해제 한다.
		*/
#               undef ARCH_NONE
#               undef ARCH_X32
#               undef ARCH_X64
#               undef ARCH_ARMv8
	}
}
