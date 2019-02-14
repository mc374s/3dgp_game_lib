#ifndef _SCENE_TITLE_H_
#define _SCENE_TITLE_H_

#include "../3dgp/template.h"
#include "../3dgp/scene.h"

class OBJ2D;

// SceneTitleクラスの定義
class SceneTitle :public Scene, public Singleton<SceneTitle>
{
private:
	OBJ2D* pBG;
public:
	int progress = 0;
	SceneTitle();
	~SceneTitle();

	void Update();
	void Draw();

};


#define SCENE_TITLE (SceneTitle::GetInstance())


#endif // !_SCENE_TITLE_H_
