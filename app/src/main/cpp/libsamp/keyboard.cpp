#include "main.h"

#include "keyboardhistory.h"
#include "util/CJavaWrapper.h"
#include "CSettings.h"
#include "gui/gui.h"
#include "game/game.h"
#include "keyboard.h"
#include "vendor/imgui/fontawesome.h"
#include "vendor/imgui/fonts.h"
#include "net/netgame.h"
#include "java_systems/CHUD.h"
#include "java_systems/CAdminRecon.h"
#include "java_systems/casino/CBaccarat.h"
#include "java_systems/CSpeedometr.h"

extern CGUI *pGUI;
extern CGame *pGame;

std::vector<kbKey> CKeyBoard::m_Rows[3][4];
std::string CKeyBoard::m_sInput;
int CKeyBoard::dop_butt;
bool CKeyBoard::m_bEnable;
ImVec2 CKeyBoard::m_Size;
ImVec2 CKeyBoard::m_Pos;
float CKeyBoard::m_fKeySizeY;
float CKeyBoard::m_fKeySizeX;
float CKeyBoard::m_fFontSize;

bool CKeyBoard::m_iPushedKeyUp;
bool CKeyBoard::m_iPushedKeyDown;

ImFont *CKeyBoard::fonticon;
ImFont *CKeyBoard::chatfuncfont;

int CKeyBoard::m_iLayout;
int CKeyBoard::m_iCase;
int CKeyBoard::m_iPushedKey;
int CKeyBoard::chatinputposx;


char CKeyBoard::m_utf8Input[MAX_INPUT_LEN * 3 + 0xF];
int CKeyBoard::m_iInputOffset;
CKeyBoardHistory *CKeyBoard::m_pkHistory;

void CKeyBoard::init()
{
	Log("Initalizing KeyBoard..");

	ImGuiIO &io = ImGui::GetIO();
	m_Size = ImVec2(io.DisplaySize.x, io.DisplaySize.y * 0.55);
	m_Pos = ImVec2(0, io.DisplaySize.y * (1 - 0.55));
	m_fFontSize = pGUI->ScaleY(70.0f);
	m_fKeySizeY = m_Size.y / 5;
	m_fKeySizeX = m_Size.x / 10;

	io.Fonts->AddFontDefault();
	ImFontConfig config;
	config.MergeMode = true;
	config.GlyphMinAdvanceX = 13.0f;
	static const ImWchar icon_ranges[] = {ICON_MIN_FA, ICON_MAX_FA, 0};
	fonticon = io.Fonts->AddFontFromMemoryTTF(iconfont, sizeof(iconfont), 20.0f, &config, icon_ranges);

	chatfuncfont = pGUI->LoadFont("visby-round-cf-extra-bold.ttf", 22);

	Log("Size: %f, %f. Pos: %f, %f", m_Size.x, m_Size.y, m_Pos.x, m_Pos.y);
	Log("font size: %f. Key's height: %f", m_fFontSize, m_fKeySizeY);

	m_bEnable = false;
	m_iLayout = LAYOUT_ENG;
	m_iCase = LOWER_CASE;
	m_iPushedKey = -1;
	chatinputposx = 0;

	dop_butt = -1;

	m_utf8Input[0] = '\0';
	m_iInputOffset = 0;

	m_pkHistory = new CKeyBoardHistory();

	InitENG();
	InitRU();
	InitNUM();

	Log("KeyBoard inited");
}

void CKeyBoard::Render()
{
	if (!m_bEnable)
		return;

	ImGuiIO &io = ImGui::GetIO();
	ImVec2 vecButSize = ImVec2(ImGui::GetFontSize() * 6, ImGui::GetFontSize() * 3.5);

	// background
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_Pos.x, m_Pos.y + m_fKeySizeY), ImVec2(m_Size.x, io.DisplaySize.y), 0xB0000000);

	// background-triangleup
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fKeySizeX * 7.6, m_Pos.y), ImVec2(m_fKeySizeX * 8.6, m_Pos.y + m_fKeySizeY), 0xB0000000);

	// background-triangledown
	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fKeySizeX * 8.8, m_Pos.y), ImVec2(m_fKeySizeX * 9.8, m_Pos.y + m_fKeySizeY), 0xB0000000);

	// triangleup
	ImGui::GetOverlayDrawList()->AddTriangleFilled(
				ImVec2(m_fKeySizeX * 7.6 + 70, m_Pos.y + 100),
				ImVec2(m_fKeySizeX * 7.6 + 115, m_Pos.y + 30),
				ImVec2(m_fKeySizeX * 7.6 + 160, m_Pos.y + 100),
				0xFF8A8886);

	// triangledown
	ImGui::GetOverlayDrawList()->AddTriangleFilled(
				ImVec2(m_fKeySizeX * 8.8 + 70, m_Pos.y + 30),
				ImVec2(m_fKeySizeX * 8.8 + 115, m_Pos.y + 100),
				ImVec2(m_fKeySizeX * 8.8 + 160, m_Pos.y + 30),
				0xFF8A8886);

	// background-inputbar
	//ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fChatPosX, m_fChatPosY + m_fChatSizeY + 50 - 40), ImVec2(m_fChatPosX + m_fChatSizeX + 45, m_fChatPosY + m_fChatSizeY + 125 - 40), 0xB0000000, 10);

	// background-settings
	//ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fChatPosX, m_fChatPosY + m_fChatSizeY + 135 - 40), ImVec2(m_fChatPosX + 70, m_fChatPosY + m_fChatSizeY + 205 - 40), 0xB0000000, 10);

	//ImGui::GetOverlayDrawList()->AddText(fonticon, ImGui::GetFontSize(),
										// ImVec2(m_fChatPosX + 16, m_fChatPosY + m_fChatSizeY + 135 + 27 - 40), 0xFFFFFFFF, ICON_FA_COG);

	// background-me
//	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fChatPosX + 80, m_fChatPosY + m_fChatSizeY + 135 - 40), ImVec2(m_fChatPosX + 200, m_fChatPosY + m_fChatSizeY + 205 - 40), 0xB0000000, 10);
//
//	ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize - pGUI->ScaleY(35.0f),
//										 ImVec2(m_fChatPosX + 80 + 23, m_fChatPosY + m_fChatSizeY + 135 + 17.5 - 40), sME ? 0xFFFFFFFF : 0xFF90908E, "/me");
//
//	// background-try
//	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fChatPosX + 210, m_fChatPosY + m_fChatSizeY + 135 - 40), ImVec2(m_fChatPosX + 330, m_fChatPosY + m_fChatSizeY + 205 - 40), 0xB0000000, 10);
//
//	ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize - pGUI->ScaleY(35.0f),
//										 ImVec2(m_fChatPosX + 210 + 23, m_fChatPosY + m_fChatSizeY + 135 + 17.5 - 40), sTRY ? 0xFFFFFFFF : 0xFF90908E, "/try");
//
//	// background-do
//	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fChatPosX + 340, m_fChatPosY + m_fChatSizeY + 135 - 40), ImVec2(m_fChatPosX + 460, m_fChatPosY + m_fChatSizeY + 205 - 40), 0xB0000000, 10);
//
//	ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize - pGUI->ScaleY(35.0f),
//										 ImVec2(m_fChatPosX + 340 + 26, m_fChatPosY + m_fChatSizeY + 135 + 17.5 - 40), sDO ? 0xFFFFFFFF : 0xFF90908E, "/do");
//
//	// background-gov
//	ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fChatPosX + 470, m_fChatPosY + m_fChatSizeY + 135 - 40), ImVec2(m_fChatPosX + 640, m_fChatPosY + m_fChatSizeY + 205 - 40), 0xB0000000, 10);
//
//	ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize - pGUI->ScaleY(35.0f),
									//	 ImVec2(m_fChatPosX + 470 + 17, m_fChatPosY + m_fChatSizeY + 135 + 17.5 - 40), sGOV ? 0xFFFFFFFF : 0xFF90908E, "/gnews");


	// input string
//	if (IsHidden())
//	{
//		char _utf8DialogInputBuffer[100 * 3 + 1];
//		strcpy(_utf8DialogInputBuffer, m_utf8Input);
//
//		for (int i = 0; i < strlen(_utf8DialogInputBuffer); i++)
//		{
//			if (_utf8DialogInputBuffer[i] == '\0')
//				break;
//			_utf8DialogInputBuffer[i] = '*';
//		}
//		ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize - pGUI->ScaleY(30.0f), ImVec2(m_fChatPosX + m_Size.x * 0.02 - 25, m_fChatPosY + m_fChatSizeY + 50 - 40 + m_Pos.y * 0.05 - 5), 0xFFFFFFFF, _utf8DialogInputBuffer);
//	}
//	else
//		ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize - pGUI->ScaleY(30.0f), ImVec2(m_fChatPosX + m_Size.x * 0.02 - 25 + chatinputposx, m_fChatPosY + m_fChatSizeY + 50 - 40 + m_Pos.y * 0.05 - 5), 0xFFFFFFFF, m_utf8Input);

	// dividing line
	ImGui::GetOverlayDrawList()->AddLine(
		ImVec2(m_Pos.x, m_Pos.y + m_fKeySizeY),
		ImVec2(m_Size.x, m_Pos.y + m_fKeySizeY), 0xFF3291F5);

	float fKeySizeY = m_fKeySizeY;

	for (int i = 0; i < 4; i++)
	{
		for (auto key : m_Rows[m_iLayout][i])
		{
			if (key.id == m_iPushedKey && key.type != KEY_SPACE)
				ImGui::GetOverlayDrawList()->AddRectFilled(
					key.pos,
					ImVec2(key.pos.x + key.width, key.pos.y + fKeySizeY),
					0xFF3291F5);

			if (m_iPushedKeyUp)
			{
				ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fKeySizeX * 7.6, m_Pos.y), ImVec2(m_fKeySizeX * 8.6, m_Pos.y + m_fKeySizeY), 0xFF3291F5);
				ImGui::GetOverlayDrawList()->AddTriangleFilled(
				ImVec2(m_fKeySizeX * 7.6 + 70, m_Pos.y + 100),
				ImVec2(m_fKeySizeX * 7.6 + 115, m_Pos.y + 30),
				ImVec2(m_fKeySizeX * 7.6 + 160, m_Pos.y + 100),
				0xFF8A8886);
			}
			else if (m_iPushedKeyDown)
			{
				ImGui::GetOverlayDrawList()->AddRectFilled(ImVec2(m_fKeySizeX * 8.8, m_Pos.y), ImVec2(m_fKeySizeX * 9.8, m_Pos.y + m_fKeySizeY), 0xFF3291F5);
				ImGui::GetOverlayDrawList()->AddTriangleFilled(
				ImVec2(m_fKeySizeX * 8.8 + 70, m_Pos.y + 30),
				ImVec2(m_fKeySizeX * 8.8 + 115, m_Pos.y + 100),
				ImVec2(m_fKeySizeX * 8.8 + 160, m_Pos.y + 30),
				0xFF8A8886);
			}

			switch (key.type)
			{
			case KEY_DEFAULT:
				ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize, key.symPos, 0xFFFFFFFF, key.name[m_iCase]);
				break;

			case KEY_SHIFT:
				ImGui::GetOverlayDrawList()->AddTriangleFilled(
					ImVec2(key.pos.x + key.width * 0.37, key.pos.y + fKeySizeY * 0.50),
					ImVec2(key.pos.x + key.width * 0.50, key.pos.y + fKeySizeY * 0.25),
					ImVec2(key.pos.x + key.width * 0.63, key.pos.y + fKeySizeY * 0.50),
					m_iCase == LOWER_CASE ? 0xFF8A8886 : 0xFF3291F5);
				ImGui::GetOverlayDrawList()->AddRectFilled(
					ImVec2(key.pos.x + key.width * 0.44, key.pos.y + fKeySizeY * 0.5 - 1),
					ImVec2(key.pos.x + key.width * 0.56, key.pos.y + fKeySizeY * 0.68),
					m_iCase == LOWER_CASE ? 0xFF8A8886 : 0xFF3291F5);
				break;

			case KEY_BACKSPACE:
				static ImVec2 points[5];
				points[0] = ImVec2(key.pos.x + key.width * 0.35, key.pos.y + fKeySizeY * 0.5);
				points[1] = ImVec2(key.pos.x + key.width * 0.45, key.pos.y + fKeySizeY * 0.25);
				points[2] = ImVec2(key.pos.x + key.width * 0.65, key.pos.y + fKeySizeY * 0.25);
				points[3] = ImVec2(key.pos.x + key.width * 0.65, key.pos.y + fKeySizeY * 0.65);
				points[4] = ImVec2(key.pos.x + key.width * 0.45, key.pos.y + fKeySizeY * 0.65);
				ImGui::GetOverlayDrawList()->AddConvexPolyFilled(points, 5, 0xFF8A8886);
				break;

			case KEY_SWITCH:
				ImGui::GetOverlayDrawList()->AddText(pGUI->GetFont(), m_fFontSize, key.symPos, 0xFFFFFFFF, "lang");
				break;

			case KEY_SPACE:
				ImGui::GetOverlayDrawList()->AddRectFilled(
					ImVec2(key.pos.x + key.width * 0.07, key.pos.y + fKeySizeY * 0.3),
					ImVec2(key.pos.x + key.width * 0.93, key.pos.y + fKeySizeY * 0.7),
					key.id == m_iPushedKey ? 0xFF3291F5 : 0xFF8A8886);
				break;

			case KEY_SEND:
				ImGui::GetOverlayDrawList()->AddTriangleFilled(
					ImVec2(key.pos.x + key.width * 0.3, key.pos.y + fKeySizeY * 0.25),
					ImVec2(key.pos.x + key.width * 0.3, key.pos.y + fKeySizeY * 0.75),
					ImVec2(key.pos.x + key.width * 0.7, key.pos.y + fKeySizeY * 0.50),
					0xFF8A8886);
				break;

			case KEY_UP:
				ImGui::GetOverlayDrawList()->AddTriangleFilled(
					ImVec2(io.DisplaySize.x - (vecButSize.x * 3) + 50, io.DisplaySize.y / 2 - vecButSize.y * 1.635 + 100),
					ImVec2(io.DisplaySize.x - (vecButSize.x * 3) + 95, io.DisplaySize.y / 2 - vecButSize.y * 1.635 + 30),
					ImVec2(io.DisplaySize.x - (vecButSize.x * 3) + 140, io.DisplaySize.y / 2 - vecButSize.y * 1.635 + 100),
					0xFF8A8886);
				break;

			case KEY_DOWN:
				ImGui::GetOverlayDrawList()->AddTriangleFilled(
					ImVec2(io.DisplaySize.x - (vecButSize.x * 1.5) + 50, io.DisplaySize.y / 2 - vecButSize.y * 1.635 + 30),
					ImVec2(io.DisplaySize.x - (vecButSize.x * 1.5) + 95, io.DisplaySize.y / 2 - vecButSize.y * 1.635 + 100),
					ImVec2(io.DisplaySize.x - (vecButSize.x * 1.5) + 140, io.DisplaySize.y / 2 - vecButSize.y * 1.635 + 30),
					0xFF8A8886);
				break;
			}
		}
	}
}

void CKeyBoard::Open()
{
	Close();
	if (m_pkHistory)
	{
		m_pkHistory->m_iCounter = 0;
	}
	m_bEnable = true;

	//g_pJavaWrapper->HideVoice();

	if(pNetGame->m_GreenZoneState) CHUD::toggleGreenZone(false);
	if(CAdminRecon::bIsToggle) CAdminRecon::tempToggle(false);
	if(pGame->isCasinoDiceActive)g_pJavaWrapper->TempToggleCasinoDice(false);
	if(CBaccarat::bIsShow) CBaccarat::tempToggle(false);
	CHUD::toggleServerLogo(false);

	//if(CSpeedometr::bIsShow) CSpeedometr::tempToggle(false);

	if (pGame->m_bRaceCheckpointsEnabled)
	{
		CHUD::toggleGps(false);
	}

}

void CKeyBoard::Close()
{
	if(!m_bEnable)return;

	m_bEnable = false;

	m_sInput.clear();
	CHUD::SetChatInput(m_sInput.c_str());
	m_iInputOffset = 0;
	m_utf8Input[0] = 0;
	m_iCase = LOWER_CASE;
	m_iPushedKey = -1;

	CHUD::ToggleChatInput(false);

	CHUD::toggleServerLogo(true);
	if (pGame->m_bRaceCheckpointsEnabled)
	{
		CHUD::toggleGps(true);
	}
	if(pGame->isCasinoDiceActive)g_pJavaWrapper->TempToggleCasinoDice(true);
	if(CBaccarat::bIsShow) CBaccarat::tempToggle(true);
	if(CAdminRecon::bIsToggle) CAdminRecon::tempToggle(true);
	if(pNetGame->m_GreenZoneState) CHUD::toggleGreenZone(true);
	//g_pJavaWrapper->ShowVoice();
}
#include "util/CJavaWrapper.h"
#include "chatwindow.h"
#include "CDebugInfo.h"
#include "java_systems/CTab.h"
#include "java_systems/CDuelsGui.h"
#include "util/patch.h"
#include "game/CVector.h"
#include "java_systems/CStyling.h"
#include "game/Models/ModelInfo.h"
#include "game/cHandlingDataMgr.h"

bool CKeyBoard::OnTouchEvent(int type, bool multi, int x, int y)
{
	static bool bWannaClose = false;

	if (!m_bEnable)
		return true;

	ImGuiIO &io = ImGui::GetIO();

	if (x >= io.DisplaySize.x - ImGui::GetFontSize() * 4 && y >= io.DisplaySize.y / 2 - (ImGui::GetFontSize() * 2.5) * 3 && y <= io.DisplaySize.y / 2) // keys
	{
		return true;
	}


	static bool bWannaCopy = false;
	static uint32_t uiTouchTick = 0;

	//ImVec2 leftCorner(m_fChatPosX, m_fChatPosY + m_fChatSizeY + 50 - 40);
	//ImVec2 rightCorner(m_fChatPosX + m_fChatSizeX / 1.35, m_fChatPosY + m_fChatSizeY + 125 - 40);

//	if (g_pJavaWrapper)
//	{
//		if (type == TOUCH_PUSH && x >= leftCorner.x && y >= leftCorner.y && x <= rightCorner.x && y <= rightCorner.y)
//		{
//			if (bWannaCopy && GetTickCount() - uiTouchTick <= 150)
//			{
//				std::string msg = g_pJavaWrapper->GetClipboardString();
//				for (int i = 0; i < msg.size(); i++)
//				{
//					AddCharToInput((char)msg[i]);
//				}
//				bWannaCopy = false;
//			}
//			else
//			{
//				bWannaCopy = true;
//				uiTouchTick = GetTickCount();
//			}
//		}
//
//		if (type == TOUCH_POP)
//		{
//			if (GetTickCount() - uiTouchTick <= 150 && bWannaCopy)
//			{
//				bWannaCopy = true;
//				uiTouchTick = GetTickCount();
//			}
//			else
//			{
//				bWannaCopy = false;
//			}
//		}
//	}

//	ImVec2 leftCornersettings(m_fChatPosX, m_fChatPosY + m_fChatSizeY + 135 - 40);
//	ImVec2 rightCornersettings(m_fChatPosX + 70, m_fChatPosY + m_fChatSizeY + 205 - 40);
//
//	if (type == TOUCH_POP && x >= leftCornersettings.x && y >= leftCornersettings.y && x <= rightCornersettings.x && y <= rightCornersettings.y)
//	{
//		g_pJavaWrapper->ShowClientSettings();
//	}
//
//	ImVec2 leftCornerME(m_fChatPosX + 80, m_fChatPosY + m_fChatSizeY + 135 - 40);
//	ImVec2 rightCornerME(m_fChatPosX + 200, m_fChatPosY + m_fChatSizeY + 205 - 40);
//
//	if (type == TOUCH_POP && x >= leftCornerME.x && y >= leftCornerME.y && x <= rightCornerME.x && y <= rightCornerME.y)
//	{
//		sME = !sME;
//		sTRY = false;
//		sDO = false;
//		sGOV = false;
//	}
//
//	ImVec2 leftCornerTRY(m_fChatPosX + 210, m_fChatPosY + m_fChatSizeY + 135 - 40);
//	ImVec2 rightCornerTRY(m_fChatPosX + 330, m_fChatPosY + m_fChatSizeY + 205 - 40);
//
//	if (type == TOUCH_POP && x >= leftCornerTRY.x && y >= leftCornerTRY.y && x <= rightCornerTRY.x && y <= rightCornerTRY.y)
//	{
//		sTRY = !sTRY;
//		sME = false;
//		sDO = false;
//		sGOV = false;
//	}
//
//	ImVec2 leftCornerDO(m_fChatPosX + 340, m_fChatPosY + m_fChatSizeY + 135 - 40);
//	ImVec2 rightCornerDO(m_fChatPosX + 460, m_fChatPosY + m_fChatSizeY + 205 - 40);
//
//	if (type == TOUCH_POP && x >= leftCornerDO.x && y >= leftCornerDO.y && x <= rightCornerDO.x && y <= rightCornerDO.y)
//	{
//		sDO = !sDO;
//		sME = false;
//		sTRY = false;
//		sGOV = false;
//	}
//
//	ImVec2 leftCornerGOV(m_fChatPosX + 470, m_fChatPosY + m_fChatSizeY + 135 - 40);
//	ImVec2 rightCornerGOV(m_fChatPosX + 590, m_fChatPosY + m_fChatSizeY + 205 - 40);
//
//	if (type == TOUCH_POP && x >= leftCornerGOV.x && y >= leftCornerGOV.y && x <= rightCornerGOV.x && y <= rightCornerGOV.y)
//	{
//		sGOV = !sGOV;
//		sME = false;
//		sTRY = false;
//		sDO = false;
//	}
//
//	if (type == TOUCH_PUSH && y < m_Pos.y)
//	{
//		bWannaClose = true;
//	}
//	if (type == TOUCH_POP && y < m_fChatPosY + m_fChatSizeY + 50 - 40 && bWannaClose)
//	{
//		bWannaClose = false;
//		Close();
//		return false;
//	}

	m_iPushedKey = -1;
	m_iPushedKeyUp = false;
	m_iPushedKeyDown = false;

	ImVec2 leftCornertriangleup(m_fKeySizeX * 7.6, m_Pos.y);
	ImVec2 rightCornertriangleup(m_fKeySizeX * 8.6, m_Pos.y + m_fKeySizeY);

	if ((type == TOUCH_PUSH || type == TOUCH_MOVE) && x >= leftCornertriangleup.x && y >= leftCornertriangleup.y && x <= rightCornertriangleup.x && y <= rightCornertriangleup.y)
	{
		m_iPushedKeyUp = true;
		if (type == TOUCH_PUSH)
		{
			m_pkHistory->PageUp();
		}
	}

	ImVec2 leftCornertriangledown(m_fKeySizeX * 8.8, m_Pos.y);
	ImVec2 rightCornertriangledown(m_fKeySizeX * 9.8, m_Pos.y + m_fKeySizeY);

	if ((type == TOUCH_PUSH || type == TOUCH_MOVE) && x >= leftCornertriangledown.x && y >= leftCornertriangledown.y && x <= rightCornertriangledown.x && y <= rightCornertriangledown.y)
	{
		m_iPushedKeyDown = true;
		if (type == TOUCH_PUSH)
		{
			m_pkHistory->PageDown();
		}
	}

	kbKey *key = GetKeyFromPos(x, y);
	// Log("CKeyBoard::OnTouchEvent(%d, %d, %d, %d)", type, multi, x, y);
	if (!key)
		return false;

	switch (type)
	{
	case TOUCH_PUSH:
		m_iPushedKey = key->id;
		break;

	case TOUCH_MOVE:
		m_iPushedKey = key->id;
		break;

	case TOUCH_POP:
		HandleInput(*key);
		break;
	}
	delete key;
	return false;
}

void CKeyBoard::HandleInput(kbKey &key)
{
	switch (key.type)
	{
	case KEY_DEFAULT:
	case KEY_SPACE:
		AddCharToInput(key.code[m_iCase]);
		break;

	case KEY_SWITCH:
		m_iLayout++;
		if (m_iLayout >= 3)
			m_iLayout = 0;
		m_iCase = LOWER_CASE;
		break;

	case KEY_BACKSPACE:
		DeleteCharFromInput();
		break;

	case KEY_SHIFT:
		m_iCase ^= 1;
		break;

	case KEY_SEND:
		Send();
		break;
	case KEY_UP:
		m_pkHistory->PageUp();
		break;
	case KEY_DOWN:
		m_pkHistory->PageDown();
		break;
	}
}

void CKeyBoard::AddCharToInput(char sym)
{
	if (m_sInput.length() < MAX_INPUT_LEN && sym)
	{
		m_sInput.push_back(sym);
		cp1251_to_utf8(m_utf8Input, &m_sInput.c_str()[m_iInputOffset]);

		CHUD::SetChatInput(m_sInput.c_str());

	}
}

void CKeyBoard::DeleteCharFromInput()
{
	if (!m_sInput.length())
		return;

//	if(CChatWindow::cursorStart != CChatWindow::cursorEnd){
//	//	for(int i = CChatWindow::cursorStart; i < CChatWindow::cursorEnd; i++){
//			m_sInput.erase(CChatWindow::cursorStart, CChatWindow::cursorEnd-CChatWindow::cursorStart);
//		//}
//	} else{
//		m_sInput.erase(CChatWindow::cursorEnd, 1);
//	}

	m_sInput.pop_back();

	CHUD::SetChatInput(m_sInput.c_str());

}
extern bool ProcessLocalCommands(const char str[]);

void CKeyBoard::Send()
{
	if(!m_sInput.empty()) {

		switch (dop_butt) {
			case 0:{
				m_sInput = "/me " + m_sInput;
				break;
			}
			case 1:{
				m_sInput = "/do " + m_sInput;
				break;
			}
			case 2:{
				m_sInput = "/try " + m_sInput;
				break;
			}
		}

		if (m_pkHistory) m_pkHistory->AddStringToHistory(m_sInput);

		if (m_sInput[0] == '/') {
			if (!ProcessLocalCommands(m_sInput.c_str())) {
				pNetGame->SendChatCommand(m_sInput.c_str());
			}
		} else {
			pNetGame->SendChatMessage(m_sInput.c_str());
		}
	}

	CKeyBoard::Close();
}

kbKey *CKeyBoard::GetKeyFromPos(int x, int y)
{
	int iRow = (y - m_Pos.y) / m_fKeySizeY;

	if (iRow <= 0)
		return nullptr;

	for (auto key : m_Rows[m_iLayout][iRow - 1])
	{
		if (x >= key.pos.x && x <= (key.pos.x + key.width))
		{
			kbKey *pKey = new kbKey;
			memcpy((void *)pKey, (const void *)&key, sizeof(kbKey));
			return pKey;
		}
	}

	Log("UNKNOWN KEY");
	return nullptr;
}

void CKeyBoard::InitENG()
{
	Log(__FUNCTION__);
	ImVec2 curPos;
	std::vector<kbKey> *row = nullptr;
	std::vector<kbKey>::iterator it;

	float defWidth = m_Size.x / 10;
	struct kbKey key;
	key.type = KEY_HISTORY;
	key.id = 0;

	// 1-�� ���
	row = &m_Rows[LAYOUT_ENG][0];
	curPos = ImVec2(0, m_Pos.y + m_fKeySizeY);

	key.type = KEY_DEFAULT;
	// q/Q
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'q';
	key.code[UPPER_CASE] = 'Q';
	cp1251_to_utf8(key.name[LOWER_CASE], "q");
	cp1251_to_utf8(key.name[UPPER_CASE], "Q");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// w/W
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'w';
	key.code[UPPER_CASE] = 'W';
	cp1251_to_utf8(key.name[LOWER_CASE], "w");
	cp1251_to_utf8(key.name[UPPER_CASE], "W");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// e/E
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'e';
	key.code[UPPER_CASE] = 'E';
	cp1251_to_utf8(key.name[LOWER_CASE], "e");
	cp1251_to_utf8(key.name[UPPER_CASE], "E");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// r/R
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'r';
	key.code[UPPER_CASE] = 'R';
	cp1251_to_utf8(key.name[LOWER_CASE], "r");
	cp1251_to_utf8(key.name[UPPER_CASE], "R");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// t/T
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 't';
	key.code[UPPER_CASE] = 'T';
	cp1251_to_utf8(key.name[LOWER_CASE], "t");
	cp1251_to_utf8(key.name[UPPER_CASE], "T");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// y/Y
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'y';
	key.code[UPPER_CASE] = 'Y';
	cp1251_to_utf8(key.name[LOWER_CASE], "y");
	cp1251_to_utf8(key.name[UPPER_CASE], "Y");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// u/U
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'u';
	key.code[UPPER_CASE] = 'U';
	cp1251_to_utf8(key.name[LOWER_CASE], "u");
	cp1251_to_utf8(key.name[UPPER_CASE], "U");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// i/I
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'i';
	key.code[UPPER_CASE] = 'I';
	cp1251_to_utf8(key.name[LOWER_CASE], "i");
	cp1251_to_utf8(key.name[UPPER_CASE], "I");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// o/O
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'o';
	key.code[UPPER_CASE] = 'O';
	cp1251_to_utf8(key.name[LOWER_CASE], "o");
	cp1251_to_utf8(key.name[UPPER_CASE], "O");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// p/P
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'p';
	key.code[UPPER_CASE] = 'P';
	cp1251_to_utf8(key.name[LOWER_CASE], "p");
	cp1251_to_utf8(key.name[UPPER_CASE], "P");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 2-� ���
	row = &m_Rows[LAYOUT_ENG][1];
	curPos.x = defWidth * 0.5;
	curPos.y += m_fKeySizeY;

	// a/A
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'a';
	key.code[UPPER_CASE] = 'A';
	cp1251_to_utf8(key.name[LOWER_CASE], "a");
	cp1251_to_utf8(key.name[UPPER_CASE], "A");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// s/S
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 's';
	key.code[UPPER_CASE] = 'S';
	cp1251_to_utf8(key.name[LOWER_CASE], "s");
	cp1251_to_utf8(key.name[UPPER_CASE], "S");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// d/D
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'd';
	key.code[UPPER_CASE] = 'D';
	cp1251_to_utf8(key.name[LOWER_CASE], "d");
	cp1251_to_utf8(key.name[UPPER_CASE], "D");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// f/F
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'f';
	key.code[UPPER_CASE] = 'F';
	cp1251_to_utf8(key.name[LOWER_CASE], "f");
	cp1251_to_utf8(key.name[UPPER_CASE], "F");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// g/G
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'g';
	key.code[UPPER_CASE] = 'G';
	cp1251_to_utf8(key.name[LOWER_CASE], "g");
	cp1251_to_utf8(key.name[UPPER_CASE], "G");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// h/H
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'h';
	key.code[UPPER_CASE] = 'H';
	cp1251_to_utf8(key.name[LOWER_CASE], "h");
	cp1251_to_utf8(key.name[UPPER_CASE], "H");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// j/J
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'j';
	key.code[UPPER_CASE] = 'J';
	cp1251_to_utf8(key.name[LOWER_CASE], "j");
	cp1251_to_utf8(key.name[UPPER_CASE], "J");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// k/K
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'k';
	key.code[UPPER_CASE] = 'K';
	cp1251_to_utf8(key.name[LOWER_CASE], "k");
	cp1251_to_utf8(key.name[UPPER_CASE], "K");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// l/L
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'l';
	key.code[UPPER_CASE] = 'L';
	cp1251_to_utf8(key.name[LOWER_CASE], "l");
	cp1251_to_utf8(key.name[UPPER_CASE], "L");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 3-� ���
	row = &m_Rows[LAYOUT_ENG][2];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	// Shift
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth * 1.5;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SHIFT;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	key.type = KEY_DEFAULT;

	// z/Z
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'z';
	key.code[UPPER_CASE] = 'Z';
	cp1251_to_utf8(key.name[LOWER_CASE], "z");
	cp1251_to_utf8(key.name[UPPER_CASE], "Z");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// x/X
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'x';
	key.code[UPPER_CASE] = 'X';
	cp1251_to_utf8(key.name[LOWER_CASE], "x");
	cp1251_to_utf8(key.name[UPPER_CASE], "X");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// c/C
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'c';
	key.code[UPPER_CASE] = 'C';
	cp1251_to_utf8(key.name[LOWER_CASE], "c");
	cp1251_to_utf8(key.name[UPPER_CASE], "C");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// v/V
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'v';
	key.code[UPPER_CASE] = 'V';
	cp1251_to_utf8(key.name[LOWER_CASE], "v");
	cp1251_to_utf8(key.name[UPPER_CASE], "V");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// b/B
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'b';
	key.code[UPPER_CASE] = 'B';
	cp1251_to_utf8(key.name[LOWER_CASE], "b");
	cp1251_to_utf8(key.name[UPPER_CASE], "B");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// n/N
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'n';
	key.code[UPPER_CASE] = 'N';
	cp1251_to_utf8(key.name[LOWER_CASE], "n");
	cp1251_to_utf8(key.name[UPPER_CASE], "N");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// m/M
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 'm';
	key.code[UPPER_CASE] = 'M';
	cp1251_to_utf8(key.name[LOWER_CASE], "m");
	cp1251_to_utf8(key.name[UPPER_CASE], "M");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// delete
	key.pos = curPos;
	key.symPos = ImVec2(0, 0);
	key.width = defWidth * 1.5;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_BACKSPACE;
	key.id++;
	it = row->begin();
	row->insert(it, key);

	// 4-� ������
	row = &m_Rows[LAYOUT_ENG][3];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	key.type = KEY_DEFAULT;

	// slash (/)
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '/';
	key.code[UPPER_CASE] = '/';
	cp1251_to_utf8(key.name[LOWER_CASE], "/");
	cp1251_to_utf8(key.name[UPPER_CASE], "/");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// comma (,)
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = ',';
	key.code[UPPER_CASE] = ',';
	cp1251_to_utf8(key.name[LOWER_CASE], ",");
	cp1251_to_utf8(key.name[UPPER_CASE], ",");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// switch language
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.2, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SWITCH;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// Space
	key.pos = curPos;
	key.symPos = ImVec2(0, 0);
	key.width = defWidth * 4;
	key.code[LOWER_CASE] = ' ';
	key.code[UPPER_CASE] = ' ';
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SPACE;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	key.type = KEY_DEFAULT;

	// ?
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '?';
	key.code[UPPER_CASE] = '?';
	cp1251_to_utf8(key.name[LOWER_CASE], "?");
	cp1251_to_utf8(key.name[UPPER_CASE], "?");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// !
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '!';
	key.code[UPPER_CASE] = '!';
	cp1251_to_utf8(key.name[LOWER_CASE], "!");
	cp1251_to_utf8(key.name[UPPER_CASE], "!");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// Send
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SEND;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x -= key.width;

	//	// UP
	//	key.pos = ImVec2(curPos.x, curPos.y - m_fKeySizeY * 5);
	//	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	//	key.width = defWidth;
	//	key.code[LOWER_CASE] = 0;
	//	key.code[UPPER_CASE] = 0;
	//	key.name[LOWER_CASE][0] = 0;
	//	key.name[UPPER_CASE][0] = 0;
	//	key.type = KEY_UP;
	//	key.id++;
	//	it = row->begin();
	//	row->insert(it, key);
	//	curPos.x += key.width;
	//
	//	// DOWN
	//	key.pos = ImVec2(curPos.x - defWidth * 1.3, curPos.y - m_fKeySizeY * 5);
	//	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	//	key.width = defWidth;
	//	key.code[LOWER_CASE] = 0;
	//	key.code[UPPER_CASE] = 0;
	//	key.name[LOWER_CASE][0] = 0;
	//	key.name[UPPER_CASE][0] = 0;
	//	key.type = KEY_DOWN;
	//	key.id++;
	//	it = row->begin();
	//	row->insert(it, key);

	return;
}

void CKeyBoard::InitRU()
{
	Log(__FUNCTION__);
	ImVec2 curPos;
	std::vector<kbKey> *row = nullptr;
	std::vector<kbKey>::iterator it;
	float defWidth = m_Size.x / 11;

	struct kbKey key;
	key.type = KEY_DEFAULT;
	key.id = 0;

	// 1-�� ���
	row = &m_Rows[LAYOUT_RUS][0];
	curPos = ImVec2(0, m_Pos.y + m_fKeySizeY);

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 2-� ���
	row = &m_Rows[LAYOUT_RUS][1];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 3-� ���
	row = &m_Rows[LAYOUT_RUS][2];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	// Shift
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SHIFT;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	key.type = KEY_DEFAULT;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// �/�
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '�';
	key.code[UPPER_CASE] = '�';
	cp1251_to_utf8(key.name[LOWER_CASE], "�");
	cp1251_to_utf8(key.name[UPPER_CASE], "�");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// backspace
	key.pos = curPos;
	key.symPos = ImVec2(0, 0);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.id++;
	key.type = KEY_BACKSPACE;
	it = row->begin();
	row->insert(it, key);

	// 4-� ������
	row = &m_Rows[LAYOUT_RUS][3];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	key.type = KEY_DEFAULT;
	defWidth = m_Size.x / 10;

	// slash (/)
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '/';
	key.code[UPPER_CASE] = '/';
	cp1251_to_utf8(key.name[LOWER_CASE], "/");
	cp1251_to_utf8(key.name[UPPER_CASE], "/");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// comma (,)
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = ',';
	key.code[UPPER_CASE] = ',';
	cp1251_to_utf8(key.name[LOWER_CASE], ",");
	cp1251_to_utf8(key.name[UPPER_CASE], ",");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// switch language
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.2, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SWITCH;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// Space
	key.pos = curPos;
	key.symPos = ImVec2(0, 0);
	key.width = defWidth * 4;
	key.code[LOWER_CASE] = ' ';
	key.code[UPPER_CASE] = ' ';
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SPACE;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	key.type = KEY_DEFAULT;

	// ?
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '?';
	key.code[UPPER_CASE] = '?';
	cp1251_to_utf8(key.name[LOWER_CASE], "?");
	cp1251_to_utf8(key.name[UPPER_CASE], "?");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// !
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '!';
	key.code[UPPER_CASE] = '!';
	cp1251_to_utf8(key.name[LOWER_CASE], "!");
	cp1251_to_utf8(key.name[UPPER_CASE], "!");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// Send
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SEND;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x -= key.width;

	//	// UP
	//	key.pos = ImVec2(curPos.x, curPos.y - m_fKeySizeY * 5);
	//	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	//	key.width = defWidth;
	//	key.code[LOWER_CASE] = 0;
	//	key.code[UPPER_CASE] = 0;
	//	key.name[LOWER_CASE][0] = 0;
	//	key.name[UPPER_CASE][0] = 0;
	//	key.type = KEY_UP;
	//	key.id++;
	//	it = row->begin();
	//	row->insert(it, key);
	//	curPos.x += key.width;
	//
	//	// DOWN
	//	key.pos = ImVec2(curPos.x - defWidth * 1.3, curPos.y - m_fKeySizeY * 5);
	//	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	//	key.width = defWidth;
	//	key.code[LOWER_CASE] = 0;
	//	key.code[UPPER_CASE] = 0;
	//	key.name[LOWER_CASE][0] = 0;
	//	key.name[UPPER_CASE][0] = 0;
	//	key.type = KEY_DOWN;
	//	key.id++;
	//	it = row->begin();
	//	row->insert(it, key);

	return;
}

void CKeyBoard::InitNUM()
{
	Log(__FUNCTION__);
	ImVec2 curPos;
	std::vector<kbKey> *row = nullptr;
	std::vector<kbKey>::iterator it;
	float defWidth = m_Size.x / 10;

	struct kbKey key;
	key.type = KEY_DEFAULT;
	key.id = 0;

	// 1-�� ���
	row = &m_Rows[LAYOUT_NUM][0];
	curPos = ImVec2(0, m_Pos.y + m_fKeySizeY);

	// 1
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '1';
	key.code[UPPER_CASE] = '1';
	cp1251_to_utf8(key.name[LOWER_CASE], "1");
	cp1251_to_utf8(key.name[UPPER_CASE], "1");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 2
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '2';
	key.code[UPPER_CASE] = '2';
	cp1251_to_utf8(key.name[LOWER_CASE], "2");
	cp1251_to_utf8(key.name[UPPER_CASE], "2");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 3
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '3';
	key.code[UPPER_CASE] = '3';
	cp1251_to_utf8(key.name[LOWER_CASE], "3");
	cp1251_to_utf8(key.name[UPPER_CASE], "3");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 4
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '4';
	key.code[UPPER_CASE] = '4';
	cp1251_to_utf8(key.name[LOWER_CASE], "4");
	cp1251_to_utf8(key.name[UPPER_CASE], "4");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 5
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '5';
	key.code[UPPER_CASE] = '5';
	cp1251_to_utf8(key.name[LOWER_CASE], "5");
	cp1251_to_utf8(key.name[UPPER_CASE], "5");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 6
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '6';
	key.code[UPPER_CASE] = '6';
	cp1251_to_utf8(key.name[LOWER_CASE], "6");
	cp1251_to_utf8(key.name[UPPER_CASE], "6");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 7
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '7';
	key.code[UPPER_CASE] = '7';
	cp1251_to_utf8(key.name[LOWER_CASE], "7");
	cp1251_to_utf8(key.name[UPPER_CASE], "7");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 8
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '8';
	key.code[UPPER_CASE] = '8';
	cp1251_to_utf8(key.name[LOWER_CASE], "8");
	cp1251_to_utf8(key.name[UPPER_CASE], "8");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 9
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '9';
	key.code[UPPER_CASE] = '9';
	cp1251_to_utf8(key.name[LOWER_CASE], "9");
	cp1251_to_utf8(key.name[UPPER_CASE], "9");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 0
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '0';
	key.code[UPPER_CASE] = '0';
	cp1251_to_utf8(key.name[LOWER_CASE], "0");
	cp1251_to_utf8(key.name[UPPER_CASE], "0");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 2-� ���
	row = &m_Rows[LAYOUT_NUM][1];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	// @
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '@';
	key.code[UPPER_CASE] = '@';
	cp1251_to_utf8(key.name[LOWER_CASE], "@");
	cp1251_to_utf8(key.name[UPPER_CASE], "@");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// #
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '#';
	key.code[UPPER_CASE] = '#';
	cp1251_to_utf8(key.name[LOWER_CASE], "#");
	cp1251_to_utf8(key.name[UPPER_CASE], "#");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// $
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '$';
	key.code[UPPER_CASE] = '$';
	cp1251_to_utf8(key.name[LOWER_CASE], "$");
	cp1251_to_utf8(key.name[UPPER_CASE], "$");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// %
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '%';
	key.code[UPPER_CASE] = '%';
	cp1251_to_utf8(key.name[LOWER_CASE], "%");
	cp1251_to_utf8(key.name[UPPER_CASE], "%");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// "
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '"';
	key.code[UPPER_CASE] = '"';
	cp1251_to_utf8(key.name[LOWER_CASE], "\"");
	cp1251_to_utf8(key.name[UPPER_CASE], "\"");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// *
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '*';
	key.code[UPPER_CASE] = '*';
	cp1251_to_utf8(key.name[LOWER_CASE], "*");
	cp1251_to_utf8(key.name[UPPER_CASE], "*");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// (
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '(';
	key.code[UPPER_CASE] = '(';
	cp1251_to_utf8(key.name[LOWER_CASE], "(");
	cp1251_to_utf8(key.name[UPPER_CASE], "(");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// )
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = ')';
	key.code[UPPER_CASE] = ')';
	cp1251_to_utf8(key.name[LOWER_CASE], ")");
	cp1251_to_utf8(key.name[UPPER_CASE], ")");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// -
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '-';
	key.code[UPPER_CASE] = '-';
	cp1251_to_utf8(key.name[LOWER_CASE], "-");
	cp1251_to_utf8(key.name[UPPER_CASE], "-");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// _
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '_';
	key.code[UPPER_CASE] = '_';
	cp1251_to_utf8(key.name[LOWER_CASE], "_");
	cp1251_to_utf8(key.name[UPPER_CASE], "_");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// 3-� ���
	row = &m_Rows[LAYOUT_NUM][2];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	// .
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '.';
	key.code[UPPER_CASE] = '.';
	cp1251_to_utf8(key.name[LOWER_CASE], ".");
	cp1251_to_utf8(key.name[UPPER_CASE], ".");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// :
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = ':';
	key.code[UPPER_CASE] = ':';
	cp1251_to_utf8(key.name[LOWER_CASE], ":");
	cp1251_to_utf8(key.name[UPPER_CASE], ":");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// ;
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = ';';
	key.code[UPPER_CASE] = ';';
	cp1251_to_utf8(key.name[LOWER_CASE], ";");
	cp1251_to_utf8(key.name[UPPER_CASE], ";");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// +
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '+';
	key.code[UPPER_CASE] = '+';
	cp1251_to_utf8(key.name[LOWER_CASE], "+");
	cp1251_to_utf8(key.name[UPPER_CASE], "+");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// =
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '=';
	key.code[UPPER_CASE] = '=';
	cp1251_to_utf8(key.name[LOWER_CASE], "=");
	cp1251_to_utf8(key.name[UPPER_CASE], "=");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// <
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '<';
	key.code[UPPER_CASE] = '<';
	cp1251_to_utf8(key.name[LOWER_CASE], "<");
	cp1251_to_utf8(key.name[UPPER_CASE], "<");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// >
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '>';
	key.code[UPPER_CASE] = '>';
	cp1251_to_utf8(key.name[LOWER_CASE], ">");
	cp1251_to_utf8(key.name[UPPER_CASE], ">");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// [
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '[';
	key.code[UPPER_CASE] = '[';
	cp1251_to_utf8(key.name[LOWER_CASE], "[");
	cp1251_to_utf8(key.name[UPPER_CASE], "[");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// ]
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = ']';
	key.code[UPPER_CASE] = ']';
	cp1251_to_utf8(key.name[LOWER_CASE], "]");
	cp1251_to_utf8(key.name[UPPER_CASE], "]");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// delete
	key.pos = curPos;
	key.symPos = ImVec2(0, 0);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_BACKSPACE;
	key.id++;
	it = row->begin();
	row->insert(it, key);

	// 4-� ������
	row = &m_Rows[LAYOUT_NUM][3];
	curPos.x = 0;
	curPos.y += m_fKeySizeY;

	key.type = KEY_DEFAULT;

	// slash (/)
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '/';
	key.code[UPPER_CASE] = '/';
	cp1251_to_utf8(key.name[LOWER_CASE], "/");
	cp1251_to_utf8(key.name[UPPER_CASE], "/");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// comma (,)
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = ',';
	key.code[UPPER_CASE] = ',';
	cp1251_to_utf8(key.name[LOWER_CASE], ",");
	cp1251_to_utf8(key.name[UPPER_CASE], ",");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// switch language
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.2, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SWITCH;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// Space
	key.pos = curPos;
	key.symPos = ImVec2(0, 0);
	key.width = defWidth * 4;
	key.code[LOWER_CASE] = ' ';
	key.code[UPPER_CASE] = ' ';
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SPACE;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	key.type = KEY_DEFAULT;

	// ?
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '?';
	key.code[UPPER_CASE] = '?';
	cp1251_to_utf8(key.name[LOWER_CASE], "?");
	cp1251_to_utf8(key.name[UPPER_CASE], "?");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// !
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = '!';
	key.code[UPPER_CASE] = '!';
	cp1251_to_utf8(key.name[LOWER_CASE], "!");
	cp1251_to_utf8(key.name[UPPER_CASE], "!");
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x += key.width;

	// Send
	key.pos = curPos;
	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	key.width = defWidth;
	key.code[LOWER_CASE] = 0;
	key.code[UPPER_CASE] = 0;
	key.name[LOWER_CASE][0] = 0;
	key.name[UPPER_CASE][0] = 0;
	key.type = KEY_SEND;
	key.id++;
	it = row->begin();
	row->insert(it, key);
	curPos.x -= key.width;

	//	// UP
	//	key.pos = ImVec2(curPos.x, curPos.y - m_fKeySizeY * 5);
	//	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	//	key.width = defWidth;
	//	key.code[LOWER_CASE] = 0;
	//	key.code[UPPER_CASE] = 0;
	//	key.name[LOWER_CASE][0] = 0;
	//	key.name[UPPER_CASE][0] = 0;
	//	key.type = KEY_UP;
	//	key.id++;
	//	it = row->begin();
	//	row->insert(it, key);
	//	curPos.x += key.width;
	//
	//	// DOWN
	//	key.pos = ImVec2(curPos.x - defWidth * 1.3, curPos.y - m_fKeySizeY * 5);
	//	key.symPos = ImVec2(curPos.x + defWidth * 0.4, curPos.y + m_fKeySizeY * 0.2);
	//	key.width = defWidth;
	//	key.code[LOWER_CASE] = 0;
	//	key.code[UPPER_CASE] = 0;
	//	key.name[LOWER_CASE][0] = 0;
	//	key.name[UPPER_CASE][0] = 0;
	//	key.type = KEY_DOWN;
	//	key.id++;
	//	it = row->begin();
	//	row->insert(it, key);
	return;
}

void CKeyBoard::Flush()
{
	if (!m_sInput.length())
		return;

	m_sInput.clear();
	CHUD::SetChatInput(m_sInput.c_str());
	m_iInputOffset = 0;

	memset(m_utf8Input, 0, sizeof(m_utf8Input) - 1);
}

extern void ApplyFPSPatch(uint8_t fps);
bool ProcessLocalCommands(const char str[])
{
	if (strstr(str, "/fontsize "))
	{
		float size = 0;
		if (sscanf(str, "%*s%f", &size) == -1)
		{
			CChatWindow::AddDebugMessage("�����������: /fontsize [scale]");
			return true;
		}
		size = 27 * size;
		CHUD::ChangeChatTextSize( ceil(size) );
		return true;
	}

	if (strstr(str, "/save "))
	{
		std::string msg;
		if (sscanf(str, "%*s%s", &msg) == -1)
		{
			CChatWindow::AddDebugMessage("�����������: /fontsize [scale]");
			return true;
		}

		CPlayerPed *pPlayer = pGame->FindPlayerPed();
		FILE *fileOut;
		uint32_t dwVehicleID;
		float fZAngle = 0.0f;

		char ff[0xFF];
		sprintf(ff, "%sSAMP/savedposition.txt", g_pszStorage);
		fileOut = fopen(ff, "a");
		if(!fileOut)
			CChatWindow::AddDebugMessage("Cant open savedposition.txt");

		if(pPlayer->IsInVehicle())
		{
			//incar savepos
			VEHICLE_TYPE *pVehicle = pPlayer->GetGtaVehicle();
			VEHICLEID m_dwGTAId = GamePool_Vehicle_GetIndex(pVehicle);
			ScriptCommand(&get_car_z_angle, m_dwGTAId, &fZAngle);
			fprintf(fileOut, "%s = %.3f, %.3f, %.3f, %.3f\n",
					msg.c_str(),
					pVehicle->entity.mat->pos.x,
					pVehicle->entity.mat->pos.y,
					pVehicle->entity.mat->pos.z,
					fZAngle
			);
			CChatWindow::AddInfoMessage("-> InCar position saved.");
		}
		else
		{
			//onfoot savepos
			PED_TYPE *pActor = pPlayer->GetGtaActor();
			ScriptCommand(&get_actor_z_angle, pPlayer->m_dwGTAId, &fZAngle);
			fprintf(fileOut, "%s = %.3f, %.3f, %.3f, %.3f\n",
					msg.c_str(),
					pActor->entity.mat->pos.x,
					pActor->entity.mat->pos.y,
					pActor->entity.mat->pos.z,
					fZAngle
			);
			CChatWindow::AddInfoMessage("-> OnFoot position saved.");
		}

		fclose(fileOut);
		return true;
	}
	if (strcmp(str, "/plate") == 0) {
		auto pPed = pNetGame->GetPlayerPool()->GetLocalPlayer()->m_pPlayerPed;
		auto pVeh = pPed->GetCurrentVehicle();

		pVeh->setPlate(NUMBERPLATE_TYPE_UA, "test", "AA");

		return true;
	}
	if (strcmp(str, "/cameditgui") == 0)
	{
		CHUD::bIsCamEditGui = !CHUD::bIsCamEditGui;
		CHUD::toggleAll( CHUD::bIsCamEditGui );
		return true;
	}
	if (strcmp(str, "/load") == 0)
	{
		//char VehicleNames[210][14] = *(char(*)[][])(g_libGTASA + 0x006B06AC);
		Log("%s", *(char*)(g_libGTASA + 0x0060EF78));
		cHandlingDataMgr::LoadHandlingData();
		return true;
	}

	if (strstr(str, "/testveh "))
	{
		int size = 0;
		if (sscanf(str, "%*s%d", &size) == -1)
		{
			CChatWindow::AddDebugMessage("�����������: /fontsize [scale]");
			return true;
		}

		CPlayerPed *pPlayer = pGame->FindPlayerPed();
		CVehicle* pVehicle = pPlayer->GetCurrentVehicle();

		auto dwModelarray = CModelInfo::ms_modelInfoPtrs;
		uint8_t* pModelInfoStart = (uint8_t*)dwModelarray[pVehicle->m_pVehicle->entity.nModelIndex];

		if (!pModelInfoStart)
		{
			return true;
		}

		uintptr_t* m_pVehicleStruct = (uintptr_t*)(pModelInfoStart + 0x74);

		auto m_avDummyPos = (CVector*)(*m_pVehicleStruct + 0x0);

		CVector vec;
		// 0 - front light
		vec.x = m_avDummyPos[size].x;
		vec.y = m_avDummyPos[size].y;
		vec.z = m_avDummyPos[size].z;
		//float x;


//		RwFrame* pWheelLB = ((RwFrame * (*)(uintptr_t, const char*))(g_libGTASA + 0x00335CEC + 1))(pVehicle->m_pVehicle->entity.m_pRwObject, "taillights"); // GetFrameFromname
//
//		RwMatrix mat;
//		memcpy(&mat, (const void*)&(pWheelLB->modelling), sizeof(RwMatrix));
//
//		//CHook::CallFunction<int>(g_libGTASA + 0x338698 + 1, pModelInfoStart, &vec, 1);
//	//	(( int (*)(uint8_t*, CVector*, int32_t))(g_libGTASA + 0x338698 + 1))(pModelInfoStart, &vec, size);

//		CText3DLabelsPool *pLabelsPool = pNetGame->GetLabelPool();
//
//		if(pLabelsPool)
//		{
//			pLabelsPool->CreateTextLabel((int)555, "x", 0xFFFFFFFF,
//										 x, y, z, 30.0f, 0, INVALID_PLAYER_ID, pPlayer->GetCurrentSampVehicleID());
//		}
		CObjectPool* pObjectPool = pNetGame->GetObjectPool();
		CVector vec11;
		vec11.x = vec11.y = vec11.z = 0.0;
		pObjectPool->New(555, 19294, vec11, vec11, 300.0f);

		CObject* pObject = pObjectPool->GetAt(555);
		if (!pObject) return true;
		pObject->AttachToVehicle(pPlayer->GetCurrentSampVehicleID(), &vec, &vec11);


		CChatWindow::AddInfoMessage("%.3f, %.3f, %.3f", vec.x, vec.y, vec.z);
		return true;
	}

	if (strcmp(str, "/tab") == 0)
	{
		CTab::toggle();
		return true;
	}

	if (strstr(str, "/fpslimit "))
	{
		int fps = 0;
		if (sscanf(str, "%*s%d", &fps) == -1)
		{
			CChatWindow::AddDebugMessage("�����������: /fpslimit [fps]");
			return true;
		}
		ApplyFPSPatch(fps);

		CChatWindow::AddInfoMessage("-> ����� ����� FPS: %d", fps);

		CSettings::m_Settings.iFPS = fps;
        CSettings::save();
		return true;
	}
	if (strcmp(str, "/dl") == 0)
	{
		pGame->m_bDl_enabled = !pGame->m_bDl_enabled;
		return true;
	}

	if (strcmp(str, "/settings") == 0)
	{
		g_pJavaWrapper->ShowClientSettings();
		return true;
	}

	if (strcmp(str, "/fpsinfo") == 0)
	{
		CDebugInfo::ToggleDebugDraw();
		return true;
	}
	if (strcmp(str, "/style") == 0)
	{
		CStyling::show(0, 0, 0, 0, 0, 0);
		return true;
	}

	return false;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_hud_Chat_SendChatButton(JNIEnv *env, jobject thiz, jint button_id) {
	CKeyBoard::dop_butt = button_id;
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_hud_Chat_toggleNativeKeyboard(JNIEnv *env, jobject thiz, jboolean toggle) {
	if (toggle) {
		CKeyBoard::Open();
	} else {
		CKeyBoard::Close();
	}
}
extern "C"
JNIEXPORT void JNICALL
Java_com_liverussia_cr_gui_hud_Chat_clickHistoryButt(JNIEnv *env, jobject thiz, jint butt_id) {
	if(butt_id)
		CKeyBoard::m_pkHistory->PageUp();
	else
		CKeyBoard::m_pkHistory->PageDown();
}