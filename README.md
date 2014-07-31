
webpageworker is a QtWebKit based crawler.

## Dependencies

Qt5 and CMake, that is all.

## Build (Unix)

    mkdir build
    cd build
    cmake ..
    make

## Build (Windows)

Building for windows with CMake should be fairly similar to the Unix example
above (depending on your generator). If you are cross compiling for Mingw I've
left a toolchain in the cmake/ folder, just call cmake as

    cmake -DCMAKE_TOOLCHAIN_FILE=../cmake/mingw32-w64-cross.toolchain.cmake  ..

## Documentation

Read MANPAGE.md for details, you can generate a manpage
if you have md2man-roff with

    make man

