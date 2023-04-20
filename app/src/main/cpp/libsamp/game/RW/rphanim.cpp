
#include "rphanim.h"

void RpHAnimHierarchySetFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc) {
    ((void(__cdecl *)(RwInt32, RwInt32))(g_libGTASA + 0x00199DD4 + 1))(blockSize, numBlocksToPrealloc);
}

RpHAnimHierarchy* RpHAnimHierarchyCreate(RwInt32 numNodes, RwUInt32* nodeFlags, RwInt32* nodeIDs, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return ((RpHAnimHierarchy*(__cdecl *)(RwInt32, RwUInt32*, RwInt32*, RpHAnimHierarchyFlag, RwInt32))(g_libGTASA + 0x00199E78 + 1))(numNodes, nodeFlags, nodeIDs, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyCreateFromHierarchy(RpHAnimHierarchy* hierarchy, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return ((RpHAnimHierarchy*(__cdecl *)(RpHAnimHierarchy*, RpHAnimHierarchyFlag, RwInt32))(g_libGTASA + 0x0019A238 + 1))(hierarchy, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyDestroy(RpHAnimHierarchy* hierarchy) {
    return ((RpHAnimHierarchy*(__cdecl *)(RpHAnimHierarchy*))(g_libGTASA + 0x0019A05C + 1))(hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyCreateSubHierarchy(RpHAnimHierarchy* parentHierarchy, RwInt32 startNode, RpHAnimHierarchyFlag flags, RwInt32 maxInterpKeyFrameSize) {
    return ((RpHAnimHierarchy*(__cdecl *)(RpHAnimHierarchy*, RwInt32, RpHAnimHierarchyFlag, RwInt32))(g_libGTASA + 0x0019A13C + 1))(parentHierarchy, startNode, flags, maxInterpKeyFrameSize);
}

RpHAnimHierarchy* RpHAnimHierarchyAttach(RpHAnimHierarchy* hierarchy) {
    return ((RpHAnimHierarchy*(__cdecl *)(RpHAnimHierarchy*))(g_libGTASA + 0x0019A3A8+ 1))(hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyDetach(RpHAnimHierarchy* hierarchy) {
    return ((RpHAnimHierarchy*(__cdecl *)(RpHAnimHierarchy*))(g_libGTASA + 0x0019A290 + 1))(hierarchy);
}

RpHAnimHierarchy* RpHAnimHierarchyAttachFrameIndex(RpHAnimHierarchy* hierarchy, RwInt32 nodeIndex) {
    return ((RpHAnimHierarchy*(__cdecl *)(RpHAnimHierarchy*, RwInt32))(g_libGTASA + 0x0019A420 + 1))(hierarchy, nodeIndex);
}

RpHAnimHierarchy* RpHAnimHierarchyDetachFrameIndex(RpHAnimHierarchy* hierarchy, RwInt32 nodeIndex) {
    return ((RpHAnimHierarchy*(__cdecl *)(RpHAnimHierarchy*, RwInt32))(g_libGTASA + 0x0019A2B8 + 1))(hierarchy, nodeIndex);
}

RwBool RpHAnimFrameSetHierarchy(RwFrame* frame, RpHAnimHierarchy* hierarchy) {
    return ((RwBool(__cdecl *)(RwFrame*, RpHAnimHierarchy*))(g_libGTASA + 0x0019A2E0 + 1))(frame, hierarchy);
}

RpHAnimHierarchy* RpHAnimFrameGetHierarchy(RwFrame* frame) {
    return ((RpHAnimHierarchy*(__cdecl *)(RwFrame*))(g_libGTASA + 0x0019A308 + 1))(frame);
}

RwMatrix* RpHAnimHierarchyGetMatrixArray(RpHAnimHierarchy* hierarchy) {
    return ((RwMatrix*(__cdecl *)(RpHAnimHierarchy*))(g_libGTASA + 0x0019A2D0 + 1))(hierarchy);
}

RwBool RpHAnimHierarchyUpdateMatrices(RpHAnimHierarchy* hierarchy) {
    return ((RwBool(__cdecl *)(RpHAnimHierarchy*))(g_libGTASA + 0x0019A47C + 1))(hierarchy);
}

RwInt32 RpHAnimIDGetIndex(RpHAnimHierarchy* hierarchy, RwInt32 ID) {
    return ((RwInt32(__cdecl *)(RpHAnimHierarchy*, RwInt32))(g_libGTASA + 0x0019A448))(hierarchy, ID);
}

RwBool RpHAnimPluginAttach() {
    return ((RwBool(__cdecl *)(void))(g_libGTASA + 0x00199DEC + 1))();
}

void RpHAnimKeyFrameApply(void* matrix, void* voidIFrame) {
    ((void(__cdecl *)(void*, void*))(g_libGTASA + 0x0019AC88 + 1))(matrix, voidIFrame);
}

void RpHAnimKeyFrameBlend(void* voidOut, void* voidIn1, void* voidIn2, RwReal alpha) {
    ((void(__cdecl *)(void*, void*, void*, RwReal))(g_libGTASA + 0x0019B234 + 1))(voidOut, voidIn1, voidIn2, alpha);
}

void RpHAnimKeyFrameInterpolate(void* voidOut, void* voidIn1, void* voidIn2, RwReal time, void* customData) {
    ((void(__cdecl *)(void*, void*, void*, RwReal, void*))(g_libGTASA + 0x0019AD5C + 1))(voidOut, voidIn1, voidIn2, time, customData);
}

void RpHAnimKeyFrameAdd(void* voidOut, void* voidIn1, void* voidIn2) {
    ((void(__cdecl *)(void*, void*, void*))(g_libGTASA + 0x0019B8D8 + 1))(voidOut, voidIn1, voidIn2);
}

void RpHAnimKeyFrameMulRecip(void* voidFrame, void* voidStart) {
    ((void(__cdecl *)(void*, void*))(g_libGTASA + 0x0019B7EC + 1))(voidFrame, voidStart);
}

RtAnimAnimation* RpHAnimKeyFrameStreamRead(RwStream* stream, RtAnimAnimation* animation) {
    return ((RtAnimAnimation*(__cdecl *)(RwStream*, RtAnimAnimation*))(g_libGTASA + 0x0019B6F8 + 1))(stream, animation);
}

RwBool RpHAnimKeyFrameStreamWrite(const RtAnimAnimation* animation, RwStream* stream) {
    return ((RwBool(__cdecl *)(const RtAnimAnimation*, RwStream*))(g_libGTASA + 0x0019B770 + 1))(animation, stream);
}

RwInt32 RpHAnimKeyFrameStreamGetSize(const RtAnimAnimation* animation) {
    return ((RwInt32(__cdecl *)(const RtAnimAnimation*))(g_libGTASA + 0x0019B7D8 + 1))(animation);
}

RwBool RpHAnimFrameSetID(RwFrame* frame, RwInt32 id) {
    return ((RwBool(__cdecl *)(RwFrame*, RwInt32))(g_libGTASA + 0x0019A324 + 1))(frame, id);
}

RwInt32 RpHAnimFrameGetID(RwFrame* frame) {
    return ((RwInt32(__cdecl *)(RwFrame*))(g_libGTASA + 0x0019A340 + 1))(frame);
}
