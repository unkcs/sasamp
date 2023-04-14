//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "Placeable.h"
#include "game/CVector.h"

#pragma pack(push, 1)
typedef struct _ENTITY_TYPE : CPlaceable
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

    uint8_t _pad2[32];

    CVector vecMoveSpeed;
    CVector vecTurnSpeed;

    uint8_t _pad3[88];

    uint32_t dwUnkModelRel;
} ENTITY_TYPE;
#pragma pack(pop)
