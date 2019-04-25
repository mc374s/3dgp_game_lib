#include "game.h"
#include "../3dgp/system.h"

using namespace GLC;
using namespace Game;

SpriteData::SpriteData(Texture* image, Game::Rectangle clipRect, int offsetX, int offsetY, int frameNum) :
	image(image),
	clipRect(clipRect),
	offsetX(offsetX),
	offsetY(offsetY),
	frameNum(frameNum)
{}

void SpriteData::Draw(const Transform& transform, UINTCOLOR blendColor)
{
	static Sprite sprite(System::pImmediateContext);
	if (image) {
		sprite.Draw(image,
			clipRect.vec4,
			transform.position + Vector3(offsetX, offsetY, 0),
			transform.scale,
			transform.rotation,
			transform.origin,
			GLC::XMConvertUIntToColor(blendColor)
		);
	}
}