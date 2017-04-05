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

#ifndef _SURVIVALMODECONFIG_H
#define _SURVIVALMODECONFIG_H

#include "game.h"
#include "common.h"
#include "utils.h"

/*
 * This class handle game mechanics of the "Survival Mode". It manages spawn time and types/levels of the enemies.
 *
 * How it works:
 * You can set a cycle, a cycle consists of "time between enemies spawn", "num spaws", "level of the enemies" and "kind of enemies"
 * Cycle = 1: Level 1 enemies, types: SOLDIER
 * Cycle = 2: Level 1 enemies, types: SOLDIER, VARON
 * and so on.
 *
 * To know what kind of enemies you can set, you can see IsoEnemyType enum in common.h
 *
 * Author: Javier Gonzalez N.
 * Last modification: 19-01-13
 */

#define MAX_CYCLES 20
#define MAX_ENEMY_TYPES 10

class SurvivalModeConfig
{
public:
	SurvivalModeConfig();
	~SurvivalModeConfig();
	void Start();
	void End();
	void Update();
	void SetCGameObject(CGame* pGame);
	string GetTimePlayed();
	int GetTimePlayedAsInt();
	bool NewRecord();
	void UpdateRecord();
	int GetBestRecord();
	void SetBestRecord(int milisec);
	void InitObject();
	bool bIsSurvivalAvailable;

private:
	void SpawnEnemyUnit(int node, int dest, int myNodeRotator);
	//Use these methods to configurate cycles.
	void SetEnemyLevel(int cycle, int level); //Level if for all enemies.
	void SetNumRespaws(int cycle, int numRespaws);
	void SetRespawnTime(int cycle, int cycleTime);
	void AddEnemyType(int cycle, IsoEnemyType type);

	//Internal variables. (Don't touch them!)
	CGame* pGame;
	int    currCycle;
	int    maxCycle;
	int    numLeftRespawns;
	int    repetitiveCycle;
	int    numCycles;
	int    enemyLevel[MAX_CYCLES];
	int    numRespawnsCycle[MAX_CYCLES];
	int    cycleTime[MAX_CYCLES];
	int    numEnemyTypes[MAX_CYCLES];
	bool   bFirstSpawn;
	IsoEnemyType enemyType[MAX_CYCLES][MAX_ENEMY_TYPES];

	Timer timePlayed;
	Timer survivalEnemySpot;
	bool bIsActive;

	int lastRecord;
	int bestRecord;

	float multStatsBy;
	int varCycleTimer;
	int varInstericial;
	bool bPauseInsterticial;
	bool bPreInsterticial;
	int nodesRotator;
};

#endif