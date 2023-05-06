/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once


#include "../../common.h"
#include "../../Entity/Vehicle.h"
#include "../../Animation/AnimBlendAssociation.h"

class CTaskUtilityLineUpPedWithCar {
public:
    CVector m_Offset;
    float   m_fDoorOpenPosZ;
    int32   m_fTime;
    int32   m_nDoorOpenPosType;
    int32   m_nDoorIdx;

public:
    CTaskUtilityLineUpPedWithCar(const CVector& offset, int32 time, int32 doorOpenPosType, int32 doorIdx);
    ~CTaskUtilityLineUpPedWithCar() = default;

    CVector GetLocalPositionToOpenCarDoor(CVehicleGta* vehicle, float animProgress, CAnimBlendAssociation* assoc);
    CVector GetPositionToOpenCarDoor(CVehicleGta* vehicle, float animProgress, CAnimBlendAssociation* assoc);
    bool ProcessPed(CPedGta* ped, CVehicleGta* vehicle, CAnimBlendAssociation* assoc);

private:
    friend void InjectHooksMain();
    static void InjectHooks();

    CTaskUtilityLineUpPedWithCar* Constructor(const CVector& offsets, int32 nTime, int32 doorOpenPosType, int32 doorIdx) { CTaskUtilityLineUpPedWithCar(offsets, nTime, doorOpenPosType, doorIdx); return this; }
    CTaskUtilityLineUpPedWithCar* Destructor() { this->CTaskUtilityLineUpPedWithCar::~CTaskUtilityLineUpPedWithCar(); return this; }
};
VALIDATE_SIZE(CTaskUtilityLineUpPedWithCar, 0x1C);
