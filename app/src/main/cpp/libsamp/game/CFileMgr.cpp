//
// Created by plaka on 14.04.2023.
//

#include "CFileMgr.h"
#include "main.h"

void CFileMgr::SetDir(const char *path) {
    ( ( void(*)(const char *path) )(g_libGTASA + 0x003F0C54 + 1) )(path);
}

FILE* CFileMgr::OpenFile(const char *path, const char *mode) {
    sprintf(ms_path, "%s%s", g_pszStorage, path);

    auto file = fopen(ms_path, mode);

    if(!file) {
        Log("Fail open file %s", ms_path);
    }
    return file;
}

int32_t CFileMgr::CloseFile(FILE* file) {
    return fclose(file);
}
