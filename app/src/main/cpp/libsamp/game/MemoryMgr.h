//
// Created by plaka on 28.04.2023.
//

#ifndef LIVERUSSIA_MEMORYMGR_H
#define LIVERUSSIA_MEMORYMGR_H


class CMemoryMgr {

public:
    static void *Malloc(unsigned int size, unsigned int nHint);

    static void Free(void *memory);

    static void *Malloc(unsigned int size);
};


#endif //LIVERUSSIA_MEMORYMGR_H
