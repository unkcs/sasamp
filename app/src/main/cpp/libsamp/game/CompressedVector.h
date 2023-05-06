/*
    Plugin-SDK file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once

#include "common.h"

#pragma pack(push, 1)
class CompressedVector {
public:
    int16 x;
    int16 y;
    int16 z;
};

#pragma pack(pop)
VALIDATE_SIZE(CompressedVector, 0x6);

CVector UncompressVector(const CompressedVector& compressedVec);
CompressedVector CompressVector(const CVector& vec);

float UncompressUnitFloat(int16 val);

CVector UncompressUnitVector(const CompressedVector& compressedVec);
CompressedVector CompressUnitVector(const CVector& vec);

CVector UncompressLargeVector(const CompressedVector& compressedVec);
CompressedVector CompressLargeVector(const CVector& vec);

CVector UncompressFxVector(const CompressedVector& compressedVec);
CVector CompressFxVector(const CompressedVector& compressedVec);
