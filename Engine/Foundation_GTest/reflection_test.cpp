#include <gtest/gtest.h>

#include <fd/reflection/lang_type.h>
#include <fd/reflection/type_name.h>
#include <fd/version.h>


TEST(type_name_test, get_type_name_test)
{
    /* 
        fd의 고정길이 타입에 대해서 검증안함. 이유는 크기만 보장하기 때문.
    */


    using namespace fd::refl;

    EXPECT_EQ("fd::refl::ETypeCLASS", GetTypeFullName<ETypeCLASS>());
    EXPECT_EQ("fd::SVersionInfo", GetTypeFullName<fd::SVersionInfo>());


    {
        using namespace fd;

        EXPECT_EQ("fd::SVersionInfo", GetTypeFullName<SVersionInfo>());
    }
}

TEST(lang_type_test, fd_type_test)
{
    using namespace fd::refl;

    EXPECT_EQ(ETypeCLASS::Void, GetTypeCLASS<void>());

    EXPECT_EQ(ETypeCLASS::Nullptr, GetTypeCLASS<decltype(nullptr)>());

    EXPECT_EQ(ETypeCLASS::Bool, GetTypeCLASS<bool>());

    EXPECT_EQ(ETypeCLASS::Char, GetTypeCLASS<char>());
    EXPECT_EQ(ETypeCLASS::SChar, GetTypeCLASS<signed char>());
    EXPECT_EQ(ETypeCLASS::UChar, GetTypeCLASS<unsigned char>());
    EXPECT_EQ(ETypeCLASS::Char16, GetTypeCLASS<char16_t>());
    EXPECT_EQ(ETypeCLASS::Char32, GetTypeCLASS<char32_t>());

    EXPECT_EQ(ETypeCLASS::Int16, GetTypeCLASS<fd::int16>());
    EXPECT_EQ(ETypeCLASS::UInt16, GetTypeCLASS<fd::uint16>());
    EXPECT_EQ(ETypeCLASS::Int32, GetTypeCLASS<fd::int32>());
    EXPECT_EQ(ETypeCLASS::UInt32, GetTypeCLASS<fd::uint32>());
    EXPECT_EQ(ETypeCLASS::Int64, GetTypeCLASS<fd::int64>());
    EXPECT_EQ(ETypeCLASS::UInt64, GetTypeCLASS<fd::uint64>());

    EXPECT_EQ(ETypeCLASS::Float, GetTypeCLASS<float>());
    EXPECT_EQ(ETypeCLASS::Double, GetTypeCLASS<double>());
    EXPECT_EQ(ETypeCLASS::Unknown, GetTypeCLASS<long double>());
}

namespace _internal_reflection_test
{
    void ItIsFunc() {}
    void ItIsFunc(int) {}

    void ItIsFuncNoexcept() noexcept {}

    template<typename... Ts>
    using ItIsFuncType = decltype(ItIsFunc(std::declval<Ts>()...))(Ts...);
}

TEST(lang_type_test, compound_type_test)
{
    using namespace fd::refl;
    using namespace _internal_reflection_test;

    struct StructType {};
    union UnionType {};
    class ClassTypeButStructType {};
    class ClassType
    {
    public:
        ~ClassType() {}

        void Method() {};

        fd::int32 m_member;
    };
    enum EnumType {};
    enum class EnumClassType {};

    // 구조
    EXPECT_EQ(ETypeCLASS::Struct, GetTypeCLASS<StructType>());
    EXPECT_EQ(ETypeCLASS::Union, GetTypeCLASS<UnionType>());
    EXPECT_EQ(ETypeCLASS::Struct, GetTypeCLASS<ClassTypeButStructType>());
    EXPECT_EQ(ETypeCLASS::Class, GetTypeCLASS<ClassType>());
    EXPECT_EQ(ETypeCLASS::Enum, GetTypeCLASS<EnumType>());
    EXPECT_EQ(ETypeCLASS::EnumClass, GetTypeCLASS<EnumClassType>());

    // 배열
    EXPECT_EQ(ETypeCLASS::Array, GetTypeCLASS<int[]>());
    EXPECT_EQ(ETypeCLASS::Array, GetTypeCLASS<ClassType[]>());
    EXPECT_EQ(ETypeCLASS::Array, GetTypeCLASS<int* []>());
    EXPECT_EQ(ETypeCLASS::Array, GetTypeCLASS<ClassType* []>());

    // 함수
    EXPECT_EQ(ETypeCLASS::Function, GetTypeCLASS<ItIsFuncType<>>());
    EXPECT_EQ(ETypeCLASS::Function, GetTypeCLASS<ItIsFuncType<int>>());
    EXPECT_EQ(ETypeCLASS::Function, GetTypeCLASS<decltype(ItIsFuncNoexcept)>());

    // 포인터
        // 객체
    EXPECT_EQ(ETypeCLASS::ObjectPtr, GetTypeCLASS<const int*>());
    EXPECT_EQ(ETypeCLASS::ObjectPtr, GetTypeCLASS<int*>());
    EXPECT_EQ(ETypeCLASS::ObjectPtr, GetTypeCLASS<const ClassType*>());
    EXPECT_EQ(ETypeCLASS::ObjectPtr, GetTypeCLASS<ClassType*>());
        // 함수
    EXPECT_EQ(ETypeCLASS::FunctionPtr, GetTypeCLASS<ItIsFuncType<>*>());
    EXPECT_EQ(ETypeCLASS::FunctionPtr, GetTypeCLASS<ItIsFuncType<int>*>());
    EXPECT_EQ(ETypeCLASS::FunctionPtr, GetTypeCLASS<decltype(&ItIsFuncNoexcept)>());
        // 객체 데이터 맴버
    EXPECT_EQ(ETypeCLASS::Int32, GetTypeCLASS<decltype(ClassType::m_member)>());
    EXPECT_EQ(ETypeCLASS::MemberDataPtr, GetTypeCLASS<decltype(&ClassType::m_member)>());
    EXPECT_EQ(ETypeCLASS::MemberFunctionPtr, GetTypeCLASS<decltype(&ClassType::Method)>());

    // 참조유형 추론
        // L value
    EXPECT_EQ(ETypeCLASS::LValObjectRef, GetTypeCLASS<const int&>());
    EXPECT_EQ(ETypeCLASS::LValObjectRef, GetTypeCLASS<int&>());
    EXPECT_EQ(ETypeCLASS::LValObjectRef, GetTypeCLASS<ClassType&>());
    EXPECT_EQ(ETypeCLASS::LValObjectRef, GetTypeCLASS<ClassType*&>());
    EXPECT_EQ(ETypeCLASS::LValObjectRef, GetTypeCLASS<ClassType(&)[]>());
    EXPECT_EQ(ETypeCLASS::LValObjectRef, GetTypeCLASS<decltype(&ItIsFuncNoexcept)&>());
    EXPECT_EQ(ETypeCLASS::LValFunctionRef, GetTypeCLASS<decltype(ItIsFuncNoexcept)&>());
    
        // R value
    EXPECT_EQ(ETypeCLASS::RValObjectRef, GetTypeCLASS<const int&&>());
    EXPECT_EQ(ETypeCLASS::RValObjectRef, GetTypeCLASS<int&&>());
    EXPECT_EQ(ETypeCLASS::RValObjectRef, GetTypeCLASS<ClassType&&>());
    EXPECT_EQ(ETypeCLASS::RValObjectRef, GetTypeCLASS<ClassType*&&>());
    EXPECT_EQ(ETypeCLASS::RValObjectRef, GetTypeCLASS<ClassType(&&)[]>());
    EXPECT_EQ(ETypeCLASS::RValObjectRef, GetTypeCLASS<decltype(&ItIsFuncNoexcept)&&>());
    EXPECT_EQ(ETypeCLASS::RValFunctionRef, GetTypeCLASS<decltype(ItIsFuncNoexcept)&&>());
}