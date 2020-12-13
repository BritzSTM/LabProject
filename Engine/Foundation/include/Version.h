/**
    @file Version.h
    @brief Version 정보 조회 기능
    @details
    이 파일의 cpp 구현은 cmake configure_file로 생성함.
    따라서 구현 템플릿 파일의 이름은 Version.cpp.in 가진다
*/
#pragma once

#include "base_types.h"

#include <string_view>


namespace fd
{
    /**
        @brief 잘 알려진 버전관리 번호인 major, minor, patch 정보를 담은 구조체. 비교 연산을 지원
    */
    struct SVersionNumber final
    {
        uint16 major;
        uint16 minor;
        uint16 patch;
    };

    inline bool operator==(const SVersionNumber& lhs, const SVersionNumber& rhs) noexcept
    {
        return (lhs.major == rhs.major) && (lhs.minor == rhs.minor) && (lhs.patch == rhs.patch);
    }

    bool operator<(const SVersionNumber& lhs, const SVersionNumber& rhs) noexcept;

    inline bool operator>(const SVersionNumber& lhs, const SVersionNumber& rhs) noexcept
    {
        return !(lhs < rhs);
    }

    inline bool operator<=(const SVersionNumber& lhs, const SVersionNumber& rhs) noexcept
    {
        return (lhs == rhs) || (lhs < rhs);
    }

    inline bool operator>=(const SVersionNumber& lhs, const SVersionNumber& rhs) noexcept
    {
        return (lhs == rhs) || (lhs > rhs);
    }

    /**
        @brief 종합적 버전에 대한 정보를 담은 구조체
    */
    struct SVersionInfo final
    {
        std::string_view name;
        std::string_view features; /**< 문자열로 구성된 특성정보. 특성의 구분자는 자유롭기 때문에 주의*/
        SVersionNumber number;
    };

    /**
        @brief Foundation LIB의 버전번호를 획득
    */
    SVersionNumber GetVersionNumber() noexcept;

    /**
        @brief Foundation LIB의 문자열로 가공된 버전번호를 획득
    */
    std::string_view GetVersionNumberStr() noexcept;

    /**
        @brief Foundation LIB의 버전정보를 획득
    */
    SVersionInfo GetVersionInfo() noexcept;
}
