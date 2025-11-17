cmake_minimum_required(VERSION 3.16)
project(Spongelang LANGUAGES CXX)

# ---------------------------------------
# C++20
# ---------------------------------------
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# ---------------------------------------
# META ENGINE 서브 모듈 추가
# ---------------------------------------
add_subdirectory(src/meta)

# ---------------------------------------
# CORE SOURCE 자동 스캔
# meta 폴더 제외
# ---------------------------------------
file(GLOB_RECURSE CORE_SRC
    "src/*.cpp"
)

# meta 아래는 제외
list(FILTER CORE_SRC EXCLUDE REGEX "src/meta/.*")

# ---------------------------------------
# 실행 파일 생성
# ---------------------------------------
add_executable(spongelang ${CORE_SRC})

# include
target_include_directories(spongelang PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

# meta 엔진 링크
target_link_libraries(spongelang PRIVATE meta_engine)

# ---------------------------------------
# 컴파일러 옵션 (GNU/Clang/MSVC/macOS)
# ---------------------------------------
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
    target_compile_options(spongelang PRIVATE
        -Wall -Wextra -Wpedantic
        -Wno-unused-parameter -Wno-unused-variable
    )
elseif (MSVC)
    target_compile_options(spongelang PRIVATE
        /W4 /permissive-
    )
endif()

if (APPLE)
    target_compile_options(spongelang PRIVATE -fvisibility=default)
    target_link_options(spongelang PRIVATE -stdlib=libc++)
endif()

# ---------------------------------------
# Install target (GitHub Actions 필수)
# ---------------------------------------
install(TARGETS spongelang
    RUNTIME DESTINATION bin
)

# 설치 후 디버그 메시지 (bin 생성 확인)
install(CODE "
    message(STATUS \"[INSTALL] Checking installed files in: \${CMAKE_INSTALL_PREFIX}/bin\")
    file(GLOB BINFILES \"\${CMAKE_INSTALL_PREFIX}/bin/*\")
    foreach(F \${BINFILES})
        message(STATUS \"[INSTALL] Installed: \${F}\")
    endforeach()
")

