#pragma once

/*
    5. 헬로월드
*/
struct sbiret {
    long error;
    long value;
};

/*
    7. 커널 패닉
*/
#define PANIC(fmt, ...)                                                        \
    do {                                                                       \
        printf("PANIC: %s:%d: " fmt "\n", __FILE__, __LINE__, ##__VA_ARGS__);  \
        while (1) {}                                                           \
    } while (0)



/*
    8. 예외
*/
#include "common.h"

struct trap_frame {
    uint32_t ra;
    uint32_t gp;
    uint32_t tp;
    uint32_t t0;
    uint32_t t1;
    uint32_t t2;
    uint32_t t3;
    uint32_t t4;
    uint32_t t5;
    uint32_t t6;
    uint32_t a0;
    uint32_t a1;
    uint32_t a2;
    uint32_t a3;
    uint32_t a4;
    uint32_t a5;
    uint32_t a6;
    uint32_t a7;
    uint32_t s0;
    uint32_t s1;
    uint32_t s2;
    uint32_t s3;
    uint32_t s4;
    uint32_t s5;
    uint32_t s6;
    uint32_t s7;
    uint32_t s8;
    uint32_t s9;
    uint32_t s10;
    uint32_t s11;
    uint32_t sp;
} __attribute__((packed));

#define READ_CSR(reg)                                                          \
    ({                                                                         \
        unsigned long __tmp;                                                   \
        __asm__ __volatile__("csrr %0, " #reg : "=r"(__tmp));                  \
        __tmp;                                                                 \
    })

#define WRITE_CSR(reg, value)                                                  \
    do {                                                                       \
        uint32_t __tmp = (value);                                              \
        __asm__ __volatile__("csrw " #reg ", %0" ::"r"(__tmp));                \
    } while (0)


/*
    11. 페이지 테이블
*/
#define SATP_SV32 (1u << 31)
#define PAGE_V    (1 << 0)   // "Valid" 비트 (엔트리가 유효함을 의미)
#define PAGE_R    (1 << 1)   // 읽기 가능
#define PAGE_W    (1 << 2)   // 쓰기 가능
#define PAGE_X    (1 << 3)   // 실행 가능
#define PAGE_U    (1 << 4)   // 사용자 모드 접근 가능

/*
    13. 유저 모드
*/
#define USER_BASE 0x1000000  // user.ld의 베이스 주소와 일치해야 함
#define SSTATUS_SPIE (1 << 5)
