#include <gtest/gtest.h>

#include <fd/base_types.h>
#include <fd/type_traits_ext.h>


namespace _internal_type_traits_ext_test
{
    void TestFunc() {}

    using TestFuncPtr = decltype(&TestFunc);

    class CTestClass
    {
    public:
        ~CTestClass() {}

        void Method() {};

        fd::int32 m_member;
    };
}

TEST(type_traits_ext_test, seed_type_traits_test)
{
    using namespace fd;
    using namespace _internal_type_traits_ext_test;

    // ÇÔ¼ö Æ÷ÀÎÅÍ ºØ±«
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr*>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr**>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr*>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr**>, decltype(TestFunc)>));

    // ÇÔ¼ö Æ÷ÀÎÅÍ ÂüÁ¶ ºØ±« -> ÇÔ¼ö
    // L
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr*&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr**&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr***&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr*&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr**&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr***&>, decltype(TestFunc)>));
    // R
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr*&&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr**&&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<TestFuncPtr***&&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr*&&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr**&&>, decltype(TestFunc)>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const TestFuncPtr***&&>, decltype(TestFunc)>));


    // ±âº» ÀÚ·áÇü Æ÷ÀÎÅÍ ºØ±«
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int*>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int**>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int*>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int**>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int* const>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int** const>, int>));

    // ÂüÁ¶ ºØ±«
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int&>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int*&>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int**&>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int&>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int*&>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const int**&>, int>));

    // ¹è¿­ ºØ±«
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int[]>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int*[]>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int(&)[]>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int*(&)[]>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int[2][2]>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int* [2][2]>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int(&)[2][2]>, int>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<int*(&)[2][2]>, int>));

    //Å¬·¡½º ¸É¹ö
    using class_mem_ptr = decltype(&CTestClass::m_member);
    
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_ptr>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_ptr>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_ptr*>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_ptr**>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_ptr*&>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_ptr**&>, class_mem_ptr>));

    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_ptr>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_ptr*>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_ptr**>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_ptr*&>, class_mem_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_ptr**&>, class_mem_ptr>));
    
    using class_mem_fn_ptr = decltype(&CTestClass::Method);

    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr*>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr**>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr*&>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr**&>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr*&&>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<class_mem_fn_ptr**&&>, class_mem_fn_ptr>));

    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_fn_ptr>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_fn_ptr*>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_fn_ptr**>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_fn_ptr*&>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_fn_ptr**&>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_fn_ptr*&&>, class_mem_fn_ptr>));
    EXPECT_TRUE((std::is_same_v<seed_traits_t<const class_mem_fn_ptr**&&>, class_mem_fn_ptr>));
}