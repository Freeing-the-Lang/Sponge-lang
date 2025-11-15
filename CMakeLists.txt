cmake_minimum_required(VERSION 3.16)
project(Spongelang LANGUAGES CXX)

# ---------------------------------------
# C++20
# ---------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------
# src 폴더 전체 자동 스캔 (*.cpp)
# ---------------------------------------
file(GLOB_RECURSE SRC_FILES
    "src/*.cpp"
)

add_executable(spongelang ${SRC_FILES})

# ---------------------------------------
# include path 자동 등록
# src 및 src/meta 자동 포함됨
# ---------------------------------------
target_include_directories(spongelang PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/meta
)

# ---------------------------------------
# GCC / Clang 경고 옵션
# ---------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(spongelang PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -Wno-unused-variable
    )
endif()

# ---------------------------------------
# MSVC 경고 옵션
# ---------------------------------------
if (MSVC)
    target_compile_options(spongelang PRIVATE /W4 /permissive-)
endif()

# ---------------------------------------
# macOS ARM64 특수 옵션
# 링크 심볼 누락 방지
# ---------------------------------------
if (APPLE)
    target_compile_options(spongelang PRIVATE -fvisibility=default)
    target_link_options(spongelang PRIVATE -stdlib=libc++)
endif()

# ---------------------------------------
# 빌드 타입 (없으면 Debug됨)
# ---------------------------------------
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()
