cmake_minimum_required(VERSION 3.16)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


project(Foundation_Test)

enable_testing()
add_executable(main
	main.cpp
	version_test.cpp
	reflection_test.cpp
	type_traits_ext_test.cpp)

find_package(Foundation CONFIG REQUIRED)
target_link_libraries(main PRIVATE Foundation)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(main PRIVATE GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main)


add_test(AllTestsInMain main)