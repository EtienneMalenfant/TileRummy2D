cmake_minimum_required(VERSION 3.20)
project(TileRummy2D VERSION 0.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#set(CMAKE_BUILD_TYPE Release)

set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs")

#link_directories(/usr/lib/x86_64-linux-gnu)

# === Boost JSON ===
message(STATUS "Using embedded Boost")
include_directories(${LIB_DIR}/boost)
set(BOOST_JSON_SRC
    ${LIB_DIR}/boost/libs/json/src/src.cpp
)

# === SFML ===
message(STATUS "Using embedded SFML")
include_directories(${LIB_DIR}/sfml/include)
link_directories(${LIB_DIR}/sfml/lib)
find_package(OpenGL REQUIRED)
set(SFML_LIBS
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    freetype
)

if(WIN32)
    add_compile_definitions(SFML_STATIC)
    list(APPEND SFML_LIBS
        opengl32
        winmm
        gdi32
    )
else()
    list(APPEND SFML_LIBS
        pthread
        X11
        Xrandr
        GL
        dl
        Xcursor
        udev
    )
endif()


# === Compiler flags ===
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE)
    message("-- Release build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
else()
    add_compile_definitions(DEBUG)
    message("-- Debug build")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

# === Static linking of runtime on Windows ===
if(MINGW)
    message("-- Compiling for Windows - enabling static linking of libgcc/libstdc++")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++")
endif()

# === Sources ===
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(TileRummy2D ${SOURCES})

target_include_directories(TileRummy2D PRIVATE ${PROJECT_SOURCE_DIR}/include)

# === Link ===
target_sources(TileRummy2D PRIVATE ${BOOST_JSON_SRC})
target_link_libraries(TileRummy2D PRIVATE ${SFML_LIBS})

