cmake_minimum_required(VERSION 3.16)
project(Spongelang LANGUAGES CXX)

# ---------------------------------------
# C++20
# ---------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------
# META ENGINE 서브 모듈
# ---------------------------------------
add_subdirectory(src/meta)

# ---------------------------------------
# Core source 자동 스캔 (meta 제외)
# ---------------------------------------
file(GLOB_RECURSE CORE_SRC
    "src/*.cpp"
)

# meta/*.cpp 제거
list(FILTER CORE_SRC EXCLUDE REGEX "src/meta/.*")

add_executable(spongelang ${CORE_SRC})

target_include_directories(spongelang PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------------------------------
# 컴파일러 옵션
# ---------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(spongelang PRIVATE
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter -Wno-unused-variable
    )
elseif(MSVC)
    target_compile_options(spongelang PRIVATE /W4 /permissive-)
endif()

if (APPLE)
    target_compile_options(spongelang PRIVATE -fvisibility=default)
    target_link_options(spongelang PRIVATE -stdlib=libc++)
endif()

# ---------------------------------------
# meta 엔진 링크
# ---------------------------------------
target_link_libraries(spongelang PRIVATE meta_engine)

# ---------------------------------------
# Install
# ---------------------------------------
install(TARGETS spongelang
    RUNTIME DESTINATION bin
)

