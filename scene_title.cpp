#include "game.h"
#include "sprite_data.h"
#include "sound_data.h"
#include "obj2d.h"
#include "scene_main.h"

#include "scene_title.h"

#include "mesh_data.h"


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
	m_pBG = new OBJ2D;
	m_pBG->m_pSprData = &e_sprTitleBG;
	m_pBG->m_custom.scaleX = m_pBG->m_custom.scaleY = 1280.0f / 1920.0f;
	//m_bg.m_pos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0 };

	//pObjManager->init();
	//pPlayerManager->init();
	//pPlayerManager->m_pPlayerTsuta->m_isOnLeftPage = false;
	//changeScene(SCENE_MAIN);

	pMeshManager->loadMeshes(e_sequencedFbxFiles);
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

		MFAudioPlay(BGM_TITLE, true);
		m_step = STEP::BEGIN;
		//break;

	case STEP::BEGIN:
		if (KEY_TRACKER.pressed.Z || PAD_TRACKER.a == PAD_TRACKER.PRESSED)
		{
			MFAudioStop(BGM_TITLE);
			MFAudioPlay(SE_START);
			changeScene(SCENE_MAIN);
			break;
		}
		break;
	default:
		break;
	}
}

void SceneTitle::draw()
{
	View::clear();

	m_pBG->draw();
	static int fbxNO = 0;
	if (KEY_TRACKER.pressed.Space)
	{
		fbxNO >= FBX_FILE_NO::ITEM_SWORD ? fbxNO = 0 : ++fbxNO;
	}
	switch (fbxNO)
	{
	case FBX_FILE_NO::PLAYER_WALK:
		e_fbxPlayerWalk.draw(XMFLOAT3(0, 0, 0));
		break;
	case FBX_FILE_NO::PLAYER_RUN:
		e_fbxPlayerRun.draw(XMFLOAT3(0, 0, 0));
		break;
	case FBX_FILE_NO::PLAYER_JUMP:
		e_fbxPlayerJump.draw(XMFLOAT3(0, 0, 0));
		break;
	case FBX_FILE_NO::ITEM_SWORD:
		e_fbxItemSword.draw(XMFLOAT3(0, 0, 0));
		break;
	default:
		break;
	}
	e_fbxItemFloor.draw(XMFLOAT3(0, 0, 0));

#ifdef  DEBUG

	drawString(SCREEN_WIDTH / 2, 100, "SCENE TITLE", 0xFFFFFFFF, STR_CENTER, 48, 48);
	drawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, "Push [Z] to next scene", 0xFFFFFFFF, STR_CENTER);

#endif //  DEBUG

}