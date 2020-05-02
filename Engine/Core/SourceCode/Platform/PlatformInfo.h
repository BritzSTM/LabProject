#pragma once


#include "Common\ArchType.h"
#include "Common\SimdType.h"
#include "Common\OSType.h"


namespace Platform
{
	/**
	  Platform Type 정보를 표현하는 공용 구조체 입니다.
	  |*********************************Type : 32bit*********************************|
	  | ECARCH_TYPE 8bit | ECOS_TYPE 8bit |         ESIMD_FEATURE_LEVEL 16bit        |
	*/
	union UPlatformType
	{
		struct
		{
			ECARCH_TYPE Arch;               //Architecture
            ECOS_TYPE OS;                   //Operating System
            SIMD::ESIMD_FEATURE_LEVEL SIMD; //SIMD Feature level
		};
		uint32 Type;                        //| ECARCH_TYPE 8bit | ECOS_TYPE 8bit | ESIMD_FEATURE_LEVEL 16bit |
	};

	/**Platform 정보를 반환 합니다.*/
	FLINE UPlatformType GetPlatformInfo() noexcept
	{
		return UPlatformType
		{
            GetArchitectureType(),
            GetOSType(),
            SIMD::GetSIMDFeatureLevel(),
		};
	}
	
}
