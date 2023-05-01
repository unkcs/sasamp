//
// Created by plaka on 07.04.2023.
//

#pragma once

#include <cstdint>
#include "common.h"
#include "game/Enums/eWeaponType.h"

enum eWeaponState : uint32 {
    WEAPONSTATE_READY = 0,
    WEAPONSTATE_FIRING,
    WEAPONSTATE_RELOADING,
    WEAPONSTATE_OUT_OF_AMMO,
    WEAPONSTATE_MELEE_MADECONTACT,
};

#pragma pack(push, 4)
struct CWeapon
{
    eWeaponType m_nType;
    eWeaponState m_nState;
    uint32_t dwAmmoInClip;
    uint32_t dwAmmo;
    uint32_t m_nTimer;
    bool m_bFirstPersonWeaponModeSelected;
    bool m_bDontPlaceInHand;
    uintptr_t *m_pWeaponFxSys;
};
static_assert(sizeof(CWeapon) == 0x1C, "Invalid size CPlaceable");
#pragma pack(pop)
