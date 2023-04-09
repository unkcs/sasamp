/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#pragma pack(push, 4)
struct tGearRatio {
    float m_maxVelocity;
    float m_changeUpVelocity;   // max velocity needed to change the current gear to higher
    float m_changeDownVelocity; // min velocity needed to change the current gear to lower
};
#pragma pack(pop)

static_assert(sizeof(tGearRatio) == 0xC, "Invalid size tGearRatio");