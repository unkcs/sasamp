//
// Created by plaka on 13.05.2023.
//

#include "World.h"
#include "../util/patch.h"

CRepeatSector* GetRepeatSector(int32 x, int32 y) {
    return &CWorld::ms_aRepeatSectors[y % MAX_REPEAT_SECTORS_Y][x % MAX_REPEAT_SECTORS_X];
}

void CWorld::InjectHooks() {
    CHook::Write(g_libGTASA + 0x005D0D8C, &CWorld::ms_listMovingEntityPtrs);
    CHook::Write(g_libGTASA + 0x005CEF64, &CWorld::ms_aRepeatSectors);
}