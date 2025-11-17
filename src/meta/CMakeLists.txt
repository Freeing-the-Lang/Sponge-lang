# -------------------------------------------
# META ENGINE MODULE (static library)
# -------------------------------------------

# 소스 자동 스캔
file(GLOB META_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

# meta 엔진 라이브러리 생성
add_library(meta_engine STATIC ${META_SRC})

# include 경로
target_include_directories(meta_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# -------------------------------------------
# Compiler warnings (GNU / Clang)
# -------------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(meta_engine PRIVATE
        -Wall
        -Wextra
        -Wpedantic
        -Wno-unused-parameter
        -Wno-unused-variable
    )
endif()

# -------------------------------------------
# MSVC 전용 옵션
# -------------------------------------------
if (MSVC)
    target_compile_options(meta_engine PRIVATE
        /W4
        /permissive-
    )
endif()

# -------------------------------------------
# macOS ARM64 대응
# -------------------------------------------
if (APPLE)
    target_compile_options(meta_engine PRIVATE -fvisibility=default)
    target_link_options(meta_engine PRIVATE -stdlib=libc++)
endif()

# -------------------------------------------
# 필요 시 다른 모듈 연결 (ex: core)
# target_link_libraries(meta_engine PRIVATE ...)
# -------------------------------------------

