#include "collision_detection.h"

#include "player.h"
#include "stage.h"
#include "../3dgp/3dgp_math.h"

using namespace Game;

void Game::DetectAllCollision()
{
	//for (auto pStageObjCollision = (pMainStage->pCollisionList).begin(), end = (pMainStage->pCollisionList).end(); pStageObjCollision != end; ++pStageObjCollision)
	//{
	//	//if(pStageObjCollision->HitJudgement())
	//}
	for (auto pStageObjCollision : pMainStage->pCollisionList)
	{
		pPlayerManager->DetectCollision(pStageObjCollision);
	}
}

