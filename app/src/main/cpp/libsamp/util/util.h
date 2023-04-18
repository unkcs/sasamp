#pragma once

#include "game/Entity/Vehicle.h"
#include "game/RW/rwcore.h"

static class CUtil {
public:
    static bool IsValidGameVehicle(CVehicleGta *pVehicle);
    static bool IsValidGamePed(CPedGta * pPed);

    static bool IsGameEntityArePlaceable(CEntityGta *pEntity);

    static void WorldRemoveEntity(uintptr_t pEnt);

    static const char *GetWeaponName(int iWeaponID);

    static int32_t FindPlayerSlotWithPedPointer(CPedGta *pPlayersPed);

    static RwTexture* GetTexture(const char* name);

    static RwTexture *LoadTextureFromDB(const char *dbname, const char *texture);
};

uintptr_t FindLibrary(const char* library);
void cp1251_to_utf8(char *out, const char *in, unsigned int len = 0);

void AND_OpenLink(const char* szLink);
char* ConvertColorToHtml(std::string format);