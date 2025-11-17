# =======================================
# META ENGINE STATIC LIBRARY
# =======================================

# meta/*.cpp 자동 스캔
file(GLOB META_SRC
    "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_library(meta_engine STATIC ${META_SRC})

# include 경로 (src/meta 폴더)
target_include_directories(meta_engine PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}
)

# ---------------------------------------
# 컴파일러 옵션
# ---------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(meta_engine PRIVATE
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter -Wno-unused-variable
    )
elseif(MSVC)
    target_compile_options(meta_engine PRIVATE
        /W4 /permissive-
    )
endif()

if (APPLE)
    target_compile_options(meta_engine PRIVATE -fvisibility=default)
    target_link_options(meta_engine PRIVATE -stdlib=libc++)
endif()

