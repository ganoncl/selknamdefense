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

#include "tutorial.h"
#include "common.h"
#include "localization.h"
#include "utils.h"
#include "texturehelper.h"
#include "game.h"
#include "mappointer.h"
#include "ms_mapselect.h"
#include "ms_store.h"
#include "background.h"
#if !defined DESKTOPPLATFORM
#include "s3eFlurry.h"
#endif
#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#endif

Tutorial* TutorialReference = NULL;

#define TIME_CURSOR_IDLE 2100 //cursor standing over a place. (doing clicking effect)
#define TIME_CLICK 700 //400 ms click state, 400 ms non-click state, swapping until TIME_CURSOR_IDLE is over.
#define CURSOR_SPEED 0.025 //value between 0 and 1, t += CURSOR_SPEED each frame.

#define STAGE1_MAGE_POSITION_X 12
#define STAGE1_MAGE_POSITION_Y 9

//Mouse Motion
void HandleMotionTutorial(s3ePointerMotionEvent* event) 
{
	if(TutorialReference == NULL)
		return;

	TutorialReference->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClickTutorial(s3ePointerEvent *event) 
{
	if(TutorialReference == NULL)
		return;

	TutorialReference->UpdateClickEvents(event);
}

void EndTutorialAction()
{
	if(TutorialReference != NULL)
	{
		TutorialReference->EndTutorial();
		TutorialReference->SetSkipped(true);
#if !defined DESKTOPPLATFORM
		if(bFlurryOn)
		{
			string tmpValue = "Finished|False";
			s3eFlurryLogEventParams("event_Tutorial", tmpValue.c_str());
		}
#endif
	}
}

void Finish()
{
	if(TutorialReference != NULL)
	{
		TutorialReference->FinishTutorial();
	}
}

void NextFrame()
{
	if(TutorialReference != NULL)
	{
		TutorialReference->NextTutorialFrame();
	}
}

void Tutorial_SkipTutorial()
{
	if(TutorialReference != NULL)
	{
		TutorialReference->SkipTutorial();
	}
}

Tutorial::Tutorial()
{
	bBackgroundFadeIn = true;
	bIncreaseTutorialStepByOne = false;

	alphaSelknam = alphaBackground = alphaTextMessage = 255;
	alphaSelknamTimer.SetTimeLimit(400);
	alphaBackgroundTimer.SetTimeLimit(200);
	alphaTextMessageTimer.SetTimeLimit(400);

	TutorialReference = this;

	tutorialYPos = Utils::GetScaledInt(450);

	CurTutorial = Tutorial_None;
	CurTutorialStep = -1;

	RenderData tmpData = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("finger_tuto_c")), MIRROR_HORIZONTAL);
	arrow = new RenderData();
	arrow->bRotate = tmpData.bRotate;
	arrow->height = tmpData.height;
	arrow->uvInfo = tmpData.uvInfo;
	arrow->width = tmpData.width;
	arrow->atlasName = tmpData.atlasName;

	
	tmpData = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("finger_tuto_a")), MIRROR_HORIZONTAL);
	
	auxCursor = new RenderData();
	Utils::CopyRenderData(auxCursor, tmpData);

	tmpData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("finger_tuto_a"));
	
	//90° rotation.
	/*
	UVInfo tmpUV = tmpData.uvInfo;
	tmpData.uvInfo.maxX = tmpUV.minX;
	tmpData.uvInfo.maxY = tmpUV.maxY;
	tmpData.uvInfo.minX = tmpUV.maxX;
	tmpData.uvInfo.minY = tmpUV.minY;
	*/

	auxCursorB = new RenderData();
	Utils::CopyRenderData(auxCursorB, tmpData);

	int tutorialBtnYOffset = 122;

	bSkipped = false;
	bFirstOpen = true;
	openTimer.SetTimeLimit(2000);

	fadeTime = 500;
	fadeTimer.SetTimeLimit(fadeTime);

	renderPannelNumWidth = 31;
	renderPannelNumHeight = 6;
	widthTutoPannel = (renderPannelNumWidth + 2) * 20;
	heightTutoPannel = (renderPannelNumHeight + 2) * 20;
	pannelPosX = (int)(currentGameWidth * 0.5f) - (int)(widthTutoPannel * 0.5f);
	pannelPosY = currentGameHeight - 180;

	centerRightX = currentGameWidth - widthTutoPannel - Utils::GetScaledInt(120);
	centerRightY = (int)(currentGameHeight * 0.5f) - (int)(heightTutoPannel * 0.5f);

	selknamTutorialRD = new RenderData();
	TextureData tmpTD = TextureHelper::GetTextureNamed("selknam_tutorial_128x128");
	RenderData tmpRD = TextureHelper::GetRenderData(tmpTD);
	selknamTutorialRD->atlasName = tmpRD.atlasName;
	selknamTutorialRD->bRotate = tmpRD.bRotate;
	selknamTutorialRD->height = tmpRD.height;
	selknamTutorialRD->uvInfo = tmpRD.uvInfo;
	selknamTutorialRD->width = tmpRD.width;

	selknamTutorial2RD = new RenderData();
	TextureData tmp2TD = TextureHelper::GetTextureNamed("selknam_tutorial_128x128_eyepatch");
	RenderData tmp2RD = TextureHelper::GetRenderData(tmp2TD);
	selknamTutorial2RD->atlasName = tmp2RD.atlasName;
	selknamTutorial2RD->bRotate = tmp2RD.bRotate;
	selknamTutorial2RD->height = tmp2RD.height;
	selknamTutorial2RD->uvInfo = tmp2RD.uvInfo;
	selknamTutorial2RD->width = tmp2RD.width;


	vecFastWaveGUI = CIwSVec2(0,0);
	vecAlmanacGUI = CIwSVec2(0,0);
	vecGoldGUI = CIwSVec2(0,0);
	vecIngameMenuGUI = CIwSVec2(0,0);
	vecWaveTimerGUI = CIwSVec2(0,0);
	basePosition = CIwSVec2(0, 0);

	bShowLifesGUI = false;

	animTime = 900;
	showingGUITime = 500;

	animDelay.SetTimeLimit(TIME_CURSOR_IDLE);
	onClickDelay.SetTimeLimit(TIME_CLICK);

	auxPointerTimer.SetTimeLimit(100);
	auxPointerTimer.Start();

	timeBeforeClose.SetTimeLimit(50);

	bStartAnimDelay = true;
	blueCursorIndex = 0;
	bBlockContinue = false;
	bShowWaveTimerGUI = false;
	bShowFastWaveGUI = false;
	bDeployEnemyUnit = false;
	bShowIngameGUI = false;
	bShowAlmanacGUI = false;
	bShowGoldGUI = false;
	bWiseAnimation = true;
	bShowSkipTutButton = false;
	bShowOnClickAnim = false;
	bClosing = false;
	currBreakpoint = Tutorial_BreakPoint_None;

	auxWaveTimer.SetTimeLimit(7000);

	mapSelectionRef = NULL;

	background = new Background(0,0,0, 170);

	bRenderMessageCenterRight = false;

	TextureData buttonData = TextureHelper::GetTextureNamed("btn_popup_long");
	button = new ButtonRenderData();
	button->numStates = 1;
	button->state[0].RollOut  = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	button->state[0].RollOn   = TextureHelper::GetRenderData(buttonData, 1, 3, 1);
	button->state[0].Pressed  = TextureHelper::GetRenderData(buttonData, 1, 3, 2);
	button->state[0].Inactive = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	buttonWidth = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).x;
	int buttonHeight = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).y;

	btnSkipTutorial = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, (int)(currentGameWidth * 0.5f), 0);
	btnSkipTutorial->OnClick(&Tutorial_SkipTutorial);
	btnSkipTutorial->SetText(loc_ui_skip);
	btnSkipTutorial->SetStyle(font_SmartKid20, 10, -20);

	winkTime.SetTimeLimit(100);
	winkDelay.SetTimeLimit(2000);

	yCursorOffset = 0;
	maxYCursorOffset = 45;

	handHelperPosition = CIwSVec2(0,0);

#if defined DESKTOPPLATFORM
	xbox360Controller = new Xbox360Controller();
	xbox360Controller->SetCallback(Xbox360_Action1, &NextFrame);
	xbox360Controller->SetCallback(Xbox360_Action2, &NextFrame);
	xbox360Controller->SetCallback(Xbox360_Cancel1, &Tutorial_SkipTutorial);
	xbox360Controller->SetCallback(Xbox360_Cancel2, &Tutorial_SkipTutorial);
#endif
}

Tutorial::~Tutorial()
{
	delete background;
	delete arrow;
	delete selknamTutorialRD;
	delete selknamTutorial2RD;
	delete auxCursor;
	delete auxCursorB;
	delete button;
	delete btnSkipTutorial;
#if defined DESKTOPPLATFORM
	delete xbox360Controller;
#endif
}

void Tutorial::HideGUIElements()
{
	vecWaveTimerGUI = CIwSVec2(0, -120);
	vecGoldGUI = CIwSVec2(0, -120);
	vecLifesGUI = CIwSVec2(0, -120);
	vecIngameMenuGUI = CIwSVec2(0, -120);
	vecAlmanacGUI = CIwSVec2(0, -120);
	vecFastWaveGUI = CIwSVec2(0, 120);

	SetCurrVectors();
}

void Tutorial::UnhideGUIElements()
{
	vecWaveTimerGUI = CIwSVec2(0, 0);
	vecGoldGUI = CIwSVec2(0, 0);
	vecLifesGUI = CIwSVec2(0, 0);
	vecIngameMenuGUI = CIwSVec2(0, 0);
	vecAlmanacGUI = CIwSVec2(0, 0);
	vecFastWaveGUI = CIwSVec2(0, 0);

	SetCurrVectors();
}

void Tutorial::SetCurrVectors()
{
	currVecWaveTimerGUI = vecWaveTimerGUI;
	currVecGoldGUI = vecGoldGUI;
	currVecLifesGUI = vecLifesGUI;
	currVecIngameMenuGUI = vecIngameMenuGUI;
	currVecAlmanacGUI = vecAlmanacGUI;
	currVecFastWaveGUI = vecFastWaveGUI;
}

void Tutorial::FadeMessage(bool bFadeIn)
{
	bBackgroundFadeIn = bFadeIn;
	alphaBackgroundTimer.Start();
	alphaSelknamTimer.Start();
}

void Tutorial::Update()
{
	if(CurTutorial == Tutorial_None)
	{
		return;
	}

	//IwTrace(SADASDA, ("%d", CurTutorialStep));

#if defined DESKTOPPLATFORM
	xbox360Controller->Update();
#endif

	openTimer.Update();
	fadeTimer.Update();
	auxPointerTimer.Update();
	alphaSelknamTimer.Update();
	alphaBackgroundTimer.Update();
	alphaTextMessageTimer.Update();

	if(bIncreaseTutorialStepByOne)
	{
		if(alphaBackgroundTimer.GetIsEnd() 
		   && alphaSelknamTimer.GetIsEnd()
		   && (alphaTextMessageTimer.GetIsEnd() || (!alphaTextMessageTimer.GetIsEnd() && alphaTextMessage <= 0)))
		{
			
			bIncreaseTutorialStepByOne = false;
			CurTutorialStep++;
		}
	}

	if(!alphaSelknamTimer.GetIsEnd())
	{
		if(bBackgroundFadeIn)
			alphaSelknam = Utils::GetFadeAlpha(alphaSelknamTimer.GetCurrTime(), alphaSelknamTimer.GetTimeLimit(), true);
			
		if(!bBackgroundFadeIn)
			alphaBackground = Utils::GetFadeAlpha(alphaSelknamTimer.GetCurrTime(), alphaSelknamTimer.GetTimeLimit(), false);
	}
	else
	{
		if(bBackgroundFadeIn)
			alphaSelknam = 255;
		else
			alphaBackground = 0;
	}

	if(!alphaBackgroundTimer.GetIsEnd())
	{
		if(bBackgroundFadeIn)
			alphaBackground = Utils::GetFadeAlpha(alphaBackgroundTimer.GetCurrTime(), alphaBackgroundTimer.GetTimeLimit(), true);

		if(!bBackgroundFadeIn)
			alphaSelknam = Utils::GetFadeAlpha(alphaBackgroundTimer.GetCurrTime(), alphaBackgroundTimer.GetTimeLimit(), false);
	}
	else
	{
		if(bBackgroundFadeIn)
			alphaBackground = 255;
		else
			alphaBackground = 0;
	}

	if(!alphaTextMessageTimer.GetIsEnd())
	{
		alphaTextMessage = Utils::GetCompleteAlpha(alphaTextMessageTimer.GetCurrTime(), alphaTextMessageTimer.GetTimeLimit(), false);
	}
	else
	{
		alphaTextMessage = 255;
	}

	if(auxPointerTimer.GetIsEnd())
	{
		blueCursorIndex++;
		if(blueCursorIndex > 3)
			blueCursorIndex = 0;
		auxPointerTimer.Reset();
		auxPointerTimer.Start();
	}

	if(!fadeTimer.GetIsEnd())
	{
		if(bFadeIn)
		{
			alpha = (int)(255 * (fadeTimer.GetCurrTime() / (float)fadeTime));
		}
		else
		{
			alpha = (int)(255 * (1 - fadeTimer.GetCurrTime() / (float)fadeTime));
		}

		yCursorOffset = alpha / (float)255;
	}
	else
	{
		fadeTimer.Reset();
		fadeTimer.Start();
		bFadeIn = !bFadeIn;
	}

	if(currTut != CurTutorialStep)
	{
		currTut = CurTutorialStep;
		if(CurTutorial == Tutorial_Map1)
		{
			if(CurTutorialStep == 0)
			{
				bShowSkipTutButton = true;

				Utils::FreezeCamera();
			}
			else if(CurTutorialStep == 1)
			{
				if(!Utils::IsCameraMoving())
				{
					Utils::MoveCamera(75, CIwFVec2((float)0, (float)0), CIwFVec2((float)(250), (float)(50)));
				}



				fadeTimer.Reset();
				fadeTimer.Start();
				bFadeIn = false;
			}
			else if(CurTutorialStep == 2)
			{		
				lerpTimer.Start(showingGUITime);
				bShowLifesGUI = true;

				fadeTimer.Reset();
				fadeTimer.Start();
				bFadeIn = false;
			}
			else if(CurTutorialStep == 3)
			{		
				if(!Utils::IsCameraMoving())
				{
					Utils::MoveCamera(75, CIwFVec2((float)0, (float)0), CIwFVec2((float)(-550), (float)(-150)));
				}

				fadeTimer.Reset();
				fadeTimer.Start();
				bFadeIn = false;
			}
			else if(CurTutorialStep == 4)
			{
				if(!Utils::IsCameraMoving())
				{
					Utils::MoveCamera(75, CIwFVec2((float)0, (float)0), CIwFVec2((float)(150), (float)(180)));
				}

				fadeTimer.Reset();
				fadeTimer.Start();
				bFadeIn = false;
				//animTimer.Start(animTime);
				t = 0;
				//SetAuxCursorInitPos(gameRef->isoTerrainAstarGrid[12][6]->getPosX(), gameRef->isoTerrainAstarGrid[12][6]->getPosY());
				//SetAuxCursorFinalPos(gameRef->isoTerrainAstarGrid[10][8]->getPosX(), gameRef->isoTerrainAstarGrid[10][8]->getPosY());
				
				SetHandHelperPos(gameRef->isoTerrainAstarGrid[10][8]->getPosX(), gameRef->isoTerrainAstarGrid[10][8]->getPosY());

				gameRef->tileToSelect = gameRef->isoTerrainAstarGrid[10][8];
				
				bBlockContinue = true;
				bBlockGUI = true;
				bStopWaveTimer = true;
				Utils::UnpauseGame();

				currBreakpoint = Tutorial_BreakPoint_OpenUnitMenu;

				animDelay.Start();
			}
			else if(CurTutorialStep == 5)
			{
				//animTimer.Start(animTime);
				t = 0;
				//SetAuxCursorInitPos(gameRef->isoTerrainAstarGrid[10][11]->getPosX(), gameRef->isoTerrainAstarGrid[10][11]->getPosY());

				float tempX = gameRef->unitMenu.GetLeftButton()->GetX() + gameRef->unitMenu.GetLeftButton()->GetWidth() * 0.5f - 40;
				float tempY = gameRef->unitMenu.GetLeftButton()->GetY() + gameRef->unitMenu.GetLeftButton()->GetHeight() * 0.5f;

				//SetAuxCursorFinalPos(tempX, tempY);
				SetHandHelperPos(tempX, tempY);

				gameRef->unitMenu.AllowOnlyButton(gameRef->unitMenu.GetLeftButton());

				currBreakpoint = Tutorial_BreakPoint_SelectWarrior;

				animDelay.Start();
			}
			else if(CurTutorialStep == 6)
			{
				gameRef->unitMenu.AllowOnlyButton(gameRef->unitMenu.GetLeftButton());

				currBreakpoint = Tutorial_BreakPoint_DeployUnit;
			}
			else if(CurTutorialStep == 7)
			{
				gameRef->unitMenu.AllowOnlyButton(NULL);
				gameRef->unitMenu.Block();
				gameRef->unitManager.Block();
				bBlockContinue = false;
				gameRef->nextWaveButton_x1->Enabled(true);

				FadeMessage(true);
			}
			else if(CurTutorialStep == 8)
			{
				lerpTimer.Start(showingGUITime);
				bShowWaveTimerGUI = true;
				currBreakpoint = Tutorial_BreakPoint_EnemyFight;
				auxWaveTimer.Reset();
				
				bDeployEnemyUnit = true;
			}
			else if(CurTutorialStep == 9)
			{
				float tempX = gameRef->nextWaveButton_x1->GetX() + gameRef->nextWaveButton_x1->GetWidth() * 0.5f + 20;
				float tempY = gameRef->nextWaveButton_x1->GetY() + gameRef->nextWaveButton_x1->GetHeight() * 0.5f - 60;
				SetHandHelperPos(tempX, tempY);

				lerpTimer.Start(showingGUITime);
				bShowFastWaveGUI = true;
				
				auxWaveTimer.Start();
			}
			else if(CurTutorialStep == 10)
			{
				currBreakpoint = Tutorial_BreakPoint_EnemyDead;
				bBlockContinue = true;
			}
			else if(CurTutorialStep == 11)
			{
				bBlockContinue = false;
				FadeMessage(true);
			}
			else if(CurTutorialStep == 12)
			{
				bShowGoldGUI = true;
				lerpTimer.Start(showingGUITime);
			}
			else if(CurTutorialStep == 13)
			{
				fadeTimer.Reset();
				fadeTimer.Start();
				bFadeIn = false;
				//animTimer.Start(animTime);
				t = 0;
				//SetAuxCursorInitPos(gameRef->isoTerrainAstarGrid[12][6]->getPosX(), gameRef->isoTerrainAstarGrid[12][6]->getPosY());
				//SetAuxCursorFinalPos(gameRef->isoTerrainAstarGrid[11][7]->getPosX(), gameRef->isoTerrainAstarGrid[11][7]->getPosY());
				SetHandHelperPos(gameRef->isoTerrainAstarGrid[STAGE1_MAGE_POSITION_X][STAGE1_MAGE_POSITION_Y]->getPosX(), gameRef->isoTerrainAstarGrid[STAGE1_MAGE_POSITION_X][STAGE1_MAGE_POSITION_Y]->getPosY());
				
				gameRef->tileToSelect = gameRef->isoTerrainAstarGrid[STAGE1_MAGE_POSITION_X][STAGE1_MAGE_POSITION_Y];
				bBlockContinue = true;
				currBreakpoint = Tutorial_BreakPoint_OpenUnitMenu;
				gameRef->unitManager.Unblock();
				gameRef->unitMenu.Unblock();

				animDelay.Start();
			}
			else if(CurTutorialStep == 14)
			{
				//animTimer.Start(animTime);
				t = 0;
				//SetAuxCursorInitPos(gameRef->isoTerrainAstarGrid[13][11]->getPosX(), gameRef->isoTerrainAstarGrid[13][11]->getPosY());

				float tempX = gameRef->unitMenu.GetDownButton()->GetX() + gameRef->unitMenu.GetDownButton()->GetWidth() * 0.5f;
				float tempY = gameRef->unitMenu.GetDownButton()->GetY() + gameRef->unitMenu.GetDownButton()->GetHeight() * 0.5f + 55;

				//SetAuxCursorFinalPos(tempX, tempY);
				SetHandHelperPos(tempX, tempY);

				gameRef->unitMenu.AllowOnlyButton(gameRef->unitMenu.GetDownButton());

				currBreakpoint = Tutorial_BreakPoint_SelectMage;

				animDelay.Start();
			}
			else if(CurTutorialStep == 15)
			{
				gameRef->unitMenu.AllowOnlyButton(gameRef->unitMenu.GetDownButton());

				currBreakpoint = Tutorial_BreakPoint_DeployUnit;
			}
			else if(CurTutorialStep == 16)
			{
				SetHandHelperPos(gameRef->isoTerrainAstarGrid[STAGE1_MAGE_POSITION_X][STAGE1_MAGE_POSITION_Y]->getPosX(), gameRef->isoTerrainAstarGrid[STAGE1_MAGE_POSITION_X][STAGE1_MAGE_POSITION_Y]->getPosY());
				gameRef->tileToSelect = gameRef->isoTerrainAstarGrid[STAGE1_MAGE_POSITION_X][STAGE1_MAGE_POSITION_Y];
				currBreakpoint = Tutorial_BreakPoint_OpenUnitManager;				
				FadeMessage(true);
				/*
				fadeTimer.Reset();
				fadeTimer.Start();
				bFadeIn = false;
				//animTimer.Start(animTime);
				t = 0;
				//SetAuxCursorInitPos(gameRef->isoTerrainAstarGrid[13][6]->getPosX(), gameRef->isoTerrainAstarGrid[13][6]->getPosY());
				//SetAuxCursorFinalPos(gameRef->isoTerrainAstarGrid[11][8]->getPosX(), gameRef->isoTerrainAstarGrid[11][8]->getPosY());
				SetHandHelperPos(gameRef->isoTerrainAstarGrid[11][8]->getPosX(), gameRef->isoTerrainAstarGrid[11][8]->getPosY());
				gameRef->tileToSelect = gameRef->isoTerrainAstarGrid[11][8];
				bBlockContinue = true;
				currBreakpoint = Tutorial_BreakPoint_OpenUnitMenu;
				gameRef->unitManager.Unblock();
				gameRef->unitMenu.Unblock();

				animDelay.Start();

				FadeMessage(true);
				*/
			}
			else if(CurTutorialStep == 17)
			{
				gameRef->unitManager.SetButtonToSelect(gameRef->unitManager.GetLeftButton());
				float tempX = gameRef->unitManager.GetLeftButton()->GetX() + gameRef->unitManager.GetLeftButton()->GetWidth() * 0.5f - 55.0f;
				float tempY = gameRef->unitManager.GetLeftButton()->GetY() + gameRef->unitManager.GetLeftButton()->GetHeight() * 0.5f;
				SetHandHelperPos(tempX, tempY);
				currBreakpoint = Tutorial_BreakPoint_MoveSelknam;
			}
			else if(CurTutorialStep == 18)
			{
				//gameRef->tileToSelect = gameRef->isoTerrainAstarGrid[11][7];
				SetHandHelperPos(gameRef->isoTerrainAstarGrid[11][7]->getPosX(), gameRef->isoTerrainAstarGrid[11][7]->getPosY());
				currBreakpoint = Tutorial_BreakPoint_ConfirmMovement;
			}
			else if(CurTutorialStep == 19)
			{
				fadeTimer.Reset();
				fadeTimer.Start();
				bFadeIn = false;
				//animTimer.Start(animTime);
				t = 0;
				//SetAuxCursorInitPos(gameRef->isoTerrainAstarGrid[13][6]->getPosX(), gameRef->isoTerrainAstarGrid[13][6]->getPosY());
				//SetAuxCursorFinalPos(gameRef->isoTerrainAstarGrid[11][8]->getPosX(), gameRef->isoTerrainAstarGrid[11][8]->getPosY());
				SetHandHelperPos(gameRef->isoTerrainAstarGrid[11][8]->getPosX(), gameRef->isoTerrainAstarGrid[11][8]->getPosY());
				gameRef->tileToSelect = gameRef->isoTerrainAstarGrid[11][8];
				bBlockContinue = true;
				currBreakpoint = Tutorial_BreakPoint_OpenUnitMenu;
				gameRef->unitManager.Unblock();
				gameRef->unitMenu.Unblock();

				animDelay.Start();

				FadeMessage(true);
			}
			else if(CurTutorialStep == 20)
			{
				//animTimer.Start(animTime);
				t = 0;
				//SetAuxCursorInitPos(gameRef->isoTerrainAstarGrid[10][11]->getPosX(), gameRef->isoTerrainAstarGrid[10][11]->getPosY());

				float tempX = gameRef->unitMenu.GetUpButton()->GetX() + gameRef->unitMenu.GetUpButton()->GetWidth() * 0.5f;
				float tempY = gameRef->unitMenu.GetUpButton()->GetY() + gameRef->unitMenu.GetUpButton()->GetHeight() * 0.5f - 50;

				//SetAuxCursorFinalPos(tempX, tempY);
				SetHandHelperPos(tempX, tempY);

				gameRef->unitMenu.AllowOnlyButton(gameRef->unitMenu.GetUpButton());

				currBreakpoint = Tutorial_BreakPoint_SelectArcher;

				animDelay.Start();
			}
			else if(CurTutorialStep == 21)
			{
				FadeMessage(false);

				gameRef->unitMenu.AllowOnlyButton(gameRef->unitMenu.GetUpButton());

				currBreakpoint = Tutorial_BreakPoint_DeployUnit;
			}
			else if(CurTutorialStep == 22)
			{
				bBlockContinue = false;
				bShowIngameGUI = true;
				bShowAlmanacGUI = true;
				gameRef->unitMenu.Block();
				gameRef->unitManager.Block();
				//Utils::PauseGame();
				FadeMessage(true);
			}
			else if(CurTutorialStep == 23)
			{

			}
			else if(CurTutorialStep == 24)
			{
				bBlockContinue = true;
				bClosing = true;
				timeBeforeClose.Start();
			}
		}
		else if(CurTutorial == Tutorial_Stars)
		{
			UpdateStoreTutorial();
		}
	}

	lerpTimer.Update();
	animTimer.Update();
	animDelay.Update();
	auxWaveTimer.Update();
	winkDelay.Update();
	winkTime.Update();
	onClickDelay.Update();
	timeBeforeClose.Update();

	t += (float)CURSOR_SPEED;

	if(bWiseAnimation)
	{
		//IwTrace(NEWTUTORIAL, ("bWiseAnimation = true"));

		if(winkTime.GetIsEnd())
		{
			bWiseAnimation = false;
			winkDelay.Start();
		}
	}
	else
	{
		//IwTrace(NEWTUTORIAL, ("bWiseAnimation = false, %d", winkDelay.GetCurrTime()));

		if(winkDelay.GetIsEnd())
		{
			bWiseAnimation = true;
			winkTime.Start();
		}
	}

	if(bShowLifesGUI)
	{
		currVecLifesGUI = CIwSVec2((int)Utils::Lerp(vecLifesGUI.x, 0, lerpTimer.GetLerpConstant()), (int)Utils::Lerp(vecLifesGUI.y, 0, lerpTimer.GetLerpConstant()));
	}

	if(bShowAlmanacGUI)
	{
		currVecAlmanacGUI = CIwSVec2((int)Utils::Lerp(vecAlmanacGUI.x, 0, lerpTimer.GetLerpConstant()), (int)Utils::Lerp(vecAlmanacGUI.y, 0, lerpTimer.GetLerpConstant()));
	}

	if(bShowIngameGUI)
	{
		currVecIngameMenuGUI = CIwSVec2((int)Utils::Lerp(vecIngameMenuGUI.x, 0, lerpTimer.GetLerpConstant()), (int)Utils::Lerp(vecIngameMenuGUI.y, 0, lerpTimer.GetLerpConstant()));
	}


	if(bShowWaveTimerGUI)
	{
		currVecWaveTimerGUI = CIwSVec2((int)Utils::Lerp(vecWaveTimerGUI.x, 0, lerpTimer.GetLerpConstant()), (int)Utils::Lerp(vecWaveTimerGUI.y, 0, lerpTimer.GetLerpConstant()));
	}

	if(bShowFastWaveGUI)
	{
		currVecFastWaveGUI = CIwSVec2((int)Utils::Lerp(vecFastWaveGUI.x, 0, lerpTimer.GetLerpConstant()), (int)Utils::Lerp(vecFastWaveGUI.y, 0, lerpTimer.GetLerpConstant()));
	}

	if(bShowGoldGUI)
	{
		currVecGoldGUI = CIwSVec2((int)Utils::Lerp(vecGoldGUI.x, 0, lerpTimer.GetLerpConstant()), (int)Utils::Lerp(vecGoldGUI.y, 0, lerpTimer.GetLerpConstant()));
	}

	if(lerpTimer.GetLerpConstant() >= 1)
	{
		bShowLifesGUI = false;
		bShowWaveTimerGUI = false;
		bShowFastWaveGUI = false;
		bShowGoldGUI = false;
		bShowAlmanacGUI = false;
		bShowIngameGUI = false;
	}

	if(bDeployEnemyUnit)
	{
		if(auxWaveTimer.GetIsEnd())
		{
			auxWaveTimer.Stop();
			bDeployEnemyUnit = false;
			gameRef->SpawnEnemyUnit(0, 1, ENEMY_Soldier);
			if(CurTutorialStep != 10)
			{
				NextTutorialFrame();
				//CurTutorialStep = 10;
				//bIncreaseTutorialStepByOne = false;
				bShowFastWaveGUI = true;
			}
		}
	}

	if(t >= 1)
	{
		if(bStartAnimDelay)
		{
			if(animDelay.GetIsEnd() || animDelay.GetIsStop())
			{
				animDelay.Start();
				bStartAnimDelay = false;
			}
		}
		else
		{
			if(animDelay.GetIsEnd())
			{
				t = 0;
				//animTimer.Start(animTime);
				bStartAnimDelay = true;
			}
		}
	}

	if(onClickDelay.GetIsEnd() || onClickDelay.GetIsStop())
	{
		onClickDelay.Start();
		bShowOnClickAnim = !bShowOnClickAnim;
	}

	if(bClosing)
	{
		if(timeBeforeClose.GetIsEnd())
		{
			if(CurTutorial == Tutorial_Map1)
			{
				bClosing = false;
				EndTutorial();
				Utils::UnfreezeCamera();
				gameRef->unitMenu.AllowOnlyButton(NULL);
				gameRef->unitMenu.Unblock();
				gameRef->unitManager.Unblock();
				gameRef->unitManager.SetButtonToSelect(NULL);
				gameRef->tileToSelect = NULL;
				Utils::UnpauseGame();
				bStopWaveTimer = false;
#if !defined DESKTOPPLATFORM
				if(bFlurryOn)
				{
					string tmpValue = "Finished|True";
					s3eFlurryLogEventParams("event_Tutorial", tmpValue.c_str());
				}
#endif
			}
		}
	}
}

void Tutorial::CheckIfFadeOutMessage()
{
	if(CurTutorial == Tutorial_Map1)
	{
		if(CurTutorialStep == 4
		   || CurTutorialStep == 9
		   || CurTutorialStep == 13
		   //|| CurTutorialStep == 17
		   || CurTutorialStep == 20)
		{
			FadeMessage(false);
		}
		else
		{
			alphaTextMessageTimer.Start();
		}
	}
	else if(CurTutorial == Tutorial_Stars)
	{
		if(CurTutorialStep == 2)
		{
			FadeMessage(false);
		}
		else
		{
			alphaTextMessageTimer.Start();
		}
	}
	else if(CurTutorial == Tutorial_SurvivalMode)
	{
		if(CurTutorialStep == 0)
		{
			FadeMessage(false);
		}
		else
		{
			alphaTextMessageTimer.Start();
		}
	}
}


void Tutorial::UpdateStoreTutorial()
{
	if(CurTutorialStep == 0)
	{
		t = 0;
		//animTimer.Start(animTime);
		bBlockContinue = true;

		float tempX = (float)(mapSelectionRef->btnUpgrade->GetX() + 144);
		float tempY = mapSelectionRef->btnUpgrade->GetY() + mapSelectionRef->btnUpgrade->GetHeight() * 0.25f * 0.5f - 20.0f;

		currBreakpoint = Tutorial_BreakPoint_MapSelection;

		//SetAuxCursorInitPos(tempX, tempY - 60);
		//SetAuxCursorFinalPos(tempX, tempY);
		SetHandHelperPos(tempX, tempY);
	}
	else if(CurTutorialStep == 1)
	{
		bPausedStore = true;
		bBlockContinue = false;
	}
	else if(CurTutorialStep == 3)
	{
		bBlockContinue = true;
		bPausedStore = false;
		currBreakpoint = Tutorial_BreakPoint_UpgradeArcher;

		float tempX = storeRef->button_panel[0][0]->GetX() + storeRef->button_panel[0][0]->GetWidth() * 0.5f + 10.0f;
		float tempY = storeRef->button_panel[0][0]->GetY() + storeRef->button_panel[0][0]->GetHeight() * 0.5f - 17.0f;

		//SetAuxCursorInitPos(tempX, tempY - 60);
		//SetAuxCursorFinalPos(tempX, tempY);
		SetHandHelperPos(tempX, tempY);
	}
	else if(CurTutorialStep == 4)
	{
		/*
		if(activeControllerType == Controller_MOUSE)
			currBreakpoint = Tutorial_BreakPoint_UpgradeArcher;
		else
			CurTutorialStep++;
		*/
	}
}

void Tutorial::RenderStoreTutorial()
{
	if(CurTutorialStep == 0)
	{
		DrawMessageCenterRight(loc_tutorial_part14);
		RenderHandHelper();
		//RenderAuxCursor();
	}
	else if(CurTutorialStep == 1)
	{
		background->Render();
		DrawMessage(loc_tutorial_part15);
	}
	else if(CurTutorialStep == 2)
	{
		background->Render();
		DrawMessage(loc_tutorial_part16);
	}
	else if(CurTutorialStep == 3)
	{
		RenderHandHelper();
		//RenderAuxCursor();
	}
	/*
	else if(CurTutorialStep == 4)
	{
		//RenderHandHelper();
		//RenderAuxCursor();
	}
	*/
	else if(CurTutorialStep >= 4)
	{
		EndTutorial();
	}
}

void Tutorial::Render()
{
	if(CurTutorial == Tutorial_None)
	{
		return;
	}

	if(CurTutorial == Tutorial_Map1)
	{
		if(CurTutorialStep == 0)
		{
			DrawMessage(loc_tutorial_part0);
		}
		else if(CurTutorialStep == 1)
		{
			DrawMessage(loc_tutorial_part1);

			if(!Utils::IsCameraMoving())
			{
				Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(arrow->atlasName), 
					                                CIwFVec2((float)(basePosition.x - xWindowOffset), (float)(basePosition.y - yWindowOffset - (int)(maxYCursorOffset * yCursorOffset))), 
													*arrow, 
													255);
			}
		}
		else if(CurTutorialStep == 2)
		{
			DrawMessage(loc_tutorial_part2);

			if(!Utils::IsCameraMoving())
			{
				Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(arrow->atlasName), 
					                                CIwFVec2((float)(basePosition.x - xWindowOffset), (float)(basePosition.y - yWindowOffset - (int)(maxYCursorOffset * yCursorOffset))), 
													*arrow, 
													255);
			}
		}
		else if(CurTutorialStep == 3)
		{
			if(!Utils::IsCameraMoving())
			{
				Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(arrow->atlasName), 
									                CIwFVec2((float)(enemySpotPosition.x - xWindowOffset), (float)(enemySpotPosition.y - yWindowOffset - (int)(maxYCursorOffset * yCursorOffset))), 
													*arrow, 
													255);
			}

			DrawMessage(loc_tutorial_part3);
		}
		else if(CurTutorialStep == 4)
		{
			DrawMessage(loc_tutorial_part4);

			if(!Utils::IsCameraMoving())
			{
				Utils::RenderYellowCircleCursorMap(gameRef->isoTerrainAstarGrid[10][8], blueCursorIndex, alpha);

				RenderHandHelper(true);
				/*
				if(t < 1)//animTimer.GetLerpConstant() < 1)
				{
					auxCursorCurrPosition = Utils::ParabolicInterp(CIwFVec2(auxCursorInitialPosition.x, auxCursorInitialPosition.y), 
						CIwFVec2(auxCursorFinalPosition.x - 5, auxCursorFinalPosition.y - 5), t);

					Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
						CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
						*auxCursor, 
						255);
				}
				else
				{
					if(bShowOnClickAnim)
					{
						Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursorB->atlasName), 
							CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
							*auxCursorB, 
							255);
					}
					else
					{
						Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
							CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
							*auxCursor, 
							255);
					}
				}
				*/
			}
		}
		else if(CurTutorialStep == 5)
		{
			RenderHandHelper(false, HandHelper_Direction_Right);
			//RenderAuxCursor();
		}
		else if(CurTutorialStep == 6)
		{
			RenderHandHelper(false, HandHelper_Direction_Right);
			//RenderAuxCursor();
		}
		else if(CurTutorialStep == 7)
		{
			DrawMessage(loc_tutorial_part5);
		}
		else if(CurTutorialStep == 8)
		{
			DrawMessage(loc_tutorial_part6);
		}
		else if(CurTutorialStep == 9)
		{
			if(!bShowFastWaveGUI)
				RenderHandHelper();

			DrawMessage(loc_tutorial_part7);
		}
		else if(CurTutorialStep == 10)
		{

		}
		else if(CurTutorialStep == 11)
		{
			DrawMessage(loc_tutorial_part8);
		}
		else if(CurTutorialStep == 12)
		{
			DrawMessage(loc_tutorial_part9);
		}
		else if(CurTutorialStep == 13)
		{
			DrawMessage(loc_tutorial_part10);

			Utils::RenderYellowCircleCursorMap(gameRef->isoTerrainAstarGrid[STAGE1_MAGE_POSITION_X][STAGE1_MAGE_POSITION_Y], blueCursorIndex, alpha);
			
			RenderHandHelper(true);
			/*
			if(t < 1)
			{
				auxCursorCurrPosition = Utils::ParabolicInterp(CIwFVec2(auxCursorInitialPosition.x, auxCursorInitialPosition.y), 
					CIwFVec2(auxCursorFinalPosition.x, auxCursorFinalPosition.y), t);

				Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
					CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
					*auxCursor, 
					255);
			}
			else
			{
				if(bShowOnClickAnim)
				{
					Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursorB->atlasName), 
						CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
						*auxCursorB, 
						255);
				}
				else
				{
					Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
						CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
						*auxCursor, 
						255);
				}
			}
			*/
		}
		else if(CurTutorialStep == 14)
		{
			RenderHandHelper(false, HandHelper_Direction_Up);
			//RenderAuxCursor();
		}
		else if(CurTutorialStep == 15)
		{
			RenderHandHelper(false, HandHelper_Direction_Up);
			//RenderAuxCursor();
		}
		else if(CurTutorialStep == 16)
		{
			DrawMessage(loc_tutorial_part18);

			//Utils::RenderYellowCircleCursorMap(gameRef->isoTerrainAstarGrid[14][7], blueCursorIndex, alpha);
			RenderHandHelper(true);
		}
		else if(CurTutorialStep == 17)
		{
			//DrawMessage(loc_tutorial_part14);
			RenderHandHelper(false, HandHelper_Direction_Right);
		}
		else if(CurTutorialStep == 18)
		{
			RenderHandHelper(true);
			//DrawMessage(loc_tutorial_part11);
		}
		else if(CurTutorialStep == 19)
		{
			DrawMessage(loc_tutorial_part11);

			Utils::RenderYellowCircleCursorMap(gameRef->isoTerrainAstarGrid[11][8], blueCursorIndex, alpha);
			RenderHandHelper(true);
			/*
			if(t < 1)
			{
				auxCursorCurrPosition = Utils::ParabolicInterp(CIwFVec2(auxCursorInitialPosition.x, auxCursorInitialPosition.y), 
					                                           CIwFVec2(auxCursorFinalPosition.x, auxCursorFinalPosition.y), t);

				Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
					CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
					*auxCursor, 
					255);
			}
			else
			{
				if(bShowOnClickAnim)
				{
					Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursorB->atlasName), 
						CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
						*auxCursorB, 
						255);
				}
				else
				{
					Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
						CIwFVec2(auxCursorCurrPosition.x - xWindowOffset, auxCursorCurrPosition.y - yWindowOffset), 
						*auxCursor, 
						255);
				}
			}
			*/
		}
		else if(CurTutorialStep == 20)
		{
			RenderHandHelper(false);
			//RenderAuxCursor();
		}
		else if(CurTutorialStep == 21)
		{
			RenderHandHelper(false);
			//RenderAuxCursor();
		}
		else if(CurTutorialStep == 22)
		{
			DrawMessage(loc_tutorial_part12);
		}
		else if(CurTutorialStep == 23)
		{
			DrawMessage(loc_tutorial_part13);
		}
		else if(CurTutorialStep == 24)
		{

		}
	}
	else if(CurTutorial == Tutorial_Stars)
	{
		RenderStoreTutorial();
	}
	else if(CurTutorial == Tutorial_SurvivalMode)
	{
		if(CurTutorialStep == 0)
		{
			 DrawMessage(loc_tutorial_part17);
		}
		else if(CurTutorialStep == 1)
		{
			EndTutorial();
		}
	}

	if(bShowSkipTutButton) 
		btnSkipTutorial->Render();

	IwGxLightingOn();

	if(CanContinue())
	{
		int tmpPosX = pannelPosX;
		int tmpPosY = pannelPosY;

		if(bRenderMessageCenterRight)
		{
			tmpPosX = centerRightX;
			tmpPosY = centerRightY;
		}


		CIwColour textColor;
		textColor.Set(255, 255, 255, alphaBackground);

#if defined DESKTOPPLATFORM
		Utils::RenderText(loc_dialog_tapToContinue_alt, CIwRect(tmpPosX, tmpPosY + heightTutoPannel - 60, widthTutoPannel, 60), font_SmartKid18, textColor);
#endif
#if !defined DESKTOPPLATFORM
		Utils::RenderText(loc_dialog_tapToContinue, CIwRect(tmpPosX, tmpPosY + heightTutoPannel - 60, widthTutoPannel, 60), font_SmartKid18, textColor);
#endif

	}
	IwGxLightingOff();
}

void Tutorial::InitTutorial(TutorialName _TutorialName)
{
	if(bSkipped)
		return;

	bBackgroundFadeIn = true;
	bIncreaseTutorialStepByOne = false;

	alphaSelknamTimer.Start();
	alphaBackgroundTimer.Start();
	alphaTextMessageTimer.Finish();
	fadeTimer.Start();

	currBreakpoint = Tutorial_BreakPoint_None;

	Utils::PauseGame();

	CurTutorial = _TutorialName;
	CurTutorialStep = 0;
	bBlockContinue = false;
	InitDeviceHandler();

	if(CurTutorial == Tutorial_SurvivalMode)
	{
		bPauseMapSelection = true;
	}

	currTut = -1;

	bWiseAnimation = true;
	winkTime.Start();

}

void Tutorial::DrawMessage(const char* message)
{
	bRenderMessageCenterRight = false;

	Utils::RenderPannel(PannelType_H, CIwRect(pannelPosX, pannelPosY, renderPannelNumWidth, renderPannelNumHeight), alphaBackground);
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(selknamTutorialRD->atlasName), *selknamTutorialRD, CIwSVec2(pannelPosX + 15, pannelPosY + 15), 255, 255, 255, alphaSelknam);

	if(!winkTime.GetIsEnd())
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(selknamTutorial2RD->atlasName), *selknamTutorial2RD, CIwSVec2(pannelPosX + 15, pannelPosY + 15), 255, 255, 255, alphaSelknam);
	}

	RenderTextMessage(message, pannelPosX, pannelPosY);
}

void Tutorial::DrawMessageCenterRight(const char* message)
{
	bRenderMessageCenterRight = true;

	Utils::RenderPannel(PannelType_H, CIwRect(centerRightX, centerRightY, renderPannelNumWidth, renderPannelNumHeight), alphaBackground);
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(selknamTutorialRD->atlasName), *selknamTutorialRD, CIwSVec2(centerRightX + 15, centerRightY + 15), 255, 255, 255, alphaSelknam);

	if(!winkTime.GetIsEnd())
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(selknamTutorial2RD->atlasName), *selknamTutorial2RD, CIwSVec2(centerRightX + 15, centerRightY + 15), 255, 255, 255, alphaSelknam);
	}
	
	RenderTextMessage(message, centerRightX, centerRightY);
}

void Tutorial::RenderTextMessage(const char* msg, int xOff, int yOff)
{
	int yOffset = 75;

	if(bBlockContinue) yOffset = 45;

	IwGxLightingOn();
	CIwColour textColor;

	if(alphaTextMessage < 255)
		textColor.Set(255, 255, 255, alphaTextMessage);
	else
		textColor.Set(255, 255, 255, alphaBackground);

	Utils::RenderText(msg, CIwRect(xOff + 143, yOff + 15, widthTutoPannel - 163, heightTutoPannel - yOffset), font_SmartKid20, textColor, IW_GX_FONT_ALIGN_LEFT);
	IwGxLightingOff();
}

void Tutorial::UpdateClickEvents(s3ePointerEvent *event)
{
	if(event->m_Pressed && !btnSkipTutorial->MouseOn(event->m_x, event->m_y))
	{
		NextTutorialFrame();
	}

	if(bShowSkipTutButton)
		btnSkipTutorial->HandleClick(event);
}

void Tutorial::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bShowSkipTutButton)
		btnSkipTutorial->HandleMotion(event);
}

void Tutorial::InitDeviceHandler()
{
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTutorial, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTutorial, NULL);
}

void Tutorial::StopDeviceHandler()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTutorial);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTutorial);
}

void Tutorial::EndTutorial()
{
	bBlockGUI = false;
	CurTutorial = Tutorial_None;
	CurTutorialStep = -1;
	bPauseGame = false;
	bPauseMapSelection = false;
	bFirstOpen = true;
	bPausedStore = false;
	StopDeviceHandler();
	UnhideGUIElements();
	bShowSkipTutButton = false;
	//bBlockContinue = false;
}

void Tutorial::NextTutorialFrame()
{
	if(CanContinue())
	{
		CheckIfFadeOutMessage();
		bIncreaseTutorialStepByOne = true;
	}
}

void Tutorial::FinishTutorial()
{
	if(CurTutorial == Tutorial_UnitMenu)
	{
		CurTutorialStep = 1;
	}
	else if(CurTutorial == Tutorial_UnlockStore)
	{
		bPausedStore = false;
	}
	else if(CurTutorial == 21)
	{

	}
}

void Tutorial::SetBasePosition(CIwSVec2 newPos) 
{
	basePosition = newPos;
	CIwSVec2 arrowDimension = TextureHelper::GetDimension(*arrow);
	basePosition.y -= arrowDimension.y + 80;
	basePosition.x -= (int)(arrowDimension.x * 0.5f);
}

void Tutorial::SetEnemySpotPosition(CIwSVec2 newPos)
{
	enemySpotPosition = newPos;
	CIwSVec2 arrowDimension = TextureHelper::GetDimension(*arrow);
	enemySpotPosition.y -= arrowDimension.y;
	enemySpotPosition.x -= (int)(arrowDimension.x * 0.5f);
}

bool Tutorial::CanContinue()
{
	return !bShowLifesGUI 
		   && !Utils::IsCameraMoving()
		   && !bBlockContinue
		   && alphaBackgroundTimer.GetIsEnd()
		   && alphaSelknamTimer.GetIsEnd()
		   && alphaTextMessageTimer.GetIsEnd();
}


void Tutorial::SetGameRef(CGame* game)
{
	gameRef = game;
}

void Tutorial::SetAuxCursorInitPos(float xPos, float yPos)
{
	auxCursorInitialPosition = CIwFVec2(xPos, yPos);
	auxCursorCurrPosition = auxCursorInitialPosition;
}

void Tutorial::SetAuxCursorFinalPos(float xPos, float yPos)
{
	auxCursorFinalPosition = CIwFVec2(xPos, yPos);
}

void Tutorial::SetHandHelperPos(float xPos, float yPos)
{
	handHelperPosition = CIwFVec2(xPos, yPos);
}

void Tutorial::ExecuteBreakpoint()
{
	if(currBreakpoint != Tutorial_BreakPoint_None)// && currBreakpoint != Tutorial_BreakPoint_)
	{
		CheckIfFadeOutMessage();
		bIncreaseTutorialStepByOne = true;

		currBreakpoint = Tutorial_BreakPoint_None;
	}
}

void Tutorial::RenderHandHelper(bool bWindowOff, HandHelper_Direction handDir)
{
	int xOff = xWindowOffset;
	int yOff = yWindowOffset;

	//Window offset must be set as true if map-relative positions are specified.
	if(!bWindowOff)
	{
		xOff = yOff = 0;
	}

	//Render ONLY if camera is not moving and message box is fully displayed or not displayed at all.
	if(!Utils::IsCameraMoving())
	{
		if(handDir == HandHelper_Direction_Down)
		{
			Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
				CIwFVec2((float)(handHelperPosition.x - 16 - xOff), (float)(handHelperPosition.y - yOff - 26 - (int)(maxYCursorOffset * yCursorOffset))), 
				*auxCursor, 255);
		}
		else if(handDir == HandHelper_Direction_Right)
		{
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(auxCursorB->atlasName), 
				CIwSVec2((int)(handHelperPosition.x - 22 - xOff - (int)(maxYCursorOffset * yCursorOffset)), (int)(handHelperPosition.y - yOff - 18)), 
				*auxCursorB, ROTATE_270);
		}
		else if(handDir == HandHelper_Direction_Up)
		{
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
				CIwSVec2((int)(handHelperPosition.x - 22 - xOff), (int)(handHelperPosition.y - yOff - 18 + (int)(maxYCursorOffset * yCursorOffset))), 
				*auxCursor, ROTATE_180);
		}
	}
}

void Tutorial::RenderAuxCursor()
{
	if(t < 1)
	{
		auxCursorCurrPosition = Utils::ParabolicInterp(CIwFVec2(auxCursorInitialPosition.x - xWindowOffset, auxCursorInitialPosition.y - yWindowOffset), 
			                                           CIwFVec2(auxCursorFinalPosition.x, auxCursorFinalPosition.y), t);


		Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
			CIwFVec2(auxCursorCurrPosition.x, auxCursorCurrPosition.y), 
			*auxCursor, 
			255);
	}
	else
	{
		if(!bShowOnClickAnim)
		{
			Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursor->atlasName), 
				CIwFVec2(auxCursorCurrPosition.x, auxCursorCurrPosition.y), 
				*auxCursor, 
				255);
		}
		else
		{
			Utils::RenderSingleTextureWithAlpha(TextureHelper::GetCIwTexture(auxCursorB->atlasName), 
				CIwFVec2(auxCursorCurrPosition.x, auxCursorCurrPosition.y), 
				*auxCursorB, 
				255);
		}
	}
}

void Tutorial::SetMapSelectionRef(MapSelection* mapSelection)
{
	mapSelectionRef = mapSelection;
}

void Tutorial::SetStoreRef(Store* store)
{
	storeRef = store;
}

void Tutorial::SkipTutorial()
{
	if(CurTutorial == Tutorial_Map1)
	{
		CurTutorialStep = 24;
	}
}