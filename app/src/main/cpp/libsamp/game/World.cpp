//
// Created by plaka on 13.05.2023.
//

#include "World.h"
#include "../util/patch.h"

//CPtrListDoubleLink CWorld::ms_listMovingEntityPtrs;


void CWorld::InjectHooks() {
   // SET_TO(CWorld::ms_listMovingEntityPtrs, 0x008CAA0C);
   // CHook::Write(g_libGTASA + 0x005D0D8C, &CWorld::ms_listMovingEntityPtrs);
 //   CHook::Write(g_libGTASA + 0x005CEF64, &CWorld::ms_aRepeatSectors);
}