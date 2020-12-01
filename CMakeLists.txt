cmake_minimum_required(VERSION 3.8)
project(receiver)
set(TARGET libreceiver)
# --------------------------------------------------------------------------
if(MSVC)
    # Visual Studio Dynamic vs Static Runtimes
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
endif()

add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/googletest-master"
)
add_subdirectory(
    "${CMAKE_CURRENT_SOURCE_DIR}/test"
)
# --------------------------------------------------------------------------
set(Headers 
    "${CMAKE_SOURCE_DIR}/include/receiver.h"
    "${CMAKE_SOURCE_DIR}/include/callback.h"
)
set(Sources 
    "${CMAKE_SOURCE_DIR}/src/receiver.cpp"
    "${CMAKE_SOURCE_DIR}/src/callback.cpp"
)
add_library(
    ${TARGET} STATIC 
    ${Sources} 
    ${Headers}
)
target_include_directories(
    ${TARGET} PRIVATE 
    "${CMAKE_SOURCE_DIR}/include"
)

set_target_properties(
    ${This} PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED ON
)

# СВОЙСТВА ДЛЯ КОМПИЛЯТОРОВ (ЛЮБЫЕ ПРЕДУПРЕЖДЕНИЯ ТРАКТОВАТЬ КАК ОШИБКИ)
if (MSVC)
    target_compile_options(${TARGET} PRIVATE
        /W4
    )
else ()
    target_compile_options(${TARGET} PRIVATE
        -Wall -Wextra -pedantic -Werror
    )
endif()

# --------------------------------------------------------------------------
enable_testing()
# --------------------------------------------------------------------------
