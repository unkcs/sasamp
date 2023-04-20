//
// Created by plaka on 06.04.2023.
//

#pragma once

#include "game/SimpleTransform.h"
#include "game/Core/MatrixLink.h"
#include "game/common.h"

#pragma pack(push, 1)
struct CPlaceable
{
    uint32_t            vtable;
    CSimpleTransform    m_transform;
    CMatrixLink         *mat;

public:
    static void InjectHooks();

    static void InitMatrixArray();

public:
    static constexpr uint32 NUM_MATRICES_TO_CREATE = 10000;

    inline CVector& GetRight() const { return mat->GetRight(); }
    inline CVector& GetForward() const { return mat->GetForward(); }
    inline CVector& GetUp() const { return mat->GetUp(); }
    inline const CVector& GetPosition() const { return mat ? mat->GetPosition() : m_transform.m_vPosn; }
    inline CVector& GetPosition() { return mat ? mat->GetPosition() : m_transform.m_vPosn; }
   // inline CVector2D GetPosition2D() { return { GetPosition() }; }
};
#pragma pack(pop)

VALIDATE_SIZE(CPlaceable, 0x18);
