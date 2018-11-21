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

	void Init();
	void Update();
	void Draw();

};

extern Stage mainStage;

}

#endif // !_STAGE_H_
