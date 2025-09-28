
#include <array>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <stdexcept>

#include "AsmCopyGen.h"
#include "AsmUtils.h"

// A random message with a sequence of fields
struct Message {
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
    uint8_t field_21;
    uint16_t field_22;
    uint32_t field_23;
    uint64_t field_24;
    int8_t field_25;
    int16_t field_26;
    int32_t field_27;
    int64_t field_28;
    int8_t field_29;
    int16_t field_30;
    int32_t field_31;
    int64_t field_32;
    int8_t field_33;
    int16_t field_34;
    int32_t field_35;
    int64_t field_36;
    int8_t field_37;
    int16_t field_38;
    int32_t field_39;
    int64_t field_40;
} __attribute__((packed));

struct MetaItem {
    uint32_t offset;
    uint32_t size;
};

using Bytes = AsmCopyGen::Bytes;

std::vector<MetaItem> metadata = {
    {offsetof(Message, field_1), sizeof(Message::field_1)},   {offsetof(Message, field_2), sizeof(Message::field_2)},
    {offsetof(Message, field_3), sizeof(Message::field_3)},   {offsetof(Message, field_4), sizeof(Message::field_4)},
    {offsetof(Message, field_5), sizeof(Message::field_5)},   {offsetof(Message, field_6), sizeof(Message::field_6)},
    {offsetof(Message, field_7), sizeof(Message::field_7)},   {offsetof(Message, field_8), sizeof(Message::field_8)},
    {offsetof(Message, field_9), sizeof(Message::field_9)},   {offsetof(Message, field_10), sizeof(Message::field_10)},
    {offsetof(Message, field_11), sizeof(Message::field_11)}, {offsetof(Message, field_12), sizeof(Message::field_12)},
    {offsetof(Message, field_13), sizeof(Message::field_13)}, {offsetof(Message, field_14), sizeof(Message::field_14)},
    {offsetof(Message, field_15), sizeof(Message::field_15)}, {offsetof(Message, field_16), sizeof(Message::field_16)},
    {offsetof(Message, field_17), sizeof(Message::field_17)}, {offsetof(Message, field_18), sizeof(Message::field_18)},
    {offsetof(Message, field_19), sizeof(Message::field_19)}, {offsetof(Message, field_20), sizeof(Message::field_20)},
    {offsetof(Message, field_21), sizeof(Message::field_21)}, {offsetof(Message, field_22), sizeof(Message::field_22)},
    {offsetof(Message, field_23), sizeof(Message::field_23)}, {offsetof(Message, field_24), sizeof(Message::field_24)},
    {offsetof(Message, field_25), sizeof(Message::field_25)}, {offsetof(Message, field_26), sizeof(Message::field_26)},
    {offsetof(Message, field_27), sizeof(Message::field_27)}, {offsetof(Message, field_28), sizeof(Message::field_28)},
    {offsetof(Message, field_29), sizeof(Message::field_29)}, {offsetof(Message, field_30), sizeof(Message::field_30)},
    {offsetof(Message, field_31), sizeof(Message::field_31)}, {offsetof(Message, field_32), sizeof(Message::field_32)},
    {offsetof(Message, field_33), sizeof(Message::field_33)}, {offsetof(Message, field_34), sizeof(Message::field_34)},
    {offsetof(Message, field_35), sizeof(Message::field_35)}, {offsetof(Message, field_36), sizeof(Message::field_36)},
    {offsetof(Message, field_37), sizeof(Message::field_37)}, {offsetof(Message, field_38), sizeof(Message::field_38)},
    {offsetof(Message, field_39), sizeof(Message::field_39)}, {offsetof(Message, field_40), sizeof(Message::field_40)}};

// Use metadata and the user input field list to create the custom assembly
Bytes genCopyAssembly(const std::vector<uint32_t> &ilist) {
    AsmCopyGen gen;
    uint32_t offset = 0;
    for (unsigned field : ilist) {
        if (field == 0 || field > metadata.size()) {
            throw std::out_of_range("Field number out of range: " + std::to_string(field));
        }
        MetaItem meta(metadata[field - 1]);
        gen.copyField(meta.offset, offset, meta.size);
        offset += meta.size;
    }
    Bytes res = gen.getBytes();
    res.push_back(0xc3);  // ret
    return res;
}

Bytes genHotpatch(const std::vector<uint32_t> &ilist, uint32_t patchsize) {
    AsmCopyGen gen;
    uint32_t offset = 0;
    for (unsigned field : ilist) {
        if (field == 0 || field > metadata.size()) {
            throw std::out_of_range("Field number out of range: " + std::to_string(field));
        }
        MetaItem meta(metadata[field - 1]);
        gen.copyField(meta.offset, offset, meta.size);
        offset += meta.size;
    }
    Bytes res = gen.getBytes();
    uint32_t left = patchsize - res.size();
    if (left < 5) {
        for (uint32_t j = 0; j < left; ++j) {
            res.push_back(0x90);  // nop
        }
    } else {
        // Short jump
        left -= 5;
        Bytes v{0xe9, uint8_t(left), uint8_t(left >> 8), uint8_t(left >> 16), uint8_t(left >> 24)};
        res.insert(res.end(), v.begin(), v.end());
    }
    return res;
}

void genTrampoline(const std::vector<uint32_t> &ilist, void *callback, Bytes &res) {
    AsmCopyGen gen;
    uint32_t offset = 0;
    for (unsigned field : ilist) {
        if (field == 0 || field > metadata.size()) {
            throw std::out_of_range("Field number out of range: " + std::to_string(field));
        }
        MetaItem meta(metadata[field - 1]);
        gen.copyField(meta.offset, offset, meta.size);
        offset += meta.size;
    }
    res = gen.getBytes();

    // Add the jump back
    Bytes v({0xe9, 0, 0, 0, 0});
    res.insert(res.end(), v.begin(), v.end());
    uint8_t *endptr = res.data() + res.size();
    uint64_t off = uint64_t(callback) - uint64_t(endptr);
    uint8_t *jump = endptr - 4;
    jump[0] = uint8_t(off);
    jump[1] = uint8_t(off >> 8);
    jump[2] = uint8_t(off >> 16);
    jump[3] = uint8_t(off >> 24);
}

// Make the copy in the traditional way using metadata and offsets
void copyVanilla(uint8_t *dst, uint8_t *src, const std::vector<uint32_t> &ilist) {
    uint32_t offset = 0;
    for (unsigned field : ilist) {
        if (field == 0 || field > metadata.size()) {
            throw std::out_of_range("Field number out of range: " + std::to_string(field));
        }
        MetaItem meta(metadata[field - 1]);
        memcpy(&dst[offset], &src[meta.offset], meta.size);
        offset += meta.size;
    }
}

extern "C" {
extern uint8_t hotpatch[];
extern uint8_t hotpatch_end[];
extern uint8_t trampoline[];
};

void compare(const std::string &msg, const std::vector<uint8_t> &golden, const std::vector<uint8_t> &bytes) {
    if (golden.size() != bytes.size()) {
        std::cout << msg << " Error: vector size is " << bytes.size() << " expected " << golden.size() << std::endl;
        return;
    }
    for (uint32_t j = 0; j < golden.size(); ++j) {
        if (golden[j] != bytes[j]) {
            std::cout << msg << " Error on byte " << j << " got " << uint32_t(bytes[j]) << " expected "
                      << uint32_t(golden[j]) << std::endl;
            std::cout << "Golden:\n";
            for (uint32_t k = 0; k < golden.size(); ++k) {
                char str[16];
                sprintf(str, "%02x ", golden[k]);
                std::cout << str;
            }
            std::cout << std::endl;
            std::cout << "Bytes:\n";
            for (uint32_t k = 0; k < golden.size(); ++k) {
                char str[16];
                sprintf(str, "%02x ", bytes[k]);
                std::cout << str;
            }
            std::cout << std::endl;
            std::exit(0);
            return;
        }
    }
}

int main(int argc, char *argv[]) {
    // Sanity
    if (argc < 2) {
        std::cout << "Usage: selectfields <num> [ <num> ... ]" << std::endl;
        std::cout << "Pass a list of field numbers that should be copied into the "
                     "output messages in any order. "
                  << std::endl;
        std::cout << "Valid field numbers from 1 to " << metadata.size() << std::endl;
        std::cout << "Example: 1 2 3 4 15 20" << std::endl;
        return 0;
    }

    // Read input vector from user
    std::vector<unsigned> fields;
    for (uint32_t j = 1; j < argc; ++j) {
        fields.push_back(::atoi(argv[j]));
    }

    // Hotpatch first
    uint32_t hotpatch_size = intptr_t(hotpatch_end) - intptr_t(hotpatch);
    std::cout << "Hotpatch size:" << hotpatch_size << std::endl;
    Bytes hp = genHotpatch(fields, hotpatch_size);
    makeExecutable(hotpatch, hotpatch_size);
    memcpy((void *)&hotpatch[0], hp.data(), hp.size());
    std::cout << "Hotpatch:" << std::endl;
    for (uint32_t j = 0; j < hp.size(); ++j) {
        char str[32];
        sprintf(str, "%02x ", hotpatch[j]);
        std::cout << str << " ";
    }
    std::cout << std::endl;

    // Create custom generated assembly for the given fields
    Bytes proc = genCopyAssembly(fields);
    makeExecutable(proc.data(), proc.size());

    // Assign pointer to function
    using CopyFieldsFn = void(Message *, uint8_t *);
    CopyFieldsFn *copyfn = reinterpret_cast<CopyFieldsFn *>(proc.data());

    // Setup trampoline
    Bytes tramp;
    uint8_t *trampend = &trampoline[5];  // next byte after jump
    genTrampoline(fields, trampend, tramp);
    makeExecutable(tramp.data(), tramp.size());
    uint64_t troffset = uint64_t(tramp.data()) - uint64_t(trampend);
    trampoline[0] = 0xe9;  // jumpq 32-bit
    trampoline[1] = uint8_t(troffset);
    trampoline[2] = uint8_t(troffset >> 8);
    trampoline[3] = uint8_t(troffset >> 16);
    trampoline[4] = uint8_t(troffset >> 24);

    // Create a typical message, fill fields
    Message msg;
    std::memset(&msg, 0, sizeof(msg));
    msg.field_1 = 1;
    msg.field_2 = 2;
    msg.field_3 = 3;
    msg.field_4 = 4;
    msg.field_5 = 5;
    msg.field_6 = 6;
    msg.field_7 = 7;
    msg.field_8 = 8;
    msg.field_9 = 9;
    msg.field_10 = 10;
    msg.field_11 = 11;
    msg.field_12 = 12;
    msg.field_13 = 13;
    msg.field_14 = 14;
    msg.field_15 = 15;
    msg.field_16 = 16;
    msg.field_17 = 17;
    msg.field_18 = 18;
    msg.field_19 = 19;
    msg.field_20 = 20;
    msg.field_21 = 21;
    msg.field_22 = 22;
    msg.field_23 = 23;
    msg.field_24 = 24;
    msg.field_25 = 25;
    msg.field_26 = 26;
    msg.field_27 = 27;
    msg.field_28 = 28;
    msg.field_29 = 29;
    msg.field_30 = 30;
    msg.field_31 = 31;
    msg.field_32 = 32;
    msg.field_33 = 33;
    msg.field_34 = 34;
    msg.field_35 = 35;
    msg.field_36 = 36;
    msg.field_37 = 37;
    msg.field_38 = 38;
    msg.field_39 = 39;
    msg.field_40 = 40;

    // Create an array to hold the results
    // This example is current not going to check the size but this should be
    // done using the metadata provided
    // So this *might* crash if you are a smart ass and pass a large enough list
    // Fill with some notorious values (0xff)
    std::vector<uint8_t> bytes(4096, 0xff);
    std::vector<uint8_t> golden = bytes;

    // Loop over a number of times doing copies and gathering
    // micro-benchmarking statistics
    double sum1 = 0;
    double sum2 = 0;
    double sum3 = 0;
    double sum4 = 0;
    double sum5 = 0;
    const unsigned numloops = 1000000;
    bool ok = false;
    for (uint32_t j = 0; j < numloops; ++j) {
        // Test "normal" way
        {
            std::fill(bytes.begin(), bytes.end(), 0xff);
            uint64_t t0 = tic();
            copyVanilla(bytes.data(), (uint8_t *)&msg, fields);
            uint64_t t1 = toc();
            sum1 += (t1 - t0);
            golden = bytes;
        }

        // Test with hotpatch
        {
            std::fill(bytes.begin(), bytes.end(), 0xff);
            uint64_t t0 = tic();
            asm(R"( 
            .globl hotpatch
            hotpatch:
            .zero 512
            .globl hotpatch_end 
            hotpatch_end:    
            )" ::"D"(&msg),
                "S"(bytes.data())
                : "rax", "memory");
            uint64_t t1 = toc();
            sum2 += (t1 - t0);

            compare("Hotpatch", golden, bytes);
        }

        // Test with trampoline
        {
            std::fill(bytes.begin(), bytes.end(), 0xff);
            uint64_t t0 = tic();
            asm(R"(
            .globl trampoline
            trampoline:
            .zero 5
            )" ::"D"(&msg),
                "S"(bytes.data())
                : "rax", "memory");
            uint64_t t1 = toc();
            sum5 += (t1 - t0);

            compare("Trampoline", golden, bytes);
        }

        // Test with pointer
        {
            std::fill(bytes.begin(), bytes.end(), 0xff);
            uint64_t t0 = tic();
            copyfn(&msg, bytes.data());
            uint64_t t1 = toc();
            sum3 += (t1 - t0);
            compare("Pointer", golden, bytes);
        }

        // Empty test
        {
            uint64_t t0 = tic();
            asm("" ::: "memory");
            uint64_t t1 = toc();
            sum4 += (t1 - t0);
        }
    }

    // Display statistics
    std::cout << "Average times:" << std::endl;
    std::cout << "\tVanilla   :" << (sum1 - sum4) / numloops << std::endl;
    std::cout << "\tHotpatch:  " << (sum2 - sum4) / numloops << std::endl;
    std::cout << "\tPointer:   " << (sum3 - sum4) / numloops << std::endl;
    std::cout << "\tTrampoline:" << (sum5 - sum4) / numloops << std::endl;
    std::cout << std::endl;

// Printout end result for peace of mind
#if 0 
    std::cout << "Bytes:\n\t";
    for (uint8_t val : bytes) {
        char str[16];
        ::sprintf(str, "%02x ", val);
        std::cout << str;
    }
    std::cout << std::endl;
#endif
    return 0;
}