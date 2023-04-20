//
// Created by plaka on 20.04.2023.
//

#include "Placeable.h"
#include "game/Core/MatrixLinkList.h"
#include "util/patch.h"

void CPlaceable::InjectHooks() {
    CHook::Redirect(g_libGTASA, 0x3ABB08, &CPlaceable::InitMatrixArray);
}

void CPlaceable::InitMatrixArray() {
    gMatrixList.Init(CPlaceable::NUM_MATRICES_TO_CREATE);
}