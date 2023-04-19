/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "Matrix.h"

class CPlaceable;

#pragma pack(push, 4)
class CMatrixLink : public CMatrix {
public:
    CMatrixLink() : CMatrix() {}
    CMatrixLink(float fScale) : CMatrix() { SetScale(fScale); }

public:
    CPlaceable*  m_pOwner;
    CMatrixLink* m_pPrev;
    CMatrixLink* m_pNext;

public:
    static void InjectHooks();

    inline void Insert(CMatrixLink* pWhere);
    inline void Remove();
};
#pragma pack(pop)
static_assert(sizeof(CMatrixLink) == 0x54);
