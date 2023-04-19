/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "../Entity.h"

#pragma pack(push, 1)
class CDummy : public CEntityGta {
//public:
//    CDummy();
//    ~CDummy() override = default;
//
//    static void* operator new(size_t size);
//    static void  operator delete(void* obj);
//
//public:
//    static void InjectHooks();
};
#pragma pack(pop)
static_assert(sizeof(CDummy) == 0x38);


bool IsDummyPointerValid(CDummy* dummy);
