# my-os-in-1000-lines

[OS in 1,000 Lines](https://operating-system-in-1000-lines.vercel.app/ko/) 튜토리얼을 따라가며 RISC-V 기반 OS를 처음부터 직접 구현하는 프로젝트입니다.

## 개발 환경

| 항목 | 도구 |
|------|------|
| 컴파일러 | Clang (LLVM) |
| 링커 | lld |
| 에뮬레이터 | QEMU (`qemu-system-riscv32`) |
| 타겟 아키텍처 | RISC-V 32bit |
| 펌웨어 | OpenSBI |
| OS | Ubuntu (WSL) |

## 설치

```bash
sudo apt update && sudo apt install -y clang llvm lld qemu-system-riscv32 curl
curl -LO https://github.com/qemu/qemu/raw/v8.0.4/pc-bios/opensbi-riscv32-generic-fw_dynamic.bin
```

## 빌드 및 실행

```bash
./run.sh
```

## 종료

- `Ctrl+A` → `X` : QEMU 즉시 종료
- `Ctrl+A` → `C` : QEMU 모니터 전환 후 `q` 입력


## 내 블로그
- [1000줄로 만드는 OS](https://ljweel.github.io/posts/my-os-in-1000-lines/)

## 참고 자료

- [OS in 1,000 Lines (한국어)](https://operating-system-in-1000-lines.vercel.app/ko/)
- [RISC-V SBI 스펙](https://github.com/riscv-non-isa/riscv-sbi-doc)
- [OpenSBI](https://github.com/riscv-software-src/opensbi)
- [GNU ld Linker Scripts](https://sourceware.org/binutils/docs/ld/Scripts.html)
- [QEMU RISC-V virt 머신 문서](https://www.qemu.org/docs/master/system/riscv/virt.html)