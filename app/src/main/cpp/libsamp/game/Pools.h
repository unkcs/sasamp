//
// Created by plaka on 17.04.2023.
//

#ifndef LIVERUSSIA_POOLS_H
#define LIVERUSSIA_POOLS_H

#include <cstddef>
#include <cstdint>
#include "game/Core/Pool.h"
#include "game/Entity/Object.h"
#include "game/Core/PtrNodeSingleLink.h"
#include "game/Core/PtrNodeDoubleLink.h"

void CPools_Initialise_hook();

class CPools {
public:
    static inline CPool<CPtrNodeSingleLink>*        ms_pPtrNodeSingleLinkPool;
    static inline CPool<CPtrNodeDoubleLink>*        ms_pPtrNodeDoubleLinkPool;
    static PoolAllocator::Pool*         ms_pEntryInfoNodePool;
    static inline CPool<CPedGta>*       ms_pPedPool;
    static inline CPool<CVehicleGta>*   ms_pVehiclePool;
    static PoolAllocator::Pool*         ms_pBuildingPool;
    static inline CPool<CObjectGta>*    ms_pObjectPool;
    static PoolAllocator::Pool*         ms_pDummyPool;
    static PoolAllocator::Pool*         ms_pColModelPool;
    static PoolAllocator::Pool*         ms_pTaskPool;
    static PoolAllocator::Pool*         ms_pEventPool;
    static PoolAllocator::Pool*         ms_pPointRoutePool;
    static PoolAllocator::Pool*         ms_pPatrolRoutePool;
    static PoolAllocator::Pool*         ms_pNodeRoutePool;
    static PoolAllocator::Pool*         ms_pTaskAllocatorPool;
    static PoolAllocator::Pool*         ms_pPedIntelligencePool;
    static PoolAllocator::Pool*         ms_pPedAttractorPool;

public:
    static void InjectHooks();

    static void Initialise();
};

static auto& GetPedPoolGta()                { return CPools::ms_pPedPool; }
static auto& GetVehiclePoolGta()          { return CPools::ms_pVehiclePool; }
static auto& GetBuildingPoolGta()          { return CPools::ms_pBuildingPool; }
static auto& GetObjectPoolGta()           { return CPools::ms_pObjectPool; }
static auto& GetDummyPoolGta()             { return CPools::ms_pDummyPool; }
static auto& GetColModelPoolGta()          { return CPools::ms_pColModelPool; }
static auto& GetTaskPoolGta()              { return CPools::ms_pTaskPool; }
static auto& GetPedIntelligencePoolGta()   { return CPools::ms_pPedIntelligencePool; }
static auto& GetPtrNodeSingleLinkPool()          { return CPools::ms_pPtrNodeSingleLinkPool; }
static auto& GetPtrNodeDoubleLinkPool()          { return CPools::ms_pPtrNodeDoubleLinkPool; }
static auto& GetEntryInfoNodePoolGta()     { return CPools::ms_pEntryInfoNodePool; }
static auto& GetPointRoutePoolGta()        { return CPools::ms_pPointRoutePool; }
static auto& GetPatrolRoutePoolGta()       { return CPools::ms_pPatrolRoutePool; }
static auto& GetEventPoolGta()             { return CPools::ms_pEventPool; }
static auto& GetNodeRoutePoolGta()         { return CPools::ms_pNodeRoutePool; }
static auto& GetTaskAllocatorPoolGta()     { return CPools::ms_pTaskAllocatorPool; }
static auto& GetPedAttractorPoolGta()      { return CPools::ms_pPedAttractorPool; }




#endif //LIVERUSSIA_POOLS_H
