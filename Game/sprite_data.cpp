#include "sprite_data.h"

using namespace GLC;
using namespace Game;

enum IMAGE
{
	NUMBERS,
	BG_TITLE,
	BG_MAIN,
	WHITE,

	MAX
};

Texture Game::images[] =
{
	{ "./Data/Images/UI/numbers.png" },

	{ "./Data/Images/BG/title.jpg" },
	{ "./Data/Images/BG/main.jpg" },

	{ "./Data/Images/white.png" },

	{ NULL },
};

SpriteData Game::sprTitleBG = SpriteData(&images[IMAGE::BG_TITLE], Game::Rectangle(0, 0, 1920, 1080));
SpriteData Game::sprMainBG = SpriteData(&images[IMAGE::BG_MAIN], Game::Rectangle(0, 0, 1920, 1080));

SpriteData Game::sprNumbers = SpriteData(&images[IMAGE::NUMBERS], Game::Rectangle(0, 0, 76, 118));
SpriteData Game::sprWhite = SpriteData(&images[IMAGE::WHITE], Game::Rectangle(0, 0, 64, 64));