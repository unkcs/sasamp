//
// Created by plaka on 18.04.2023.
//

#include "Vehicle.h"
#include "util/patch.h"

void CVehicleGta::SetDriver(CVehicleGta *thiz, CPedGta *pPed)
{
    CPedGta **p_pDriver; // r6
    CPedGta *pDriver; // r0

    p_pDriver = &thiz->pDriver;
    pDriver = thiz->pDriver;

    if ( pDriver )
        CHook::CallFunction<void>(g_libGTASA + 0x003B0D8C + 1, pDriver, p_pDriver);
       // CEntity::CleanUpOldReference(pDriver, p_pDriver);

    thiz->pDriver = pPed;

    CHook::CallFunction<void>(g_libGTASA + 0x003B0E6C + 1, pPed, p_pDriver);
   // CEntity::RegisterReference(pPed, p_pDriver);

//    switch (m_nVehicleType) {
//        case VEHICLE_TYPE_BIKE: {
//            ApplyTurnForce(
//                    GetUp() * passenger->m_fMass / -50.f,
//                    GetForward() / -10.f // Behind the bike
//            );
//            break;
//        }
//        default: {
//            ApplyTurnForce(
//                    CVector{ .0f, .0f, passenger->m_fMass / -5.f },
//                    CVector{ CVector2D{passenger->GetPosition() - GetPosition()}, 0.f }
//            );
//            break;
//        }
//    }
}

void CVehicleGta::InjectHooks() {
    CHook::Redirect(g_libGTASA, 0x0050FECC, &CVehicleGta::SetDriver);
}
