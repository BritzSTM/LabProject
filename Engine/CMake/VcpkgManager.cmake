# 사용자 스토리
# 1. 사용자가 vcpkg를 사용하기위해 VcpkgManager.cmake를 로드한다
# 2. 사용자가 VcpkgManager에 배열로 필요 라이브러리를 요구한다. (find_packages)
# 3. VcpkgManager는 지정된경로가 없으면 디폴트 경로에 vcpkg를 설치한다
# 4. VcpkgManager는 지정된폴더명이 없으면 Vcpkg라는 이름의 폴더를 기본값으로 사용한다

# 기타사항
# 1. vcpkg의 빌드 기본출력이 repository root folder
# 2. .vcpkg-root와 vcpkg file로 Manager의 유효상황을 판별할 수 있다
# 3. 환경변수로 처리하면 윈도우에서는 explorer가 재로딩되어야 한다.
#    즉 로그아웃 되어야 함. 환경변수로 VcpkgManager의 유효성 판별은 옳지 못하다 
# 4. bootstrap-vcpkg을 통해 실행파일을 repository root folder에 생성할 수 있다
# 5. 이 모듈을 적재한 이상 CMAKE_TOOLCHAIN_FILE은 vcpkg의 제공이 확정적이고 불변이다
# 6. 만약 추가 tool chain을 제공해야 한다면 VCPKG_CHAINLOAD_TOOLCHAIN_FILE을 제공
# 7. 독립적으로 동작하는 모듈이라고 생각하고 모든 의존성을 제거해라
# 8. $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake

# How to use
# 1. LAB_VCPKG_ROOT_PATH 변수 정의가 우선순위를 가지며 정의되어 있지 않다면
#    VCPKG_ROOT 시스템 환경변수를 찾습니다. 
#    만약 LAB_VCPKG_ROOT_PATH 변수를 정의하지 못한다면 FATAL_ERROR가 발생
cmake_minimum_required(VERSION 3.7)

#
# etc variables
#
set(VCPKG_MANAGER_VERSION 0.7)

#
# public variables
#
if(NOT DEFINED LAB_VCPKG_ROOT_PATH)
    if(DEFINED ENV{VCPKG_ROOT})
        set(LAB_VCPKG_ROOT_PATH $ENV{VCPKG_ROOT})
    else()
        message(
            FATAL_ERROR 
            "Failed LAB_VCPKG_ROOT_PATH setup. check VcpkgManager path definition")
    endif()
endif()
get_filename_component(LAB_VCPKG_ROOT_PATH ${LAB_VCPKG_ROOT_PATH} ABSOLUTE)

if(NOT DEFINED LAB_VCPKG_REPO_URL)
    set(LAB_VCPKG_REPO_URL https://github.com/Microsoft/vcpkg.git)
endif()

#
# prviate variables
#
# LAB_VCPKG_ROOT_PATH를 기준으로 작성함
set(LAB_VCPKG_ROOT_SIMBOL_FILE_ .vcpkg-root)

if(WIN32)
    set(LAB_VCPKG_EXE_FILE_ vcpkg.exe)
    set(LAB_VCPKG_BOOTSTRAP_FILE_ bootstrap-vcpkg.bat)
else()
    set(LAB_VCPKG_EXE_FILE_ vcpkg)
    set(LAB_VCPKG_BOOTSTRAP_FILE_ bootstrap-vcpkg.sh)
endif()

set(LAB_VCPKG_TOOL_CHAIN_FILE_ scripts/buildsystems/vcpkg.cmake)

set(LAB_VCPKG_MANAGER_READY_ FALSE)
set(LAB_VCPKG_MANAGER_INTEGRATION_ FALSE)


#
# private function
#
macro(VcpkgManager_printLoadedInfo_)
    message("Vcpkg manager ${VCPKG_MANAGER_VERSION}")
    message("Vcpkg manager loaded in ${CMAKE_PARENT_LIST_FILE}")
endmacro()

# git cmd의 명령어의 존재를 보장여부에 대한 계약 함수
function(VcpkgManager_ensureGitCmd_)
    set(GIT_WHERE_CMD where git)
    
    execute_process(
      COMMAND ${GIT_WHERE_CMD}
      RESULT_VARIABLE WHERE_RESULT
      OUTPUT_VARIABLE WHERE_OUTPUT
    )

    #found result is 0
    if(${WHERE_RESULT})
        message(FATAL_ERROR "Failed ensure git cmd")
    endif()
endfunction()

macro(VcpkgManager_ensureReadyState_)
    if(NOT ${LAB_VCPKG_MANAGER_READY_})
        message(FATAL_ERROR "Vcpkg manager not ready")
    endif()
endmacro()

macro(VcpkgManager_ensureintegrationState_)
    if(NOT ${LAB_VCPKG_MANAGER_INTEGRATION_})
        message(FATAL_ERROR "Vcpkg manager not integration")
    endif()
endmacro()

# 전달받은 경로에 vcpkg를 다운받습니다. 실패시 FATAL_ERROR
function(VcpkgManager_downloadVcpkg_ DOWNLOAD_PATH_ARG)
    set(LAB_VCPKG_GIT_CLONE_CMD "git")
    set(LAB_VCPKG_GIT_CLONE_ARG clone;--depth;1;${LAB_VCPKG_REPO_URL};./;)

    message(STATUS "Download Vcpkg from Git[${LAB_VCPKG_REPO_URL}]")
    execute_process(
      COMMAND ${LAB_VCPKG_GIT_CLONE_CMD} ${LAB_VCPKG_GIT_CLONE_ARG}
      WORKING_DIRECTORY ${DOWNLOAD_PATH_ARG}
      RESULT_VARIABLE GIT_RESULT
      OUTPUT_VARIABLE GIT_OUTPUT
    )

    # succeed is 0
    if(${GIT_RESULT})
        message(FATAL_ERROR "Failed vcpkg startup process. Git[${GIT_RESULT}]: ${GIT_OUTPUT}")
    else()
        message("Git[${GIT_RESULT}]: ${GIT_OUTPUT}")
    endif()
endfunction()

# 전달받은 경로에 있는 vcpkg를 설치합니다. 실패시 FATAL_ERROR
function(VcpkgManager_setupVcpkg_ DOWNLOADED_PATH_ARG)
    message(STATUS "VcpkgBootstrap...... wait for few minutes")

    execute_process(
      COMMAND ${LAB_VCPKG_BOOTSTRAP_FILE_}
      WORKING_DIRECTORY ${DOWNLOADED_PATH_ARG}
      RESULT_VARIABLE BOOTSTRAP_RESULT
      OUTPUT_VARIABLE BOOTSTRAP_OUTPUT
      )
     
    # succeed is 0
    if(${BOOTSTRAP_RESULT})
        message(FATAL_ERROR "Failed VcpkgBootstrap[${BOOTSTRAP_RESULT}]:${BOOTSTRAP_OUTPUT}")
    else()
        message("VcpkgBootstrap[${BOOTSTRAP_RESULT}]:${BOOTSTRAP_OUTPUT}")
    endif()
endfunction()

function(VcpkgManager_handleToolChain_ TARGET_VCPKG_ROOT_PATH_)
    if(NOT EXISTS "${TARGET_VCPKG_ROOT_PATH_}/${LAB_VCPKG_TOOL_CHAIN_FILE_}")
        message(FATAL_ERROR
            "No exists vcpkg toolchain file. ${TARGET_VCPKG_ROOT_PATH_}/${LAB_VCPKG_TOOL_CHAIN_FILE_}")
    endif()
endfunction()

function(VcpkgManager_handleBootstrap_ TARGET_VCPKG_ROOT_PATH_)
    if(EXISTS "${TARGET_VCPKG_ROOT_PATH_}/${LAB_VCPKG_BOOTSTRAP_FILE_}")
        VcpkgManager_setupVcpkg_(${TARGET_VCPKG_ROOT_PATH_})
        VcpkgManager_handleToolChain_(${TARGET_VCPKG_ROOT_PATH_})
    else()
        message(FATAL_ERROR
            "No exists vcpkg bootstrap file. ${TARGET_VCPKG_ROOT_PATH_}/${LAB_VCPKG_BOOTSTRAP_FILE_}")
    endif()
endfunction()

# simbol파일이 있고 실행파일에 대한 처리
function(VcpkgManager_handleExeFile_ TARGET_VCPKG_ROOT_PATH_)
    if(EXISTS "${TARGET_VCPKG_ROOT_PATH_}/${LAB_VCPKG_EXE_FILE_}")
        VcpkgManager_handleToolChain_(${TARGET_VCPKG_ROOT_PATH_})
    else()
        VcpkgManager_handleBootstrap_(${TARGET_VCPKG_ROOT_PATH_})
    endif()
endfunction()

# simbol파일이 없고 내부에 파일에 대한 처리
function(VcpkgManager_handleExistsFileInFolder_ TARGET_VCPKG_ROOT_PATH_)
    file(GLOB RES_FILES_ "${TARGET_VCPKG_ROOT_PATH_}/*.*")
    list(LENGTH RES_FILES_ RES_FILE_COUNT_)

    if(${RES_FILE_COUNT_})
        message(FATAL_ERROR 
            "Exists another file in LAB_VCPKG_ROOT_PATH=${TARGET_VCPKG_ROOT_PATH_}")
    else()
        VcpkgManager_downloadVcpkg_(${TARGET_VCPKG_ROOT_PATH_})
        VcpkgManager_setupVcpkg_(${TARGET_VCPKG_ROOT_PATH_})
    endif()
endfunction()

# 전달경로 폴더가 존재할 경우에 대한 처리
function(VcpkgManager_handleSimbolFile_ TARGET_VCPKG_ROOT_PATH_)
    if(EXISTS "${TARGET_VCPKG_ROOT_PATH_}/${LAB_VCPKG_ROOT_SIMBOL_FILE_}")
        VcpkgManager_handleExeFile_(${TARGET_VCPKG_ROOT_PATH_})
    else()
        VcpkgManager_handleExistsFileInFolder_(${TARGET_VCPKG_ROOT_PATH_})
    endif()
endfunction()

function(VcpkgManager_handleVcpkgRootFolder_ TARGET_VCPKG_ROOT_PATH_)
    if(EXISTS ${TARGET_VCPKG_ROOT_PATH_})
        VcpkgManager_handleSimbolFile_(${TARGET_VCPKG_ROOT_PATH_})
    else()
        # 클린 설치작업 개시
        file(MAKE_DIRECTORY ${TARGET_VCPKG_ROOT_PATH_})
        VcpkgManager_downloadVcpkg_(${TARGET_VCPKG_ROOT_PATH_})
        VcpkgManager_setupVcpkg_(${TARGET_VCPKG_ROOT_PATH_})
    endif()
endfunction()

#
# public function section
#
macro(VcpkgManager_printReadyState)
    message("Vcpkg manager ready ${LAB_VCPKG_MANAGER_READY_}")
    message(STATUS "LAB_VCPKG_ROOT_PATH=${LAB_VCPKG_ROOT_PATH}")
    message(STATUS "LAB_VCPKG_ROOT_SIMBOL_FILE=${LAB_VCPKG_ROOT_SIMBOL_FILE_}")
    message(STATUS "LAB_VCPKG_EXE_FILE=${LAB_VCPKG_EXE_FILE_}")
    message(STATUS "LAB_VCPKG_BOOTSTRAP_FILE=${LAB_VCPKG_BOOTSTRAP_FILE_}")
    message(STATUS "LAB_VCPKG_TOOL_CHAIN_FILE=${LAB_VCPKG_TOOL_CHAIN_FILE_}")
endmacro()

macro(VcpkgManager_printIntegrationState)
    message("Vcpkg manager integration ${LAB_VCPKG_MANAGER_INTEGRATION_}")
    message(STATUS "CMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}")
    message(STATUS "VCPKG_CHAINLOAD_TOOLCHAIN_FILE=${VCPKG_CHAINLOAD_TOOLCHAIN_FILE}")
endmacro()

# 전달받은 경로에 VcpkgManager를 사용가능 상태로 준비합니다.
# 전달인자가 0인경우 LAB_VCPKG_ROOT_PATH의 경로로 설정됩니다.
# vcpkg가 사용가능상태가된 것이지 통합상태가 아닙니다
function(VcpkgManager_ready)
    if(${ARGC})
        get_filename_component(LAB_VCPKG_ROOT_PATH ${ARGV0} ABSOLUTE)
    endif()

    message(STATUS "Ready to Vcpkg in ${LAB_VCPKG_ROOT_PATH}")

    VcpkgManager_handleVcpkgRootFolder_(${LAB_VCPKG_ROOT_PATH})
    set(LAB_VCPKG_MANAGER_READY_ TRUE PARENT_SCOPE)
endfunction()

function(VcpkgManager_integrateCleanly)
    VcpkgManager_ensureReadyState_()

endfunction()

function(VcpkgManager_requireLib)
    VcpkgManager_ensureReadyState_()
    VcpkgManager_ensureintegrationState_()

endfunction()


#
# work process section
#
VcpkgManager_printLoadedInfo_()
VcpkgManager_ensureGitCmd_()