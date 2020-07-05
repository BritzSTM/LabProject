# cmake에서 Git관련된 작업을 할 때 도움을 주기 위한 유틸리티 함수 모음
#
# CMake에는 함수를 선언만 할 수 없기 때문에 
# A private 함수 집합 -> A public API 함수 순으로 작성한다.
cmake_minimum_required(VERSION 3.7)

#
# GitUtils_clone private function
#
macro(GitUtils_clone_parseArgs_)
    set(OPTIONS CLEAN)
    set(ONE_ARGS URL PATH DEPTH HASH BRANCH)
    set(MULTI_ARGS)

    cmake_parse_arguments(CLONE "${OPTIONS}" "${ONE_ARGS}" "${MULTI_ARGS}" ${ARGN})

    # 후처리로 절대 경로로 변경
    if(DEFINED CLONE_PATH)
        get_filename_component(CLONE_PATH ${CLONE_PATH} ABSOLUTE)
    endif()
endmacro()

function(GitUtils_clone_ensures_)
    # Arguments
    if(NOT DEFINED CLONE_PATH)
        message(FATAL_ERROR "Not found PATH argument")
    endif()

    if(NOT DEFINED CLONE_URL)
        message(FATAL_ERROR "Not found URL argument")
    endif()

    # PATH
    if(NOT EXISTS ${CLONE_PATH})
        message(FATAL_ERROR "Not found PATH=${CLONE_PATH} directory.")
    endif()
endfunction()

macro(GitUtils_clone_setupVars_)
    # ! 이름에 반드시 CLONE_ prefix를 요구함
    set(CLONE_VER_FILE "${CLONE_PATH}/CloneVer.txt")
endmacro()

macro(GitUtils_clone_makeCMD_)
    set(CLONE_CMD git clone)
    
    if(DEFINED CLONE_DEPTH)
        if(${CLONE_DEPTH} MATCHES "^[0-9]+$")
            list(APPEND CLONE_CMD --depth ${CLONE_DEPTH})
        else()
            message(WARNING "GitUtils_clone DEPTH=${CLONE_DEPTH} is not number and is ignored.")
        endif()
    endif()

    # finish
    list(APPEND CLONE_CMD ${CLONE_URL} ${CLONE_PATH})
endmacro()

function(GitUtils_clone_printCMD_)
    string(REPLACE ";" " " FORMATED_CMD_FOR_PRINT_ "${CLONE_CMD}")
    message("GitUtils_clone command=${FORMATED_CMD_FOR_PRINT_}")
endfunction()

function(GitUtils_clone_writeHashToFile_)
    set(GIT_VER_FMT_CMD git;log;-1;--pretty=format:\"%H\";)
    
    message("${GIT_VER_FMT_CMD}")
    execute_process(
        COMMAND ${GIT_VER_FMT_CMD}
        WORKING_DIRECTORY ${CLONE_PATH}
        RESULT_VARIABLE VER_RESULT
        OUTPUT_VARIABLE VER_OUTPUT
        ERROR_VARIABLE CLONE_ERROR
        )

    if(${CLONE_RESULT})
       message("${CLONE_ERROR}")
    else()
       string(REPLACE "\"" "" CLONE_OUTPUT ${CLONE_OUTPUT})
       file(APPEND ${CLONE_VER_FILE} "Current: ${CLONE_OUTPUT}\n")
    endif()
endfunction()

macro(GitUtils_clone_preprocess_)
    GitUtils_clone_setupVars_()
    GitUtils_clone_writeHashToFile_()
    GitUtils_clone_makeCMD_()
    GitUtils_clone_printCMD_()
endmacro()

function(GitUtils_clone_executeClone_)
    execute_process(
      COMMAND ${CLONE_CMD}
      WORKING_DIRECTORY ${CLONE_PATH}
      RESULT_VARIABLE CLONE_RESULT
      OUTPUT_VARIABLE CLONE_OUTPUT
      ERROR_VARIABLE CLONE_ERROR
      )

    # succeed is 0
    if(${CLONE_RESULT})
        message(FATAL_ERROR "GitUtils_clone failed.\nGit[${CLONE_RESULT}]\n${CLONE_ERROR}")
    else()
        message("Git[${CLONE_RESULT}]: ${CLONE_OUTPUT}")
    endif()
endfunction()

function(GitUtils_clone_handleClean_)
    if(DEFINED CLONE_CLEAN)
        if(${CLONE_CLEAN})
            #
        endif()
    endif()
endfunction()

function(GitUtils_clone_handleHash_)
    if(DEFINED CLONE_HASH)
        if(${CLONE_HASH})
            #
        endif()
    endif()
endfunction()

function(GitUtils_clone_postprocess_)
    GitUtils_clone_writeHashToFile_()

    GitUtils_clone_handleHash_()
    GitUtils_clone_handleClean_()
endfunction()

#
# public function
#

# 필수 PATH, URL. 비필수 DEPTH, HASH, CLEAN(Option), BRANCH, CLEAN
function(GitUtils_clone)
    # Prefix CLONE_
    GitUtils_clone_parseArgs_(${ARGN})
    GitUtils_clone_ensures_()

    GitUtils_clone_preprocess_()

    GitUtils_clone_executeClone_()

    GitUtils_clone_postprocess_()
endfunction()