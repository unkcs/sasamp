//
// Created by plaka on 06.04.2023.
//

#ifndef LIVERUSSIA_PLACEABLE_H
#define LIVERUSSIA_PLACEABLE_H

#include "../RwMatrix.h"
#include "game/SimpleTransform.h"

struct CPlaceable
{
    uint32_t            vtable;
    CSimpleTransform    m_transform;
    RwMatrix            *mat;
};
static_assert(sizeof(CPlaceable) == 0x18, "Invalid size CPlaceable");

#endif //LIVERUSSIA_PLACEABLE_H
