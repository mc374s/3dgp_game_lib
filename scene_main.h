#ifndef _SCENE_MAIN_H_
#define _SCENE_MAIN_H_


class OBJ2D;

class SceneMain :public Scene, public Singleton<SceneMain>
{
private:
	OBJ2D* m_pBG;

	bool m_isPaused = false;
	int m_selectionNO;
	
public:
	SceneMain();
	~SceneMain();

	void init();
	void update();
	void draw();

	bool pause();
	void gameMain();
};

#define SCENE_MAIN (SceneMain::getInstance())

#endif // !_SCENE_MAIN_H_
