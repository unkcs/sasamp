//
// Created by plaka on 02.05.2023.
//

#include "Sprite.h"
#include "../../util/patch.h"

bool
Sprite::CalcScreenCoors(const RwV3d *posn, RwV3d *out, float *w, float *h, bool checkMaxVisible,
                        bool checkMinVisible) {
    return CHook::CallFunction<bool>(g_libGTASA + 0x54EEC0 + 1, posn, out, w, h, checkMaxVisible, checkMinVisible);
}
