//
// Created by plaka on 20.04.2023.
//
#include "ClumpModelInfo.h"
#include "util/patch.h"

RwFrame* CClumpModelInfo::GetFrameFromName(RpClump* clump, const char* name)
{
    return CHook::CallFunction<RwFrame*>(g_libGTASA + 0x00335CEC + 1, clump, name);
//    auto searchInfo = tCompSearchStructByName(name, nullptr);
//    RwFrameForAllChildren(RpClumpGetFrame(clump), FindFrameFromNameCB, &searchInfo);
//    return searchInfo.m_pFrame;
}