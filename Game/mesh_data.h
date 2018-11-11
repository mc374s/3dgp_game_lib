#ifndef _MESH_DATA_H_
#define _MESH_DATA_H_

// Fbx Files numbering
enum FBX_FILE_NO
{
	PLAYER_WALK,
	//PLAYER_RUN,
	PLAYER_JUMP,
	//
	//ITEM_SWORD,
	ITEM_FLOOR,
	ITEM_ONE_METER_BOX
};

// Fbx files' path to be loaded
extern MeshFile e_sequencedFbxFiles[];

// Player Animation
extern MeshData e_fbxPlayerWalk;
//extern MeshData e_fbxPlayerRun;
extern MeshData e_fbxPlayerJump;

//extern MeshData e_fbxItemSword;
extern MeshData e_fbxItemFloor;
//extern MeshData e_fbxItemBox;



#endif // !_MESH_DATA_H_
