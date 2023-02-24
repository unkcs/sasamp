/*
    Plugin-SDK (Grand Theft Auto San Andreas) header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

enum eWeaponType : int32_t
{
    WEAPON_UNARMED = 0,
    WEAPON_BRASSKNUCKLE,
    WEAPON_GOLFCLUB,
    WEAPON_NIGHTSTICK,
    WEAPON_KNIFE,
    WEAPON_BASEBALLBAT,
    WEAPON_SHOVEL,
    WEAPON_POOL_CUE,
    WEAPON_KATANA,
    WEAPON_CHAINSAW,

    // gifts
    WEAPON_DILDO1, // 10
    WEAPON_DILDO2,
    WEAPON_VIBE1,
    WEAPON_VIBE2,
    WEAPON_FLOWERS,
    WEAPON_CANE,

    WEAPON_GRENADE,
    WEAPON_TEARGAS,
    WEAPON_MOLOTOV,
    WEAPON_ROCKET,
    WEAPON_ROCKET_HS, // 20
    WEAPON_FREEFALL_BOMB,

    // FIRST SKILL WEAPON
    WEAPON_PISTOL,
    WEAPON_PISTOL_SILENCED,
    WEAPON_DESERT_EAGLE,
    WEAPON_SHOTGUN,
    WEAPON_SAWNOFF_SHOTGUN, // one handed
    WEAPON_SPAS12_SHOTGUN,
    WEAPON_MICRO_UZI,
    WEAPON_MP5,
    WEAPON_AK47, // 30
    WEAPON_M4,
    WEAPON_TEC9,
    // END SKILL WEAPONS

    WEAPON_COUNTRYRIFLE,
    WEAPON_SNIPERRIFLE,
    WEAPON_RLAUNCHER,
    WEAPON_RLAUNCHER_HS,
    WEAPON_FLAMETHROWER,
    WEAPON_MINIGUN,
    WEAPON_REMOTE_SATCHEL_CHARGE,
    WEAPON_DETONATOR, // 40
    WEAPON_SPRAYCAN,
    WEAPON_EXTINGUISHER,
    WEAPON_CAMERA,
    WEAPON_NIGHTVISION,
    WEAPON_INFRARED,
    WEAPON_PARACHUTE,   // 46 (0x2E)
    WEAPON_LAST_WEAPON, // 47 (0x2F)

    WEAPON_ARMOUR, // 48 (0x30)

    // these are possible ways to die
    WEAPON_RAMMEDBYCAR, // 49 (0x31)
    WEAPON_RUNOVERBYCAR, // 50 (0x32)
    WEAPON_EXPLOSION,
    WEAPON_UZI_DRIVEBY,
    WEAPON_DROWNING,
    WEAPON_FALL,
    WEAPON_UNIDENTIFIED, // Used for damage being done
    WEAPON_ANYMELEE,
    WEAPON_ANYWEAPON,
    WEAPON_FLARE,
};
