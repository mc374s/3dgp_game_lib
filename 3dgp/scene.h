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

	virtual void Initialize() {
		timer = 0;
		step = 0;
		pNextScene = nullptr;
	};

	virtual void Update() {};
	virtual void Draw() {};

	// Set current scene's next scene with pTaregtScene
	// @doInitialization = true: initialize target scene before scene changing
	// @doInitialization = false: do not call target scene's initialization
	virtual void ChangeScene(Scene* pTargetScene, bool doInitialization = true) {
		pTargetScene->pNextScene = nullptr;
		if (doInitialization) {
			pTargetScene->Initialize();
		}
		pNextScene = pTargetScene;
	};
};


#endif // !_SCENE_H_
