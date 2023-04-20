//
// Created by plaka on 09.04.2023.
//

#pragma once

#include "game/Enums/eSurfaceType.h"
#include "Entity.h"

#pragma pack(push, 1)
struct CPhysical : public CEntityGta {
    float       m_fPrevDistFromCam;
    uint32_t    m_LastCollisionTime;
    union {
        struct {
            uint32_t bExtraHeavy: 1;
            uint32_t bDoGravity: 1;
            uint32_t bInfiniteMass: 1;
            uint32_t bInfiniteMassFixed: 1;
            uint32_t bPedPhysics: 1;
            uint32_t bDisableMoveForce: 1;
            uint32_t bHangingPhysics: 1;
            uint32_t bPoolBallPhysics: 1;

            uint32_t bIsInWater: 1;
            uint32_t bCollidedThisFrame: 1;
            uint32_t bUnFreezable: 1;
            uint32_t bTrainForceCol: 1;
            uint32_t bSkipLineCol: 1;
            uint32_t bDontApplySpeed: 1;
            uint32_t bDontLoadCollision: 1;
            uint32_t bHalfSpeedCollision: 1;

            uint32_t bForceHitReturnFalse: 1;
            uint32_t bDontProcessCollisionOurSelves: 1;
            uint32_t bNotDamagedByBullets: 1;
            uint32_t bNotDamagedByFlames: 1;
            uint32_t bNotDamagedByCollisions: 1;
            uint32_t bNotDamagedByMelee: 1;
            uint32_t bOnlyDamagedByPlayer: 1;
            uint32_t bIgnoresExplosions: 1;

            uint32_t bFlyer: 1;
            uint32_t bNeverGoStatic: 1;
            uint32_t bUsingSpecialColModel: 1;
            uint32_t bForceFullWaterCheck: 1;
            uint32_t bUsesCollisionRecords: 1;
            uint32_t bRenderScorched: 1;
            uint32_t bDoorHitEndStop: 1;
            uint32_t bCarriedByRope: 1;
        }physicalFlags;
        uint32 m_nPhysicalFlags;
    };
    CVector         m_vecMoveSpeed;
    CVector         m_vecTurnSpeed;
    CVector         m_vecMoveFriction;
    CVector         m_vecTurnFriction;
    CVector         m_vecAverageMoveSpeed;
    CVector         m_vecAverageTurnSpeed;
    float           m_fMass;
    float           m_fTurnMass;
    float           m_fMassMultiplier;
    float           m_fAirResistance;
    float           m_fElasticity;
    float           m_fBuoyancyConstant;
    CVector         m_vecCentreOfMass;
    uint8_t         m_pCollisionList[4];

    uint32_t        m_pMovingList;

    uint8_t         m_nFakePhysics;
    uint8_t         m_nNumEntitiesCollided;
    eSurfaceType    m_nContactSurface;
    uint8_t         _pad4;
    CEntityGta*     m_apCollidedEntities[6];
    float           m_fMovingSpeed; // m_fTrueDistanceTravelled
    float           m_fDamageIntensity; // m_fDamageImpulseMagnitude
    CEntityGta      *m_pDamageEntity;
    CVector         m_vecDamageNormal;
    CVector         m_vecDamagePos;
    uint16_t        m_nDamagedPieceType;
    uint8_t         _pad5[2];
    CEntityGta      *m_pAttachToEntity;
    CVector         m_vecAttachPosnOffset;
    CVector         m_vecAttachTiltOffset;
    CQuaternion     m_qAttachedEntityRotation;
    CEntityGta     *m_pNOCollisionVehicle;
    float           m_lightingFromCollision;
    float           m_lightingFromPointLights;
    uintptr_t      *m_pRealTimeShadow;

public:
    void RemoveAndAdd();
    void AddToMovingList();

    void RemoveFromMovingList();
    //void SetDamagedPieceRecord(float fDamageIntensity, CEntityGta* entity, CColPoint& colPoint, float fDistanceMult);
    void ApplyMoveForce(float x, float y, float z);
    void ApplyMoveForce(CVector force);
    void ApplyTurnForce(CVector force, CVector point);
    void ApplyForce(CVector vecMoveSpeed, CVector point, bool bUpdateTurnSpeed);

    CVector GetSpeed( CVector point);
    void ApplyMoveSpeed();
    void ApplyTurnSpeed();

    CVector& GetMoveSpeed()                 { return m_vecMoveSpeed; }
    void     SetVelocity(CVector velocity)  { m_vecMoveSpeed = velocity; } // 0x441130
    void     ResetMoveSpeed()               { SetVelocity(CVector{}); }

    CVector& GetTurnSpeed() { return m_vecTurnSpeed; }
    void ResetTurnSpeed() { m_vecTurnSpeed = CVector(); }
};
#pragma pack(pop)

static_assert(sizeof(CPhysical) == 0x138, "Invalid size CPhysical");