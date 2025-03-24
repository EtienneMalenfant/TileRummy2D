cmake_minimum_required(VERSION 3.10)

project(tile_rummy_2d VERSION 1.0)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
find_package(Boost CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_executable(tile_rummy_2d ${SOURCES})

target_compile_options(tile_rummy_2d PRIVATE
    -fdiagnostics-color=always
)

# Release or Debug configs
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
    add_compile_definitions(DEBUG)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")  # Enable optimizations and disable assertions
    add_compile_definitions(RELEASE)
endif()

# Dependencies
target_link_libraries(tile_rummy_2d
    sfml-graphics
    sfml-window
    sfml-system
)
