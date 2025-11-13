cmake_minimum_required(VERSION 3.16)
project(Spongelang LANGUAGES CXX)

# C++20 사용
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 소스 파일 추가
add_executable(spongelang
    src/spongelang.cpp
)

# 경고 옵션 (필요하면 꺼도 됨)
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang" OR
    CMAKE_CXX_COMPILER_ID MATCHES "GNU")
    target_compile_options(spongelang PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Windows MSVC 옵션
if (MSVC)
    target_compile_options(spongelang PRIVATE /W4)
endif()
