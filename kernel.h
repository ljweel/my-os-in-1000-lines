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

/*
    14. 시스템 콜
*/
#define SCAUSE_ECALL 8
#define PROC_EXITED   2
/*
    15. 디스크 I/O
*/
#define SECTOR_SIZE       512
#define VIRTQ_ENTRY_NUM   16
#define VIRTIO_DEVICE_BLK 2
#define VIRTIO_BLK_PADDR  0x10001000
#define VIRTIO_REG_MAGIC         0x00
#define VIRTIO_REG_VERSION       0x04
#define VIRTIO_REG_DEVICE_ID     0x08
#define VIRTIO_REG_PAGE_SIZE     0x28
#define VIRTIO_REG_QUEUE_SEL     0x30
#define VIRTIO_REG_QUEUE_NUM_MAX 0x34
#define VIRTIO_REG_QUEUE_NUM     0x38
#define VIRTIO_REG_QUEUE_PFN     0x40
#define VIRTIO_REG_QUEUE_READY   0x44
#define VIRTIO_REG_QUEUE_NOTIFY  0x50
#define VIRTIO_REG_DEVICE_STATUS 0x70
#define VIRTIO_REG_DEVICE_CONFIG 0x100
#define VIRTIO_STATUS_ACK       1
#define VIRTIO_STATUS_DRIVER    2
#define VIRTIO_STATUS_DRIVER_OK 4
#define VIRTQ_DESC_F_NEXT          1
#define VIRTQ_DESC_F_WRITE         2
#define VIRTQ_AVAIL_F_NO_INTERRUPT 1
#define VIRTIO_BLK_T_IN  0
#define VIRTIO_BLK_T_OUT 1

struct virtq_desc {
    uint64_t addr;    // 데이터가 있는 물리 주소
    uint32_t len;     // 데이터 크기
    uint16_t flags;   // NEXT, WRITE 등
    uint16_t next;    // 체인의 다음 디스크립터 인덱스
} __attribute__((packed));

struct virtq_avail {
    uint16_t flags;                    // 인터럽트 제어 플래그
    uint16_t index;                    // 다음에 쓸 위치
    uint16_t ring[VIRTQ_ENTRY_NUM];    // 디스크립터 헤드 인덱스 배열
} __attribute__((packed));

struct virtq_used_elem {
    uint32_t id;     // 처리한 디스크립터 체인의 헤드 인덱스
    uint32_t len;    // 장치가 실제로 쓴 바이트 수
} __attribute__((packed));

struct virtq_used {
    uint16_t flags;
    uint16_t index;                              // 장치가 다음에 쓸 위치
    struct virtq_used_elem ring[VIRTQ_ENTRY_NUM];
} __attribute__((packed));

struct virtio_virtq {
    struct virtq_desc descs[VIRTQ_ENTRY_NUM];   // 디스크립터 테이블
    struct virtq_avail avail;                    // Available Ring
    struct virtq_used used __attribute__((aligned(PAGE_SIZE)));  // Used Ring (페이지 정렬 필요)
    int queue_index;                             // 이 큐의 번호
    volatile uint16_t *used_index;               // used.index를 가리키는 포인터
    uint16_t last_used_index;                    // 드라이버가 마지막으로 확인한 used index
} __attribute__((packed));

struct virtio_blk_req {
    uint32_t type;       // 요청 타입 (IN=읽기, OUT=쓰기)
    uint32_t reserved;   // 사양에서 예약된 필드 (0)
    uint64_t sector;     // 접근할 섹터 번호
    uint8_t data[512];   // 읽기/쓰기할 데이터 (1섹터 = 512바이트)
    uint8_t status;      // 장치가 기록하는 처리 결과 (0=성공)
} __attribute__((packed));

/*
    16. 파일 시스템
*/
#define FILES_MAX      2
#define DISK_MAX_SIZE  align_up(sizeof(struct file) * FILES_MAX, SECTOR_SIZE)
#define SSTATUS_SUM  (1 << 18)

struct tar_header {
    char name[100];
    char mode[8];
    char uid[8];
    char gid[8];
    char size[12];
    char mtime[12];
    char checksum[8];
    char type;
    char linkname[100];
    char magic[6];
    char version[2];
    char uname[32];
    char gname[32];
    char devmajor[8];
    char devminor[8];
    char prefix[155];
    char padding[12];
    char data[];      // 헤더 뒤의 데이터를 가리키는 유연한 배열 멤버
} __attribute__((packed));

struct file {
    bool in_use;
    char name[100];
    char data[1024];
    size_t size;
};