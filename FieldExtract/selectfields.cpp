
#include "AsmCopyGen.h"
#include "AsmUtils.h"

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <array>
#include <iostream>

// A random message with a sequence of fields
struct Message
{
    uint8_t field_1;
    uint16_t field_2;
    uint32_t field_3;
    uint64_t field_4;
    int8_t field_5;
    int16_t field_6;
    int32_t field_7;
    int64_t field_8;
    int8_t field_9;
    int16_t field_10;
    int32_t field_11;
    int64_t field_12;
    int8_t field_13;
    int16_t field_14;
    int32_t field_15;
    int64_t field_16;
    int8_t field_17;
    int16_t field_18;
    int32_t field_19;
    int64_t field_20;
} __attribute__((packed));

struct MetaItem
{
    uint32_t offset;
    uint32_t size;
};

using Bytes = AsmCopyGen::Bytes;

std::vector<MetaItem> metadata = {
    {offsetof(Message, field_1), sizeof(Message::field_1)},
    {offsetof(Message, field_2), sizeof(Message::field_2)},
    {offsetof(Message, field_3), sizeof(Message::field_3)},
    {offsetof(Message, field_4), sizeof(Message::field_4)},
    {offsetof(Message, field_5), sizeof(Message::field_5)},
    {offsetof(Message, field_6), sizeof(Message::field_6)},
    {offsetof(Message, field_7), sizeof(Message::field_7)},
    {offsetof(Message, field_8), sizeof(Message::field_8)},
    {offsetof(Message, field_9), sizeof(Message::field_9)},
    {offsetof(Message, field_10), sizeof(Message::field_10)},
    {offsetof(Message, field_11), sizeof(Message::field_11)},
    {offsetof(Message, field_12), sizeof(Message::field_12)},
    {offsetof(Message, field_13), sizeof(Message::field_13)},
    {offsetof(Message, field_14), sizeof(Message::field_14)},
    {offsetof(Message, field_15), sizeof(Message::field_15)},
    {offsetof(Message, field_16), sizeof(Message::field_16)},
    {offsetof(Message, field_17), sizeof(Message::field_17)},
    {offsetof(Message, field_18), sizeof(Message::field_18)},
    {offsetof(Message, field_19), sizeof(Message::field_19)},
    {offsetof(Message, field_20), sizeof(Message::field_20)}};

// Use metadata and the user input field list to create the custom assembly
Bytes genCopyAssembly(const std::vector<uint32_t> &ilist)
{
    AsmCopyGen gen;
    uint32_t offset = 0;
    for (unsigned field : ilist)
    {
        MetaItem meta(metadata[field - 1]);
        gen.copyField(meta.offset, offset, meta.size);
        offset += meta.size;
    }
    return gen.getBytes();
}

// Make the copy in the traditional way using metadata and offsets
void copyVanilla(uint8_t *dst, uint8_t *src, const std::vector<uint32_t> &ilist)
{
    uint32_t offset = 0;
    for (unsigned field : ilist)
    {
        MetaItem meta(metadata[field - 1]);
        memcpy(&dst[offset], &src[meta.offset], meta.size);
        offset += meta.size;
    }
}

int main(int argc, char *argv[])
{
    // Sanity
    if (argc < 2)
    {
        std::cout << "Usage: selectfields <num> [ <num> ... ]" << std::endl;
        std::cout << "Pass a list of field numbers that should be copied into the output messages in any order. " << std::endl;
        std::cout << "Valid field numbers from 1 to 20" << std::endl;
        std::cout << "Example: 1 2 3 4 15 20" << std::endl;
        return 0;
    }

    // Read input vector from user
    std::vector<unsigned> fields;
    for (uint32_t j = 1; j < argc; ++j)
    {
        fields.push_back(::atoi(argv[j]));
    }

    // Create custom generated assembly for the given fields
    Bytes proc = genCopyAssembly(fields);

    // Make contents of vector executable
    makeExecutable(proc.data(), proc.size());

    // Assign pointer to function
    using CopyFieldsFn = void(Message *, uint8_t *);
    CopyFieldsFn *copyfn = reinterpret_cast<CopyFieldsFn *>(proc.data());

    // Create a typical message, fill fields
    Message msg;
    std::memset(&msg, 0, sizeof(msg));
    msg.field_1 = 1;
    msg.field_2 = 2;
    msg.field_3 = 3;
    msg.field_4 = 4;
    msg.field_10 = 10;
    msg.field_19 = 19;
    msg.field_20 = 20;

    // Create an array to hold the results
    // This example is current not going to check the size but this should be
    // done using the metadata provided
    // So this *might* crash if you are a smart ass and pass a large enough list
    // Fill with some notorious values (0xff)
    std::vector<uint8_t> bytes(4096, 0xff);

    // Loop over a number of times doing copies and gathering
    // micro-benchmarking statistics
    double sum1 = 0;
    double sum2 = 0;
    const unsigned numloops = 1000000;
    for (uint32_t j = 0; j < numloops; ++j)
    {
        // Test optimized
        uint64_t t0 = now();
        copyfn(&msg, bytes.data());
        uint64_t t1 = now();

        // Test "normal" way
        copyVanilla(bytes.data(), (uint8_t *)&msg, fields);
        uint64_t t2 = now();

        // Add to stats
        sum1 += (t1 - t0);
        sum2 += (t2 - t1);
    }

    // Display statistics
    std::cout << "Average times:" << std::endl;
    std::cout << "\tAssembly Gen:" << sum1 / numloops << std::endl;
    std::cout << "\tVanilla copy:" << sum2 / numloops << std::endl;
    std::cout << std::endl;

    // Printout end result for peace of mind
    std::cout << "Bytes:\n\t";
    for (uint8_t val : bytes)
    {
        char str[16];
        ::sprintf(str, "%02x ", val);
        std::cout << str;
    }
    std::cout << std::endl;
    return 0;
}