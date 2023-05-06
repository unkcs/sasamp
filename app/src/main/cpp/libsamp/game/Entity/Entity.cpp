//
// Created by plaka on 20.04.2023.
//
#include "../common.h"

#include "Entity.h"
#include "../RW/rwcore.h"
#include "../RwHelper.h"
#include "game/RW/rpskin.h"
#include "util/patch.h"
#include "game/Models/ModelInfo.h"
#include "game/References.h"
#include "game/Pools.h"

void CEntityGta::UpdateRwFrame()
{
    if (!m_pRwObject)
        return;

    RwFrameUpdateObjects(static_cast<RwFrame*>(rwObjectGetParent(m_pRwObject)));
}

void CEntityGta::UpdateRpHAnim() {
    if (const auto atomic = GetFirstAtomic(m_pRwClump)) {
        if (RpSkinGeometryGetSkin(RpAtomicGetGeometry(atomic)) && !m_bDontUpdateHierarchy) {
            RpHAnimHierarchyUpdateMatrices(GetAnimHierarchyFromSkinClump(m_pRwClump));
        }
    }
}

CColModel* CEntityGta::GetColModel() const {
    if (IsVehicle()) {
        const auto veh = static_cast<const CVehicleGta*>(this);
        if (veh->m_vehicleSpecialColIndex > -1) {
            return &CVehicleGta::m_aSpecialColModel[veh->m_vehicleSpecialColIndex];
        }
    }

    return CModelInfo::GetModelInfo(m_nModelIndex)->GetColModel();
}

CVector CEntityGta::TransformFromObjectSpace(const CVector& offset)
{
    auto result = CVector();
    if (m_matrix) {
        result = *m_matrix * offset;
        return result;
    }

    CUtil::TransformPoint(result, m_placement, offset);
    return result;
}

// 0x533560
CVector* CEntityGta::TransformFromObjectSpace(CVector& outPos, const CVector& offset)
{
    auto result = TransformFromObjectSpace(offset);
    outPos = result;
    return &outPos;
}

void CEntityGta::SetCollisionChecking(bool bCheck)
{
    m_bCollisionProcessed = bCheck;
}

CVector* CEntityGta::GetBoundCentre(CVector* pOutCentre)
{
    auto mi = CModelInfo::GetModelInfo(m_nModelIndex);
    const auto& colCenter = mi->GetColModel()->GetBoundCenter();
    return TransformFromObjectSpace(*pOutCentre, colCenter);
}

// 0x534290
void CEntityGta::GetBoundCentre(CVector& outCentre) {
    TransformFromObjectSpace(outCentre, GetColModel()->GetBoundCenter());
}

CVector CEntityGta::GetBoundCentre()
{
    CVector v;
    GetBoundCentre(v);
    return v;
}

bool CEntityGta::GetIsTouching(CEntityGta* entity)
{
    CVector thisVec;
    GetBoundCentre(thisVec);

    CVector otherVec;
    entity->GetBoundCentre(otherVec);

    auto fThisRadius = CModelInfo::GetModelInfo(m_nModelIndex)->GetColModel()->GetBoundRadius();
    auto fOtherRadius = CModelInfo::GetModelInfo(entity->m_nModelIndex)->GetColModel()->GetBoundRadius();

    return (thisVec - otherVec).Magnitude() <= (fThisRadius + fOtherRadius);
}

void CEntityGta::RegisterReference(CEntityGta** entity)
{
    if (IsBuilding() && !m_bIsTempBuilding && !m_bIsProcObject && !m_nIplIndex)
        return;

    auto refs = m_pReferences;
    while (refs) {
        if (refs->m_ppEntity == entity) {
            return;
        }
        refs = refs->m_pNext;
    }

    if (!m_pReferences && !CReferences::pEmptyList) {
        auto iPedsSize = GetPedPoolGta()->GetSize();
        for (int32 i = 0; i < iPedsSize; ++i) {
            auto ped = GetPedPoolGta()->GetAt(i);
            if (ped) {
                ped->PruneReferences();
                if (CReferences::pEmptyList)
                    break;
            }

        }

        if (!CReferences::pEmptyList) {
            auto iVehsSize = GetVehiclePoolGta()->GetSize();
            for (int32 i = 0; i < iVehsSize; ++i) {
                auto vehicle = GetVehiclePoolGta()->GetAt(i);
                if (vehicle) {
                    vehicle->PruneReferences();
                    if (CReferences::pEmptyList)
                        break;
                }

            }
        }

        if (!CReferences::pEmptyList) {
            auto iObjectsSize = GetObjectPoolGta()->GetSize();
            for (int32 i = 0; i < iObjectsSize; ++i) {
                auto obj = GetObjectPoolGta()->GetAt(i);
                if (obj) {
                    obj->PruneReferences();
                    if (CReferences::pEmptyList)
                        break;
                }
            }
        }
    }

    if (CReferences::pEmptyList) {
        auto pEmptyRef = CReferences::pEmptyList;
        CReferences::pEmptyList = pEmptyRef->m_pNext;
        pEmptyRef->m_pNext = m_pReferences;
        m_pReferences = pEmptyRef;
        pEmptyRef->m_ppEntity = entity;
    }
}

void CEntityGta::ResolveReferences()
{
    auto refs = m_pReferences;
    while (refs) {
        if (*refs->m_ppEntity == this)
            *refs->m_ppEntity = nullptr;

        refs = refs->m_pNext;
    }

    refs = m_pReferences;
    if (!refs)
        return;

    refs->m_ppEntity = nullptr;
    while (refs->m_pNext)
        refs = refs->m_pNext;

    refs->m_pNext = CReferences::pEmptyList;
    CReferences::pEmptyList = m_pReferences;
    m_pReferences = nullptr;
}

void CEntityGta::PruneReferences()
{
    if (!m_pReferences)
        return;

    auto refs = m_pReferences;
    auto ppPrev = &m_pReferences;
    while (refs) {
        if (*refs->m_ppEntity == this) {
            ppPrev = &refs->m_pNext;
            refs = refs->m_pNext;
        }
        else {
            auto refTemp = refs->m_pNext;
            *ppPrev = refs->m_pNext;
            refs->m_pNext = CReferences::pEmptyList;
            CReferences::pEmptyList = refs;
            refs->m_ppEntity = nullptr;
            refs = refTemp;
        }
    }
}

void CEntityGta::CleanUpOldReference(CEntityGta** entity)
{
    if (!m_pReferences)
        return;

    auto refs = m_pReferences;
    auto ppPrev = &m_pReferences;
    while (refs->m_ppEntity != entity) {
        ppPrev = &refs->m_pNext;
        refs = refs->m_pNext;
        if (!refs)
            return;
    }

    *ppPrev = refs->m_pNext;
    refs->m_pNext = CReferences::pEmptyList;
    refs->m_ppEntity = nullptr;
    CReferences::pEmptyList = refs;
}

bool CEntityGta::DoesNotCollideWithFlyers()
{
    auto mi = CModelInfo::GetModelInfo(m_nModelIndex);
    return mi->SwaysInWind() || mi->bDontCollideWithFlyer;
}


// ------------- hooks

inline void CleanUpOldReference_hook(CEntityGta *thiz, CEntityGta** entity) {
    thiz->CleanUpOldReference(entity);
}

inline void ResolveReferences_hook(CEntityGta *thiz) {
    thiz->ResolveReferences();
}

inline void PruneReferences_hook(CEntityGta* thiz) {
    thiz->PruneReferences();
}

inline void RegisterReference_hook(CEntityGta* thiz, CEntityGta** entity) {
    thiz->RegisterReference(entity);
}

void CEntityGta::InjectHooks() {
    CHook::Redirect(g_libGTASA + 0x003B0E6C, &RegisterReference_hook);
    CHook::Redirect(g_libGTASA + 0x003B0D8C, &CleanUpOldReference_hook);
    CHook::Redirect(g_libGTASA + 0x003B0DD8, &ResolveReferences_hook);
    CHook::Redirect(g_libGTASA + 0x003B0E28, &PruneReferences_hook);
}