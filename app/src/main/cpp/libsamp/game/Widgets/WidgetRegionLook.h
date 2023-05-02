//
// Created by plaka on 01.05.2023.
//

#pragma once

#include "WidgetRegion.h"

#pragma pack(push, 1)
struct CWidgetRegionLook : CWidgetRegion
{
    bool m_bLookBack;
    bool m_bDisableLookBack;
    uint8_t pad1[2];
};
#pragma pack(pop)

VALIDATE_SIZE(CWidgetRegionLook, 0x94);
