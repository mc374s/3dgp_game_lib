#include "game.h"
#include "sprite_data.h"
#include "sound_data.h"
#include "obj2d.h"

#include "player.h"
#include "judge.h"
#include "scene_title.h"

#include "scene_main.h"

float SCROLL_Y = 0;

SceneMain::SceneMain()
{
	//init();
	m_pBG = new OBJ2D;

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
		//init();
		++m_timer;

		break;
	case STEP::BEGIN:

		gameMain();

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
	char buf[256];
	sprintf_s(buf, "STAGE %d \nStageNO: %d \nm_step %d \nm_selectedStageNO %d", m_stageNO - STAGE_SELECT_MAX_NUM, m_stageNO, m_step, m_selectedStageNO);
	drawString(SCREEN_WIDTH / 2, 400, buf, COLOR_WHITE >> 8 << 8 | 0xA0, STR_CENTER, 40, 40);

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
