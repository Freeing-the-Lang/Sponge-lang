cmake_minimum_required(VERSION 3.16)
project(Spongelang LANGUAGES CXX)

# ---------------------------------------
# C++20
# ---------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# =======================================
# META ENGINE 모듈 (별도 라이브러리)
# =======================================
file(GLOB META_SRC
    "src/meta/*.cpp"
)

add_library(meta_engine STATIC ${META_SRC})

target_include_directories(meta_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src/meta
)

# ---------------------------------------
# META 엔진 경고 옵션
# ---------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(meta_engine PRIVATE
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter -Wno-unused-variable
    )
endif()

if (MSVC)
    target_compile_options(meta_engine PRIVATE /W4 /permissive-)
endif()

# =======================================
# SPONGELANG 메인 실행 파일
# =======================================
file(GLOB_RECURSE CORE_SRC
    "src/*.cpp"
)

# meta/*.cpp 제외
list(FILTER CORE_SRC EXCLUDE REGEX "src/meta/.*")

add_executable(spongelang ${CORE_SRC})

target_include_directories(spongelang PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# ---------------------------------------
# CORE 엔진 경고 옵션
# ---------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(spongelang PRIVATE
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter -Wno-unused-variable
    )
endif()

if (MSVC)
    target_compile_options(spongelang PRIVATE /W4 /permissive-)
endif()

# ---------------------------------------
# macOS ARM64 특수 옵션
# ---------------------------------------
if (APPLE)
    target_compile_options(spongelang PRIVATE -fvisibility=default)
    target_link_options(spongelang PRIVATE -stdlib=libc++)
endif()

# =======================================
# 실행 파일이 META ENGINE 의존
# =======================================
target_link_libraries(spongelang PRIVATE meta_engine)

# =======================================
# Install Target (GitHub Actions용)
# =======================================
install(TARGETS spongelang
    RUNTIME DESTINATION bin
)
