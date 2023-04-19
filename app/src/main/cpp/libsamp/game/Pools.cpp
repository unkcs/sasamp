//
// Created by plaka on 17.04.2023.
//

#include "Pools.h"
#include "../util/patch.h"
#include "IplDef.h"

PoolAllocator::Pool*      CPools::ms_pPtrNodeSingleLinkPool = nullptr;
PoolAllocator::Pool*      CPools::ms_pPtrNodeDoubleLinkPool;
PoolAllocator::Pool*      CPools::ms_pEntryInfoNodePool;
PoolAllocator::Pool*      CPools::ms_pPedPool;
PoolAllocator::Pool*      CPools::ms_pVehiclePool;
PoolAllocator::Pool*      CPools::ms_pBuildingPool;
PoolAllocator::Pool*      CPools::ms_pObjectPool;
PoolAllocator::Pool*      CPools::ms_pDummyPool;
PoolAllocator::Pool*      CPools::ms_pColModelPool;
PoolAllocator::Pool*      CPools::ms_pTaskPool;
PoolAllocator::Pool*      CPools::ms_pEventPool;
PoolAllocator::Pool*      CPools::ms_pPointRoutePool;
PoolAllocator::Pool*      CPools::ms_pPatrolRoutePool;
PoolAllocator::Pool*      CPools::ms_pNodeRoutePool;
PoolAllocator::Pool*      CPools::ms_pTaskAllocatorPool;
PoolAllocator::Pool*      CPools::ms_pPedIntelligencePool;
PoolAllocator::Pool*      CPools::ms_pPedAttractorPool;

void (*CPools_Initialise)(void);
void CPools_Initialise_hook(void)
{


    // 600000 / 75000 = 8
    CPools::ms_pPtrNodeSingleLinkPool = PoolAllocator::Allocate(100000, 8);		// 75000
    // 72000 / 6000 = 12
    CPools::ms_pPtrNodeDoubleLinkPool = PoolAllocator::Allocate(60000, 12);	// 6000
    // 10000 / 500 = 20
    CPools::ms_pEntryInfoNodePool = PoolAllocator::Allocate(20000, 20);	// 500
    // 279440 / 140 = 1996
    CPools::ms_pPedPool = PoolAllocator::Allocate(240, 1996);	// 140
    // 286440 / 110 = 2604
    CPools::ms_pVehiclePool = PoolAllocator::Allocate(2000, 2604);	// 110
    // 840000 / 14000 = 60
    CPools::ms_pBuildingPool = PoolAllocator::Allocate(20000, 60);	// 14000
    // 147000 / 350 = 420
    CPools::ms_pObjectPool = PoolAllocator::Allocate(3000, 420);	// 350
    // 210000 / 3500 = 60
    CPools::ms_pDummyPool = PoolAllocator::Allocate(40000, 60);	// 3500
    // 487200 / 10150 = 48
    CPools::ms_pColModelPool = PoolAllocator::Allocate(50000, 48);	// 10150
    // 64000 / 500 = 128
    CPools::ms_pTaskPool = PoolAllocator::Allocate(5000, 128);	// 500
    // 13600 / 200 = 68
    CPools::ms_pEventPool = PoolAllocator::Allocate(1000, 68);	// 200
    // 6400 / 64 = 100
    CPools::ms_pPointRoutePool = PoolAllocator::Allocate(200, 100);	// 64
    // 13440 / 32 = 420
    CPools::ms_pPatrolRoutePool = PoolAllocator::Allocate(200, 420);	// 32
    // 2304 / 64 = 36
    CPools::ms_pNodeRoutePool = PoolAllocator::Allocate(200, 36);	// 64
    // 512 / 16 = 32
    CPools::ms_pTaskAllocatorPool = PoolAllocator::Allocate(3000, 32);	// 16
    // 92960 / 140 = 664
    CPools::ms_pPedIntelligencePool = PoolAllocator::Allocate(240, 664);	// 140
    // 15104 / 64 = 236
    CPools::ms_pPedAttractorPool = PoolAllocator::Allocate(200, 236);	// 64

    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93E0) = CPools::ms_pPtrNodeSingleLinkPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93DC) = CPools::ms_pPtrNodeDoubleLinkPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93D8) = CPools::ms_pEntryInfoNodePool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93D4) = CPools::ms_pPedPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93D0) = CPools::ms_pVehiclePool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93CC) = CPools::ms_pBuildingPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93C8) = CPools::ms_pObjectPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93C4) = CPools::ms_pDummyPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93C0) = CPools::ms_pColModelPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93BC) = CPools::ms_pTaskPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93B8) = CPools::ms_pEventPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93B4) = CPools::ms_pPointRoutePool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93B0) = CPools::ms_pPatrolRoutePool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93AC) = CPools::ms_pNodeRoutePool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93A8) = CPools::ms_pTaskAllocatorPool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93A4) = CPools::ms_pPedIntelligencePool;
    *(PoolAllocator::Pool**)(g_libGTASA + 0x8B93A0) = CPools::ms_pPedAttractorPool;
}

void CPools::InjectHooks() {
    CHook::InlineHook(g_libGTASA, 0x3AF1A0, &CPools_Initialise_hook, &CPools_Initialise);
}