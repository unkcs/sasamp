/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "common.h"

#pragma pack(push, 1)
class CQueuedMode {
public:
    uint16  m_nMode;
    uint8   pad_0[2];
    float   m_fDuration;
    uint16  m_nMinZoom;
    uint16  m_nMaxZoom;
};
#pragma pack(pop)

VALIDATE_SIZE(CQueuedMode, 0xC);
