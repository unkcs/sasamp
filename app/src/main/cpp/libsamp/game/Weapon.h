//
// Created by plaka on 07.04.2023.
//

#pragma once

#include <cstdint>
#include "common.h"
#include "game/Enums/eWeaponType.h"

enum class eWeaponSlot : uint32 {
    UNARMED,
    MELEE,
    HANDGUN,
    SHOTGUN,
    SMG,        // Used for drive-by's
    RIFLE,
    SNIPER,
    HEAVY,
    THROWN,
    SPECIAL,
    GIFT,
    PARACHUTE,
    DETONATOR,
};
constexpr auto NUM_WEAPON_SLOTS = static_cast<size_t>(eWeaponSlot::DETONATOR) + 1u;

enum eWeaponState : uint32 {
    WEAPONSTATE_READY = 0,
    WEAPONSTATE_FIRING,
    WEAPONSTATE_RELOADING,
    WEAPONSTATE_OUT_OF_AMMO,
    WEAPONSTATE_MELEE_MADECONTACT,
};

#pragma pack(push, 1)
struct CWeapon
{
    eWeaponType m_nType;
    eWeaponState m_nState;
    uint32_t dwAmmoInClip;
    uint32_t m_nTotalAmmo;
    uint32_t m_nTimer;
    bool m_bFirstPersonWeaponModeSelected;
    bool m_bDontPlaceInHand;
    uint8_t pad[2];
    uintptr_t *m_pWeaponFxSys;

public:
    bool IsTypeMelee();
    bool IsType2Handed();
    bool IsTypeProjectile();

    bool HasWeaponAmmoToBeUsed();

    bool IsFiredWeapon() const;
};
static_assert(sizeof(CWeapon) == 0x1C, "Invalid size CPlaceable");
#pragma pack(pop)
