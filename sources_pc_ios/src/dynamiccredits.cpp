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

#include "dynamiccredits.h"
#include "game.h"
#if !defined DESKTOPPLATFORM
#include "s3eFlurry.h"
#endif
#ifdef STEAMREQUIRED
#include "helpers/steam_achievements.h"
#endif

DynamicCredits* pDynamicCredits = NULL;

void DynamicCredits_HandleClick(s3ePointerEvent *event) 
{
	if(pDynamicCredits == NULL)
		return;

	pDynamicCredits->UpdateClickEvents(event);
}

#if defined DESKTOPPLATFORM
void DynamicCredits_Xbox360_Up()
{
	pDynamicCredits->EasterEggIsNextSeq(TapPlace_Up);
}

void DynamicCredits_Xbox360_Down()
{
	pDynamicCredits->EasterEggIsNextSeq(TapPlace_Down);
}

void DynamicCredits_Xbox360_Left()
{
	pDynamicCredits->EasterEggIsNextSeq(TapPlace_Left);
}
void DynamicCredits_Xbox360_Right()
{
	pDynamicCredits->EasterEggIsNextSeq(TapPlace_Right);
}
#endif

DynamicCredits::DynamicCredits()
{
	pDynamicCredits = this;

	easterEgg_active = false;
	numSections = 6;
	easterEgg_currSequence = 0;
	easterEggSpawnCycleMaster = 0;
	easterEggSpawnCycleMinor = 0;

	titleSection[0] = loc_credits_title0;
	titleSection[1] = loc_credits_title1;
	titleSection[2] = loc_credits_title2;
	titleSection[3] = loc_credits_title3;
	titleSection[4] = loc_credits_title4;
	titleSection[5] = loc_credits_title5;

	numNamesPerSection[0] = 1;
	namesSection[0][0] = loc_credits_name0;

	numNamesPerSection[1] = 1;
	namesSection[1][0] = loc_credits_name1;

	numNamesPerSection[2] = 1;
	namesSection[2][0] = loc_credits_name2;

	numNamesPerSection[3] = 1;
	namesSection[3][0] = loc_credits_name3;

	numNamesPerSection[4] = 1;
	namesSection[4][0] = loc_credits_name4;

	numNamesPerSection[5] = 3;
	namesSection[5][0] = loc_credits_name5;
	namesSection[5][1] = loc_credits_name6;
	namesSection[5][2] = loc_credits_name7;

	textPosition[0][0] = Utils::GetFixedScaledInt(128); textPosition[0][1] = Utils::GetScaledInt(256);
	textPosition[1][0] = Utils::GetFixedScaledInt(596); textPosition[1][1] = Utils::GetScaledInt(512);
	textPosition[2][0] = Utils::GetFixedScaledInt(128); textPosition[2][1] = Utils::GetScaledInt(512);
	textPosition[3][0] = Utils::GetFixedScaledInt(596); textPosition[3][1] = Utils::GetScaledInt(128);
	textPosition[4][0] = Utils::GetFixedScaledInt(212); textPosition[4][1] = Utils::GetScaledInt(128);
	textPosition[5][0] = Utils::GetFixedScaledInt(128); textPosition[5][1] = Utils::GetScaledInt(256);

	changeTextTimer.SetTimeLimit(6500);
	textAppearingTimer.SetTimeLimit(1000);
	dummyTimer.SetTimeLimit(75);

	bIsActive = false;

	easterEgg_Sequence[0] = TapPlace_Up;
	easterEgg_Sequence[1] = TapPlace_Up;
	easterEgg_Sequence[2] = TapPlace_Down;
	easterEgg_Sequence[3] = TapPlace_Down;
	easterEgg_Sequence[4] = TapPlace_Left;
	easterEgg_Sequence[5] = TapPlace_Right;
	easterEgg_Sequence[6] = TapPlace_Left;
	easterEgg_Sequence[7] = TapPlace_Right;

	easterEgg_timer.SetTimeLimit(5000);
	easterEgg_timer2.SetTimeLimit(2000);

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Up, &DynamicCredits_Xbox360_Up);
	xbox360Controller.SetCallback(Xbox360_Left, &DynamicCredits_Xbox360_Left);
	xbox360Controller.SetCallback(Xbox360_Right, &DynamicCredits_Xbox360_Right);
	xbox360Controller.SetCallback(Xbox360_Down, &DynamicCredits_Xbox360_Down);
#endif
}

DynamicCredits::~DynamicCredits()
{

}

void DynamicCredits::InitObject()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		s3eFlurryLogEvent("event_endingGame");
	}
#endif
#ifdef STEAMREQUIRED
	//GAME STATS
	if(g_SteamAchievements)
		g_SteamAchievements->StoreGlobalStats_SteamID("event_endingGame");
#endif
	bIsActive = true;
	easterEgg_active = false;

	currentSection = 0;
	currentPosition = 0;
	easterEggSpawnCycleMaster = 0;
	easterEggSpawnCycleMinor = 0;

	changeTextTimer.Reset();
	changeTextTimer.Start();
	textAppearingTimer.Reset();
	textAppearingTimer.Start();
	dummyTimer.Reset();
	dummyTimer.Start();

	s3eAudioStop();
	if(bIsAudioPaused)
	{
		LastMusicStacked = Music_Ending;
	}
	else
	{
		Utils::PlayBGM("bgm/sd_ending_theme.mp3", 0);
		LastMusicStacked = Music_None;
	}

	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)DynamicCredits_HandleClick, NULL);

	easterEgg_currSequence = 0;
}

void DynamicCredits::EndObject()
{
	bIsActive = false;
	easterEgg_active = false;
	bBlockCamera = false;

	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)DynamicCredits_HandleClick);

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		pGame->isoEnemyArr[i].CleanIso();
	}
}

void DynamicCredits::Update()
{
	if(!bIsActive) return;

#if defined DESKTOPPLATFORM
	if(!easterEgg_active)
		xbox360Controller.Update();
#endif

	changeTextTimer.Update();
	textAppearingTimer.Update();
	dummyTimer.Update();

	if(dummyTimer.GetIsEnd() && !easterEgg_active)
	{
		if(!bPauseGame)
		{
			bPauseGame = true;
		}
	}

	if(changeTextTimer.GetIsEnd() && currentSection < numSections && !easterEgg_active)
	{
		currentSection++;

		currentPosition = (currentPosition + 1) % MAX_POSITIONS;

		if(currentSection >= numSections)
		{
			fade->FadeIn();
		}
		else
		{
			changeTextTimer.Reset();
			changeTextTimer.Start();
			textAppearingTimer.Reset();
			textAppearingTimer.Start();
		}
	}

	if(fade->GetAlpha() == 255)
	{
		gameState = GS_MainMenu;
		mainMenuState = GSM_FinalScreen;
	}

	bBlockCamera = !easterEgg_active;

	if(easterEgg_currSequence >= 8 && !easterEgg_active)
	{
		EasterEgg_Init();
	}

	EasterEgg_Update();
}


void DynamicCredits::EasterEgg_Init()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		s3eFlurryLogEvent("event_easterEggActive");
	}
#endif
#ifdef STEAMREQUIRED
	//GAME STATS
	if(g_SteamAchievements)
		g_SteamAchievements->StoreGlobalStats_SteamID("event_easterEggActive");
#endif
	easterEgg_active = true;
	bPause = false;
	easterEgg_timer2.Reset();
	easterEgg_timer2.Start();
	pGame->InitAttackStage();

	if(bPauseGame)
	{
		bPauseGame = false;
	}

	s3eAudioStop();
	if(bIsAudioPaused)
	{
		LastMusicStacked = Music_Forest_Ingame_02;
	}
	else
	{
		Utils::PlayBGM("bgm/sd_ingame_forest_02_theme_loop.mp3", 0);
		LastMusicStacked = Music_None;
	}

	//Achievement nº 15.
	//Gran secreto: Descubre la cruda realidad.
	achievementsHelper->SetAchievementCompletedById(14);
}

void DynamicCredits::EasterEgg_Update()
{
	if(easterEgg_active)
	{
		//easterEgg_timer.Update();
		easterEgg_timer2.Update();
		/*
		if(easterEgg_timer.GetIsEnd() && pGame->bRenderAttackText)
		{
			easterEgg_timer2.Reset();
			easterEgg_timer2.Start();
			pGame->bRenderAttackText = false;
		}
		*/
		if(easterEgg_timer2.GetIsEnd())
		{
			SpawnEnemyUnit(0, 1);
			easterEgg_timer2.Reset();
			easterEgg_timer2.Start();
			easterEggSpawnCycleMinor++;
			if(easterEggSpawnCycleMinor >= 5)
			{
				easterEggSpawnCycleMinor = 0;
				easterEggSpawnCycleMaster++;
			}
		}
	}
}

void DynamicCredits::SpawnEnemyUnit(int node, int dest)
{
	if(pGame->initNodes[node] == NULL) 
	{
		//IwTrace(BOSSBATTLE, ("initNodes[node] = NULL"));
		return;
	}

	//Enemy Spawn
	IsoEnemy* CurrentEnemy = NULL;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		CurrentEnemy = &pGame->isoEnemyArr[i];

		if(CurrentEnemy == NULL) continue;

		if(CurrentEnemy->getInitialized()) continue;

		int enemyIndex = rand() % 7 + 1;
		CurrentEnemy->setEnemyType((IsoEnemyType)enemyIndex);
		CurrentEnemy->setEnemyLevel(3);

		CurrentEnemy->setInitPathNum(node + 1);
		CurrentEnemy->setTargetNum(dest);

		CurrentEnemy->InitIsoPawn(pGame->initNodes[node]);

		float myMasterCycle = (float)easterEggSpawnCycleMaster;
		float myExtraHealth = 0.2f * myMasterCycle + 1;

		CurrentEnemy->SetCurrentHealth((int)(CurrentEnemy->GetMaxHealth() * myExtraHealth));
		CurrentEnemy->SetCurrentMaxHealth((int)(CurrentEnemy->GetMaxHealth() * myExtraHealth));

		return;
	}
}

void DynamicCredits::Render()
{
	if(!bIsActive || easterEgg_active) return;

	IwGxLightingOn();
	
	if(currentSection < numSections)
	{
		//Entering Text
		int alpha;
		if(textAppearingTimer.GetIsEnd())
		{
			alpha = 255;
		}
		else
		{
			alpha = (int)((255 / (float)textAppearingTimer.GetTimeLimit()) * textAppearingTimer.GetCurrTime());
		}

		CIwColour myFontColor;
		myFontColor.r = 255;
		myFontColor.g = 255;
		myFontColor.b = 255;
		myFontColor.a = alpha;

		Utils::RenderText(titleSection[currentSection].c_str(), 
			CIwRect(textPosition[currentPosition][0], textPosition[currentPosition][1], 600, 40), 
			font_komikadisplaybold22, 
			myFontColor, 
			IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_MIDDLE, 
			false, 2);

		for(int i = 0; i < numNamesPerSection[currentPosition]; i++)
		{
			int margin = 60;
			Utils::RenderText(namesSection[currentSection][i].c_str(),
				CIwRect(textPosition[currentPosition][0], textPosition[currentPosition][1] + margin * (i + 1), 300, 40), 
				font_komikadisplaybold18, 
				myFontColor, 
				IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_MIDDLE, 
				false, 2);
		}

		//Exiting Text
		if(currentSection > 1)
		{
			if(textAppearingTimer.GetIsEnd())
			{
				alpha = 0;
			}
			else
			{
				alpha = (int)(255 * (1 - ((textAppearingTimer.GetCurrTime()) / (float)(textAppearingTimer.GetTimeLimit()))));
			}

			CIwColour myFontColor;
			myFontColor.r = 255;
			myFontColor.g = 255;
			myFontColor.b = 255;
			myFontColor.a = alpha;

			Utils::RenderText(titleSection[currentSection - 1].c_str(), 
				CIwRect(textPosition[currentPosition - 1][0], textPosition[currentPosition - 1][1], 600, 40), 
				font_komikadisplaybold22, myFontColor, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_MIDDLE, 
				false, 2);

			for(int i = 0; i < numNamesPerSection[currentPosition - 1]; i++)
			{
				int margin = 60;
				Utils::RenderText(namesSection[currentSection - 1][i].c_str(),
					CIwRect(textPosition[currentPosition - 1][0], textPosition[currentPosition - 1][1] + margin * (i + 1), 300, 40), 
					font_komikadisplaybold18, myFontColor, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_MIDDLE, 
					false, 2);
			}
		}
	}


	IwGxLightingOn();
}

void DynamicCredits::UpdateClickEvents(s3ePointerEvent *event)
{
	if(easterEgg_currSequence >= 8)
		return;

	int32 areaToTap = 117;
	int32 secureArea = 5;

	if(event->m_Pressed)
	{
		//UP
		if(event->m_x > (areaToTap + secureArea) 
		   && event->m_x < ((int32)currentGameWidth - (areaToTap + secureArea))
		   && event->m_y > 0
		   && event->m_y < areaToTap)
		{
			if(easterEgg_Sequence[easterEgg_currSequence] == TapPlace_Up)
			{
				easterEgg_currSequence++;
			}
			else
			{
				easterEgg_currSequence = 0;
			}
		}
		else
		//DOWN
		if(event->m_x > (areaToTap + secureArea) 
			&& event->m_x < ((int32)currentGameWidth - secureArea)
			&& event->m_y > ((int32)currentGameHeight - areaToTap)
			&& event->m_y < (int32)currentGameHeight)
		{
			if(easterEgg_Sequence[easterEgg_currSequence] == TapPlace_Down)
			{
				easterEgg_currSequence++;
			}
			else
			{
				easterEgg_currSequence = 0;
			}
		}
		else
		//LEFT
		if(event->m_x > 0 
			&& event->m_x < areaToTap
			&& event->m_y > (areaToTap + secureArea)
			&& event->m_y < ((int32)currentGameHeight - (areaToTap + secureArea)))
		{
			if(easterEgg_Sequence[easterEgg_currSequence] == TapPlace_Left)
			{
				easterEgg_currSequence++;
			}
			else
			{
				easterEgg_currSequence = 0;
			}
		}
		else
		//RIGHT
		if(event->m_x > ((int32)currentGameWidth - areaToTap) 
			&& event->m_x < (int32)currentGameWidth
			&& event->m_y > (areaToTap + secureArea)
			&& event->m_y < ((int32)currentGameHeight - (areaToTap + secureArea)))
		{
			if(easterEgg_Sequence[easterEgg_currSequence] == TapPlace_Right)
			{
				easterEgg_currSequence++;
			}
			else
			{
				easterEgg_currSequence = 0;
			}
		}
		else
		{
			easterEgg_currSequence = 0;
		}
	}
}

void DynamicCredits::EasterEggIsNextSeq(TapPlace place)
{
	if(easterEgg_Sequence[easterEgg_currSequence] == place)
	{
		easterEgg_currSequence++;
	}
	else
	{
		easterEgg_currSequence = 0;
	}
}

bool DynamicCredits::EasterEggActive()
{
	return easterEgg_active;
}

void DynamicCredits::SetGameRef(CGame* pGame)
{
	this->pGame = pGame;
}
