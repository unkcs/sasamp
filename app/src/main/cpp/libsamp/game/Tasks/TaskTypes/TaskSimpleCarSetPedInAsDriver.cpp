#include "../../common.h"
#include "TaskSimpleCarSetPedInAsDriver.h"
#include "util/patch.h"

// 0x6470E0
CTaskSimpleCarSetPedInAsDriver::CTaskSimpleCarSetPedInAsDriver(CVehicleGta* targetVehicle, CTaskUtilityLineUpPedWithCar* utility) : CTaskSimple() {
    m_bIsFinished = 0;
    m_pAnim = 0;
    m_pTargetVehicle = targetVehicle;
    m_pUtility = utility;
    m_bWarpingInToCar = 0;
    m_nDoorFlagsToClear = 0;
    m_nNumGettingInToClear = 0;
    CEntityGta::SafeRegisterRef(m_pTargetVehicle);
}

CTaskSimpleCarSetPedInAsDriver::CTaskSimpleCarSetPedInAsDriver(CVehicleGta* targetVehicle, bool warpingInToCar, CTaskUtilityLineUpPedWithCar* utility) : // NOTSA
        CTaskSimpleCarSetPedInAsDriver{ targetVehicle, utility }
{
    m_bWarpingInToCar = warpingInToCar;
}

CTaskSimpleCarSetPedInAsDriver::~CTaskSimpleCarSetPedInAsDriver() {
    CEntityGta::SafeCleanUpRef(m_pTargetVehicle);
}

// 0x649E00
CTask* CTaskSimpleCarSetPedInAsDriver::Clone() {
    auto task = new CTaskSimpleCarSetPedInAsDriver(m_pTargetVehicle, m_pUtility);
    task->m_bWarpingInToCar = m_bWarpingInToCar;
    task->m_nDoorFlagsToClear = m_nDoorFlagsToClear;
    task->m_nNumGettingInToClear = m_nNumGettingInToClear;
    return task;
}

// 0x64B950
bool CTaskSimpleCarSetPedInAsDriver::ProcessPed(CPed* ped) {
    return CHook::CallFunction<bool>(g_libGTASA + 0x004904AC + 1, this, ped);
}
