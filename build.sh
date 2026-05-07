#!/bin/sh

# Usage
#
# Debug Build
#     $ ./build.sh
# Release Build
#     $ ./build.sh --release
#
# Build and Run
#     $ ./build.sh && ./app.exe

EXE=app.exe

CC=gcc.exe
SRC="src/main.c"
CFLAGS="${CFLAGS} -Wall"
CFLAGS="${CFLAGS} -Wextra"
CFLAGS="${CFLAGS} -Wpedantic"
CFLAGS="${CFLAGS} -Wconversion"
CFLAGS="${CFLAGS} -Wdouble-promotion"
CFLAGS="${CFLAGS} -Wno-unused-parameter"
CFLAGS="${CFLAGS} -Wno-unused-function"
CFLAGS="${CFLAGS} -Wno-sign-conversion"
INCLUDE="${INCLUDE} -Ivendor/SDL3-3.4.8/include"
INCLUDE="${INCLUDE} -Ivendor/SDL3_ttf-3.2.2/include"
LDFLAGS="${LDFLAGS} -Lvendor/SDL3-3.4.8/lib"
LDFLAGS="${LDFLAGS} -Lvendor/SDL3_ttf-3.2.2/lib"
LIBS="-lSDL3 -lSDL3_ttf"

if [ "$1" = "--release" ]; then
    CFLAGS="${CFLAGS} -O2 -DNDEBUG -D_NDEBUG"
    LDFLAGS="${LDFLAGS} -Wl,--subsystem,windows"
else
    CFLAGS="${CFLAGS} -DDUBUG -D_DUBUG"
fi

$CC $SRC -o $EXE $CFLAGS $INCLUDE $LDFLAGS $LIBS
EXIT_STATUS=$?

if [ $EXIT_STATUS -ne 0 ]; then
    echo [31mFailed to compilation...[0m
    exit $EXIT_STATUS
fi

echo [32mCompilation succeeded...[0m
