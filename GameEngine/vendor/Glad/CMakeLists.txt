project(Glad LANGUAGES C)
add_library(Glad STATIC)

target_sources(Glad PRIVATE
    include/glad/glad.h
    include/KHR/khrplatform.h
    src/glad.c
)

target_include_directories(Glad PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/include")

if(MSVC)
    target_compile_options(Glad PRIVATE /W0)
    if(CMAKE_BUILD_TYPE STREQUAL "Debug-AS")
        target_compile_options(Glad PRIVATE /fsanitize=address /Zi)
        target_link_options(Glad PRIVATE /INCREMENTAL:NO)
    endif()
else()
    target_compile_options(Glad PRIVATE -w)
endif()

if(UNIX OR APPLE)
    set_target_properties(Glad PROPERTIES POSITION_INDEPENDENT_CODE ON)
endif()