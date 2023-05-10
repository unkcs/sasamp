//
// Created by plaka on 09.04.2023.
//

#pragma once

#include "game/Enums/eSurfaceType.h"
#include "Entity.h"
#include "game/Core/PtrNodeDoubleLink.h"
#include "game/Core/EntryInfoList.h"

#pragma pack(push, 1)
struct CPhysical : public CEntityGta {
    float       m_fPrevDistFromCam;
    uint32_t    m_LastCollisionTime;
    union {
        struct {
            uint32_t bMakeMassTwiceAsBig: 1;
            uint32_t bDoGravity: 1;
            uint32_t bInfiniteMass: 1;
            uint32_t bInfiniteMassFixed: 1;
            uint32_t bDisableTurnForce: 1;
            uint32_t bDisableMoveForce: 1;
            uint32_t bHangingPhysics: 1;
            uint32_t bDisableZ: 1;

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
            uint32_t bCanBeCollidedWith: 1;
            uint32_t bRenderScorched: 1;
            uint32_t bDoorHitEndStop: 1;
            uint32_t bCarriedByRope: 1;
        }physicalFlags;
        uint32 m_nPhysicalFlags;
    };
    CVector         m_vecMoveSpeed;
    CVector         m_vecTurnSpeed;
    CVector         m_vecFrictionMoveSpeed;
    CVector         m_vecFrictionTurnSpeed;
    CVector         m_vecAverageMoveSpeed;
    CVector         m_vecAverageTurnSpeed;
    float           m_fMass;
    float           m_fTurnMass;
    float           m_fMassMultiplier;
    float           m_fAirResistance;
    float           m_fElasticity;
    float           m_fBuoyancyConstant;
    CVector         m_vecCentreOfMass;
    CEntryInfoList  m_pCollisionList;

    CPtrNodeDoubleLink *m_pMovingList;

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
    CEntityGta      *m_pAttachedTo;
    CVector         m_vecAttachPosnOffset;
    CVector         m_vecAttachTiltOffset;
    CQuaternion     m_qAttachedEntityRotation;
    CEntityGta     *m_pNOCollisionVehicle;
    float           m_lightingFromCollision;
    float           m_lightingFromPointLights;
    uintptr_t      *m_pRealTimeShadow;

public:
    static inline float DAMPING_LIMIT_IN_FRAME = 0.25;
    static inline float DAMPING_LIMIT_OF_SPRING_FORCE = 0.999;
    static inline float PHYSICAL_SHIFT_SPEED_DAMP = 0.707;
    static inline float SOFTCOL_SPEED_MULT = 0.95;
    static inline float SOFTCOL_SPEED_MULT2 = 1.05;
    static inline float SOFTCOL_DEPTH_MIN = 0.5;
    static inline float SOFTCOL_DEPTH_MULT = 2.0;
    static inline float SOFTCOL_CARLINE_SPEED_MULT = 0.4;
    static inline float TEST_ADD_AMBIENT_LIGHT_FRAC = 0.5;
    static inline float HIGHSPEED_ELASTICITY_MULT_COPCAR = 2.0;
  //  static CVector& fxDirection;

public:
    static void InjectHooks();

    void RemoveAndAdd();
    void AddToMovingList();

    void RemoveFromMovingList();
    //void SetDamagedPieceRecord(float fDamageIntensity, CEntityGta* entity, CColPoint& colPoint, float fDistanceMult);
    void ApplyMoveForce(float x, float y, float z);
    void ApplyMoveForce(CVector force);
    void ApplyTurnForce(CVector force, CVector point);
    void ApplyForce(CVector vecMoveSpeed, CVector point, bool bUpdateTurnSpeed);
    bool ApplySpringDampening(float fDampingForce, float fSpringForceDampingLimit, CVector& direction, CVector& collisionPoint, CVector& collisionPos);
    bool ApplySpringDampeningOld(float arg0, float arg1, CVector& arg2, CVector& arg3, CVector& arg4);
    bool ApplySpringCollisionAlt(float fSuspensionForceLevel, CVector& direction, CVector& collisionPoint, float fSpringLength, float fSuspensionBias, CVector& normal, float& fSpringForceDampingLimit);

    CVector GetSpeed( CVector point);
    void ApplyMoveSpeed();
    void ApplyTurnSpeed();

    CVector& GetMoveSpeed()                 { return m_vecMoveSpeed; }
    void     SetVelocity(CVector velocity)  { m_vecMoveSpeed = velocity; } // 0x441130

    CVector& GetTurnSpeed()         { return m_vecTurnSpeed; }
    void ResetTurnSpeed()           { m_vecTurnSpeed = CVector(); }
    void SetTurnSpeed(CVector vec)  { m_vecTurnSpeed = vec; }

    void ResetMoveSpeed()           { SetVelocity(CVector{}); }
    void ResetFrictionMoveSpeed()   { m_vecFrictionMoveSpeed = CVector(); }
    void ResetFrictionTurnSpeed()   { m_vecFrictionTurnSpeed = CVector(); }

    float GetMass(const CVector& pos, const CVector& dir) {
        return 1.0f / (CrossProduct(pos, dir).SquaredMagnitude() / m_fTurnMass +
                       1.0f / m_fMass);
    }
};
#pragma pack(pop)

static_assert(sizeof(CPhysical) == 0x13C-4, "Invalid size CPhysical");