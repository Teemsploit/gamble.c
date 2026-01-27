#define SYS_READ  0
#define SYS_WRITE 1
#define SYS_OPEN  2
#define SYS_LSEEK 8

int balance = 100;
int fd;
char buf;

static inline long sc(long n, long a1, long a2, long a3) {
    long r;
    __asm__ volatile ("syscall" : "=a"(r) : "a"(n), "D"(a1), "S"(a2), "d"(a3) : "rcx", "r11", "memory");
    return r;
}

void run() {
    fd = sc(SYS_OPEN, (long)"/dev/shm/g", 66, 0666);
    sc(SYS_READ, fd, (long)&balance, 4);
    if (balance <= 0) balance = 100;

    while (balance > 0) {
        unsigned long start_tsc, end_tsc;

        sc(SYS_WRITE, 1, (long)"$ ", 2);

        __asm__ volatile ("rdtsc" : "=a"(start_tsc) : : "edx");

        sc(SYS_READ, 0, (long)&buf, 1);
        
        if (buf != '\n') {
            char junk;
            while (sc(SYS_READ, 0, (long)&junk, 1) > 0 && junk != '\n');
        }

        __asm__ volatile ("rdtsc" : "=a"(end_tsc) : : "edx");

        if ((end_tsc - start_tsc) % 10 > 7) { 
            balance += 50;
            sc(SYS_WRITE, 1, (long)"W\n", 2);
        } else {
            balance -= 10;
            sc(SYS_WRITE, 1, (long)"L\n", 2);
        }

        sc(SYS_LSEEK, fd, 0, 0);
        sc(SYS_WRITE, fd, (long)&balance, 4);
    }
    sc(60, 0, 0, 0);
}

void __attribute__((naked)) _start() {
    __asm__ volatile ("call run\n");
}
