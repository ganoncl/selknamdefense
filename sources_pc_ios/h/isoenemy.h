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

#ifndef _ISO_ENEMY_H_
#define _ISO_ENEMY_H_

#include "common.h"
#include "isopawn.h"
#include "animation.h"
#include "isoterrain.h"

#define LAST_TILES_ARR_SIZE 3

class IsoEnemy : public IsoPawn
{
protected:
	IsoEnemyType enemyType;
	int enemyLevel;
	int initPathNum;
	int waveToSpawn;
	int secToSpawn;
	int targetNum;

	CIwResGroup* pGroup;

	//Suicidal
	bool b_suicidal_moving_enemy;
	bool bIgnoreEnemies;

	bool bFollow;
	int xFollowTile;
	int yFollowTile;
	IsoTerrain* follow_tile;
	IsoTerrain* target_terrain;
	IsoTerrain* myTerrainLock;

	virtual int GetAttackTimeAnimation();
	virtual void CheckIsCanDoDamageByAnimation();
	void DoShoot(IsoProjectileType type, IsoPawn* enemy);

	bool bLockDone;
	virtual void DoSpecificActionOnTakeDamage();
	bool isValidToWalk(IsoTerrain* tile);
	void MoveToIsoTerrain_PrunePath(IsoTerrain* tile, IsoTerrain* obj);
	void IsoEnemy_Offensive_Algorithm(IsoPawn* enemy);
	void IsoEnemy_RangeSingleFocus_Algorithm(IsoPawn* enemy);
	void IsoEnemy_Suicidal(IsoPawn* enemy);

	bool CheckIfCanAttack(IsoPawn* enemy);

	IsoPawn *SearchNearestEnemyOnTheWholeMap();
	bool IsPawnInRange_ByIsoPosition(IsoPawn* enemy);

	void SetAttackSpeed();

public:
	IsoEnemy();
	~IsoEnemy();
	virtual void Update();
	virtual void CleanIso();
	virtual void InitIsoPawn(IsoTerrain* _tile);

	void setEnemyType(const IsoEnemyType _enemyType) { enemyType = _enemyType; }
	IsoEnemyType getEnemyType() const { return enemyType; }
	void setEnemyLevel(const int _enemyLevel) { enemyLevel = _enemyLevel; }
	int getEnemyLevel() const { return enemyLevel; }
	void setInitPathNum(const int _initPathNum) { initPathNum = _initPathNum; }
	int getInitPathNum() const { return initPathNum; }
	void setWaveToSpawn(const int _waveToSpawn) { waveToSpawn = _waveToSpawn; }
	int getWaveToSpawn() const { return waveToSpawn; }
	void setSecToSpawn(const int _secToSpawn) { secToSpawn = _secToSpawn; }
	int getSecToSpawn() const { return secToSpawn; }
	void setTargetNum(const int _targetNum) { targetNum = _targetNum; }
	int getTargetNum() const { return targetNum; }
	

	void SetEnemyAnimSeq();
	IsoEnemyType GetEnemyType() const	{ return enemyType;}
	void GoToObjective(int numObj);

	virtual void InteractWithPawn(IsoPawn* enemy);
	virtual IsoPawn* SearchPawnNearby();
	virtual void UpdateBehaviour();
	virtual bool ChangeTargetCondition(IsoPawn* target);
	virtual void NoPawnInteraction();
	virtual void MoveToIsoTerrain(IsoTerrain* _tileFinal, bool bPermament = true);
	IsoPawn* CloserEnemyInRange();
	IsoPawn* EnemyWithMoreEnemiesAround(int range = 1);
	void AttackPawn(IsoPawn* enemy);
	virtual bool CheckIfCanStop();
	virtual void DoSpecificActionOnDebuff(Skill_Type skill);
	void virtual HandleDie();
	virtual bool SpecificAnimationOnWalk(IsoDirection dir);
	virtual void DoSpecificActionOnEndDebuff(Skill_Type skill);

	void LockTerrain(IsoTerrain* tile);
	void UnLockTerrain(IsoTerrain* tile);
	IsoTerrain* FindClosestFreeTileTo(IsoTerrain* tile, int range_tile = 1);

private:
	//When confusion effect is gone and enemy still is going to the random tile, we need to redirect his path.
	bool bGoToTargetAfterConfusion;

	//Convict
	IsoPawn* LastPawnTargeted;

	//Last tile where isoEnemy tried to go, to prevent enemies looping.
	pair<int, int> lastTiles[LAST_TILES_ARR_SIZE];
	int lastTilesCnt;
};

#endif // _ISO_ENEMY_H_