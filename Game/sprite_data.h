#ifndef _SPRDATA_H_
#define	_SPRDATA_H_

#include "game.h"

// ラベル定義
enum TEX_NO {

	TEX_NUMBERS,

	TEX_BG_TITLE,
	TEX_BG_MAIN,

	TEX_WHITE,
};


extern LOAD_TEXTURE e_loadTexture[];

// 背景
extern SPRITE_LEFTTOP e_sprMainBG;
extern SPRITE_LEFTTOP e_sprTitleBG;


// キャラなど


// UI関係

extern SPRITE_CENTER e_sprNumbers;

extern SPRITE_CENTER sprCenterWhite;

// Effect関係

#endif // !_SPRDATA_H_
