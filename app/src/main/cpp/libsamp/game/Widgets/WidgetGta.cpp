//
// Created by plaka on 03.02.2023.
//

#include "WidgetGta.h"
#include "main.h"
#include "game/game.h"
#include "net/netgame.h"
#include "util/patch.h"
#include "java_systems/CHUD.h"
#include "WidgetRegionLook.h"
#include "java_systems/SelectEntity.h"

uintptr_t *CWidgetGta::pWidgets = nullptr; // int CTouchInterface::m_pWidgets[]
extern CNetGame *pNetGame;
extern CGame *pGame;

void (*CWidgetButton__Draw)(int thiz);

void CWidgetButton__Draw_hook(int thiz) {
//    if (thiz == CWidgetGta::pWidgets[WIDGET_POSITION_HORN]) {
//        return;
//    }

    CWidgetButton__Draw(thiz);
}

void CWidgetGta::setEnabled(int thiz, bool bEnabled) {
    *(BYTE *) (thiz + 0x4D) = bEnabled; // this->m_bEnabled
}

bool (*CWidget__IsTouched)(uintptr_t *thiz, CVector2D *pVecOut);

bool CWidget__IsTouched_hook(uintptr_t *thiz, CVector2D *pVecOut) {
//    if(*thiz == CWidgetGta::pWidgets[WIDGET_POSITION_HORN]) {
//        return true;
//    }
//    if(!CHUD::bIsShow)
//        return false;

    return CWidget__IsTouched(thiz, pVecOut);
}

void (*CWidgetButton__Update)(int result, int a2, int a3, int a4);

void CWidgetButton__Update_hook(int result, int a2, int a3, int a4) {
    if (!result) {
        return;
    }

    //  CWidgetGta::setEnabled(CWidgetGta::pWidgets[WIDGET_POSITION_ENTER_CAR], false);

    if (pNetGame && pNetGame->m_GreenZoneState) {
        CWidgetGta::setEnabled(CWidgetGta::pWidgets[WIDGET_POSITION_CAR_SHOOT], false);
    }
    CWidgetButton__Update(result, a2, a3, a4);
}

void (*CWidgetRegionLook__Update)(CWidgetRegionLook *thiz);

void CWidgetRegionLook__Update_hook(CWidgetRegionLook *thiz) {
    CWidgetRegionLook__Update(thiz);

    if (thiz->m_bLookBack) {
        thiz->m_bLookBack = false;

        SelectEntity::findSelectEntity();
    }
}

void (*CWidgetButtonEnterCar__Draw)(uintptr_t *thiz);
void CWidgetButtonEnterCar__Draw_hook(uintptr_t *thiz) {


    if(!CHUD::bIsShowEnterExitButt)
        return;

    CWidgetButtonEnterCar__Draw(thiz);
}

void CWidgetGta::init() {
    CWidgetGta::pWidgets = (uintptr_t *) (g_libGTASA + 0x00657E48);

    CHook::InlineHook(g_libGTASA, 0x00281398, &CWidgetRegionLook__Update_hook,&CWidgetRegionLook__Update);
  //  CHook::InlineHook(g_libGTASA, 0x00274218, &CWidget__IsTouched_hook, &CWidget__IsTouched);
    CHook::InlineHook(g_libGTASA, 0x00276510, &CWidgetButtonEnterCar__Draw_hook,&CWidgetButtonEnterCar__Draw);
    CHook::InlineHook(g_libGTASA, 0x00274AB4, &CWidgetButton__Update_hook, &CWidgetButton__Update);
   // CHook::SetUpHook(g_libGTASA + 0x00274748, (uintptr_t) CWidgetButton__Draw_hook,(uintptr_t *) &CWidgetButton__Draw);
}


