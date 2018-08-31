#ifndef _SCENE_H_
#define _SCENE_H_

class Scene
{
protected:
	int m_timer;
	int m_step;

public:
	Scene() :m_timer(0), m_step(0), m_pNextScene(nullptr) {};
	virtual ~Scene() {
		if (m_pNextScene) {
			m_pNextScene = nullptr;
		}
	};

	Scene *m_pNextScene;

	virtual void init() {
		m_timer = 0;
		m_step = 0;
		m_pNextScene = nullptr;
	};
	/*virtual void update(float _elapsedTime = .0f) {};
	virtual void draw(float _elapsedTime = .0f) {};*/

	virtual void update() {};
	virtual void draw() {};

	virtual void changeScene(Scene* a_pNextScene) {
		a_pNextScene->m_pNextScene = nullptr;
		a_pNextScene->init();
		m_pNextScene = a_pNextScene;
	};
};


#endif // !_SCENE_H_
