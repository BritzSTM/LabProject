# 사용자 스토리
# 1. 사용자가 vcpkg를 사용하기위해 VcpkgManager.cmake를 로드한다
# 2. 사용자가 VcpkgManager에 배열로 필요 라이브러리를 요구한다
# 3. VcpkgManager는 지정된경로가 없으면 디폴트 경로에 vcpkg를 설치한다
# 4. VcpkgManager는 지정된폴더명이 없으면 Vcpkg라는 이름의 폴더를 기본값으로 사용한다

# 기타사항
# 1. vcpkg의 빌드 기본출력이 repository root folder
# 2. .vcpkg-root와 vcpkg file로 Manager의 유효상황을 판별할 수 있다
# 3. 환경변수로 처리하면 윈도우에서는 explorer가 재로딩되어야 한다.
#    즉 로그아웃 되어야 함. 환경변수로 VcpkgManager의 유효성 판별은 옳지 못하다 
# 4. bootstrap-vcpkg을 통해 실행파일을 repository root folder에 생성할 수 있다

# How to use
# 1. LAB_VCPKG_ROOT_PATH
#
#
#
cmake_minimum_required(VERSION 3.7)

set(VCPKG_MANAGER_VERSION 0.2.1)
message(STATUS "VcpkgManager ${VCPKG_MANAGER_VERSION}")


# include common path env
include(CommonPathEnv)
if(DEFINED LAB_INSTALLED_PATH)
    message(STATUS "Found installed dir path=${LAB_INSTALLED_PATH}")

else()
    message(FATAL_ERROR "Not Found installed dir Path")

endif()


#
# variable section
#
# public variables
if(NOT DEFINED LAB_VCPKG_ROOT_PATH)
    set(LAB_VCPKG_ROOT_PATH "${LAB_INSTALLED_PATH}/Vcpkg")
    message(STATUS "Used default LAB_VCPKG_ROOT_PATH")

endif()

# prviate variables
set(LAB_VCPKG_ROOT_SIMBOL_FILE_NAME .vcpkg-root)
set(LAB_VCPKG_EXE_FILE_NAME vcpkg)

# git repository clone
set(LAB_VCPKG_REPO_URL https://github.com/Microsoft/vcpkg.git)
set(LAB_VCPKG_GIT_CLONE_CMD "git")
set(LAB_VCPKG_GIT_CLONE_ARG clone;--depth;1;${LAB_VCPKG_REPO_URL};./;)
set(LAB_VCPKG_GIT_CLONE_CMD_SUCCEED 0)
set(LAB_VCPKG_BOOTSTRAP_CMD_SUCCEED 0)


#
# function section
#
macro(printVcpkgVar)
    message(STATUS "LAB_VCPKG_ROOT_PATH=${LAB_VCPKG_ROOT_PATH}")
    message(STATUS "LAB_VCPKG_ROOT_SIMBOL_FILE_NAME=${LAB_VCPKG_ROOT_SIMBOL_FILE_NAME}")
    message(STATUS "LAB_VCPKG_EXE_FILE_NAME=${LAB_VCPKG_EXE_FILE_NAME}")
    message(STATUS "LAB_VCPKG_EXE_FILE_EXT=${LAB_VCPKG_EXE_FILE_EXT}")

endmacro()

macro(printVcpkgRepoVar)
    message(STATUS "LAB_VCPKG_REPO_URL=${LAB_VCPKG_REPO_URL}")
    message(STATUS "Vcpkg clone cmd=${LAB_VCPKG_GIT_CLONE_CMD} ${LAB_VCPKG_GIT_CLONE_ARG}")

endmacro()

macro(MatchVcpkgRepo)
    if(EXISTS "${LAB_VCPKG_DOWNLOAD_PATH}/${LAB_VCPKG_GIT_REPO_SIMBOL}")
        message(STATUS "Found Vcpkg Repository")
        return()
    endif()
endmacro()

# 전달받은 경로에 vcpkg를 다운받습니다. 실패시 FATAL_ERROR
function(downloadVcpkg DOWNLOAD_PATH_ARG)
    execute_process(
      COMMAND ${LAB_VCPKG_GIT_CLONE_CMD} ${LAB_VCPKG_GIT_CLONE_ARG}
      WORKING_DIRECTORY ${DOWNLOAD_PATH_ARG}
      RESULT_VARIABLE GIT_RESULT
      OUTPUT_VARIABLE GIT_OUTPUT
    )

    # check succeed git clone
    if(NOT ${GIT_RESULT} EQUAL ${LAB_VCPKG_GIT_CLONE_CMD_SUCCEED})
        message(FATAL_ERROR "Failed vcpkg startup process. Git[${GIT_RESULT}]: ${GIT_OUTPUT}")

    else()
        message(STATUS "Git[${GIT_RESULT}]: ${GIT_OUTPUT}")

    endif()
endfunction()

# 전달받은 경로에 있는 vcpkg를 설치합니다. 실패시 FATAL_ERROR
function(setupVcpkg DOWNLOADED_PATH_ARG)
    if(WIN32)
        set(LAB_VCPKG_BOOTSTRAP_FILE_PATH bootstrap-vcpkg.bat)

    else()
        set(LAB_VCPKG_BOOTSTRAP_FILE_PATH bootstrap-vcpkg.sh)

    endif()

    message(STATUS "VcpkgBootstrap...... wait for few minutes")
    execute_process(
      COMMAND ${LAB_VCPKG_BOOTSTRAP_FILE_PATH}
      WORKING_DIRECTORY ${DOWNLOADED_PATH_ARG}
      RESULT_VARIABLE BOOTSTRAP_RESULT
      OUTPUT_VARIABLE BOOTSTRAP_OUTPUT
      )

    # check succeed
    if(NOT ${BOOTSTRAP_RESULT} EQUAL ${LAB_VCPKG_BOOTSTRAP_CMD_SUCCEED})
        message(FATAL_ERROR "Failed VcpkgBootstrap[${BOOTSTRAP_RESULT}]:${BOOTSTRAP_OUTPUT}")

    else()
        message(STATUS "VcpkgBootstrap[${BOOTSTRAP_RESULT}]:${BOOTSTRAP_OUTPUT}")

    endif()
endfunction()

#
# work process section
#

printVcpkgVar()
printVcpkgRepoVar()
#downloadVcpkg(${LAB_VCPKG_ROOT_PATH})
#setupVcpkg(${LAB_VCPKG_ROOT_PATH})