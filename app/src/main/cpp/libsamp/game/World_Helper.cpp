//
// Created by plaka on 04.05.2023.
//

#include "World.h"
#include "util/patch.h"

// inlined
CPlayerInfoGta& FindPlayerInfo(int32 playerId) {
    return CWorld::Players[playerId < 0 ? CWorld::PlayerInFocus : playerId];
}

// Returns player ped
// 0x56E210
CPlayerPedGta* FindPlayerPed(int32 playerId) {
    return FindPlayerInfo(playerId).m_pPed;
}

// Returns player vehicle
// 0x56E0D0
CVehicleGta* FindPlayerVehicle(int32 playerId, bool bIncludeRemote) {
    CPlayerPedGta* player = FindPlayerPed(playerId);
    if (!player || !player->bInVehicle)
        return nullptr;

    return player->pVehicle;
}
