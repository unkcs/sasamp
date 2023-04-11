//
// Created by plaka on 11.04.2023.
//

#include "rpworld.h"

RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* data) {
    return ((RpClump*(__cdecl *)(RpClump*, RpAtomicCallBack, void*))((g_libGTASA + 0x001E0EA0 + 1)))(clump, callback, data);
}

RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack, void* data) {
    return ((RpGeometry*(__cdecl *)(RpGeometry*, RpMaterialCallBack, void*))(g_libGTASA + 0x001E284C + 1))(geometry, fpCallBack, data);
}