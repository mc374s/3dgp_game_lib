#ifndef _SCENE_H_
#define _SCENE_H_

class Scene
{
protected:
	int timer;
	int step;

public:
	Scene() :timer(0), step(0), pNextScene(nullptr) {};
	virtual ~Scene() {
		if (pNextScene) {
			pNextScene = nullptr;
		}
	};

	Scene *pNextScene;

	virtual void init() {
		timer = 0;
		step = 0;
		pNextScene = nullptr;
	};
	/*virtual void update(float _elapsedTime = .0f) {};
	virtual void Draw(float _elapsedTime = .0f) {};*/

	virtual void Update() {};
	virtual void Draw() {};

	virtual void changeScene(Scene* pNextScene) {
		pNextScene->pNextScene = nullptr;
		pNextScene->init();
		pNextScene = pNextScene;
	};
};


#endif // !_SCENE_H_
