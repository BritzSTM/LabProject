# 실험을 위한 파일
cmake_minimum_required(VERSION 3.7)
# ^ !현재 실시간 편집기에서 통합이 진행이 안된다.
# test url https://github.com/BritzSTM/SimpleTest.git

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/../../CMake")
include(CommonPathEnv)
include(GitUtils)


GitUtils_Clone(
    URL https://github.com/BritzSTM/SimpleTest.git
    PATH c:/DockerShare/SimpleTest
    HASH ABCDEFG1234
    DEPTH 01

)

#
#set(LAB_VCPKG_ROOT_PATH "${LAB_INSTALLED_PATH}/Vcpkg")
#get_filename_component(LAB_VCPKG_ROOT_PATH ${LAB_VCPKG_ROOT_PATH} ABSOLUTE)
#set(LAB_VCPKG_TOOL_CHAIN_FILE_ scripts/buildsystems/vcpkg.cmake)
#set(CMAKE_TOOLCHAIN_FILE "${LAB_VCPKG_ROOT_PATH}/${LAB_VCPKG_TOOL_CHAIN_FILE_}" CACHE STRING "")


#include(VcpkgManager)

#VcpkgManager_ready(
#   ROOT_PATH vcpkg2 
#    GIT_URL google.com
#    )

project(Total_External)

#message("0TOOL CHAIN=${CMAKE_TOOLCHAIN_FILE}")
#add_subdirectory(IdleReflector)
#add_subdirectory(TestModule2)
        #message(
        #    FATAL_ERROR 
        #    "Failed LAB_VCPKG_ROOT_PATH setup. check VcpkgManager path definition")

# ready 인터페이스 변경 경로, URL hash를 받아서 프랜치 가능하도록
# require 인터페이스 생성


#don used
#include_directories
#add_definitions
#link_libraries
#외부 사용금지
#target_include_directories()