#include "scene_main.h"

#include "game.h"
#include "sprite_data.h"
#include "sound_data.h"
#include "obj2d.h"

#include "scene_title.h"

#include "mesh_data.h"

#include "player.h"

#include "stage.h"
#include "collision_detection.h"

#include "skill_construction.h"



SceneMain::SceneMain()
{
	//init();
	pBG = new OBJ2D;
	pBG->pSprData = &e_sprMainBG;
	pBG->transform2D.scaleX = pBG->transform2D.scaleY = 1280.0f / 1920.0f;

	skillConstructPanel = new SkillConstructionPanel;
	isPanelVisible = false;

}

void SceneMain::Init()
{
	Scene::Init();

	pPlayerManager->Init();
	pMainStage->Init();
}

SceneMain::~SceneMain()
{
	if (pNextScene)
	{
		//delete nextScene;
		pNextScene = nullptr;
	}
	SAFE_DELETE(pBG);
	SAFE_DELETE(skillConstructPanel)
};

void SceneMain::Update()
{	
	if (pause()){
		return;
	}
	if (Input::KEY_TRACKER.pressed.P) {
		isPanelVisible = !isPanelVisible;
	}

	switch (step)
	{
	case STEP::INIT:
		GLC::MFAudioPlay(BGM_MAIN, true);
		step = STEP::BEGIN;

		//break;
	case STEP::BEGIN:

		gameMain();
		if (Input::KEY_TRACKER.pressed.Enter || Input::PAD_TRACKER.start == Input::PAD_TRACKER.PRESSED)
		{
			GLC::MFAudioStop(BGM_MAIN);
			GLC::MFAudioPlay(SE_START);
			changeScene(SCENE_TITLE);
			break;
		}


		pPlayerManager->Update();
		Game::DetectAllCollision();


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

	// -210F
	e_fbxItemFloor.Draw(DirectX::g_XMZero, DirectX::g_XMOne, DirectX::g_XMZero);
	//e_fbxItemBox.draw();

	//static OBJ3D temp = g_player;



	// -60F
	pPlayerManager->Draw();
	pMainStage->Draw();


	if (isPanelVisible && skillConstructPanel) {
		skillConstructPanel->Draw();
	}


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
	if ((Input::KEY_TRACKER.pressed.Space || Input::PAD_TRACKER.menu == Input::PAD_TRACKER.PRESSED)) {
		isPaused = true;
	}

	if (isPaused)
	{
		pressTimer++;

		if ((Input::KEY_TRACKER.released.C || Input::PAD_TRACKER.x == Input::PAD_TRACKER.RELEASED))
		{

		}
		if (pressTimer > 15 && (Input::KEY_TRACKER.pressed.Space || Input::PAD_TRACKER.start == Input::PAD_TRACKER.PRESSED))
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
