
#include "TaskUtilityLineUpPedWithCar.h"
#include "util/patch.h"

void CTaskUtilityLineUpPedWithCar::InjectHooks() {
}

// 0x64FBB0
CTaskUtilityLineUpPedWithCar::CTaskUtilityLineUpPedWithCar(const CVector& offset, int32 time, int32 doorOpenPosType, int32 doorIdx) {
    m_Offset = offset;
    m_fDoorOpenPosZ = -999.99f;
    m_fTime = time;
    m_nDoorOpenPosType = doorOpenPosType;
    m_nDoorIdx = doorIdx;
}

// The following 2 functions seem to have copy elision on the returned CVector, that the compiled functions
// took a vector ptr as their first arg. Now, hopefully our code will compile to the same stuff.
// If not, it might crash here, in that case a wrapper function should be used.

// 0x64FC10
CVector CTaskUtilityLineUpPedWithCar::GetLocalPositionToOpenCarDoor(CVehicleGta* vehicle, float animProgress, CAnimBlendAssociation* assoc) {
    CVector out;
    CHook::CallFunction<CVector>(g_libGTASA + 0x004937C0 + 1, this, &out, vehicle, animProgress, assoc);
    return out;
}

// 0x650A80
CVector CTaskUtilityLineUpPedWithCar::GetPositionToOpenCarDoor(CVehicleGta* vehicle, float animProgress, CAnimBlendAssociation* assoc) {
    CVector out;
    CHook::CallFunction<CVector>(g_libGTASA + 0x00493BFC + 1, this, &out, vehicle, animProgress, assoc);
    return out;
}

// 0x6513A0
bool CTaskUtilityLineUpPedWithCar::ProcessPed(CPedGta* ped, CVehicleGta* vehicle, CAnimBlendAssociation* assoc) {
    return CHook::CallFunction<bool>(g_libGTASA + 0x00493C90 + 1, this, ped, vehicle, assoc);
}
