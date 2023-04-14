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
	splashTexture = (RwTexture*)LoadTextureFromDB("txd", "hud_bg");
}

void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);
void ImGui_ImplRenderWare_NewFrame();

void RenderBackgroundHud()
{

	ImGui_ImplRenderWare_NewFrame();
	ImGui::NewFrame();
		
	if (CHUD::bIsShow)
	{

		if (splashTexture)
		{//183 55.. 399..33
			ImGui::GetBackgroundDrawList()->AddImage((ImTextureID)splashTexture->raster,
													 ImVec2(CHUD::radarBgPos1.X, CHUD::radarBgPos1.Y),
													 ImVec2(CHUD::radarBgPos2.X, CHUD::radarBgPos2.Y),
													 ImVec2(0, 0), ImVec2(1, 1));
		}
	}

	ImGui::EndFrame();
	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());
}