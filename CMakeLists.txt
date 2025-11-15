cmake_minimum_required(VERSION 3.16)
project(Spongelang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# src 폴더 내 모든 .cpp 자동 포함
file(GLOB_RECURSE SRC_FILES "src/*.cpp")

add_executable(spongelang
    ${SRC_FILES}
)

# GCC / Clang 공통 경고 옵션
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(spongelang PRIVATE -Wall -Wextra -Wpedantic)
endif()

# MSVC 경고 옵션
if (MSVC)
    target_compile_options(spongelang PRIVATE /W4)
endif()
