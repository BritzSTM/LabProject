cmake_minimum_required(VERSION 3.16)


project(VcpkgPipe VERSION 0.0.5)

#set(CMAKE_BUILD_TYPE Release)
#set(wxWidgets_LIBRARIES "C:/DockerShare/LabProject/Engine/Installed/Vcpkg/installed/x64-windows")
#set(wxWidgets_INCLUDE_DIRS "C:/DockerShare/LabProject/Engine/Installed/Vcpkg/installed/x64-windows/include")
#set(wxWidgets_LIB_DIRS "C:/DockerShare/LabProject/Engine/Installed/Vcpkg/installed/x64-windows/lib")
#set(wxWidgets_CONFIGURATION "mswd")


 
#   target_compile_definitions(vpp PRIVATE WXUSINGDLL=1)

  add_definitions(-D_UNICODE -DUNICODE -DWXUSINGDLL -DwxUSE_GUI=1 -D__WXMSW__)
  if("${CMAKE_BUILD_TYPE}" MATCHES "Debug")
    add_definitions(-D_DEBUG)
    include_directories(${_VCPKG_INSTALLED_DIR}/x64-windows/debug/lib/mswud)
    include_directories(${_VCPKG_INSTALLED_DIR}/x64-windows/include)
    set(wxWidgets_LIB_DIR ${_VCPKG_INSTALLED_DIR}/x64-windows/debug/lib)
    set(wxWidgets_LIBRARIES
      ${wxWidgets_LIB_DIR}/wxbase31ud_net.lib
      ${wxWidgets_LIB_DIR}/wxmsw31ud_core.lib
      ${wxWidgets_LIB_DIR}/wxbase31ud.lib
      winmm comctl32 oleacc rpcrt4 shlwapi version wsock32
    )
  elseif("${CMAKE_BUILD_TYPE}" MATCHES "Release")
    include_directories(${_VCPKG_INSTALLED_DIR}/x64-windows/lib/mswu)
    include_directories(${_VCPKG_INSTALLED_DIR}/x64-windows/include)
    set(wxWidgets_LIB_DIR ${_VCPKG_INSTALLED_DIR}/x64-windows/lib)
    set(wxWidgets_LIBRARIES
      ${wxWidgets_LIB_DIR}/wxbase31u_net.lib
      ${wxWidgets_LIB_DIR}/wxmsw31u_core.lib
      ${wxWidgets_LIB_DIR}/wxbase31u.lib
      winmm comctl32 oleacc rpcrt4 shlwapi version wsock32
    )
  endif()

    add_executable(vpp WIN32 src/main.cpp)

#find_package(wxWidgets CONFIG REQUIRED COMPONENTS core base)
#target_link_libraries(vpp PRIVATE ${wxWidgets_LIBRARIES})
#target_link_libraries(vpp PRIVATE wxWidgets::wxwidgets)
#target_link_libraries(vpp PRIVATE wxwidgets)

#find_package(TBB CONFIG REQUIRED)
#target_link_libraries(vpp PRIVATE TBB::tbb)
