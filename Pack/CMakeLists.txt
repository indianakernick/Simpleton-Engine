cmake_minimum_required(VERSION 3.8)
project(pack)

set(CMAKE_BUILD_TYPE "Release")

add_executable(pack
        "search dir.cpp"
        "search dir.hpp"
        "sort by frame.cpp"
        "sort by frame.hpp"
        "load images.cpp"
        "load images.hpp"
        "stb_rect_pack.c"
        "stb_rect_pack.h"
        "rects from images.cpp"
        "rects from images.hpp"
        "pack rects.cpp"
        "pack rects.hpp"
        "blit images.cpp"
        "blit images.hpp"
        "write atlas.cpp"
        "write atlas.hpp"
        "write image.cpp"
        "write image.hpp"
        "app.cpp"
        "app.hpp"
        "main.cpp"
)

target_compile_features(pack
        PRIVATE
        cxx_std_17
)

install(TARGETS pack
        RUNTIME DESTINATION bin
)

target_include_directories(pack
        PRIVATE
        /usr/local/include
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/..
)

target_link_libraries(pack
        PRIVATE
        /usr/local/lib/libpng16.a
        /usr/local/opt/zlib/lib/libz.a
)

add_definitions(-DRELEASE)

