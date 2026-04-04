project(GameEngine-Editor)

file(GLOB_RECURSE EDITOR_SOURCES "src/*.cpp" "src/*.h")
add_executable(GameEngine-Editor ${EDITOR_SOURCES})

if(GE_ENABLE_ALL_WARNINGS)
    target_compile_options(GameEngine-Editor PRIVATE ${GE_WARNING_FLAGS})
endif()

target_link_libraries(GameEngine-Editor PRIVATE GameEngine)

if(UNIX)
    target_link_libraries(GameEngine-Editor PRIVATE PkgConfig::GTKMM)
endif()

target_compile_definitions(GameEngine-Editor PRIVATE
    $<$<CONFIG:Debug>:GE_BUILD_DEBUG>
    $<$<CONFIG:Release>:GE_BUILD_RELEASE>
    $<$<CONFIG:Dist>:GE_BUILD_DIST>
)

add_custom_command(TARGET GameEngine-Editor POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:GameEngine-Editor>/assets"
)

apply_sanitizers(GameEngine-Editor)