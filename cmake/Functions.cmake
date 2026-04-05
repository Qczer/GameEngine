function(apply_sanitizers TARGET_NAME)
    if(NOT MSVC AND CMAKE_BUILD_TYPE STREQUAL "Debug")
        target_compile_options(${TARGET_NAME} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
        target_link_options(${TARGET_NAME} PRIVATE -fsanitize=address,undefined)
    endif()
endfunction()

function(silence_target_warnings TARGET_NAME)
    if(TARGET ${TARGET_NAME})
        if(MSVC OR CMAKE_CXX_SIMULATE_ID STREQUAL "MSVC") # clang-cl / msvc
            target_compile_options(${TARGET_NAME} PRIVATE /W0)
        else() # Clang / GCC
            target_compile_options(${TARGET_NAME} PRIVATE -w)
        endif()
    endif()
endfunction()