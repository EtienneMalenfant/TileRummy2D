cmake_minimum_required(VERSION 3.20)

project(TileRummy2D VERSION 0.9)

find_package(SFML 2.6 COMPONENTS graphics window system REQUIRED)
find_package(Boost CONFIG REQUIRED COMPONENTS json)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# set(CMAKE_BUILD_TYPE Release)
set(CMAKE_BUILD_TYPE Debug)


file(GLOB_RECURSE SOURCES
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
add_executable(TileRummy2D ${SOURCES})

target_compile_options(TileRummy2D PRIVATE
    -fdiagnostics-color=always
)

target_include_directories( TileRummy2D PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/lib
    ${Boost_INCLUDE_DIRS}
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
target_link_libraries(TileRummy2D PRIVATE
    sfml-system
    sfml-graphics
    sfml-window
    Boost::json
)
