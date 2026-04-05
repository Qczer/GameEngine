cmake_minimum_required(VERSION 3.30)

project(GameEngine-Workspace)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(cmake/Functions.cmake)

option(GE_ENABLE_ALL_WARNINGS "Enable all compiler warnings" ON)
option(myproject_WARNINGS_AS_ERRORS "Set warnings as errors" OFF)

message("Enable ALL WARNINGS: " ${GE_ENABLE_ALL_WARNINGS})
message("Enable WARNINGS as ERRORS: " ${GE_ENABLE_ALL_WARNINGS})

add_library(myproject_warnings INTERFACE)

if(GE_ENABLE_ALL_WARNINGS)
    include(cmake/CompilerWarnings.cmake)
    myproject_set_project_warnings(
        myproject_warnings
        ${myproject_WARNINGS_AS_ERRORS}
        ""
        ""
        ""
        "")
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
    add_compile_options(/utf-8)
endif()

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
endif()

add_definitions(-DGE_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int")

add_subdirectory(GameEngine)
add_subdirectory(Sandbox)
add_subdirectory(GameEngine-Editor)