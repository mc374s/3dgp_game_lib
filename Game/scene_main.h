#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_

#include "../3dgp/template.h"
#include "../3dgp/scene.h"

class OBJ2D;
class SkillConstructionPanel;

class SceneMain :public Scene, public Singleton<SceneMain>
{
private:
	OBJ2D* pBG;
	SkillConstructionPanel* skillConstructPanel;
	bool isPanelVisible;

	bool isPaused = false;
	int selectionNO;
	
public:
	SceneMain();
	~SceneMain();

	void Initialize();
	void Update();
	void Draw();

	bool Pause();
	void gameMain();
};

#define SCENE_MAIN (SceneMain::GetInstance())

#endif // !_SCENE_MAIN_H_
