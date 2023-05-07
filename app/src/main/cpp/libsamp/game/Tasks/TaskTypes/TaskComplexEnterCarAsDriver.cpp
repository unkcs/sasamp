#include "TaskComplexEnterCarAsDriver.h"
#include "util/patch.h"

// 0x6402F0
CTaskComplexEnterCarAsDriver::CTaskComplexEnterCarAsDriver(CVehicleGta* targetVehicle) :
        CTaskComplexEnterCar(targetVehicle, true, false, false, false)
{
}

CTaskComplexEnterCarAsDriver::CTaskComplexEnterCarAsDriver(CVehicleGta* targetVehicle, eMoveState moveState) : // NOTSA
        CTaskComplexEnterCarAsDriver{ targetVehicle }
{
    m_moveState = moveState;
}

// 0x643780
CTask* CTaskComplexEnterCarAsDriver::Clone() {
    auto task = new CTaskComplexEnterCarAsDriver(m_car);
    task->m_moveState = m_moveState;
    return task;
}

void CTaskComplexEnterCarAsDriver::InjectHooks() {
  //  CHook::Redirect(g_libGTASA + 0x00482E60, &CTaskComplexEnterCarAsDriver);

}
