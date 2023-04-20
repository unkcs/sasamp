
#include "rtquat.h"
#include "rpworld.h"

void RtQuatConvertToMatrix(const RtQuat* const qpQuat, RwMatrix* const mpMatrix) {
    RwReal              rS;
    RwV3d               rV;
    RwV3d               rW;
    RwV3d               square;
    RwV3d               cross;

    rS = ((RwReal)2) / (qpQuat->imag.x * qpQuat->imag.x
                        + qpQuat->imag.y * qpQuat->imag.y
                        + qpQuat->imag.z * qpQuat->imag.z
                        + qpQuat->real * qpQuat->real);

    RwV3dScale(&rV, &(qpQuat)->imag, rS);
    RwV3dScale(&rW, &rV, (qpQuat)->real);

    square.x = (qpQuat)->imag.x * rV.x;
    square.y = (qpQuat)->imag.y * rV.y;
    square.z = (qpQuat)->imag.z * rV.z;

    cross.x = (qpQuat)->imag.y * rV.z;
    cross.y = (qpQuat)->imag.z * rV.x;
    cross.z = (qpQuat)->imag.x * rV.y;

    (mpMatrix)->right.x = ((RwReal)1) - (square.y + square.z);
    (mpMatrix)->right.y = cross.z + rW.z;
    (mpMatrix)->right.z = cross.y - rW.y;

    (mpMatrix)->up.x = cross.z - rW.z;
    (mpMatrix)->up.y = ((RwReal)1) - (square.z + square.x);
    (mpMatrix)->up.z = cross.x + rW.x;

    (mpMatrix)->at.x = cross.y + rW.y;
    (mpMatrix)->at.y = cross.x - rW.x;
    (mpMatrix)->at.z = ((RwReal)1) - (square.x + square.y);

    /* Set position */
    (mpMatrix)->pos.x = ((RwReal)0);
    (mpMatrix)->pos.y = ((RwReal)0);
    (mpMatrix)->pos.z = ((RwReal)0);

    /* Matrix is orthogonal */
    mpMatrix->flags = rwMATRIXTYPEORTHONORMAL & ~rwMATRIXINTERNALIDENTITY;
}

RwBool RtQuatConvertFromMatrix(RtQuat* qpQuat, const RwMatrix* const mpMatrix) {
    return ((RwBool(__cdecl*)(RtQuat*, const RwMatrix* const))(g_libGTASA + 0x001DE0DC + 1))(qpQuat, mpMatrix);
}

RtQuat* RtQuatRotate(RtQuat* quat, const RwV3d* axis, RwReal angle, RwOpCombineType combineOp) {
    return ((RtQuat*(__cdecl *)(RtQuat*, const RwV3d*, RwReal, RwOpCombineType))(g_libGTASA + 0x001DE1C4 + 1))(quat, axis, angle, combineOp);
}

const RtQuat* RtQuatQueryRotate(const RtQuat* quat, RwV3d* unitAxis, RwReal* angle) {
    return ((const RtQuat*(__cdecl *)(const RtQuat*, RwV3d*, RwReal*))(g_libGTASA + 0x001DE350 + 1))(quat, unitAxis, angle);
}

RwV3d* RtQuatTransformVectors(RwV3d* vectorsOut, const RwV3d* vectorsIn, const RwInt32 numPoints, const RtQuat* quat) {
    return ((RwV3d*(__cdecl *)(RwV3d*, const RwV3d*, const RwInt32, const RtQuat*))(g_libGTASA + 0x001DE490 + 1))(vectorsOut, vectorsIn, numPoints, quat);
}

RwReal RtQuatModulus(RtQuat* q) {
    return ((RwReal(__cdecl *)(RtQuat*))(g_libGTASA + 0x001DE570 + 1))(q);
}
