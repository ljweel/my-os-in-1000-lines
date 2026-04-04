#!/bin/bash
set -xue

QEMU=qemu-system-riscv32
OBJCOPY=/usr/bin/llvm-objcopy

# clang 경로와 컴파일 옵션
CC=/usr/bin/clang  # Ubuntu 등 환경에 따라 경로 조정: CC=clang
CFLAGS="-std=c11 -O2 -g3 -Wall -Wextra --target=riscv32-unknown-elf -fuse-ld=lld -fno-stack-protector -ffreestanding -nostdlib"

# 애플리케이션 컴파일 & 링크
$CC $CFLAGS -Wl,-Tuser.ld -Wl,-Map=shell.map -o shell.elf shell.c user.c common.c
# ELF -> 순수 바이너리
$OBJCOPY --set-section-flags .bss=alloc,contents -O binary shell.elf shell.bin
# 바이너리 -> 링크 가능한 오브젝트
$OBJCOPY -Ibinary -Oelf32-littleriscv shell.bin shell.bin.o


# 커널 빌드
$CC $CFLAGS -Wl,-Tkernel.ld -Wl,-Map=kernel.map -o kernel.elf \
    kernel.c common.c shell.bin.o

# QEMU 실행
$QEMU -machine virt -bios default -nographic -serial mon:stdio --no-reboot \
    -drive id=drive0,file=lorem.txt,format=raw,if=none \
    -device virtio-blk-device,drive=drive0,bus=virtio-mmio-bus.0 \
    -kernel kernel.elf