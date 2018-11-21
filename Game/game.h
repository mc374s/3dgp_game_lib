#ifndef _GAME_H_
#define	_GAME_H_

#include "../3dgp/game_system.h"
#include "../3dgp/mf_audio.h"
#include "../3dgp/template.h"
#include "../3dgp/scene.h"


#include "SimpleMath.h"
using namespace DirectX::SimpleMath;


struct Transform
{
	Vector3 position;
	Vector3 scaling;
	Vector3 rotationDegree;

	Transform() :position(Vector3::Zero), scaling(Vector3::One), rotationDegree(Vector3::Zero) {};

	void Clear() {
		position = Vector3::Zero;
		scaling = Vector3::One;
		rotationDegree = Vector3::Zero;
	};

	static Transform& initialValue() {
		static Transform initialValue;
		return initialValue;
	};
};



#define DEBUG

#define GROUND_HEIGHT (600)


#define CAMERA_BEST_DISTANCE (0.66f)
#define CAMERA_MAX_DISTANCE (1.45f)

#define CAMERA_BEST_DISTANCE_PX (CAMERA_BEST_DISTANCE*SCREEN_WIDTH)


// TODO : Systemly Learn how Header.h and Source.cpp work


#endif // !_GAME_H_
