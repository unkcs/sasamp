
#include "TaskComplexEnterCar.h"

void CTaskComplexEnterCar::InjectHooks() {

}

// 0x63A220
CTaskComplexEnterCar::CTaskComplexEnterCar(CVehicleGta* targetVehicle, bool bAsDriver, bool bQuitAfterOpeningDoor, bool bQuitAfterDraggingPedOut, bool bCarryOnAfterFallingOff) :
        m_bAsDriver{bAsDriver},
        m_bQuitAfterOpeningDoor{bQuitAfterOpeningDoor},
        m_bQuitAfterDraggingPedOut{bQuitAfterDraggingPedOut},
        m_bCarryOnAfterFallingOff{bCarryOnAfterFallingOff},
        m_car{targetVehicle}
{
    CEntityGta::SafeRegisterRef(m_car);
}

// 0x63DFA0
CTaskComplexEnterCar::~CTaskComplexEnterCar() {
    delete m_lineUpUtil;

    CEntityGta::SafeCleanUpRef(m_car);
    if (m_car) {
        m_car->m_nNumGettingIn -= m_numGettingInSet;
        m_car->ClearGettingInFlags(m_doorFlagsSet);
    }
}

// 0x63A730
bool CTaskComplexEnterCar::MakeAbortable(CPedGta* ped, eAbortPriority priority, const CEvent* event) {
    return 1;
}

// 0x63E990
CTask* CTaskComplexEnterCar::CreateNextSubTask(CPedGta* ped) {
    return nullptr;
}

// 0x643A60
CTask* CTaskComplexEnterCar::CreateFirstSubTask(CPedGta* ped) {
    return nullptr;
}

// 0x63A890
CTask* CTaskComplexEnterCar::ControlSubTask(CPedGta* ped) {
    return nullptr;
}

// 0x63E040
CTask* CTaskComplexEnterCar::CreateSubTask(eTaskType type, CPedGta* ped) {
    return nullptr;
}

// 0x63F970
CTask* CTaskComplexEnterCar::CreateNextSubTask_AfterSimpleCarAlign(CPedGta* ped) {
    return nullptr;
}

CVector CTaskComplexEnterCar::GetTargetPos() {
    CVector temp;

    return temp;
}

// 0x63A690
CVehicleGta* CTaskComplexEnterCar::GetCameraAvoidVehicle() {
    if (m_car) {
        if (const auto st = GetSubTask()) {
            switch (st->GetTaskType()) {
                case TASK_COMPLEX_LEAVE_CAR:
                case TASK_SIMPLE_CAR_OPEN_DOOR_FROM_OUTSIDE:
                case TASK_SIMPLE_CAR_OPEN_LOCKED_DOOR_FROM_OUTSIDE:
                case TASK_SIMPLE_BIKE_PICK_UP:
                case TASK_SIMPLE_STAND_STILL:
                case TASK_SIMPLE_CAR_QUICK_DRAG_PED_OUT:
                case TASK_SIMPLE_CAR_SLOW_DRAG_PED_OUT:
                case TASK_SIMPLE_CAR_SET_PED_OUT:
                case TASK_COMPLEX_ENTER_BOAT_AS_DRIVER:
                case TASK_SIMPLE_CAR_DRIVE_TIMED:
                case TASK_SIMPLE_CAR_GET_IN:
                case TASK_SIMPLE_CAR_CLOSE_DOOR_FROM_INSIDE:
                case TASK_SIMPLE_CAR_SHUFFLE:
                case TASK_SIMPLE_CAR_ALIGN:
                case TASK_SIMPLE_CAR_SET_PED_IN_AS_DRIVER:
                case TASK_SIMPLE_CAR_SET_PED_IN_AS_PASSENGER:
                    return m_car;
            }
        }
    }
    return nullptr;
}
