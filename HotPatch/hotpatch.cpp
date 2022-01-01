

#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

// Makes the memory pointed executable
void makeExecutable(void *code, size_t size)
{
    size_t pagesize = getpagesize();
    uint64_t codeaddr = uint64_t(code);
    uint64_t offset = codeaddr % pagesize;
    codeaddr -= offset;
    size += offset;
    size_t codesize = ((size - 1) / pagesize + 1) * pagesize;
    mprotect((void *)codeaddr, codesize, PROT_READ | PROT_WRITE | PROT_EXEC);
}

extern "C"
{
    extern int mul2(int, int);
    extern int mul2_end;
};

asm(R"(
.globl mul2, mul2_end
.type mul2, @function
mul2:
    mov %edi, %eax
    imul %esi, %eax
    nop
    ret
mul2_end:
)");

int main()
{

    // Execute code
    std::cout << "Size:" << intptr_t(&mul2_end) - intptr_t(&mul2) << std::endl;
    std::cout << "Result:" << mul2(2, 3) << std::endl;

    return 0;
}
