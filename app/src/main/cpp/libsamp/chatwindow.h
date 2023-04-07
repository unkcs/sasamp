#pragma once
#include <vector>
#include "vendor/raknet/SingleProducerConsumer.h"
#define MAX_MESSAGE_LENGTH	144
#define MAX_LINE_LENGTH		MAX_MESSAGE_LENGTH / 2

#define NEW_MESSAGE_PUSH_TIME	500 // in ms

#define MAX_TIME_CHAT_HIDE	10000	// in ms
#define TIME_CHAT_HIDE_BEGIN	5000	// in ms

enum eChatMessageType
{
	CHAT_TYPE_NONE = 0,
	CHAT_TYPE_CHAT,
	CHAT_TYPE_INFO,
	CHAT_TYPE_DEBUG
};

typedef struct
{
	eChatMessageType	eType;
	char 				utf8Message[MAX_MESSAGE_LENGTH * 3 + 1];
	char 				szNick[MAX_PLAYER_NAME + 1];
	uint32_t			dwTextColor;
	uint32_t 			dwNickColor;
} CHAT_WINDOW_ENTRY;

struct BUFFERED_COMMAND_CHAT
{
	int type;
	char buff[300];
};

class CChatWindow
{
	friend class CGUI;
public:
	CChatWindow();
	~CChatWindow();

	static void AddChatMessage(char* szNick, uint32_t dwNickColor, char* szMessage);
	static void AddInfoMessage(char* szFormat, ...);
	static void AddDebugMessage(char* szFormat, ...);
	static void AddDebugMessageNonFormatted(char* szStr);

	static void FilterInvalidChars(char* szString);
};