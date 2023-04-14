//
// Created by plaka on 07.03.2023.
//

#pragma once

#include <cstdint>

#include "TimeInfo.h"

enum ModelInfoType : uint8_t {
    MODEL_INFO_ATOMIC = 1,
    MODEL_INFO_TIME = 3,
    MODEL_INFO_WEAPON = 4,
    MODEL_INFO_CLUMP = 5,
    MODEL_INFO_VEHICLE = 6,
    MODEL_INFO_PED = 7,
    MODEL_INFO_LOD = 8
};

class CTimeInfo;

class CAtomicModelInfo;
class CClumpModelInfo;
class CDamageAtomicModelInfo;
class CLodAtomicModelInfo;
class CLodTimeModelInfo;
class CPedModelInfo;
class CTimeModelInfo;
class CVehicleModelInfo;
class CWeaponModelInfo;
struct RwObject;

#pragma pack(push, 4)
struct CBaseModelInfo {
    uintptr_t 	vtable;
    uint32_t    m_nKey;
    char        m_modelName[21];
    int16_t     m_nRefCount;
    int16_t     m_nTxdIndex;
    uint8_t     m_nAlpha;
    uint8_t     m_n2dfxCount;
    int16_t     m_n2dEffectIndex;
    int16_t     m_nObjectInfoIndex; // m_dynamicIndex
    union {
        uint16_t m_nFlags;

        struct {
            /* https://github.com/multitheftauto/mtasa-blue/blob/master/Client/game_sa/CModelInfoSA.h */
            uint8_t bHasBeenPreRendered : 1; // we use this because we need to apply changes only once
            uint8_t bDrawLast : 1;
            uint8_t bAdditiveRender : 1;
            uint8_t bDontWriteZBuffer : 1;
            uint8_t bDontCastShadowsOn : 1;
            uint8_t bDoWeOwnTheColModel : 1;
            uint8_t bIsBackfaceCulled : 1;
            uint8_t bIsLod : 1;

            // 1st byte
            union {
                struct { // Atomic flags
                    uint8_t bIsRoad : 1;
                    uint8_t bAtomicFlag0x200: 1;
                    uint8_t bDontCollideWithFlyer : 1;
                    uint8_t nSpecialType : 4;
                    uint8_t bWetRoadReflection : 1;
                };
                struct { // Vehicle flags
                    uint8_t bUsesVehDummy : 1;
                    uint8_t : 1;
                    uint8_t nCarmodId : 5;
                    uint8_t bUseCommonVehicleDictionary : 1;
                };
                struct { // Clump flags
                    uint8_t bHasAnimBlend : 1;
                    uint8_t bHasComplexHierarchy : 1;
                    uint8_t bAnimSomething : 1;
                    uint8_t bOwnsCollisionModel : 1;
                    uint8_t : 3;
                    uint8_t bTagDisabled : 1;
                };
            };
        };
    };
    uintptr_t   *m_pColModel;     // CColModel
    float       m_fDrawDistance;  // m_lodDistance
    union {
        struct RwObject *m_pRwObject;
        struct RwClump  *m_pRwClump;
        struct RpAtomic *m_pRwAtomic;
    };

    CVehicleModelInfo* AsVehicleModelInfoPtr() { return reinterpret_cast<CVehicleModelInfo*>(this); }
    CPedModelInfo*     AsPedModelInfoPtr()     { return reinterpret_cast<CPedModelInfo*>(this); }
    CWeaponModelInfo*  AsWeaponModelInfoPtr()  { return reinterpret_cast<CWeaponModelInfo*>(this); }
};
#pragma pack(pop)

static_assert(sizeof(CBaseModelInfo) == 0x38, "Invalid size CBaseModelInfo");
