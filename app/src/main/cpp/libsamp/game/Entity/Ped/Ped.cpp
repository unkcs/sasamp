//
// Created by plaka on 05.05.2023.
//

#include "Ped.h"
#include "../../../util/patch.h"
#include "game/Plugins/RpAnimBlendPlugin/RpAnimBlend.h"

void CPedGta::GetBonePosition(RwV3d *posn, uint32 boneTag, bool bCalledFromCamera) {
    CHook::CallFunction<void>(g_libGTASA + 0x00436590 + 1, this, posn, boneTag, bCalledFromCamera);
}

void CPedGta::StopNonPartialAnims() {
    for (auto assoc = RpAnimBlendClumpGetFirstAssociation(m_pRwClump); assoc; assoc = RpAnimBlendGetNextAssociation(assoc)) {
        if ((assoc->m_nFlags & ANIMATION_PARTIAL) == 0) {
            assoc->SetFlag(ANIMATION_STARTED, false);
        }
    }
}