#pragma once

#include <array>
#include <type_traits>


namespace fd
{
    /**
        @brief 템플릿 변수에 따라 isEnumType 속성 값이 결정되는 템플릿 타입
    */
    template<typename _Ty, bool b = std::is_enum_v<_Ty>>
    struct IsEnumProperty
    {
        static constexpr bool isEnumType{ false };
    };

    template<typename _Ty>
    struct IsEnumProperty<_Ty, true>
    {
        static constexpr bool isEnumType{ true };
    };

    /**
        @brief enum class 타입을 컴파일 타임에 상수로 쓸 수 있도록 변환
    */
    template<typename T>
    constexpr auto ecTo(const T value) noexcept -> std::underlying_type_t<T>
    {
        return static_cast<std::underlying_type_t<T>>(value);
    }

    /**
        @brief enum, enum class에 값들에 대해 그룹화 합니다.
        _notGroupVal 값은 이 그룹의 값이 아닌 경우 표현되는 값 입니다.
    */
    template<typename _Ty, _Ty _notGroupVal, _Ty... _vals>
    class enum_group final
        : public IsEnumProperty<_Ty>
    {
        static_assert(enum_group::isEnumType, "_Ty type is not enum type.");

    public:
        using value_type = _Ty;
        using set_type = std::array<value_type, sizeof...(_vals)>;
        using const_iterator = typename set_type::const_iterator;

    public:
        static constexpr value_type filterByGroup(const value_type v) noexcept
        {
            for (auto i{ 0 }; i<s_kSet.size(); ++i)
            {
                if (v == s_kSet[i])
                {
                    return v;
                }
            }

            return kNotGroupVal;
        }

        static constexpr bool isGroupVal(const value_type v) noexcept
        {
            return (filterByGroup(v) != kNotGroupVal);
        }

        static constexpr const_iterator cbegin() noexcept
        {
            return s_kSet.cbegin();
        }

        static constexpr const_iterator cend() noexcept
        {
            return s_kSet.cend();
        }

        ~enum_group() = default;

        constexpr enum_group(const value_type v) noexcept
            : m_val{ enum_group::filterByGroup(v) }
        {

        }

        constexpr enum_group(const enum_group&) = default;

        constexpr operator value_type() noexcept
        {
            return m_val;
        }

        constexpr value_type operator=(const enum_group & rhs) noexcept
        {
            m_val = rhs.m_val;
        }

        constexpr bool operator==(const value_type & rhs) noexcept
        {
            return m_val == rhs.m_val;
        }

    public:
        static constexpr value_type kNotGroupVal{ _notGroupVal };

    private:
        static constexpr set_type s_kSet{ _vals... };

        value_type m_val;
    };
}
