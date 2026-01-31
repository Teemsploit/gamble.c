#define SYS_EXIT  1
#define SYS_READ  3
#define SYS_WRITE 4
#define SYS_OPEN  5
#define SYS_LSEEK 19

static inline int sc(int n, int a1, int a2, int a3) {
    int r;
    __asm__ volatile ("int $0x80" : "=a"(r) : "a"(n), "b"(a1), "c"(a2), "d"(a3) : "memory");
    return r;
}

void _start() {
    int balance;
    int fd;
    char buf;

    fd = sc(SYS_OPEN, (int)"/dev/shm/g", 66, 0666);
    
    if (sc(SYS_READ, fd, (int)&balance, 4) <= 0) {
        balance = 100;
    }

    while (balance > 0) {
        unsigned int t1, t2;

        sc(SYS_WRITE, 1, (int)"$ ", 2);
        
        __asm__ volatile ("rdtsc" : "=a"(t1) : : "edx");
        sc(SYS_READ, 0, (int)&buf, 1);
        
        if (buf != '\n') {
            char junk;
            while (sc(SYS_READ, 0, (int)&junk, 1) > 0 && junk != '\n');
        }
        __asm__ volatile ("rdtsc" : "=a"(t2) : : "edx");

        if ((t2 - t1) % 11 > 7) {
            balance += 50;
            sc(SYS_WRITE, 1, (int)"W\n", 2);
        } else {
            balance -= 10;
            sc(SYS_WRITE, 1, (int)"L\n", 2);
        }

        sc(SYS_LSEEK, fd, 0, 0);
        sc(SYS_WRITE, fd, (int)&balance, 4);
    }
    sc(SYS_EXIT, 0, 0, 0);
}
