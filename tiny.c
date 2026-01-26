#define SYS_READ 0
#define SYS_WRITE 1
#define SYS_OPEN 2
#define SYS_POLL 7
#define SYS_EXIT 60

#define sc(n, a1, a2, a3) ({ \
    long r; \
    __asm__ volatile ("syscall" : "=a"(r) : "a"(n), "D"((long)a1), "S"((long)a2), "d"((long)a3) : "rcx", "r11", "memory"); \
    r; \
})

void __attribute__((naked)) _start() {
    __asm__ volatile (
        "sub $8, %rsp\n"
        "call main_loop\n"
        "mov $60, %rax\n"
        "xor %rdi, %rdi\n"
        "syscall\n"
    );
}

void main_loop() {
    int balance = 100;
    int fd = sc(SYS_OPEN, "/dev/shm/g", 66, 0666); 
    sc(SYS_READ, fd, &balance, 4);

    while (balance > 0) {
        unsigned long target, count = 1;
        __asm__ volatile ("rdrand %0" : "=r"(target));
        target = (target % 12) + 1;

        sc(SYS_WRITE, 1, "$ ", 2); 

        struct { int fd; short ev, rev; } p = {0, 1, 0};
        while (sc(SYS_POLL, &p, 1, 0) <= 0) {
            count = (count % 12) + 1;
        }

        char buf;
        sc(SYS_READ, 0, &buf, 1);

        if (count == target) {
            balance += 50;
            sc(SYS_WRITE, 1, "W\n", 2);
        } else {
            balance -= 10;
            sc(SYS_WRITE, 1, "L\n", 2);
        }

        sc(SYS_OPEN, "/dev/shm/g", 2, 0); 
        sc(SYS_WRITE, fd, &balance, 4);
    }
}
