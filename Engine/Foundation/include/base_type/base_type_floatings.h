/**
    @file BaseType_Floatings.h
    @brief Platform 이식성을 높이기 위해 실수 기본 원자타입을 정의하며 fp16(Soft)을 제공
    @details
    알려진 fp32(float), fp64(double)을 정의함과 동시에 최근 사용율이 높아진 fp16 type을 정의합니다.
    fp16의 경우 C++에서 미지원 자료형이므로 직접 구현하며 지금은 하드웨어 가속을 지원하지 않습니다.

    @remark fp16
    모든 소프트웨어 연산은 fp16->fp32로 변환 후 연산을 진행하며 다시 fp32->fp16으로 변환합니다.

    fp16 구조 참조
    https://en.wikipedia.org/wiki/Half-precision_floating-point_format

    요약 Format
    Sign bit : 1 bit | Exponent width : 5 bits | Significand precision : 11 bits(10 explicitly stored)
    set(DOC_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/share/doc/${PROJECT_NAME}" CACHE PATH "Path to the documentation")

install(FILES readme.md
        DESTINATION "${DOC_INSTALL_DIR}"
        COMPONENT doc)

install(DIRECTORY include/rapidjson
    DESTINATION "${INCLUDE_INSTALL_DIR}"
    COMPONENT dev)

install(DIRECTORY example/
    DESTINATION "${DOC_INSTALL_DIR}/examples"
    COMPONENT examples
    # Following patterns are for excluding the intermediate/object files
    # from an install of in-source CMake build.
    PATTERN "CMakeFiles" EXCLUDE
    PATTERN "Makefile" EXCLUDE
    PATTERN "cmake_install.cmake" EXCLUDE)
    FILES_MATCHING PATTERN "*.h"

*/
#pragma once


#include <type_traits>


namespace fd
{
    namespace base_type
    {
        ///*
        //    기본으로 사용될 실수 원자 타입에 대한 크기 확인
        //*/
        //static_assert(sizeof(float)  == 4, "float type size isn't 4byte!");
        //static_assert(sizeof(double) == 8, "double type size isn't 8byte!");

        ///*
        //    정규화된 실수형 타입 정의
        //*/
        //using fp32 = float;
        //using fp64 = double;

        ///*
        //    fp16의 기반이 되는 저수준 강타입.
        //    이 타입은 POD이며 2바이트 기준으로 정렬
        //*/
        //struct alignas(2) raw_fp16 final
        //{
        //    unsigned __int16 encoded;
        //};

        //// Check _FP16RawType
        //static_assert((sizeof(raw_fp16) == 2), "_FP16RawType type size isn't 2byte!");
        //static_assert((std::is_pod<raw_fp16>::value == true), "_FP16RawType type must be pod type");


        //template<typename _Ty>
        //struct fp_type_traits;

        //template<>
        //struct fp_type_traits<fp32>
        //{
        //    union Bits
        //    {
        //        float   f;
        //        int32  si;
        //        uint32 ui;
        //    };

        //    static constexpr int32 shift = 13;
        //    static constexpr int32 shiftSign = 16;

        //    static constexpr int32 infN  = 0x7F800000; // flt32 infinity
        //    static constexpr int32 maxN  = 0x477FE000; // max flt16 normal as a flt32
        //    static constexpr int32 minN  = 0x38800000; // min flt16 normal as a flt32
        //    static constexpr int32 signN = 0x80000000; // flt32 sign bit

        //    static constexpr int32 infC = infN >> shift;
        //    static constexpr int32 nanN = (infC + 1) << shift; // minimum flt16 nan as a flt32
        //    static constexpr int32 maxC = maxN >> shift;
        //    static constexpr int32 minC = minN >> shift;
        //    static constexpr int32 signC = signN >> shiftSign; // flt16 sign bit

        //    static constexpr int32 mulN = 0x52000000; // (1 << 23) / minN
        //    static constexpr int32 mulC = 0x33800000; // minN / (1 << (23 - shift))

        //    static constexpr int32 subC = 0x003FF;    // max flt32 subnormal down shifted
        //    static constexpr int32 norC = 0x00400;    // min flt32 normal down shifted

        //    static constexpr int32 maxD = infC - maxC - 1;
        //    static constexpr int32 minD = minC - subC - 1;
        //};

        //template<typename _ChildTy>
        //class FP16ControlBase
        //{
        //public:
        //    /** Positive infinity 반환 */
        //    static constexpr raw_fp16 GetInfinity() noexcept
        //    {
        //        /*
        //            Infinity State
        //            0 11111 0000000000 = infinity
        //            1 11111 0000000000 = −infinity
        //        */
        //        return raw_fp16{ 0b'0'11111'0000000000 };
        //    }

        //    /*
        //        NaN의 경우 2가지 상태(qnan, snan)가 있으며
        //        Inf 상태에서 SP의 수가 0이 아닐경우를 의미함
        //    */

        //    /** QuietNan을 반환 */
        //    static constexpr raw_fp16 GetQuietNaN() noexcept
        //    {
        //        //‭0 11111 1000000000‬
        //        return raw_fp16{ 0b'0'11111'1000000000 };
        //    }

        //    /** 초기화되지 않은 변수나 표준에서 벗어난 연산 등을 위한 SignalingNaN을 반환 */
        //    static constexpr raw_fp16 GetSignalingNaN() noexcept
        //    {
        //        return raw_fp16{ 0b'0'11111'1000000001 };
        //    }

        //    raw_fp16 GetFP16FromFP32(const fp32 f32) const noexcept;
        //    fp32 GetFP32FromFP16(const raw_fp16 rf16) const noexcept;
        //};

        //inline namespace _detail_v1
        //{
        //    
        //}


        //enum class EFP16State
        //{
        //    Soft,
        //    Hard
        //};

        //EFP16State GetFP16State() noexcept;
        //EFP16State SetFP16State(const EFP16State state) noexcept;
        //FP16Control& GetCurrentFP16Control() noexcept;


        ///*
        //  fp16에 대한 통제를 담당하는 제어 클래스
        //*/
        //class FP16Control
        //{
        //public:

        //    /*
        //      변환
        //    */
        //    /*
        //      소프트웨어적으로 _FP16RawType => FP32 타입으로 변환하는 함수입니다.
        //      예외가 발생하지 않는 함수입니다.
        //      더 이상 사용되지 않습니다.
        //    */
        //    [[deprecated("Do not use v1 decode func")]]
        //    FLINE static constexpr fp32 ConvertFP16RawToFP32_Soft(const _FP16RawType) noexcept;

        //    /*
        //      소프트웨어적으로 _FP16RawType => FP32 타입으로 변환하는 함수입니다.
        //      예외가 발생하지 않는 함수입니다.
        //    */
        //    FLINE static constexpr fp32 ConvertFP16RawToFP32_Softv2(const _FP16RawType) noexcept;

        //    /*
        //      소프트웨어적으로 FP32 => _FP16RawType 타입으로 변환하는 함수입니다.
        //      예외가 발생하지 않는 함수입니다.
        //      더 이상 사용되지 않습니다.
        //    */
        //    [[deprecated("Do not use v1 encode func")]]
        //    FLINE static constexpr _FP16RawType ConvertFP32ToFP16Raw_Soft(const fp32) noexcept;

        //    /*
        //      소프트웨어적으로 FP32 => _FP16RawType 타입으로 변환하는 함수입니다.
        //      예외가 발생하지 않는 함수입니다.
        //    */
        //    FLINE static constexpr _FP16RawType ConvertFP32ToFP16Raw_Softv2(const fp32) noexcept;


        //    /*
        //      _FP16RawType타입이 Nan인지 확인하는 함수 입니다.
        //      예외가 발생하지 않는 함수입니다.
        //    */
        //    FLINE static bool IsNaN_FP16RawType(const _FP16RawType hf) noexcept
        //    {
        //        static FP16RawIsNaNStater& targetProc = _InitializeNaNStater();
        //        return targetProc(hf);
        //    }


        //    /*
        //      연산
        //    */
        //    FLINE static _FP16RawType AddFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        //fp32로 변환 후 연산
        //        return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) + ConvertFP16RawToFP32(rhs));
        //    }

        //    FLINE static _FP16RawType SubFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        //fp32로 변환 후 연산
        //        return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) - ConvertFP16RawToFP32(rhs));
        //    }

        //    FLINE static _FP16RawType MulFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        //fp32로 변환 후 연산
        //        return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) * ConvertFP16RawToFP32(rhs));
        //    }

        //    /*
        //      나누기 연산
        //      0으로 나눌 경우 예외 발생
        //    */
        //    FLINE static _FP16RawType DivFP16Raw_Soft(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        return ConvertFP32ToFP16Raw(ConvertFP16RawToFP32(lhs) / ConvertFP16RawToFP32(rhs));
        //    }

        //    /*
        //      _FP16RawType 덧셈 연산
        //    */
        //    FLINE static _FP16RawType AddFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        static FP16RawAddOperator& targetPrco = _InitializeAddOperator();
        //        return targetPrco(lhs, rhs);
        //    }
        //    FLINE static _FP16RawType SubFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        static FP16RawSubOperator& targetPrco = _InitializeSubOperator();
        //        return targetPrco(lhs, rhs);
        //    }
        //    FLINE static _FP16RawType MulFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        static FP16RawMulOperator& targetPrco = _InitializeMulOperator();
        //        return targetPrco(lhs, rhs);
        //    }
        //    FLINE static _FP16RawType DivFP16Raw(const _FP16RawType lhs, const _FP16RawType rhs) noexcept
        //    {
        //        static FP16RawDivOperator& targetPrco = _InitializeDivOperator();
        //        return targetPrco(lhs, rhs);
        //    }

        //};

        //constexpr fp32 FP16Control::ConvertFP16RawToFP32_Soft(const _FP16RawType hf) noexcept
        //{
        //    //         sing 1bit      ,   exponent 5bits                      ,   Significand 11bits
        //    fp32 f = static_cast<fp32>(((hf.encoded & 0x8000) << 16) | (((hf.encoded & 0x7c00) + 0x1C000) << 13) | ((hf.encoded & 0x03FF) << 13));

        //    return static_cast<fp32>(f);
        //}

        //constexpr _FP16RawType FP16Control::ConvertFP32ToFP16Raw_Soft(const fp32 f) noexcept
        //{
        //    unsigned __int32 x = *((unsigned __int32*)&f);

        //    return  _FP16RawType{ ((x >> 16) & 0x8000) | ((((x & 0x7f800000) - 0x38000000) >> 13) & 0x7c00) | ((x >> 13) & 0x03ff) };
        //}

        //constexpr fp32 FP16Control::ConvertFP16RawToFP32_Softv2(const _FP16RawType hf) noexcept
        //{
        //    /*
        //      https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
        //    */
        //    using fs = _fp32State;

        //    fs::Bits v{ 0.f };

        //    v.ui = hf.encoded;
        //    int32 sign = v.si & fs::signC;
        //    v.si ^= sign;
        //    sign <<= fs::shiftSign;
        //    v.si ^= ((v.si + fs::minD) ^ v.si) & -(v.si > fs::subC);
        //    v.si ^= ((v.si + fs::maxD) ^ v.si) & -(v.si > fs::maxC);

        //    fs::Bits s{ 0.f };
        //    s.si = fs::mulC;
        //    s.f *= v.si;
        //    int32 mask = -(fs::norC > v.si);
        //    v.si <<= fs::shift;
        //    v.si ^= (s.si ^ v.si) & mask;
        //    v.si |= sign;

        //    return v.f;
        //}

        //constexpr _FP16RawType FP16Control::ConvertFP32ToFP16Raw_Softv2(const fp32 f) noexcept
        //{
        //    /*
        //      https://stackoverflow.com/questions/1659440/32-bit-to-16-bit-floating-point-conversion
        //    */
        //    using fs = _fp32State;

        //    fs::Bits v{ f }, s{ 0.f };

        //    //v.f = value;
        //    uint32 sign = v.si & fs::signN;
        //    v.si ^= sign;
        //    sign >>= fs::shiftSign; // logical shift
        //    s.si = fs::mulN;
        //    s.si = static_cast<int32>(s.f * v.f);       // correct subnormals
        //    v.si ^= (s.si ^ v.si) & -(fs::minN > v.si);
        //    v.si ^= (fs::infN ^ v.si) & -((fs::infN > v.si) & (v.si > fs::maxN));
        //    v.si ^= (fs::nanN ^ v.si) & -((fs::nanN > v.si) & (v.si > fs::infN));
        //    v.ui >>= fs::shift;     // logical shift
        //    v.si ^= ((v.si - fs::maxD) ^ v.si) & -(v.si > fs::maxC);
        //    v.si ^= ((v.si - fs::minD) ^ v.si) & -(v.si > fs::subC);

        //    return _FP16RawType{ static_cast<uint16>(v.ui | sign) };
        //}

        //bool FP16Control::IsNaN_FP16RawType_Soft(const _FP16RawType hf) noexcept
        //{
        //    /*
        //    Nan State
        //    0 11111 1000000000 = qnan
        //    1 11111 1000000001 = snan

        //    두개의 상태를 다 확인해야함.
        //    */
        //    using uint16 = unsigned __int16;
        //    using uint32 = unsigned __int32;

        //    constexpr uint16 mask1 = _Get_FP16RawTypeQuietNaN().encoded;
        //    constexpr uint16 mask2 = _Get_FP16RawTypeSignalingNaN().encoded;

        //    union alignas(4) Board
        //    {
        //        struct
        //        {
        //            uint16 m1;
        //            uint16 m2;
        //        };
        //        uint32 m32;
        //    };

        //    constexpr Board masks{ mask1, mask2 };  //setup +, - mask
        //    Board target{ hf.encoded   , hf.encoded };

        //    target.m32 = (target.m32 & masks.m32);


        //    if ((target.m1 == masks.m1) || (target.m1 == masks.m2))
        //    {
        //        return true;
        //    }
        //    else
        //    {
        //        return false;
        //    }
        //}


        ///*
        //    Half-precision floating-point(2 Byte)
        //    이 타입은 POD이며 2바이트 기준으로 정렬됩니다.
        //*/
        //struct alignas(2) fp16 final
        //{
        //private:
        //    friend constexpr fp16 operator""hf(long double d) noexcept;

        //public:
        //    constexpr operator float() const noexcept;

        //    constexpr fp16& operator=(const float& f) noexcept;
        //    constexpr fp16& operator/(const fp16& rhs);

        //private:
        //    _FP16RawType m_fp16RawData;
        //};

        //constexpr fp16::operator float() const noexcept
        //{
        //    return static_cast<float>(FP16Control::ConvertFP16RawToFP32(m_fp16RawData));
        //}

        //fp16& fp16::operator=(const float& f) noexcept
        //{
        //    m_fp16RawData = FP16Control::ConvertFP32ToFP16Raw(f);
        //    return *this;
        //}

        //fp16& fp16::operator/(const fp16& rhs)
        //{
        //    m_fp16RawData = FP16Control::DivFP16Raw(this->m_fp16RawData, rhs.m_fp16RawData);
        //    return *this;
        //}

        ///**
        //    fp16(half-floating) literal 컴파일 타임 변환을 보장
        //*/
        //constexpr fp16 operator""hf(long double d) noexcept
        //{
        //    fp16 f{};
        //    f.m_fp16RawData = FP16Control::ConvertFP32ToFP16Raw_Softv2(static_cast<float>(d));

        //    return f;
        //}

        //// fp16 alias
        //using hfloat = fp16;  

        ///*
        //    완성된 fp16에 대한 정적 검사
        //*/
        //static_assert((sizeof(fp16) == 2), "fp16 type size isn't 2byte!");
        //static_assert((std::is_pod<fp16>::value == true), "fp16 type must be pod type");
        //static_assert((std::is_same<fp16, hfloat>::value == true), "fp16 and hfloat type is not same");

        ///*
        //    helper
        //*/
        //constexpr bool isnan(const fp16 hf) noexcept
        //{
        //    return true;
        //    //return GYP::Engine::Core::BaseTypes::FP16IsNan(hf);
        //}
        //constexpr fp16 nanhf() noexcept
        //{
        //    return fp16{};
        //    //return GYP::Engine::Core::BaseTypes::GetFP16QuietNan();
        //}

        ////template<typename _Ty>
        ////class numeric_limits;

        ////template<>
        ////class numeric_limits<fp16>
        ////{

        ////};
    }
}

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
