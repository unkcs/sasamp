#include "RpAnimBlend.h"
#include "../../../util/patch.h"

// 0x4D6150
bool RpAnimBlendPluginAttach() {
    return CHook::CallFunction<bool>(g_libGTASA + 0x0033FF8C + 1);
}

// 0x4D5FA0
void* RtAnimBlendKeyFrameApply(void* result, void* frame) {
    return CHook::CallFunction<void*>(g_libGTASA + 0x0033FE24 + 1, result, frame);

    auto matrix = static_cast<RwMatrix*>(result);
    auto quat = static_cast<RtQuat*>(frame);
}

// 0x4D5F50
CAnimBlendClumpData* RpAnimBlendAllocateData(RpClump* clump) {
    return CHook::CallFunction<CAnimBlendClumpData*>(g_libGTASA + 0x0033FF68 + 1, clump);
}

// 0x4D6790
CAnimBlendAssociation* RpAnimBlendClumpAddAssociation(RpClump* clump, CAnimBlendAssociation* association, uint32 flags, float startTime, float blendAmount) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x0034049C + 1, clump, association, flags, startTime, blendAmount);
}

// 0x4D6BE0
CAnimBlendAssociation* RpAnimBlendClumpExtractAssociations(RpClump* clump) {
    int *v1; // r1
    int v2; // r3
    int result; // r0

    v1 = *(int **)(clump + ClumpOffset);
    v2 = *v1;
    *v1 = 0;
    result = v2 - 4;
    *(uint32_t *)(v2 + 4) = 0;
    return reinterpret_cast<CAnimBlendAssociation *>(result);
}

// 0x4D64A0
void RpAnimBlendClumpFillFrameArray(RpClump* clump, AnimBlendFrameData** frameData) {
    CHook::CallFunction<void>(g_libGTASA + 0x00340364 + 1, clump, frameData);
}

// 0x4D6400
AnimBlendFrameData* RpAnimBlendClumpFindBone(RpClump* clump, uint32 id) {
    return CHook::CallFunction<AnimBlendFrameData*>(g_libGTASA + 0x00340334 + 1, clump, id);
}

// 0x4D62A0
AnimBlendFrameData* RpAnimBlendClumpFindFrame(RpClump* clump, const char* name) {
    return CHook::CallFunction<AnimBlendFrameData*>(g_libGTASA + 0x0034027C + 1, clump, name);
}

// 0x4D6370
AnimBlendFrameData* RpAnimBlendClumpFindFrameFromHashKey(RpClump* clump, uint32 key) {
    return CHook::CallFunction<AnimBlendFrameData*>(g_libGTASA + 0x003402D8 + 1, clump, key);
}

// 0x4D68E0
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, bool bStopFunctionConfusion, CAnimBlendHierarchy* hierarchy) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x003405F8 + 1, clump, bStopFunctionConfusion, hierarchy);
}

// 0x4D6870
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, const char* name) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x00340594 + 1, clump, name);
}

// AnimationId animId
// 0x4D68B0
CAnimBlendAssociation* RpAnimBlendClumpGetAssociation(RpClump* clump, uint32 animId) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x003405C8 + 1, clump, animId);
}

// 0x4D15E0
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump* clump) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x0033D970 + 1, clump);
}

// 0x4D6A70
CAnimBlendAssociation* RpAnimBlendClumpGetFirstAssociation(RpClump* clump, uint32 flags) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x0034076C + 1, clump, flags);
}

// 0x4D6910
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation(RpClump* clump, CAnimBlendAssociation** pp2ndAnim, float* pBlendVal2nd) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x00340624 + 1, clump, pp2ndAnim, pBlendVal2nd);
}

// 0x4D6A30
CAnimBlendAssociation* RpAnimBlendClumpGetMainAssociation_N(RpClump* clump, int32 n) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x003406EC + 1, clump, n);
}

// 0x4D69A0
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation(RpClump* clump) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x003406A0 + 1, clump);
}

// 0x4D69F0
CAnimBlendAssociation* RpAnimBlendClumpGetMainPartialAssociation_N(RpClump* clump, int32 n) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x0034072C + 1, clump, n);
}

// 0x4D6B60
uint32 RpAnimBlendClumpGetNumAssociations(RpClump* clump) {
    return CHook::CallFunction<uint32>(g_libGTASA + 0x00340824 + 1, clump);
}

// 0x4D6BB0
uint32 RpAnimBlendClumpGetNumNonPartialAssociations(RpClump* clump) {
    return CHook::CallFunction<uint32>(g_libGTASA + 0x00340888 + 1, clump);
}

// 0x4D6B80
uint32 RpAnimBlendClumpGetNumPartialAssociations(RpClump* clump) {
    return CHook::CallFunction<uint32>(g_libGTASA + 0x00340850 +1, clump);
}

// 0x4D6C30
void RpAnimBlendClumpGiveAssociations(RpClump* clump, CAnimBlendAssociation* association) {
    CHook::CallFunction<void>(g_libGTASA + 0x003408E4 + 1, clump, association);
}

// 0x4D6720
void RpAnimBlendClumpInit(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + 0x003403DC + 1, clump);
}

// 0x4D6760
bool RpAnimBlendClumpIsInitialized(RpClump* clump) {
    return CHook::CallFunction<bool>(g_libGTASA + 0x00340478 + 1, clump);
}

// 0x4D6B00
void RpAnimBlendClumpPauseAllAnimations(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + 0x003407CC + 1, clump);
}

// 0x4D6C00
void RpAnimBlendClumpRemoveAllAssociations(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + 0x00340584 + 1, clump);
}

// 0x4D6820
void RpAnimBlendClumpRemoveAssociations(RpClump* clump, uint32 flags) {
    CHook::CallFunction<void>(g_libGTASA + 0x00340520 + 1, clump, flags);
}

// 0x4D67E0
void RpAnimBlendClumpSetBlendDeltas(RpClump* clump, uint32 flags, float delta) {
    CHook::CallFunction<void>(g_libGTASA + 0x003404E4 + 1, clump, flags, delta);
}

// 0x4D6B30
void RpAnimBlendClumpUnPauseAllAnimations(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + 0x003407F8 + 1, clump);
}

// 0x4D34F0
void RpAnimBlendClumpUpdateAnimations(RpClump* clump, float step, bool onScreen) {
    CHook::CallFunction<void>(g_libGTASA + 0x0033D6E4 + 1, clump, step, onScreen);
}

// 0x4D60E0
RtAnimAnimation* RpAnimBlendCreateAnimationForHierarchy(RpHAnimHierarchy* hierarchy) {
    return CHook::CallFunction<RtAnimAnimation*>(g_libGTASA + 0x00340048 + 1, hierarchy);
}

// 0x4D5EF0
char* RpAnimBlendFrameGetName(RwFrame* frame) {
    return CHook::CallFunction<char*>(g_libGTASA + 0x0033FF48 + 1, frame);
}

// 0x4D5F00
void RpAnimBlendFrameSetName(RwFrame* frame, char* name) {
    CHook::CallFunction<void>(g_libGTASA + 0x0033FF58 + 1, frame, name);
}

// 0x4D6AB0
CAnimBlendAssociation* RpAnimBlendGetNextAssociation(CAnimBlendAssociation* association) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x00340798 + 1, association);
}

// 0x4D6AD0
CAnimBlendAssociation* RpAnimBlendGetNextAssociation(CAnimBlendAssociation* association, uint32 flags) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x003407AC + 1, association, flags);
}

// 0x4D60C0
void RpAnimBlendKeyFrameInterpolate(void *pVoidOut, void *pVoidIn1, void *pVoidIn2, RwReal time, void *customData) {
    CHook::CallFunction<void>(g_libGTASA + 0x0033FEF8 + 1, pVoidOut, pVoidIn1, pVoidIn2, time, customData);
}
