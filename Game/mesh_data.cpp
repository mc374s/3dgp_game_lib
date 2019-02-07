#include "mesh_data.h"

// 
MeshFile e_sequencedFbxFiles[] = {

	{ PLAYER_MODEL,			"./Data/FbxFiles/FightingUnityChan/Animations/idle.fbx" },
	{ PLAYER_STANDBY,		"./Data/FbxFiles/FightingUnityChan/Animations/idle.fbx" },
	{ PLAYER_RUN,			"./Data/FbxFiles/FightingUnityChan/Animations/run.fbx" },
	{ PLAYER_JUMP,			"./Data/FbxFiles/FightingUnityChan/Animations/jump.fbx" },
	{ PLAYER_ATTACK,		"./Data/FbxFiles/FightingUnityChan/Animations/screw_kick.fbx" },

	{ PLAYER_B_MODEL,		"./Data/FbxFiles/FromMixamo/Motion/lola_b_styperek_idle.fbx" },
	{ PLAYER_B_STANDBY,		"./Data/FbxFiles/FromMixamo/Motion/fight_idle_rig.fbx" },
	{ PLAYER_B_RUN,			"./Data/FbxFiles/FromMixamo/Motion/walk_rig.fbx" },
	{ PLAYER_B_JUMP,		"./Data/FbxFiles/FromMixamo/Motion/jump_rig.fbx" },
	{ PLAYER_B_ATTACK,		"./Data/FbxFiles/FromMixamo/Motion/fire_ball_rig.fbx" },

	//{ PLAYER_WALK,		"./Data/FbxFiles/Karate Warrior Mecanim Animation Pack Free/Characters/Karate@Run.fbx" },
	//{ PLAYER_RUN,			"./Data/FbxFiles/FightingUnityChan/Animations/run.fbx" },
	//{ PLAYER_JUMP,		"./Data/FbxFiles/FightingUnityChan/Animations/jump.fbx" },
	//{ PLAYER_ATTACK,		"./Data/FbxFiles/FightingUnityChan/Animations/screw_kick.fbx" },
	//{ PLAYER_ATTACK,		"./Data/FbxFiles/FightingMotionsVolume1/FBX/hp_straight_left_A.fbx" },
	//{ ITEM_SWORD,			"./Data/FbxFiles/sword.fbx" },
	{ ITEM_FLOOR,			"./Data/FbxFiles/street_meter.fbx" },
	//{ ITEM_ONE_METER_BOX,	"./Data/FbxFiles/one_meter_box.fbx" },
	
	{ -1, NULL },
};

// 64000 polygons, 1000 frames


MeshData fbxPlayerModel		= MeshData(PLAYER_MODEL, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerStandby	= MeshData(PLAYER_STANDBY, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerRun		= MeshData(PLAYER_RUN, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerJump		= MeshData(PLAYER_JUMP, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerAttack	= MeshData(PLAYER_ATTACK, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));

MeshData fbxPlayerBModel	= MeshData(PLAYER_B_MODEL, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerBStandby	= MeshData(PLAYER_B_STANDBY, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerBRun		= MeshData(PLAYER_B_RUN, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerBJump		= MeshData(PLAYER_B_JUMP, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));
MeshData fbxPlayerBAttack	= MeshData(PLAYER_B_ATTACK, Vector3(1, 1, 1), Vector3(0, 0, 0), Vector3(0, 90, 0));

//MeshData e_fbxItemSword	= MeshData(ITEM_SWORD, Vector3(0.05, 0.05, 0.05));
MeshData e_fbxItemFloor		= MeshData(ITEM_FLOOR, Vector3(1, 1, 1), Vector3(0, -0.09, 0), Vector3(0, 0, 0));
//MeshData e_fbxItemBox		= MeshData(ITEM_ONE_METER_BOX, Vector3(1, 1, 1), Vector3(-0.5, 0.5, 0));

