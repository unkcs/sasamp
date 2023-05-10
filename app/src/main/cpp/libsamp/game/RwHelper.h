//
// Created by plaka on 20.04.2023.
//
#pragma once

RpAtomic* GetFirstAtomicCallback(RpAtomic* atomic, void* data);
RpAtomic* GetFirstAtomic(RpClump* clump);
RpHAnimHierarchy* GetAnimHierarchyFromSkinClump(RpClump* clump);
RpAtomic* SkinAtomicGetHAnimHierarchCB(RpAtomic* atomic, void* data);
RpAtomic* AtomicRemoveAnimFromSkinCB(RpAtomic* atomic, void* data);