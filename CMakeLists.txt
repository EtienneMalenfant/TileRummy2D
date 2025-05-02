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

find_package(OpenGL REQUIRED)
set(SFML_LIBS
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
)

if(WIN32)
    add_compile_definitions(SFML_STATIC)
    link_directories(${LIB_DIR}/sfml/lib-windows)
    list(APPEND SFML_LIBS
        opengl32
        winmm
        gdi32
        ws2_32
        user32
        advapi32
        ${LIB_DIR}/sfml/lib-windows/libfreetype.a
    )
    message("-- Compiling for Windows - enabling static linking of runtime libraries")
    # Force static linking of everything including winpthread
    set(CMAKE_FIND_LIBRARY_SUFFIXES ".a")
    set(BUILD_SHARED_LIBS OFF)

    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pthread -static -static-libgcc -static-libstdc++")
else()
    link_directories(${LIB_DIR}/sfml/lib)
    list(APPEND SFML_LIBS
        pthread
        X11
        Xrandr
        GL
        dl
        Xcursor
        udev
        freetype
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

# === Sources ===
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(TileRummy2D ${SOURCES})

target_include_directories(TileRummy2D PRIVATE ${PROJECT_SOURCE_DIR}/include)

# === Link ===
target_sources(TileRummy2D PRIVATE ${BOOST_JSON_SRC})
target_link_libraries(TileRummy2D PRIVATE ${SFML_LIBS})

