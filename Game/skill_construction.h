#ifndef _SKILL_CONSTRUCTION_H_
#define _SKILL_CONSTRUCTION_H_

#include "obj2d.h"

class UIOBJ :public OBJ2D 
{
private:
	
public:
	UIOBJ();
	~UIOBJ();
	OBJ2D absoluteData;
	UIOBJ* parent;

	virtual void Draw();

};


class SkillConstructionPanel:public UIOBJ 
{
private:

public:
	SkillConstructionPanel();
	~SkillConstructionPanel();

	enum PANEL
	{
		LEFT,
		RIGHT,
		BOTTOM,

		//P1_EQUIPPED,
		//P2_EQUIPPED,

		P1_CURSOR,
		P2_CURSOR,

		MAX_NUM_PANEL
	};
	enum SKILL
	{
		A,
		B,
		C,
		D,
		E,

		MAX_NUM_SKILL=25
	};

	UIOBJ panel[MAX_NUM_PANEL];
	UIOBJ skillIcon[MAX_NUM_SKILL];
	
	void Update();
	void Draw();
};



#endif // !_SKILL_CONSTRUCTION_H_
