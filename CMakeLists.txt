cmake_minimum_required(VERSION 4.0)
project(GameEngine)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
    add_compile_options(/MP /utf-8) # MultiProcessorCompile
endif()

add_definitions(-DGE_PROJECT_DIR="${CMAKE_CURRENT_SOURCE_DIR}")

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    set(CMAKE_BUILD_TYPE Debug)
endif()

set(OUTPUT_DIR_SUFFIX "${CMAKE_BUILD_TYPE}-${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR_SUFFIX}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/${OUTPUT_DIR_SUFFIX}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin-int/${OUTPUT_DIR_SUFFIX}")

if(UNIX AND NOT APPLE)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-4.0)
endif()

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)

set(USE_MSVC_RUNTIME_LIBRARY_DLL OFF CACHE BOOL "" FORCE)

add_subdirectory(GameEngine/vendor/GLFW)
add_subdirectory(GameEngine/vendor/Glad)
add_subdirectory(GameEngine/vendor/imgui)
add_subdirectory(GameEngine/vendor/yaml-cpp)

# ==========================
# Project: GameEngine
# ==========================
set(GE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/src")
set(GE_VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/GameEngine/vendor")

file(GLOB_RECURSE GE_SOURCES 
    "GameEngine/src/*.cpp"
    "GameEngine/src/*.h"
    "GameEngine/vendor/stb_image/*.cpp"
    "GameEngine/vendor/stb_image/*.h"
)

file(GLOB IMGUIZMO_SOURCES
    "GameEngine/vendor/ImGuizmo/*.cpp"
    "GameEngine/vendor/ImGuizmo/*.h"
)

list(APPEND GE_SOURCES ${IMGUIZMO_SOURCES})
list(FILTER GE_SOURCES EXCLUDE REGEX "src/Platform/Windows/")
list(FILTER GE_SOURCES EXCLUDE REGEX "src/Platform/Linux/")

# Platform specific
if(WIN32)
    file(GLOB_RECURSE PLATFORM_SOURCES "GameEngine/src/Platform/Windows/*.cpp" "GameEngine/src/Platform/Windows/*.h")
elseif(UNIX)
    file(GLOB_RECURSE PLATFORM_SOURCES "GameEngine/src/Platform/Linux/*.cpp" "GameEngine/src/Platform/Linux/*.h")
endif()
list(APPEND GE_SOURCES ${PLATFORM_SOURCES})

add_library(GameEngine STATIC ${GE_SOURCES})

# PCH
target_precompile_headers(GameEngine PRIVATE "${GE_SRC_DIR}/gepch.h")

# Turn off PCH for ImGuizmo
file(GLOB IMGUIZMO_SOURCES "GameEngine/vendor/ImGuizmo/*.cpp")
set_source_files_properties(${IMGUIZMO_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

target_include_directories(GameEngine PUBLIC
    "${GE_SRC_DIR}"
    "${GE_VENDOR_DIR}/spdlog/include"
    "${GE_VENDOR_DIR}/GLFW/include"
    "${GE_VENDOR_DIR}/Glad/include"
    "${GE_VENDOR_DIR}/imgui"
    "${GE_VENDOR_DIR}/glm"
    "${GE_VENDOR_DIR}/stb_image"
    "${GE_VENDOR_DIR}/entt/include"
    "${GE_VENDOR_DIR}/yaml-cpp/include"
    "${GE_VENDOR_DIR}/ImGuizmo"
)

target_compile_definitions(GameEngine PUBLIC YAML_CPP_STATIC_DEFINE)

# Link GameEngine
target_link_libraries(GameEngine PUBLIC glfw Glad ImGui yaml-cpp)

if(WIN32)
    target_link_libraries(GameEngine PUBLIC opengl32.lib)
    target_compile_definitions(GameEngine PUBLIC GLFW_INCLUDE_NONE)
elseif(UNIX)
    target_link_libraries(GameEngine PUBLIC 
        GL pthread dl X11 Xrandr Xi Xxf86vm Xinerama Xcursor
        PkgConfig::GTKMM
    )
endif()

# Build configuration definitions
target_compile_definitions(GameEngine PRIVATE
    $<$<CONFIG:Debug>:GE_BUILD_DEBUG>
    $<$<CONFIG:Release>:GE_BUILD_RELEASE>
    $<$<CONFIG:Dist>:GE_BUILD_DIST>
)

# ==========================
# Project: Sandbox
# ==========================
file(GLOB_RECURSE SANDBOX_SOURCES "Sandbox/src/*.cpp" "Sandbox/src/*.h")
add_executable(Sandbox ${SANDBOX_SOURCES})

target_include_directories(Sandbox PRIVATE
    "${GE_SRC_DIR}"
    "${GE_VENDOR_DIR}"
    "${GE_VENDOR_DIR}/spdlog/include"
    "${GE_VENDOR_DIR}/glm"
    "${GE_VENDOR_DIR}/entt/include"
)

target_link_libraries(Sandbox PRIVATE GameEngine)

# Post-build: Copy assets to output directory
add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/Sandbox/assets"
    "$<TARGET_FILE_DIR:Sandbox>/assets"
)

# ==========================
# Project: GameEngine-Editor
# ==========================
file(GLOB_RECURSE EDITOR_SOURCES "GameEngine-Editor/src/*.cpp" "GameEngine-Editor/src/*.h")
add_executable(GameEngine-Editor ${EDITOR_SOURCES})

target_include_directories(GameEngine-Editor PRIVATE
    "${GE_SRC_DIR}"
    "${GE_VENDOR_DIR}"
    "${GE_VENDOR_DIR}/spdlog/include"
    "${GE_VENDOR_DIR}/glm"
    "${GE_VENDOR_DIR}/entt/include"
    "${GE_VENDOR_DIR}/ImGuizmo"
)

target_link_libraries(GameEngine-Editor PRIVATE GameEngine)

if(UNIX)
    target_link_libraries(GameEngine-Editor PRIVATE PkgConfig::GTKMM)
endif()

# Post-build: Copy assets to output directory
add_custom_command(TARGET GameEngine-Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/GameEngine-Editor/assets"
    "$<TARGET_FILE_DIR:GameEngine-Editor>/assets"
)