project(Sandbox)

file(GLOB_RECURSE SANDBOX_SOURCES "src/*.cpp" "src/*.h")
add_executable(Sandbox ${SANDBOX_SOURCES})

target_link_libraries(Sandbox PRIVATE GameEngine myproject_warnings)

target_compile_definitions(Sandbox PRIVATE
    $<$<CONFIG:Debug>:GE_BUILD_DEBUG>
    $<$<CONFIG:Release>:GE_BUILD_RELEASE>
    $<$<CONFIG:Dist>:GE_BUILD_DIST>
)

add_custom_command(TARGET Sandbox POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    "${CMAKE_CURRENT_SOURCE_DIR}/assets"
    "$<TARGET_FILE_DIR:Sandbox>/assets"
)

apply_sanitizers(Sandbox)