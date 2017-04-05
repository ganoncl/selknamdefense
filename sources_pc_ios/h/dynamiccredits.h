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

#ifndef _DYNAMICCREDITS_H
#define _DYNAMICCREDITS_H

#include "IwGx.h"
#include "utils.h"
#include "common.h"
#include "localization.h"
#include "fadescreen.h"

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#endif

class CGame;

#define MAX_SUBSECTIONS 10
#define MAX_NAMES_PER_SECTION 5
#define MAX_POSITIONS 6

typedef enum
{
	TapPlace_None,
	TapPlace_Up,
	TapPlace_Down,
	TapPlace_Right,
	TapPlace_Left
} TapPlace;

class DynamicCredits
{
public:
	DynamicCredits();
	~DynamicCredits();
	void InitObject();
	void EndObject();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);

	bool GetIsActive()
	{
		return bIsActive;
	}

	void SetGameRef(CGame* pGame);
	bool EasterEggActive();
	void EasterEggIsNextSeq(TapPlace place);
	void EasterEgg_Init();

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
#endif

private:
	void EasterEgg_Update();
	void SpawnEnemyUnit(int node, int dest);

	CGame* pGame;
	Timer easterEgg_timer;
	Timer easterEgg_timer2;

	bool bIsActive;

	string titleSection[MAX_SUBSECTIONS];
	string namesSection[MAX_SUBSECTIONS][MAX_NAMES_PER_SECTION];

	int textPosition[MAX_POSITIONS][2];
	int numNamesPerSection[MAX_SUBSECTIONS];
	int numSections;
	int currentSection;
	int currentPosition;

	Timer changeTextTimer;
	Timer textAppearingTimer;
	Timer dummyTimer;

	TapPlace easterEgg_Sequence[8];
	int easterEgg_currSequence;
	bool easterEgg_active;
	int easterEggSpawnCycleMaster;
	int easterEggSpawnCycleMinor;
};

#endif