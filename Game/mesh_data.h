#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "game.h"

// Fbx Files numbering
enum FBX_FILE_NO
{
	PLAYER_STANDBY,
	//PLAYER_WALK,
	PLAYER_RUN,
	PLAYER_JUMP,
	PLAYER_ATTACK,
	//
	//ITEM_SWORD,
	ITEM_FLOOR,
	ITEM_ONE_METER_BOX
};

// Fbx files' path to be loaded
extern MeshFile e_sequencedFbxFiles[];

// Player Model

// Player Animation
extern MeshData fbxPlayerStandby;
//extern MeshData fbxPlayerWalk;
extern MeshData fbxPlayerRun;
extern MeshData fbxPlayerJump;
extern MeshData fbxPlayerAttack;

//extern MeshData e_fbxItemSword;
extern MeshData e_fbxItemFloor;
//extern MeshData e_fbxItemBox;



#endif // !_MESH_DATA_H_
