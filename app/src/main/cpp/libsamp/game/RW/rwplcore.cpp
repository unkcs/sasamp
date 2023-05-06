//
// Created by plaka on 19.04.2023.
//
#include "rwplcore.h"
#include "../../main.h"

RwMatrix* RwMatrixUpdate(RwMatrix* matrix) {
    return ((RwMatrix*(__cdecl *)(RwMatrix*))(g_libGTASA + 0x001B8C7C + 1))(matrix);
}

RwBool RwMatrixDestroy(RwMatrix* mpMat) {
    return ((RwBool(__cdecl *)(RwMatrix*))(g_libGTASA + 0x001B9980 + 1))(mpMat);
}

RwUInt32 RwStreamRead(RwStream* stream, void* buffer, RwUInt32 length) {
    return ((RwUInt32(__cdecl *)(RwStream*, void*, RwUInt32))(g_libGTASA + 0x001BAA50 + 1))(stream, buffer, length);
}

RwStream* RwStreamOpen(RwStreamType type, RwStreamAccessType accessType, const void* data) {
    return ((RwStream*(__cdecl *)(RwStreamType, RwStreamAccessType, const void*))(g_libGTASA + 0x001BADA4 + 1))(type, accessType, data);
}

RwBool RwStreamClose(RwStream* stream, void* data) {
    return ((RwBool(__cdecl *)(RwStream*, void*))(g_libGTASA + 0x001BACF4 + 1))(stream, data);
}