#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_OPEN  2
#define SYS_POLL  7
#define SYS_EXIT  60

static inline long sc(long n, long a1, long a2, long a3) {
    long r;
    __asm__ volatile ("syscall" : "=a"(r) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");
    return r;
}

void __attribute__((noreturn)) _start() {
    long balance = 100;
    long fd = sc(SYS_OPEN, (long)"/dev/shm/g", 66, 0666);
    sc(SYS_READ, fd, (long)&balance, 4);

    while (balance > 0) {
        unsigned long target, count = 1;
        
        __asm__ volatile ("rdrand %0" : "=r"(target));
        target = (target % 12) + 1;

        sc(SYS_WRITE, 1, (long)"$ ", 2);

        struct { int fd; short ev, rev; } p = {0, 1, 0};
        while (sc(SYS_POLL, (long)&p, 1, 0) <= 0) {
            count = (count % 12) + 1;
        }

        char buf;
        sc(SYS_READ, 0, (long)&buf, 1);

        if (count == target) {
            balance += 50;
            sc(SYS_WRITE, 1, (long)"W\n", 2);
        } else {
            balance -= 10;
            sc(SYS_WRITE, 1, (long)"L\n", 2);
        }

        sc(8, fd, 0, 0); 
        sc(SYS_WRITE, fd, (long)&balance, 4);
    }

    sc(SYS_EXIT, 0, 0, 0);
    __builtin_unreachable();
}
