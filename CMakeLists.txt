cmake_minimum_required(VERSION 3.15)
project(Archiver)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    include_directories(/usr/include)
    link_libraries(zstd)
else()
    include(FetchContent)

    set(ZSTD_BUILD_STATIC ON CACHE BOOL "" FORCE)
    set(ZSTD_BUILD_SHARED OFF CACHE BOOL "" FORCE)

    FetchContent_Declare(
        zstd
        GIT_REPOSITORY https://github.com/facebook/zstd.git
        GIT_TAG v1.5.5
        SOURCE_SUBDIR build/cmake
    )
    FetchContent_MakeAvailable(zstd)
endif()

add_executable(archiver
    src/main.cpp
    src/archiver.cpp
)

target_include_directories(archiver PRIVATE src)

if(CMAKE_SYSTEM_NAME MATCHES "Linux" OR CMAKE_SYSTEM_NAME MATCHES "Darwin")
    target_link_libraries(archiver PRIVATE zstd)
else()
    target_include_directories(archiver PRIVATE ${zstd_SOURCE_DIR}/lib)
    target_link_directories(archiver PRIVATE "${CMAKE_BINARY_DIR}/_deps/zstd-build/lib/${CMAKE_BUILD_TYPE}")
    target_link_libraries(archiver PRIVATE zstd_static)
endif()