cmake_minimum_required(VERSION 3.20)
project(TileRummy2D VERSION 0.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_BUILD_TYPE Release) # or Debug

set(LIB_DIR "${CMAKE_SOURCE_DIR}/libs")

#link_directories(/usr/lib/x86_64-linux-gnu)

# === Boost JSON ===
if(EXISTS "${LIB_DIR}/boost")
    message(STATUS "Using embedded Boost")
    include_directories(${LIB_DIR}/boost)
    add_compile_definitions(BOOST_ALL_NO_LIB)
    set(BOOST_JSON_SRC
        ${LIB_DIR}/boost/libs/json/src/src.cpp
    )
else()
    message(STATUS "Using system Boost")
    find_package(Boost REQUIRED COMPONENTS json)
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
endif()

# === SFML ===
if(EXISTS "${LIB_DIR}/sfml")
    message(STATUS "Using embedded SFML")
    include_directories(${LIB_DIR}/sfml/include)
    link_directories(${LIB_DIR}/sfml/lib)
    find_package(OpenGL REQUIRED)
    set(SFML_LIBS
        sfml-graphics-s
        sfml-window-s
        sfml-system-s
        pthread
        X11
        Xrandr
        freetype
        jpeg
        GL
        dl
        Xcursor
        udev 
    )
else()
    message(STATUS "Using system SFML")
    find_package(SFML 2.5 COMPONENTS graphics window system REQUIRED)
    set(SFML_LIBS sfml-graphics sfml-window sfml-system)
endif()

# === Compiler flags ===
if(CMAKE_BUILD_TYPE STREQUAL "Release")
    add_compile_definitions(RELEASE)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -DNDEBUG")
else()
    add_compile_definitions(DEBUG)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fsanitize=address")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fsanitize=address")
endif()

# === Sources ===
file(GLOB_RECURSE SOURCES ${PROJECT_SOURCE_DIR}/src/*.cpp)

add_executable(TileRummy2D ${SOURCES})

target_include_directories(TileRummy2D PRIVATE ${PROJECT_SOURCE_DIR}/include)

# === Link ===
if(BOOST_JSON_SRC)
    target_sources(TileRummy2D PRIVATE ${BOOST_JSON_SRC})
endif()

target_link_libraries(TileRummy2D PRIVATE ${SFML_LIBS})

