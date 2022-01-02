

#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

// Makes the memory pointed executable
void makeWritable(void *code, size_t size)
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
};

asm(R"(
.globl mul2
.type mul2, @function
mul2:
.zero 8
)");

int main()
{
    // Assembly
    uint8_t code[] =
        {
            0x89, 0xf8,       // mov eax, edi
            0x0f, 0xaf, 0xc6, // imul eax, esi
            0xc3              // ret
        };

    // Patch function
    makeWritable((void *)mul2, sizeof(code));
    memcpy((void *)mul2, code, sizeof(code));

    // Execute
    std::cout << "Result of 2x3=" << mul2(2, 3) << std::endl;
    return 0;
}
