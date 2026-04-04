project(GameEngine)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_TOOLS OFF CACHE BOOL "" FORCE)
set(YAML_CPP_BUILD_CONTRIB OFF CACHE BOOL "" FORCE)

add_subdirectory(vendor/GLFW)
add_subdirectory(vendor/Glad)
add_subdirectory(vendor/imgui)
add_subdirectory(vendor/yaml-cpp)

set(GE_SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(GE_VENDOR_DIR "${CMAKE_CURRENT_SOURCE_DIR}/vendor")

file(GLOB_RECURSE GE_SOURCES "${GE_SRC_DIR}/*.cpp" "${GE_SRC_DIR}/*.h")
file(GLOB IMGUIZMO_SOURCES "${GE_VENDOR_DIR}/ImGuizmo/*.cpp" "${GE_VENDOR_DIR}/ImGuizmo/*.h")
file(GLOB STB_SOURCES "${GE_VENDOR_DIR}/stb_image/*.cpp" "${GE_VENDOR_DIR}/stb_image/*.h")

list(FILTER GE_SOURCES EXCLUDE REGEX "Platform/Windows/")
list(FILTER GE_SOURCES EXCLUDE REGEX "Platform/Linux/")

if(WIN32)
    file(GLOB_RECURSE PLATFORM_SOURCES "${GE_SRC_DIR}/Platform/Windows/*.cpp" "${GE_SRC_DIR}/Platform/Windows/*.h")
elseif(UNIX)
    file(GLOB_RECURSE PLATFORM_SOURCES "${GE_SRC_DIR}/Platform/Linux/*.cpp" "${GE_SRC_DIR}/Platform/Linux/*.h")
endif()
list(APPEND GE_SOURCES ${PLATFORM_SOURCES})

add_library(ImGuizmo STATIC ${IMGUIZMO_SOURCES})
target_include_directories(ImGuizmo PUBLIC "${GE_VENDOR_DIR}/ImGuizmo" "${GE_VENDOR_DIR}/imgui")
set_source_files_properties(${IMGUIZMO_SOURCES} PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

add_library(stb_image STATIC ${STB_SOURCES})
target_include_directories(stb_image PUBLIC "${GE_VENDOR_DIR}/stb_image")

# TARGET
add_library(GameEngine STATIC ${GE_SOURCES})

if(GE_ENABLE_ALL_WARNINGS)
    target_compile_options(GameEngine PRIVATE ${GE_WARNING_FLAGS})
endif()
message("Enable all warnings: " ${GE_ENABLE_ALL_WARNINGS})

target_precompile_headers(GameEngine PRIVATE "${GE_SRC_DIR}/gepch.h")

# SYSTEM to disable warnings
target_include_directories(GameEngine SYSTEM PUBLIC
    "${GE_VENDOR_DIR}"
    "${GE_VENDOR_DIR}/entt/include"
    "${GE_VENDOR_DIR}/Glad/include"
    "${GE_VENDOR_DIR}/GLFW/include"
    "${GE_VENDOR_DIR}/glm"
    "${GE_VENDOR_DIR}/imgui"
    "${GE_VENDOR_DIR}/ImGuizmo"
    "${GE_VENDOR_DIR}/spdlog/include"
    "${GE_VENDOR_DIR}/stb_image"
    "${GE_VENDOR_DIR}/yaml-cpp/include"
)
target_include_directories(GameEngine PUBLIC
    "${GE_SRC_DIR}"
)

target_link_libraries(GameEngine PUBLIC 
    ImGuizmo 
    stb_image 
    glfw 
    Glad 
    ImGui 
    yaml-cpp
)

target_compile_definitions(GameEngine PUBLIC YAML_CPP_STATIC_DEFINE)
target_link_libraries(GameEngine PUBLIC glfw Glad ImGui yaml-cpp)

if(WIN32)
    target_link_libraries(GameEngine PUBLIC opengl32.lib)
    target_compile_definitions(GameEngine PUBLIC GLFW_INCLUDE_NONE)
elseif(UNIX)
    target_link_libraries(GameEngine PUBLIC GL pthread dl X11 Xrandr Xi Xxf86vm Xinerama Xcursor PkgConfig::GTKMM)
endif()

target_compile_definitions(GameEngine PRIVATE
    $<$<CONFIG:Debug>:GE_BUILD_DEBUG>
    $<$<CONFIG:Release>:GE_BUILD_RELEASE>
    $<$<CONFIG:Dist>:GE_BUILD_DIST>
)

apply_sanitizers(GameEngine)