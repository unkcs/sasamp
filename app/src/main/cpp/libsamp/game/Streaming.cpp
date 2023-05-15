//
// Created by plaka on 11.01.2023.
//

#include "Streaming.h"
#include "../util/patch.h"
#include "StreamingInfo.h"
#include "game/Models/ModelInfo.h"
#include <algorithm>
#include <functional>
#include <vector>
#include <iostream>

void CStreaming::RemoveModel(int32 modelId) {
    CHook::CallFunction<void>(g_libGTASA + 0x00290C4C + 1, modelId);
}

//void CStreaming::RemoveAllUnusedModels()
//{
//
//}

// Remove majority of loaded models
void CStreaming::RemoveAllUnusedModels() {
  //  return  ((void (*) ())(g_libGTASA + 0x293325))();

    // Remove all possibly removable vehicles
//    for (int32 i = 0; i < MAX_VEHICLES_LOADED; i++) {
//        RemoveLoadedVehicle();
//    }

    // Remove majority of models with no refs
    for (int32 modelId = 1000; IsModelDFF(modelId); modelId++) {
        CStreamingInfo& streamingInfo = GetInfo(modelId);
        if (streamingInfo.IsLoaded() && !CModelInfo::GetModelInfo(modelId)->m_nRefCount) {
            RemoveModel(modelId);
            streamingInfo.ClearAllFlags();
        }
    }
}

void CStreaming::InjectHooks() {
    CHook::Write(g_libGTASA + 0x005D1EA8, &CStreaming::ms_memoryUsed);
    CHook::Write(g_libGTASA + 0x005D111C, &CStreaming::ms_memoryAvailable);
    CHook::Write(g_libGTASA + 0x005D1508, &CStreaming::desiredNumVehiclesLoaded);
    CHook::Write(g_libGTASA + 0x005CE80C, &CStreaming::ms_files);

    CHook::Write(g_libGTASA + 0x005CF32C, &CStreaming::ms_rwObjectInstances);
    CHook::Write(g_libGTASA + 0x005CFC04, &CStreaming::ms_aInfoForModel);

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

void CStreaming::RequestModel(int32_t index, int32_t flags)
{
    ((void (*) (int32_t, int32_t))(g_libGTASA + 0x0028EB10 + 1))(index, flags);
}

void CStreaming::LoadAllRequestedModels(bool bPriorityRequestsOnly)
{
    ((void (*) (bool))(g_libGTASA + 0x00294CB4 + 1))(bPriorityRequestsOnly);
}

void CStreaming::RemoveBuildingsNotInArea(int areaCode) {
    ((void (*) (int))(g_libGTASA + 0x0028FBAC + 1))(areaCode);
}
