//
// Created by plaka on 05.05.2023.
//

#include "Ped.h"
#include "../../../util/patch.h"

void CPedGta::GetBonePosition(RwV3d *posn, uint32 boneTag, bool bCalledFromCamera) {
    CHook::CallFunction<void>(g_libGTASA + 0x00436590 + 1, this, posn, boneTag, bCalledFromCamera);
}
