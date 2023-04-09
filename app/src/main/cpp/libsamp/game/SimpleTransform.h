//
// Created by plaka on 09.04.2023.
//

#ifndef LIVERUSSIA_SIMPLETRANSFORM_H
#define LIVERUSSIA_SIMPLETRANSFORM_H


#include "CVector.h"
#include "RwMatrix.h"

class CSimpleTransform {
public:
    CSimpleTransform() : m_vPosn(), m_fHeading(0.0F) {}

public:
    CVector m_vPosn;
    float m_fHeading;

    void UpdateRwMatrix(RwMatrix* out);
    void Invert(const CSimpleTransform& base);
    void UpdateMatrix(class CMatrix* out);
};
static_assert(sizeof(CSimpleTransform) == 0x10, "Invalid size CSimpleTransform");

#endif //LIVERUSSIA_SIMPLETRANSFORM_H
