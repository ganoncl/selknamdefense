//Copyright 2017 Sebastian Gana, Rodrigo Alarcon, Walter Berendsen y Javier Gonzalez
//
//Licensed under the Apache License, Version 2.0 (the "License");
//you may not use this file except in compliance with the License.
//You may obtain a copy of the License at
//
//    http://www.apache.org/licenses/LICENSE-2.0
//
//Unless required by applicable law or agreed to in writing, software
//distributed under the License is distributed on an "AS IS" BASIS,
//WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
//See the License for the specific language governing permissions and
//limitations under the License.

#include "common.h"
#include "game.h"
#include "isoenemy.h"
#include "utils.h"

IsoEnemy::IsoEnemy()
{
	pGroup = NULL;
	CleanIso();
}

IsoEnemy::~IsoEnemy()
{
	pGroup = NULL;
}

void IsoEnemy::InitIsoPawn(IsoTerrain* _tile)
{
	int myTmpEnemyLevel = enemyLevel - 1;
	IsoPawn::InitIsoPawn(_tile);

	if(myTmpEnemyLevel < 0 || myTmpEnemyLevel > 2)
		myTmpEnemyLevel = 0;

	if(enemyType == ENEMY_Soldier)
	{
		SetIsoOffset(isoESoldierOffset);
		SetPawnAtributes(Enemy_Att_Soldier[myTmpEnemyLevel]);
	}
	else if(enemyType == ENEMY_Varon)
	{
		SetIsoOffset(isoEVaronOffset);
		SetPawnAtributes(Enemy_Att_Varon[myTmpEnemyLevel]);
	}
	else if(enemyType == ENEMY_Convict)
	{
		SetIsoOffset(isoESuicideOffset);
		SetPawnAtributes(Enemy_Att_Convict[myTmpEnemyLevel]);
		bIsAttackableOnMotion = true;
	}
	else if(enemyType == ENEMY_Scientist)
	{
		SetIsoOffset(isoEScientistOffset);
		SetPawnAtributes(Enemy_Att_Scientist[myTmpEnemyLevel]);
	}
	else if(enemyType == ENEMY_Tank)
	{
		SetIsoOffset(IsoETankOffset);
		SetPawnAtributes(Enemy_Att_Tank[myTmpEnemyLevel]);	
		bIsAttackableOnMotion = true;
	}
	else if(enemyType == ENEMY_Rabbit)
	{
		SetIsoOffset(isoERabbitOffset);
		SetPawnAtributes(Enemy_Att_Rabbit[myTmpEnemyLevel]);
		bIsAttackableOnMotion = true;
	}
	else if(enemyType == ENEMY_Pirate)
	{
		SetIsoOffset(isoEPirateOffset);
		SetPawnAtributes(Enemy_Att_Pirate[myTmpEnemyLevel]);
	}
	health = healthMax;
	SetEnemyAnimSeq();	
}

void IsoEnemy::Update()
{
	IsoPawn::Update();

	//We must ensure that the EnemyConvict has to be invisible inmediately after his death.
	if(enemyType == ENEMY_Convict)
	{
		if(bDead)
			setTextureColor(0,0,0,0);
	}

	if(CheckIfCanStop() && bGoToTargetAfterConfusion)
	{
		IwTrace(CONFBUG, ("Confusion is gone."));
		UnLockTerrain(GetTile());
		bGoToTargetAfterConfusion = false;
		if(eBehavior == BHE_Suicidal)
		{
			if(pawnTargeted == NULL)
			{
				IwTrace(SUICIDA, ("Go home"));
				MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
			}
			else
			{
				IwTrace(SUICIDA, ("Confusion OFF, target remains."));
				//pawnTargeted = LastPawnTargeted;
				b_suicidal_moving_enemy = false;
				InteractWithPawn(pawnTargeted);
			}
		}
		else
		{
			MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
		}
	}

	if(bMove && CheckIfCanStop() && GetEnemyType() != ENEMY_Tank && GetEnemyType() != ENEMY_Rabbit)
	{

		int actual_tile_idx;
		for(actual_tile_idx = 0; currentIsoPath[actual_tile_idx]; actual_tile_idx++)
		{
			if(currentIsoPath[actual_tile_idx] == GetTile())
				break;
		}
		actual_tile_idx++;
		if(currentIsoPath[actual_tile_idx])
		{
			if(currentIsoPath[actual_tile_idx]->CurrentIsoSelknam != NULL //|| //There is a selknam
			   //currentIsoPath[actual_tile_idx]->num_enemies_on_this_tile != 0  //There is an enemy
			   )
			{
				if(FinalTile != NULL) {
					MoveToIsoTerrain(FinalTile);
				}
			}


			/*
			if( currentIsoPath[actual_tile_idx]->CurrentIsoSelknam != NULL || 
			    (currentIsoPath[actual_tile_idx]->aStar_aditionalCost != 0 && currentIsoPath[actual_tile_idx] != markedTile))
			{
				if(FinalTile != NULL) {
					MoveToIsoTerrain(FinalTile);
				}
			}
			*/
		}
		else
		{
			/*
			if(FinalTile != NULL && actual_tile_idx >= 3)
			{
				MoveToIsoTerrain(FinalTile);
			}
			*/
		}

	
		IsoTerrain* currTile = GetTile();
		if(currTile != NULL)
		{
			if(currTile->num_enemies_on_this_tile > 1 && currentIsoPath[actual_tile_idx] != NULL)
			{
				IsoTerrain* tempTile = currentIsoPath[actual_tile_idx];
				bool bHasAditionalCost = false;
				if(currentIsoPath[actual_tile_idx]->aStar_aditionalCost == ASTAR_ENEMYONTILECOST)
				{
					bHasAditionalCost = true;
				}

				currentIsoPath[actual_tile_idx]->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
				if(FinalTile != NULL)
				{
					MoveToIsoTerrain(FinalTile);
					bBlockRepathingAction = true;

				}

				if(!bHasAditionalCost)
				{
					tempTile->aStar_aditionalCost = 0;
				}
			}
		}


		
		
		/*
		if(!bMove && CheckIfCanStop())
		{
			IwTrace(ENEMYBUG, ("%d", GetTile()->num_locks));
		}
		IwTrace(ENEMYBUG, ("%d", GetTile()->num_locks));
		*/
		//IwTrace(OVERLAP, ("Overlapping: %d", currTile->num_enemies_on_this_tile));
/*
		if(currTile != NULL && !bBlockRepathingAction)
		{
			currTile->num_enemies_on_this_tile++;
			
			if(currTile->num_enemies_on_this_tile > 1 && currentIsoPath[actual_tile_idx] != NULL)
			{
				bool bHasAditionalCost = false;
				if(currentIsoPath[actual_tile_idx]->aStar_aditionalCost == ASTAR_ENEMYONTILECOST)
				{
					bHasAditionalCost = true;
				}

				currentIsoPath[actual_tile_idx]->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
				if(FinalTile != NULL)
				{
					MoveToIsoTerrain(FinalTile);
					bBlockRepathingAction = true;
					
				}

				if(!bHasAditionalCost)
				{
					currentIsoPath[actual_tile_idx]->aStar_aditionalCost = 0;
				}
			}
		}
		*/
	}
	

	if(bDead)
		return;
	//Test code: When an enemy reach an objective, damage to Player is applied.
	IsoTerrain* Objective = gameRef->objectivesNodes[getTargetNum() - 1];
	IsoTerrain* _CurrentTile = GetTile();

	if(Objective == NULL || _CurrentTile == NULL)
		return;

	if(_CurrentTile == Objective)
	{
		if(gameRef != NULL)
			gameRef->TakeDamage(1, this);
		_CurrentTile->aStar_aditionalCost = 0;
		this->CleanIso();
	}
}

void IsoEnemy::CleanIso()
{
	IsoPawn::CleanIso();
	enemyType = ENEMY_None;
	enemyLevel = 0;
	initPathNum = -1;
	waveToSpawn = -1;
	secToSpawn = -1;
	targetNum = -1;

	bIsInvincible = false;
	b_suicidal_moving_enemy = false;
	bIgnoreEnemies = false;
	bRecalcPath = true;
	bFollow = false;
	xFollowTile = -1;
	yFollowTile = -1;
	target_terrain = NULL;
	follow_tile = NULL;
	bLockDone = false;
	myTerrainLock = NULL;
	bFirstMove = true;
	doActionOnThisTile = NULL;
	bBlockRepathingAction = false;
	bGoToTargetAfterConfusion = false;

	//Convict
	LastPawnTargeted = NULL;
}

void IsoEnemy::SetEnemyAnimSeq()
{
	Utils::SetEnemyAnim(enemyType, &animations, this);
	SetAttackSpeed();
}

IsoPawn* IsoEnemy::SearchPawnNearby()
{
	if(bDead)
		return NULL;

	if(!bCanAttack)
		return NULL;

	if(eBehavior == BHE_Melee_Ofensive_Defensive)
	{
		return CloserEnemyInRange();
	}
	else if(eBehavior == BHE_Range_SingleFocus)
	{
		return CloserEnemyInRange();
	}
	else if(eBehavior == BHE_Suicidal)
	{
		return SearchNearestEnemyOnTheWholeMap();
		//return EnemyWithMoreEnemiesAround(Skill_Att_Explode[level - 1].range);
	}

	return NULL;
}

IsoPawn* IsoEnemy::EnemyWithMoreEnemiesAround(int range)
{
	int enemies_around = 0;
	IsoSelknam* enemy = NULL; 

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		IsoSelknam* CurrentIsoSelknam = &gameRef->isoSelknamArr[i];

		if(!CurrentIsoSelknam->getInitialized())
			continue;

		if(CurrentIsoSelknam->GetSelknamType() != SELKNAM_None && !CurrentIsoSelknam->GetIsDead())
		{
			int xTile = Utils::Round(CurrentIsoSelknam->getIsoPosition().x / 100);
			int yTile = Utils::Round(CurrentIsoSelknam->getIsoPosition().y / 100);
			int enemies_around_local = 0;

			for(int j = 0; j < MAX_ISO_ENEMIES; j++)
			{
				IsoSelknam* selknam_nearby = &gameRef->isoSelknamArr[j];	

				if(!selknam_nearby->getInitialized())
					continue;

				if(selknam_nearby->GetSelknamType() != SELKNAM_None && !selknam_nearby->GetIsDead())
				{
					int x_enemy_nearby = Utils::Round(selknam_nearby->getIsoPosition().x / 100);
					int y_enemy_nearby = Utils::Round(selknam_nearby->getIsoPosition().y / 100);
					
					if(abs(xTile - x_enemy_nearby) <= range && abs(yTile - y_enemy_nearby) <= range)
						enemies_around_local++;
				}
			}

			if(enemies_around_local > enemies_around)
			{
				enemy = CurrentIsoSelknam;
				enemies_around = enemies_around_local;
			}
		}
	}
	return enemy;
}

IsoPawn* IsoEnemy::CloserEnemyInRange()
{
	int curr_distance = 100000;
	int min_distance = 10000;
	IsoSelknam* CloserEnemy = NULL; 

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		IsoSelknam* CurrentIsoSelknam = &gameRef->isoSelknamArr[i];

		if(!CurrentIsoSelknam->getInitialized())
			continue;

		if(CurrentIsoSelknam->GetSelknamType() != SELKNAM_None && !CurrentIsoSelknam->GetIsDead())
		{
			if(IsPawnInRange(CurrentIsoSelknam, &curr_distance)) {
				if(curr_distance < min_distance)
				{
					CloserEnemy = CurrentIsoSelknam;
					min_distance = curr_distance;
				}
			}
		}
	}
	
	return CloserEnemy;
}

void IsoEnemy::UpdateBehaviour()
{
	if(bDead)
		return;

	if(has_skill_effect[(int)Skill_Type_Confusion])
		bRecalcPath = true;

	if(eBehavior == BHE_Melee_Ofensive_Defensive)
	{		
			UpdateAttackPattern();
	}
	else if(eBehavior == BHE_Range_SingleFocus)
	{
		if(CheckIfCanStop())
			UpdateAttackPattern();
	}
	else if(eBehavior == BHE_Suicidal)
	{
		//if(bCanStop)
			UpdateAttackPattern();
	}
	else if(eBehavior == BHE_Objective)
	{
		if(CheckIfCanStop())
			UpdateAttackPattern();
	}
}

void IsoEnemy::NoPawnInteraction()
{
	if(bDead)
		return;

	if((bRecalcPath && CheckMovementConstraints() && CheckIfCanStop()))
	{
		//IwTrace(RAMP, ("NoPawnInteraction."));
		//bFirstMove = false;

		if(myTerrainLock != NULL)
		{
			UnLockTerrain(myTerrainLock);
			myTerrainLock = NULL;
		}

		MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
		bRecalcPath = false;
	}
}

bool IsoEnemy::ChangeTargetCondition(IsoPawn* target)
{
	if(bDead)
		return true;


	if(eBehavior == BHE_Melee_Ofensive_Defensive)
	{
		return false;
	}
	else if(eBehavior == BHE_Range_SingleFocus)
	{
		return (pawnTargeted->GetIsDead() || !IsPawnInRange(pawnTargeted) || !pawnTargeted->getInitialized());
	}
	else if(eBehavior == BHE_Suicidal)
	{
		bRecalcPath = true;
		return true;
	}
	else if(eBehavior == BHE_Objective)
	{
		return true;
	}

	return false;
}

void IsoEnemy::MoveToIsoTerrain_PrunePath(IsoTerrain* tile, IsoTerrain* obj)
{
	if(bDead)
		return;

	CurrentTile = GetTile();

	bMove = aStarHelperRef->AStarRetrievePathByGrid(gameRef->isoTerrainAstarGrid, CurrentTile, tile, currentIsoPath);

	int new_min = 999;
	int new_min_idx = -1;
	for(int i = 0; currentIsoPath[i]; i++)
	{
		if(isValidToWalk(currentIsoPath[i]))
		{
			int temp_new_min = DistanceBetweenTiles(obj, currentIsoPath[i]);
			if(temp_new_min < new_min)
			{
				new_min = temp_new_min;
				new_min_idx = i;
			}
		}
	}

	currentIsoPath[new_min_idx + 1] = NULL;

	if(currentIsoPath[0])
	{
		FinalTile = tile;
		NextTile = currentIsoPath[0];
		direction = GetDirectionByAdyTiles(CurrentTile, NextTile);
		LastCurrentTile = CurrentTile;
		LastNextTile = NextTile;
		CurrentTile->aStar_aditionalCost = 0;
		anim = ANIM_Walk;
	} 
}

void IsoEnemy::MoveToIsoTerrain(IsoTerrain* _tileFinal, bool bPermanent)
{
	if(bDead)
		return;

	CurrentTile = GetTile();

	bMove = aStarHelperRef->AStarRetrievePathByGrid(gameRef->isoTerrainAstarGrid, CurrentTile, _tileFinal, currentIsoPath);
	
	int xTile = Utils::Round(getIsoPosition().x / 100);
	int yTile = Utils::Round(getIsoPosition().y / 100);
	
	if(currentIsoPath[0])
	{
		FinalTile = _tileFinal;
		NextTile = currentIsoPath[0];
		direction = GetDirectionByAdyTiles(CurrentTile, NextTile);
		LastCurrentTile = CurrentTile;
		LastNextTile = NextTile;
		CurrentTile->aStar_aditionalCost = 0;
		//NextTile->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
		anim = ANIM_Walk;
	} 

}

void IsoEnemy::AttackPawn(IsoPawn* enemy)
{
	if(bDead)
		return;

	if(time_per_attack.GetIsEnd() || time_per_attack.GetIsStop())
	{
		if(bCanAttack)
		{
			time_per_attack.Reset();
			time_per_attack.Start();
			DoAttackOnce = true;
			anim = ANIM_Attack;
			bAttack = true;
			bCanAttack = false;
			animations.ResetAnim();
		}
	}
}


bool IsoEnemy::CheckIfCanStop()
{
	bool ret = IsoPawn::CheckIfCanStop();
	
	if(!bMove)
		return ret;

	if(ret)
	{
		if(GetTile()->CurrentIsoSelknam != NULL) return false;

		return true;
		//return bSecondStopCheck;
	}

	return false;
}

void IsoEnemy::HandleDie()
{
	IsoPawn::HandleDie();

	gameRef->setMPCoins(gameRef->getMPCoins() + coins_perkill);
	Utils::PlayFXSound(FX_Sound_COINS);
	//IwTrace(DEATH, ("COIN"));
	UnLockTerrain(GetTile());

	if(isoCoin_idx == -1)
	{
		for(int i = 0; i < MAX_ISO_ENEMIES; i++)
		{
			if(gameRef->isoCoin[i].index == -1)
			{
				gameRef->isoCoin[i].index = 0;
				gameRef->isoCoin[i]._x = getPosX();
				gameRef->isoCoin[i]._y = getPosY();
				gameRef->isoCoin[i]._z = getPosZ();
				gameRef->isoCoin[i].timer.Reset();
				gameRef->isoCoin[i].timer.Start();
				isoCoin_idx = i;
				break;
			}
		}
	}

	if(enemyType == ENEMY_Scientist)
	{
		//Poison on death
		IsoSelknam* target = NULL;
		int x_tile_me = Utils::Round(getIsoPosition().x / 100);
		int y_tile_me = Utils::Round(getIsoPosition().y / 100);
		int explode_range = Skill_Att_PoisonOnDeath[level - 1].range;

		for(int i = 0; i < MAX_ISO_SELKNAM; i++)
		{
			target = &gameRef->isoSelknamArr[i];
			if(target->getInitialized() && target->GetSelknamType() != SELKNAM_None)
			{
				int x_tile = Utils::Round(target->getIsoPosition().x / 100);
				int y_tile = Utils::Round(target->getIsoPosition().y / 100);
				if(abs(x_tile - x_tile_me) <= explode_range && abs(y_tile - y_tile_me) <= explode_range)
				{
					target->TakeDamage(0, this, GetPawnAttributes(), skill);
					for(int j = 0; j < MAX_ISO_SELKNAM; j++)
					{
						if(gameRef->isoPoisonOnDeath[j].index == -1)
						{
							gameRef->isoPoisonOnDeath[j].index = 0;
							gameRef->isoPoisonOnDeath[j]._x = target->getPosX();
							gameRef->isoPoisonOnDeath[j]._y = target->getPosY();
							gameRef->isoPoisonOnDeath[j]._z = target->getPosZ();
							gameRef->isoPoisonOnDeath[j].timer.Reset();
							gameRef->isoPoisonOnDeath[j].timer.Start();
							target->SetIsoPoisonOnDeathIndex(j);
							break;
						}
					}
				}
			}
		}


		for(int i = x_tile_me - explode_range; i <= (x_tile_me + explode_range); i++)
		{
			for(int j = y_tile_me - explode_range; j <= (y_tile_me + explode_range); j++)
			{
				if(i >= 0 && i < MAX_TILESROW && j >= 0 && j < MAX_TILESROW)
				{
					if(gameRef->isoTerrainAstarGrid[i][j] != NULL && gameRef->isoTerrainAstarGrid[i][j]->CurrentIsoSelknam == NULL)
					{
						for(int k = 0; k < MAX_ISO_ENEMIES; k++)
						{
							if(gameRef->isoPoisonOnDeath[k].index == -1)
							{
								gameRef->isoPoisonOnDeath[k].index = 0;
								gameRef->isoPoisonOnDeath[k]._x = gameRef->isoTerrainAstarGrid[i][j]->getPosX();
								gameRef->isoPoisonOnDeath[k]._y = gameRef->isoTerrainAstarGrid[i][j]->getPosY() - 16.0f;
								gameRef->isoPoisonOnDeath[k]._z = gameRef->isoTerrainAstarGrid[i][j]->getPosZ();
								gameRef->isoPoisonOnDeath[k].bRender = true;
								gameRef->isoPoisonOnDeath[k].timer.Reset();
								gameRef->isoPoisonOnDeath[k].timer.Start();
								//target->SetIsoExplotionIndex(j);
								break;
							}
						}
					}
				}
			}
		}

		Utils::PlayFXSound(FX_Sound_BUBBLES);
	}
	else if(enemyType == ENEMY_Convict)
	{
		setTextureColor(0,0,0,0);

		IsoSelknam* target = NULL;
		int x_tile_me = Utils::Round(getIsoPosition().x / 100);
		int y_tile_me = Utils::Round(getIsoPosition().y / 100);
		int explode_range = Skill_Att_Explode[level - 1].range;

		for(int i = 0; i < MAX_ISO_SELKNAM; i++)
		{
			target = &gameRef->isoSelknamArr[i];
			if(target->getInitialized() && target->GetSelknamType() != SELKNAM_None)
			{
				int x_tile = Utils::Round(target->getIsoPosition().x / 100);
				int y_tile = Utils::Round(target->getIsoPosition().y / 100);
				if(abs(x_tile - x_tile_me) <= explode_range && abs(y_tile - y_tile_me) <= explode_range)
				{
					target->TakeDamage(Skill_Att_Explode[level - 1].damage);
					for(int j = 0; j < MAX_ISO_ENEMIES; j++)
					{
						if(gameRef->isoExplotion[j].index == -1)
						{
							gameRef->isoExplotion[j].index = 0;
							gameRef->isoExplotion[j]._x = target->getPosX();
							gameRef->isoExplotion[j]._y = target->getPosY();
							gameRef->isoExplotion[j]._z = target->getPosZ();
							gameRef->isoExplotion[j].timer.Reset();
							gameRef->isoExplotion[j].timer.Start();
							target->SetIsoExplotionIndex(j);
							break;
						}
					}
				}
			}
		}
		
		IwTrace(EXPLOBUG, ("Buscando tiles sin selknams: %d", explode_range));
		for(int i = x_tile_me - explode_range; i <= (x_tile_me + explode_range); i++)
		{
			for(int j = y_tile_me - explode_range; j <= (y_tile_me + explode_range); j++)
			{
				
				if(i >= 0 && i < MAX_TILESROW && j >= 0 && j < MAX_TILESROW)
				{
					if(gameRef->isoTerrainAstarGrid[i][j] != NULL && gameRef->isoTerrainAstarGrid[i][j]->CurrentIsoSelknam == NULL)
					{
						for(int k = 0; k < MAX_ISO_ENEMIES; k++)
						{
							if(gameRef->isoExplotion[k].index == -1)
							{
								gameRef->isoExplotion[k].index = 0;
								gameRef->isoExplotion[k]._x = gameRef->isoTerrainAstarGrid[i][j]->getPosX();
								gameRef->isoExplotion[k]._y = gameRef->isoTerrainAstarGrid[i][j]->getPosY() - 16.0f;
								gameRef->isoExplotion[k]._z = gameRef->isoTerrainAstarGrid[i][j]->getPosZ();
								gameRef->isoExplotion[k].timer.Reset();
								gameRef->isoExplotion[k].timer.Start();
								gameRef->isoExplotion[k].bRender = true;
								//IwTrace(EXPLOBUG, ("%d %d", i, j));
								//target->SetIsoExplotionIndex(j);
								break;
							}
						}
					}
				}
			}
		}

		Utils::PlayFXSound(FX_Sound_EXPLOSION);
	}


}

void IsoEnemy::SetAttackSpeed()
{
	animations.UpdateAnimTime("AttackNorth",     10 - attackspeed - attackspeed_modifier);
	animations.UpdateAnimTime("AttackSouth",     10 - attackspeed - attackspeed_modifier);
	animations.UpdateAnimTime("AttackEast",      10 - attackspeed - attackspeed_modifier);
	animations.UpdateAnimTime("AttackWest",      10 - attackspeed - attackspeed_modifier);
	animations.UpdateAnimTime("AttackNorthEast", 10 - attackspeed - attackspeed_modifier);
	animations.UpdateAnimTime("AttackNorthWest", 10 - attackspeed - attackspeed_modifier);
	animations.UpdateAnimTime("AttackSouthWest", 10 - attackspeed - attackspeed_modifier);
	animations.UpdateAnimTime("AttackSouthEast", 10 - attackspeed - attackspeed_modifier);

	SetAttackCooldown(GetAttackCooldown());
	SetCastCooldown(GetCastCooldown());
}

bool IsoEnemy::SpecificAnimationOnWalk(IsoDirection dir)
{
	if(enemyType == ENEMY_Rabbit && has_skill_effect[(int)Skill_Type_Slow])
	{
		if(direction == DIR_North)      animations.SetAnim("AttackNorth", true);
		else if(direction == DIR_East)  animations.SetAnim("AttackEast", true);
		else if(direction == DIR_South) animations.SetAnim("AttackSouth", true);
		else if(direction == DIR_West)  animations.SetAnim("AttackWest", true);
		return true;
	}

	return false;
}

int IsoEnemy::GetAttackTimeAnimation()
{
	return attackspeed;
}

bool IsoEnemy::isValidToWalk(IsoTerrain* tile)
{
	if(tile == NULL) return false;
	//Cannot put unit
	//if(gameRef->isoTerrainAstarGrid[i][j]->getCanPutUnit() == false) continue;
	if(tile->getEnemyCanStopHere() == false) return false;
	if(tile->getIsRamp()) return false;
	//There is a selknam on it
	if(tile->CurrentIsoSelknam != NULL) return false;
	if(tile->aStar_aditionalCost != 0) return false;
	return true;
}

void IsoEnemy::DoSpecificActionOnDebuff(Skill_Type skill)
{
	if(skill == Skill_Type_Confusion)
	{
		//bRecalcPath = true;
		//pawnTargeted = NULL;
	}
	else if(skill == Skill_Type_Root)
	{
		//LockTerrain(GetTile());
	}
	else if(skill == Skill_Type_Stun)
	{
		//LockTerrain(GetTile());
		animations.PauseAnim();
	}
}

void IsoEnemy::DoSpecificActionOnEndDebuff(Skill_Type skill)
{
	if(skill == Skill_Type_Stun)
	{
		bStun = false;
		//IwTrace(STUNG_BUGGED, ("Resuming animations."));
		animations.ResumeAnim();
		//UnLockTerrain(GetTile());
	}
	else if(skill == Skill_Type_Root)
	{
		bRoot = false;
		
		UnLockTerrain(GetTile());
	}
	else if(skill == Skill_Type_Confusion) 
	{
		UnLockTerrain(GetTile());
		bGoToTargetAfterConfusion = true;
		//MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
		/*
		if(eBehavior == BHE_Suicidal)
		{
			pawnTargeted = LastPawnTargeted;
		}
		*/
	}
}

void IsoEnemy::DoShoot(IsoProjectileType type, IsoPawn* enemy)
{
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		if(gameRef->isoProjectileArr[i].GetType() == Projectile_None)
		{
			Utils::PlayFXSound(FX_Sound_SHOT01);
			gameRef->isoProjectileArr[i].InitInflictor(this, GetPawnAttributes(), damage + damage_modifier, skill);
			gameRef->isoProjectileArr[i].InitTarget(enemy);
			gameRef->isoProjectileArr[i].InitProjectile(type);
			break;
		}
	}
}

void IsoEnemy::CheckIsCanDoDamageByAnimation()
{
	if(Utils::CanAttackByAnimation(enemyType, animations.GetCurrentAnimID()) && DoAttackOnce)
	{
		if(enemyType == ENEMY_Scientist)
			pawnTargeted->TakeDamage(damage + damage_modifier, this, GetPawnAttributes(), Skill_Type_None);
		else if(enemyType == ENEMY_Varon)
			DoShoot(Projectile_Bullet, pawnTargeted);
		else
			pawnTargeted->TakeDamage(damage + damage_modifier, this, GetPawnAttributes(), skill);
		DoAttackOnce = false;

		if(enemyType == ENEMY_Soldier) 
		{
			Utils::PlayFXSound(FX_Sound_KNIFESWORD01);
		}

	}
}

void IsoEnemy::LockTerrain(IsoTerrain* tile)
{
	if(tile == NULL)
	{
		return;
	}

	if(!bLockDone)
	{
		IsoPawn::LockTerrain(tile);
		tile->setEnemyCanStopHere(false);
		bLockDone = true;
	}
}

void IsoEnemy::UnLockTerrain(IsoTerrain* tile)
{
	if(tile == NULL)
	{
		return;
	}

	IsoPawn::UnLockTerrain(tile);
	tile->setEnemyCanStopHere(true);
	bLockDone = false;
}

IsoTerrain* IsoEnemy::FindClosestFreeTileTo(IsoTerrain* tile, int range_tile)
{
	if(tile == NULL) return NULL;
	
	IsoTerrain* closest_tile = NULL;
	int min_distance = 1 << 12;
	for(int i = tile->getXNumTile() - range_tile; i <= (tile->getXNumTile() + range_tile); i++)
	{
		for(int j = tile->getYNumTile() - range_tile; j <= (tile->getYNumTile() + range_tile); j++)
		{
			//Out of border
			if(i < 0 || j < 0 || i >= MAX_TILESROW || j >= MAX_TILESROW) continue;
			if(gameRef->isoTerrainAstarGrid[i][j] == NULL) continue;
			
			if(!gameRef->isoTerrainAstarGrid[i][j]->getIsRamp())
			{
				if(gameRef->isoTerrainAstarGrid[i][j]->getLayer() != tile->getLayer()) continue;
			}

			if(gameRef->isoTerrainAstarGrid[i][j]->CurrentIsoSelknam != NULL) continue;
			
			if(gameRef->isoTerrainAstarGrid[i][j] == GetTile() && gameRef->isoTerrainAstarGrid[i][j]->num_locks <= 1) 
			{
				return gameRef->isoTerrainAstarGrid[i][j];
			}

			if(gameRef->isoTerrainAstarGrid[i][j]->getEnemyCanStopHere() == false) continue;
			//if(gameRef->isoTerrainAstarGrid[i][j]->getIsRamp()) continue;
			//There is a selknam on it
			if(gameRef->isoTerrainAstarGrid[i][j]->aStar_aditionalCost != 0) continue;		
			int distance = DistanceBetweenTiles(gameRef->isoTerrainAstarGrid[i][j], CurrentTile);

			if(min_distance > distance)
			{
				min_distance = distance;
				closest_tile = gameRef->isoTerrainAstarGrid[i][j];
			} 
			else if(min_distance == distance)
			{
				int closest_dist = DistanceBetweenTiles(CurrentTile, closest_tile);
				if(closest_dist > DistanceBetweenTiles(CurrentTile, gameRef->isoTerrainAstarGrid[i][j]))
				{
					closest_tile = gameRef->isoTerrainAstarGrid[i][j];
				}
			}
		}
	}

	//IwTrace(RAMP, ("Finding tile..."));
	return closest_tile;
}


void IsoEnemy::InteractWithPawn(IsoPawn* enemy)
{
	//return;

	if(bDead)
		return;

	if(has_skill_effect[(int)Skill_Type_Confusion])
		return;

	if(eBehavior == BHE_Melee_Ofensive_Defensive)
	{
		IsoEnemy_Offensive_Algorithm(enemy);
	}
	else if(eBehavior == BHE_Range_SingleFocus)
	{
		IsoEnemy_RangeSingleFocus_Algorithm(enemy);
	}
	else if(eBehavior == BHE_Suicidal)
	{
		IsoEnemy_Suicidal(enemy);
	}
}
#pragma region Enemy Algorithms
void IsoEnemy::IsoEnemy_Offensive_Algorithm(IsoPawn* enemy)
{
	//Although is in root state, he can attack if the enemy is in range.
	if(has_skill_effect[(int)Skill_Type_Root] && CheckIfCanStop()) {
		bMove = false;
		if(IsPawnInRange(enemy, NULL, 1) && (enemy->getLayer() == getLayer()))
		{
			direction = GetDirection(enemy);
			AttackPawn(enemy);
		}
		return;
	}

	if(!bMove && GetTile() == follow_tile)
	{
		//Lock Terrain makes tile unavailable for another enemy.
		if(bStun)
		{
			//IwTrace(ATTACKTRACK, ("Stunned."));
			return;
		}

		LockTerrain(GetTile());

		//The enemy has dissappear (it was sold or he dies), then go to original objective.
		if(pawnTargeted->getInitialized() == false || pawnTargeted->GetIsDead())
		{
			//IwTrace(ATTACKTRACK, ("Enemigo ha muerto."));
			pawnTargeted = CloserEnemyInRange();
			if(pawnTargeted != NULL)
			{
				follow_tile = FindClosestFreeTileTo(pawnTargeted->GetTile());
				if(follow_tile != NULL)
				{
					MoveToIsoTerrain(follow_tile);
					UnLockTerrain(GetTile());
				}
			}

			if(pawnTargeted == NULL || follow_tile == NULL)
			{
				MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
				UnLockTerrain(GetTile());
				bFollow = false;
				pawnTargeted = NULL;
			}

			return;
		}
		
		//There is an objetive. If enemy is in range, then attack.
		CurrentTile = GetTile();
		if(CheckIfCanAttack(enemy))
		{
			//IwTrace(ATTACKTRACK, ("Atacar."));
			direction = GetDirection(enemy);
			AttackPawn(enemy);

			if(GetTile()->num_locks > 1 || GetTile()->CurrentIsoSelknam != NULL)
			{
				IwTrace(ATTACKTRACK, ("Tile ocupado."));
				IsoTerrain* tempTile = FindClosestFreeTileTo(enemy->GetTile());
				if(tempTile != NULL)
				{
					//IwTrace(ENEMYBUG, ("Overlap, new tile %d %d", tempTile->getXNumTile(), tempTile->getYNumTile()));
					MoveToIsoTerrain(tempTile);
					follow_tile = tempTile;
				}
				else
				{
					MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
				}
				UnLockTerrain(GetTile());
			}
			//IwTrace(ENEMYBUG, ("%d, tile: %d %d", GetTile()->num_locks, GetTile()->getXNumTile(), GetTile()->getYNumTile()));
		}
		else
		{
			//Sanity check
			if(pawnTargeted == NULL) return;

			if(!IsPawnInRange(enemy, NULL, range))
			{
				IwTrace(ATTACKTRACK, ("Enemigo fuera del rango."));
				MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
				UnLockTerrain(GetTile());
				bFollow = false;
			}
			else
			{
				IwTrace(ATTACKTRACK, ("Enemigo en rango."));
				follow_tile = FindClosestFreeTileTo(enemy->GetTile());
				MoveToIsoTerrain(follow_tile);
				UnLockTerrain(GetTile());
			}
		}
	}
	else
	{
		if(!bFollow && CheckIfCanStop() && !enemy->bMove)
		{
			//IwTrace(RAMP, ("Following!"));
			//Tile where me and my Enemy are.
			int xEnemy = Utils::Round(enemy->getIsoPosition().x / 100);
			int yEnemy = Utils::Round(enemy->getIsoPosition().y / 100);
			int xMe    = Utils::Round(getIsoPosition().x / 100);
			int yMe    = Utils::Round(getIsoPosition().y / 100);
			int layerEnemy = enemy->getLayer();

			int xFinal = -1;
			int yFinal = -1;
			int min_distance = 99;

			follow_tile = FindClosestFreeTileTo(enemy->GetTile());
			if(follow_tile != NULL)
			{
				xFinal = follow_tile->getXNumTile();
				yFinal = follow_tile->getYNumTile();
			}
					
			//If I could found an available tile, then try to move to it.
			if(xFinal >= 0 && yFinal >= 0 && xFinal < MAX_TILESROW && yFinal < MAX_TILESROW 
			   && gameRef->isoTerrainAstarGrid[xFinal][yFinal] != NULL
			   )
			{
				IsoTerrain* ObjTile = gameRef->isoTerrainAstarGrid[xFinal][yFinal];
				CurrentTile = GetTile();

				if(ObjTile == CurrentTile)
				{
					bMove = false;
					bFollow = false;
					return;
				}
				
				bMove = aStarHelperRef->AStarRetrievePathByGrid(gameRef->isoTerrainAstarGrid,
					                                            CurrentTile, 
																ObjTile, 
																currentIsoPath);
						
				int xMe    = Utils::Round(getIsoPosition().x / 100);
				int yMe    = Utils::Round(getIsoPosition().y / 100);
				bool bValidPath = true;

				//IwTrace(RAMP, ("--------------------"));
				int new_min = 999;
				int new_min_idx = -1;
				for(int i = 0; currentIsoPath[i]; i++)
				{
					if(isValidToWalk(currentIsoPath[i]) && currentIsoPath[i]->getLayer() == ObjTile->getLayer())
					{
						int temp_new_min = DistanceBetweenTiles(enemy->GetTile(), currentIsoPath[i]);
						if(temp_new_min < new_min)
						{
							new_min = temp_new_min;
							new_min_idx = i;
						}
					}
				}

				if(new_min_idx != -1)
				{
					for(int i = new_min_idx + 1; currentIsoPath[i]; i++)
					{
						currentIsoPath[i] = NULL;
					}

					follow_tile = currentIsoPath[new_min_idx];
					ObjTile = follow_tile;
					//IwTrace(RAMP, ("(%d, %d)", currentIsoPath[new_min_idx]->getXNumTile(), currentIsoPath[new_min_idx]->getYNumTile()));
				}
				//IwTrace(RAMP, ("--------------------"));
								
				//There is no path to ObjTile
				if(currentIsoPath[0] == NULL)
				{
					bValidPath = false;
				}
				//Check every tile in the path, if is in the range is valid.
				for(int i = 0; currentIsoPath[i]; i++)
				{
					int xTile = currentIsoPath[i]->getXNumTile();
					int yTile = currentIsoPath[i]->getYNumTile();

					if(abs(xTile - xMe) > range || abs(yTile - yMe) > range)
					{
						bValidPath = false;
					}
				}
				
				int path_length = 0;
				for(; currentIsoPath[path_length]; path_length++);
				if(path_length > (range + 1))
				{
					bValidPath = false;
				}
				
				if(bValidPath)
				{
				    CurrentTile = GetTile();
				    bFollow = true;
					bRecalcPath = true;
					NextTile = currentIsoPath[0];
					direction = GetDirectionByAdyTiles(CurrentTile, NextTile);
					LastCurrentTile = CurrentTile;
					LastNextTile = NextTile;
					anim = ANIM_Walk;
					FinalTile = ObjTile;
					CurrentTile->aStar_aditionalCost = 0;
					NextTile->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
					xFollowTile = follow_tile->getXNumTile();
					yFollowTile = follow_tile->getYNumTile();
					//IwTrace(RAMP, ("(%d, %d)", ObjTile->getXNumTile(), ObjTile->getYNumTile()));
				}
				else
				{
					pawnTargeted = NULL;
					MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
				}	
			}
		}			
	}
}

void IsoEnemy::IsoEnemy_RangeSingleFocus_Algorithm(IsoPawn* enemy)
{
	if(GetTile()->getEnemyCanStopHere())
	{
		if(bMove)
		{
			bMove = false;
			bRecalcPath = true;
			LockTerrain(GetTile());
			myTerrainLock = GetTile();
		}
	}

	if(GetTile() != myTerrainLock && !GetTile()->getEnemyCanStopHere())
	{
		pawnTargeted = NULL;
	}
	else
	{
		direction = GetDirection(enemy);

		AttackPawn(enemy);
	}
}

void IsoEnemy::IsoEnemy_Suicidal(IsoPawn* enemy)
{
	//Sanity Check
	if(pawnTargeted == NULL) return;
	if(LastPawnTargeted != pawnTargeted)
	{
		IwTrace(SUICIDA, ("New target."));
		b_suicidal_moving_enemy = false;
		bIgnoreEnemies = false;
		LastPawnTargeted = pawnTargeted;
	}

	/*
	if(!pawnTargeted->getInitialized() || pawnTargeted->GetIsDead())
	{
		IwTrace(SUICIDA, ("target has dissappear."));
		bRecalcPath = true;
		b_suicidal_moving_enemy = false;
		pawnTargeted = NULL;
		bIgnoreEnemies = false;
		MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
		return;
	}
	*/

	if(bIgnoreEnemies) return;

	if(!b_suicidal_moving_enemy && CheckIfCanStop())
	{
		if(pawnTargeted != NULL)
		{
			target_terrain = pawnTargeted->GetTile();
			follow_tile = FindClosestFreeTileTo(target_terrain);
			
			IwTrace(SUICIDA, ("Objective at (%d, %d)", target_terrain->getXNumTile(), target_terrain->getYNumTile()));

			if(follow_tile != NULL) {
				MoveToIsoTerrain(follow_tile);
				b_suicidal_moving_enemy = true;
			}
			else
			{
				//MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
			}
				
			
			if(!currentIsoPath[0])
			{
				bIgnoreEnemies = true;
				MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
				return;
			}
			
		}	
	}
	else
	{
		//Sanity Check
		if(follow_tile == NULL) return;
		if(follow_tile == GetTile() && (IsPawnInRange_ByIsoPosition(pawnTargeted) || follow_tile->getIsRamp()))
		{
			IwTrace(SUICIDA, ("Explotion At: %d %d", GetTile()->getXNumTile(), GetTile()->getYNumTile()));
			TakeDamage(10000);
		}
		else if(CheckIfCanStop())
		{
			if(pawnTargeted != NULL)
			{
				if(pawnTargeted->GetTile() != target_terrain) 
				{
					IwTrace(SUICIDA, ("Enemy is not there."));
					target_terrain = pawnTargeted->GetTile();
					follow_tile = FindClosestFreeTileTo(target_terrain);
					if(follow_tile != NULL)
					{
						IwTrace(SUICIDA, ("Objective at (%d, %d)", target_terrain->getXNumTile(), target_terrain->getYNumTile()));
						MoveToIsoTerrain(follow_tile);
						/*
						if(!currentIsoPath[0])
						{
							bIgnoreEnemies = true;
							MoveToIsoTerrain(gameRef->objectivesNodes[getTargetNum() - 1]);
							return;
						}
						else
						{
							target_terrain = pawnTargeted->GetTile();
						}
						*/
					}
				}
			}
		}
	}
}
#pragma endregion Enemy Algorithms

bool IsoEnemy::IsPawnInRange_ByIsoPosition(IsoPawn* enemy)
{
	int diffX = (int)fabs(getIsoPosition().x - enemy->getIsoPosition().x);
	int diffY = (int)fabs(getIsoPosition().y - enemy->getIsoPosition().y);
	return diffX < 110 && diffY < 110;
}

bool IsoEnemy::CheckIfCanAttack(IsoPawn* enemy)
{
	if(eBehavior == BHE_Melee_Ofensive_Defensive)
	{
		if(IsPawnInRange(enemy, NULL, 1))
		{
			if(GetTile()->getIsRamp())
			{
				return true;
			}
			else
			{
				return enemy->getLayer() == getLayer();
			}
		}

		return false;
		
		//return IsPawnInRange(enemy, NULL, 1) && (enemy->getLayer() == getLayer());
	}

	return false;
}

IsoPawn* IsoEnemy::SearchNearestEnemyOnTheWholeMap()
{
	int min_manhattan_dist = 999;
	IsoSelknam* enemy = NULL; 

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		IsoSelknam* CurrentIsoSelknam = &gameRef->isoSelknamArr[i];

		if(!CurrentIsoSelknam->getInitialized())
			continue;

		if(CurrentIsoSelknam->GetSelknamType() != SELKNAM_None && !CurrentIsoSelknam->GetIsDead())
		{
			int xTile = Utils::Round(CurrentIsoSelknam->getIsoPosition().x / 100);
			int yTile = Utils::Round(CurrentIsoSelknam->getIsoPosition().y / 100);
			
			int temp_dist = DistanceBetweenTiles(GetTile(), CurrentIsoSelknam->GetTile());
			if(temp_dist < min_manhattan_dist)
			{
				min_manhattan_dist = temp_dist;
				enemy = CurrentIsoSelknam;
			}
		}
	}
	return (IsoPawn*)enemy;
}

void IsoEnemy::DoSpecificActionOnTakeDamage()
{
	if(GetEnemyType() == ENEMY_Tank)
	{
		int dx = rand() % 2;
		if(dx == 0)
			Utils::PlayFXSound(FX_Sound_METALIMPACT01);
		else
			Utils::PlayFXSound(FX_Sound_METALIMPACT02);
	}
}