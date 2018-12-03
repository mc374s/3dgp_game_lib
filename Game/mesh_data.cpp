#include "mesh_data.h"

// 
MeshFile e_sequencedFbxFiles[] = {
	{ PLAYER_STANDBY,		"./Data/FbxFiles/FightingUnityChan/Animations/idle.fbx" },
	//{ PLAYER_WALK,		"./Data/FbxFiles/Karate Warrior Mecanim Animation Pack Free/Characters/Karate@Run.fbx" },
	{ PLAYER_RUN,			"./Data/FbxFiles/FightingUnityChan/Animations/right_rotate_high_kick.fbx" },
	{ PLAYER_JUMP,			"./Data/FbxFiles/FightingUnityChan/Animations/jump.fbx" },

	//{ ITEM_SWORD,			"./Data/FbxFiles/sword.fbx" },
	//{ ITEM_FLOOR,			"./Data/FbxFiles/street_meter.fbx" },
	//{ ITEM_ONE_METER_BOX,	"./Data/FbxFiles/one_meter_box.fbx" },
	
	{ -1, NULL },
};


MeshData fbxPlayerStandby = MeshData(PLAYER_STANDBY, /*Vector3(0.05, 0.05, 0.05)*/Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
//MeshData fbxPlayerWalk = MeshData(PLAYER_WALK, /*Vector3(0.05, 0.05, 0.05)*/Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerRun = MeshData(PLAYER_RUN, /*Vector3(0.05, 0.05, 0.05)*/Vector3(0.01, 0.01, 0.01), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerJump = MeshData(PLAYER_JUMP, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));

//MeshData e_fbxItemSword = MeshData(ITEM_SWORD, Vector3(0.05, 0.05, 0.05));
MeshData e_fbxItemFloor = MeshData(ITEM_FLOOR, Vector3(1, 1, 1), Vector3(0, -0.09, 0), Vector3(0, 0, 0));
//MeshData e_fbxItemBox = MeshData(ITEM_ONE_METER_BOX, Vector3(1, 1, 1), Vector3(-0.5, 0.5, 0));

