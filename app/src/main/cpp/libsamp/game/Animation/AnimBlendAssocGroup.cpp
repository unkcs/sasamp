/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/

#include "AnimBlendAssocGroup.h"
#include "../util/patch.h"

// 0x4CDE70
CAnimBlendAssocGroup::CAnimBlendAssocGroup() {
    CHook::CallFunction<void>(g_libGTASA + 0x00339590 + 1, this);
    //plugin::CallMethod<0x4CDE70, CAnimBlendAssocGroup*>(this);
}

// 0x4CE0B0
CAnimBlendAssociation* CAnimBlendAssocGroup::CopyAnimation(const char* AnimName) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x00339BAC + 1, this, AnimName);
}

// 0x4CE130
CAnimBlendAssociation* CAnimBlendAssocGroup::CopyAnimation(uint32 ID) {
    return CHook::CallFunction<CAnimBlendAssociation*>(g_libGTASA + 0x00339BD4 + 1, this, ID);
}

// 0x4CE220
void CAnimBlendAssocGroup::CreateAssociations(const char* szBlockName) {
    return CHook::CallFunction<void>(g_libGTASA + 0x00339730 + 1, szBlockName);
}

// 0x4CE6E0
void CAnimBlendAssocGroup::CreateAssociations(const char* AnimName, RpClump* clump, char** arg3, int32 NumAnimations) {
    CHook::CallFunction<void>(g_libGTASA + 0x00339A78 + 1, this, AnimName, clump, arg3, NumAnimations);
}

// 0x4CE3B0
void CAnimBlendAssocGroup::CreateAssociations(const char* AnimName, const char* arg2, const char* arg3, uint32 strStorageSz) {
    CHook::CallFunction<void>(g_libGTASA + 0x00339840 + 1, this, AnimName, arg2, arg3, strStorageSz);
}

// 0x4CDFF0
void CAnimBlendAssocGroup::DestroyAssociations() {
    CHook::CallFunction<void>(g_libGTASA + 0x003396E0, this);
}

CAnimBlendStaticAssociation* CAnimBlendAssocGroup::GetAnimation(const char* AnimName) {
    return CHook::CallFunction<CAnimBlendStaticAssociation*>(g_libGTASA + 0x00339B50 + 1, this, AnimName);
}

CAnimBlendStaticAssociation* CAnimBlendAssocGroup::GetAnimation(uint32 id) {
    return &this->m_pAssociations[id - this->m_nIdOffset];
   // return CHook::CallFunction<CAnimBlendStaticAssociation*>(g_libGTASA + 0x00339B90 + 1, this, ID);
}

// 0x4CE1B0
uint32 CAnimBlendAssocGroup::GetAnimationId(const char* AnimName) {
    return CHook::CallFunction<uint32>(g_libGTASA + 0x00339BFC + 1, this, AnimName);
}

// 0x4CDFB0
void CAnimBlendAssocGroup::InitEmptyAssociations(RpClump* clump) {
    CHook::CallFunction<void>(g_libGTASA + 0x003396A8 + 1, this, clump);
}

// 0x4CE1D0
CAnimBlendAssocGroup::~CAnimBlendAssocGroup() {
    CHook::CallFunction<void>(g_libGTASA + 0x00339720 + 1, this);
}

