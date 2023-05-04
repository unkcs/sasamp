//
// Created by plaka on 11.01.2023.
//

#include "Streaming.h"
#include "../util/patch.h"
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

void CStreaming::RemoveModel(int32 modelId) {
    CHook::CallFunction<void>(g_libGTASA + 0x00290C4C + 1, modelId);
}

void CStreaming::RemoveAllUnusedModels()
{
    ((void (*) ())(g_libGTASA + 0x293325))();
}

void CStreaming::InjectHooks() {
    CHook::Write(g_libGTASA + 0x005D111C, &CStreaming::ms_memoryAvailable);
    CHook::Write(g_libGTASA + 0x005D1508, &CStreaming::desiredNumVehiclesLoaded);
    CHook::Write(g_libGTASA + 0x005CE80C, &CStreaming::ms_files);
    CHook::Write(g_libGTASA + 0x005CF32C, &CStreaming::ms_rwObjectInstances);

    CHook::Redirect(g_libGTASA, 0x28E83C, &CStreaming::InitImageList);
}

int CStreaming::AddImageToList(char const* pFileName, bool bNotPlayerImg) {
    // find a free slot
    std::int32_t fileIndex = 0;
    for (; fileIndex < TOTAL_IMG_ARCHIVES; fileIndex++) {
        if (!ms_files[fileIndex].m_szName[0])
            break;
    }
    if (fileIndex == TOTAL_IMG_ARCHIVES)
        return 0;
    // free slot found, load the IMG file
    strcpy(ms_files[fileIndex].m_szName, pFileName);
    ms_files[fileIndex].m_StreamHandle = CdStreamOpen(pFileName);
    ms_files[fileIndex].m_bNotPlayerImg = bNotPlayerImg;
    return fileIndex;
}

void CStreaming::InitImageList() {
    for (auto & ms_file : ms_files) {
        ms_file.m_szName[0] = 0;
        ms_file.m_StreamHandle = 0;
    }

    CStreaming::AddImageToList("TEXDB\\GTA3.IMG", true);
    CStreaming::AddImageToList("TEXDB\\SKINS.IMG", true);
    CStreaming::AddImageToList("TEXDB\\CARS.IMG", true);
    CStreaming::AddImageToList("TEXDB\\SAMP.IMG", true);
    CStreaming::AddImageToList("TEXDB\\SAMPCOL.IMG", true);
}
