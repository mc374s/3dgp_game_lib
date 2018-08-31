#ifndef _SCENE_TITLE_H_
#define _SCENE_TITLE_H_

class OBJ2D;

// SceneTitleクラスの定義
class SceneTitle :public Scene, public Singleton<SceneTitle>
{
private:
	OBJ2D* m_pBG;
public:
	SceneTitle();
	~SceneTitle();

	void update();
	void draw();

};


#define SCENE_TITLE (SceneTitle::getInstance())


#endif // !_SCENE_TITLE_H_
