//******************************************************************************
//
//
//		2D画像データ
//
//
//******************************************************************************

// インクルード
#include "sprite_data.h"


// 2D画像ロードデータ
LOAD_TEXTURE e_loadTexture[] = {

	{ TEX_NUMBERS,					"./Data/Images/UI/numbers.png" },

	{ TEX_BG_TITLE,					"./Data/Images/BG/title.jpg" },
	{ TEX_BG_MAIN,					"./Data/Images/BG/main.jpg" },

	{ TEX_WHITE,					"./Data/Images/white.png" },

	{ -1, NULL },
};

// 背景
SPRITE_LEFTTOP e_sprMainBG = SPRITE_LEFTTOP(TEX_BG_MAIN, 0, 0, 1920, 1080);
SPRITE_LEFTTOP e_sprTitleBG = SPRITE_LEFTTOP(TEX_BG_TITLE, 0, 0, 1920, 1080);


// キャラなど



// UI関係

SPRITE_CENTER e_sprNumbers = SPRITE_CENTER(TEX_NUMBERS, 0, 0, 76, 118);

SPRITE_CENTER sprCenterWhite = SPRITE_CENTER(TEX_WHITE, 0, 0, 64, 64);


//******************************************************************************
