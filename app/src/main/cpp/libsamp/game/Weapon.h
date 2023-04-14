//
// Created by plaka on 07.04.2023.
//

#ifndef LIVERUSSIA_WEAPON_H
#define LIVERUSSIA_WEAPON_H

#include <cstdint>

#pragma pack(push, 4)
struct CWeapon
{
    uint32_t dwType;
    uint32_t dwState;
    uint32_t dwAmmoInClip;
    uint32_t dwAmmo;
    uint32_t m_nTimer;
    bool m_bFirstPersonWeaponModeSelected;
    bool m_bDontPlaceInHand;
    uintptr_t *m_pWeaponFxSys;
};
static_assert(sizeof(CWeapon) == 0x1C, "Invalid size CPlaceable");
#pragma pack(pop)

#endif //LIVERUSSIA_WEAPON_H
