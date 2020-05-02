#include "stdafx.h"
#include "OSType.h"
#include "Container/string.h"


namespace Platform
{

	const char* GetOSTypeToChars(const ECOS_TYPE type) noexcept
	{

		ConstexprString(kszpWIN32,  TEXTL("Win32"));
		ConstexprString(kszpWIN64,  TEXTL("Win64"));
		ConstexprString(kszpLINUX32,TEXTL("Linux32"));
		ConstexprString(kszpLINUX64,TEXTL("Linux64"));
		ConstexprString(kszpNONE,   TEXTL("NONE"));

		switch (type)
		{
		case ECOS_TYPE::WIN32_:
			return kszpWIN32;

		case ECOS_TYPE::WIN64_:
			return kszpWIN64;

		case ECOS_TYPE::LINUX32_:
			return kszpLINUX32;

		case ECOS_TYPE::LINUX64_:
			return kszpLINUX64;

		default:
			return kszpNONE;
		}
	}

}