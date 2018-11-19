#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_


class OBJ2D;

class SceneMain :public Scene, public Singleton<SceneMain>
{
private:
	OBJ2D* pBG;

	bool isPaused = false;
	int selectionNO;
	
public:
	SceneMain();
	~SceneMain();

	void Init();
	void Update();
	void Draw();

	bool pause();
	void gameMain();
};

#define SCENE_MAIN (SceneMain::getInstance())

#endif // !_SCENE_MAIN_H_
