#include "game.h"
#include "mesh_data.h"

// 
MeshFile e_sequencedFbxFiles[] = {
	{ PLAYER_WALK,			"./Data/FbxFiles/walk.fbx" },
	{ PLAYER_RUN,			"./Data/FbxFiles/run.fbx" },
	{ PLAYER_JUMP,			"./Data/FbxFiles/jump.fbx" },

	{ ITEM_SWORD,			"./Data/FbxFiles/sword.fbx" },
	{ ITEM_FLOOR,			"./Data/FbxFiles/street.fbx" },

	{ -1, NULL },
};


MeshData e_fbxPlayerWalk = MeshData(PLAYER_WALK, XMFLOAT3(0.05, 0.05, 0.05));
MeshData e_fbxPlayerRun = MeshData(PLAYER_RUN, XMFLOAT3(0.05, 0.05, 0.05));
MeshData e_fbxPlayerJump = MeshData(PLAYER_JUMP, XMFLOAT3(0.05, 0.05, 0.05));

MeshData e_fbxItemSword = MeshData(ITEM_SWORD, XMFLOAT3(0.05, 0.05, 0.05));
MeshData e_fbxItemFloor = MeshData(ITEM_FLOOR, XMFLOAT3(0.01, 0.01, 0.01), XMFLOAT3(0, -90, 0));
