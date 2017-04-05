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
#include "isopawn.h"
#include "utils.h"
#include <math.h>

IsoPawn::IsoPawn()
{
	CleanIso();
	timeToBanish.SetTimeLimit(2000);
	timerStunEffect.SetTimeLimit(100);
	timerConfusionEffect.SetTimeLimit(50);
	confusion_time_delay.SetTimeLimit(CONFUSION_STOP_TIME);
	lastID = currID = 1;

	animations.ClearAnimation();
}

IsoPawn::~IsoPawn()
{

}

void IsoPawn::InitIsoPawn(IsoTerrain* _tile)
{
	IsoObject::InitIso();
	setInitialized(true);
	CurrentTile = _tile;
	int myLayer = CurrentTile->getLayer() + 1;
	float myXPos = CurrentTile->getPosX();
	float myYPos = CurrentTile->getPosY() - 16.0f;
	float myZPos = CurrentTile->getPosZ(); //RetrieveZPos(myLayer, myYPos);
	setLayer(myLayer);
	setPosition(myXPos, myYPos, myZPos);
	timeToBanish.Reset();

	anim = ANIM_Idle;
	spawn_tile = _tile;
}

void IsoPawn::CleanIso()
{
	IsoObject::CleanIso();
	bInitialized = false;
	CleanAtributes();
	bMove = false;
	CurrentTile = NULL;
	NextTile = NULL;
	LastCurrentTile = NULL;
	LastNextTile = NULL;
	direction = DIR_None;
	for (int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		currentIsoPath[i] = NULL;
	}
	bIsInvincible = false;
	bDead = false;
	timeToBanish.Reset();
	bAttack = false;
	bCanAttack = true;
	bMagic = false;
	bCanSpellMagic = true;
	pawnTargeted = NULL;
	pawnTargettingMe = NULL;
	bRecalcPath = false;
	if(CurrentTile != NULL)
	{
		CurrentTile->CurrentIsoSelknam = NULL;
	}
	for(int i = 0; i < MAX_SKILLS + 1; i++) 
		has_skill_effect[i] = false;
	damage_modifier = 0;
	attackspeed_modifier = 0;
	speed_modifier = 0;
	bCanStop = true;
	bRoot = false;
	bStun = false;
	anim = ANIM_Idle;
	direction = DIR_South;
	last_debuff = Skill_Type_None;
	bDoEffectConfusion = false;
	FinalTile = NULL;
	bRampWalked = false;
	markedTile = NULL;
	bIsTerrainInitiallyLocked = false;
	bIsAttackableOnMotion = false;
	//bHealEffect = false;
	texture_r = texture_b = texture_g = 255;
	texture_alpha = 255;
	isoHeal_idx = -1; //Holds information about if the heal animation is rendering (!= -1) or not (==1)
	isoMight_idx = -1; //The same for Might
	isoHaste_idx = -1; //Same as above
	isoExplotion_idx = -1; //same...
	isoPoisonOnDeath_idx = -1;
	isoStun_idx = 0;
	isoConfusion_idx = 0;
	isoCoin_idx = -1;
	animations.ResumeAnim();
	bUseEnemyConstraint = false;
	PawnTakeDamageToMe =  NULL;
}

void IsoPawn::CleanAtributes()
{
	sBehavior = BHS_None;
	eBehavior = BHE_None;
	level = 0;
	health = 0;
	healthMax = 0;
	damage = 0;
	speed = 0;
	attackspeed = 0;
	attack_cooldown = 0;
	attack_cooldown_timer.Clear();
	range = 0;
	rangealt = 0;
	cost = 0;
	move_cost = 0;
	sold_value = 0;
	casting_speed = 0;
	cast_cooldown = 0;
	cast_cooldown_timer.Clear();
	skill = Skill_Type_None;
}

void IsoPawn::Update()
{
	if(!getInitialized())
		return;

	IsoObject::Update();
	

	UpdateAnimation();
	VerifyDeathStatus();

	if(bDead)
		return;

	timerStunEffect.Update();
	timerConfusionEffect.Update();
	//confusion_time_delay.Update();

	//Movement Update (Little correction due to ramps)
	if(GetTile() != NULL && GetTile()->getIsRamp()) {
		bRampWalked = true;
	}


	UpdateMovement();
	setIsoPosition();

	UpdateCooldownAttackTime();
	UpdateCooldownMagicTime();
	
	time_per_attack.Update();

	if(!time_per_attack.GetIsEnd() && !time_per_attack.GetIsStop())
		CheckIsCanDoDamageByAnimation();

	VerifyInMotionStatus();
	VerifyBuffStatus();
	VerifyDebuffStatus();

	UpdateBehaviour();
}

void IsoPawn::VerifyDeathStatus()
{
	timeToBanish.Update();

	//Banish after death
	if(bDead)
	{
		if(timeToBanish.GetIsEnd())
		{
			if(GetTile() != NULL)
				GetTile()->aStar_aditionalCost = 0;

			setTextureColor(0,0,0,0);

			if(isoPoisonOnDeath_idx == -1 && isoExplotion_idx == -1 && isoCoin_idx == -1)
			{
				CleanIso();
				IwTrace(COINBUG, ("Muerto"));
			}
		}
	}
}

void IsoPawn::UpdateCooldownAttackTime()
{
	attack_cooldown_timer.Update();

	if(!bAttack)
	{
		//IwTrace(APPLOG, ("RESET ANIM"));
		if(attack_cooldown_timer.GetIsEnd())
		{
			//IwTrace(APPLOG, ("RESET ANIM"));
			//animations.ResetAnim();
			bAttack = true;
			bCanAttack = true;
		}
	} 
	else
	{
		if(animations.AnimIsEnded())
		{
			attack_cooldown_timer.Reset();
			attack_cooldown_timer.Start();
			bAttack = false;
		}
	}

	/*
	//Cooldown attack
	if(GetIsAttacking())
	{
		if(animations.AnimIsEnded())
		{
			bAttack = false;
			bCanAttack = false;
			attack_cooldown_timer.Reset();
			attack_cooldown_timer.Start();
		}
	} 
	else
	{
		//IwTrace(APPLOG, ("%i", cooldown_timer.GetTimeLimit()));
		if(attack_cooldown_timer.GetIsEnd())
		{
			//IwTrace(APPLOG, ("Cooldown timer is over."));
			bCanAttack = true;
		}
	}
	*/
}

void IsoPawn::UpdateCooldownMagicTime()
{
	cast_cooldown_timer.Update();

	//Cooldown attack
	if(GetIsCastingMagic())
	{
		if(animations.AnimIsEnded())
		{
			animations.ResetAnim();
			bMagic = false;
			bCanSpellMagic = false;
			cast_cooldown_timer.Reset();
			cast_cooldown_timer.Start();
		}
	} 
	else
	{
		//IwTrace(APPLOG, ("%i", cooldown_timer.GetTimeLimit()));
		if(cast_cooldown_timer.GetIsEnd())
		{
			//IwTrace(APPLOG, ("Cooldown timer is over."));
			bCanSpellMagic = true;
		}
	}
}

void IsoPawn::UpdateAnimation()
{
	if(bDead)
		anim = ANIM_Death;
	

	if(anim == ANIM_Attack)
	{
		if(direction == DIR_North)           SetAttackAnim("AttackNorth");
		else if(direction == DIR_East)		 SetAttackAnim("AttackEast");
		else if(direction == DIR_South)		 SetAttackAnim("AttackSouth");
		else if(direction == DIR_West)		 SetAttackAnim("AttackWest");
		else if(direction == DIR_NorthWest)  SetAttackAnim("AttackNorthWest");
		else if(direction == DIR_NorthEast)  SetAttackAnim("AttackNorthEast");
		else if(direction == DIR_SouthWest)  SetAttackAnim("AttackSouthWest");
		else if(direction == DIR_SouthEast)  SetAttackAnim("AttackSouthEast");
	}
	else if(anim == ANIM_Death)
	{
		if(direction == DIR_North)			 animations.SetAnim("DieNorth");
		else if(direction == DIR_East)		 animations.SetAnim("DieEast");
		else if(direction == DIR_South)		 animations.SetAnim("DieSouth");
		else if(direction == DIR_West)		 animations.SetAnim("DieWest");
		else if(direction == DIR_NorthWest)  animations.SetAnim("DieNorthWest");
		else if(direction == DIR_NorthEast)  animations.SetAnim("DieNorthEast");
		else if(direction == DIR_SouthWest)  animations.SetAnim("DieSouthWest");
		else if(direction == DIR_SouthEast)  animations.SetAnim("DieSouthEast");		
	}
	else if(anim == ANIM_Walk)
	{	
		if(!SpecificAnimationOnWalk(direction))
		{
			if(direction == DIR_North)      animations.SetAnim("WalkNorth", true);
			else if(direction == DIR_East)  animations.SetAnim("WalkEast", true);
			else if(direction == DIR_South) animations.SetAnim("WalkSouth", true);
			else if(direction == DIR_West)  animations.SetAnim("WalkWest", true);
		}
	}
	else if(anim == ANIM_Idle)
	{	
		if(direction == DIR_North)           animations.SetAnim("IdleNorth");
		else if(direction == DIR_East)       animations.SetAnim("IdleEast");
		else if(direction == DIR_South)      animations.SetAnim("IdleSouth");
		else if(direction == DIR_West)       animations.SetAnim("IdleWest");
		/*
		else if(direction == DIR_NorthWest)  animations.SetAnim("IdleNorthWest");
		else if(direction == DIR_NorthEast)  animations.SetAnim("IdleNorthEast");
		else if(direction == DIR_SouthWest)  animations.SetAnim("IdleSouthWest");
		else if(direction == DIR_SouthEast)  animations.SetAnim("IdleSouthEast");
		*/
	}

	animations.NextAnim();
	SetUV(animations.GetCurrentAnimID());
}

#pragma region Movement
void IsoPawn::UpdateMovement()
{
	float rampYOffset = 0.0f;
	bool bIsRamp = false;
	float yFakePos = 0.0f;
	IsoDirection currentDir = direction;

	if(!bMove)
		return;

	if(bRoot || bStun)
		return;

	if(CurrentTile && NextTile && (CurrentTile->getIsRamp() || NextTile->getIsRamp()))
	{
		bIsRamp = true;
		rampYOffset = UpdateCurrentRampTileByDir(direction, &yFakePos);
	}

	if(!bIsRamp)
		UpdateCurrentTileByDir(direction);

	if(CurrentTile != LastCurrentTile && NextTile != LastNextTile)
	{
		currentDir = GetDirectionByAdyTiles(CurrentTile, NextTile);
		
		if(NextTile)
		{
			NextTile->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
			if(CurrentTile->CurrentIsoSelknam == NULL)
				CurrentTile->aStar_aditionalCost = 0;
			markedTile = NextTile;
		}
		LastCurrentTile = CurrentTile;
		LastNextTile = NextTile;
		//IwTrace(APPLOG, ("UpdateMovement() called, diretion %i", direction));
	}

	if (currentDir == DIR_None)
	{
		bMove = false;
		anim = ANIM_Idle;
		
	} 
	else
	{
		direction = currentDir;
	}

	//IwTrace(APPLOG, ("UpdateMovement() called, direction %i", direction));
	setVelocByIsoDirection(direction);

	float newXPos = getPosX() + Veloc.x;
	float newYPos = getPosY() + Veloc.y + rampYOffset;
	//float newZPos = RetrieveZPos(layer, newYPos, yFakePos);
	float newZPos = 0.0f;
	RetrieveZPosAndLayer(CurrentTile, NextTile, &newZPos, &layer);
	//newZPos = -0.7111*newYPos + 575.9664;
	setPosition(newXPos, newYPos, newZPos);

	/*
	if(CurrentTile != FinalTile)
		CurrentTile->aStar_aditionalCost = 0;

	if(NextTile != NULL) 
		NextTile->aStar_aditionalCost = ASTAR_ENEMYONTILECOST;
	*/
	//IwTrace(APPLOG, ("f(%f) = %f", newYPos, newZPos));
}

float IsoPawn::RetrieveZPos()
{
	if(direction == DIR_North && CurrentTile != NULL)
		return CurrentTile->getPosZ();
	
	if(direction == DIR_South && NextTile != NULL)
		return NextTile->getPosZ();

	if(direction == DIR_West && CurrentTile != NULL)
		return CurrentTile->getPosZ();

	if(direction == DIR_East && NextTile != NULL)
		return NextTile->getPosZ();

	if(CurrentTile != NULL)
		return CurrentTile->getPosZ();

	if(NextTile != NULL)
		return NextTile->getPosZ();

	return getPosZ();
}

IsoDirection IsoPawn::GetDirectionByAdyTiles(IsoTerrain* _tileSource, IsoTerrain* _tileTarget)
{
	if(!_tileSource || !_tileTarget)
		return DIR_None;
	if (_tileSource->northNode == _tileTarget)
	{
		//IwTrace(APPLOG, ("UpdateMovement() called, direction north"));
		return DIR_North;
	}
	else if (_tileSource->southNode == _tileTarget)
	{
		//IwTrace(APPLOG, ("UpdateMovement() called, direction south"));
		return DIR_South;
	}
	else if (_tileSource->eastNode == _tileTarget)
	{
		//IwTrace(APPLOG, ("UpdateMovement() called, direction east"));
		return DIR_East;
	}
	else if (_tileSource->westNode == _tileTarget)
	{
		//IwTrace(APPLOG, ("UpdateMovement() called, direction west"));
		return DIR_West;
	}
	return DIR_None;
}

bool IsoPawn::RetrieveTerrainIsoPathByTargetNum(IsoTerrain* _tile, int _targetNum)
{
	bool bResult = false;
	_targetNum-=1;
	if(_targetNum >=  MAX_ASTAROBJECTIVES)
		return false;
	
	//IwTrace(APPLOG, ("RetrieveTerrainIsoPathByTargetNum() called, current Tile, _targetNum %i", _targetNum));
	//IwTrace(APPLOG, ("RetrieveTerrainIsoPathByTargetNum() called, current Tile, xpos %f, ypos %f  ", _tile->getPosX(), _tile->getPosY()));

	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		if(_tile->isoPath[_targetNum][i])
		{
			bResult = true;
			currentIsoPath[i] = _tile->isoPath[_targetNum][i];
			//IwTrace(APPLOG, ("RetrieveTerrainIsoPathByTargetNum() called, %i, xpos %f, ypos %f ", i, currentIsoPath[i]->getPosX(), currentIsoPath[i]->getPosY()));
		}
	}
	return bResult;
}

float IsoPawn::getSpeed()
{
	float mySpeed = 0.0f;
	if(speed >= speed_modifier)
	{
		mySpeed = (float)(speed - speed_modifier);
	}
	else
	{
		mySpeed = 1;
	}
	
	mySpeed = mySpeed * 0.1f;
	//IwTrace(APPLOG, ("getSpeed() called, mySpeed %f", mySpeed));
	return mySpeed;
}

void IsoPawn::setVelocByIsoDirection(IsoDirection _myDirection)
{
	float myVelocX = 0.0f;
	float myVelocY = 0.0f;

	if(_myDirection == DIR_North)
	{
		myVelocX = getSpeed();
		myVelocY = getSpeed() * -0.5f;
	}
	else if(_myDirection == DIR_South)
	{
		myVelocX = getSpeed() * -1.0f;
		myVelocY = getSpeed() * 0.5f;
	}
	else if(_myDirection == DIR_East)
	{
		myVelocX = getSpeed();
		myVelocY = getSpeed() * 0.5f;
	}
	else if(_myDirection == DIR_West)
	{
		myVelocX = getSpeed() * -1.0f;
		myVelocY = getSpeed() * -0.5f;
	}

	setVeloc(myVelocX, myVelocY);
}

float IsoPawn::RetrieveZPos(int _layer, float _yPos, float _yFakePos)
{
	float zResult = 0.0f;
	float zResultTmp = 0.0f;
	int _yPosInt = (int)_yPos;
	if(_yFakePos != 0.0f)
	{
		_yPos = _yFakePos;
	}
	zResult = ((_yPos * -0.625f) + 560.0f) - (_layer * 10);
	zResult = zResult * 0.1f;
	zResult = floorf(zResult);
	zResult = zResult * 10.0f;
	zResult = zResult - _layer + 1;
	//post process by Y pos.
	//IwTrace(APPLOG, ("RetrieveZPos() called, layer %i, yPos %f, _yFakePos %f, zResult %f, zResultTmp %f", _layer, getPosY(), _yFakePos, zResult));
	zResultTmp = getPosY() * 0.001f;
	zResult = zResult - zResultTmp;
	//IwTrace(APPLOG, ("RetrieveZPos() called, layer %i, yPos %f, zResult %f, zResultTmp %f", _layer, getPosY(), zResult, zResultTmp));
	return zResult;
}

void IsoPawn::ApplyPositionCorrectionByTile(IsoTerrain* _tile, float _yOffset)
{
	if(!_tile)
		return;
	//_yOffset = 0;
	//if(_yOffset != 0) _yOffset += -3;
	float newXPos = _tile->getPosX();
	float newYPos = _tile->getPosY() - (16 + _yOffset);
	float newZPos = RetrieveZPos(layer, newYPos);

	//bCanStop = true;

	setPosition(newXPos, newYPos, newZPos);
}

void IsoPawn::UpdateCurrentTileByDir(IsoDirection _myDirection)
{
	float pawnXPos = getPosX();
	float nextTileXPos = 0.0f;
	bool bRetrieveNextTile = false;

	if(NextTile)
		nextTileXPos = NextTile->getPosX();
	else
		return;

	//IwTrace(APPLOG, ("UpdateCurrentTileByDir() called, pawn pos %f %f, nexttilepos %f %f", getPosX(), getPosY(), NextTile->getPosX(), NextTile->getPosY()));

	if(_myDirection == DIR_North || _myDirection == DIR_East)
	{
		if(pawnXPos >= nextTileXPos)
		{
			//IwTrace(APPLOG, ("UpdateCurrentTileByDir() called, _myDirection %i", _myDirection));
			bRetrieveNextTile = true;
			
		}
	}
	else if(_myDirection == DIR_South || _myDirection == DIR_West)
	{
		if(pawnXPos <= nextTileXPos)
		{
			//IwTrace(APPLOG, ("UpdateCurrentTileByDir() called, _myDirection %i", _myDirection));
			bRetrieveNextTile = true;
			
		}
	}
	if(bRetrieveNextTile && NextTile)
	{
		//bCanStop = true;
		
		CurrentTile->num_enemies_on_this_tile--;
		NextTile->num_enemies_on_this_tile++;

		CurrentTile = NextTile;
		for (int i = 0; i < ASTAR_LISTSMAX - 1; i++)
		{
			if(currentIsoPath[i] && NextTile == currentIsoPath[i])
			{
				if(currentIsoPath[i + 1])
					NextTile = currentIsoPath[i + 1];
				else
					NextTile = NULL;

				break;
			}
		}
		ApplyPositionCorrectionByTile(CurrentTile);
		//IwTrace(APPLOG, ("UpdateCurrentTileByDir() NextTile retrieved, fix pos"));
	}
	return;
}

float IsoPawn::UpdateCurrentRampTileByDir(IsoDirection _myDirection, float *_yFakePos)
{
	float pawnXPos = getPosX();
	float currentTileXPos = 0.0f;
	float nextTileXPos = 0.0f;
	float halfTileXPos = 0.0f;
	float xCorrection = 16.0f;
	float yCorrection = 0.0f;
	float yVelocMultiplier = 0.5f;
	float rampYOffset = 0.0f;
	bool bRetrieveNextTile = false;

	currentTileXPos = CurrentTile->getPosX();
	nextTileXPos = NextTile->getPosX();
	int nextTileLayer = NextTile->getLayer() + 1;

	//IwTrace(APPLOG, ("UpdateCurrentTileByDir() called, pawn pos %f %f, nexttilepos %f %f", getPosX(), getPosY(), NextTile->getPosX(), NextTile->getPosY()));
	//IwTrace(APPLOG, ("UpdateCurrentTileByDir() called, currentTileXPos %f, nextTileXPos %f", currentTileXPos, nextTileXPos));

	if(_myDirection == DIR_North || _myDirection == DIR_East)
	{
		halfTileXPos = nextTileXPos - xCorrection;
		//plane to up
		if(!CurrentTile->getIsRamp() && (NextTile->getIsRamp() && (NextTile->getRampConnOnSouth() || NextTile->getRampConnOnWest())))
		{
			//first half
			if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
			{
				return 0.0f;
			}
			//second half
			else if(pawnXPos >= halfTileXPos)
			{
				if(pawnXPos >= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * -yVelocMultiplier;
					return rampYOffset;
				}
			}
		}
		//plane to down
		else if(!CurrentTile->getIsRamp() && (NextTile->getIsRamp() && (NextTile->getRampConnOnNorth() || NextTile->getRampConnOnEast())))
		{
			//first half
			if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
			{
				return 0.0f;
			}
			//second half
			else if(pawnXPos >= halfTileXPos)
			{
				if(pawnXPos >= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * yVelocMultiplier;
					return rampYOffset;
				}
			}
		}
		//up to up
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnSouth() || CurrentTile->getRampConnOnWest()))
			&& (NextTile->getIsRamp() && (NextTile->getRampConnOnSouth() || NextTile->getRampConnOnWest())))
		{
			//first half
			if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
			{
				rampYOffset = getSpeed() * -yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos >= halfTileXPos)
			{
				if(pawnXPos >= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * -yVelocMultiplier;
					return rampYOffset;
				}
			}
		}
		//up to plane
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnSouth() || CurrentTile->getRampConnOnWest()))
			&& !NextTile->getIsRamp())
		{
			//first half
			if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
			{
				rampYOffset = getSpeed() * -yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos >= halfTileXPos)
			{
				if(pawnXPos >= nextTileXPos)
				{
					yCorrection = 0.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					return 0.0f;
				}
			}
		}
		//up to down
		//else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnSouth() || CurrentTile->getRampConnOnWest()))
		//	&& (NextTile->getIsRamp() && (NextTile->getRampConnOnNorth() || NextTile->getRampConnOnEast())))
		//{
		//	//first half
		//	if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
		//	{
		//		rampYOffset = getSpeed() * -yVelocMultiplier;
		//		return rampYOffset;
		//	}
		//	//second half
		//	else if(pawnXPos >= halfTileXPos)
		//	{
		//		if(pawnXPos >= nextTileXPos)
		//		{
		//			yCorrection = -8.0f;
		//			bRetrieveNextTile = true;
		//		}
		//		else
		//		{
		//			rampYOffset = getSpeed() * yVelocMultiplier;
		//			return rampYOffset;
		//		}
		//	}
		//}
		//down to up
		//else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnNorth() || CurrentTile->getRampConnOnEast()))
		//	&& (NextTile->getIsRamp() && (NextTile->getRampConnOnSouth() || NextTile->getRampConnOnWest())))
		//{
		//	//first half
		//	if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
		//	{
		//		rampYOffset = getSpeed() * yVelocMultiplier;
		//		return rampYOffset;
		//	}
		//	//second half
		//	else if(pawnXPos >= halfTileXPos)
		//	{
		//		if(pawnXPos >= nextTileXPos)
		//		{
		//			yCorrection = -8.0f;
		//			bRetrieveNextTile = true;
		//		}
		//		else
		//		{
		//			rampYOffset = getSpeed() * -yVelocMultiplier;
		//			return rampYOffset;
		//		}
		//	}
		//}
		//down to plane
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnNorth() || CurrentTile->getRampConnOnEast()))
			&& !NextTile->getIsRamp())
		{
			//first half
			if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
			{
				rampYOffset = getSpeed() * yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos >= halfTileXPos)
			{
				if(pawnXPos >= nextTileXPos)
				{
					yCorrection = 0.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					return 0.0f;
				}
			}
		}
		//down to down
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnNorth() || CurrentTile->getRampConnOnEast()))
			&& (NextTile->getIsRamp() && (NextTile->getRampConnOnNorth() || NextTile->getRampConnOnEast())))
		{
			//first half
			if(pawnXPos >= currentTileXPos && pawnXPos < halfTileXPos)
			{
				rampYOffset = getSpeed() * yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos >= halfTileXPos)
			{
				if(pawnXPos >= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * yVelocMultiplier;
					return rampYOffset;
				}
			}
		}		
	}
	else if(_myDirection == DIR_South || _myDirection == DIR_West)
	{
		halfTileXPos = nextTileXPos + xCorrection;
		//plane to up
		if(!CurrentTile->getIsRamp() && (NextTile->getIsRamp() && (NextTile->getRampConnOnNorth() || NextTile->getRampConnOnEast())))
		{
			//first half
			if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
			{
				return 0.0f;
			}
			//second half
			else if(pawnXPos <= halfTileXPos)
			{
				if(pawnXPos <= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * -yVelocMultiplier;
					return rampYOffset;
				}
			}
		}
		//plane to down
		else if(!CurrentTile->getIsRamp() && (NextTile->getIsRamp() && (NextTile->getRampConnOnSouth() || NextTile->getRampConnOnWest())))
		{
			//first half
			if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
			{
				return 0.0f;
			}
			//second half
			else if(pawnXPos <= halfTileXPos)
			{
				if(pawnXPos <= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * yVelocMultiplier;
					return rampYOffset;
				}
			}
		}
		//up to up
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnNorth() || CurrentTile->getRampConnOnEast()))
			&& (NextTile->getIsRamp() && (NextTile->getRampConnOnNorth() || NextTile->getRampConnOnEast())))
		{
			//first half
			if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
			{
				rampYOffset = getSpeed() * -yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos <= halfTileXPos)
			{
				if(pawnXPos <= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * -yVelocMultiplier;
					return rampYOffset;
				}
			}
		}
		//up to plane
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnNorth() || CurrentTile->getRampConnOnEast()))
			&& !NextTile->getIsRamp())
		{
			//first half
			if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
			{
				rampYOffset = getSpeed() * -yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos <= halfTileXPos)
			{
				if(pawnXPos <= nextTileXPos)
				{
					yCorrection = 0.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					return 0.0f;
				}
			}
		}
		////up to down
		//else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnNorth() || CurrentTile->getRampConnOnEast()))
		//	&& (NextTile->getIsRamp() && (NextTile->getRampConnOnSouth() || NextTile->getRampConnOnWest())))
		//{
		//	//first half
		//	if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
		//	{
		//		rampYOffset = getSpeed() * -yVelocMultiplier;
		//		return rampYOffset;
		//	}
		//	//second half
		//	else if(pawnXPos <= halfTileXPos)
		//	{
		//		if(pawnXPos <= nextTileXPos)
		//		{
		//			yCorrection = -8.0f;
		//			bRetrieveNextTile = true;
		//		}
		//		else
		//		{
		//			rampYOffset = getSpeed() * yVelocMultiplier;
		//			return rampYOffset;
		//		}
		//	}
		//}
		////down to up
		//else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnSouth() || CurrentTile->getRampConnOnWest()))
		//	&& (NextTile->getIsRamp() && (NextTile->getRampConnOnNorth() || NextTile->getRampConnOnEast())))
		//{
		//	//first half
		//	if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
		//	{
		//		rampYOffset = getSpeed() * yVelocMultiplier;
		//		return rampYOffset;
		//	}
		//	//second half
		//	else if(pawnXPos <= halfTileXPos)
		//	{
		//		if(pawnXPos <= nextTileXPos)
		//		{
		//			yCorrection = -8.0f;
		//			bRetrieveNextTile = true;
		//		}
		//		else
		//		{
		//			rampYOffset = getSpeed() * -yVelocMultiplier;
		//			return rampYOffset;
		//		}
		//	}
		//}
		//down to plane
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnSouth() || CurrentTile->getRampConnOnWest()))
			&& !NextTile->getIsRamp())
		{
			//first half
			if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
			{
				rampYOffset = getSpeed() * yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos <= halfTileXPos)
			{
				if(pawnXPos <= nextTileXPos)
				{
					yCorrection = 0.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					return 0.0f;
				}
			}
		}
		//down to down
		else if((CurrentTile->getIsRamp() && (CurrentTile->getRampConnOnSouth() || CurrentTile->getRampConnOnWest()))
			&& (NextTile->getIsRamp() && (NextTile->getRampConnOnSouth() || NextTile->getRampConnOnWest())))
		{
			//first half
			if(pawnXPos <= currentTileXPos && pawnXPos > halfTileXPos)
			{
				rampYOffset = getSpeed() * yVelocMultiplier;
				return rampYOffset;
			}
			//second half
			else if(pawnXPos <= halfTileXPos)
			{
				if(pawnXPos <= nextTileXPos)
				{
					yCorrection = -8.0f;
					bRetrieveNextTile = true;
				}
				else
				{
					rampYOffset = getSpeed() * yVelocMultiplier;
					return rampYOffset;
				}
			}
		}	
	}
	if(bRetrieveNextTile && NextTile)
	{
		CurrentTile = NextTile;
		for (int i = 0; i < ASTAR_LISTSMAX - 1; i++)
		{
			if(currentIsoPath[i] && NextTile == currentIsoPath[i])
			{
				if(currentIsoPath[i + 1])
					NextTile = currentIsoPath[i + 1];
				else
					NextTile = NULL;

				break;
			}
		}
		ApplyPositionCorrectionByTile(CurrentTile, yCorrection);
		//IwTrace(APPLOG, ("UpdateCurrentTileByDir() NextTile retrieved, fix pos"));
	}
	return rampYOffset;
}

void IsoPawn::SetPawnAtributes(PawnAttributes _newPawnAtributes)
{
	sBehavior = _newPawnAtributes.sBehavior;
	eBehavior = _newPawnAtributes.eBehavior;
	level = _newPawnAtributes.level;
	health = _newPawnAtributes.health;
	healthMax = _newPawnAtributes.health;
	damage = _newPawnAtributes.damage;
	speed = _newPawnAtributes.speed;
	attackspeed = _newPawnAtributes.attackspeed;
	attack_cooldown = _newPawnAtributes.attack_cooldown;
	attack_cooldown_timer.SetTimeLimit(attack_cooldown * 100);
	range = _newPawnAtributes.range;
	rangealt = _newPawnAtributes.rangealt;
	cost = _newPawnAtributes.cost;
	move_cost = _newPawnAtributes.move_cost;
	sold_value = _newPawnAtributes.sold_value;
	coins_perkill = _newPawnAtributes.coins_perkill;
	casting_speed = _newPawnAtributes.casting_speed;
	cast_cooldown = _newPawnAtributes.cast_cooldown;	
	cast_cooldown_timer.SetTimeLimit(cast_cooldown * 100);	
	skill = _newPawnAtributes.skill;
	time_per_attack.SetTimeLimit(attack_cooldown * 100);
}

IsoDirection IsoPawn::GetDirection(IsoPawn* pawn)
{
	CIwFVec3 pawn_position = pawn->getIsoPosition();
	float _diffX = pawn_position.x - this->getIsoPosition().x;
	float _diffY = pawn_position.y - this->getIsoPosition().y;

	int tile_x_me    = Utils::Round(getIsoPosition().x / 100);
	int tile_y_me    = Utils::Round(getIsoPosition().y / 100);
	int tile_x_enemy = Utils::Round(pawn->getIsoPosition().x / 100);
	int tile_y_enemy = Utils::Round(pawn->getIsoPosition().y / 100);

	//IwTrace(APPLOG, ("GETDIRECION (%f , %f)", _diffX, _diffY));
	//IwTrace(APPLOG, ("GETDIRECION (%d , %d)", tile_x_me, tile_y_me));
	/*
	IwTrace(APPLOG, ("======================="));
	IwTrace(APPLOG, ("GETDIRECION (%f , %f)", getIsoPosition().x / 100, getIsoPosition().y / 100));
	IwTrace(APPLOG, ("GETDIRECION (%d , %d)", tile_x_me, tile_y_me));
	IwTrace(APPLOG, ("GETDIRECION (%d , %d)", tile_x_me - tile_x_enemy, tile_y_me - tile_y_enemy));
	IwTrace(APPLOG, ("======================="));
	*/
	int dx = tile_x_me - tile_x_enemy;
	int dy = tile_y_me - tile_y_enemy;

	//return DIR_NorthWest;

	if(dy == 0)
	{
		if(dx > 0)
		{
			return DIR_West;
		}

		if(dx < 0)
		{
			return DIR_East;
		}
	}

	if(dx == 0)
	{
		if(dy > 0)
		{
			return DIR_North;
		}

		if(dy < 0)
		{
			return DIR_South;
		}
	}

	if(dx > 0 && dy > 0)
	{
		return DIR_NorthWest;
	}

	if(dx < 0 && dy < 0)
	{
		return DIR_SouthEast;
	}

	if(dx > 0 && dy < 0)
	{
		return DIR_SouthWest;
	}

	if(dx < 0 && dy > 0)
	{
		return DIR_NorthEast;
	}

	return direction;
}

IsoDirection IsoPawn::GetDirectionToIsoTerrain(IsoTerrain* tile)
{
	if (tile == NULL)
	{
		return DIR_None;
	}

	int tile_x_me    = Utils::Round(getIsoPosition().x / 100);
	int tile_y_me    = Utils::Round(getIsoPosition().y / 100);
	int tile_x_enemy = tile->getXNumTile();
	int tile_y_enemy = tile->getYNumTile();

	//IwTrace(APPLOG, ("GETDIRECION (%f , %f)", _diffX, _diffY));
	//IwTrace(APPLOG, ("GETDIRECION (%d , %d)", tile_x_me, tile_y_me));
	/*
	IwTrace(APPLOG, ("======================="));
	IwTrace(APPLOG, ("GETDIRECION (%f , %f)", getIsoPosition().x / 100, getIsoPosition().y / 100));
	IwTrace(APPLOG, ("GETDIRECION (%d , %d)", tile_x_me, tile_y_me));
	IwTrace(APPLOG, ("GETDIRECION (%d , %d)", tile_x_me - tile_x_enemy, tile_y_me - tile_y_enemy));
	IwTrace(APPLOG, ("======================="));
	*/
	int dx = tile_x_me - tile_x_enemy;
	int dy = tile_y_me - tile_y_enemy;

	//return DIR_NorthWest;

	if(dy == 0)
	{
		if(dx > 0)
		{
			return DIR_West;
		}

		if(dx < 0)
		{
			return DIR_East;
		}
	}

	if(dx == 0)
	{
		if(dy > 0)
		{
			return DIR_North;
		}

		if(dy < 0)
		{
			return DIR_South;
		}
	}

	if(dx > 0 && dy > 0)
	{
		return DIR_NorthWest;
	}

	if(dx < 0 && dy < 0)
	{
		return DIR_SouthEast;
	}

	if(dx > 0 && dy < 0)
	{
		return DIR_SouthWest;
	}

	if(dx < 0 && dy > 0)
	{
		return DIR_NorthEast;
	}

	return direction;
}

void IsoPawn::RetrieveZPosAndLayer(IsoTerrain* Tile1, IsoTerrain* Tile2, float* newZPos, int* newLayer)
{
	if(Tile1 == NULL && Tile2 == NULL)
		return;

	if(Tile1 == NULL)
	{
		*newZPos = Tile2->getPosZ();
		*newLayer = Tile2->getLayer() + 1;
	} 
	else if(Tile2 == NULL)
	{
		*newZPos = Tile1->getPosZ();
		*newLayer = Tile1->getLayer() + 1;
	}
	else 
	{
		if(Tile1->getPosZ() < Tile2->getPosZ())
		{
			*newZPos = Tile1->getPosZ();
			*newLayer = Tile1->getLayer() + 1;
		}
		else
		{
			*newZPos = Tile2->getPosZ();
			*newLayer = Tile2->getLayer() + 1;
		}
	}
	PostProcessZPos(newZPos);
}

void IsoPawn::PostProcessZPos(float* newZPos)
{
	float zResultTmp = getPosY() * 0.001f;
	*newZPos = *newZPos - zResultTmp;
}
#pragma endregion

void IsoPawn::SetUV(int id)
{
	int renderIdx = 0;
	if(id >= atlasMaxID)
	{
		currID = 2;

		id -= atlasMaxID;
		renderIdx++;
	}
	else
	{
		currID = 1;
	}

	int id_x = id % atlasInfo[renderIdx].rowSize;
	int id_y = (int)(id / atlasInfo[renderIdx].rowSize);

	this->setUvMaxX(atlasInfo[renderIdx].uvWidth * (id_x + 1));
	this->setUvMinX(atlasInfo[renderIdx].uvWidth * id_x);
	this->setUvMaxY(atlasInfo[renderIdx].uvHeight * (id_y + 1));
	this->setUvMinY(atlasInfo[renderIdx].uvHeight * id_y);
}

void IsoPawn::SetAttackAnim(string anim_name)
{
	if(animations.GetCurrenAnimName() != anim_name) 
		animations.SetAnim(anim_name, false, GetAttackTimeAnimation()*100);
}

void IsoPawn::HandleDie()
{
	if(bDead)
		return;

	if(GetTile() != NULL)
	{
		GetTile()->CurrentIsoSelknam = NULL;
	}

	//When pawn is dead, all buff/debuff are removed.
	for(int i = 0; i < 11; i++)
	{
		has_skill_effect[i] = false;
	}

	pawnTargettingMe = NULL;
	pawnTargeted = NULL;
	animations.ResumeAnim();
	anim = ANIM_Death;
	bDead = true;
	bMove = false;
	bCanAttack = false;
	bCanSpellMagic = false;
	if(timeToBanish.GetIsStop())
	{
		//timeToBanish.SetTimeLimit(time_banish_limit);
		timeToBanish.Reset();
		timeToBanish.Start();
	}
}

bool IsoPawn::IsPawnInRange(IsoPawn* enemy, int* distance, int altRange)
{
	int xtile_enemy = Utils::Round(enemy->getIsoPosition().x / 100);
	int ytile_enemy = Utils::Round(enemy->getIsoPosition().y / 100);
	int xtile_me = Utils::Round(getIsoPosition().x / 100);
	int ytile_me = Utils::Round(getIsoPosition().y / 100);
	int xdiff = abs(xtile_enemy - xtile_me);
	int ydiff = abs(ytile_enemy - ytile_me);
	
	if(distance != NULL)
		*distance = xdiff + ydiff;

	int tmp_range = (altRange != -1) ? altRange : range;

	if(xdiff <= tmp_range && ydiff <= tmp_range)
		return true;
	
	return false;	
}

void IsoPawn::UpdateAttackPattern()
{
	//Standard pattern
	if(pawnTargeted != NULL && ChangeTargetCondition(pawnTargeted))
		pawnTargeted = NULL;
	
	if(pawnTargeted == NULL)
		pawnTargeted = SearchPawnNearby();
		
	if(pawnTargeted != NULL)
		InteractWithPawn(pawnTargeted);
	else
		NoPawnInteraction();
}

void IsoPawn::VerifyBuffStatus()
{
	//nothing..
}

void IsoPawn::VerifyDebuffStatus()
{
	bool has_atleast_one_debuff = false;

	if(has_skill_effect[(int)Skill_Type_Poison])
	{
		has_atleast_one_debuff = true;
		poison_interval_time.Update();

		if(curPoison <= 106)
			setTextureColor(79, curPoison, 144);
		else 
			setTextureColor(curPoison, curPoison, curPoison);
		
		curPoison += delta_poison;
		if(curPoison <= 49)
		{
			curPoison = 49;
			delta_poison = -delta_poison;
			//delta_poison = 0;
		}
		else if(curPoison >= 255)
		{
			curPoison = 255;
			delta_poison = -delta_poison;
		}
		

		if(poison_interval_time.GetIsEnd())
		{
			TakeDamage(poison_damage);
			poison_num_interval--;
			if(poison_num_interval <= 0)
			{
				has_skill_effect[(int)Skill_Type_Poison] = false;
				setTextureColor(255,255,255);
			}
			else
			{
				poison_interval_time.Reset();
				poison_interval_time.Start();
			}
		}
	}

	if(has_skill_effect[(int)Skill_Type_PoisonOnDeath])
	{
		has_atleast_one_debuff = true;
		poison_ondeath_interval_time.Update();

		if(curPoison <= 106)
			setTextureColor(120, curPoison, 120);
		else 
			setTextureColor(curPoison, curPoison, curPoison);

		curPoison += delta_poison;
		if(curPoison <= 0)
		{
			curPoison = 0;
			delta_poison = -delta_poison;
		}
		else if(curPoison >= 255)
		{
			curPoison = 255;
			delta_poison = -delta_poison;
		}

		if(poison_ondeath_interval_time.GetIsEnd())
		{
			TakeDamage(poison_ondeath_damage);
			poison_ondeath_num_interval--;
			if(poison_ondeath_num_interval <= 0)
			{
				has_skill_effect[(int)Skill_Type_PoisonOnDeath] = false;
				setTextureColor(255, 255, 255);
			}
			else
			{
				poison_ondeath_interval_time.Reset();
				poison_ondeath_interval_time.Start();
			}
		}
	}

	if(has_skill_effect[(int)Skill_Type_Slow])
	{
		has_atleast_one_debuff = true;
		slow_time.Update();

		if(curSlow <= 130)
			setTextureColor(curSlow, 185, 215);
		else 
			setTextureColor(curSlow, curSlow, curSlow);

		curSlow += delta_slow;
		if(curSlow <= 0)
		{
			curSlow = 0;
			delta_slow = -delta_slow;
		}
		else if(curSlow >= 255)
		{
			curSlow = 255;
			delta_slow = -delta_slow;
		}

		if(slow_time.GetIsEnd())
		{
			speed_modifier = 0;
			has_skill_effect[(int)Skill_Type_Slow] = false;
			setTextureColor(255, 255, 255);
		}
	}

	if(has_skill_effect[(int)Skill_Type_Stun])
	{
		has_atleast_one_debuff = true;
		stun_time.Update();

		if(timerStunEffect.GetIsEnd())
		{
			isoStun_idx++;
			if(isoStun_idx >= 4)
				isoStun_idx = 0;
			timerStunEffect.Reset();
			timerStunEffect.Start();
		}


		if(stun_time.GetIsEnd())
		{
			has_skill_effect[(int)Skill_Type_Stun] = false;
			timerStunEffect.Stop();
			DoSpecificActionOnEndDebuff(Skill_Type_Stun);
			bStun = false;
		}
	}

	if(has_skill_effect[(int)Skill_Type_Root])
	{
		has_atleast_one_debuff = true;
		root_time.Update();

		if(root_time.GetIsEnd())
		{
			has_skill_effect[(int)Skill_Type_Root] = false;
			DoSpecificActionOnEndDebuff(Skill_Type_Root);
		}
	}

	if(has_skill_effect[(int)Skill_Type_Confusion])
	{
		has_atleast_one_debuff = true;

		//Animation.
		if(timerConfusionEffect.GetIsEnd())
		{
			isoConfusion_idx++;
			if(isoConfusion_idx > 9)
				isoConfusion_idx = 0;
			timerConfusionEffect.Reset();
			timerConfusionEffect.Start();
		}

		confusion_time.Update();
		
		//Algorithm
		if(confusion_time.GetIsEnd())
		{
			//Confusion effect is gone, so the unit can do its normal behaviour.
			has_skill_effect[(int)Skill_Type_Confusion] = false;
			DoSpecificActionOnEndDebuff(Skill_Type_Confusion);
			//Stop the timer, just for security.
			confusion_time.Stop();
		}
		else
		{
			//If the unit is confused, then he is going to move to a random tile adyacent to his current position.
			if(CheckIfCanStop())
			{
				MoveToRandomTerrain();
			}
		}


		/*
		if(bDoEffectConfusion && CheckIfCanStop())
		{
			MoveToRandomTerrain();
			bDoEffectConfusion = false;
			//confusion_time_remaining--;
			bResetConfusionTimer = true;
			IwTrace(CONFBUG, ("First move."));
		}

		
		if(confusion_time_remaining > 0)
		{
			if(CheckIfCanStop())
			{	
				if(confusion_time_delay.GetIsEnd() && !bResetConfusionTimer)
				{
					MoveToRandomTerrain();
					confusion_time_remaining--;
					bResetConfusionTimer = true;
					confusion_time_delay.Stop();
				}
				else
				{
					if(bResetConfusionTimer)
					{
						confusion_time_delay.Reset();
						confusion_time_delay.Start();
						bResetConfusionTimer = false;
					}
					else
					{
						confusion_time_delay.Update();
					}
				}
			}
		}
		else
		{
			has_skill_effect[(int)Skill_Type_Confusion] = false;
			DoSpecificActionOnEndDebuff(Skill_Type_Confusion);
		}
		*/
	}

	if(!has_atleast_one_debuff)
		last_debuff = Skill_Type_None;
}

void IsoPawn::TakeDamage(int damage, IsoPawn* inflictor, PawnAttributes inflictor_att, Skill_Type skill_type)
{
	if(this == NULL) return;

	if(bDead)
		return;

	if(bIsInvincible)
		return;

	PawnTakeDamageToMe = inflictor;

	if(skill_type == Skill_Type_Poison && !has_skill_effect[(int)Skill_Type_Poison])
	{
		delta_poison = 10;
		curPoison = 1;

		DoSpecificActionOnDebuff(skill_type);
		last_debuff = skill_type;

		has_skill_effect[(int)skill_type] = true;
		poison_damage = Skill_Att_Poison[inflictor_att.level - 1].damage_per_interval;
		poison_num_interval = Skill_Att_Poison[inflictor_att.level - 1].num_intervals;
		poison_interval_time.SetTimeLimit(Skill_Att_Poison[inflictor_att.level - 1].interval_time * 100);
		poison_interval_time.Reset();
		poison_interval_time.Start();
	}
	else if(skill_type == Skill_Type_PoisonOnDeath && !has_skill_effect[(int)Skill_Type_PoisonOnDeath])
	{
		delta_poison = 10;
		curPoison = 1;

		DoSpecificActionOnDebuff(skill_type);
		last_debuff = skill_type;

		has_skill_effect[(int)skill_type] = true;
		poison_ondeath_damage = Skill_Att_PoisonOnDeath[inflictor_att.level - 1].damage_per_interval;
		poison_ondeath_num_interval = Skill_Att_PoisonOnDeath[inflictor_att.level - 1].num_intervals;
		poison_ondeath_interval_time.SetTimeLimit(Skill_Att_PoisonOnDeath[inflictor_att.level - 1].interval_time * 100);
		poison_ondeath_interval_time.Reset();
		poison_ondeath_interval_time.Start();
	}
	else if(skill_type == Skill_Type_Slow && !has_skill_effect[(int)Skill_Type_Slow])
	{
		delta_slow = -10;
		curSlow = 255;

		DoSpecificActionOnDebuff(skill_type);
		last_debuff = skill_type;

		has_skill_effect[(int)skill_type] = true;
		speed_modifier = Skill_Att_Slow[inflictor_att.level - 1].debuff_slow_amount;
		slow_time.SetTimeLimit(Skill_Att_Slow[inflictor_att.level - 1].debuff_time * 100);
		slow_time.Reset();
		slow_time.Start();
	}
	else if(skill_type == Skill_Type_Stun && !has_skill_effect[(int)Skill_Type_Stun])
	{
		//IwTrace(STUNBUG, ("Stunned."));
		bStun = true;
		DoSpecificActionOnDebuff(skill_type);
		last_debuff = skill_type;

		has_skill_effect[(int)skill_type] = true;
		timerStunEffect.Reset();
		timerStunEffect.Start();
		stun_time.SetTimeLimit(Skill_Att_Stun[inflictor->level - 1].debuff_time * 100);
		stun_time.Reset();
		stun_time.Start();
	}
	else if(skill_type == Skill_Type_Root && !has_skill_effect[(int)Skill_Type_Root])
	{
		bRoot = true;
		DoSpecificActionOnDebuff(skill_type);
		last_debuff = skill_type;

		has_skill_effect[(int)skill_type] = true;
		root_time.SetTimeLimit(Skill_Att_Root[inflictor->level - 1].debuff_time * 100);
		root_time.Reset();
		root_time.Start();
	}
	else if(skill_type == Skill_Type_Confusion && !has_skill_effect[(int)Skill_Type_Confusion])
	{
		has_skill_effect[(int)skill_type] = true;

		//Duration of each confusion animation frame.
		timerConfusionEffect.Reset();
		timerConfusionEffect.Start();

		DoSpecificActionOnDebuff(skill_type);
		last_debuff = skill_type;


		bDoEffectConfusion = true;
		bResetConfusionTimer = true;

		//confusion_time_remaining = CONFUSION_TIMES;
		
		confusion_time.SetTimeLimit(Skill_Att_Confusion[inflictor->level - 1].debuff_time * 100);
		confusion_time.Reset();
		confusion_time.Start();
	}

	health -= damage;
	DoSpecificActionOnTakeDamage();

	if(health <= 0)
	{
		health = 0;
		bIsInvincible = true;
		HandleDie();
	}
}

void IsoPawn::MoveToRandomTerrain()
{
	srand(time(NULL));

	int dx[] = {0, 0, 1, -1};
	int dy[] = {1, -1, 0, 0};
	
	int dir = rand() % 4;
	int num_dir = 4;
	
	int xTile = GetTile()->getXNumTile();
	int yTile = GetTile()->getYNumTile();

	while(num_dir--)
	{
		int nTileX = xTile + dx[dir];
		int nTileY = yTile + dy[dir];

		if(nTileX >= 0 && nTileY >= 0 && nTileY < MAX_TILESROW && nTileX < MAX_TILESROW) 
		{
			if(gameRef->isoTerrainAstarGrid[nTileX][nTileY] != NULL)
			{
				if(gameRef->isoTerrainAstarGrid[nTileX][nTileY]->getEnemyCanStopHere())
				{
					if(gameRef->isoTerrainAstarGrid[nTileX][nTileY]->CurrentIsoSelknam == NULL) 
					{
						if(!gameRef->isoTerrainAstarGrid[nTileX][nTileY]->getIsRamp())
						{
							UnLockTerrain(GetTile());
							MoveToIsoTerrain(gameRef->isoTerrainAstarGrid[nTileX][nTileY]);
						}
					}
				}
			}
		}
		else
		{
			dir++;
			if(dir >= 4)
				dir = 0;
			//dir = (++dir % 4);
		}


		/*
		if(i < 0 || j < 0 || i >= MAX_TILESROW || j >= MAX_TILESROW) continue;
		if(gameRef->isoTerrainAstarGrid[i][j] == NULL) continue;

		if(!gameRef->isoTerrainAstarGrid[i][j]->getIsRamp())
		{
		if(gameRef->isoTerrainAstarGrid[i][j]->getLayer() != tile->getLayer()) continue;
		}

		if(gameRef->isoTerrainAstarGrid[i][j] == GetTile() && gameRef->isoTerrainAstarGrid[i][j]->num_locks <= 1) 
		{
		return gameRef->isoTerrainAstarGrid[i][j];
		}

		if(gameRef->isoTerrainAstarGrid[i][j]->getEnemyCanStopHere() == false) continue;
		//if(gameRef->isoTerrainAstarGrid[i][j]->getIsRamp()) continue;
		//There is a selknam on it
		if(gameRef->isoTerrainAstarGrid[i][j]->CurrentIsoSelknam != NULL) continue;
		if(gameRef->isoTerrainAstarGrid[i][j]->aStar_aditionalCost != 0) continue;	
		*/
	}

	/*
	int xTile = rand() % MAX_TILESROW;
	int yTile = rand() % MAX_TILESROW;

	while(gameRef->isoTerrainAstarGrid[xTile][yTile] == NULL || 
		!gameRef->isoTerrainAstarGrid[xTile][yTile]->getCanPutUnit())
	{
		xTile = rand() % MAX_TILESROW;
		yTile = rand() % MAX_TILESROW;
	}

	MoveToIsoTerrain(FindClosestFreeTileTo(gameRef->isoTerrainAstarGrid[xTile][yTile], 3));
	*/
}

void IsoPawn::TakeDamage(int damage)
{
	if(bDead)
		return;

	if(bIsInvincible)
		return;

	health -= damage;

	if(health <= 0)
	{
		health = 0;
		bIsInvincible = true;
		HandleDie();
	}
}

IsoPawn* IsoPawn::SearchPawnNearby()
{
	return NULL;
}

void IsoPawn::InteractWithPawn(IsoPawn* target)
{
	//nothing...
}

void IsoPawn::VerifyInMotionStatus()
{
	if(!CheckIfCanStop())
		return;
}

void IsoPawn::TakeBuff(Skill_Type skill_type, int _level)
{
	if(skill_type == Skill_Type_Heal)
	{
		IwTrace(ATT, ("Heal level: %d", _level));
		Utils::PlayFXSound(FX_Sound_HEAL);

		health += Skill_Att_Heal[_level - 1].health_amount;
		if(health > healthMax)
			health = healthMax;

		if(isoHeal_idx == -1)
		{
			for(int i = 0; i < MAX_ISO_SELKNAM; i++)
			{
				if(gameRef->isoHeal[i].index == -1)
				{
					gameRef->isoHeal[i].index = 0;
					gameRef->isoHeal[i]._x = getPosX();
					gameRef->isoHeal[i]._y = getPosY();
					gameRef->isoHeal[i]._z = getPosZ();
					gameRef->isoHeal[i].timer.Reset();
					gameRef->isoHeal[i].timer.Start();
					isoHeal_idx = i;
					break;
				}
			}
		}
	}
	else if(skill_type == Skill_Type_Might)
	{
		if(!has_skill_effect[(int)Skill_Type_Might])
		{
			Utils::PlayFXSound(FX_Sound_HEAL);

			if(isoMight_idx == -1)
			{
				for(int i = 0; i < MAX_ISO_SELKNAM; i++)
				{
					if(gameRef->isoMight[i].index == -1)
					{
						gameRef->isoMight[i].index = 0;
						gameRef->isoMight[i]._x = getPosX();
						gameRef->isoMight[i]._y = getPosY();
						gameRef->isoMight[i]._z = getPosZ();
						gameRef->isoMight[i].timer.Reset();
						gameRef->isoMight[i].timer.Start();
						isoMight_idx = i;
						break;
					}
				}
			}
		}


		has_skill_effect[(int)Skill_Type_Might] = true;
		damage_modifier = Skill_Att_Might[_level - 1].buff_might_amount;
	}
	else if(skill_type == Skill_Type_Haste)
	{
		if(!has_skill_effect[(int)Skill_Type_Haste])
		{
			Utils::PlayFXSound(FX_Sound_HEAL);

			if(isoHaste_idx == -1)
			{
				for(int i = 0; i < MAX_ISO_SELKNAM; i++)
				{
					if(gameRef->isoHaste[i].index == -1)
					{
						gameRef->isoHaste[i].index = 0;
						gameRef->isoHaste[i]._x = getPosX();
						gameRef->isoHaste[i]._y = getPosY();
						gameRef->isoHaste[i]._z = getPosZ();
						gameRef->isoHaste[i].timer.Reset();
						gameRef->isoHaste[i].timer.Start();
						isoHaste_idx = i;
						break;
					}
				}
			}
		}

		has_skill_effect[(int)Skill_Type_Haste] = true;
		attackspeed_modifier = Skill_Att_Haste[_level - 1].buff_haste_amount;
	}
}

void IsoPawn::SetAttackSpeed()
{

}

bool IsoPawn::ChangeTargetCondition(IsoPawn* target)
{
	return false;
}

void IsoPawn::UpdateBehaviour()
{
	//nothing...
}

void IsoPawn::AttackPawn(IsoPawn* enemy)
{
	if(time_per_attack.GetIsEnd() || time_per_attack.GetIsStop())
	{
		if(bCanAttack)
		{
			time_per_attack.Reset();
			time_per_attack.Start();

			enemy->TakeDamage(damage + damage_modifier, this, GetPawnAttributes(), skill);
			anim = ANIM_Attack;
			bAttack = true;
			bCanAttack = false;
		}
	}
}

void IsoPawn::NoPawnInteraction()
{
	bAttack = false;
	bCanAttack = true;
	
	//anim = ANIM_Idle;

	if(!attack_cooldown_timer.GetIsStop())
	{
		bCanAttack = true;
		attack_cooldown_timer.Stop();
	}
}

bool IsoPawn::isBuff(Skill_Type skill)
{
	return skill == Skill_Type_Haste ||
		   skill == Skill_Type_Might;
}

bool IsoPawn::CheckIfCanStop()
{
	//return bCanStop;
	/*
	float tmpX = getPosX();
	float tmpY = getPosY();
	float tileX = GetTile()->getPosX();
	float tileY = GetTile()->getPosY();

	return tmpX == tileX && tmpY == tileY;
	*/
		
	//int tmpX = (int)(Utils::Round((int)(getIsoPosition().x / 10)) * 10);
	//int tmpY = (int)(Utils::Round((int)(getIsoPosition().y / 10)) * 10);
	//IwTrace(GDB, ("%d %d => %d %d", (int)getIsoPosition().x, (int)getIsoPosition().y, tmpX, tmpY));
	int tmpX = (int)(getIsoPosition().x);
	int tmpY = (int)(getIsoPosition().y);
	
	bool bTemp = false;
	int diffX = abs(tmpX % 100 - 50);
	int diffY = abs(tmpY % 100 - 50);
	if(diffX <= 0 && diffY <= 0) {
		bRampWalked = false;
		return true;
	}
	int dCorrection = (bRampWalked) ? 3 : 0;
	dCorrection = 6;
	if(diffX <= dCorrection && diffY <= dCorrection) bTemp = true;
	//IwTrace(DEBUGSELKNAM, ("%d %d %d", tmpX, tmpY, (bTemp) ? 1 : 0));
	return bTemp;
	
	/*
	if(tmpX % 100 == 50 && tmpY % 100 == 50)
		return true;
	
	return false;
	*/
}

void IsoPawn::MoveToIsoTerrain(IsoTerrain* _tileFinal, bool bPermanent)
{
	//....
}

void IsoPawn::DoSpecificActionOnDebuff(Skill_Type skill)
{
	/*
	if(skill == Skill_Type_Stun)
	{
		animations.
	}
	*/
}

bool IsoPawn::SpecificAnimationOnWalk(IsoDirection dir)
{
	return false;
}

bool IsoPawn::CheckMovementConstraints()
{
	return !has_skill_effect[(int)Skill_Type_Stun] && 
		   !has_skill_effect[(int)Skill_Type_Root] &&
		   !has_skill_effect[(int)Skill_Type_Confusion];
}

int IsoPawn::GetAttackTimeAnimation() 
{
	return 0;
};

PawnAttributes IsoPawn::GetPawnAttributes()
{
	PawnAttributes att;
	att.level = level;
	att.health = health;
	att.health = healthMax;
	att.damage = damage;
	att.speed = speed;
	att.attackspeed = attackspeed;
	att.range = range;
	att.cost = cost;
	att.move_cost = move_cost;
	att.sold_value = sold_value;
	att.attack_cooldown = attack_cooldown;
	att.casting_speed = casting_speed;
	att.cast_cooldown = cast_cooldown;
	att.skill = skill;

	return att;
}

void IsoPawn::GetBuffList(Skill_Type list[], int* size)
{
	int cnt = 0;
	for(int i = 1; i <= MAX_SKILLS; i++)
	{
		if(has_skill_effect[i] && isBuff((Skill_Type)(i)))
		{
			list[cnt++] = (Skill_Type)(i);
		}
	}

	*size = cnt;
}


void IsoPawn::GetDebuffList(Skill_Type list[], int* size)
{
	int cnt = 0;
	for(int i = 1; i <= MAX_SKILLS; i++)
	{
		if(has_skill_effect[i] && !isBuff((Skill_Type)(i)))
		{
			list[cnt++] = (Skill_Type)(i);
		}
	}

	*size = cnt;
}


IsoTerrain* IsoPawn::GetTile()
{
	int xMe = Utils::Round(getIsoPosition().x / 100);
	int yMe = Utils::Round(getIsoPosition().y / 100);
	return gameRef->isoTerrainAstarGrid[xMe][yMe];
}

void IsoPawn::DoSpecificActionOnEndDebuff(Skill_Type skill)
{

}

void IsoPawn::LockTerrain(IsoTerrain* tile)
{
	if(tile != NULL)
	{
		bIsTerrainInitiallyLocked = tile->getCanPutUnit();
		tile->setCanPutUnit(false);
		tile->num_locks++;
	}
}

void IsoPawn::UnLockTerrain(IsoTerrain* tile)
{
	if(tile != NULL && !tile->getIsRamp())
	{
		tile->num_locks--;
		if(tile->num_locks <= 0)
		{
			if(bIsTerrainInitiallyLocked)
			{
				tile->setCanPutUnit(true);
			}
		}
	}

	if(tile != NULL && tile->getIsRamp())
	{
		tile->num_locks--;
	}
}

void IsoPawn::CheckIsCanDoDamageByAnimation()
{

}

IsoTerrain* IsoPawn::FindClosestFreeTileTo(IsoTerrain* tile, int range_tile)
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
			//Cannot put unit
			if(gameRef->isoTerrainAstarGrid[i][j]->getCanPutUnit() == false) continue;
			if(gameRef->isoTerrainAstarGrid[i][j]->num_enemies_on_this_tile > 0) continue;
			//There is a selknam on it
			if(gameRef->isoTerrainAstarGrid[i][j]->CurrentIsoSelknam != NULL) continue;
			if(gameRef->isoTerrainAstarGrid[i][j]->aStar_aditionalCost != 0) continue;
			int distance = DistanceBetweenTiles(gameRef->isoTerrainAstarGrid[i][j], tile);
			if(min_distance > distance)
			{
				min_distance = distance;
				closest_tile = gameRef->isoTerrainAstarGrid[i][j];
				//IwTrace(GDB, ("Candidato: %d %d", closest_tile->getXNumTile(), closest_tile->getYNumTile()));
			} 
			else if(min_distance == distance)
			{
				int closest_dist = DistanceBetweenTiles(CurrentTile, closest_tile);
				if(closest_dist > DistanceBetweenTiles(CurrentTile, gameRef->isoTerrainAstarGrid[i][j]))
				{
					closest_tile = gameRef->isoTerrainAstarGrid[i][j];
					//IwTrace(GDB, ("Candidato: %d %d", closest_tile->getXNumTile(), closest_tile->getYNumTile()));
				}
			}
		}
	}
	return closest_tile;
}

int IsoPawn::DistanceBetweenTiles(IsoTerrain* tile1, IsoTerrain* tile2)
{
	if(tile1 == NULL || tile2 == NULL) return 2 << 12;
	return abs(tile1->getXNumTile() - tile2->getXNumTile()) + abs(tile1->getYNumTile() - tile2->getYNumTile());
}

void IsoPawn::DoSpecificActionOnTakeDamage()
{
	//virtual
}