//******************************************************************************
//
//
//		サウンドデータ
//
//
//******************************************************************************

#include "3dgp/mf_audio.h"
#include "sound_data.h"

RESOURCE_DATA audio_data[] = {
	{ BGM_TITLE,		"./DATA/Sounds/BGM/title.mp3" },
	{ BGM_MAIN,		"./DATA/Sounds/BGM/main.mp3" },

	{ SE_START,		"./DATA/Sounds/SE/start.wav" },
	{ SE_FALL,		"./DATA/Sounds/SE/fall.mp3" },
	{ SE_OPEN,		"./DATA/Sounds/SE/book_open.mp3" },
	{ SE_CLOSE,		"./DATA/Sounds/SE/book_close.mp3" },
	{ SE_TURN,		"./DATA/Sounds/SE/book_turn.mp3" },
	{ SE_SPLASH,	"./DATA/Sounds/SE/splash.wav" },

	{ SE_KEY_GOT,			"./DATA/Sounds/SE/key_got.mp3" },
	{ SE_DOOR_OPENED,		"./DATA/Sounds/SE/door_opened.mp3" },
	{ SE_DEAD,				"./DATA/Sounds/SE/dead.mp3" },
	{ SE_CURSOR,			"./DATA/Sounds/SE/cursor.mp3" },


	{ -1, nullptr }
};

//******************************************************************************
