//
// Created by plaka on 07.04.2023.
//

#ifndef LIVERUSSIA_WEAPON_H
#define LIVERUSSIA_WEAPON_H

#pragma pack(1)

#include <cstdint>

struct CWeapon
{
    uint32_t dwType;
    uint32_t dwState;
    uint32_t dwAmmoInClip;
    uint32_t dwAmmo;
    uint8_t _pwep1[12];
};  // MUST BE EXACTLY ALIGNED TO 28 bytes
static_assert(sizeof(CWeapon) == 0x1C, "Invalid size CPlaceable");

#endif //LIVERUSSIA_WEAPON_H
