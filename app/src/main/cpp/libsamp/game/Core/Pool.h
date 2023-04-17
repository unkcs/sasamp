//
// Created by plaka on 17.04.2023.
//

#pragma once

#include <cstddef>
#include <cstdint>

union tPoolObjectFlags {
    struct {
        uint8_t nId : 7;    // Mask: 0x7F
        bool  bEmpty : 1; // Mask: 0x80
    };

private:
    uint8_t nValue;

public:
    void Init() {
        bEmpty = true;
        nId = 0;
    }

    uint8_t IntValue() {
        return nValue;
    }
};

struct PoolAllocator {

    struct Pool {
        void* objects;
        uint8_t* flags;
        uint32_t count;
        uint32_t top;
        uint32_t bInitialized;
    };
    static_assert(sizeof(Pool) == 0x14);

    static Pool* Allocate(size_t count, size_t size) {

        Pool* p = new Pool;

        p->objects = new char[size * count];
        p->flags = new uint8_t[count];
        p->count = count;
        p->top = 0xFFFFFFFF;
        p->bInitialized = 1;

        for (size_t i = 0; i < count; i++) {
            p->flags[i] |= 0x80;
            p->flags[i] &= 0x80;
        }

        return p;
    }
};
