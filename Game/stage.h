#ifndef _STAGE_H_
#define _STAGE_H_

#include <vector>

class OBJ3D;
struct Collision;

namespace Game{

class Stage
{
private:

public:
	Stage();
	~Stage();

	std::vector<OBJ3D> pObjList;
	std::vector<Collision*> pCollisionList;

	void Initialize();
	void Update();
	void Draw();


	static Stage* GetInstance() {
		static Stage instance;
		return &instance;
	};
};

#define pMainStage (Game::Stage::GetInstance())

};

#endif // !_STAGE_H_
