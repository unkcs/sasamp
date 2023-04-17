#include "CCustomPlateManager.h"

#include "../main.h"
#include "RW/RenderWare.h"
#include "game.h"
#include "..//net/netgame.h"
#include "..//chatwindow.h"

extern CNetGame* pNetGame;

CRenderTarget* CCustomPlateManager::m_pRenderTarget = nullptr;

struct CSprite2d* CCustomPlateManager::m_pRuSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pUaSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pBuSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pKzSprite = nullptr;
struct CSprite2d* CCustomPlateManager::m_pRuPoliceSprite = nullptr;

RwTexture* CCustomPlateManager::pNoPlateTex = nullptr;

CFontInstance* CCustomPlateManager::m_pRUFont = nullptr;
CFontInstance* CCustomPlateManager::m_pKZFont = nullptr;
CFontInstance* CCustomPlateManager::m_pUAFont = nullptr;

uint8_t* CCustomPlateManager::m_pBitmap = nullptr;

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
	char path[0xFF];
	sprintf(path, "%sSAMP/plates/ru_font.ttf", g_pszStorage);

	m_pRUFont = CFontRenderer::AddFont(path, 132);

	m_pKZFont = CFontRenderer::AddFont(path, 132);

	sprintf(path, "%sSAMP/plates/ua_font.ttf", g_pszStorage);
	m_pUAFont = CFontRenderer::AddFont(path, 68);

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
	if (!m_pUaSprite)
	{
		return nullptr;
	}
	if (!m_pUaSprite->m_pRwTexture)
	{
		return nullptr;
	}

	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH); // what kind of u?

	int x_max, y_max;
	CFontRenderer::RenderText(szNumber, m_pUAFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);

	for (int i = 0; i < PLATE_BITMAP_WIDTH * PLATE_BITMAP_HEIGHT; i++)
	{
		m_pBitmap[i] = 255 - m_pBitmap[i];
	}

	RwRaster* pTextRaster = GetRWRasterFromBitmap(m_pBitmap + (PLATE_BITMAP_WIDTH * 0), PLATE_BITMAP_WIDTH, 256, 64);

	if (!pTextRaster)
	{
		return nullptr;
	}

	CSprite2d* pText = new CSprite2d();

	pText->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pTextRaster); // RwTextureCreate

	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pUaSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);
	pText->Draw(28.0f, 8.0f, 210.0f, 50.0f, white);
	RwTexture* pTexture = m_pRenderTarget->End();

	delete pText;

	return pTexture;
}

RwTexture* CCustomPlateManager::createBuPlate(char* szNumber, char* szRegion)
{

	// process text
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);

	int x_max, y_max;
	CFontRenderer::RenderText(szNumber, m_pRUFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);

	for (int i = 0; i < PLATE_BITMAP_WIDTH * PLATE_BITMAP_HEIGHT; i++)
	{
		m_pBitmap[i] = 255 - m_pBitmap[i];
	}
	RwRaster* pTextRaster = GetRWRasterFromBitmap(m_pBitmap, PLATE_BITMAP_WIDTH, 256, 64);

	if (!pTextRaster)
	{
		return nullptr;
	}

	auto pText = new CSprite2d();

	pText->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pTextRaster); // RwTextureCreate

	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pBuSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);
	//pText->Draw(50.0f, 8.0f, 200.0f, 38.0f, white); // x y width height ÒÅÊÑÒ
	pText->Draw(41.0f, 8.0f, 210.0f, 42.0f, white);
	RwTexture* pTexture = m_pRenderTarget->End();

	delete pText;

	return pTexture;
}

RwTexture* CCustomPlateManager::createKzPlate(char* szNumber, char* szRegion)
{
	// process text
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);

	int x_max, y_max;
	CFontRenderer::RenderText(szNumber, m_pKZFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);

	for (int i = 0; i < PLATE_BITMAP_WIDTH * PLATE_BITMAP_HEIGHT; i++)
	{
		m_pBitmap[i] = 255 - m_pBitmap[i];
	}

	RwRaster* pTextRaster = GetRWRasterFromBitmap(m_pBitmap + (PLATE_BITMAP_WIDTH * 30), PLATE_BITMAP_WIDTH, 256, 64);

	// process region code
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);
	CFontRenderer::RenderText(szRegion, m_pRUFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);
	for (int i = 0; i < PLATE_BITMAP_WIDTH * PLATE_BITMAP_HEIGHT; i++)
	{
		m_pBitmap[i] = 255 - m_pBitmap[i];
	}

	RwRaster* pRegionRaster = GetRWRasterFromBitmap(m_pBitmap + (PLATE_BITMAP_WIDTH * 30), PLATE_BITMAP_WIDTH, 128, 64);

	if (!pRegionRaster || !pTextRaster)
	{
		if (pRegionRaster)
		{
			RwRasterDestroy(pRegionRaster);
		}
		if (pTextRaster)
		{
			RwRasterDestroy(pTextRaster);
		}
		return nullptr;
	}

	CSprite2d* pText = new CSprite2d();
	CSprite2d* pRegion = new CSprite2d();

	pText->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pTextRaster); // RwTextureCreate
	pRegion->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pRegionRaster); // RwTextureCreate

	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pKzSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);
	pText->Draw(48.0f, 0, 154.0f, 48.0f, white);
	pRegion->Draw(214.0f, 10.0f, 40.0f, 44.0f, white);
	RwTexture* pTexture = m_pRenderTarget->End();

	delete pText;
	delete pRegion;

	return pTexture;
}

RwTexture* CCustomPlateManager::createRuPolicePlate(char* szNumber, char* szRegion)
{
	// process text
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);

	int x_max, y_max;
	CFontRenderer::RenderText(szNumber, m_pRUFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);

	RwRaster* pTextRaster = GetRWRasterFromBitmapPalette(m_pBitmap + (PLATE_BITMAP_WIDTH * 30), PLATE_BITMAP_WIDTH, 256, 64, 0, 85, 185, 0);

	// process region code
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);
	CFontRenderer::RenderText(szRegion, m_pRUFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);

	RwRaster* pRegionRaster = GetRWRasterFromBitmapPalette(m_pBitmap + (PLATE_BITMAP_WIDTH * 30), PLATE_BITMAP_WIDTH, 128, 64, 0, 85, 185, 0);

	if (!pRegionRaster || !pTextRaster)
	{
		if (pRegionRaster)
		{
			RwRasterDestroy(pRegionRaster);
		}
		if (pTextRaster)
		{
			RwRasterDestroy(pTextRaster);
		}
		return nullptr;
	}

	auto pText = new CSprite2d();
	auto pRegion = new CSprite2d();


	pText->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pTextRaster); // RwTextureCreate
	pRegion->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pRegionRaster); // RwTextureCreate

	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pRuPoliceSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);
	pText->Draw(22.0f, 12.0f, 160.0f, 44.0f, white);
	pRegion->Draw(206.0f, 14.0f, 38.0f, 24.0f, white);
	RwTexture* pTexture = m_pRenderTarget->End();

	delete pText;
	delete pRegion;

	return pTexture;
}

RwTexture* CCustomPlateManager::createRuPlate(char* szNumber, char* szRegion)
{
	// process text
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);

	int x_max, y_max;
	CFontRenderer::RenderText(szNumber, m_pRUFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);

	for (int i = 0; i < PLATE_BITMAP_WIDTH * PLATE_BITMAP_HEIGHT; i++)
	{
		m_pBitmap[i] = 255 - m_pBitmap[i];
	}

	RwRaster* pTextRaster = GetRWRasterFromBitmap(m_pBitmap + (PLATE_BITMAP_WIDTH * 30), PLATE_BITMAP_WIDTH, 256, 64);

	// process region code
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);
	CFontRenderer::RenderText(szRegion, m_pRUFont, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);
	for (int i = 0; i < PLATE_BITMAP_WIDTH * PLATE_BITMAP_HEIGHT; i++)
	{
		m_pBitmap[i] = 255 - m_pBitmap[i];
	}

	RwRaster* pRegionRaster = GetRWRasterFromBitmap(m_pBitmap + (PLATE_BITMAP_WIDTH * 30), PLATE_BITMAP_WIDTH, 128, 64);

	if (!pRegionRaster || !pTextRaster)
	{
		if (pRegionRaster)
		{
			RwRasterDestroy(pRegionRaster);
		}
		if (pTextRaster)
		{
			RwRasterDestroy(pTextRaster);
		}
		return nullptr;
	}

	CSprite2d* pText = new CSprite2d();
	CSprite2d* pRegion = new CSprite2d();

	pText->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pTextRaster); // RwTextureCreate
	pRegion->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pRegionRaster); // RwTextureCreate

	CRGBA white {255, 255, 255, 255};

	m_pRenderTarget->Begin();
	m_pRuSprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);
	pText->Draw(22.0f, 8.0f, 160.0f, 38.0f, white); // x y width height ÒÅÊÑÒ
	pRegion->Draw(212.0f, 11.0f, 33.0f, 20.0f, white); // x y width height ÐÅÃÈÎÍ
	RwTexture* pTexture = m_pRenderTarget->End();

	delete pText;
	delete pRegion;

	return pTexture;
}
