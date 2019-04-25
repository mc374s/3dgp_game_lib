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
	rgba = rgba >> 8 << 8 | alpha;

	if (parent) {
		absoluteData.transform.position = transform.position + parent->absoluteData.transform.position;
		//absoluteData.alpha = alpha*parent->alpha / 255;
	}
	else {
		absoluteData.transform.position = transform.position;
	}
	if (pSprData) {
		pSprData->Draw(transform, rgba);
	}
	else {
		//DrawRectangle(absoluteData.pos.x, absoluteData.pos.y, size.x, size.y, transform2D.angle, rgba);
	}
	
}

SkillConstructionPanel::SkillConstructionPanel()
{
	size = Vector3(1000, 600, 0);
	transform.position = Vector3((SCREEN_WIDTH - size.x)*0.5f, (SCREEN_HEIGHT - size.y)*0.5f, 0);

	for (int i = 0; i < PANEL::MAX_NUM_PANEL; ++i) {
		panel[i].parent = this;
	}

	panel[LEFT].size.x = size.x / 2;
	panel[LEFT].size.y = 300;
	panel[LEFT].rgba = 0xFF0000FF;

	panel[RIGHT].size = panel[LEFT].size;
	panel[RIGHT].transform.position.x = panel[LEFT].size.x;
	panel[RIGHT].rgba = 0x00FF00FF;

	panel[BOTTOM].size.x = size.x;
	panel[BOTTOM].size.y = size.y - panel[LEFT].size.y;
	panel[BOTTOM].transform.position.y = panel[LEFT].size.y;
	panel[BOTTOM].rgba = 0x888888FF;

	panel[BOTTOM].size.x = size.x;


	// アイコンのサイズとマージンによって、パネル内の相対位置を算出(改行を含め)
	Vector3 skillIconSize(Game::sprWhite.clipRect.width, Game::sprWhite.clipRect.height, 0);
	Vector3 margin(50, 50, 0);

	int marginedSize = skillIconSize.x + margin.x;
	int column = panel[BOTTOM].size.x / marginedSize + 1;// 一行のアイコンの数
	int adjustX = (panel[BOTTOM].size.x - column*marginedSize + margin.x) / 2 + skillIconSize.x / 2;
	int adjustY = skillIconSize.y / 2 + 20;
	for (int i = 0, j = 0; i < SKILL::MAX_NUM_SKILL; ++i) {
		skillIcon[i].parent = &panel[BOTTOM];
		skillIcon[i].pSprData = &Game::sprWhite;
		skillIcon[i].size = skillIconSize;
		skillIcon[i].transform.position.x = i%column * marginedSize + adjustX;
		skillIcon[i].transform.position.y = i / column * marginedSize + adjustY;
		
		skillIcon[i].rgba = 0x0000FFFF;
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