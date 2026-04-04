cmake_minimum_required(VERSION 3.20)

set(CMAKE_CONFIGURATION_TYPES "Debug;Release;Dist" CACHE STRING "" FORCE)

project(GameEngine-Workspace)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GE_ENABLE_ALL_WARNINGS "Enable all compiler warnings" ON)
if(NOT MSVC)
    set(GE_WARNING_FLAGS
        -Wall
        -Wextra
        -Wconversion
        -Wshadow
        -Wpedantic
    )
else()
    set(GE_WARNING_FLAGS /W4)
endif()

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)
    add_compile_options(/MP /utf-8) # MultiProcessorCompile
endif()

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
endif()

add_definitions(-DGE_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

if(WIN32)
    set(GE_PLATFORM "Windows")
elseif(APPLE)
    set(GE_PLATFORM "MacOS")
else()
    set(GE_PLATFORM "Linux")
endif()

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(GE_ARCH "x86_64")
else()
    set(GE_ARCH "x86")
endif()

set(GE_OUTPUT_NAME "$<CONFIG>-${GE_PLATFORM}-${GE_ARCH}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${GE_OUTPUT_NAME}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${GE_OUTPUT_NAME}")
set(CMAKE_PDB_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${GE_OUTPUT_DIR_NAME}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int/${GE_OUTPUT_NAME}")

function(apply_sanitizers target_name)
    if(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${target_name} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${target_name} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

add_subdirectory(GameEngine)
add_subdirectory(Sandbox)
add_subdirectory(GameEngine-Editor)