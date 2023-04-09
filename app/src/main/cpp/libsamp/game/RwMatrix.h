//
// Created by plaka on 06.04.2023.
//

#ifndef LIVERUSSIA_RWMATRIX_H
#define LIVERUSSIA_RWMATRIX_H

#pragma pack(1)

#include "Core/Vector.h"
#include "game/RW/rwlpcore.h"

struct RwMatrixTag
{
    VECTOR right;		// 0-12 	; r11 r12 r13
    uint32_t  flags;	// 12-16
    VECTOR up;			// 16-28	; r21 r22 r23
    float  pad_u;		// 28-32
    VECTOR at;			// 32-44	; r31 r32 r33
    float  pad_a;		// 44-48
    RwV3d   pos;			// 48-60
    float  pad_p;		// 60-64
};
typedef RwMatrixTag RwMatrix;

#endif //LIVERUSSIA_RWMATRIX_H
