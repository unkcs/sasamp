#include "../main.h"
#include "RW/RenderWare.h"
#include "game.h"
#include "../gui/gui.h"
#include "../util/CJavaWrapper.h"
#include "java_systems/CHUD.h"

extern CGame *pGame;
extern CGUI* pGUI;

RwTexture* splashTexture = nullptr;

void LoadSplashTexture()
{
	Log("Loading hud bg texture..");
	splashTexture = CUtil::LoadTextureFromDB("txd", "hud_bg");

	for(int i = 0; i < std::size(CVehicle::m_pVinyls); i++) {
		char name[55];
		sprintf(name, "remapbody%d", i + 1);
		CVehicle::m_pVinyls[i] = CUtil::LoadTextureFromDB("gta3", name);
	}

	CVehicle::pNeonTex = CUtil::LoadTextureFromDB("samp", "neonaper3");
}

void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);
void ImGui_ImplRenderWare_NewFrame();

void RenderBackgroundHud()
{
	if (CHUD::bIsShow)
	{

		if (splashTexture)
		{//183 55.. 399..33
			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)splashTexture->raster,
													 ImVec2(CHUD::radarBgPos1.x, CHUD::radarBgPos1.y),
													 ImVec2(CHUD::radarBgPos2.x, CHUD::radarBgPos2.y),
													 ImVec2(0, 0), ImVec2(1, 1));
		}
	}
}