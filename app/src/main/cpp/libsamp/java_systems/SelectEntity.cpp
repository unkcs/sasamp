//
// Created by plaka on 01.05.2023.
//

#include "SelectEntity.h"
#include "game/Core/Vector.h"
#include "net/netgame.h"
#include "game/Render/Sprite.h"
#include "../game/Widgets/TouchInterface.h"
#include "game/util.h"

void SelectEntity::findSelectEntity() {
    if (!pNetGame) return;

    CVector vecOut;
    PLAYERID selected = INVALID_PLAYER_ID;
    float minDist = 1000.f;

    for (auto pPlayer: pNetGame->GetPlayerPool()->m_pPlayers) {
        if (pPlayer == nullptr || !pPlayer->GetPlayerPed())
            continue;

        CVector vec = pPlayer->GetPlayerPed()->m_pEntity->GetPosition();

        Sprite::CalcScreenCoors(&vec, &vecOut, nullptr, nullptr, false, false);

        float dist = GetDistanceBetween3DPoints(
                {(RwReal) CTouchInterface::lastPosX, (RwReal) CTouchInterface::lastPosY, (RwReal) vecOut.z},
                vecOut
        );
        dist = abs(dist);

        if(minDist > dist && dist < 100) {
            minDist = dist;
            selected = pPlayer->GetID();
        }
    }
    if(selected != INVALID_PLAYER_ID) {

        return;
    }
}

void SelectEntity::onSelectedPed(PLAYERID playerId) {

}
