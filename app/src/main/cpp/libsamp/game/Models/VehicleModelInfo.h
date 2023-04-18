//
// Created by plaka on 07.03.2023.
//

#ifndef LIVERUSSIA_VEHICLEMODELINFO_H
#define LIVERUSSIA_VEHICLEMODELINFO_H


#include "ClumpModelInfo.h"
#include "game/Enums/eVehicleClass.h"
#include "../quaternion.h"
#include "../CVector.h"
#include "game/RW/rpworld.h"
#include "game/Enums/eVehicleType.h"

enum eVehicleDummy {
    DUMMY_LIGHT_FRONT_MAIN      = 0,
    DUMMY_LIGHT_REAR_MAIN       = 1,

    DUMMY_LIGHT_FRONT_SECONDARY = 2,
    DUMMY_LIGHT_REAR_SECONDARY  = 3,

    DUMMY_SEAT_FRONT            = 4,
    DUMMY_SEAT_REAR             = 5,

    DUMMY_EXHAUST               = 6,
    DUMMY_ENGINE                = 7,
    DUMMY_GAS_CAP               = 8,
    DUMMY_TRAILER_ATTACH        = 9,
    DUMMY_HAND_REST             = 10,
    DUMMY_EXHAUST_SECONDARY     = 11,
    DUMMY_WING_AIR_TRAIL        = 12,
    DUMMY_VEHICLE_GUN           = 13,
};

union tVehicleCompsUnion {
    uint32_t m_nComps;
    struct {
        uint32_t nExtraA_comp1 : 4;
        uint32_t nExtraA_comp2 : 4;
        uint32_t nExtraA_comp3 : 4;
        uint32_t : 4;

        uint32_t nExtraB_comp1 : 4;
        uint32_t nExtraB_comp2 : 4;
        uint32_t nExtraB_comp3 : 4;
        uint32_t : 4;
    };
    struct {
        uint32_t nExtraAComp : 12;
        uint32_t nExtraARule : 4;

        uint32_t nExtraBComp : 12;
        uint32_t nExtraBRule : 4;
    };

    struct {
        uint32_t nExtraA : 16;
        uint32_t nExtraB : 16;
    };
};

struct UpgradePosnDesc {
    CVector         m_vPosition;
    CQuaternion     m_qRotation;
    int32_t         m_nParentComponentId;
};
static_assert(sizeof(UpgradePosnDesc) == 0x20, "Invalid size UpgradePosnDesc");

#pragma pack(push, 1)
struct CVehicleModelInfo : public CClumpModelInfo {
    RpMaterial*         m_pPlateMaterial; // 3C
    char                m_szPlateText[9]; // 40
    unsigned char       m_nPlateType;    //  49
    char                m_szGameName[8]; //  4a
    char               _pad3A[2];
    eVehicleType        m_nVehicleType;  //  54
    float               m_fWheelSizeFront;// 58
    float               m_fWheelSizeRear;    //5C
    int16_t             m_nWheelModelIndex;  //60
    int16_t             m_nHandlingId;       // 62
    int8_t              m_nNumDoors;     //64
    eVehicleClass       m_nVehicleClass; //65
    int8_t              m_nFlags;    //66
    int8_t              m_nWheelUpgradeClass;//67
    int8_t              m_nTimesUsed;    //68
    uint8_t             pad_0;
    int16_t             m_freq;
    tVehicleCompsUnion  m_extraComps;    //6c
    float               m_fBikeSteerAngle; // 70

    struct {
        CVector         m_avDummyPos[15];
        UpgradePosnDesc m_aUpgrades[18];
        RpAtomic*       m_apExtras[6];
        RpAtomic*       m_pXtraAtomicLOD[6];
        int8_t          m_nNumExtras;
        uint32_t        m_nMaskComponentsDamagable;
    } * m_pVehicleStruct;

    RpMaterial* m_firstColour[49];
    RpMaterial* m_secondColour[33];
    RpMaterial* m_thirdColour[17];
    RpMaterial* m_fourthColour[17];
    RpMaterial* m_apDirtMaterials[64];
    uint8_t     m_possibleColours[4][8];
    uint8_t     m_numPossibleColours;
    uint8_t     m_lastColUsed;
    uint8_t     m_lastCol[4];
    int16_t     m_anUpgrades[18];
    uint8_t     pad_1[2];
    const char* m_remaps[4];

    union {
        uintptr_t* m_pAnimBlock;
        char* m_animBlockFileName;
        unsigned int m_dwAnimBlockIndex;
    };

};
#pragma pack(pop)
static_assert(sizeof(CVehicleModelInfo) == 0x3a8, "Invalid size cTransmission");


#endif //LIVERUSSIA_VEHICLEMODELINFO_H
