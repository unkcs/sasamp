//
// Created by plaka on 09.04.2023.
//

#include "EntityGta.h"
#include "game/Enums/eSurfaceType.h"

#pragma pack(push, 1)
struct CPhysical : public CEntityGta {
    float       m_fPrevDistFromCam;
    uint32_t    m_nLastCollisionTime;
    union {
        struct {
            uint32_t bMakeMassTwiceAsBig: 1;
            uint32_t bApplyGravity: 1;
            uint32_t bDisableCollisionForce: 1;
            uint32_t bCollidable: 1;
            uint32_t bDisableTurnForce: 1;
            uint32_t bDisableMoveForce: 1;
            uint32_t bInfiniteMass: 1;
            uint32_t bDisableZ: 1;
    
            uint32_t bSubmergedInWater: 1;
            uint32_t bOnSolidSurface: 1;
            uint32_t bBroken: 1;
            uint32_t bProcessCollisionEvenIfStationary: 1; // ref @ 0x6F5CF0
            uint32_t bSkipLineCol: 1;                               // only used for peds
            uint32_t bDontApplySpeed: 1;
            uint32_t b15: 1;
            uint32_t bProcessingShift: 1;
    
            uint32_t b17: 1;
            uint32_t bDisableSimpleCollision: 1; // ref @ CPhysical::ProcessCollision
            uint32_t bBulletProof: 1;
            uint32_t bFireProof: 1;
            uint32_t bCollisionProof: 1;
            uint32_t bMeleeProof: 1;
            uint32_t bInvulnerable: 1;
            uint32_t bExplosionProof: 1;
    
            uint32_t bDontCollideWithFlyers: 1;
            uint32_t bAttachedToEntity: 1;
            uint32_t bAddMovingCollisionSpeed: 1;
            uint32_t bTouchingWater: 1;
            uint32_t bCanBeCollidedWith: 1;
            uint32_t bDestroyed: 1;
            uint32_t b31: 1;
            uint32_t b32: 1;
        } physicalFlags;
        uint32_t m_nPhysicalFlags;
    };
    CVector m_vecMoveSpeed;
    CVector m_vecTurnSpeed;
    CVector m_vecFrictionMoveSpeed;
    CVector m_vecFrictionTurnSpeed;
    CVector m_vecForce;
    CVector m_vecTorque;
    float m_fMass;
    float m_fTurnMass;
    float m_fVelocityFrequency;
    float m_fAirResistance;
    float m_fElasticity;
    float m_fBuoyancyConstant;
    CVector m_vecCentreOfMass;
    uintptr_t m_pCollisionList; // CEntryInfoList
    uintptr_t *m_pMovingList; // CPtrNodeDoubleLink
    uint8_t m_nFakePhysics;
    uint8_t m_nNumEntitiesCollided;
    eSurfaceType m_nContactSurface;
    uint8_t pad_0;
    CEntityGta *m_apCollidedEntities[6];
    float m_fMovingSpeed; // ref @ CTheScripts::IsVehicleStopped
    float m_fDamageIntensity;
    CEntityGta *m_pDamageEntity;
    CVector m_vecLastCollisionImpactVelocity;
    CVector m_vecLastCollisionPosn;
    uint16_t m_nPieceType;
    uint16_t pad_1;
    CEntityGta *m_pAttachedTo;
    CVector m_vecAttachOffset;
    CVector m_vecAttachedEntityRotation;
    CQuaternion m_qAttachedEntityRotation;
    CEntityGta *m_pEntityIgnoredCollision;
    float m_fContactSurfaceBrightness;
    float m_fDynamicLighting;
    uintptr_t *m_pShadowData; // CRealTimeShadow
};
#pragma pack(pop)
static_assert(sizeof(CPhysical) == 0x13C, "Invalid size CPhysical");