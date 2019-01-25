#include "scene_title.h"

//#include "game.h"
#include "sprite_data.h"
#include "sound_data.h"
#include "obj2d.h"
#include "scene_main.h"

#include <thread>

#include "mesh_data.h"

//void loadTextureProgress()
//{
//	pTextureManager->loadTextures(e_loadTexture);		// 2D画像の一括ロード
//}
//void loadAudioProgress()
//{
//	pMFAudioManager->loadAudios(audio_data);			// 音声データの一括ロード
//}

void loadMeshesProgress(void(*callback)(),int *progress)
{
	pMeshManager->LoadMeshes(e_sequencedFbxFiles, progress);
	if (callback)
	{
		callback();
	}
}

void loadeMessage() {
	MessageBox(0, L"Meshes loaded!", L"loadeMessage()", MB_OK);
}

SceneTitle::SceneTitle()
{
	Scene::Init();
	//pTextureManager->loadTextures(e_loadTexture);		// 2D画像の一括ロード
	//std::thread loadThread_1(loadTextureProgress);
	//loadThread_1.detach();
	/*std::thread loadThread_2(loadAudioProgress);
	loadThread_2.detach();*/
	// pMFAudioManager->loadAudios(audio_data);
	pBG = new OBJ2D;
	pBG->pSprData = &e_sprTitleBG;
	pBG->transform2D.scaleX = pBG->transform2D.scaleY = 1280.0f / 1920.0f;
	//bg.pos = { SCREEN_WIDTH / 2,SCREEN_HEIGHT / 2,0 };

	//pObjManager->init();
	//pPlayerManager->init();
	//pPlayerManager->pPlayerTsuta->isOnLeftPage = false;
	//changeScene(SCENE_MAIN);


	std::thread loadThread(loadMeshesProgress, nullptr, &progress);
	loadThread.detach();

	changeScene(SCENE_MAIN);


}

SceneTitle::~SceneTitle() {
	if (pNextScene)
	{
		//delete nextScene;
		pNextScene = nullptr;
	}
	SAFE_DELETE(pBG);
}

void SceneTitle::Update()
{
	switch (step) {
	case STEP::INIT:

		GLC::MFAudioPlay(BGM_TITLE, true);
		step = STEP::BEGIN;
		//break;

	case STEP::BEGIN:
		if (Input::KEY_TRACKER.pressed.Enter || Input::PAD_TRACKER[0].start == Input::PAD_TRACKER[0].PRESSED)
		{
			GLC::MFAudioStop(BGM_TITLE);
			GLC::MFAudioPlay(SE_START);
			changeScene(SCENE_MAIN);
			break;
		}


		break;
	default:
		break;
	}

}

void SceneTitle::Draw()
{
	View::Clear();
	pBG->Draw();
	


#ifdef  DEBUG

	DrawString(SCREEN_WIDTH / 2, 100, "SCENE TITLE", 0xFFFFFFFF, STR_CENTER, 48, 48);
	DrawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 200, "Push [ENTER] to next scene", 0xFFFFFFFF, STR_CENTER);

	char buf[256];
	sprintf_s(buf, "Loading Progress: %d %%", progress);
	DrawString(SCREEN_WIDTH / 2, SCREEN_HEIGHT - 100, buf, 0xFF0000FF, STR_CENTER);

#endif //  DEBUG

}