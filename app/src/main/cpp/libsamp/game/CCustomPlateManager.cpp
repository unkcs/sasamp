#include "CCustomPlateManager.h"

#include "../main.h"
#include "RW/RenderWare.h"
#include "game.h"
#include "..//net/netgame.h"
#include "..//chatwindow.h"
#include "util/util.h"
#include "imgui/imgui.h"
#include "gui/gui.h"

extern CNetGame* pNetGame;
extern CGUI* pGUI;

CRenderTarget* CCustomPlateManager::m_pRenderTarget = nullptr;

struct CSprite2d* CCustomPlateManager::m_pRuSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pUaSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pBuSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pKzSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pRuPoliceSprite = nullptr;

RwTexture* CCustomPlateManager::pNoPlateTex = nullptr;

uint8_t* CCustomPlateManager::m_pBitmap = nullptr;

void ImGui_ImplRenderWare_RenderDrawData(ImDrawData* draw_data);

RwTexture* CCustomPlateManager::createTexture(ePlateType type, char* szNumber, char* szRegion)
{
//	type = NUMBERPLATE_TYPE_BY;
//    szNumber = "8682 AX-3";
//    szRegion = "01";

	switch(type) {
		case NUMBERPLATE_TYPE_UA: {
			return createUaPlate(szNumber, szRegion);
		}
		case NUMBERPLATE_TYPE_KZ: {
			return createKzPlate(szNumber, szRegion);
		}
		case NUMBERPLATE_TYPE_BY: {
			return createBuPlate(szNumber, szRegion);
		}
		case NUMBERPLATE_TYPE_RUS: {
			return createRuPlate(szNumber, szRegion);
		}
		case NUMBERPLATE_TYPE_RU_POLICE: {
			return createRuPolicePlate(szNumber, szRegion);
		}
		default: {
			return nullptr;
		}
	}

}

void CCustomPlateManager::Initialise()
{
	m_pRenderTarget = new CRenderTarget(256, 64, true);

	m_pUaSprite = new CSprite2d();
	m_pRuSprite = new CSprite2d();
	m_pBuSprite = new CSprite2d();
	m_pKzSprite = new CSprite2d();
	m_pRuPoliceSprite = new CSprite2d();


	m_pUaSprite->m_pRwTexture = CUtil::LoadTextureFromDB("samp", "plate_ua");
	m_pRuSprite->m_pRwTexture = CUtil::LoadTextureFromDB("samp", "plate_ru");
	m_pBuSprite->m_pRwTexture = CUtil::LoadTextureFromDB("samp", "plate_bu");
	m_pKzSprite->m_pRwTexture = CUtil::LoadTextureFromDB("samp", "plate_kz");
	m_pRuPoliceSprite->m_pRwTexture = CUtil::LoadTextureFromDB("samp", "plate_ru_police");

	pNoPlateTex = CUtil::LoadTextureFromDB("txd", "nomer");

	m_pBitmap = new uint8_t[PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH];

	Log("CCustomPlateManager::Initialise ..");
}

void CCustomPlateManager::Shutdown()
{
	if (m_pRenderTarget)
	{
		delete m_pRenderTarget;
		m_pRenderTarget = nullptr;
	}

	if (m_pRuSprite)
	{
		delete m_pRuSprite;
		m_pRuSprite = nullptr;
	}

	if (m_pUaSprite)
	{
		delete m_pUaSprite;
		m_pUaSprite = nullptr;
	}

	if (m_pBuSprite)
	{
		delete m_pBuSprite;
		m_pBuSprite = nullptr;
	}

	if (m_pKzSprite)
	{
		delete m_pKzSprite;
		m_pKzSprite = nullptr;
	}

	if (m_pRuPoliceSprite)
	{
		delete m_pRuPoliceSprite;
		m_pRuPoliceSprite = nullptr;
	}

	if (m_pBitmap)
	{
		delete[] m_pBitmap;
		m_pBitmap = nullptr;
	}

}

RwTexture* CCustomPlateManager::createUaPlate(char* szNumber, char* szRegion)
{
	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pUaSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);

	ImGui::NewFrame();

	// num
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 35.f, ImVec2(41, 10), ImColor(IM_COL32_BLACK), szNumber);

	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());

	RwTexture* pTexture = m_pRenderTarget->End();

	return pTexture;
}

RwTexture* CCustomPlateManager::createBuPlate(char* szNumber, char* szRegion)
{
	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pBuSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);

	ImGui::NewFrame();

	// num
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 40.f, ImVec2(42, 10), ImColor(IM_COL32_BLACK), szNumber);

	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());

	RwTexture* pTexture = m_pRenderTarget->End();

	return pTexture;
}

RwTexture* CCustomPlateManager::createKzPlate(char* szNumber, char* szRegion)
{
	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pKzSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);

	ImGui::NewFrame();

	// num
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 36.f, ImVec2(40, 10), ImColor(IM_COL32_BLACK), szNumber,
											nullptr, 160.f);

	// region
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 35.f, ImVec2(201, 10), ImColor(IM_COL32_BLACK), szRegion);

	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());

	RwTexture* pTexture = m_pRenderTarget->End();

	return pTexture;
}

RwTexture* CCustomPlateManager::createRuPolicePlate(char* szNumber, char* szRegion)
{
	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pRuPoliceSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);

	ImGui::NewFrame();

	// num
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 45.f, ImVec2(25, 8), ImColor(IM_COL32_WHITE), szNumber,
											nullptr, 160.f);

	// region
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 28.f, ImVec2(207, 9), ImColor(IM_COL32_WHITE), szRegion,
											nullptr, 160.f);

	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());

	RwTexture* pTexture = m_pRenderTarget->End();

	return pTexture;
}

RwTexture* CCustomPlateManager::createRuPlate(char* szNumber, char* szRegion)
{
	char utfNum[50];
	cp1251_to_utf8(utfNum, szNumber);
	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pRuSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);

	ImGui::NewFrame();

	// num
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 40.f, ImVec2(21, 8), ImColor(IM_COL32_BLACK), utfNum);

	// region
	ImGui::GetBackgroundDrawList()->AddText(nullptr, 28.f, ImVec2(207, 10), ImColor(IM_COL32_BLACK), szRegion);


	ImGui::Render();
	ImGui_ImplRenderWare_RenderDrawData(ImGui::GetDrawData());

	RwTexture* pTexture = m_pRenderTarget->End();

	return pTexture;

	// y - отступ сверху. h - высота сверху
}

