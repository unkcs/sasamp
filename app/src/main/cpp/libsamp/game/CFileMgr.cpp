//
// Created by plaka on 14.04.2023.
//

#include "CFileMgr.h"
#include "main.h"

void CFileMgr::SetDir(const char *path) {
    ( ( void(*)(const char *path) )(g_libGTASA + 0x003F0C54 + 1) )(path);
}
