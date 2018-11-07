#include "game.h"
#include "mesh_data.h"

// 
MeshFile e_sequencedFbxFiles[] = {
	{ PLAYER_WALK,			"./Data/FbxFiles/walk_meter.fbx" },
	//{ PLAYER_RUN,			"./Data/FbxFiles/run.fbx" },
	{ PLAYER_JUMP,			"./Data/FbxFiles/jump.fbx" },

	//{ ITEM_SWORD,			"./Data/FbxFiles/sword.fbx" },
	//{ ITEM_FLOOR,			"./Data/FbxFiles/street_meter.fbx" },
	//{ ITEM_ONE_METER_BOX,	"./Data/FbxFiles/one_meter_box.fbx" },
	
	{ -1, NULL },
};


MeshData e_fbxPlayerWalk = MeshData(PLAYER_WALK, /*XMFLOAT3(0.05, 0.05, 0.05)*/XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 90, 0));
//MeshData e_fbxPlayerRun = MeshData(PLAYER_RUN, XMFLOAT3(0.05, 0.05, 0.05));
MeshData e_fbxPlayerJump = MeshData(PLAYER_JUMP, XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 90, 0));

//MeshData e_fbxItemSword = MeshData(ITEM_SWORD, XMFLOAT3(0.05, 0.05, 0.05));
//MeshData e_fbxItemFloor = MeshData(ITEM_FLOOR, XMFLOAT3(1, 1, 1), XMFLOAT3(0, 0, 0), XMFLOAT3(0, 0, 0));
//MeshData e_fbxItemBox = MeshData(ITEM_ONE_METER_BOX, XMFLOAT3(1, 1, 1), XMFLOAT3(-0.5, 0.5, 0));

