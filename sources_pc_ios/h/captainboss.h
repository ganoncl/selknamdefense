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

#ifndef _CAPTAINBOSS_H
#define _CAPTAINBOSS_H

#include "IwGx.h"
#include "utils.h"
#include "common.h"
#include "localization.h"
#include "model3d.h"
#include "timer.h"
#include "isoenemy.h"
#include "background.h"

#define CAPTAIN_NUM_ANIM_ATTACK 7
#define CAPTAIN_NUM_ANIM_IDLE 5
#define CAPTAIN_NUM_ANIM_PAIN 7
#define CAPTAIN_NUM_ANIM_DEATH 17
#define CAPTAIN_NUM_WAVES 10

#define CANNON_DAMAGE 10
#define CANNON_RECHARGE_TIME 5000

#define FORWARD_TIME 10000

#define TIME_AFTER_FORWARD_MOVEMENT 5000

class CGame;

typedef enum
{
	CaptainCycle_None,
	CaptainCycle_1,
} CaptainCycle;

typedef enum
{
	ShipPosition_None,
	ShipPosition_Backward,
	ShipPosition_Forward
} ShipPosition;

typedef enum
{
	CaptainState_Attack,
	CaptainState_Idle,
	CaptainState_Pain,
	CaptainState_Death
} CaptainState;

typedef enum
{
	CaptainAnim_None,
	CaptainAnim_Idle,
	CaptainAnim_Fire
} CaptainAnim;

class CaptainBoss
{
public:
	CaptainBoss();
	~CaptainBoss();
	void Render();
	void Update();
	void SetActive(bool bIsActive);
	void Move(CIwFVec3 desp, int time);
	void SetPosition(CIwFVec3 position);
	void PlayShipAnim(int index, float speed);
	void MoveForward();
	void MoveBackward();
	AtlasName GetAtlasByCaptainState(CaptainState state);
	RenderData GetRenderDataByCaptainState(CaptainState state); 
	CaptainState GetState();
	CIwFVec3 GetPosition();
	bool GetIsActive();
	void SetEnemyObject(IsoEnemy* enemy);
	void PlayCaptainAnim(CaptainState state);
	void SpawnEnemyUnit(int node, int dest);
	void SetGameRef(CGame* gameRef);
	void Init();
	void EndObject();
	void SetCycle(int n);
	void MarkPlaceToShoot(IsoTerrain* terrain, int delay, Skill_Type skillType);
	void Shoot(int delay = 2000, Skill_Type skillType = Skill_Type_None, bool bFriendlyDamage = false, int percent = 0);
	void Shoot(IsoTerrain* terrain, Skill_Type skillType = Skill_Type_None);
	void CancelSpecialAttack();
	bool IsMoving();
	void DestroyCannons();
	void RenderShipFragment(int index);
	bool IsVulnerable();
	void PerformSpecialAttack();
	void DrawSpecialAttackCountdown();
	void PlayEndingScene();
	void UpdateEndingScene();
	void SetVisible(bool value);
	bool GetIsVisible();
	void RenderSpecialForeground();
	void ApplyDebuffToBoss(Skill_Type skillType);
	void EntranceScene();
	void SetWaveSettings();
	bool CanBeHit();
	void SetEntrancePosition();
	void EndEntranceScene();
	void EndGameAction();

	bool bEndingScenePlaying;

private:
	void DestroyCannonsSceneUpdate();
	void PlaceInitialSelknams();
	void SpawnSelknam(IsoSelknamType type, IsoTerrain* terrain);

	bool bStartInitialDialog;
	bool bCaptainIgnorePauseStatus;

	float entranceOffsetX;
	float entranceOffsetY;

	int currCannon;
	float cannonOffset[2][2];

	Timer yellowTextTimer;

	bool bAffectedByRoot;

	FadeScreen* whiteFade;
	bool bFirstWave;
	bool bEndGameAction;
	bool bVisible;

	Timer shipExplotionTime;
	Timer shipExplotionDelay;
	Timer shipExplotionBlinkDelay;
	Timer shipExplotionBlinkTime;
	Timer timerSpeakAtBackward;
	Timer shipCannonExplotionTime;
	Timer shipCannonExplotionDelay;
	
	bool bCannonExplotionScene;
	bool bIsDead;
	int backwardTime[4];
	int waveTime[4];
	int nextWaveTime[4];
	int timeBeforeSPAttack[4];
	int delayBetweenSPShots[4];

	void UpdateAnimation();
	IsoEnemy* enemyHolder;
	CGame* pGame;
	bool bIsActive;
	Model3D* shipModel;
	CaptainAnim curCaptainAnim;
	CaptainState curCaptainState;
	ShipPosition curShipPosition;
	ShipPosition lastShipPosition;
	int lastHealth;

	//Gameplay
	Timer cycleTime;
	Timer waveDelay;
	Timer timeBetweenWaves;
	Timer rechargingCannon;
	Timer restingTime;
	int numWave;
	int numLoops;
	IsoEnemyType waveEnemyInfo[CAPTAIN_NUM_WAVES][10];
	int numEnemiesLength[CAPTAIN_NUM_WAVES];
	bool bStartForwardTimer;
	bool bStartBackwardTimer;
	bool bPerformSpecialAttack;

	int curCaptainCycle;

	//Animations
	Timer animationDelay;
	Timer painAnimationDelay;
	int curAnimFrame;
	bool bIncreaseAnimFrame;
	RenderData attackAnimRenderData[CAPTAIN_NUM_ANIM_ATTACK];
	RenderData idleAnimRenderData[CAPTAIN_NUM_ANIM_IDLE];
	RenderData painAnimRenderData[CAPTAIN_NUM_ANIM_PAIN];
	RenderData deathAnimRenderData[CAPTAIN_NUM_ANIM_DEATH];
	AtlasName  attackAnimAtlasName[CAPTAIN_NUM_ANIM_ATTACK];
	AtlasName  idleAnimAtlasName[CAPTAIN_NUM_ANIM_IDLE];
	AtlasName  painAnimAtlasName[CAPTAIN_NUM_ANIM_PAIN];
	AtlasName  deathAnimAtlasName[CAPTAIN_NUM_ANIM_DEATH];

	int healthLimit[5];
	int currentHealtLimit;

	int enemyHitPercent;
	int allyHitPercent;
};

#endif