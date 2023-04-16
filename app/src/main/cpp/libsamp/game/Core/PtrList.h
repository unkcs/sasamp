/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include <cstdint>
#include "PtrNode.h"

class CPtrList {
public:
    CPtrNode* m_node;

public:
    static void InjectHooks();

    CPtrList();

    uint32_t CountElements() const;
    bool IsMemberOfList(void* data) const;

    CPtrNode* GetNode() const { return m_node; }
    bool IsEmpty() const { return !m_node; }
};

static_assert(sizeof(CPtrList) == 0x4);
