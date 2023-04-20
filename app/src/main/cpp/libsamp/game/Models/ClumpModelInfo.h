//
// Created by plaka on 07.03.2023.
//

#ifndef LIVERUSSIA_CLUMPMODELINFO_H
#define LIVERUSSIA_CLUMPMODELINFO_H


#include "BaseModelInfo.h"
#include "game/common.h"
#include "game/RW/rpworld.h"

#pragma pack(push, 1)
struct CClumpModelInfo : public CBaseModelInfo {
    union {
        char *m_animFileName;
        unsigned int m_dwAnimFileIndex;
    };

public:
    // static functions
    static RpAtomic* SetAtomicRendererCB(RpAtomic* atomic, void* renderFunc);
    static RpAtomic* AtomicSetupLightingCB(RpAtomic* atomic, void* data);
    static RpAtomic* SetHierarchyForSkinAtomic(RpAtomic* atomic, void* data);
    /* struct tSearchData { char *name; RwFrame *result; };
      returns 0 if we found frame, or last frame if we need to continue searching */
    static RwFrame* FindFrameFromNameCB(RwFrame* frame, void* searchData);
    static RwFrame* FindFrameFromNameWithoutIdCB(RwFrame* frame, void* searchData);
    static RwFrame* FindFrameFromIdCB(RwFrame* frame, void* searchData);
    static RwFrame* FillFrameArrayCB(RwFrame* frame, void* data);
    static RwFrame* GetFrameFromId(RpClump* clump, int32 id);
    static RwFrame* GetFrameFromName(RpClump* clump, const char* name);
    static void FillFrameArray(RpClump* clump, RwFrame** frames);
};
static_assert(sizeof(CClumpModelInfo) == 0x3c);
#pragma pack(pop)


#endif //LIVERUSSIA_CLUMPMODELINFO_H
