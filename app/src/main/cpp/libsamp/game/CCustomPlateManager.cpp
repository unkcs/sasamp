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
	if(type == NUMBERPLATE_TYPE_NONE) {
		return nullptr;
	}
	CFontInstance* font;
	switch(type) {
		case NUMBERPLATE_TYPE_UA: {
			font = m_pUAFont;
			break;
		}
		case NUMBERPLATE_TYPE_KZ: {
			font = m_pKZFont;
			break;
		}
		default: {
			font = m_pRUFont;
			break;
		}
	}

	CSprite2d* sprite;
	switch (type) {
		case NUMBERPLATE_TYPE_KZ: {
			sprite = m_pKzSprite;
			break;
		}
		case NUMBERPLATE_TYPE_BY: {
			sprite = m_pBuSprite;
			break;
		}
		case NUMBERPLATE_TYPE_UA: {
			sprite = m_pUaSprite;
			break;
		}
		default: {
			sprite = m_pRuSprite;
			break;
		}
	}
	memset(m_pBitmap, 0, PLATE_BITMAP_HEIGHT * PLATE_BITMAP_WIDTH);

	int x_max, y_max;
	CFontRenderer::RenderText(szNumber, font, m_pBitmap, PLATE_BITMAP_WIDTH, x_max, y_max);

	for (int i = 0; i < PLATE_BITMAP_WIDTH * PLATE_BITMAP_HEIGHT; i++)
	{
		m_pBitmap[i] = 255 - m_pBitmap[i];
	}

	RwRaster* pTextRaster = GetRWRasterFromBitmap(m_pBitmap + (PLATE_BITMAP_WIDTH * 0), PLATE_BITMAP_WIDTH, 256, 64);

	if (!pTextRaster)
	{
		return nullptr;
	}

	auto pText = new CSprite2d();

	pText->m_pRwTexture = ((struct RwTexture* (*)(struct RwRaster*))(g_libGTASA + 0x1B1B4C + 1))(pTextRaster); // RwTextureCreate

	CRGBA white;
	white.a = 255;
	white.r = 255;
	white.g = 255;
	white.b = 255;

	m_pRenderTarget->Begin();
	sprite->Draw(0.0f, 0.0f, 256.0f, 64.0f, white);
	pText->Draw(36.0f, 8.0f, 210.0f, 42.0f, white);
	RwTexture* pTexture = m_pRenderTarget->End();

	delete pText;

	return pTexture;
}

void CCustomPlateManager::Initialise()
{
	char path[0xFF];
	sprintf(path, "%sSAMP/plates/ru_font.ttf", g_pszStorage);

	m_pRUFont = CFontRenderer::AddFont(path, 128);
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
