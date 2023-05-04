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

void CPools_Initialise_hook();

class CPools {
public:
    static inline CPool<CPtrNodeSingleLink>*        ms_pPtrNodeSingleLinkPool;
    static PoolAllocator::Pool*         ms_pPtrNodeDoubleLinkPool;
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

static auto& GetPedPoolGta()                { return CPools::ms_pPedPool; }               // 0x404550
static auto& GetVehiclePoolGta()          { return CPools::ms_pVehiclePool; }           // 0x404560
static auto& GetBuildingPoolGta()          { return CPools::ms_pBuildingPool; }          // 0x403DF0
static auto& GetObjectPoolGta()            { return CPools::ms_pObjectPool; }            // 0x404570
static auto& GetDummyPoolGta()             { return CPools::ms_pDummyPool; }             // 0x404580
static auto& GetColModelPoolGta()          { return CPools::ms_pColModelPool; }          // 0x40F000
static auto& GetTaskPoolGta()              { return CPools::ms_pTaskPool; }              // 0x61A330
static auto& GetPedIntelligencePoolGta()   { return CPools::ms_pPedIntelligencePool; }   // 0x5FF930
static auto& GetPtrNodeSingleLinkPool() { return CPools::ms_pPtrNodeSingleLinkPool; } // 0x552190
static auto& GetPtrNodeDoubleLinkPoolGta() { return CPools::ms_pPtrNodeDoubleLinkPool; } // 0x5521A0
static auto& GetEntryInfoNodePoolGta()     { return CPools::ms_pEntryInfoNodePool; }     // 0x536C80
static auto& GetPointRoutePoolGta()        { return CPools::ms_pPointRoutePool; }        // 0x41B400
static auto& GetPatrolRoutePoolGta()       { return CPools::ms_pPatrolRoutePool; }       // 0x41B410
static auto& GetEventPoolGta()             { return CPools::ms_pEventPool; }             // 0x4ABF80
static auto& GetNodeRoutePoolGta()         { return CPools::ms_pNodeRoutePool; }         // 0x41B420
static auto& GetTaskAllocatorPoolGta()     { return CPools::ms_pTaskAllocatorPool; }     // 0x69BB70
static auto& GetPedAttractorPoolGta()      { return CPools::ms_pPedAttractorPool; }      // 0x5E95A0




#endif //LIVERUSSIA_POOLS_H
