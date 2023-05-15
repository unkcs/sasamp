/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "../TaskSimple.h"
#include "TaskUtilityLineUpPedWithCar.h"
#include "../../Enums/eTargetDoor.h"

class CVehicleGta;
class CAnimBlendAssociation;
class CTaskUtilityLineUpPedWithCar;


class CTaskSimpleCarSetPedInAsPassenger : public CTaskSimple {
public:
    bool                          m_bIsFinished{};
    uint8_t                       pad0[3];
    CAnimBlendAssociation*        m_pAnim{};
    CVehicleGta*                  m_pTargetVehicle{};
    eTargetDoor                   m_nTargetDoor{};
    CTaskUtilityLineUpPedWithCar* m_pUtility{};
    bool                          m_bWarpingInToCar{};
    uint8                         m_nDoorFlagsToClear{};
    uint8                         m_nNumGettingInToClear{};
    uint8_t                       pad1;

public:
    static constexpr auto Type = TASK_SIMPLE_CAR_SET_PED_IN_AS_PASSENGER;

    CTaskSimpleCarSetPedInAsPassenger(CVehicleGta* targetVehicle, eTargetDoor nTargetDoor, bool warpingInToCar /* notsa arg */ = false, CTaskUtilityLineUpPedWithCar* utility = nullptr);
    ~CTaskSimpleCarSetPedInAsPassenger() override;

    eTaskType GetTaskType() override { return Type; }
    CTask* Clone() override;
    bool MakeAbortable(CPedGta* ped, eAbortPriority priority, const CEvent* event) override { return false; }
    bool ProcessPed(CPedGta* ped) override;
};
VALIDATE_SIZE(CTaskSimpleCarSetPedInAsPassenger, 0x20);
