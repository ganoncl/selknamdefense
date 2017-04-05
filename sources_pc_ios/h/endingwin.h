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

#ifndef _ENDINGWIN_H
#define _ENDINGWIN_H

#include "IwGx.h"
#include "IwGxFont.h"
#include "button.h"
#include "timer.h"
#include "background.h"
#include "dynamiccounter.h"

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

class EndingWin
{
public:
	EndingWin();
	~EndingWin();
	void Update();
	void Render();
	void InitObject();
	void EndObject();
	bool bIsActiveObj;

	
	int GetHeartBonus() const { return heartBonus; }
	void SetHeartBonus(int val) { heartBonus = val; }
	int GetCoinsBonus() const { return coinsBonus; }
	void SetCoinsBonus(int val) { coinsBonus = val; }
	int GetWaveBonus() const { return waveBonus; }
	void SetWaveBonus(int val) { waveBonus = val; }

	int GetLifesOnLevel() const { return lifesOnLevel; }
	void SetLifesOnLevel(int val) { lifesOnLevel = val; }

	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);

	void CheckAchievement_FirstApproach();
	void PerformWriteBestScoreOnLB(int myLevel, int currentScore);

	s3eKey lastKeyPressed;

	Timer time_victory_picture;
	bool bShowVictoryPicture;

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

private:
	CIwGxFont* pfont;
	uint32 textColor;

	CIwRect titlePos;
	const char* title;

	CIwRect heartBonusPos;
	CIwRect coinsBonusPos;
	CIwRect waveBonusPos;
	CIwRect totalPointsPos;

	CIwRect heartBonusPos_n;
	CIwRect coinsBonusPos_n;
	CIwRect waveBonusPos_n;
	CIwRect totalPointsPos_n;

	int heartBonus;
	int coinsBonus;
	int waveBonus;
	
	int totalScore;

	int lifesOnLevel;

	Button* btnContinue;

	CIwSVec2 star_position[3][3];
	bool bRenderStars;
	int num_stars;
	int current_stars;
	Timer star_timer;

	Background* background;

	bool bBlockContinueButton;

	DynamicCounter heart_counter;
	DynamicCounter coins_counter;
	DynamicCounter wave_counter;
	DynamicCounter total_counter;

	CIwSVec2 VictoryPosition;
	RenderData VictoryRenderData;

	ButtonRenderData buttonRenderData;
	RenderData label_starBig;

#if !defined DESKTOPPLATFORM
	ButtonRenderData facebookButtonRD;
	Button* facebookButton;

	ButtonRenderData twitterButtonRD;
	Button* twitterButton;
#endif


	int renderPanelNumWidth;
	int renderPanelNumHeight;
	int panelXPos;
	int panelYPos;
	int panelWidth;
	int panelHeight;
	CIwSVec2 panelPosition;

	bool bShowingiAd;
};

#endif

