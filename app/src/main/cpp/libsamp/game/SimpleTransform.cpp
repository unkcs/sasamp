//
// Created by plaka on 09.04.2023.
//

#include "SimpleTransform.h"
#include "main.h"

void CSimpleTransform::UpdateRwMatrix(RwMatrix* out)
{
    ((void(__thiscall*)(CSimpleTransform*, RwMatrix*))(g_libGTASA + 0x00408618 + 1))(this, out);
}

void CSimpleTransform::Invert(const CSimpleTransform& base)
{
    ((void(__thiscall*)(CSimpleTransform*, const CSimpleTransform&))(g_libGTASA + 0x0040869A + 1))(this, base);
}

void CSimpleTransform::UpdateMatrix(CMatrix* out)
{
    ((void(__thiscall*)(CSimpleTransform*, class CMatrix*))(g_libGTASA + 0x0040867C + 1))(this, out);
}