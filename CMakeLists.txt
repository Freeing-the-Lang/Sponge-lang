cmake_minimum_required(VERSION 3.16)
project(Spongelang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 🔥 install prefix 강제 설정 (가장 위)
set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}" CACHE PATH "" FORCE)

# ---- META MODULE ----
add_subdirectory(src/meta)

# ---- CORE SOURCES ----
file(GLOB_RECURSE CORE_SRC
    "src/*.cpp"
)

list(FILTER CORE_SRC EXCLUDE REGEX "src/meta/.*")

add_executable(spongelang ${CORE_SRC})

target_include_directories(spongelang PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/src
)

target_link_libraries(spongelang PRIVATE meta_engine)

# ------------------------------------------------
# 🔥🔥🔥 install() 은 항상 target 정의 끝난 뒤 맨 아래!!
# ------------------------------------------------
install(TARGETS spongelang
    RUNTIME DESTINATION bin
)
