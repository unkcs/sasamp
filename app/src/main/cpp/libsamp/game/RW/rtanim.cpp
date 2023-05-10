#include "RenderWare.h"

void RtAnimAnimationFreeListCreateParams(RwInt32 blockSize, RwInt32 numBlocksToPrealloc) {
    ((void(__cdecl *)(RwInt32, RwInt32))(g_libGTASA + 0x001BF704 + 1))(blockSize, numBlocksToPrealloc);
}

RwBool RtAnimInitialize() {
    return ((RwBool(__cdecl *)(void))(g_libGTASA + 0x001BF71C + 1))();
}

RwBool RtAnimRegisterInterpolationScheme(RtAnimInterpolatorInfo* interpolatorInfo) {
    return ((RwBool(__cdecl *)(RtAnimInterpolatorInfo*))(g_libGTASA + 0x001BF744 + 1))(interpolatorInfo);
}

RtAnimInterpolatorInfo* RtAnimGetInterpolatorInfo(RwInt32 typeID) {
    return ((RtAnimInterpolatorInfo*(__cdecl *)(RwInt32))(g_libGTASA + 0x001BF850 + 1))(typeID);
}

RtAnimAnimation* RtAnimAnimationCreate(RwInt32 typeID, RwInt32 numFrames, RwInt32 flags, RwReal duration) {
    return ((RtAnimAnimation*(__cdecl *)(RwInt32, RwInt32, RwInt32, RwReal))(g_libGTASA + 0x001BF97C + 1))(typeID, numFrames, flags, duration);
}

RwBool RtAnimAnimationDestroy(RtAnimAnimation* animation) {
    return ((RwBool(__cdecl *)(RtAnimAnimation*))(g_libGTASA + 0x001BF9E0 + 1))(animation);
}

RtAnimAnimation* RtAnimAnimationRead(const RwChar* filename) {
    return ((RtAnimAnimation*(__cdecl *)(const RwChar*))(g_libGTASA + 0x001BFA80 + 1))(filename);
}

RwBool RtAnimAnimationWrite(const RtAnimAnimation* animation, const RwChar* filename) {
    return ((RwBool(__cdecl *)(const RtAnimAnimation*, const RwChar*))(g_libGTASA + 0x001BFB58 + 1))(animation, filename);
}

RtAnimAnimation* RtAnimAnimationStreamRead(RwStream* stream) {
    return ((RtAnimAnimation*(__cdecl *)(RwStream*))(g_libGTASA + 0x001BF9FC + 1))(stream);
}

RwBool RtAnimAnimationStreamWrite(const RtAnimAnimation* animation, RwStream* stream) {
    return ((RwBool(__cdecl *)(const RtAnimAnimation*, RwStream*))(g_libGTASA + 0x001BFAC8 + 1))(animation, stream);
}

RwInt32 RtAnimAnimationStreamGetSize(const RtAnimAnimation* animation) {
    return ((RwInt32(__cdecl *)(const RtAnimAnimation*))(g_libGTASA + 0x001BFAB8 + 1))(animation);
}

RwUInt32 RtAnimAnimationGetNumNodes(const RtAnimAnimation* animation) {
    return ((RwUInt32(__cdecl *)(const RtAnimAnimation*))(g_libGTASA + 0x001BFB84 + 1))(animation);
}

RtAnimInterpolator* RtAnimInterpolatorCreate(RwInt32 numNodes, RwInt32 maxInterpKeyFrameSize) {
    return ((RtAnimInterpolator*(__cdecl *)(RwInt32, RwInt32))(g_libGTASA + 0x001BFBAC + 1))(numNodes, maxInterpKeyFrameSize);
}

void RtAnimInterpolatorDestroy(RtAnimInterpolator* anim) {
    ((void(__cdecl *)(RtAnimInterpolator*))(g_libGTASA + 0x001BFC04 + 1))(anim);
}

RwBool RtAnimInterpolatorSetCurrentAnim(RtAnimInterpolator* animI, RtAnimAnimation* anim) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RtAnimAnimation*))(g_libGTASA + 0x001BFC20 + 1))(animI, anim);
}

RwBool RtAnimInterpolatorSetKeyFrameCallBacks(RtAnimInterpolator* anim, RwInt32 keyFrameTypeID) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RwInt32))(g_libGTASA + 0x001BFCDC + 1))(anim, keyFrameTypeID);
}

void RtAnimInterpolatorSetAnimLoopCallBack(RtAnimInterpolator* anim, RtAnimCallBack callBack, void* data) {
    ((void(__cdecl *)(RtAnimInterpolator*, RtAnimCallBack, void*))(g_libGTASA + 0x001BFD04 + 1))(anim, callBack, data);
}

void RtAnimInterpolatorSetAnimCallBack(RtAnimInterpolator* anim, RtAnimCallBack callBack, RwReal time, void* data) {
    ((void(__cdecl *)(RtAnimInterpolator*, RtAnimCallBack, RwReal, void*))(g_libGTASA + 0x001BFD14 + 1))(anim, callBack, time, data);
}

RwBool RtAnimInterpolatorCopy(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RtAnimInterpolator*))(g_libGTASA + 0x001BFD28 + 1))(outAnim, inAnim);
}

RwBool RtAnimInterpolatorSubAnimTime(RtAnimInterpolator* anim, RwReal time) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RwReal))(g_libGTASA + 0x001BFED0 + 1))(anim, time);
}

RwBool RtAnimInterpolatorAddAnimTime(RtAnimInterpolator* anim, RwReal time) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RwReal))(g_libGTASA + 0x001BFD44 + 1))(anim, time);
}

RwBool RtAnimInterpolatorSetCurrentTime(RtAnimInterpolator* anim, RwReal time) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RwReal))(g_libGTASA + 0x001C0018 + 1))(anim, time);
}

RwBool RtAnimAnimationMakeDelta(RtAnimAnimation* animation, RwInt32 numNodes, RwReal time) {
    return ((RwBool(__cdecl *)(RtAnimAnimation*, RwInt32, RwReal))(g_libGTASA + 0x001C0050 + 1))(animation, numNodes, time);
}

RwBool RtAnimInterpolatorBlend(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim1, RtAnimInterpolator* inAnim2, RwReal alpha) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RtAnimInterpolator*, RtAnimInterpolator*, RwReal))(g_libGTASA + 0x001C00E8 + 1))(outAnim, inAnim1, inAnim2, alpha);
}

RwBool RtAnimInterpolatorAddTogether(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim1, RtAnimInterpolator* inAnim2) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RtAnimInterpolator*, RtAnimInterpolator*))(g_libGTASA + 0x001C0130 + 1))(outAnim, inAnim1, inAnim2);
}

RtAnimInterpolator* RtAnimInterpolatorCreateSubInterpolator(RtAnimInterpolator* parentAnim, RwInt32 startNode, RwInt32 numNodes, RwInt32 maxInterpKeyFrameSize) {
    return ((RtAnimInterpolator*(__cdecl *)(RtAnimInterpolator*, RwInt32, RwInt32, RwInt32))(g_libGTASA + 0x001C0174 + 1))(parentAnim, startNode, numNodes, maxInterpKeyFrameSize);
}

RwBool RtAnimInterpolatorBlendSubInterpolator(RtAnimInterpolator* outAnim, RtAnimInterpolator* inAnim1, RtAnimInterpolator* inAnim2, RwReal alpha) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RtAnimInterpolator*, RtAnimInterpolator*, RwReal))(g_libGTASA + 0x001C0194 + 1))(outAnim, inAnim1, inAnim2, alpha);
}

RwBool RtAnimInterpolatorAddSubInterpolator(RtAnimInterpolator* outAnim, RtAnimInterpolator* mainAnim, RtAnimInterpolator* subAnim) {
    return ((RwBool(__cdecl *)(RtAnimInterpolator*, RtAnimInterpolator*, RtAnimInterpolator*))(g_libGTASA + 0x001C0314 + 1))(outAnim, mainAnim, subAnim);
}
