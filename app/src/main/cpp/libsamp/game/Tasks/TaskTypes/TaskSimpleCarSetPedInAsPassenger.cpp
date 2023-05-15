#include "TaskSimpleCarSetPedInAsPassenger.h"
#include "TaskUtilityLineUpPedWithCar.h"
#include "util/patch.h"

// OG constructor was at 0x646FE0
CTaskSimpleCarSetPedInAsPassenger::CTaskSimpleCarSetPedInAsPassenger(CVehicleGta* targetVehicle, eTargetDoor nTargetDoor, bool warpingInToCar, CTaskUtilityLineUpPedWithCar* utility) :
        m_nTargetDoor{ nTargetDoor },
        m_pTargetVehicle{ targetVehicle },
        m_pUtility{ utility },
        m_bWarpingInToCar{warpingInToCar}
{
    CEntityGta::SafeRegisterRef(m_pTargetVehicle);
}


// 0x647080
CTaskSimpleCarSetPedInAsPassenger::~CTaskSimpleCarSetPedInAsPassenger() {
    CEntityGta::SafeCleanUpRef(m_pTargetVehicle);
}

// 0x649D90
CTask* CTaskSimpleCarSetPedInAsPassenger::Clone() {
    auto task = new CTaskSimpleCarSetPedInAsPassenger(m_pTargetVehicle, m_nTargetDoor, m_pUtility);
    task->m_bWarpingInToCar = m_bWarpingInToCar;
    task->m_nDoorFlagsToClear = m_nDoorFlagsToClear;
    task->m_nNumGettingInToClear = m_nNumGettingInToClear;
    return task;
}

// 0x64B5D0
bool CTaskSimpleCarSetPedInAsPassenger::ProcessPed(CPedGta* ped) {
    return CHook::CallFunction<bool>(g_libGTASA + 0x0048EE40 + 1, this, ped);
  //  return plugin::CallMethodAndReturn<bool, 0x64B5D0, CTask*, CPed*>(this, ped);
}
