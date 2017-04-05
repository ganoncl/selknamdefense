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

#ifndef _ISO_PAWN_H_
#define _ISO_PAWN_H_

#include "isoobject.h"
#include "isoterrain.h"
#include "animation.h"
#include "astarhelper.h"
#include "timer.h"
#include "IwSound.h"
#include "texturehelper.h"
#include "common.h"

typedef struct
{
	float uvWidth, uvHeight;
	int rowSize;
} RenderInfo;

class IsoPawn : public IsoObject
{
protected:
	bool bInitialized;
	//COMMON ATRIBUTES
	//Selknam Behavior (none on enemies)
	Pawn_SelknamBehavior sBehavior;
	//Enemy Behavior (none on selknam)
	Pawn_EnemyBehavior eBehavior;
	//current level
	int level;
	//current health
	int health;
	//max health (useful to retrieve pct calcs)
	int healthMax;
	//damage to inflict
	int damage;
	//speed multiplier (useful to add as multiplier on velocity), normal_value:10
	int speed;
	//delay multiplier to apply on attack animation, en decimas de segundos.
	int attackspeed;
	//delay cooldown between two consecutive attacks, en decimas de segundos.
	int attack_cooldown;
	//range of unit.
	int range;
	//range alt of unit. (used on melees)
	int rangealt;
	//cost of unit.
	int cost;
	//cost to move a unit
	int move_cost;
	//when a unit is sold, mp_coins increase in sold_value
	int sold_value;	
	//coins gained per kill enemy
	int coins_perkill;
	//delay multiplier to apply on magic skills (heal), en decimas de segundos.
	int casting_speed;
	//delay cooldown between two consecutive magic skills (heal), en decimas de segundos.
	int cast_cooldown;
	//skill type for unit.
	Skill_Type skill;

	//Is dead.
	bool bDead;
	bool bCanStop;
	bool bIsInvincible;
	bool bAttack;
	bool bCanAttack;
	bool bMagic;
	bool bCanSpellMagic;

	Timer attack_cooldown_timer;
	Timer cast_cooldown_timer;
	Timer timeToBanish;
	Timer time_per_attack;

	int GetAttackCooldown() { return attack_cooldown + (10 - attackspeed) - attackspeed_modifier; }
	int GetCastCooldown() { return cast_cooldown + (10 - attackspeed); }
	void SetAttackCooldown(int cooldown) { 
		attack_cooldown_timer.SetTimeLimit(cooldown * 100);
		time_per_attack.SetTimeLimit(cooldown * 100);
	}
	void SetCastCooldown(int cooldown) { cast_cooldown_timer.SetTimeLimit(cooldown * 100); }

	int time_banish_limit;
	void SetTimeToBanish(int time) {time_banish_limit = time; };

	Animation animations;

	IsoTerrain* FinalTile;
	IsoTerrain* CurrentTile;
	IsoTerrain* NextTile;
	IsoTerrain* LastCurrentTile;
	IsoTerrain* LastNextTile;
	CIwFVec2 Veloc;
	IsoDirection direction;
	IsoAnim anim;

	
	virtual void HandleDie();
	virtual int GetAttackTimeAnimation();
	void SetAttackAnim(string anim_name);
	void UpdateCooldownAttackTime();
	void UpdateCooldownMagicTime();
	void VerifyDeathStatus();
	void VerifyDebuffStatus();
	void VerifyInMotionStatus();
	virtual void SetAttackSpeed();
	IsoDirection GetDirection(IsoPawn* pawn);
	IsoDirection GetDirectionToIsoTerrain(IsoTerrain* tile);
	virtual void DoSpecificActionOnTakeDamage();
	
	//Poison
	Timer poison_interval_time;
	int poison_num_interval;
	int poison_damage;
	//Poison on death
	Timer poison_ondeath_interval_time;
	int poison_ondeath_num_interval;
	int poison_ondeath_damage;
	//Slow
	Timer slow_time;
	//Stun
	bool bRoot;
	bool bStun;
	Timer stun_time;

	Timer confusion_time;
	Timer root_time;

	int speed_modifier;
	int attackspeed_modifier;
	int damage_modifier;

	//True: Move to a random tile
	bool bDoEffectConfusion; 
	void MoveToRandomTerrain();

	virtual bool CheckIfCanStop();

	virtual void CheckIsCanDoDamageByAnimation();
	bool DoAttackOnce;

	//When a unit walks trough a ramp, (x, y) position is bugged.
	bool bRampWalked;
	IsoTerrain* markedTile;

	//Utility methods
	bool bUseEnemyConstraint; //Use enemies_on_tile to choose a tile in FindClosestFreeTileTo method.
	int DistanceBetweenTiles(IsoTerrain* tile1, IsoTerrain* tile2);
	IsoTerrain* FindClosestFreeTileTo(IsoTerrain* tile, int range_tile = 1);

	bool bIsTerrainInitiallyLocked;

	//Animation effects
	int isoHeal_idx;
	int isoMight_idx;
	int isoHaste_idx;
	int isoExplotion_idx;
	int isoPoisonOnDeath_idx;
	int isoConfusion_idx;
	int isoStun_idx;
	int isoCoin_idx;

	Timer timerStunEffect;
	Timer timerConfusionEffect;

	Timer confusion_time_delay;
	int confusion_time_remaining;
	bool bResetConfusionTimer;

	IsoPawn* PawnTakeDamageToMe; //The Last Pawn that called to my TakeDamage method.

public:
	IsoPawn();
	~IsoPawn();
	virtual void Update();
	virtual void CleanIso();
	virtual void InitIsoPawn(IsoTerrain* _tile);

	void setInitialized(bool _bInitialized) { bInitialized = _bInitialized; }
	bool getInitialized() const { return bInitialized; }

	//need to be setted once on entire game
	void CleanAtributes();	
	void SetPawnAtributes(PawnAttributes _newPawnAtributes);

	float RetrieveZPos(int _layer, float _yPos, float _yFakePos = 0.f);

	float RetrieveZPos();
	void RetrieveZPosAndLayer(IsoTerrain* Tile1, IsoTerrain* Tile2, float* newZPos, int* newLayer);
	void PostProcessZPos(float* newZPos);
	
	//movement
	bool bMove;
	virtual void UpdateMovement();
	//path to current objective
	IsoTerrain *currentIsoPath[ASTAR_LISTSMAX];
	bool RetrieveTerrainIsoPathByTargetNum(IsoTerrain* _tile, int _targetNum);
	void setVeloc(float _velX, float _velY)	{ Veloc.x = _velX; Veloc.y = _velY;	}
	CIwFVec2 getVeloc() const	{ return Veloc; }
	IsoDirection GetDirectionByAdyTiles(IsoTerrain* _tileSource, IsoTerrain* _tileTarget);
	void setVelocByIsoDirection(IsoDirection _myDirection);
	float getSpeed();
	void UpdateCurrentTileByDir(IsoDirection _myDirection);
	float UpdateCurrentRampTileByDir(IsoDirection _myDirection, float *_yFakePos);
	//float UpdateCurrentTileFirstHalfRampByDir(IsoDirection _myDirection, int _offSetNum);
	//float UpdateCurrentTileSecondHalfRampByDir(IsoDirection _myDirection, int _offSetNum);
	void ApplyPositionCorrectionByTile(IsoTerrain* _tile, float _yOffset = 0.0f);

	//render
	void SetUV(int id);
	//animation
	void UpdateAnimation();

	//A-Star
	AStarHelper *aStarHelperRef;
	CGame *gameRef;

	int GetLevel() { return level; };
	int GetMoveCost() { return move_cost; };
	int GetMaxHealth() { return healthMax; };
	int GetCurrentHealth() { return health; };
	bool GetIsDead() { return bDead; };
	bool GetIsAttacking() { return bAttack; };
	bool GetIsCastingMagic() { return bMagic; };
	int GetRange() { return range; };

	float uvWidth;
	float uvHeight;
	int horizontalImages;

	IsoTerrain* GetCurrentTile() { return CurrentTile; };

	IsoPawn* pawnTargeted;
	void UpdateAttackPattern();
	bool IsPawnInRange(IsoPawn* pawn, int* distance = NULL, int altRange = -1);
	void TakeDamage(int damage, IsoPawn* inflictor, PawnAttributes inflictor_att, Skill_Type skill_type);
	void TakeBuff(Skill_Type skill_type, int _level = 1);
	void TakeDamage(int damage);
	virtual void InteractWithPawn(IsoPawn* target);
	virtual IsoPawn* SearchPawnNearby();
	virtual bool ChangeTargetCondition(IsoPawn* target);
	virtual void VerifyBuffStatus();
	virtual void UpdateBehaviour();
	virtual void MoveToIsoTerrain(IsoTerrain* _tileFinal, bool bPermanent = true);
	virtual void DoSpecificActionOnDebuff(Skill_Type skill);
	virtual void DoSpecificActionOnEndDebuff(Skill_Type skill);
	PawnAttributes GetPawnAttributes();

	IsoPawn* pawnTargettingMe;
	bool bRecalcPath;
	void AttackPawn(IsoPawn* enemy);
	virtual void NoPawnInteraction();
	virtual bool SpecificAnimationOnWalk(IsoDirection dir);

	void GetBuffList(Skill_Type list[], int* size);
	void GetDebuffList(Skill_Type list[], int* size);

	//Tile where Pawn was initially spawned
	IsoTerrain* spawn_tile;
	
	bool bIsAttackableOnMotion; //IsoSelknam can attack this unit, no matter if this unit is walking or not.

	Skill_Type last_debuff;
	Skill_Type GetLastDebuffReceived() { return last_debuff; };
	bool has_skill_effect[MAX_SKILLS + 1];
	bool isBuff(Skill_Type skill);
	bool CheckMovementConstraints();

	IsoTerrain* GetTile();
	void LockTerrain(IsoTerrain* tile);
	void UnLockTerrain(IsoTerrain* tile);
	IsoDirection GetOrientation() { return direction;};
	bool GetIsStun() {return bStun; };
	bool GetIsRoot() {return bRoot; };

	int  GetIsoHealIndex()          { return isoHeal_idx;  }
	void SetIsoHealIndex(int idx)   { isoHeal_idx = idx;   }
	int  GetIsoMightIndex()         { return isoMight_idx; }
	void SetIsoMightIndex(int idx)  { isoMight_idx = idx;  }
	int  GetIsoHasteIndex()         { return isoHaste_idx; }
	void SetIsoHasteIndex(int idx)  { isoHaste_idx = idx;  }
	int  GetIsoExplotionIndex()     { return isoExplotion_idx; }
	void SetIsoExplotionIndex(int idx) { isoExplotion_idx = idx; } 
	int GetIsoPoisonOnDeathIndex() { return isoPoisonOnDeath_idx; }
	void SetIsoPoisonOnDeathIndex(int idx) { isoPoisonOnDeath_idx = idx; }
	int GetIsoConfusionIndex() { return isoConfusion_idx; }
	int GetIsoStunIndex() { return isoStun_idx; }
	int GetCoinIndex() { return isoCoin_idx; }
	void SetCoinIndex(int idx) { isoCoin_idx = idx; }

	/*
	uint32 texture_color;
	void setTextureColor(uint32 color) { this->texture_color = color; }
	uint32 getTextureColor() { return texture_color; }
	*/
	int texture_r;
	int texture_g;
	int texture_b;
	int texture_alpha;
	void setTextureColor(int r, int g, int b, int a = 255) {
		texture_r = r;
		texture_g = g;
		texture_b = b;
		texture_alpha = a;
	}
	int getTextureR() { return texture_r; }
	int getTextureG() { return texture_g; }
	int getTextureB() { return texture_b; }
	int getTextureAlpha() { return texture_alpha; }

	int delta_poison;
	int curPoison;

	int delta_slow;
	int curSlow;

	//if id < atlasMaxID then use atlasData[0], use atlasData[1] otherwise.
	int atlasMaxID;
	AtlasName atlasData[2];
	RenderInfo atlasInfo[2];
	int lastID, currID;

	bool ChangeTexture()
	{
		if(lastID != currID)
		{
			lastID = currID;
			return true;
		}
		return false;
	}

	void AddAtlasData(AtlasName data, RenderInfo info, int index)
	{
		atlasData[index] = data;
		atlasInfo[index] = info;
	}

	void SetMaxAtlasID(int index)
	{
		atlasMaxID = index;
	}

	CIwTexture* GetTexture()
	{
		return TextureHelper::GetCIwTexture(atlasData[currID - 1]);
	}
};

#endif // _ISO_PAWN_H_