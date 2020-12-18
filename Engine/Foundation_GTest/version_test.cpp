#include <gtest/gtest.h>

#include <fd/version.h>


TEST(version_api_test, version_compare_test)
{
    using namespace fd;

    //test version
    SVersionNumber v1{ 1, 2, 3 };
    SVersionNumber v2{ 1, 3, 2 };

    EXPECT_EQ(true, v1 < v2);
    EXPECT_EQ(true, v1 <= v2);

    EXPECT_EQ(false, v1 > v2);
    EXPECT_EQ(false, v1 >= v2);

    EXPECT_EQ(false, v1 == v2);
    EXPECT_EQ(true,  v1 != v2);


    SVersionNumber v3{ 2, 1, 4 };
    SVersionNumber v4{ 2, 1, 1 };

    EXPECT_EQ(false, v3 < v4);
    EXPECT_EQ(false, v3 <= v4);

    EXPECT_EQ(true, v3 > v4);
    EXPECT_EQ(true, v3 >= v4);

    EXPECT_EQ(false, v3 == v4);
    EXPECT_EQ(true,  v3 != v4);


    //v1, v3
    EXPECT_EQ(true, v1 < v3);
    EXPECT_EQ(true, v1 <= v3);

    EXPECT_EQ(false, v1 > v3);
    EXPECT_EQ(false, v1 >= v3);

    EXPECT_EQ(false, v1 == v3);
    EXPECT_EQ(true,  v1 != v3);


    //v1 == v5
    SVersionNumber v5{ 1, 2, 3 };

    EXPECT_EQ(false,v1 < v5);
    EXPECT_EQ(true, v1 <= v5);

    EXPECT_EQ(false, v1 > v5);
    EXPECT_EQ(true, v1 >= v5);

    EXPECT_EQ(true, v1 == v5);
    EXPECT_EQ(false,v1 != v5);
}