#include "AnimBlendStaticAssociation.h"
#include "util/patch.h"

void CAnimBlendStaticAssociation::InjectHooks() {

}

// 0x4CE940
CAnimBlendStaticAssociation::CAnimBlendStaticAssociation() {
    m_nAnimId = -1;
    m_nAnimGroup = -1;
    m_nFlags = 0;
    m_pSequenceArray = 0;
    m_pHierarchy = 0;
}

// 0x4CEF60
CAnimBlendStaticAssociation::CAnimBlendStaticAssociation(RpClump* clump, CAnimBlendHierarchy* hier) {
    CHook::CallFunction<void>(g_libGTASA + 0x00339D04 + 1, this, clump, hier);
}

// 0x4CDF50 virtual
CAnimBlendStaticAssociation::~CAnimBlendStaticAssociation() {
    assert(0);
}

// 0x4CEC20
void CAnimBlendStaticAssociation::Init(RpClump* clump, CAnimBlendHierarchy* hier) {
    CHook::CallFunction<void>(g_libGTASA + 0x00339C68 + 1, this, clump, hier);
}

// 0x4CE960
void CAnimBlendStaticAssociation::AllocateSequenceArray(int32 count) {
    m_pSequenceArray = (CAnimBlendSequence**)CMemoryMgr::Malloc(sizeof(CAnimBlendSequence*) * count);
    for (auto& array : std::span{ m_pSequenceArray, (size_t)count }) {
        array = nullptr;
    }
}

// 0x4CE9A0
void CAnimBlendStaticAssociation::FreeSequenceArray() {
    if (m_pSequenceArray) {
        CMemoryMgr::Free(m_pSequenceArray);
    }
}
