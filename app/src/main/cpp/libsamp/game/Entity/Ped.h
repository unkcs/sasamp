//
// Created by plaka on 07.04.2023.
//

#pragma once

#pragma pack(1)

#include "Entity.h"
#include "../PedIntelligence.h"
#include "../Weapon.h"

typedef struct _PED_TYPE
{
    ENTITY_TYPE entity; 		// 0000-0184	;entity
    uint8_t _pad106[174];
    uint32_t _pad107;			// 0358-0362	;dwPedType
    uint8_t _pad101[722];
    CPedIntelligence* pPedIntelligence; // 1084-1088
    uint8_t _pad100[8];
    uint32_t dwAction;			// 1096-1100	;Action
    uint8_t _pad102[52];
    uint32_t dwStateFlags; 		// 1152-1156	;StateFlags

    uintptr_t dwInvulFlags; // 1136-1140		0x1000 = can_decap
    uint8_t _pad228[8];
    uintptr_t Tasks; // 1148-1152
    uintptr_t dwPlayerInfoOffset; // 1152-1156

    uint8_t _pad103[168];
    float fHealth;		 		// 1344-1348	;Health
    float fMaxHealth;			// 1348-1352	;MaxHealth
    float fArmour;				// 1352-1356	;Armour
    float fAim;
    uint8_t _pad104[8];
    float m_fCurrentRotation;			// 1368-1372	;Rotation1
    float m_fAimingRotation;			// 1372-1376	;Rotation2
    uint8_t _pad105[44];
    uint32_t pVehicle;			// 1420-1424	;pVehicle
    uint8_t _pad108[8];
    uint32_t dwPedType;			// 1432-1436	;dwPedType
    uint32_t dwUnk1;	 // 1436-1440
    CWeapon WeaponSlots[13]; // 1440-1804
    uint8_t _pad270[12];
    uint8_t byteCurWeaponSlot; // 1816-1817
    uint8_t _pad280[23];
    uint32_t pFireObject;	 // 1840-1844
    uint8_t _pad281[44];
    uint32_t  dwWeaponUsed; // 1888-1892
    uintptr_t pdwDamageEntity; // 1892-1896
} PED_TYPE;
