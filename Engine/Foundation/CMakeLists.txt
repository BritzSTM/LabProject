cmake_minimum_required(VERSION 3.16)

# LIB version Info
set(LIB_VERSION_NAME "${PROJECT_NAME}")
set(LIB_FEATURES_INFO "")
set(LIB_MAJOR_VERSION "0")
set(LIB_MINOR_VERSION "0")
set(LIB_PATCH_VERSION "5")
set(LIB_VERSION_STRING "${LIB_MAJOR_VERSION}.${LIB_MINOR_VERSION}.${LIB_PATCH_VERSION}")


project(Foundation)
message(STATUS "${PROJECT_NAME} Static LIB(${CMAKE_BUILD_TYPE}) ${LIB_VERSION_STRING}")

# Minimal Lang version
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Path
set(FD_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(FD_VCPKG_PORT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/VcpkgPort")

# Build Delegation
# 하위 폴더에서 설치까지 다 알아서 처리할 것
# ! 하위 폴더들은 자동 생성파일이 아니라면 GLOB을 통한 파일 등록을 하지 말 것.
#   대신 target_sources로 등록할 것
add_library(${PROJECT_NAME} STATIC "")

add_subdirectory(${FD_SOURCE_DIR})
#add_subdirectory(${FD_VCPKG_PORT_DIR})

# Install
set(FD_INCLUDE_INSTALL_DIR "${CMAKE_INSTALL_PREFIX}/include/fd" CACHE PATH "Header files install target")
set(FD_LIB_INSTALL_DIR     "${CMAKE_INSTALL_PREFIX}/lib" CACHE STRING "Lib file install target")

set_target_properties(${PROJECT_NAME} PROPERTIES DEBUG_POSTFIX "_${CMAKE_BUILD_TYPE}")
set_target_properties(${PROJECT_NAME} PROPERTIES RELEASE_POSTFIX "_${CMAKE_BUILD_TYPE}")

install (
    TARGETS ${PROJECT_NAME}
    LIBRARY DESTINATION "${FD_LIB_INSTALL_DIR}")


set(FD_INSTALL_EXCLUDE_REGEX "out|VcpkgPort|\.vs")
install(
    DIRECTORY "${FD_SOURCE_DIR}" DESTINATION "${FD_INCLUDE_INSTALL_DIR}"
    FILES_MATCHING
    PATTERN "*.h"
    PATTERN "*.cpp" EXCLUDE
    PATTERN "*.in" EXCLUDE
    REGEX ${FD_INSTALL_EXCLUDE_REGEX} EXCLUDE)
