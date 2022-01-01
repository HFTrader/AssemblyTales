#pragma once
#include <vector>
#include <cstdint>

class AsmCopyGen
{
public:
    using Bytes = std::vector<uint8_t>;
    void copyField(uint32_t source, uint32_t target, uint32_t size);
    Bytes getBytes();

private:
    Bytes bytes;
    void insert(const std::initializer_list<uint8_t> &v)
    {
        bytes.insert(bytes.end(), v.begin(), v.end());
    }
};
