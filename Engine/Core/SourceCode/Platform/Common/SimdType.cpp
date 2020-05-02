#include "stdafx.h"
#include "SimdType.h"
#include "Container/string.h"


namespace Platform
{
	namespace SIMD
	{

		API ESIMD_FEATURE_LEVEL GetSIMDFeatureLevel() noexcept
		{
			ESIMD_FEATURE_LEVEL level = ESIMD_FEATURE_TYPE::NONE;

			/////////////////////////
			// X64 Architecture
			/////////////////////////
			//SSE series
			level |= (IsSupportSSE()) ? (ESIMD_FEATURE_TYPE::SSE) : (ESIMD_FEATURE_TYPE::NONE);
			level |= (IsSupportSSE2()) ? (ESIMD_FEATURE_TYPE::SSE2) : (ESIMD_FEATURE_TYPE::NONE);
			level |= (IsSupportSSE3()) ? (ESIMD_FEATURE_TYPE::SSE3) : (ESIMD_FEATURE_TYPE::NONE);
			level |= (IsSupportSSSE3()) ? (ESIMD_FEATURE_TYPE::SSSE3) : (ESIMD_FEATURE_TYPE::NONE);
			level |= (IsSupportSSE4_1()) ? (ESIMD_FEATURE_TYPE::SSE4_1) : (ESIMD_FEATURE_TYPE::NONE);
			level |= (IsSupportSSE4_2()) ? (ESIMD_FEATURE_TYPE::SSE4_2) : (ESIMD_FEATURE_TYPE::NONE);

			//New AVX series
			level |= (IsSupportAVX()) ? (ESIMD_FEATURE_TYPE::AVX) : (ESIMD_FEATURE_TYPE::NONE);
			level |= (IsSupportAVX2()) ? (ESIMD_FEATURE_TYPE::AVX2) : (ESIMD_FEATURE_TYPE::NONE);

			//FP16 Convert isa
			level |= (IsSupportFP16()) ? (ESIMD_FEATURE_TYPE::FP16) : (ESIMD_FEATURE_TYPE::NONE);

			//MOVBE isa => byte order change
			level |= (IsSupportMOVBE()) ? (ESIMD_FEATURE_TYPE::MOVBE) : (ESIMD_FEATURE_TYPE::NONE);

			/////////////////////////
			// ARM Architecture
			/////////////////////////
			//ARM
			level |= (IsSupportNEON()) ? (ESIMD_FEATURE_TYPE::NEON) : (ESIMD_FEATURE_TYPE::NONE);

			/////////////////////////
			// SOFTWARE Architecture
			/////////////////////////
			//SOFTWARE
			level |= (IsSupportSoftware()) ? (ESIMD_FEATURE_TYPE::SOFTWARE) : (ESIMD_FEATURE_TYPE::NONE);

			return level;
		}

		API void GetSIMDFeatureLevelToChars(
			const ESIMD_FEATURE_LEVEL level,
			const char** ppszStringArr,
			size_t& ppszStringArrLength)
		{
			using E = ESIMD_FEATURE_TYPE;

			ConstexprString(kszpNONE,  TEXTL("NONE"));
			ConstexprString(kszpSSE,   TEXTL("SSE"));
			ConstexprString(kszpSSE2,  TEXTL("SSE2"));
			ConstexprString(kszpSSE3,  TEXTL("SSE3"));
			ConstexprString(kszpSSSE3, TEXTL("SSSE3"));
			ConstexprString(kszpSSE4_1,TEXTL("SSE4.1"));
			ConstexprString(kszpSSE4_2,TEXTL("SSE4.2"));
			ConstexprString(kszpAVX,   TEXTL("AVX"));
			ConstexprString(kszpAVX2,  TEXTL("AVX2"));
			ConstexprString(kszpFP16,  TEXTL("FP16"));
			ConstexprString(kszpMOVBE, TEXTL("MOVBE"));
			ConstexprString(kszpNEON,  TEXTL("NEON"));
			ConstexprString(kszpSOFTWARE, TEXTL("Software"));

			std::vector<const char*> list;

#define _Check(_TYPE, _VALUE) ((_VALUE & _TYPE) == _TYPE)

			if (E::NONE == level)
			{
				return;
			}
			
			if (_Check(E::SSE, level) == true)
			{
				list.push_back(kszpSSE);
			}

			if (_Check(E::SSE2, level) == true)
			{
				list.push_back(kszpSSE2);
			}

			if (_Check(E::SSE3, level) == true)
			{
				list.push_back(kszpSSE3);
			}

			if (_Check(E::SSSE3, level) == true)
			{
				list.push_back(kszpSSSE3);
			}

			if (_Check(E::SSE4_1, level) == true)
			{
				list.push_back(kszpSSE4_1);
			}

			if (_Check(E::SSE4_2, level) == true)
			{
				list.push_back(kszpSSE4_2);
			}

			if (_Check(E::AVX, level) == true)
			{
				list.push_back(kszpAVX);
			}

			if (_Check(E::AVX2, level) == true)
			{
				list.push_back(kszpAVX2);
			}

			if (_Check(E::FP16, level) == true)
			{
				list.push_back(kszpFP16);
			}

			if (_Check(E::MOVBE, level) == true)
			{
				list.push_back(kszpMOVBE);
			}

			if (_Check(E::NEON, level) == true)
			{
				list.push_back(kszpNEON);
			}

			if (_Check(E::SOFTWARE, level) == true)
			{
				list.push_back(kszpSOFTWARE);
			}

			if (ppszStringArr == nullptr)
			{
				ppszStringArrLength = list.size();
			}
			else
			{
				//list.size() 복사될 갯수, ppszStringArrLength 확보된 수
				size_t copyCount = (list.size() > ppszStringArrLength) ?
					(ppszStringArrLength) : (list.size());

				std::copy_n(std::begin(list), copyCount, &ppszStringArr[0]);
				ppszStringArrLength = copyCount;
			}


#undef _Check
			return;
		}
	}
}
