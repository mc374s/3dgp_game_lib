#include "input.h"

std::unique_ptr<Keyboard>		Input::pKeyboard	= std::make_unique<Keyboard>();
Keyboard::State					Input::KEY_BOARD	= Keyboard::State();
Keyboard::KeyboardStateTracker	Input::KEY_TRACKER	= Keyboard::KeyboardStateTracker();

std::unique_ptr<GamePad>		Input::pGamePad		= std::make_unique<GamePad>();
GamePad::State					Input::GAME_PAD		= GamePad::State();
GamePad::ButtonStateTracker		Input::PAD_TRACKER	= GamePad::ButtonStateTracker();

bool							Input::isAnyKeyDown = false;