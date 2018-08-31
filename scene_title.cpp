#include "game.h"
#include "sprite_data.h"
#include "sound_data.h"
#include "obj2d.h"
#include "scene_main.h"

#include "scene_title.h"



#include <thread>

//void loadTextureProgress()
//{
//	pTextureManager->loadTextures(e_loadTexture);		// 2D画像の一括ロード
//}
//void loadAudioProgress()
//{
//	pMFAudioManager->loadAudios(audio_data);			// 音声データの一括ロード
//}

SceneTitle::SceneTitle()
{
	Scene::init();
	//pTextureManager->loadTextures(e_loadTexture);		// 2D画像の一括ロード
	//std::thread loadThread_1(loadTextureProgress);
	//loadThread_1.detach();
	/*std::thread loadThread_2(loadAudioProgress);
	loadThread_2.detach();*/
	// pMFAudioManager->loadAudios(audio_data);
	//m_pBG = new OBJ2D;
	//m_pBG->m_pSprData = &e_sprTitleBG;
	//m_bg.m_pos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0 };

	//pObjManager->init();
	//pPlayerManager->init();
	//pPlayerManager->m_pPlayerTsuta->m_isOnLeftPage = false;
	//changeScene(SCENE_MAIN);
}

SceneTitle::~SceneTitle() {
	if (m_pNextScene)
	{
		//delete nextScene;
		m_pNextScene = nullptr;
	}
	SAFE_DELETE(m_pBG);
}

void SceneTitle::update()
{
	switch (m_step) {
	case STEP::INIT:

		//MFAudioPlay(BGM_MAIN, true);
		m_step = STEP::BEGIN;
		//break;

	case STEP::BEGIN:
		if (KEY_TRACKER.pressed.Z || PAD_TRACKER.a == PAD_TRACKER.PRESSED)
		{
			//MFAudioStop(BGM_MAIN);
			MFAudioPlay(SE_START);
			changeScene(SCENE_MAIN); 
		}
		changeScene(SCENE_MAIN);
		break;
	default:
		break;
	}
}

void SceneTitle::draw()
{
	View::clear();

	//m_pBG->draw();

}