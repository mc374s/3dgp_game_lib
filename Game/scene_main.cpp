#include "game.h"
#include "sprite_data.h"
#include "sound_data.h"
#include "obj2d.h"

#include "scene_title.h"

#include "scene_main.h"

float SCROLL_Y = 0;

SceneMain::SceneMain()
{
	//init();
	pBG = new OBJ2D;
	pBG->pSprData = &e_sprMainBG;
	pBG->transform2D.scaleX = pBG->transform2D.scaleY = 1280.0f / 1920.0f;

}

void SceneMain::init()
{
	Scene::init();
}

SceneMain::~SceneMain()
{
	if (pNextScene)
	{
		//delete nextScene;
		pNextScene = nullptr;
	}
	SAFE_DELETE(pBG);
};

void SceneMain::Update()
{	
	if (pause()){
		return;
	}

	switch (step)
	{
	case STEP::INIT:
		MFAudioPlay(BGM_MAIN, true);
		step = STEP::BEGIN;

		//break;
	case STEP::BEGIN:

		gameMain();
		if (KEY_TRACKER.pressed.Enter || PAD_TRACKER.start == PAD_TRACKER.PRESSED)
		{
			MFAudioStop(BGM_MAIN);
			MFAudioPlay(SE_START);
			changeScene(SCENE_TITLE);
			break;
		}
		break;
	case STEP::END:
		timer++;

		break;
	default:
		break;
	}

}

void SceneMain::Draw()
{
	View::Clear();

	pBG->Draw();


#ifdef  DEBUG

	DrawString(SCREEN_WIDTH / 2, 100, "SCENE MAIN", 0xFFFFFFFF, STR_CENTER, 48, 48);
	DrawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, "Push [ENTER] to SCENE TITLE", 0xFFFFFFFF, STR_CENTER);


#endif //  DEBUG

	if (step == STEP::INIT) {
		DrawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0x000000FF & (unsigned int)((20 - timer) / 20.0f * 255));
	}

}

bool SceneMain::pause()
{
	static int pressTimer = 0;
	if ((KEY_TRACKER.pressed.Space || PAD_TRACKER.menu == PAD_TRACKER.PRESSED)) {
		isPaused = true;
	}

	if (isPaused)
	{
		pressTimer++;

		if ((KEY_TRACKER.released.C || PAD_TRACKER.x == PAD_TRACKER.RELEASED))
		{

		}
		if (pressTimer > 15 && (KEY_TRACKER.pressed.Space || PAD_TRACKER.start == PAD_TRACKER.PRESSED))
		{
			pressTimer = 0;
			isPaused = false;
		}
	}
	return isPaused;
}

void SceneMain::gameMain()
{

	
}
