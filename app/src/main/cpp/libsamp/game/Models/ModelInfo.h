//
// Created by plaka on 07.03.2023.
//

#ifndef LIVERUSSIA_MODELINFO_H
#define LIVERUSSIA_MODELINFO_H


#include "PedModelInfo.h"
#include "AtomicModelInfo.h"
#include "VehicleModelInfo.h"
#include "game/Core/CStore.h"

class CModelInfo {
public:

    static CBaseModelInfo* ms_modelInfoPtrs[30000];

    static void injectHooks();

    static CStore<CPedModelInfo, 350> ms_pedModelInfoStore;
    static CStore<CAtomicModelInfo, 20000> ms_atomicModelInfoStore;
    static CStore<CVehicleModelInfo, 300> ms_vehicleModelInfoStore;

    static CPedModelInfo *AddPedModel(int index);

    static CAtomicModelInfo *AddAtomicModel(int index);

    static CVehicleModelInfo *AddVehicleModel(int index);

    static CBaseModelInfo* GetModelInfo(int index) { return ms_modelInfoPtrs[index]; }
    static void SetModelInfo(int index, CBaseModelInfo* pInfo) { ms_modelInfoPtrs[index] = pInfo; }
};


#endif //LIVERUSSIA_MODELINFO_H
