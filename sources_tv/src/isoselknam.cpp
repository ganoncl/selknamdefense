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

#include "s3e.h"
#include "isoterrain.h"
#include "game.h"
#include "common.h"
#include "isoselknam.h"
#include "utils.h"

IsoSelknam::IsoSelknam()
{
	pGroup = NULL;

	//Sounds
	
}

IsoSelknam::~IsoSelknam()
{

}

void IsoSelknam::InitIsoPawn(IsoTerrain* _tile)
{
	IsoPawn::InitIsoPawn(_tile);
	_tile->CurrentIsoSelknam = this;
	_tile->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
}

void IsoSelknam::Update()
{
	IsoPawn::Update();

	if(bDead)
		return;

	if(SelknamType == SELKNAM_Warrior_Base)
	{
		//attackspeed_modifier = 8;
		//IwTrace(HASTE, ("%d %d", animations.GetTotalAnimationTime("AttackNorth"), attackspeed_modifier));
		//IwTrace(HASTE, ("%d %d", animations.GetTotalAnimationTime("AttackNorth"), attackspeed_modifier));
	}	
	//IwTrace(HASTE, ("%d %d", animations.GetTotalAnimationTime("AttackNorth"), attackspeed_modifier));
	
	SetAttackSpeed();

	//If Selknam is stopped on a tile, then set that information in the tile.
	if(!bMove && CurrentTile != NULL && CurrentTile->CurrentIsoSelknam == NULL)
	{
		CurrentTile->CurrentIsoSelknam = this;
	}

	
	if(sBehavior == BHS_Melee_Ofensive_Defensive)
	{
		if(GetTile() == spawn_tile && !bMove)
		{
			spawn_tile->setCanPutUnit(true);
		}

		if(CurObjTile != NULL && GetTile() == CurObjTile && !bMove)
		{
			CurObjTile->setCanPutUnit(true);
		}

		/*
		if(!bMove && CurObjTile != NULL)
		{
			//spawn_tile->setCanPutUnit(true);
			CurObjTile->setCanPutUnit(true);
		}
		*/
	}

	if(!bMove && UnitManagerMoveTile != NULL && GetTile() == UnitManagerMoveTile)
	{
		UnitManagerMoveTile->setCanPutUnit(true);
		UnitManagerMoveTile = NULL;
	}

	if(bPerformingMoveAction)
	{
		if(CheckIfCanStop() || GetTile()->getIsRamp())
		{
			if(LastTileMarked == NULL || LastTileMarked != GetTile())
			{
				gameRef->movementMarkMap[GetTile()->getXNumTile()][GetTile()->getYNumTile()]--;
				LastTileMarked = GetTile();
			}

			if(LastTileMarked == FinalTile)
			{
				bPerformingMoveAction = false;
			}
		}

		//gameRef->movementMarkMap[GetTile()->getXNumTile()][GetTile()->getYNumTile()]--;
	}
	
	//IwTrace(RAMP, ("(%d, %d)", GetTile()->getXNumTile(), GetTile()->getYNumTile()));
	//IwTrace(RAMP, ("ISOPOSITION: (%f, %f, %d)", getIsoPosition().x, getIsoPosition().y, getLayer()));
}

void IsoSelknam::CleanIso()
{
	IsoPawn::CleanIso();
	SelknamType = SELKNAM_None;
	enemy_to_attack = NULL;
	UnitManagerMoveTile = NULL;
	CurObjTile = NULL;
	LastTileMarked = NULL;
	bPerformingMoveAction = false;
	InitialTile = NULL;
}

void IsoSelknam::InitSelknam(IsoSelknamType _selknamType)
{
	this->SelknamType = _selknamType;
	if(SelknamType == SELKNAM_Archer_Base)
	{
		SetPawnAtributes(SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1]);
		SetIsoOffset(isoSArcherOffset);
	}
	else if(SelknamType == SELKNAM_Archer_Poisoner)
	{
		SetPawnAtributes(SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1]);
		SetIsoOffset(isoSArcherPoisonerOffset);
	}
	else if(SelknamType == SELKNAM_Archer_Retarder)
	{
		SetPawnAtributes(SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1]);
		SetIsoOffset(isoSArcherRetarderOffset);
	}
	else if(SelknamType == SELKNAM_Shaman_Healer)
	{
		SetPawnAtributes(SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1]);
		SetIsoOffset(isoSShamanHealerOffset);
	}
	else if(SelknamType == SELKNAM_Shaman_Might)
	{
		SetPawnAtributes(SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1]);
		SetIsoOffset(isoSShamanMightOffset);
	}
	else if(SelknamType == SELKNAM_Shaman_Haste)
	{
		SetPawnAtributes(SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1]);
		SetIsoOffset(isoSShamanHasteOffset);
	}
	else if(SelknamType == SELKNAM_Warrior_Base)
	{
		this->SetPawnAtributes(SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1]);
		SetIsoOffset(isoSWarriorOffset);
	}
	else if(SelknamType == SELKNAM_Warrior_Tank)
	{
		this->SetPawnAtributes(SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1]);
		SetIsoOffset(isoSWarriorTankOffset);
	}
	else if(SelknamType == SELKNAM_Warrior_Berserk)
	{
		this->SetPawnAtributes(SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1]);
		SetIsoOffset(isoSWarriorBerserkOffset);
	}
	else if(SelknamType == SELKNAM_Bowler_Stuner)
	{
		this->SetPawnAtributes(SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1]);
		SetIsoOffset(isoSBowlerStunerOffset);
	}
	else if(SelknamType == SELKNAM_Bowler_Rooter)
	{
		this->SetPawnAtributes(SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1]);
		SetIsoOffset(isoSBowlerRooterOffset);
	}
	else if(SelknamType == SELKNAM_Bowler_Confuser)
	{
		this->SetPawnAtributes(SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1]);
		SetIsoOffset(isoSBowlerConfuserOffset);
	}
	SetSelknamAnim();
}

//Adjust velocity animation according with its attackspeed
void IsoSelknam::SetAttackSpeed()
{
	//attackspeed_modifier = 0;
	//attackspeed = 0;
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

	/*
	if(SelknamType == SELKNAM_Warrior_Base)
		IwTrace(SPEEDBUG, ("AttackCooldown: %d, %d", attack_cooldown, GetAttackCooldown()));
	*/
}

void IsoSelknam::SetSelknamAnim()
{
	Utils::SetSelknamAnim(SelknamType, &animations, this);
}

void IsoSelknam::MoveToIsoTerrain(IsoTerrain* _tileFinal, bool bPermanent)
{
	if(bDead)
		return;

	bMove = aStarHelperRef->AStarRetrievePathByGrid(gameRef->isoTerrainAstarGrid, CurrentTile, _tileFinal, currentIsoPath);
	if(currentIsoPath[0])
	{
		FinalTile = _tileFinal;
		NextTile = currentIsoPath[0];
		direction = GetDirectionByAdyTiles(CurrentTile, NextTile);
		LastCurrentTile = CurrentTile;
		LastNextTile = NextTile;
		CurrentTile->CurrentIsoSelknam = NULL;
		CurrentTile->aStar_aditionalCost = 0;
		NextTile->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
		//_tileFinal->setCanPutUnit(false);
		anim = ANIM_Walk;

		if(bPermanent)
			spawn_tile = _tileFinal;
	}
}

void IsoSelknam::MoveToIsoTerrainRecordPath(IsoTerrain* _tileFinal, bool bPermanent)
{
	if(bDead)
		return;

	bMove = aStarHelperRef->AStarRetrievePathByGrid(gameRef->isoTerrainAstarGrid, CurrentTile, _tileFinal, currentIsoPath);
	if(currentIsoPath[0])
	{
		//Drawing the path.
		for(int i = 0; currentIsoPath[i]; i++)
		{
			gameRef->movementMarkMap[currentIsoPath[i]->getXNumTile()][currentIsoPath[i]->getYNumTile()]++;
		}
		bPerformingMoveAction = true;
		InitialTile = GetTile();

		FinalTile = _tileFinal;
		NextTile = currentIsoPath[0];
		direction = GetDirectionByAdyTiles(CurrentTile, NextTile);
		LastCurrentTile = CurrentTile;
		LastNextTile = NextTile;
		CurrentTile->CurrentIsoSelknam = NULL;
		CurrentTile->aStar_aditionalCost = 0;
		NextTile->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
		//_tileFinal->setCanPutUnit(false);
		anim = ANIM_Walk;

		if(bPermanent)
			spawn_tile = _tileFinal;
	}
}

IsoPawn* IsoSelknam::SearchEnemyByDebuff(Skill_Type skill)
{
	if(bDead)
		return NULL;

	if(!bCanAttack)
		return NULL;

	int curr_distance = 100000;
	int min_distance = 10000;
	IsoEnemy* CloserEnemy = NULL; 

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		IsoEnemy* CurrentIsoEnemy = &gameRef->isoEnemyArr[i];

		if(!CurrentIsoEnemy->getInitialized())
			continue;

		if(CurrentIsoEnemy->GetEnemyType() != ENEMY_None && !CurrentIsoEnemy->GetIsDead() &&
		   !CurrentIsoEnemy->has_skill_effect[(int)skill])
		{
			if(IsPawnInRange(CurrentIsoEnemy, &curr_distance)) {
				if(curr_distance < min_distance)
				{
					CloserEnemy = CurrentIsoEnemy;
					min_distance = curr_distance;
				}
			}
		}
	}

	return CloserEnemy;
}

IsoPawn* IsoSelknam::SearchPawnNearby()
{
	if(bDead)
		return NULL;

	if(!bCanAttack)
		return NULL;

	if(sBehavior == BHS_SupportActive)
		return SearchAlliesNearby();
	else if(sBehavior == BHS_SupportPasive)
		return SearchSelknamToBuff();
	else if(sBehavior == BHS_Range_NearFocus)
		return SearchEnemyByDebuff(skill);

	int curr_distance = 100000;
	int min_distance = 10000;
	IsoEnemy* CloserEnemy = NULL; 

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		IsoEnemy* CurrentIsoEnemy = &gameRef->isoEnemyArr[i];
		
		if(!CurrentIsoEnemy->getInitialized())
			continue;

		if(CurrentIsoEnemy->GetEnemyType() != ENEMY_None && !CurrentIsoEnemy->GetIsDead())
		{
			if(IsPawnInRange(CurrentIsoEnemy, &curr_distance)) {
				if(curr_distance < min_distance)
				{
					CloserEnemy = CurrentIsoEnemy;
					min_distance = curr_distance;
				}
			}
		}
	}

	return CloserEnemy;
}

IsoPawn* IsoSelknam::SearchAlliesNearby()
{
	if(bDead)
		return NULL;

	int curr_hp = 100000;
	int min_hp = 10000;
	IsoSelknam* CloserAlly = NULL; 

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		IsoSelknam* CurrentIsoSelknam = &gameRef->isoSelknamArr[i];

		if(!CurrentIsoSelknam->getInitialized())
			continue;

		if(CurrentIsoSelknam == this) continue;

		if(CurrentIsoSelknam->GetSelknamType() != SELKNAM_None && !CurrentIsoSelknam->GetIsDead() 
		   && (CurrentIsoSelknam->GetCurrentHealth() < CurrentIsoSelknam->GetMaxHealth()))
		{
			if(IsPawnInRange(CurrentIsoSelknam)) {
				curr_hp = CurrentIsoSelknam->GetCurrentHealth();
				if(curr_hp < min_hp)
				{
					CloserAlly = CurrentIsoSelknam;
					min_hp = curr_hp;
				}
			}
		}
	}

	return CloserAlly;
}

IsoPawn* IsoSelknam::SearchSelknamToBuff()
{
	if(bDead)
		return NULL;

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		IsoSelknam* CurrentIsoSelknam = &gameRef->isoSelknamArr[i];

		if(!CurrentIsoSelknam->getInitialized() || CurrentIsoSelknam == this)
			continue;

		if(CurrentIsoSelknam->GetSelknamType() != SELKNAM_None && !CurrentIsoSelknam->GetIsDead())
		{
			if(IsPawnInRange(CurrentIsoSelknam)) {
				return CurrentIsoSelknam;
			}
		}
	}

	return NULL;
}

void IsoSelknam::CastSpellOnPawn(IsoPawn* enemy)
{
	if(bDead)
		return;

	if(bCanSpellMagic && !bMagic)
	{
		IwTrace(MAGIC, ("Spelling magic lv.%d", level));
		bMagic = true;
		bCanSpellMagic = false;
		if(sBehavior == BHS_SupportActive)
			enemy->TakeBuff(skill, level);
		anim = ANIM_Attack;
	}
}

bool IsoSelknam::ChangeTargetCondition(IsoPawn* target)
{
	if(bDead)
		return true;

	if(sBehavior == BHS_Range_SingleFocus)
	{
		return (pawnTargeted->GetIsDead() || !IsPawnInRange(pawnTargeted));
	}
	else if(sBehavior == BHS_Range_NearFocus)
	{
		//return false;

		if(pawnTargeted->GetIsDead() || !IsPawnInRange(pawnTargeted))
			return true;

		if(!time_per_attack.GetIsEnd())
			return false;
		
		if(SelknamType == SELKNAM_Archer_Poisoner)
			return pawnTargeted->has_skill_effect[(int)Skill_Type_Poison];
		else if(SelknamType == SELKNAM_Archer_Retarder)
			return pawnTargeted->has_skill_effect[(int)Skill_Type_Slow];
		else if(SelknamType == SELKNAM_Bowler_Stuner)
			return pawnTargeted->has_skill_effect[(int)Skill_Type_Stun];
		else if(SelknamType == SELKNAM_Bowler_Rooter)
			return pawnTargeted->has_skill_effect[(int)Skill_Type_Root];
		else if(SelknamType == SELKNAM_Bowler_Confuser)
			return pawnTargeted->has_skill_effect[(int)Skill_Type_Confusion];
		
		return false;
	}
	else if(sBehavior == BHS_SupportActive)
	{
		return true;
	}
	else if(sBehavior == BHS_Melee_Defensive)
	{
		return (pawnTargeted->GetIsDead() || !IsPawnInRange(pawnTargeted));
	}
	else if(sBehavior == BHS_Melee_Ofensive_Defensive)
	{
		return (pawnTargeted->GetIsDead() || !IsPawnInRange(pawnTargeted));
	}
	else if(sBehavior == BHS_SupportPasive)
	{
		return true;
	}
	
	return false;
}

void IsoSelknam::VerifyBuffStatus()
{
	if(bDead)
		return;

	IsoPawn::VerifyBuffStatus();

	//A Shaman can't heals itself.
	if(SelknamType == SELKNAM_Shaman_Haste || SelknamType == SELKNAM_Shaman_Might || SelknamType == SELKNAM_Shaman_Healer)
		return;

	bool might_detected = false;
	bool haste_detected = false;

	IsoSelknam* CurrentIsoSelknam = NULL;
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		CurrentIsoSelknam = &gameRef->isoSelknamArr[i];

		if(CurrentIsoSelknam->GetSelknamType() == SELKNAM_None)
			continue;

		if(!CurrentIsoSelknam->getInitialized() || CurrentIsoSelknam->GetIsDead())
			continue;
			
		if(CurrentIsoSelknam->GetSelknamType() == SELKNAM_Shaman_Might)
		{
			int diff;
			if(CurrentIsoSelknam->IsPawnInRange(this, &diff))
			{
				if(!might_detected)
				{
					TakeBuff(Skill_Type_Might, CurrentIsoSelknam->GetLevel());
					/*
					if(!has_skill_effect[(int)Skill_Type_Might])
					{
						Utils::PlayFXSound(FX_Sound_HEAL);
					}
					*/
					might_detected = true;
				}
			}
		}
		else if(CurrentIsoSelknam->GetSelknamType() == SELKNAM_Shaman_Haste)
		{
			if(!CurrentIsoSelknam->IsPawnInRange(this))
				continue;

			if(!haste_detected)
			{
				TakeBuff(Skill_Type_Haste, CurrentIsoSelknam->GetLevel());
				/*
				if(!has_skill_effect[(int)Skill_Type_Haste])
				{
					Utils::PlayFXSound(FX_Sound_HEAL);
				}
				*/
				haste_detected = true;
			}
		}
	}

	if(!haste_detected)
	{
		attackspeed_modifier = 0;
		has_skill_effect[(int)Skill_Type_Haste] = false;
	}

	if(!might_detected)
	{
		damage_modifier = 0;
		has_skill_effect[(int)Skill_Type_Might] = false;
	}
}

void IsoSelknam::UpdateBehaviour()
{
	if(bDead)
		return;
	
	
	if(!time_per_attack.GetIsStop() && !time_per_attack.GetIsEnd())
		return;
	

	animations.GetCurrentAnimID();
	
	if(sBehavior == BHS_Range_SingleFocus)
	{
		if(!bMove)
			UpdateAttackPattern();
	}
	else if(sBehavior == BHS_Range_NearFocus)
	{
		if(!bMove)
			UpdateAttackPattern();
	}
	else if(sBehavior == BHS_SupportActive)
	{
		if(!bMove)
			UpdateAttackPattern();
	}
	else if(sBehavior == BHS_Melee_Defensive)
	{
		if(!bMove)
			UpdateAttackPattern();
	}
	else if(sBehavior == BHS_Melee_Ofensive_Defensive)
	{
		if(!bMove)
			UpdateAttackPattern();
	}
	else if(sBehavior == BHS_SupportPasive)
	{
		if(!bMove)
			UpdateAttackPattern();
	}
}

void IsoSelknam::NoPawnInteraction()
{
	if(bDead)
		return;

	IsoPawn::NoPawnInteraction();

	if(sBehavior == BHS_Melee_Ofensive_Defensive)
	{
		//Searching for a new enemy in the range.

		if(spawn_tile == NULL)
		{
			return;
		}

		animations.ResumeAnim();

		IsoEnemy* ClosedEnemy = NULL;

		int i_min = rangealt;
		int j_min = rangealt;

		int ymin_tile = spawn_tile->getYNumTile() - rangealt;
		int ymax_tile = spawn_tile->getYNumTile() + rangealt;
		int xmin_tile = spawn_tile->getXNumTile() - rangealt;
		int xmax_tile = spawn_tile->getYNumTile() - rangealt;

		for(int i = 0; i < MAX_ISO_ENEMIES; i++)
		{
			if(!gameRef->isoEnemyArr[i].getInitialized())
				continue;

			if(gameRef->isoEnemyArr[i].GetEnemyType() == ENEMY_None)
				continue;

			if(gameRef->isoEnemyArr[i].GetIsDead())
				continue;

			int xtile_enemy = Utils::Round(gameRef->isoEnemyArr[i].getIsoPosition().x / 100);
			int ytile_enemy = Utils::Round(gameRef->isoEnemyArr[i].getIsoPosition().y / 100);

			if(xtile_enemy <= xmax_tile && xtile_enemy >= xmin_tile &&
			   ytile_enemy <= ymax_tile && ytile_enemy >= ymin_tile)
			{
				int diff_x = abs(xtile_enemy - spawn_tile->getXNumTile());
				int diff_y = abs(ytile_enemy - spawn_tile->getYNumTile());

				if(diff_x < i_min || diff_y < j_min)
				{
					i_min = diff_x;
					j_min = diff_y;
					ClosedEnemy = &gameRef->isoEnemyArr[i];
				}
			}
		}

		pawnTargeted = ClosedEnemy;

		if(pawnTargeted == NULL)
		{
			pawnTargeted = PawnTakeDamageToMe;
			PawnTakeDamageToMe = NULL;
		}

		if(pawnTargeted == NULL)
		{
			//IwTrace(R1, ("No target, return to inital position"));
			//He is already in his tile.
			if(GetTile() == spawn_tile) return;
			GetTile()->setCanPutUnit(true);
			MoveToIsoTerrain(spawn_tile);
		}

		/*
		//It's not in his tile, but his tile is available so he can move to it.
		if(spawn_tile->getCanPutUnit() && spawn_tile->CurrentIsoSelknam == NULL) {
			MoveToIsoTerrain(spawn_tile);
			return;
		}

		//Spawn_tile is not available, he needs to find a new tile to return.
		if(!bMove && pawnTargeted == NULL && CurrentTile != spawn_tile) 
		{
			//IwTrace(SELKNAM_T, ("Finding a new Spawn_Tile"));
			MoveToIsoTerrain(FindClosestFreeTileTo(spawn_tile, 3), true);
		}
		*/
	}
	else if(sBehavior == BHS_SupportPasive)
	{

	}
	else if(sBehavior == BHS_Range_NearFocus)
	{

	}
}

int IsoSelknam::GetAttackTimeAnimation()
{
	if(sBehavior == BHS_SupportActive)
		return casting_speed;

	if(sBehavior == BHS_SupportPasive)
		return casting_speed;

	//All other units.
	return (attackspeed + attackspeed_modifier);
}

void IsoSelknam::AttackPawn(IsoPawn* enemy)
{
	if(time_per_attack.GetIsEnd() || time_per_attack.GetIsStop())
	{
		if(bCanAttack)
		{
			//IwTrace(SELKNAM, ("ATTACK!!!!"));
			time_per_attack.Reset();
			time_per_attack.Start();
			anim = ANIM_Attack;
			bAttack = true;
			bCanAttack = false;
			DoAttackOnce = true;
			enemy_to_attack = enemy;
			animations.ResetAnim();
		}
	}
}

void IsoSelknam::DoShoot(IsoProjectileType type, IsoPawn* enemy)
{
	//IwTrace(SELKNAM, ("Preparandome para lanzar una pieda."));

	if(enemy == NULL) return;

	//IwTrace(SELKNAM, ("Projectil lanzado a la posicion %d %d", enemy->GetTile()->getXNumTile(), enemy->GetTile()->getYNumTile()));

	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		if(gameRef->isoProjectileArr[i].GetType() == Projectile_None)
		{
			float ox = 0.0f;
			float oy = 0.0f;
			float tox = 0.0f;
			float toy = 0.0f;
			if(SelknamType == SELKNAM_Archer_Base     ||
			   SelknamType == SELKNAM_Archer_Poisoner ||
			   SelknamType == SELKNAM_Archer_Retarder)
			{
				if(direction == DIR_SouthWest)      { ox = 24.0f ; oy = 5.0f ; tox = 24.0f;}
				else if(direction == DIR_North)     { ox = 0.0f  ; oy = 12.0f; }
				else if(direction == DIR_South)     { ox = 24.0f ; oy = 0.0f ; tox = 24.0f;}
				else if(direction == DIR_East)      { ox = 0.0f  ; oy = 12.0f; }
				else if(direction == DIR_West)      { ox = 10.0f ; oy = 12.0f; }
				else if(direction == DIR_SouthEast) { ox = 6.0f  ; oy = 0.0f ; }
				else if(direction == DIR_NorthEast) { ox = -10.0f; oy = 12.0f; }
				else if(direction == DIR_NorthWest) { ox = 12.0f ; oy = 16.0f; }

				//Arrow_Shooting->Play();
				Utils::PlayFXSound(FX_Sound_ARROWSHOOTING);

				gameRef->isoProjectileArr[i].bRotate = true;

			}
			else if(SelknamType == SELKNAM_Bowler_Confuser     ||
				    SelknamType == SELKNAM_Bowler_Rooter       ||
				    SelknamType == SELKNAM_Bowler_Stuner)
			{
				if(direction == DIR_SouthWest)      { ox = 24.0f; oy = 24.0f; }
				else if(direction == DIR_North)     { ox = -12.0f;   oy = 12.0f; }
				else if(direction == DIR_South)     { ox = 24.0f; oy = 24.0f; }
				else if(direction == DIR_East)      { ox = 24.0f;   oy = 20.0f; }
				else if(direction == DIR_West)      { ox = 24.0f;  oy = 24.0f; }
				else if(direction == DIR_SouthEast) { ox = 20.0f;  oy = 24.0f; }
				else if(direction == DIR_NorthEast) { ox = -10; oy = 10.0f; }
				else if(direction == DIR_NorthWest) { ox = -10.0f;   oy = 10.0f;}

				Utils::PlayFXSound(FX_Sound_ARROWSHOOTING);

				gameRef->isoProjectileArr[i].bRotate = false;

			}
			

			if(direction == DIR_SouthWest)      { IwTrace(APPLOG, ("SouthWest")); }
			else if(direction == DIR_North)     { IwTrace(APPLOG, ("North")); }
			else if(direction == DIR_South)     { IwTrace(APPLOG, ("South")); }
			else if(direction == DIR_East)      { IwTrace(APPLOG, ("East")); }
			else if(direction == DIR_West)      { IwTrace(APPLOG, ("West")); }
			else if(direction == DIR_SouthEast) { IwTrace(APPLOG, ("SouthEast")); }
			else if(direction == DIR_NorthEast) { IwTrace(APPLOG, ("NorthEast")); }
			else if(direction == DIR_NorthWest) { IwTrace(APPLOG, ("NorthWest")); }

			//damage = 0;
			//damage_modifier = 0;

			gameRef->isoProjectileArr[i].InitInflictor(this, GetPawnAttributes(), damage + damage_modifier, skill);
			gameRef->isoProjectileArr[i].InitTarget(enemy);
			gameRef->isoProjectileArr[i].SetCorrection(ox, oy);
			gameRef->isoProjectileArr[i].SetTargetCorrection(tox, toy);
			gameRef->isoProjectileArr[i].InitProjectile(type);

			break;
		}
	}
}

void IsoSelknam::CheckIsCanDoDamageByAnimation()
{
	
	if(Utils::CanAttackByAnimation(SelknamType, animations.GetCurrentAnimID()) && DoAttackOnce)
	{
		//IwTrace(SELKNAM, ("Checking if in this animation I can deal damage..."));
		if(SelknamType == SELKNAM_Archer_Base || SelknamType == SELKNAM_Archer_Retarder || SelknamType == SELKNAM_Archer_Poisoner)
			DoShoot(Projectile_Arrow, enemy_to_attack);
		else if(SelknamType == SELKNAM_Bowler_Confuser || SelknamType == SELKNAM_Bowler_Stuner || SelknamType == SELKNAM_Bowler_Rooter)
			DoShoot(Projectile_Rock, enemy_to_attack);
		else
			enemy_to_attack->TakeDamage(damage + damage_modifier, this, GetPawnAttributes(), skill);
		DoAttackOnce = false;
		enemy_to_attack = NULL;
	}
}
void IsoSelknam::InteractWithPawn(IsoPawn* enemy)
{
	if(bDead)
		return;

	if(sBehavior == BHS_Range_SingleFocus)
	{
		direction = GetDirection(enemy);
		AttackPawn(enemy);
	}
	else if(sBehavior == BHS_Range_NearFocus)
	{
		direction = GetDirection(enemy);
		AttackPawn(enemy);
	}
	else if(sBehavior == BHS_SupportActive)
	{
		IsoDirection enemy_dir = GetDirection(enemy);
		if(enemy_dir == DIR_North || enemy_dir == DIR_South || enemy_dir == DIR_East || enemy_dir == DIR_West)
			direction = enemy_dir;
		CastSpellOnPawn(enemy);
	}
	else if(sBehavior == BHS_Melee_Defensive)
	{
		if(CheckIfCanAttack(enemy))
		{
			direction = GetDirection(enemy);
			if(!enemy->bMove || CheckSpecialAttackCases(enemy) || enemy->GetIsStun() || enemy->GetIsRoot())
			{
				AttackPawn(enemy);
			}
			else
			{
				if(!bMove)
				{
					animations.PauseAnim();
				}
			}
		}
	}
	else if(sBehavior == BHS_SupportPasive)
	{
		//anim = ANIM_Attack;
		IsoDirection enemy_dir = GetDirection(enemy);
		if(enemy_dir == DIR_North || enemy_dir == DIR_South || enemy_dir == DIR_East || enemy_dir == DIR_West)
			direction = enemy_dir;
		CastSpellOnPawn(enemy);

	}
	else if(sBehavior == BHS_Melee_Ofensive_Defensive)
	{
		//I can attack my enemy only if him is just next to me and in the same layer.
		if(CheckIfCanAttack(enemy) && !bMove)
		{
			direction = GetDirection(enemy);
			if(!enemy->bMove || CheckSpecialAttackCases(enemy) || enemy->GetIsStun() || enemy->GetIsRoot())
				AttackPawn(enemy);
			else
			{
				if(!bMove)
				{
					animations.PauseAnim();
				}
				else
				{
					animations.ResumeAnim();
				}
			}

			//IwTrace(OVERLAP, ("Overlapping: %d", GetTile()->num_locks));
			
			if((!GetTile()->getCanPutUnit() && GetTile() != CurObjTile))// || (GetTile()->CurrentIsoSelknam != this))
			{
				animations.ResumeAnim();
				//IwTrace(OVERLAP2, ("Overlap detected."));
				//pawnTargeted = NULL;
				//bUseEnemyConstraint = true;
				IsoTerrain* newObj = FindClosestFreeTileTo(enemy->GetTile());
				//bUseEnemyConstraint = false;
				if(newObj != NULL)
				{
					if(CurObjTile != NULL)
						CurObjTile->setCanPutUnit(true);
					CurObjTile = newObj;
					MoveToIsoTerrain(newObj, false);
				}
				else
				{
					pawnTargeted = NULL;
					MoveToIsoTerrain(spawn_tile);
				}
			}
			
		}
		else
		{
			animations.ResumeAnim();
			//Tell the enemy that I am targeting him, not working
			//enemy->pawnTargettingMe = this;
			//If enemy is stopped (Ready to battle)
			if(!bMove && (!enemy->bMove || CheckSpecialAttackCases(enemy, false) || enemy->GetIsStun() || enemy->GetIsRoot()))
			{
				//Tile where me and my Enemy are.
				int tile_x_enemy = Utils::Round(enemy->getIsoPosition().x / 100);
				int tile_y_enemy = Utils::Round(enemy->getIsoPosition().y / 100);
				int tile_x = Utils::Round(getIsoPosition().x / 100);
				int tile_y = Utils::Round(getIsoPosition().y / 100);

				//I know where my enemy is, now I want to find and available tile next to him to attack him.
				int final_x = -1;
				int final_y = -1;
				int dxi = 9999;
				int dyi = 9999;

				for(int i = tile_x_enemy - 1; i <= tile_x_enemy + 1; i++)
				{
					for(int j = tile_y_enemy - 1; j <= tile_y_enemy + 1; j++)
					{
						if(i >= 0 && j >= 0 && i < MAX_TILESROW && j < MAX_TILESROW)
						{
							if(gameRef->isoTerrainAstarGrid[i][j] != NULL)
							{
								if(gameRef->isoTerrainAstarGrid[i][j]->CurrentIsoSelknam != NULL) continue;
								if(gameRef->isoTerrainAstarGrid[i][j]->getCanPutUnit() == false) continue;

								if((gameRef->isoTerrainAstarGrid[i][j]->getLayer() + 1) == enemy->getLayer() || (enemy->GetTile()->getIsRamp()))
								{
									int dx = abs(tile_x - i);
									int dy = abs(tile_y - j);
									if(dxi > dx || dyi > dy)
									{
										dxi = dx;
										dyi = dy;
										final_x = i;
										final_y = j;
									}
								}
							}
						}
					}
				}
				
				//If I could found an available tile, then try to move to it.
				if(final_x >= 0 && final_y >= 0 && final_x < MAX_TILESROW && final_y < MAX_TILESROW && spawn_tile != NULL)
				{
					IsoTerrain* ObjTile = gameRef->isoTerrainAstarGrid[final_x][final_y];
					//The candidate tile is a valid one.
					if(ObjTile != NULL)
					{
						bMove = aStarHelperRef->AStarRetrievePathByGrid(gameRef->isoTerrainAstarGrid, CurrentTile, ObjTile, currentIsoPath);
						//Check if all the tiles on the path are in my movement range
						for(int i = 0; currentIsoPath[i] != NULL; i++)
						{
							int min_x = spawn_tile->getXNumTile() - rangealt;
							int max_x = spawn_tile->getXNumTile() + rangealt;
							int min_y = spawn_tile->getYNumTile() - rangealt;
							int max_y = spawn_tile->getYNumTile() + rangealt;

							if(currentIsoPath[i]->getXNumTile() < min_x ||
							   currentIsoPath[i]->getXNumTile() > max_x ||
							   currentIsoPath[i]->getYNumTile() < min_y ||
							   currentIsoPath[i]->getYNumTile() > max_y)
							{
								bMove = false;
								break;
							}
						}

						//Check if the path to the tile is too long.
						bool bValid = true;
						
						if(currentIsoPath[0])
						{
							//pruning...
							int nearest_dist = 999;
							int nearest_tile = -1;
							IsoTerrain* enemyTile = enemy->GetTile();
							for(int i = 0; currentIsoPath[i]; i++)
							{
								if(currentIsoPath[i] == enemyTile)
								{
									continue;
								}

								int temp = DistanceBetweenTiles(currentIsoPath[i], enemyTile);
								if(nearest_dist > temp)
								{
									nearest_dist = temp;
									nearest_tile = i;
								}
							}

							if(nearest_tile != -1)
							{
								for(int i = nearest_tile + 1; currentIsoPath[i]; i++)
								{
									currentIsoPath[i] = NULL;
								}
							}
						}
						
						int path_length = 0;
						for(; currentIsoPath[path_length]; path_length++);
						//IwTrace(SELKNAMBUG, ("%d", path_length));

						if(path_length > (range + 1))
						{
							bValid = false;
							for(int i = 0; currentIsoPath[i]; i++)
							{
								currentIsoPath[i] = NULL;
							}
							bMove = false;
						}

						//All right, then move to the selected tile and prepare to destroy my enemy!
						if(currentIsoPath[0] && bMove && bValid)
						{
							NextTile = currentIsoPath[0];
							direction = GetDirectionByAdyTiles(CurrentTile, NextTile);
							LastCurrentTile = CurrentTile;
							LastNextTile = NextTile;
							CurrentTile->CurrentIsoSelknam = NULL;
							animations.ResumeAnim();
							anim = ANIM_Walk;
							spawn_tile->setCanPutUnit(false);
							
							if(CurObjTile != NULL)
								CurObjTile->setCanPutUnit(true);
							CurObjTile = ObjTile;
							CurObjTile->setCanPutUnit(false);
						} 
						else
						{
							//Fail to find an available tile, so I just forget to attack and search for a new enemy.
							pawnTargeted = NULL;
							CurObjTile = NULL;
							//enemy->pawnTargettingMe = NULL;
						}
					}
				}
			}
		}
	}
}

bool IsoSelknam::CheckIfCanAttack(IsoPawn* enemy)
{
	if(sBehavior == BHS_Melee_Ofensive_Defensive || sBehavior == BHS_Melee_Defensive)
	{
		if(IsPawnInRange(enemy, NULL, 1))
		{
			if(enemy->GetTile()->getIsRamp())
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

bool IsoSelknam::CheckSpecialAttackCases(IsoPawn* enemy, bool bCheckRange)
{
	bool bInRange = false;

	if(bCheckRange)
	{
		int diffX = (int)fabs(getIsoPosition().x - enemy->getIsoPosition().x);
		int diffY = (int)fabs(getIsoPosition().y - enemy->getIsoPosition().y);

		// IwTrace(SPECIALCASE, ("%d %d", diffX, diffY));

		bInRange = diffX < 120 && diffY < 120;
	
		return enemy->bIsAttackableOnMotion && bInRange;
	}

	return enemy->bIsAttackableOnMotion;
}

void IsoSelknam::HandleDie()
{
	if(spawn_tile != NULL && !spawn_tile->getCanPutUnit())
	{
		spawn_tile->setCanPutUnit(true);
	}

	if(UnitManagerMoveTile != NULL && !UnitManagerMoveTile->getCanPutUnit())
	{
		UnitManagerMoveTile->setCanPutUnit(true);
	}

	if(CurObjTile != NULL)
	{
		CurObjTile->setCanPutUnit(true);
	}

	bool bEraseTiles = false;
	for(int i = 0; currentIsoPath[i]; i++)
	{
		if(GetTile() == currentIsoPath[i]) 
		{
			bEraseTiles = true;
		}

		if(GetTile() == InitialTile && !bEraseTiles)
		{
			bEraseTiles = true;
		}

		if(bEraseTiles)
		{
			gameRef->movementMarkMap[currentIsoPath[i]->getXNumTile()][currentIsoPath[i]->getYNumTile()]--;
		}
	}

	IsoPawn::HandleDie();
}

void IsoSelknam::CleanSpawnTile()
{
	if(SelknamType == SELKNAM_Warrior_Base)
	{
		if(spawn_tile != NULL)
		{
			spawn_tile->setCanPutUnit(true);
			spawn_tile = NULL;
		}
	}
}