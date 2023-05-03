//
// Created by plaka on 01.05.2023.
//

#include "TouchInterface.h"
#include "game/RW/RenderWare.h"
#include "game/Enums/OSDeviceForm.h"
#include "util/patch.h"

void CTouchInterface::LoadDefaultConfigFile()
{
    int v0; // r5
    int v1; // r6

    v0 = 2;
    v1 = 0;
    if ( (RsGlobal->maximumWidth / RsGlobal->maximumHeight) >= 1.5 )
    {
        v0 = 3;
        v1 = 1;
    }
    const char* v5[] = {"TouchDefaultTablet4x3.cfg",
                        "TouchDefaultTabletWidescreen.cfg",
                        "TouchDefaultPhone3x2.cfg",
                        "TouchDefaultPhoneWidescreen.cfg",
                        "360Default1280x720.cfg",
                        "360Default960x720.cfg"
    };

    if ( CUtil::OS_SystemForm() == OSDF_Phone )
        v1 = v0;

    CTouchInterface::LoadFromFile(v5[v1], true);
}

void __fastcall CTouchInterface::LoadFromFile(const char *pszFilename, bool bFullLoad) {
    CHook::CallFunction<void>(g_libGTASA + 0x0026DF20 + 1, pszFilename, bFullLoad);
}

void CTouchInterface::InjectHooks() {
    CHook::Redirect(g_libGTASA + 0x0026E228, &CTouchInterface::LoadDefaultConfigFile);
}
