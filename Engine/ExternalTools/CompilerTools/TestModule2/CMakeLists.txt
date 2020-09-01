cmake_minimum_required(VERSION 3.7)

#list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../../CMake")
#include(CommonPathEnv)

#set(LAB_VCPKG_ROOT_PATH "${LAB_INSTALLED_PATH}/Vcpkg2")
#get_filename_component(LAB_VCPKG_ROOT_PATH ${LAB_VCPKG_ROOT_PATH} ABSOLUTE)
#set(LAB_VCPKG_TOOL_CHAIN_FILE_ scripts/buildsystems/vcpkg.cmake)

#set(CMAKE_TOOLCHAIN_FILE "${LAB_VCPKG_ROOT_PATH}/${LAB_VCPKG_TOOL_CHAIN_FILE_}")
#mark_as_advanced(CMAKE_TOOLCHAIN_FILE)

message("2TOOL CHAIN=${CMAKE_TOOLCHAIN_FILE}")

#set(VCPKG_TARGET_TRIPLET x64-windows)
#message("${VCPKG_TARGET_TRIPLET}")

#include(VcpkgManager)
#VcpkgManager_ready()
#VcpkgManager_integrateCleanly()
#VcpkgManager_printReadyState()
#VcpkgManager_printIntegrationState()


project(TestModule2)

find_package(TBB CONFIG REQUIRED)

add_executable(TestModule2
    main.cpp
)

target_link_libraries(TestModule2 PRIVATE TBB::tbb)


 add_subdirectory(TestModule3)