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

#ifndef _ISO_SELKNAM_H_
#define _ISO_SELKNAM_H_

#include "IwSound.h"
#include "common.h"
#include "isopawn.h"
#include "isoenemy.h"

class IsoSelknam : public IsoPawn
{
protected:
	IsoSelknamType SelknamType;
	CIwResGroup* pGroup;
	IsoPawn* enemy_to_attack;

	virtual IsoPawn* SearchPawnNearby();
	virtual void InteractWithPawn(IsoPawn* target);
	virtual bool ChangeTargetCondition(IsoPawn* target);
	virtual void VerifyBuffStatus();
	virtual void UpdateBehaviour();
	virtual void NoPawnInteraction();
	virtual void CheckIsCanDoDamageByAnimation();
	virtual int GetAttackTimeAnimation();
	virtual void HandleDie();
	void SetAttackSpeed();
;	IsoPawn* SearchAlliesNearby();
	IsoPawn* SearchSelknamToBuff();
	IsoPawn* SearchEnemyByDebuff(Skill_Type skill);
	void AttackPawn(IsoPawn* enemy);
	void CastSpellOnPawn(IsoPawn* enemy);
	void DoShoot(IsoProjectileType type, IsoPawn* enemy);
	bool CheckIfCanAttack(IsoPawn* enemy);

public:
	IsoSelknam();
	~IsoSelknam();
	virtual void Update();
	virtual void CleanIso();
	virtual void InitIsoPawn(IsoTerrain* _tile);

	void InitSelknam(IsoSelknamType _selknamType);
	IsoSelknamType GetSelknamType() const	{ return SelknamType;}

	void SetSelknamAnim();
	virtual void MoveToIsoTerrain(IsoTerrain* _tileFinal, bool bPermanent = true);
	void MoveToIsoTerrainRecordPath(IsoTerrain* _tileFinal, bool bPermament = true);

	int GetSellPrice() { return sold_value; };
	bool CheckSpecialAttackCases(IsoPawn* enemy, bool bCheckRange = true);

	IsoTerrain* UnitManagerMoveTile;
	void CleanSpawnTile();
private:
	IsoTerrain* CurObjTile;
	IsoTerrain* LastTileMarked;
	bool bPerformingMoveAction;
	IsoTerrain* InitialTile;
};

#endif // _ISO_SELKNAM_H_