#!/bin/sh
set -e

gcc -m32 -Os -nostdlib -fno-ident -fno-stack-protector -fno-pie -no-pie \
    -fomit-frame-pointer -fno-asynchronous-unwind-tables -fno-unwind-tables \
    -march=i386 \
    -Wl,-T,tiny.ld -Wl,--build-id=none -Wl,-nmagic \
    tiny.c -o tiny

sstrip tiny