//
// Created by plaka on 20.04.2023.
//
#include "Entity.h"
#include "../common.h"
#include "../RW/rwcore.h"
#include "../RwHelper.h"
#include "game/RW/rpskin.h"
#include "util/patch.h"
#include "game/Models/ModelInfo.h"

void CEntityGta::UpdateRwFrame()
{
    if (!m_pRwObject)
        return;

    RwFrameUpdateObjects(static_cast<RwFrame*>(rwObjectGetParent(m_pRwObject)));
}

void CEntityGta::UpdateRpHAnim() {
    if (const auto atomic = GetFirstAtomic(m_pRwClump)) {
        if (RpSkinGeometryGetSkin(RpAtomicGetGeometry(atomic)) && !nEntityFlags.m_bDontUpdateHierarchy) {
            RpHAnimHierarchyUpdateMatrices(GetAnimHierarchyFromSkinClump(m_pRwClump));
        }
    }
}







// ------------- hooks


void CEntityGta::InjectHooks() {

}