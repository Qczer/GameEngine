add_library(ImGui STATIC)

target_sources(ImGui PRIVATE
    imconfig.h
    imgui.h
    imgui.cpp
    imgui_draw.cpp
    imgui_demo.cpp
    imgui_internal.h
    imgui_tables.cpp
    imgui_widgets.cpp
    imstb_rectpack.h
    imstb_textedit.h
    imstb_truetype.h
    backends/imgui_impl_opengl3.cpp
    backends/imgui_impl_opengl3.h
    backends/imgui_impl_opengl3_loader.h
    backends/imgui_impl_glfw.h
    backends/imgui_impl_glfw.cpp
)

target_include_directories(ImGui SYSTEM PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/backends"
)

target_link_libraries(ImGui PUBLIC glfw Glad)

if(MSVC)
    set_property(TARGET ImGui PROPERTY MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
endif()

if(UNIX AND NOT APPLE)
    set_target_properties(ImGui PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()