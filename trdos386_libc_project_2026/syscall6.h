#define _syscall1(eax) syscall(eax, 0, 0, 0, 0, 0)
#define _syscall2(eax, ebx) syscall(eax, ebx, 0, 0, 0, 0)