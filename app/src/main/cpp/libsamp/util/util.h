#pragma once

#include "game/Entity/Vehicle.h"
#include "game/RW/rwcore.h"
#include "game/Enums/OSDeviceForm.h"

class CVehicleGta;
class CPedGta;

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

    // Converts degrees to radians
    // keywords: 0.017453292 flt_8595EC
    constexpr static float DegreesToRadians(float angleInDegrees) {
        return angleInDegrees * PI / 180.0F;
    }

    // Converts radians to degrees
    // 57.295826
    constexpr static float RadiansToDegrees(float angleInRadians) {
        return angleInRadians * 180.0F / PI;
    }

    static uintptr_t FindLibrary(const char *library);

    static float GetDistanceBetween3DPoints(const RwV3d f, const RwV3d s);

    static OSDeviceForm OS_SystemForm();
};

void cp1251_to_utf8(char *out, const char *in, unsigned int len = 0);

void AND_OpenLink(const char* szLink);
char* ConvertColorToHtml(std::string format);