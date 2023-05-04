#include "References.h"
#include "World.h"
#include "util/patch.h"

void CReferences::InjectHooks() {
    CHook::Write(g_libGTASA + 0x005CEB28, &CReferences::pEmptyList);
    CHook::Write(g_libGTASA + 0x005D0F28, &CReferences::aRefs);
}

// 0x5719B0
void CReferences::Init() {
    pEmptyList = aRefs;

    // todo: do better
    for (int32 i = 0; i < MAX_NUM_REFERENCES - 1; ++i) {
        (&aRefs[i])->m_pNext = &aRefs[i + 1];
        (&aRefs[i])->m_ppEntity = nullptr;
    }

    (&aRefs[MAX_NUM_REFERENCES - 1])->m_pNext = nullptr;
}

uint32 CReferences::ListSize(CReference* ref) {
    uint32 count = 0;
    while (ref) {
        ref = ref->m_pNext;
        ++count;
    }

    return count;
}

void CReferences::RemoveReferencesToPlayer() {
    auto vehicle = FindPlayerVehicle();
    if (vehicle)
        vehicle->ResolveReferences();

    auto player = FindPlayerPed();
    if (player) {
        player->ResolveReferences();
//        auto& group = CPedGroups::GetGroup(player->m_pPlayerData->m_nPlayerGroup);
//        group.GetMembership().SetLeader(player);
//        group.Process();
    }
}

