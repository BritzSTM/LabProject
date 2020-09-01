# Precompile header
target_precompile_headers(${PROJECT_NAME} PRIVATE "${FD_SOURCE_DIR}/pch.h")

# GenerateFiles
set(CMAKE_CONFIG_GEN_SUFFIX "cmake_gen")

configure_file(
    "${FD_SOURCE_DIR}/Version.cpp.in"
    "${FD_SOURCE_DIR}/Version_${CMAKE_CONFIG_GEN_SUFFIX}.cpp")

target_sources(${PROJECT_NAME}
    PRIVATE
        BaseTypes.h
        Version.h
        Version_cmake_gen.cpp)

#add_subdirectory(BaseTypes)
add_subdirectory(Dummy)
