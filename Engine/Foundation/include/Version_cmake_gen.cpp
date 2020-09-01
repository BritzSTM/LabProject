/**
    @file Version.cpp.in
    @brief CMake에 사용되는 템플릿
*/
#include "Version.h"


namespace fd
{
    bool operator<(const SVersionNumber& lhs, const SVersionNumber& rhs) noexcept
    {
        if (lhs.major < rhs.major)
        {
            return true;
        }

        if (lhs.major == rhs.major)
        {
            if (lhs.minor < rhs.minor)
            {
                return true;
            }

            if (lhs.minor == rhs.minor)
            {
                if (lhs.patch < rhs.patch)
                {
                    return true;
                }
            }
        }

        return false;
    }

    SVersionNumber GetVersionNumber() noexcept
    {
        return { 0, 0, 5 };
    }

    SVersionInfo GetVersionInfo() noexcept
    {
        return { "", "", GetVersionNumber() };
    }

    std::string_view GetVersionNumberStr() noexcept
    {
        return { "0.0.5" };
    }
}
