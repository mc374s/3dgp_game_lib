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
	m_pBG = new OBJ2D;
	m_pBG->m_pSprData = &e_sprMainBG;
	m_pBG->m_custom.scaleX = m_pBG->m_custom.scaleY = 1280.0f / 1920.0f;

}

void SceneMain::init()
{
	Scene::init();
}

SceneMain::~SceneMain()
{
	if (m_pNextScene)
	{
		//delete nextScene;
		m_pNextScene = nullptr;
	}
	SAFE_DELETE(m_pBG);
};

void SceneMain::update()
{	
	if (pause()){
		return;
	}

	switch (m_step)
	{
	case STEP::INIT:
		MFAudioPlay(BGM_MAIN, true);
		m_step = STEP::BEGIN;

		//break;
	case STEP::BEGIN:

		gameMain();
		if (KEY_TRACKER.pressed.Z || PAD_TRACKER.a == PAD_TRACKER.PRESSED)
		{
			MFAudioStop(BGM_MAIN);
			MFAudioPlay(SE_START);
			changeScene(SCENE_TITLE);
			break;
		}
		break;
	case STEP::END:
		m_timer++;

		break;
	default:
		break;
	}

}

void SceneMain::draw()
{
	View::clear();

	m_pBG->draw();


#ifdef  DEBUG

	drawString(SCREEN_WIDTH / 2, 100, "SCENE MAIN", 0xFFFFFFFF, STR_CENTER, 48, 48);
	drawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, "Push [Z] to SCENE TITLE", 0xFFFFFFFF, STR_CENTER);


#endif //  DEBUG

	if (m_step == STEP::INIT) {
		drawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, 0, 0x000000FF & (unsigned int)((20 - m_timer) / 20.0f * 255));
	}

}

bool SceneMain::pause()
{
	static int pressTimer = 0;
	if ((KEY_TRACKER.pressed.Space || PAD_TRACKER.menu == PAD_TRACKER.PRESSED)) {
		m_isPaused = true;
	}

	if (m_isPaused)
	{
		pressTimer++;

		if ((KEY_TRACKER.released.C || PAD_TRACKER.x == PAD_TRACKER.RELEASED))
		{

		}
		if (pressTimer > 15 && (KEY_TRACKER.pressed.Space || PAD_TRACKER.start == PAD_TRACKER.PRESSED))
		{
			pressTimer = 0;
			m_isPaused = false;
		}
	}
	return m_isPaused;
}

void SceneMain::gameMain()
{

	
}
