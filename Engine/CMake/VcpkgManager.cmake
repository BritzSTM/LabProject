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
        set(LAB_VCPKG_ROOT_PATH NOTFOUND)
    endif()
endif()

if(LAB_VCPKG_ROOT_PATH)
    get_filename_component(LAB_VCPKG_ROOT_PATH ${LAB_VCPKG_ROOT_PATH} ABSOLUTE)
endif()

if(NOT DEFINED LAB_VCPKG_GIT_URL)
    set(LAB_VCPKG_GIT_URL https://github.com/Microsoft/vcpkg.git)
endif()

# LAB_VCPKG_GIT_HASH은 옵션 값

#
# prviate variables
#
if(NOT DEFINED LAB_VCPKG_MANAGER_INIT_)    
    set(LAB_VCPKG_MANAGER_INIT_)

    # ! LAB_VCPKG_ROOT_PATH를 기준으로 작성
    set(LAB_VCPKG_ROOT_SIMBOL_FILE_ .vcpkg-root)

    if(WIN32)
        set(LAB_VCPKG_EXE_FILE_ vcpkg.exe)
        set(LAB_VCPKG_BOOTSTRAP_FILE_ bootstrap-vcpkg.bat)
    else()
        set(LAB_VCPKG_EXE_FILE_ vcpkg)
        set(LAB_VCPKG_BOOTSTRAP_FILE_ bootstrap-vcpkg.sh)
    endif()

    set(LAB_VCPKG_TOOL_CHAIN_FILE_ scripts/buildsystems/vcpkg.cmake)

    # 제어변수
    set(LAB_VCPKG_MANAGER_READY_ FALSE)
    set(LAB_VCPKG_MANAGER_INTEGRATION_ FALSE)
endif()

#
# private function
#
macro(VcpkgManager_printLoadedInfo_)
    message("Vcpkg manager ${VCPKG_MANAGER_VERSION}")
    message("Vcpkg manager loaded in ${CMAKE_PARENT_LIST_FILE}")

    if(LAB_VCPKG_ROOT_PATH)
        message(STATUS "Found LAB_VCPKG_ROOT_PATH=${LAB_VCPKG_ROOT_PATH}")
    else()
        message(STATUS "NotFound LAB_VCPKG_ROOT_PATH or ENV{VCPKG_ROOT}")
    endif()

    if(LAB_VCPKG_GIT_URL)
        message(STATUS "LAB_VCPKG_GIT_URL=${LAB_VCPKG_GIT_URL}")
    endif()

    if(LAB_VCPKG_GIT_HASH)
        message(STATUS "LAB_VCPKG_GIT_HASH=${LAB_VCPKG_GIT_HASH}")
    endif()
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

macro(VcpkgManager_downloadVcpkg_ensure_)
    # 다운로드전 반드시 목적지 폴더와 URL이 필수
    if(NOT EXISTS ${DOWNLOAD_PATH_ARG})
        message(FATAL_ERROR "Not found DOWNLOAD_PATH_ARG directory")
    endif()
endmacro()

macro(VcpkgManager_downloadVcpkg_parseArgs_)
    set(OPTIONS_)
    set(ONE_VALUE_ARGS_ PATH GIT_URL GIT_HASH)
    set(MULTI_VALUE_ARGS_)

    cmake_parse_arguments(
        DOWNLOAD
        "${OPTIONS_}" 
        "${ONE_VALUE_ARGS_}"
        "${MULTI_VALUE_ARGS_}"
        ${ARGN})
endmacro()

macro(VcpkgManager_downloadVcpkg_combineArgsToCMD)
    set(DOWNLOAD_CMD git)

    if(LAB_VCPKG_GIT_HASH)
        # ! 특정 시점이 필요하다면 모든 이력을 포함해야 한다
        list(APPEND DOWNLOAD_CMD clone ;${LAB_VCPKG_GIT_URL};./;)
    else()
        list(APPEND DOWNLOAD_CMD clone;--depth;1;${LAB_VCPKG_GIT_URL};./;)
    endif()
endmacro()

macro(VcpkgManager_downloadVcpkg_printInfo_)
    if(LAB_VCPKG_GIT_HASH)
        message(STATUS "Download Vcpkg from Git ${LAB_VCPKG_GIT_URL}[${LAB_VCPKG_GIT_HASH}]")
    else()
        message(STATUS "Download Vcpkg from Git ${LAB_VCPKG_GIT_URL}")
    endif()
endmacro()

macro(VcpkgManager_downloadVcpkg_checkout)

endmacro()

macro(VcpkgManager_downloadVcpkg_expects_)
    # succeed is 0
    if(${GIT_RESULT})
        message(FATAL_ERROR "Failed vcpkg startup process. Git[${GIT_RESULT}]: ${GIT_OUTPUT}")
    else()
        message("Git[${GIT_RESULT}]: ${GIT_OUTPUT}")
    endif()
endmacro()

# 전달받은 경로에 vcpkg를 다운받습니다. 실패시 FATAL_ERROR
function(VcpkgManager_downloadVcpkg_ DOWNLOAD_PATH_ARG)
    VcpkgManager_downloadVcpkg_ensure_()

    # 현재 함수에서 public과 전달인자가 섞임... 문제야 ... 
    # 섞이면 안되는 이유.... 순환참조문제
    # 여기서 전달받은 git을 재기록중...
    # 완전히 영향을 없애기 위해 전부 분리할 것

    # Prefix DOWNLOAD_. GIT_URL, GIT_TAG arguments
    VcpkgManager_downloadVcpkg_parseArgs_(${ARGN})
    VcpkgManager_downloadVcpkg_combineArgsToCMD()
    
    VcpkgManager_downloadVcpkg_printInfo_()
    execute_process(
      COMMAND ${DOWNLOAD_CMD}
      WORKING_DIRECTORY ${DOWNLOAD_PATH_ARG}
      RESULT_VARIABLE GIT_RESULT
      OUTPUT_VARIABLE GIT_OUTPUT
    )
    VcpkgManager_downloadVcpkg_checkout()

    VcpkgManager_downloadVcpkg_expects_()
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

# ! 반드시 전달 값을 모조리 받을 것이 조건. 반드시 function 내부안에서 호출되어야 함.
macro(VcpkgManager_integratePure_)
    #mark_as_advanced(CMAKE_TOOLCHAIN_FILE)
    # 첫 인자만 파일입력 값으로 평가함
    if(${ARGC})
        get_filename_component(CONV_ABS_PATH_ ${ARGV0} ABSOLUTE)
        
        if(EXISTS ${CONV_ABS_PATH_})
            set(CMAKE_TOOLCHAIN_FILE ${CONV_ABS_PATH_} PARENT_SCOPE)
            unset(CONV_ABS_PATH_)
        else()
            message(FATAL_ERROR "No exists vcpkg tool chain file. ${CONV_ABS_PATH_}")
        endif()
    else()
        set(CMAKE_TOOLCHAIN_FILE "${LAB_VCPKG_ROOT_PATH}/${LAB_VCPKG_TOOL_CHAIN_FILE_}" PARENT_SCOPE)
        #set(CMAKE_TOOLCHAIN_FILE
        #   "${LAB_VCPKG_ROOT_PATH}/${LAB_VCPKG_TOOL_CHAIN_FILE_}" CACHE STRING "")
    endif()

    #mark_as_advanced(CMAKE_TOOLCHAIN_FILE)
endmacro()

macro(VcpkgManager_Test)
    include(${LAB_VCPKG_ROOT_PATH}/${LAB_VCPKG_TOOL_CHAIN_FILE_})
endmacro()

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

macro(VcpkgManager_ready_parseArgs_)
    set(OPTIONS_)
    set(ONE_VALUE_ARGS_ ROOT_PATH GIT_URL HASH_TAG)
    set(MULTI_VALUE_ARGS_)

    cmake_parse_arguments(
        READY
        "${OPTIONS_}" 
        "${ONE_VALUE_ARGS_}"
        "${MULTI_VALUE_ARGS_}"
        ${ARGN})
endmacro()

macro(VcpkgManager_ready_handleArgs_)
    if(READY_ROOT_PATH)
        get_filename_component(LAB_VCPKG_ROOT_PATH ${READY_ROOT_PATH} ABSOLUTE)
        message("Change LAB_VCPKG_ROOT_PATH=${LAB_VCPKG_ROOT_PATH} in ready func")
    endif()

    if(READY_GIT_URL)
        message("${READY_GIT_URL}")
    endif()

    if(READY_HASH_TAG)
        message("${READY_HASH_TAG}")
    endif()
endmacro()

# 전달받은 경로에 VcpkgManager를 사용가능 상태로 준비합니다.
# 전달인자가 0인경우 LAB_VCPKG_ROOT_PATH의 경로로 설정됩니다.
# vcpkg가 사용가능상태가된 것이지 통합상태가 아닙니다
# ROOT_PATH, URL, HASH
function(VcpkgManager_ready)
    VcpkgManager_ready_parseArgs_(${ARGN})
    VcpkgManager_ready_handleArgs_()
    
    message(STATUS "Ready to Vcpkg in ${LAB_VCPKG_ROOT_PATH}")
    #VcpkgManager_handleVcpkgRootFolder_(${LAB_VCPKG_ROOT_PATH})

    set(LAB_VCPKG_MANAGER_READY_ TRUE PARENT_SCOPE)
endfunction()

# 전달인자가 없으면 LAB_VCPKG_TOOL_CHAIN_FILE로 통합을 진행합니다
# 외부에서 주입되는 VCPKG_CHAINLOAD_TOOLCHAIN_FILE 또한 상속 됩니다
function(VcpkgManager_integrateInheritance)
    VcpkgManager_ensureReadyState_()
    
    VcpkgManager_integratePure_(${ARGV})

    set(LAB_VCPKG_MANAGER_INTEGRATION_ TRUE PARENT_SCOPE)
endfunction()

# 전달인자가 없으면 LAB_VCPKG_TOOL_CHAIN_FILE로 통합을 진행합니다
# 외부에서 주입되는 VCPKG_CHAINLOAD_TOOLCHAIN_FILE 또한 초기화 됩니다
function(VcpkgManager_integrateCleanly)
    VcpkgManager_ensureReadyState_()

    VcpkgManager_integratePure_(${ARGV})
    set(VCPKG_CHAINLOAD_TOOLCHAIN_FILE PARENT_SCOPE)

    set(LAB_VCPKG_MANAGER_INTEGRATION_ TRUE PARENT_SCOPE)
endfunction()

function(VcpkgManager_requireLib)
    VcpkgManager_ensureReadyState_()
    VcpkgManager_ensureintegrationState_()
    
    foreach(TARGET_LIB_ARG_ ${ARGV})
        message(${TARGET_LIB_ARG_})
    endforeach()

    # tripet이 매우 중요함 불일치시에는 찾기 불가임;
    # ! 이미 설치되어 있으면 0이 뜬다. 성공해도 0이 뜬다
    # set(TARGET_CMD_ vcpkg install tinyxml2)
    #execute_process(
    #    COMMAND ${TARGET_CMD_}
    #    WORKING_DIRECTORY ${LAB_VCPKG_ROOT_PATH}
    #    RESULT_VARIABLE _RESULT
    #    OUTPUT_VARIABLE _OUTPUT
	#)


    #message("[${_RESULT}]:${_OUTPUT}")
endfunction()


#
# work process section
#
VcpkgManager_printLoadedInfo_()
VcpkgManager_ensureGitCmd_()