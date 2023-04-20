//
// Created by plaka on 20.04.2023.
//

#include "RwHelper.h"
#include "game/RW/rphanim.h"
#include "game/RW/rpskin.h"

RpAtomic* GetFirstAtomic(RpClump* clump) {
    RpAtomic* atomic{};
    RpClumpForAllAtomics(clump, GetFirstAtomicCallback, &atomic);
    return atomic;
}

RpAtomic* GetFirstAtomicCallback(RpAtomic* atomic, void* data) {
    *(RpAtomic**)(data) = atomic;
    return nullptr;
}

RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* clump) {
    RpHAnimHierarchy* anim{};
    RpClumpForAllAtomics(clump, SkinAtomicGetHAnimHierarchCB, &anim);
    return anim;
}

// name not from Android
RpAtomic* SkinAtomicGetHAnimHierarchCB(RpAtomic* atomic, void* data) {
    *(RpHAnimHierarchy**)(data) = RpSkinAtomicGetHAnimHierarchy(atomic);
    return nullptr;
}