# Tiny Syscall Game

A minimal **Linux x86-64 terminal game** written **without libc**, using only **raw syscalls** and **inline assembly**. The game persists your balance in `/dev/shm/g` and uses `rdrand` for randomness.

---

## Features

- No libc, no standard startup (`_start` is defined manually)
- Raw syscalls for I/O, file access, polling, and exit
- Uses `rdrand` for random
- Game loop persists 'balance' in shared memory
- Ultra-tiny and minimal linker script

---

## Target CPU

**AMD Ryzen 5 3400G with Radeon Vega Graphics**

>  This program **requires ****rdrand** and **Linux x86-64**. It will not run on CPUs without `rdrand` or non-x86 architectures.

---

## Build

Use the included linker script `tiny.ld` and compile with:

```sh
gcc -Os -nostdlib -fno-ident -fno-stack-protector -fno-pie -no-pie \
    -fomit-frame-pointer -fno-asynchronous-unwind-tables -fno-unwind-tables \
    -Wl,-T,tiny.ld -Wl,--build-id=none -Wl,-nmagic \
    tiny.c -o tiny && sstrip tiny
```

---

## Notes

- Assumes System V AMD64 ABI
- Tested on AMD Ryzen 5 3400G

