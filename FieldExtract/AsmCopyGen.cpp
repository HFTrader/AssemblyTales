#include "AsmCopyGen.h"

AsmCopyGen::Bytes AsmCopyGen::getBytes()
{
    Bytes res = bytes;
    res.push_back(0xc3); // ret
    return res;
}

void AsmCopyGen::copyField(uint32_t source, uint32_t target, uint32_t size)
{
    switch (size)
    {
    case 1:
    {
        // movzbl source(%rdi), %eax
        if (source == 0)
        {
            insert({0x0f, 0xb6, 0x07});
        }
        else if (source < 256)
        {
            insert({0x0f, 0xb6, 0x47, uint8_t(source)});
        }
        else
        {
            insert({0x0f, 0xb6, 0x87, uint8_t(source), uint8_t(source >> 8), uint8_t(source >> 16), uint8_t(source >> 24)});
        }
        // mov %al,target(%rsi)
        if (target == 0)
        {
            insert({0x88, 0x06});
        }
        else if (target < 256)
        {
            insert({0x88, 0x46, uint8_t(target)});
        }
        else
        {
            insert({0x88, 0x86, uint8_t(target), uint8_t(target >> 8), uint8_t(target >> 16), uint8_t(target >> 24)});
        }
    };
    break;
    case 2:
    {
        // movzwl source(%rdi), %eax
        if (source == 0)
        {
            insert({0x0f, 0xb7, 0x07});
        }
        else if (source < 256)
        {
            insert({0x0f, 0xb7, 0x47, uint8_t(source)});
        }
        else
        {
            insert({0x0f, 0xb7, 0x87, uint8_t(source), uint8_t(source >> 8), uint8_t(source >> 16), uint8_t(source >> 24)});
        }
        // mov %ax,target(%rsi)
        if (target == 0)
        {
            insert({0x66, 0x89, 0x06});
        }
        else if (target < 256)
        {
            insert({0x66, 0x89, 0x46, uint8_t(target)});
        }
        else
        {
            insert({0x66, 0x89, 0x86, uint8_t(target), uint8_t(target >> 8), uint8_t(target >> 16), uint8_t(target >> 24)});
        }
    };
    break;
    case 4:
    {
        // mov source(%rdi), %eax
        if (source == 0)
        {
            insert({0x8b, 0x07});
        }
        else if (source < 256)
        {
            insert({0x8b, 0x47, uint8_t(source)});
        }
        else
        {
            insert({0x8b, 0x87, uint8_t(source), uint8_t(source >> 8), uint8_t(source >> 16), uint8_t(source >> 24)});
        }
        // mov %eax,target(%rsi)
        if (target == 0)
        {
            insert({0x89, 0x06});
        }
        else if (target < 256)
        {
            insert({0x89, 0x46, uint8_t(target)});
        }
        else
        {
            insert({0x89, 0x86, uint8_t(target), uint8_t(target >> 8), uint8_t(target >> 16), uint8_t(target >> 24)});
        }
    };
    break;
    case 8:
    {
        // mov source(%rdi), %rax
        if (source == 0)
        {
            insert({0x48, 0x8b, 0x07});
        }
        else if (source < 256)
        {
            insert({0x48, 0x8b, 0x47, uint8_t(source)});
        }
        else
        {
            insert({0x48, 0x8b, 0x87, uint8_t(source), uint8_t(source >> 8), uint8_t(source >> 16), uint8_t(source >> 24)});
        }
        // mov %rax,target(%rsi)
        if (target == 0)
        {
            insert({0x48, 0x89, 0x06});
        }
        else if (target < 256)
        {
            insert({0x48, 0x89, 0x46, uint8_t(target)});
        }
        else
        {
            insert({0x48, 0x89, 0x86, uint8_t(target), uint8_t(target >> 8), uint8_t(target >> 16), uint8_t(target >> 24)});
        }
    };
    break;
    }
}
