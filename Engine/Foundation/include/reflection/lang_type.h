#pragma once

#include "../base_types.h"
#include "../type_traits_ext.h"


namespace fd::refl
{
    /**
        @brief 언어에서 구분한 모든 타입을 열거형으로 정의
        @detail
        기반 자료형의 경우 고정길이 자료형으로 구분이 되는 경우만 정의 했다. 플랫폼 공통적으로 자료형을 질의하고 싶기 때문이다.
        따라서 long double, long, 단순 int 같이 길이가 모호한 경우 열거형으로 정의되지 않았다.

        그리고 c++ 언어에서 class, struct는 기본 접근지정자의 차이밖에 없기 때문에 유형구분이 외적도구나 비표준 기능을 통하지 않는한 구별되지 않는다.
        따라서 여기서 정의된 struct의 경우 pod형이며 fd형(기본 자료형 int, double etc..)이 아닌 경우 struct로 구분하도록 한다.
        즉 옛 c언어 스타일의 타입만 struct형으로 구별하겠다는 것이다.
    */
    enum class ETypeCLASS : uint8
    {
        Unknown,
        Void,
        Nullptr,
        Bool,
        Char,
        SChar,
        UChar,
        Char16,
        Char32,
        Int16,
        UInt16,
        Int32,
        UInt32,
        Int64,
        UInt64,
        Float,
        Double,
        LValObjectRef,
        LValFunctionRef,
        RValObjectRef,
        RValFunctionRef,
        ObjectPtr,
        FunctionPtr,
        MemberDataPtr,
        MemberFunctionPtr,
        Array,
        Function,
        Enum,
        EnumClass,
        Union,
        Struct, /**< it is pod type and not fd type. */
        Class
    };

    namespace _internal_lang_type
    {
        template<typename _Ty>
        constexpr ETypeCLASS FDTypeToETypeCLASS() noexcept
        {
            return ETypeCLASS::Unknown;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<void>() noexcept
        {
            return ETypeCLASS::Void;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<std::nullptr_t>() noexcept
        {
            return ETypeCLASS::Nullptr;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<bool>() noexcept
        {
            return ETypeCLASS::Bool;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<char>() noexcept
        {
            return ETypeCLASS::Char;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<signed char>() noexcept
        {
            return ETypeCLASS::SChar;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<unsigned char>() noexcept
        {
            return ETypeCLASS::UChar;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<char16_t>() noexcept
        {
            return ETypeCLASS::Char16;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<char32_t>() noexcept
        {
            return ETypeCLASS::Char32;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<int16>() noexcept
        {
            return ETypeCLASS::Int16;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<uint16>() noexcept
        {
            return ETypeCLASS::UInt16;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<int32>() noexcept
        {
            return ETypeCLASS::Int32;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<uint32>() noexcept
        {
            return ETypeCLASS::UInt32;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<int64>() noexcept
        {
            return ETypeCLASS::Int64;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<uint64>() noexcept
        {
            return ETypeCLASS::UInt64;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<float>() noexcept
        {
            return ETypeCLASS::Float;
        }

        template<>
        constexpr ETypeCLASS FDTypeToETypeCLASS<double>() noexcept
        {
            return ETypeCLASS::Double;
        }

        template<typename _Ty>
        constexpr ETypeCLASS RefTypeToETypeCLASS() noexcept
        {
            using namespace std;
            using Ty = _Ty;
            using seedTy = seed_traits_t<_Ty>;

            if constexpr (is_lvalue_reference_v<Ty>)
            {
                if (is_object_v<seedTy>)
                {
                    return ETypeCLASS::LValObjectRef;
                }

                return ETypeCLASS::LValFunctionRef;
            }
            else if (is_rvalue_reference_v<Ty>)
            {
                if (is_object_v<seedTy>)
                {
                    return ETypeCLASS::RValObjectRef;
                }

                return ETypeCLASS::RValFunctionRef;
            }
            else
            {
                ETypeCLASS::Unknown;
            }

            return ETypeCLASS::Unknown;
        }

        template<typename _Ty>
        constexpr ETypeCLASS PtrTypeToETypeCLASS() noexcept
        {
            using namespace std;
            using seedTy = seed_traits_t<_Ty>;

            if constexpr (is_function_v<seedTy>)
            {
                return ETypeCLASS::FunctionPtr;
            }
            else if (is_object_v<seedTy>)
            {
                return ETypeCLASS::ObjectPtr;
            }
            else
            {
                return ETypeCLASS::Unknown;
            }

            return ETypeCLASS::Unknown;
        }

        template<typename _Ty>
        constexpr ETypeCLASS GetTypeCLASS() noexcept
        {
            /*
                _Ty형을 변형하지 않고 바로 유형을 구분하여 반환하도록 구현 할 것.
                만약 유형변경을 통한 판독이 필요하다면 외부 함수로 분리할 것.
            */
            using namespace std;
            using Ty = _Ty;

            if constexpr (is_fundamental_v<Ty>)
            {
                return FDTypeToETypeCLASS<Ty>();
            }
            // compound types
            else if (is_reference_v<Ty>)
            {
                return RefTypeToETypeCLASS<Ty>();
            }
            else if (is_pointer_v<Ty>)
            {
                return PtrTypeToETypeCLASS<Ty>();
            }
            else if (is_member_pointer_v<Ty>)
            {
                if (is_member_object_pointer_v<Ty>)
                {
                    return ETypeCLASS::MemberDataPtr;
                }

                return ETypeCLASS::MemberFunctionPtr;
            }
            else if (is_array_v<Ty>)
            {
                return ETypeCLASS::Array;
            }
            else if (is_function_v<Ty>)
            {
                return ETypeCLASS::Function;
            }
            else if (is_enum_v<Ty>)
            {
                if (is_scoped_enum_v<Ty>)
                {
                    return ETypeCLASS::EnumClass;
                }

                return ETypeCLASS::Enum;
            }
            else if (is_class_v<Ty>)
            {
                if (is_pod_v<Ty>)
                {
                    return ETypeCLASS::Struct;
                }

                return ETypeCLASS::Class;
            }
            else if (is_union_v<Ty>)
            {
                return ETypeCLASS::Union;
            }
            else
            {
                return ETypeCLASS::Unknown;
            }

            return ETypeCLASS::Unknown;
        }
    }

    /**
        @brief 타입을 전달받아 언어에서 구분한 타입유형의 열거형으로 반환합니다.
        구별되지 않는 유형은 Unknown값을 반환합니다.
    */
    template<typename _Ty>
    constexpr ETypeCLASS GetTypeCLASS() noexcept
    {
        return _internal_lang_type::GetTypeCLASS<_Ty>();
    }
}
