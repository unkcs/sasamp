//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "rwcore.h"
#include "rwlpcore.h"

/**
 * \ingroup rpclump
 * \struct RpClump
 * Clump Geometry object. This should be
 * considered an opaque type. Use the RpClump API functions to access.
 */
typedef struct RpClump RpClump;

/**
 * \ingroup rpclump
 * RpClumpCallBack represents the function called from
 * \ref RwCameraForAllClumpsInFrustum when a clump lies inside the current
 * camera's view frustum. It is also the callback type used with
 * \ref RpWorldForAllClumps.
 *
 * This function should return a pointer to the current clump to indicate
 * success. The callback may return NULL to terminate further callbacks on
 * the clumps.
 *
 * \param  clump   Pointer to the current clump, supplied by
 *                 iterator.
 * \param  data    Pointer to developer-defined data structure.
 *
 * \return Pointer to the current clump.
 *
 * \see RpWorldForAllClumps
 * \see RpClumpGetCallBack
 * \see RpClumpSetCallBack
 * \see RwCameraForAllClumpsInFrustum
 */
typedef RpClump    *(*RpClumpCallBack) (RpClump * clump, void *data);

struct RpClump
{
    RwObject            object;

    /* Information about all the Atomics */
    RwLinkList          atomicList;

    /* Lists of lights and cameras */
    RwLinkList          lightList;
    RwLinkList          cameraList;

    /* The clump in a world */
    RwLLLink            inWorldLink;

    /* Clump frustum callback */
    RpClumpCallBack     callback;
};

/**
 * \ingroup rpatomic
 * \struct RpAtomic
 * Atomic Geometry object. This should be
 * considered an opaque type. Use the RpAtomic API functions to access.
 */
typedef struct RpAtomic RpAtomic;

typedef struct RpInterpolator RpInterpolator;

struct RpInterpolator
{
    RwInt32             flags; /**< flags */
    RwInt16             startMorphTarget; /**< startMorphTarget */
    RwInt16             endMorphTarget; /**< endMorphTarget */
    RwReal              time; /**< time */
    RwReal              recipTime; /**< recipTime */
    RwReal              position; /**< position */
};

typedef RpAtomic   *(*RpAtomicCallBackRender) (RpAtomic * atomic);

struct RpAtomic
{
    RwObjectHasFrame    object;

    /* Information for an instance */
    RwResEntry         *repEntry;

    /* Triangles making the object */
    RpGeometry         *geometry;

    /* Interpolated bounding sphere (in object space and world space) */
    RwSphere            boundingSphere;
    RwSphere            worldBoundingSphere;

    /* Connections to other atomics */
    RpClump            *clump;
    RwLLLink            inClumpLink;

    /* callbacks */
    RpAtomicCallBackRender renderCallBack;

    /* Interpolation animation pointer */
    RpInterpolator      interpolator;

    /* Counter for checks of "render has occurred already" */
    RwUInt16            renderFrame;
    RwUInt16            pad;

    /* Connections to sectors */
    RwLinkList          llWorldSectorsInAtomic;

    /* The Atomic object pipeline for this Atomic */
    RxPipeline         *pipeline;
};
