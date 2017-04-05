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

#include "survivalmodeconfig.h"
#include "parserhelper.h"
#include "savedatahelper.h"

SurvivalModeConfig::SurvivalModeConfig()
{
	bIsSurvivalAvailable = false;

	for(int i = 0; i < MAX_CYCLES; i++)
	{
		numEnemyTypes[i] = 0;
	}
	maxCycle = 0;
	repetitiveCycle = 8;
	bIsActive = false;
	lastRecord = 0;

	varInstericial = 0;
	bPauseInsterticial = false;
	nodesRotator = 0;
	multStatsBy = 1.0f;
	varCycleTimer = 0;

	//Initial
	//Cycle = 1
	SetEnemyLevel(1, 1);
	SetNumRespaws(1, 7);
	SetRespawnTime(1, 7000);
	AddEnemyType(1, ENEMY_Rabbit);
	AddEnemyType(1, ENEMY_Rabbit);
	AddEnemyType(1, ENEMY_Rabbit);
	AddEnemyType(1, ENEMY_Rabbit);

	//Cycle = 2
	SetEnemyLevel(2, 1);
	SetNumRespaws(2, 7);
	SetRespawnTime(2, 7000);
	AddEnemyType(2, ENEMY_Soldier);
	AddEnemyType(2, ENEMY_Soldier);
	AddEnemyType(2, ENEMY_Soldier);
	AddEnemyType(2, ENEMY_Soldier);

	//Cycle = 3
	SetEnemyLevel(3, 1);
	SetNumRespaws(3, 5);
	SetRespawnTime(3, 7000);
	AddEnemyType(3, ENEMY_Soldier);
	AddEnemyType(3, ENEMY_Varon);
	AddEnemyType(3, ENEMY_Soldier);
	AddEnemyType(3, ENEMY_Varon);

	//Cycle = 4
	SetEnemyLevel(4, 1);
	SetNumRespaws(4, 5);
	SetRespawnTime(4, 7000);
	AddEnemyType(4, ENEMY_Soldier);
	AddEnemyType(4, ENEMY_Varon);
	AddEnemyType(4, ENEMY_Pirate);
	AddEnemyType(4, ENEMY_Rabbit);

	//Cycle = 5
	SetEnemyLevel(5, 1);
	SetNumRespaws(5, 2);
	SetRespawnTime(5, 5000);
	AddEnemyType(5, ENEMY_Tank);
	AddEnemyType(5, ENEMY_Tank);
	AddEnemyType(5, ENEMY_Tank);
	AddEnemyType(5, ENEMY_Tank);

	//Cycle = 6
	SetEnemyLevel(6, 2);
	SetNumRespaws(6, 4);
	SetRespawnTime(6, 9000);
	AddEnemyType(6, ENEMY_Soldier);
	AddEnemyType(6, ENEMY_Varon);
	AddEnemyType(6, ENEMY_Scientist);
	AddEnemyType(6, ENEMY_Rabbit);

	//Cycle = 7
	SetEnemyLevel(7, 2);
	SetNumRespaws(7, 3);
	SetRespawnTime(7, 4000);
	AddEnemyType(7, ENEMY_Soldier);
	AddEnemyType(7, ENEMY_Soldier);
	AddEnemyType(7, ENEMY_Soldier);
	AddEnemyType(7, ENEMY_Soldier);

	//Repetitive.
	//Cycle = 8
	SetEnemyLevel(8, 3);
	SetNumRespaws(8, 2);
	SetRespawnTime(8, 4000);
	AddEnemyType(8, ENEMY_Pirate);
	AddEnemyType(8, ENEMY_Varon);
	AddEnemyType(8, ENEMY_Pirate);
	AddEnemyType(8, ENEMY_Varon);

	//Cycle = 9
	SetEnemyLevel(9, 2);
	SetNumRespaws(9, 3);
	SetRespawnTime(9, 4000);
	AddEnemyType(9, ENEMY_Scientist);
	AddEnemyType(9, ENEMY_Varon);
	AddEnemyType(9, ENEMY_Pirate);
	AddEnemyType(9, ENEMY_Rabbit);

	//Cycle = 10
	SetEnemyLevel(10, 3);
	SetNumRespaws(10, 5);
	SetRespawnTime(10, 4000);
	AddEnemyType(10, ENEMY_Soldier);
	AddEnemyType(10, ENEMY_Varon);
	AddEnemyType(10, ENEMY_Pirate);
	AddEnemyType(10, ENEMY_Scientist);

	//Cycle = 11
	SetEnemyLevel(11, 3);
	SetNumRespaws(11, 2);
	SetRespawnTime(11, 4000);
	AddEnemyType(11, ENEMY_Convict);
	AddEnemyType(11, ENEMY_Convict);
	AddEnemyType(11, ENEMY_Convict);
	AddEnemyType(11, ENEMY_Convict);

	//Cycle = 12
	SetEnemyLevel(12, 3);
	SetNumRespaws(12, 6);
	SetRespawnTime(12, 4000);
	AddEnemyType(12, ENEMY_Soldier);
	AddEnemyType(12, ENEMY_Varon);
	AddEnemyType(12, ENEMY_Pirate);
	AddEnemyType(12, ENEMY_Scientist);

	//Cycle = 13
	SetEnemyLevel(13, 1);
	SetNumRespaws(13, 2);
	SetRespawnTime(13, 4000);
	AddEnemyType(13, ENEMY_Tank);
	AddEnemyType(13, ENEMY_Tank);
	AddEnemyType(13, ENEMY_Tank);
	AddEnemyType(13, ENEMY_Tank);

	//Cycle = 14
	SetEnemyLevel(14, 3);
	SetNumRespaws(14, 1);
	SetRespawnTime(14, 4000);
	AddEnemyType(14, ENEMY_Rabbit);
	AddEnemyType(14, ENEMY_Rabbit);
	AddEnemyType(14, ENEMY_Rabbit);
	AddEnemyType(14, ENEMY_Rabbit);

	//Cycle = 14
	SetEnemyLevel(15, 3);
	SetNumRespaws(15, 2);
	SetRespawnTime(15, 4000);
	AddEnemyType(15, ENEMY_Convict);
	AddEnemyType(15, ENEMY_Convict);
	AddEnemyType(15, ENEMY_Convict);
	AddEnemyType(15, ENEMY_Convict);
}

SurvivalModeConfig::~SurvivalModeConfig()
{

}

void SurvivalModeConfig::SetBestRecord(int milisec)
{
	bestRecord = milisec;
}

void SurvivalModeConfig::InitObject()
{
	if(pGame != NULL)
	{
		pGame->setMPCoins(12 * 6);
	}
}

void SurvivalModeConfig::Start()
{
	bIsActive = true;

	varInstericial = 0;
	bPauseInsterticial = false;
	nodesRotator = 0;
	multStatsBy = 1.0f;
	varCycleTimer = 0;
	currCycle = 1;

	srand(time(NULL));
	numLeftRespawns = numRespawnsCycle[currCycle];
	survivalEnemySpot.SetTimeLimit(cycleTime[currCycle]);
	survivalEnemySpot.Stop();

	timePlayed.Reset();
	timePlayed.Start();

	bFirstSpawn = true;
}

void SurvivalModeConfig::End()
{
	bIsActive = false;
	lastRecord = timePlayed.GetCurrTime();

	//Enemy Spawn
	IsoEnemy* CurrentEnemy = NULL;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		CurrentEnemy = &pGame->isoEnemyArr[i];
		if(CurrentEnemy == NULL) continue;
		CurrentEnemy->CleanIso();
	}

	//IwTrace(SURVIVALASDAD, ("Ending Survival Mode"));
}

bool SurvivalModeConfig::NewRecord()
{
	return lastRecord > bestRecord;
}

void SurvivalModeConfig::UpdateRecord()
{
	bestRecord = lastRecord;
	if(saveDataHelper)
	{
		saveDataHelper->SaveGameDataSurvival();
	}
}

int SurvivalModeConfig::GetBestRecord()
{
	return bestRecord;
}

void SurvivalModeConfig::Update()
{
	if(!bIsActive || inGameState != GSG_Attack) 
	{
		//if(!bIsActive) IwTrace(SURVIVALBATTLE, ("bIsActive = false"));
		//if(inGameState != GSG_Attack) IwTrace(SURVIVALBATTLE, ("inGameState != GSG_Attack"));

		return;
	}

	if(bPauseGame || bPause)
	{
		if(!timePlayed.GetIsStop()) timePlayed.Stop();
		if(!survivalEnemySpot.GetIsStop()) survivalEnemySpot.Stop();
	}
	else
	{
		if(timePlayed.GetIsStop()) timePlayed.Resume();
		if(survivalEnemySpot.GetIsStop()) survivalEnemySpot.Resume();
	}

	timePlayed.Update();
	survivalEnemySpot.Update();

	if(bFirstSpawn || survivalEnemySpot.GetIsEnd())
	{
		bFirstSpawn = false;
		
		for(int i = 0; i < 4; i++)
		{
			SpawnEnemyUnit(i, 1, nodesRotator);
		}
		nodesRotator++;
		if(nodesRotator >= 4)
			nodesRotator = 0;
		
		numLeftRespawns--;
		if(numLeftRespawns <= 0)
		{
			currCycle++;

			if(currCycle > maxCycle)
			{
				currCycle = repetitiveCycle;
				multStatsBy *= 1.1f;
				varCycleTimer += 750;
			}

			int tmpCycleTime = cycleTime[currCycle];
			tmpCycleTime = tmpCycleTime - varCycleTimer;
			if(tmpCycleTime <= 1000)
			{
				tmpCycleTime = 1000;
				varInstericial++;
				//include pause insterticial in case you have on critical advance :)
				if(varInstericial==14 && !bPauseInsterticial)
				{
					bPauseInsterticial = true;
					varCycleTimer = 2000;
					currCycle = 1;
					multStatsBy *= 1.3f;
				}
			}

			numLeftRespawns = numRespawnsCycle[currCycle];
			survivalEnemySpot.SetTimeLimit(tmpCycleTime);
		}

		survivalEnemySpot.Reset();
		survivalEnemySpot.Start();
	}
}

void SurvivalModeConfig::SetCGameObject(CGame* pGame)
{
	this->pGame = pGame;
}

string SurvivalModeConfig::GetTimePlayed()
{
	return Utils::GetClockFormat(timePlayed.GetCurrTime() / 1000);
}

int SurvivalModeConfig::GetTimePlayedAsInt()
{
	return timePlayed.GetCurrTime();
}

void SurvivalModeConfig::SpawnEnemyUnit(int node, int dest, int myNodeRotator)
{
	if(pGame->initNodes[node] == NULL) return;

	//Enemy Spawn
	IsoEnemy* CurrentEnemy = NULL;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		CurrentEnemy = &pGame->isoEnemyArr[i];

		if(CurrentEnemy == NULL) continue;

		if(CurrentEnemy->getInitialized()) continue;

		//int enemyIndex = rand() % numEnemyTypes[currCycle];
		int enemyIndex = node + myNodeRotator;
		if(enemyIndex>=4)
			enemyIndex = enemyIndex - 4;
		CurrentEnemy->setEnemyType(enemyType[currCycle][enemyIndex]);
		CurrentEnemy->setEnemyLevel(enemyLevel[currCycle]);

		CurrentEnemy->setInitPathNum(node + 1);
		CurrentEnemy->setTargetNum(dest);

		CurrentEnemy->InitIsoPawn(pGame->initNodes[node]);

		CurrentEnemy->SetCurrentHealth((int)(CurrentEnemy->GetMaxHealth() * multStatsBy));
		CurrentEnemy->SetCurrentMaxHealth((int)(CurrentEnemy->GetMaxHealth() * multStatsBy));
		//CurrentEnemy->SetDamage((int)(CurrentEnemy->GetDamage() * multStatsBy));

		return;
	}
}

void SurvivalModeConfig::SetNumRespaws(int cycle, int numRespaws)
{
	maxCycle = max(maxCycle, cycle);

	numRespawnsCycle[cycle] = numRespaws;
}

void SurvivalModeConfig::SetRespawnTime(int cycle, int t)
{
	maxCycle = max(maxCycle, cycle);

	cycleTime[cycle] = t;
}

void SurvivalModeConfig::SetEnemyLevel(int cycle, int level)
{
	maxCycle = max(maxCycle, cycle);

	enemyLevel[cycle] = level;
}

void SurvivalModeConfig::AddEnemyType(int cycle, IsoEnemyType type)
{
	maxCycle = max(maxCycle, cycle);

	enemyType[cycle][numEnemyTypes[cycle]++] = type;
}