//
// Created by plaka on 20.04.2023.
//
#include "rwcore.h"
#include "rpworld.h"

RwFrame* RwFrameUpdateObjects(RwFrame* frame) {
    return ((RwFrame*(__cdecl *)(RwFrame*))(g_libGTASA + 0x001AEB1C + 1))(frame);
}

RwTexture* RwTextureCreate(RwRaster* raster) {
    return ((RwTexture*(__cdecl *)(RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(raster);
}