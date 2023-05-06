//
// Created by plaka on 18.04.2023.
//
#include "../common.h"
#include "Vehicle.h"
#include "util/patch.h"
#include "game/World.h"

void CVehicleGta::SetDriver(CPedGta* driver) {
    CEntityGta::ChangeEntityReference(pDriver, driver);

    ApplyTurnForceToOccupantOnEntry(driver);
}

void CVehicleGta::ApplyTurnForceToOccupantOnEntry(CPedGta* passenger) {
    // Apply some turn force
    switch (m_nVehicleType) {
        case VEHICLE_TYPE_BIKE: {
            ApplyTurnForce(
                    GetUp() * passenger->m_fMass / -50.f,
                    GetForward() / -10.f // Behind the bike
            );
            break;
        }
        default: {
            ApplyTurnForce(
                    CVector{ .0f, .0f, passenger->m_fMass / -5.f },
                    CVector{ CVector2D{passenger->GetPosition() - GetPosition()}, 0.f }
            );
            break;
        }
    }
}

int CVehicleGta::GetPassengerIndex(const CPedGta* passenger) {
    for(int i = 0; i <  std::size(m_apPassengers); i++) {
        if(passenger == m_apPassengers[i])
            return i;
    }
    return -1;
}

// ----------------------------------- hooks

void SetDriver_hook(CVehicleGta *thiz, CPedGta *pPed)
{
    thiz->SetDriver(pPed);
}

void CVehicleGta::InjectHooks() {
    // var
    CHook::Write(g_libGTASA + 0x005CDBF4, &CVehicleGta::m_aSpecialColModel);

    CHook::Redirect(g_libGTASA, 0x0050FECC, &SetDriver_hook);
}
