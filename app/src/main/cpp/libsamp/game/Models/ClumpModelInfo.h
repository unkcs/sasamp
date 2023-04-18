//
// Created by plaka on 07.03.2023.
//

#ifndef LIVERUSSIA_CLUMPMODELINFO_H
#define LIVERUSSIA_CLUMPMODELINFO_H


#include "BaseModelInfo.h"

#pragma pack(push, 1)
struct CClumpModelInfo : public CBaseModelInfo {
    union {
        char *m_animFileName;
        unsigned int m_dwAnimFileIndex;
    };
};
static_assert(sizeof(CClumpModelInfo) == 0x3c);
#pragma pack(pop)


#endif //LIVERUSSIA_CLUMPMODELINFO_H
