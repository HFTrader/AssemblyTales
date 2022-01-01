

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

int main()
{
    // The "generated" assembly code
    uint8_t code[] = {0x89, 0xf8,       // mov eax, edi
                      0x0f, 0xaf, 0xc6, // imul eax, esi
                      0xc3};            // ret
    makeExecutable(code, sizeof(code));

    // Defines a pointer to a function that takes two integers
    // and returns one integer
    using multifn = int(int a, int b);
    multifn *m = reinterpret_cast<multifn *>(code);

    // Execute code
    std::cout << "Result:" << m(2, 3) << std::endl;

    return 0;
}
/*
$ gcc mkexec.cpp -o mkexec

$ ./mkexec
Result:6
*/