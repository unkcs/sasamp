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