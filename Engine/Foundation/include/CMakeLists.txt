# Precompile header
target_precompile_headers(${PROJECT_NAME} PRIVATE "${FD_SOURCE_DIR}/pch.h")

# GenerateFiles
set(_FD_GEN_FILE_LIST)
set(_FD_CMAKE_CONFIG_GEN_SUFFIX "cmake_gen")

configure_file(
    "${FD_SOURCE_DIR}/version.cpp.in"
    "${FD_SOURCE_DIR}/version_${_FD_CMAKE_CONFIG_GEN_SUFFIX}.cpp")

list(APPEND _FD_GEN_FILE_LIST "${FD_SOURCE_DIR}/version_${_FD_CMAKE_CONFIG_GEN_SUFFIX}.cpp")


target_sources(${PROJECT_NAME}
    PRIVATE
        compiler_directive.h
        base_types.h
        version.h
        hash.h
        lang_util.h
        enum_util.h
        type_traits_ext.h
        span.h
        ${_FD_GEN_FILE_LIST}
        )


add_subdirectory(base_type)
add_subdirectory(dummy)
add_subdirectory(platform)


# Reflection Module

add_subdirectory(reflection)
