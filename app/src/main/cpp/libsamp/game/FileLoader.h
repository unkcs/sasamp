//
// Created by plaka on 15.04.2023.
//

#pragma once


#include "common.h"
#include "CFileMgr.h"

enum eSection : uint8 {
    UNDEFINED             = 0,
    OBJECT                = 1,
    TIME_OBJECT           = 3,
    WEAPON_OBJECT         = 4,
    CLUMP_OBJECT          = 5,
    ANIMATED_CLUMP_OBJECT = 6,
    VEHICLE_OBJECT        = 7,
    PED_OBJECT            = 8,
    PATH_NODE             = 9,
    TWO_D_EFFECT          = 10,
    TXD_PARENT            = 11,
};

enum eIDE : uint8 {
    IDE_NONE,
    IDE_OBJS,
    IDE_MLO, // ?
    IDE_TOBJ,
    IDE_WEAP,
    IDE_HIER,
    IDE_ANIM,
    IDE_CARS,
    IDE_PEDS,
    IDE_PATH,
    IDE_2DFX,
    IDE_TXDP,
};

enum eIPL : uint8 {
    IPL_NONE,
    IPL_PATH,
    IPL_INST,
    IPL_MULT,
    IPL_ZONE,
    IPL_CULL,
    IPL_OCCL,
    IPL_UNK7,
    IPL_GRGE,
    IPL_ENEX,
    IPL_PICK,
    IPL_CARS,
    IPL_JUMP,
    IPL_TCYC,
    IPL_AUZO,
};

class CFileLoader {
public:
    static inline char ms_line[512];

public:
    static char* LoadLine(FILE* file);
    static char* LoadLine(char*& outLine, int32& outSize);

    static char *FindFirstNonNullOrWS(char *it);
};

