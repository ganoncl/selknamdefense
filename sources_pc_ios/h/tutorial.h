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

#ifndef _TUTORIAL_H
#define _TUTORIAL_H

#include "IwGx.h"
#include "button.h"
#include "timer.h"
#include <string>
using namespace std;

class ButtonRenderData;
class RenderData;
class CGame;
class MapPointer;
class MapSelection;
class Background;
class Store;
class Button;
#if defined DESKTOPPLATFORM
class Xbox360Controller;
#endif

typedef enum 
{
	Tutorial_None,
	Tutorial_Map1,
	Tutorial_Stars,
	Tutorial_UnitMenu,
	Tutorial_SurvivalMode,
	Tutorial_UnlockStore
} TutorialName;

typedef enum
{
	Tutorial_BreakPoint_None,
	Tutorial_BreakPoint_OpenUnitMenu,
	Tutorial_BreakPoint_SelectWarrior,
	Tutorial_BreakPoint_SelectMage,
	Tutorial_BreakPoint_SelectArcher,
	Tutorial_BreakPoint_DeployUnit,
	Tutorial_BreakPoint_EnemyFight,
	Tutorial_BreakPoint_EnemyDead,
	Tutorial_BreakPoint_MapSelection,
	Tutorial_BreakPoint_UpgradeArcher,
	Tutorial_BreakPoint_OpenUnitManager,
	Tutorial_BreakPoint_MoveSelknam,
	Tutorial_BreakPoint_ConfirmMovement
} Tutorial_BreakPoint;

typedef enum
{
	HandHelper_Direction_Up,
	HandHelper_Direction_Right,
	HandHelper_Direction_Down,
} HandHelper_Direction;

class Tutorial
{
private:
	TutorialName CurTutorial;
	int CurTutorialStep;
	bool bSkipped;
	void DrawMessage(const char* message);
    void DrawMessageCenterRight(const char* message);
	void InitDeviceHandler();
	void StopDeviceHandler();
	bool CanContinue();
	void RenderAuxCursor();
	void RenderHandHelper(bool bWindowOff = false, HandHelper_Direction helperDir = HandHelper_Direction_Down);
	bool bFirstOpen;
	Timer openTimer;
	RenderData* arrow;
	RenderData* auxCursor;
	RenderData* auxCursorB;
	int currTut;

	Button* btnSkipTutorial;

	Timer fadeTimer;
	Timer auxPointerTimer;
	bool bFadeIn;
	int fadeTime;
	int alpha;
	int blueCursorIndex;

	LerpTimer lerpTimer;
	LerpTimer animTimer;
	Timer animDelay;
	Timer winkTime;
	Timer winkDelay;
	Timer onClickDelay;
	bool bShowOnClickAnim;

	bool bClosing;
	Timer timeBeforeClose;

	float yCursorOffset;
	int maxYCursorOffset;

	float t; //movement factor, t e [0, 1]

	int tutorialYPos;
	int animTime;
	int renderPannelNumWidth;
	int renderPannelNumHeight;
	int widthTutoPannel;
	int heightTutoPannel;
	int pannelPosX;
	int pannelPosY;

	RenderData* selknamTutorialRD;
	RenderData* selknamTutorial2RD;

	CIwSVec2 vecWaveTimerGUI;
	CIwSVec2 vecAlmanacGUI;
	CIwSVec2 vecIngameMenuGUI;
	CIwSVec2 vecLifesGUI;
	CIwSVec2 vecGoldGUI;
	CIwSVec2 vecFastWaveGUI;

	CIwSVec2 currVecWaveTimerGUI;
	CIwSVec2 currVecAlmanacGUI;
	CIwSVec2 currVecIngameMenuGUI;
	CIwSVec2 currVecLifesGUI;
	CIwSVec2 currVecGoldGUI;
	CIwSVec2 currVecFastWaveGUI;

	CIwSVec2 basePosition;
	CIwSVec2 enemySpotPosition;
	CIwFVec2 auxCursorInitialPosition;
	CIwFVec2 auxCursorFinalPosition;
	CIwFVec2 auxCursorCurrPosition;
	CIwFVec2 handHelperPosition;

	bool bStartAnimDelay;
	bool bShowLifesGUI;
	bool bShowWaveTimerGUI;
	bool bShowFastWaveGUI;
	bool bShowGoldGUI;
	bool bBlockContinue;
	bool bDeployEnemyUnit;
	bool bShowAlmanacGUI;
	bool bShowIngameGUI;
	bool bRenderMessageCenterRight;
	bool bShowSkipTutButton;
	bool bWiseAnimation;

	int centerRightX, centerRightY;
	int buttonWidth;
	int showingGUITime;

	CGame* gameRef;
	MapSelection* mapSelectionRef;
	Store* storeRef;
	ButtonRenderData* button;

	int alphaBackground;
	int alphaSelknam;
	int alphaTextMessage;

	Timer alphaSelknamTimer;
	Timer alphaBackgroundTimer;
	Timer alphaTextMessageTimer;

	void FadeMessage(bool bFadeIn);
	void CheckIfFadeOutMessage();

	bool bIncreaseTutorialStepByOne;
	bool bBackgroundFadeIn;

#if defined DESKTOPPLATFORM
	Xbox360Controller* xbox360Controller;
#endif

	void UpdateStoreTutorial();
	void RenderStoreTutorial();
	void RenderTextMessage(const char* msg, int xOff, int yOff);

public:
	Tutorial();
	~Tutorial();
	void Update();
	void Render();
	void InitTutorial(TutorialName _TutorialName);
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void EndTutorial();
	void NextTutorialFrame();
	void FinishTutorial();
	void SetSkipped(bool b) { bSkipped = b; }
	void SkipTutorial();
	bool IsActive() { return CurTutorialStep != -1; }

	CIwSVec2 OffsetWaveTimerGUI() { return currVecWaveTimerGUI; };
	CIwSVec2 OffsetAlmanacGUI() { return currVecAlmanacGUI; };
	CIwSVec2 OffsetIngameMenuGUI() { return currVecIngameMenuGUI; };
	CIwSVec2 OffsetLifesGUI() { return currVecLifesGUI; };
	CIwSVec2 OffsetGoldGUI() { return currVecGoldGUI; };
	CIwSVec2 OffsetFastWaveGUI() { return currVecFastWaveGUI; };

	void HideGUIElements();
	void UnhideGUIElements();
	void SetBasePosition(CIwSVec2 newPos);
	void SetEnemySpotPosition(CIwSVec2 nePos);
	void SetCurrVectors();

	void SetGameRef(CGame* game);
	void SetMapSelectionRef(MapSelection* mapSelection);
	void SetStoreRef(Store* store);
	void SetAuxCursorInitPos(float xPos, float yPos);
	void SetAuxCursorFinalPos(float xPos, float yPos);
	void SetHandHelperPos(float xPos, float yPos);

	void ExecuteBreakpoint();
	TutorialName GetTutorialName() { return CurTutorial; };
	bool ClickeableFrame() { return CanContinue(); };

	Timer auxWaveTimer;
	Background* background;
	Tutorial_BreakPoint currBreakpoint;
};

#endif