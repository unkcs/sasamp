//
// Created by plaka on 18.04.2023.
//

#include "Vehicle.h"
#include "util/patch.h"
#include "game/World.h"

void CVehicleGta::SetDriver(CPedGta* driver) {
    CEntityGta::ChangeEntityReference(pDriver, driver);

  //  ApplyTurnForceToOccupantOnEntry(driver);
}


// ----------------------------------- hooks

void SetDriver_hook(CVehicleGta *thiz, CPedGta *pPed)
{
    thiz->SetDriver(pPed);
}

void CVehicleGta::InjectHooks() {
    CHook::Redirect(g_libGTASA, 0x0050FECC, &SetDriver_hook);
}
