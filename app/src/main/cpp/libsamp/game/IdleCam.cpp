//
// Created by plaka on 01.05.2023.
//

#include "IdleCam.h"
#include "util/patch.h"

CIdleCam gIdleCam;

void CIdleCam::InjectHooks() {
    CHook::Write(g_libGTASA + 0x005D001C, &gIdleCam);
}
