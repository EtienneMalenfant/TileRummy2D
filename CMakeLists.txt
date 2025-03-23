cmake_minimum_required(VERSION 3.10)

project(rummy_game VERSION 1.0)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
find_package(Boost CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BUILD_TYPE Debug)

include_directories(${PROJECT_SOURCE_DIR}/include)

file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_executable(rummy_game ${SOURCES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=address -g")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")

target_compile_options(rummy_game PRIVATE
    -fdiagnostics-color=always
)
# set(CMAKE_COLOR_MAKEFILE OFF)

target_link_libraries(rummy_game
    sfml-graphics
    sfml-window
    sfml-system
)
