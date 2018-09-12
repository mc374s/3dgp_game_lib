#include "game.h"
#include "mesh_data.h"

MeshFile e_sequencedFbxFiles[] = {
	{ PLAYER_WALK,			"./Data/FbxAnimations/walk.fbx" },
	{ PLAYER_RUN,			"./Data/FbxAnimations/run.fbx" },
	{ PLAYER_JUMP,			"./Data/FbxAnimations/jump.fbx" },

	{ ITEM_SWORD,			"./Data/FbxAnimations/sword.fbx" },
	{ ITEM_SWORD,			"./Data/FbxAnimations/floor.fbx" },

	{ -1, NULL },
};

MeshData e_fbxPlayerWalk = MeshData(PLAYER_WALK, XMFLOAT3(0.05, 0.05, 0.05));
MeshData e_fbxPlayerRun = MeshData(PLAYER_RUN, XMFLOAT3(0.05, 0.05, 0.05));
MeshData e_fbxPlayerJump = MeshData(PLAYER_JUMP, XMFLOAT3(0.05, 0.05, 0.05));

MeshData e_fbxItemSword = MeshData(ITEM_SWORD, XMFLOAT3(0.05, 0.05, 0.05));
MeshData e_fbxItemFloor = MeshData(ITEM_FLOOR, XMFLOAT3(0.05, 0.05, 0.05));
