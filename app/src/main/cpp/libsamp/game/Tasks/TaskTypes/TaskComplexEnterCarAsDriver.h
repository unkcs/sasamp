/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "TaskComplexEnterCar.h"
//#include "Ped.h" // eMoveState

class CTaskComplexEnterCarAsDriver : public CTaskComplexEnterCar {
public:
    static void InjectHooks();
    static constexpr auto Type = TASK_COMPLEX_ENTER_CAR_AS_DRIVER;

    explicit CTaskComplexEnterCarAsDriver(CVehicleGta* targetVehicle);
    CTaskComplexEnterCarAsDriver(CVehicleGta* targetVehicle, eMoveState moveState);
    ~CTaskComplexEnterCarAsDriver() override = default;

    eTaskType GetTaskType() override { return Type; }
    CTask* Clone() override;
};
VALIDATE_SIZE(CTaskComplexEnterCarAsDriver, 0x50);
