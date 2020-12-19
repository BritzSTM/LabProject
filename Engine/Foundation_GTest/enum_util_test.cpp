#include <gtest/gtest.h>

#include <fd/enum_util.h>

#include <string>


namespace _internal_enul_util_test
{
    enum class ETest
    {
        Unknown, A, B, C, D, E, F, G, H, I, J, K, L, M, N, O, P, Q, R
    };
}

TEST(enum_util_test, ecTo_api_test)
{
    using namespace fd;
    using _internal_enul_util_test::ETest;

    std::string arr{ "\nABCDEFGHIJKLMNOPQR" };

    EXPECT_TRUE('A' == arr[ecTo(ETest::A)]);
    EXPECT_TRUE('D' == arr[ecTo(ETest::D)]);
    EXPECT_TRUE('K' == arr[ecTo(ETest::K)]);
    EXPECT_TRUE('O' == arr[ecTo(ETest::O)]);
    EXPECT_TRUE('R' == arr[ecTo(ETest::R)]);
}

TEST(enum_util_test, enum_group_test)
{
    using _internal_enul_util_test::ETest;
    using fd::enum_group;

    using sub1_group = enum_group<ETest, ETest::Unknown, ETest::B, ETest::C, ETest::D>;
    using sub2_group = enum_group<ETest, ETest::Unknown, ETest::A, ETest::D, ETest::E>;

    EXPECT_TRUE(ETest::Unknown == sub1_group(ETest::A));
    EXPECT_TRUE(ETest::B == sub1_group(ETest::B));
    EXPECT_TRUE(sub1_group(ETest::D) == sub2_group(ETest::D));
}