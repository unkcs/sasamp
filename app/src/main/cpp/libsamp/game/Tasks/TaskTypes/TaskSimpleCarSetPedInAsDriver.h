/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "../../Entity/Vehicle.h"
#include "../TaskSimple.h"
#include "../../Animation/AnimBlendAssociation.h"
#include "TaskUtilityLineUpPedWithCar.h"

class CTaskSimpleCarSetPedInAsDriver : public CTaskSimple {
public:
    bool                          m_bIsFinished;
    uint8_t                       pad11[3];
    CAnimBlendAssociation*        m_pAnim;
    CVehicleGta*                  m_pTargetVehicle;
    CTaskUtilityLineUpPedWithCar* m_pUtility;
    bool                          m_bWarpingInToCar;
    uint8                         m_nDoorFlagsToClear;
    uint8                         m_nNumGettingInToClear;
    uint8_t                       pad12;

public:
    static constexpr auto Type = TASK_SIMPLE_CAR_SET_PED_IN_AS_DRIVER;

    CTaskSimpleCarSetPedInAsDriver(CVehicleGta *targetVehicle, CTaskUtilityLineUpPedWithCar *utility = nullptr);
    CTaskSimpleCarSetPedInAsDriver(CVehicleGta *targetVehicle, bool warpingInToCar, CTaskUtilityLineUpPedWithCar *utility = nullptr);
    ~CTaskSimpleCarSetPedInAsDriver() override;

    eTaskType GetTaskType() override { return Type; }
    CTask* Clone() override;
    bool MakeAbortable(CPed* ped, eAbortPriority priority, const CEvent* event) override { return false; }
    bool ProcessPed(CPed* ped) override;
};
VALIDATE_SIZE(CTaskSimpleCarSetPedInAsDriver, 0x1C);
