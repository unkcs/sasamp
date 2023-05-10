//
// Created by plaka on 20.04.2023.
//

#include "RenderWare.h"

RpSkin* RpSkinGeometryGetSkin(RpGeometry* geometry) {
    return ((RpSkin*(__cdecl *)(RpGeometry*))(g_libGTASA + 0x001A0000 + 1))(geometry);
}

RpHAnimHierarchy* RpSkinAtomicGetHAnimHierarchy(const RpAtomic* atomic) {
    return ((RpHAnimHierarchy*(__cdecl *)(const RpAtomic*))(g_libGTASA + 0x0019FFE4 + 1))(atomic);
}
