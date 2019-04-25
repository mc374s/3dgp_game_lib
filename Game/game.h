#ifndef _GAME_H_
#define	_GAME_H_

#include "../3dgp/game_system.h"
#include "../3dgp/mf_audio.h"
#include "../3dgp/template.h"
#include "../3dgp/sprite.h"
#include "../3dgp/texture.h"
#include "../3dgp/scene.h"

// For more efficient performance, using DirectXMath instead of SimpleMath
#include "SimpleMath.h"
using namespace DirectX::SimpleMath;

#include "../3dgp/3dgp_math.h"

#define DEBUG

#define RESET_HEIGHT (-10)


#define CAMERA_BEST_DISTANCE (0.66f)
#define CAMERA_MAX_DISTANCE (1.45f)

#define CAMERA_BEST_DISTANCE_PX (CAMERA_BEST_DISTANCE*SCREEN_WIDTH)

// TODO : Systemly Learn how Header.h and Source.cpp work

namespace Game {

	// Affine transformation
	// Vector3: position, scale, rotation, origin.
	// @origin: scale and rotation pivot
	struct Transform
	{
		Vector3 position;
		Vector3 scale;
		Vector3 rotation;
		Vector3 origin;

		Transform() :position(Vector3::Zero), scale(Vector3::One), rotation(Vector3::Zero), origin(Vector3::Zero) {}

		// Reset all of the members to initial value
		void Clear() {
			position = Vector3::Zero;
			scale = Vector3::One;
			rotation = Vector3::Zero;
			origin = Vector3::Zero;
		}
	}; // struct Transform

	// int: left, top, width, height.
	// vec4: x, y, z, w regard as left, top, width, height.
	union Rectangle {
		struct {
			int left;
			int top;
			int width;
			int height;
		};

		// xyzw: left, top, width, height
		Vector4 vec4;
		Rectangle(int left = 0, int top = 0, int width = 0, int height = 0) :vec4(left, top, width, height) {};
	}; // union Rectangle

	// 2D sprite data
	// @clipRect: Clip a rectangle from texture @image to draw
	// @offsetX, @offsetY: draw position's offset
	// @frameNum: animation frames per second
	struct SpriteData
	{
		GLC::Texture* image;
		Game::Rectangle clipRect;
		int offsetX, offsetY;
		int frameNum;
		SpriteData() {};
		SpriteData(GLC::Texture* image, Game::Rectangle clipRect, int offsetX = 0, int offsetY = 0, int frameNum = 1);

		void Draw(const Game::Transform& transform, GLC::UINTCOLOR blendColor = 0xFFFFFFFF);
	}; // struct SpriteData

	typedef SpriteData SpriteLefttop;

	struct SpriteCenter : public SpriteData
	{
		SpriteCenter(GLC::Texture* image, Game::Rectangle clipRect, int frameNum = 1) :
			SpriteData(image, clipRect, -clipRect.width / 2, -clipRect.height / 2, frameNum) {};
	}; // struct SpriteCenter

	struct SpriteBottom : public SpriteData
	{
		SpriteBottom(GLC::Texture* image, Game::Rectangle clipRect, int frameNum = 1) :
			SpriteData(image, clipRect, -clipRect.width / 2, -clipRect.height, frameNum) {};
	}; // struct SpriteBottom



}; // namespace Game



#endif // !_GAME_H_
