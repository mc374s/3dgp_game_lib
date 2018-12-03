#include "skill_construction.h"

#include "sprite_data.h"

UIOBJ::UIOBJ()
{
}

UIOBJ::~UIOBJ()
{
}

void UIOBJ::Draw()
{
	if (alpha > 255) {
		alpha = 255;
	}
	if (alpha < 0) {
		alpha = 0;
	}
	transform2D.rgba = transform2D.rgba >> 8 << 8 | alpha;

	if (parent) {
		absoluteData.pos = pos + parent->absoluteData.pos;
		//absoluteData.alpha = alpha*parent->alpha / 255;
	}
	else {
		absoluteData.pos = pos;
	}
	if (pSprData) {
		pSprData->Draw(absoluteData.pos, transform2D, transform.position, transform.rotationDegree);
	}
	else {
		DrawRectangle(absoluteData.pos.x, absoluteData.pos.y, size.x, size.y, transform2D.angle, transform2D.rgba);
	}
	
}

SkillConstructionPanel::SkillConstructionPanel()
{
	size = Vector3(1000, 600, 0);
	pos = Vector3((SCREEN_WIDTH - size.x)*0.5f, (SCREEN_HEIGHT - size.y)*0.5f, 0);

	for (int i = 0; i < PANEL::MAX_NUM_PANEL; ++i) {
		panel[i].parent = this;
	}

	panel[LEFT].size.x = size.x / 2;
	panel[LEFT].size.y = 300;
	panel[LEFT].transform2D.rgba = 0xFF0000FF;

	panel[RIGHT].size = panel[LEFT].size;
	panel[RIGHT].pos.x = panel[LEFT].size.x;
	panel[RIGHT].transform2D.rgba = 0x00FF00FF;

	panel[BOTTOM].size.x = size.x;
	panel[BOTTOM].size.y = size.y - panel[LEFT].size.y;
	panel[BOTTOM].pos.y = panel[LEFT].size.y;
	panel[BOTTOM].transform2D.rgba = 0x888888FF;

	panel[BOTTOM].size.x = size.x;


	// アイコンのサイズとマージンによって、パネル内の相対位置を算出(改行を含め)
	Vector3 skillIconSize(sprCenterWhite.width, sprCenterWhite.height, 0);
	Vector3 margin(50, 50, 0);

	int marginedSize = skillIconSize.x + margin.x;
	int column = panel[BOTTOM].size.x / marginedSize + 1;// 一行のアイコンの数
	int adjustX = (panel[BOTTOM].size.x - column*marginedSize + margin.x) / 2 + skillIconSize.x / 2;
	int adjustY = skillIconSize.y / 2 + 20;
	for (int i = 0, j = 0; i < SKILL::MAX_NUM_SKILL; ++i) {
		skillIcon[i].parent = &panel[BOTTOM];
		skillIcon[i].pSprData = &sprCenterWhite;
		skillIcon[i].size = skillIconSize;
		skillIcon[i].pos.x = i%column * marginedSize + adjustX;
		skillIcon[i].pos.y = i / column * marginedSize + adjustY;
		
		skillIcon[i].transform2D.rgba = 0x0000FFFF;
	}

	cursorP1.parent = this;
	cursorP2.parent = this;

}

SkillConstructionPanel::~SkillConstructionPanel() 
{

}

void SkillConstructionPanel::Update() 
{

}

void SkillConstructionPanel::Draw()
{
	UIOBJ::Draw();

	for (int i = 0; i < PANEL::MAX_NUM_PANEL; ++i) {
		panel[i].Draw();
	}
	for (int i = 0; i < SKILL::MAX_NUM_SKILL; ++i) {
		skillIcon[i].Draw();
	}


#ifdef DEBUG


#endif // DEBUG


}