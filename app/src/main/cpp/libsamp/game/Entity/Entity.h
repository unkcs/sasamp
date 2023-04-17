//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "Placeable.h"
#include "game/CVector.h"
#include "game/Reference.h"
#include "../Core/Link.h"
#include "game/Enums/eEntityStatus.h"
#include "game/Enums/eEntityType.h"
#include "game/Enums/eSurfaceType.h"

#pragma pack(push, 1)
struct ENTITY_TYPE : CPlaceable
{
    union {
        uintptr_t m_pRwObject;
        uintptr_t m_pRpClump;
        uintptr_t m_pRpAtomic;
        uintptr_t pdwRenderWare;    // 24-28	;rwObject
    };

    union {
        uint32_t m_nEntityFlags;
        struct {
            uint32_t m_bUsesCollision : 1;
            uint32_t m_bCollisionProcessed : 1;
            uint32_t m_bIsStatic : 1;
            uint32_t m_bHasContacted : 1;
            uint32_t m_bIsStuck : 1;
            uint32_t m_bIsInSafePosition : 1;
            uint32_t m_bWasPostponed : 1;
            uint32_t m_bIsVisible : 1;

            uint32_t m_bIsBIGBuilding : 1;
            uint32_t m_bRenderDamaged : 1;
            uint32_t m_bStreamingDontDelete : 1;
            uint32_t m_bRemoveFromWorld : 1;
            uint32_t m_bHasHitWall : 1;
            uint32_t m_bImBeingRendered : 1;
            uint32_t m_bDrawLast :1;
            uint32_t m_bDistanceFade : 1;

            uint32_t m_bDontCastShadowsOn : 1;
            uint32_t m_bOffscreen : 1;
            uint32_t m_bIsStaticWaitingForCollision : 1;
            uint32_t m_bDontStream : 1;
            uint32_t m_bUnderwater : 1;
            uint32_t m_bHasPreRenderEffects : 1;
            uint32_t m_bIsTempBuilding : 1;
            uint32_t m_bDontUpdateHierarchy : 1;

            uint32_t m_bHasRoadsignText : 1;
            uint32_t m_bDisplayedSuperLowLOD : 1;
            uint32_t m_bIsProcObject : 1;
            uint32_t m_bBackfaceCulled : 1;
            uint32_t m_bLightObject : 1;
            uint32_t m_bUnimportantStream : 1;
            uint32_t m_bTunnel : 1;
            uint32_t m_bTunnelTransition : 1;
        } nEntityFlags;
    };

    uint16_t RandomSeed;

    uint16_t nModelIndex;
    CReference *pReferences;
    CLink<ENTITY_TYPE*> *m_pLastRenderedLink;
    uint16_t m_nScanCode;
    uint8_t m_iplIndex;
    uint8_t m_areaCode;
    ENTITY_TYPE *m_pLod;
    uint8_t numLodChildren;
    int8_t numLodChildrenRendered;
    eEntityType   m_nType : 3;          // Mask: & 0x7  = 7
    eEntityStatus m_nStatus : 5;        // Mask: & 0xF8 = 248 (Remember: In the original code unless this was left shifted the value it's compared to has to be left shifted by 3!)
    uint8_t pad_0;

    float m_fPrevDistFromCam;
    uint32_t m_LastCollisionTime;
    struct
    {
        uint32_t bExtraHeavy : 1;
        uint32_t bDoGravity : 1;
        uint32_t bInfiniteMass : 1;
        uint32_t bInfiniteMassFixed : 1;
        uint32_t bPedPhysics : 1;
        uint32_t bDoorPhysics : 1;
        uint32_t bHangingPhysics : 1;
        uint32_t bPoolBallPhysics : 1;
        uint32_t bIsInWater : 1;
        uint32_t bCollidedThisFrame : 1;
        uint32_t bUnFreezable : 1;
        uint32_t bTrainForceCol : 1;
        uint32_t bSkipLineCol : 1;
        uint32_t bCoorsFrozenByScript : 1;
        uint32_t bDontLoadCollision : 1;
        uint32_t bHalfSpeedCollision : 1;
        uint32_t bForceHitReturnFalse : 1;
        uint32_t bDontProcessCollisionOurSelves : 1;
        uint32_t bNotDamagedByBullets : 1;
        uint32_t bNotDamagedByFlames : 1;
        uint32_t bNotDamagedByCollisions : 1;
        uint32_t bNotDamagedByMelee : 1;
        uint32_t bOnlyDamagedByPlayer : 1;
        uint32_t bIgnoresExplosions : 1;
        uint32_t bFlyer : 1;
        uint32_t bNeverGoStatic : 1;
        uint32_t bUsingSpecialColModel : 1;
        uint32_t bForceFullWaterCheck : 1;
        uint32_t bUsesCollisionRecords : 1;
        uint32_t bRenderScorched : 1;
        uint32_t bDoorHitEndStop : 1;
        uint32_t bCarriedByRope : 1;
    };

    CVector vecMoveSpeed;
    CVector vecTurnSpeed;
    CVector m_vecMoveFriction;
    CVector m_vecTurnFriction;
    CVector m_vecAverageMoveSpeed;
    CVector m_vecAverageTurnSpeed;
    float m_fMass;
    float m_fTurnMass;
    float m_fMassMultiplier;
    float m_fAirResistance;
    float m_fElasticity;
    float m_fBuoyancyConstant;
    CVector m_vecCOM;
    uint8_t _pad3[4];

    uint32_t dwUnkModelRel;

    uint8_t             m_nFakePhysics;
    uint8_t             m_nNumEntitiesCollided;
    eSurfaceType        m_nContactSurface;
    uint8_t             _pad4;
    ENTITY_TYPE*        m_apCollidedEntities[6];
    float               m_fMovingSpeed; // m_fTrueDistanceTravelled
    float               m_fDamageIntensity; // m_fDamageImpulseMagnitude
    ENTITY_TYPE         *m_pDamageEntity;
    CVector             m_vecDamageNormal;
    CVector             m_vecDamagePos;
    uint16_t            m_nDamagedPieceType;
};
#pragma pack(pop)
