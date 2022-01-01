#include <cstddef>
#include <cstdint>
#include <unistd.h>
#include <sys/mman.h>

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
