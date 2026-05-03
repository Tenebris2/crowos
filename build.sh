#!/bin/sh
set -e
. ./headers.sh

for PROJECT in $PROJECTS; do
  (cd $PROJECT && DESTDIR="$SYSROOT" $MAKE install)
done

cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=cmake/i686-elf-toolchain.cmake
ln -sf build/compile_commands.json compile_commands.json
