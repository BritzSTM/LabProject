#include "stdafx.h"
#include "ArchType.h"
#include "Container/string.h"


namespace Platform
{

	const char* GetArchitectureTypeToChars(const ECARCH_TYPE type) noexcept
	{
		ConstexprString(kszpX32,   TEXTL("Arch X32"));
		ConstexprString(kszpX64,   TEXTL("Arch X64"));
		ConstexprString(kszpARMv8, TEXTL("Arch ARMv8"));
		ConstexprString(kszpNONE,  TEXTL("Arch NONE"));

		switch (type)
		{
		case ECARCH_TYPE::X32:
			return kszpX32;

		case ECARCH_TYPE::X64:
			return kszpX64;

		case ECARCH_TYPE::ARMv8:
			return kszpARMv8;

		default:
			return kszpNONE;
		}
	}

}