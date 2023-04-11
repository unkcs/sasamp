//
// Created by plaka on 07.03.2023.
//

#ifndef LIVERUSSIA_CLUMPMODELINFO_H
#define LIVERUSSIA_CLUMPMODELINFO_H


#include "BaseModelInfo.h"

struct CClumpModelInfo : public CBaseModelInfo {
    union {
        char *m_animFileName;
        unsigned int m_dwAnimFileIndex;
    };
};
// 0x3C


#endif //LIVERUSSIA_CLUMPMODELINFO_H
