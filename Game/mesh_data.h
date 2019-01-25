#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

#include "game.h"

// Fbx Files numbering
enum FBX_FILE_NO
{
	PLAYER_MODEL,
	PLAYER_STANDBY,
	PLAYER_RUN,
	PLAYER_JUMP,
	PLAYER_ATTACK,

	PLAYER_B_MODEL,
	PLAYER_B_STANDBY,
	PLAYER_B_RUN,
	PLAYER_B_JUMP,
	PLAYER_B_ATTACK,
	//
	//ITEM_SWORD,
	ITEM_FLOOR,
	ITEM_ONE_METER_BOX
};

// Fbx files' path to be loaded
extern MeshFile e_sequencedFbxFiles[];

// Player Model
extern MeshData fbxPlayerModel;
extern MeshData fbxPlayerBModel;

// Player Animation
extern MeshData fbxPlayerStandby;
extern MeshData fbxPlayerRun;
extern MeshData fbxPlayerJump;
extern MeshData fbxPlayerAttack;

extern MeshData fbxPlayerBStandby;
extern MeshData fbxPlayerBRun;
extern MeshData fbxPlayerBJump;
extern MeshData fbxPlayerBAttack;

//extern MeshData e_fbxItemSword;
extern MeshData e_fbxItemFloor;
//extern MeshData e_fbxItemBox;



#endif // !_MESH_DATA_H_
