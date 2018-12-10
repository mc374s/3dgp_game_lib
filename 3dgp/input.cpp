#include "input.h"

using namespace DirectX;

std::unique_ptr<Keyboard>		Input::pKeyboard	= std::make_unique<Keyboard>();
Keyboard::State					Input::KEY			= Keyboard::State();
Keyboard::KeyboardStateTracker	Input::KEY_TRACKER	= Keyboard::KeyboardStateTracker();

//const int						Input::MAX_PLAYER_COUNT					= GamePad::MAX_PLAYER_COUNT;
std::unique_ptr<GamePad>		Input::pGamePad							= std::make_unique<GamePad>();
GamePad::State					Input::PAD[MAX_PLAYER_COUNT]			= { GamePad::State() };
GamePad::ButtonStateTracker		Input::PAD_TRACKER[MAX_PLAYER_COUNT]	= { GamePad::ButtonStateTracker() };

bool							Input::isAnyKeyDown = false;