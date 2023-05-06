//
// Created by plaka on 07.04.2023.
//

#pragma once

#include "rwplcore.h"
#include "rwcore.h"
#include <stdint.h>
extern uintptr_t g_libGTASA;

#define RpAtomicGetGeometryMacro(_atomic)                               \
    ((_atomic)->geometry)

#define RpMaterialGetColorMacro(_material)                          \
    (&((_material)->color))

#define RpAtomicGetGeometry(_atomic) \
    RpAtomicGetGeometryMacro(_atomic)

#define RpMaterialGetColor(_material)                               \
    RpMaterialGetColorMacro(_material)

#define RpMaterialSetColorMacro(_material, _color)                  \
    (RwRGBAAssign(&((_material)->color), (_color)), (_material))

#define RpMaterialGetColorMacro(_material)                          \
    (&((_material)->color))

#define RpMaterialSetSurfacePropertiesMacro(_material, _surfProps)  \
    (RwSurfacePropertiesAssign(&((_material)->surfaceProps),        \
                               (_surfProps)), (_material))

#define RpMaterialGetSurfacePropertiesMacro(_material)              \
    (&((_material)->surfaceProps))

#define RpMaterialGetTextureMacro(_material)                        \
    ((_material)->texture)

#define RpMaterialAddRef(_material)                                 \
    RpMaterialAddRefMacro(_material)

#define RpMaterialSetColor(_material, _color)                       \
    RpMaterialSetColorMacro(_material, _color)

#define RpMaterialGetColor(_material)                               \
    RpMaterialGetColorMacro(_material)

#define RpMaterialSetSurfaceProperties(_material, _surfProps)       \
    RpMaterialSetSurfacePropertiesMacro(_material, _surfProps)

#define RpMaterialGetSurfaceProperties(_material)                   \
    RpMaterialGetSurfacePropertiesMacro(_material)

#define RpMaterialGetTexture(_material)                             \
    RpMaterialGetTextureMacro(_material)

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

/**
 * \ingroup rpatomic
 * \ref RpAtomicCallBack represents the function called from \ref RpWorldSectorForAllAtomics and
 * \ref RpClumpForAllAtomics for all atomics in a given world sector or clump.
 * This function should return a pointer to the current atomic to indicate
 * success. The callback may return NULL to terminate further callbacks on
 * the world sector.
 *
 * \param  atomic   Pointer to the current atomic, supplied by
 *                  iterator.
 * \param  data     Pointer to developer-defined data structure.
 *
 * \return Returns a pointer to the current atomic
 */
typedef RpAtomic   *(*RpAtomicCallBack) (RpAtomic * atomic, void *data);

/**
 * \ingroup rpmaterial
 * \ref RpMaterialCallBack
 * represents the function called from \ref RpGeometryForAllMaterials and
 * \ref RpWorldForAllMaterials for all materials referenced by polygons in a
 * given geometry. This function should return a pointer to the current
 * material to indicate success. The callback may return NULL to terminate
 * further callbacks on the materials.
 *
 * \param  material   Pointer to the current material
 * \param  data       Pointer to developer-defined data structure.
 *
 * \return Pointer to the current material.
 */
typedef RpMaterial *(*RpMaterialCallBack)(RpMaterial *material, void *data);

RpClump* RpClumpForAllAtomics(RpClump* clump, RpAtomicCallBack callback, void* data);
RpGeometry* RpGeometryForAllMaterials(RpGeometry* geometry, RpMaterialCallBack fpCallBack, void* data);
RwBool RpClumpDestroy(RpClump* clump);
