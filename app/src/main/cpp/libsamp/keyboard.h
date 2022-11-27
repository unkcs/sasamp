#pragma once
#include "keyboardhistory.h"
#include "vendor/raknet/SingleProducerConsumer.h"
#include <jni.h>

#define LAYOUT_ENG 0
#define LAYOUT_RUS 1
#define LAYOUT_NUM 2

#define LOWER_CASE 0
#define UPPER_CASE 1

#define KEY_DEFAULT 0
#define KEY_SHIFT 1
#define KEY_BACKSPACE 2
#define KEY_SWITCH 3
#define KEY_SPACE 4
#define KEY_SEND 5
#define KEY_HISTORY 6
#define KEY_UP 7
#define KEY_DOWN 8

#define MAX_INPUT_LEN 255

struct kbKey
{
	ImVec2 pos;
	ImVec2 symPos;
	float width;
	char code[2];
	char name[2][4];
	int type;
	int id;
};

class CKeyBoard
{
	friend class CGUI;

public:
	CKeyBoard();
	~CKeyBoard();

	void Open();
	void Close();

	bool IsOpen() { return m_bEnable; }
	void AddCharToInput(char sym);
	void Flush();

	void EnableNewKeyboard();
	void EnableOldKeyboard();

	bool IsNewKeyboard();

	void ProcessInputCommands();
	void OnNewKeyboardInput(JNIEnv *pEnv, jobject thiz, jbyteArray str);

	std::vector<kbKey> m_Rows[3][4]; // eng, rus, num

	std::string m_sInput;

	int dop_butt;
private:
	void Render();
	bool OnTouchEvent(int type, bool multi, int x, int y);

	void InitENG();
	void InitRU();
	void InitNUM();
	kbKey *GetKeyFromPos(int x, int y);

	void HandleInput(kbKey &key);
	void DeleteCharFromInput();
	void Send();

	bool m_bEnable;

	ImVec2 m_Size;
	ImVec2 m_Pos;
	float m_fKeySizeY;
	float m_fKeySizeX;
	float m_fFontSize;

	bool m_iPushedKeyUp;
	bool m_iPushedKeyDown;

	ImFont *fonticon;
	ImFont *chatfuncfont;

	int m_iLayout;
	int m_iCase;
	int m_iPushedKey;
	int chatinputposx;


	char m_utf8Input[MAX_INPUT_LEN * 3 + 0xF];
	int m_iInputOffset;
	CKeyBoardHistory *m_pkHistory;

	bool m_bNewKeyboard;
	DataStructures::SingleProducerConsumer<std::string> bufferedStrings;
};