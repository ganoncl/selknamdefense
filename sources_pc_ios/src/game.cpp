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

#include "s3e.h"
#include "s3eOSReadString.h"
#include "IwGx.h"
#include "stdlib.h"

//app
#include "game.h"
#include "drawutils.h"
#include "common.h"
#include "utils.h"
#include "tutorial.h"
#include "captainboss.h"
#include "survivalmodeconfig.h"
#if !defined DESKTOPPLATFORM
#include "s3eFlurry.h"
#endif
#include <algorithm>

#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif

bool order_by_z(IsoPawnRenderContainer A, IsoPawnRenderContainer B)
{
	return A._z < B._z;
}

CGame* CGameRef = NULL;

//
// HandleSingleTouchMotionCB - The system will call this callback when the user moves their finger on the screen
//
void HandleSingleTouchMotionCB(s3ePointerMotionEvent* event)
{
	if(bFreezeCamera) return;
	
	CGameRef->currentMousePosition = CIwSVec2(event->m_x, event->m_y);

	if(CGameRef == NULL || 
	   tutorial->IsActive() || 
	   CGameRef->unitManager.GetIsActive() || 
	   CGameRef->unitMenu.GetIsActive() ||
	   (dialogController != NULL && dialogController->IsActive())
	   || bBlockCamera) return;

	if(CGameRef->nextWaveButton_x1->MouseOn(event->m_x, event->m_y)
	   || CGameRef->nextWaveButton_x2->MouseOn(event->m_x, event->m_y)
	   || CGameRef->nextWaveButton_x3->MouseOn(event->m_x, event->m_y)
	   || CGameRef->pauseMenuButton->MouseOn(event->m_x, event->m_y) 
	   || CGameRef->almanacButton->MouseOn(event->m_x, event->m_y))
	{
		return;
	}

	 if(inGameState != GSG_Attack && inGameState != GSG_Defending) return;

	if(bPause || !CGameRef->leftClickPressed) return;

	if(LastTouchX == -1 || LastTouchY == -1)
	{
		LastTouchX = event->m_x;
		LastTouchY = event->m_y;
	}

	if(LastTouchX != event->m_x || LastTouchY != event->m_y)
	{
		int diffX = event->m_x - LastTouchX;
		int diffY = event->m_y - LastTouchY;

		CGameRef->xTouchMovement += diffX;
		CGameRef->yTouchMovement += diffY;

		if(abs(CGameRef->xTouchMovement) >= TouchMotionTolerance || abs(CGameRef->yTouchMovement) >= TouchMotionTolerance || CGameRef->bMovingCamera)
		{
			if(!CGameRef->bMovingCamera)
			{
				CGameRef->bMovingCamera = true;
				CGameRef->lastTouchMotionCBTime = s3eTimerGetMs();
				CGameRef->lastMousePositionRegistered = CGameRef->currentMousePosition;

				if(CGameRef->unitMenu.bUnitMenuRecentlyClosedByTap)
					CGameRef->unitMenu.bUnitMenuRecentlyClosedByTap = false;
				if(CGameRef->unitManager.bUnitManagerRecentlyClosedByTap)
					CGameRef->unitManager.bUnitManagerRecentlyClosedByTap = false;
			}

#if !defined DESKTOPPLATFORM
			xWindowOffset -= diffX;
			yWindowOffset -= diffY;
#endif
			CGameRef->CheckWindowOffset();
		}

		LastTouchX = event->m_x;
		LastTouchY = event->m_y;
	}

	//CGameRef->currentMousePosition = CIwSVec2(LastTouchX, LastTouchY);
}

//Mouse Motion
void HandleMotion_CGame(s3ePointerMotionEvent* event) 
{
	if(CGameRef == NULL)
		return;

	CGameRef->UpdateMotionEvents(event);
}

//Mouse handle
void HandleClick_CGame(s3ePointerEvent *event) 
{
	if(CGameRef == NULL)
		return;

	CGameRef->currentMousePosition = CIwSVec2(event->m_x, event->m_y);

	CGameRef->UpdateClickEvents(event);

#if defined DESKTOPPLATFORM
	if (event->m_Button != S3E_POINTER_BUTTON_LEFTMOUSE)
		return;
#endif

	LastTouchX = event->m_x;
	LastTouchY = event->m_y;
	CGameRef->xTouchMovement = CGameRef->yTouchMovement = 0;

	if(event->m_Pressed && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
	{
		CGameRef->leftClickPressed = true;
	}
	else
	{
		CGameRef->leftClickPressed = false;
		CGameRef->CheckWindowOffset();
		if(CGameRef->bMovingCamera)
		{
			CGameRef->bMovingCamera = false;
			CGameRef->bSlowStopEffect = true;
		}
	}
}

void CGame_PauseGame_Action()
{
	if(bBlockGUI) return;

	if(!CGameRef->unitManager.GetIsActive() && !bPause)
	{
		bPause = true;
		inGamePausedState = GSGP_MainMenuIG;
	}
}

void CGame_AlmanacGame_Action()
{
	if(bBlockGUI) return;

	if(!CGameRef->unitManager.GetIsActive() && !bPause)
	{
		bPause = true;
		inGamePausedState = GSGP_AlmanacIG;
	}
}

void CGame_NextWave_Action()
{
	CGameRef->NextWaveBtnAction();
}

#pragma region UnitMenu_Button_Functions
void UnitMenu_ArcherBaseButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|ArcherBase";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Archer_Base);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_ARCHER);
}

void UnitMenu_ArcherPoisonerButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|ArcherPoisoner";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Archer_Poisoner);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_ARCHER);
}

void UnitMenu_ArcherRetarderButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|ArcherRetarder";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Archer_Retarder);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_ARCHER);
}

void UnitMenu_BowlerStunerButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|BowlerStunner";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Bowler_Stuner);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_BOWLER);
}

void UnitMenu_BowlerRooterButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|BowlerRooter";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Bowler_Rooter);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_BOWLER);
}

void UnitMenu_BowlerConfuserButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|BowlerConfuser";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Bowler_Confuser);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_BOWLER);
}

void UnitMenu_MageMightButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|MageMight";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Shaman_Might);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_MAGE);
}

void UnitMenu_MageHasteButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|MageHaste";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Shaman_Haste);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_MAGE);
}

void UnitMenu_MageHealerButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|MageHealer";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Shaman_Healer);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_MAGE);
}

void UnitMenu_WarriorTankButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|WarriorTank";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Warrior_Tank);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_TANK);
}

void UnitMenu_WarriorBerserkerButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|WarriorBerserk";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Warrior_Berserk);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_TANK);
}

void UnitMenu_WarriorBaseButton_Action()
{
#if !defined DESKTOPPLATFORM
	string tmpValue = "Unit|WarriorBase";
	s3eFlurryLogEventParams("event_SpawnUnit", tmpValue.c_str());
#endif
	CGameRef->SpawnSelknam(SELKNAM_Warrior_Base);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_TANK);
}

void UnitMenu_OKButton_Action()
{
	CGameRef->unitMenu.Close();
}
#pragma endregion UnitMenu_Button_Functions

#if defined DESKTOPPLATFORM
void CGame_Xbox360_StopMovePointWest()
{
	CGameRef->mapPointer.StopUpLooping();
}

void CGame_Xbox360_MovePointerWest()
{
	CGameRef->mapPointer.GoUpLooping();
	//CGameRef->mapPointer.SetGrid(CGameRef->mapPointer.findAvailableGridForPointer(-1, 0));
}

void CGame_Xbox360_StopMovePointEast()
{
	CGameRef->mapPointer.StopDownLooping();
}

void CGame_Xbox360_MovePointerEast()
{
	CGameRef->mapPointer.GoDownLooping();
	//CGameRef->mapPointer.SetGrid(CGameRef->mapPointer.findAvailableGridForPointer(1, 0));
}

void CGame_Xbox360_StopMovePointNorth()
{
	CGameRef->mapPointer.StopLeftLooping();
}

void CGame_Xbox360_MovePointerNorth()
{
	CGameRef->mapPointer.GoLeftLooping();
	//CGameRef->mapPointer.SetGrid(CGameRef->mapPointer.findAvailableGridForPointer(0, 1));
}

void CGame_Xbox360_StopMovePointSouth()
{
	CGameRef->mapPointer.StopRightLooping();
}

void CGame_Xbox360_MovePointerSouth()
{
	CGameRef->mapPointer.GoRightLooping();

	//CGameRef->mapPointer.SetGrid(CGameRef->mapPointer.findAvailableGridForPointer(0, -1));
}

void CGame_Xbox360_Action()
{
	CGameRef->ResolveActionEvent();
}

void CGame_Xbox360_IncreaseWaveSpeed()
{
	CGameRef->NextWaveSpeed();
}

void CGame_Xbox360_DecreaseWaveSpeed()
{
	CGameRef->PrevWaveSpeed();
}
#endif

CGame::CGame()
{
	lastCameraTerrainLerp = NULL;
	bMoveCameraToTile = false;

	bBreakEnemySpawn = false;

	bIsActiveObj = false;
	bUnfreezeCameraAfterInitObject = false;
	bRestoreSettingsWhenInitObjectIsCalled = true;

	survivalModeConfig->SetCGameObject(this);

	loadingfade = new FadeScreen();
	timeToFadeOut.SetTimeLimit(TIME_BLACKSCREEN);

	//textureHelper.RetrieveAllTextures();
	parserHelper.RetrievePawnsAttributesFromFile();
	parserHelper.RetrievePawnsAnimsFromFile();	
	CleanMapProperties();
	CleanTerrainObjects();
	CleanPawns();
	AStarPropagateHelper();
	IsoPawnRenderInit();

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		isoEnemyArr[i].setPawnId(i);
	}
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoSelknamArr[i].setPawnId(i);
	}

	//Hud Pos

	int16 heightOffset = 0;

	if(currentGameHeight <= 600)
		heightOffset = -15;

	label_timeWave_posX = 40;
	label_timeWave_posY = -16 + heightOffset;

	label_pause_posX = 260;
	label_pause_posY = -8 + heightOffset;

	label_almanac_posX = 375;
	label_almanac_posY = -8 + heightOffset;

	label_heartGold_posX = currentGameWidth - 288;
	label_heartGold_posY = 0 + heightOffset;

	arrowProjectileName = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("fx_proyectile_arrow"));
	rockProjectileName  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("fx_proyectile_stone"));
	cannonProjectileName  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("ship_bullet"));
	rootRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fx_root"));

	expData = TextureHelper::GetTextureNamed("fx_explotion");	        explosionName   = TextureHelper::GetAtlasName(expData);
	healData = TextureHelper::GetTextureNamed("fx_heal");	            healName        =  TextureHelper::GetAtlasName(healData);
	poisonData = TextureHelper::GetTextureNamed("fx_poison");           poisonName      = TextureHelper::GetAtlasName(poisonData);
	stunData = TextureHelper::GetTextureNamed("fx_stun");               stunName        = TextureHelper::GetAtlasName(stunData);
	disorientedData = TextureHelper::GetTextureNamed("fx_disoriented"); disorientedName = TextureHelper::GetAtlasName(disorientedData);
	coinsData = TextureHelper::GetTextureNamed("fx_coins");             coinsName       = TextureHelper::GetAtlasName(coinsData);

	enemySpotData = TextureHelper::GetTextureNamed("graphic_enter_enemy_big");
	enemySpotAtlasName = TextureHelper::GetAtlasName(enemySpotData);

	//InGame Windows
	mainmenuig  = new MainMenuIG();
	endingwin   = new EndingWin();
	endingloose = new EndingLoose();
	dynamicCredits = new DynamicCredits();

	CGameRef = this;
	dynamicCredits->SetGameRef(this);
	
	//General
	pFont = font_komikadisplaybold25;
	graphic_defense_stage = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_defense_stage"));
	CIwSVec2 defenseStateDimension = TextureHelper::GetDimension(graphic_defense_stage);
	ingameMessagePos = CIwSVec2((int)((currentGameWidth - defenseStateDimension.x) * 0.5f), 
				                (int)((currentGameHeight - defenseStateDimension.y) * 0.5f));

	defense_stage_back = new Background(0,0,0, 170);

	TextureData nextWave_x1 = TextureHelper::GetTextureNamed("btn_nextwave_x1");
	nextWaveData_x1.numStates = 1;
	nextWaveData_x1.state[0].RollOut  = TextureHelper::GetRenderData(nextWave_x1, 1, 4, 0);
	nextWaveData_x1.state[0].RollOn   = TextureHelper::GetRenderData(nextWave_x1, 1, 4, 1);
	nextWaveData_x1.state[0].Pressed  = TextureHelper::GetRenderData(nextWave_x1, 1, 4, 2);
	nextWaveData_x1.state[0].Inactive = TextureHelper::GetRenderData(nextWave_x1, 1, 4, 3);

	nextWaveButton_x1 = new Button(TextureHelper::GetCIwTexture(nextWaveData_x1.state[0].RollOut.atlasName), &nextWaveData_x1, 40, currentGameHeight - 110);
	nextWaveButton_x1->OnClick(&CGame_NextWave_Action);

	TextureData nextWave_x2 = TextureHelper::GetTextureNamed("btn_nextwave_x2");
	nextWaveData_x2.numStates = 1;
	nextWaveData_x2.state[0].RollOut  = TextureHelper::GetRenderData(nextWave_x2, 1, 4, 0);
	nextWaveData_x2.state[0].RollOn   = TextureHelper::GetRenderData(nextWave_x2, 1, 4, 1);
	nextWaveData_x2.state[0].Pressed  = TextureHelper::GetRenderData(nextWave_x2, 1, 4, 2);
	nextWaveData_x2.state[0].Inactive = TextureHelper::GetRenderData(nextWave_x2, 1, 4, 3);

	nextWaveButton_x2 = new Button(TextureHelper::GetCIwTexture(nextWaveData_x2.state[0].RollOut.atlasName), &nextWaveData_x2, 40, currentGameHeight - 110);
	nextWaveButton_x2->OnClick(&CGame_NextWave_Action);

	TextureData nextWave_x3 = TextureHelper::GetTextureNamed("btn_nextwave_x3");
	nextWaveData_x3.numStates = 1;
	nextWaveData_x3.state[0].RollOut  = TextureHelper::GetRenderData(nextWave_x3, 1, 4, 0);
	nextWaveData_x3.state[0].RollOn   = TextureHelper::GetRenderData(nextWave_x3, 1, 4, 1);
	nextWaveData_x3.state[0].Pressed  = TextureHelper::GetRenderData(nextWave_x3, 1, 4, 2);
	nextWaveData_x3.state[0].Inactive = TextureHelper::GetRenderData(nextWave_x3, 1, 4, 3);

	nextWaveButton_x3 = new Button(TextureHelper::GetCIwTexture(nextWaveData_x3.state[0].RollOut.atlasName), &nextWaveData_x3, 40, currentGameHeight - 110);
	nextWaveButton_x3->OnClick(&CGame_NextWave_Action);

	TextureData pauseMenu = TextureHelper::GetTextureNamed("btn_pause");
	pauseMenuData.numStates = 1;
	pauseMenuData.state[0].RollOut  = TextureHelper::GetRenderData(pauseMenu, 2, 1, 0);
	pauseMenuData.state[0].RollOn   = TextureHelper::GetRenderData(pauseMenu, 2, 1, 1);
	pauseMenuData.state[0].Pressed  = TextureHelper::GetRenderData(pauseMenu, 2, 1, 1);
	pauseMenuData.state[0].Inactive = TextureHelper::GetRenderData(pauseMenu, 2, 1, 0);

	TextureData almanacMenu = TextureHelper::GetTextureNamed("btn_almanac");
	almanacMenuData.numStates = 1;
	almanacMenuData.state[0].RollOut  = TextureHelper::GetRenderData(almanacMenu, 2, 1, 0);
	almanacMenuData.state[0].RollOn   = TextureHelper::GetRenderData(almanacMenu, 2, 1, 1);
	almanacMenuData.state[0].Pressed  = TextureHelper::GetRenderData(almanacMenu, 2, 1, 1);
	almanacMenuData.state[0].Inactive = TextureHelper::GetRenderData(almanacMenu, 2, 1, 0);

	pauseMenuButton = new Button(TextureHelper::GetCIwTexture(pauseMenuData.state[0].Inactive.atlasName), &pauseMenuData, label_pause_posX, label_pause_posY);
	pauseMenuButton->OnClick(&CGame_PauseGame_Action);

	almanacButton = new Button(TextureHelper::GetCIwTexture(almanacMenuData.state[0].Inactive.atlasName), &almanacMenuData, label_almanac_posX, label_almanac_posY);
	almanacButton->OnClick(&CGame_AlmanacGame_Action);

	CIwSVec2 pauseMenuButtonDimension = TextureHelper::GetDimension(pauseMenuData.state[0].Inactive);

	//GSG_Attack
	bRenderAttackText = false;

	//Timer
	TimeText = loc_ui_time;

	//Wave
	Wave_Text = loc_ui_wave;

	bPause = false;

	//Unit Menu
	int unitMenuPosX = 0;
	int unitMenuPosY = 0;

	unitMenuPosX = Utils::GetFixedScaledInt(480);
	unitMenuPosY = 300;

	if(currentGameHeight <= 600)
		unitMenuPosY = 250;

	unitMenu.SetPos(unitMenuPosX, unitMenuPosY);
	unitMenu.SetArcherBaseButtonFunction(&UnitMenu_ArcherBaseButton_Action);
	unitMenu.SetArcherPoisonerButtonFunction(&UnitMenu_ArcherPoisonerButton_Action);
	unitMenu.SetArcherRetarderButtonFunction(&UnitMenu_ArcherRetarderButton_Action);
	unitMenu.SetWarriorBaseButtonFunction(&UnitMenu_WarriorBaseButton_Action);
	unitMenu.SetWarriorBerserkButtonFunction(&UnitMenu_WarriorBerserkerButton_Action);
	unitMenu.SetWarriorTankButtonFunction(&UnitMenu_WarriorTankButton_Action);
	unitMenu.SetShamanHasteButtonFunction(&UnitMenu_MageHasteButton_Action);
	unitMenu.SetShamanHealerButtonFunction(&UnitMenu_MageHealerButton_Action);
	unitMenu.SetShamanMightButtonFunction(&UnitMenu_MageMightButton_Action);
	unitMenu.SetBowlerConfuserButtonFunction(&UnitMenu_BowlerConfuserButton_Action);
	unitMenu.SetBowlerRooterButtonFunction(&UnitMenu_BowlerRooterButton_Action);
	unitMenu.SetBowlerStunerButtonFunction(&UnitMenu_BowlerStunerButton_Action);
	unitMenu.SetOKButtonFunction(&UnitMenu_OKButton_Action);
	unitMenu.pointerRef = &mapPointer;
	unitMenu.gameCoinsRef = &mp_coins;

	unitManager.SetPos(unitMenuPosX, unitMenuPosY);

	damageFadeScreen = new FadeScreen(238,0,0);
	unitManager.SetWorldMapPointer(&mapPointer);	

	label_infocard = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_infocard"));

	label_hud_lives = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_hud_lives"));
	label_hud_gold  = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_hud_gold"));

	buffBarTextureData = TextureHelper::GetTextureNamed("fx_buffbar");
	buffBarAtlasName   = TextureHelper::GetAtlasName(buffBarTextureData);

	background = new Background(3);
	bIsKeyPressed = false;
	bPrintOutput = false;

	bChangeTheme = true;

	//Touch
	xTouchMovement = 0;
	yTouchMovement = 0;
	bMovingCamera = false;

	//Debug
	leftClickPressed = false;

	rangeTileMapData = TextureHelper::GetTextureNamed("graphic_range_decal_big");
	rangeTileMapAtlasName = TextureHelper::GetAtlasName(rangeTileMapData);

	unavailableTileMapData = TextureHelper::GetTextureNamed("graphic_unavailable_decal_big");
	unavailableTileMapAtlasName = TextureHelper::GetAtlasName(unavailableTileMapData);

	unitManager.gameRef = this;
	blinkMapTileTime = 500;

	bSlowStopEffect = false;

	//mapPointer will be active only when user click on a tile or the unitmanager/unitmenu is open.
	mapPointer.HideCursor();

	//Survival Mode
	survival_enemyspot[0].SetTimeLimit(2500);
	survival_enemyspot[1].SetTimeLimit(2500);
	survival_enemyspot[2].SetTimeLimit(2500);
	survival_enemyspot[3].SetTimeLimit(2500);

	selknamvs_enemyspot[0].SetTimeLimit(3000);
	selknamvs_enemyspot[1].SetTimeLimit(3000);
	selknamvs_enemyspot[2].SetTimeLimit(3000);
	selknamvs_enemyspot[3].SetTimeLimit(3000);
	selknamvs_enemyspot[4].SetTimeLimit(3000);

	//Enemy Spot Decal
	enemySpotBlinkTime.SetTimeLimit(500);
	enemySpotFadeIn = true;

	captainBoss = new CaptainBoss();
	captainBoss->SetGameRef(this);

	mapPointer.unitMenuRef = &unitMenu;
	mapPointer.unitManagerRef = &unitManager;

	bossBarLifeWidth = 517;
	bossBarLifeHeight = 40;
	bossBarLifeX = ((currentGameWidth * 0.5f) - (bossBarLifeWidth * 0.5f)) + 10;
#if defined DESKTOPPLATFORM
	bossBarLifeX = ((currentGameWidth * 0.5f) - (bossBarLifeWidth * 0.5f));
#endif
	bossBarLifeY = currentGameHeight - (float)Utils::GetScaledInt(100);
	bossBarRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("bar_boss"));

	waveTimerSpeed = WTIMER_NORMAL;
	currentGameSpeedValue = normalGameSpeed;
	bOnTripleVelocAchievement = false;
	bDeathOnlyByPoison = 0;
	bUnitBuffedAchievementChecked = false;

	//nextWaveData_x1
	//pauseMenuButton
	//almanacButton
	//label_infocard

	CIwSVec2 hudLivesDimension = TextureHelper::GetDimension(label_hud_lives);

	dynamicGUIWrapper[0].w = Utils::GetScreenWidth();
	dynamicGUIWrapper[0].h = Utils::GetScreenHeight() - Utils::MaxInt(pauseMenuButtonDimension.y, hudLivesDimension.y) - (Utils::GetScreenHeight() - nextWaveButton_x1->GetY());
	dynamicGUIWrapper[0].x = 0;
	dynamicGUIWrapper[0].y = pauseMenuButton->GetY() + Utils::MaxInt(pauseMenuButtonDimension.y, hudLivesDimension.y);

	tileToSelect = NULL;

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Up, &CGame_Xbox360_MovePointerWest);
	xbox360Controller.SetCallbackOnRelease(Xbox360_Up, &CGame_Xbox360_StopMovePointWest);
	xbox360Controller.SetCallback(Xbox360_Down, &CGame_Xbox360_MovePointerEast);
	xbox360Controller.SetCallbackOnRelease(Xbox360_Down, &CGame_Xbox360_StopMovePointEast);
	xbox360Controller.SetCallback(Xbox360_Right, &CGame_Xbox360_MovePointerSouth);
	xbox360Controller.SetCallbackOnRelease(Xbox360_Right, &CGame_Xbox360_StopMovePointSouth);
	xbox360Controller.SetCallback(Xbox360_Left, &CGame_Xbox360_MovePointerNorth);
	xbox360Controller.SetCallbackOnRelease(Xbox360_Left, &CGame_Xbox360_StopMovePointNorth);
	xbox360Controller.SetCallback(Xbox360_Action1, &CGame_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Action2, &CGame_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_IncreaseWaveSpeed, &CGame_Xbox360_IncreaseWaveSpeed);
	xbox360Controller.SetCallback(Xbox360_DecreaseWaveSpeed, &CGame_Xbox360_DecreaseWaveSpeed);
	xbox360Controller.SetCallback(Xbox360_OpenIngameAlmanac, &CGame_AlmanacGame_Action);
	xbox360Controller.SetCallback(Xbox360_OpenIngameMenu, &CGame_PauseGame_Action);
#endif

	bCaptainEvent01 = false;
	bCaptainEvent02 = false;
	bCaptainEvent03 = false;
}


CGame::~CGame()
{
	if(s_Texture)
		delete s_Texture;

	//InGame Windows
	delete mainmenuig;
	delete endingwin;
	delete endingloose;
	delete nextWaveButton_x1;
	delete nextWaveButton_x2;
	delete nextWaveButton_x3;
	delete damageFadeScreen;
	delete background;
	delete defense_stage_back;
	delete pauseMenuButton;
	delete almanacButton;
	delete loadingfade;
	delete captainBoss;
	delete dynamicCredits;

	prop64_64         = NULL;
	prop64_96         = NULL;
	prop64_128        = NULL;
	RemoteControlColorButtonsTexture = NULL;
}

bool CGame::CheckWindowOffset()
{
	bool bCorrection = false;
	
	if(xWindowOffset < xMinOffset) { xWindowOffset = xMinOffset; bCorrection = true; }
	if(xWindowOffset > xMaxOffset) { xWindowOffset = xMaxOffset; bCorrection = true; }
	if(yWindowOffset < yMinOffset) { yWindowOffset = yMinOffset; bCorrection = true; }
	if(yWindowOffset > yMaxOffset) { yWindowOffset = yMaxOffset; bCorrection = true; }
	
	return bCorrection;
}

void CGame::CleanMapProperties()
{
	mp_lifes = 0;
	mp_coins = 0;
	mp_cinematic = 0;
	mp_howtoplaypage = 0;
	mp_howtoplaymaxpages = 0;
	mp_timedefense = 0;
	for (int i = 0; i < 9; i++ )
		mp_timetowaves[i] = 0;
	mp_tiletype = 0;

	for(int i = 0; i < MAX_INITNODES; i++)
	{
		initNodes[i] = NULL;
	}
}

void CGame::CleanTerrainObjects()
{
	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		isoTerrainArr[i].CleanIso();
	}
	for(int i = 0; i < MAX_ISO_PROPS; i++)
	{
		isoPropArr[i].CleanIso();
	}
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		isoProjectileArr[i].CleanIso();
	}
	for(int i = 0; i < MAX_ASTAROBJECTIVES; i++)
	{
		objectivesNodes[i] = NULL;
	}
	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			isoTerrainAstarGrid[i][j] = NULL;
			isoTerrainCursorGrid[i][j] = NULL;
		}
	}
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		isoTerrainAstarList[i] = NULL;
	}

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoExplotion[i].index = -1;
		isoHaste[i].index = -1;
		isoHeal[i].index  = -1;
		isoMight[i].index = -1;
		isoPoisonOnDeath[i].index = -1;
		isoExplotion[i].timer.SetTimeLimit(67);
		isoExplotion[i].bRender = false;
		isoHaste[i].timer.SetTimeLimit(75);
		isoHeal[i].timer.SetTimeLimit(75);
		isoMight[i].timer.SetTimeLimit(75);
		isoPoisonOnDeath[i].timer.SetTimeLimit(67);
		isoPoisonOnDeath[i].bRender = false;
	}

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		isoCoin[i].index = -1;
		isoCoin[i].timer.SetTimeLimit(50);
	}

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		inTerrainIsoCoin[i].index = -1;
		inTerrainIsoCoin[i].timer.SetTimeLimit(50);
	}
}

void CGame::CleanPawns()
{
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		isoEnemyArr[i].CleanIso();
	}
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoSelknamArr[i].CleanIso();
	}
}

void CGame::IsoPawnRenderInit()
{
	currentIsoPawn = 0;
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = &isoSelknamArr[i];
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoCoin  = NULL;
		isoPawnRenderContainer[currentIsoPawn]._z            = isoSelknamArr[i].getPosZ();
		currentIsoPawn++;
	}
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = &isoEnemyArr[i];
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoCoin  = NULL;
		isoPawnRenderContainer[currentIsoPawn]._z            = isoEnemyArr[i].getPosZ();
		currentIsoPawn++;
	}
	for(int i = 0; i < MAX_ISO_PROPS; i++) 
	{
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = &isoPropArr[i];
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoCoin  = NULL;
		isoPawnRenderContainer[currentIsoPawn]._z            = isoPropArr[i].getPosZ();
		currentIsoPawn++;
	}
	 
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = &isoProjectileArr[i];
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoCoin  = NULL;
		isoPawnRenderContainer[currentIsoPawn]._z            = isoProjectileArr[i].getPosZ();
		
		currentIsoPawn++;
	}

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = &isoPoisonOnDeath[i];
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoCoin  = NULL;
		isoPawnRenderContainer[currentIsoPawn]._z            = isoPoisonOnDeath[i]._z;
		currentIsoPawn++;
	}

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = &isoExplotion[i];
		isoPawnRenderContainer[currentIsoPawn].isoCoin  = NULL;
		isoPawnRenderContainer[currentIsoPawn]._z            = isoExplotion[i]._z;
		currentIsoPawn++;
	}
	

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoCoin  = &inTerrainIsoCoin[i];
		isoPawnRenderContainer[currentIsoPawn]._z            = inTerrainIsoCoin[i]._z;
		currentIsoPawn++;
	}
}

void CGame::IsoPawnRenderUpdateZ()
{
	for(int i = 0; i < currentIsoPawn; i++)
	{
		if(isoPawnRenderContainer[i].isoEnemy != NULL)
		{
			isoPawnRenderContainer[i]._z = isoPawnRenderContainer[i].isoEnemy->getPosY();
		}
		else if(isoPawnRenderContainer[i].isoSelknam != NULL)
		{
			isoPawnRenderContainer[i]._z = isoPawnRenderContainer[i].isoSelknam->getPosY();
		}
		else if(isoPawnRenderContainer[i].isoProp != NULL)
		{
			isoPawnRenderContainer[i]._z = isoPawnRenderContainer[i].isoProp->getPosY();
		}
		else if(isoPawnRenderContainer[i].isoProjectile != NULL)
		{
			isoPawnRenderContainer[i]._z = isoPawnRenderContainer[i].isoProjectile->getPosY();
		}
		else if(isoPawnRenderContainer[i].isoPoison != NULL)
		{
			isoPawnRenderContainer[i]._z = isoPawnRenderContainer[i].isoPoison->_y;
		}
		else if(isoPawnRenderContainer[i].isoExplotion != NULL)
		{
			isoPawnRenderContainer[i]._z = isoPawnRenderContainer[i].isoExplotion->_y;
		}
		else if(isoPawnRenderContainer[i].isoCoin != NULL)
		{
			isoPawnRenderContainer[i]._z = isoPawnRenderContainer[i].isoCoin->_y;
		}
	}

	sort(isoPawnRenderContainer, isoPawnRenderContainer + currentIsoPawn, order_by_z);

	if(bPrintOutput)
	{
		for(int i = 0; i < currentIsoPawn; i++)
		{
			if(isoPawnRenderContainer[i].isoProjectile != NULL && isoPawnRenderContainer[i].isoProjectile->GetType() != Projectile_None) IwTrace(APPLOG, ("Projectile"));
			else if(isoPawnRenderContainer[i].isoSelknam    != NULL && isoPawnRenderContainer[i].isoSelknam->getInitialized()) IwTrace(APPLOG, ("Selknam"));
		}
		bPrintOutput = false;
	}
}

void CGame::InitObject()
{
	bNumDebugSpawns = -1;

	lastCameraTerrainLerp = NULL;
	bMoveCameraToTile = false;

	bBreakEnemySpawn = false;

	bUnfreezeCameraAfterInitObject = true;

	waveTimerSpeed = WTIMER_NORMAL;

	srand (time(NULL));

	bRestoreSettingsWhenInitObjectIsCalled = true;

	if(currentMap >= 25 && gameMode == GAMEMODE_CAMPAIGN)
	{
		loadingfade->SetColor(255, 255, 255);
	}
	else
	{
		loadingfade->SetColor(0, 0, 0);
		fade->SetColor(0, 0, 0);
	}

	loadingfade->FadeIn(255);
	loadingfade->Update();
	loadingfade->Render();

	bIsActiveObj = true;
	InitResources();

	parserHelper.RetrieveMapFromFile(this);
	CursorFillGrid();
	AStarFillGrid();
	aStarHelper.AStarInitMapGetObjectiveNodes(this);
	aStarHelper.AStarInitPreCalcAdjNodes(this);
	GetInitNodes();
	
	TextureData prop64_64Texture, prop64_96Texture, prop64_128Texture;

	if(getMPTileType() == 1)
	{
		terrainTextureData = TextureHelper::GetTextureNamed("terrain_01");
		prop64_64Texture   = TextureHelper::GetTextureNamed("props_64x64_01");
		prop64_96Texture   = TextureHelper::GetTextureNamed("props_64x96_01");
		prop64_128Texture  = TextureHelper::GetTextureNamed("props_64x128_01");
	}
	else if(getMPTileType() == 2)
	{
		terrainTextureData = TextureHelper::GetTextureNamed("terrain_02");
		prop64_64Texture   = TextureHelper::GetTextureNamed("props_64x64_02");
		prop64_96Texture   = TextureHelper::GetTextureNamed("props_64x96_02");
		prop64_128Texture  = TextureHelper::GetTextureNamed("props_64x128_02");
	}
	else if(getMPTileType() == 3)
	{
		terrainTextureData = TextureHelper::GetTextureNamed("terrain_03");
		prop64_64Texture   = TextureHelper::GetTextureNamed("props_64x64_03");
		prop64_96Texture   = TextureHelper::GetTextureNamed("props_64x96_03");
		prop64_128Texture  = TextureHelper::GetTextureNamed("props_64x128_03");
	}
	else if(getMPTileType() == 4)
	{
		terrainTextureData = TextureHelper::GetTextureNamed("terrain_04");
		prop64_64Texture   = TextureHelper::GetTextureNamed("props_64x64_04");
		prop64_96Texture   = TextureHelper::GetTextureNamed("props_64x96_04");
		prop64_128Texture  = TextureHelper::GetTextureNamed("props_64x128_04");
	}

	terrainData     = TextureHelper::GetRenderData(terrainTextureData);
	prop64_64_Data  = TextureHelper::GetRenderData(prop64_64Texture);
	prop64_96_Data  = TextureHelper::GetRenderData(prop64_96Texture);
	prop64_128_Data = TextureHelper::GetRenderData(prop64_128Texture);

	for(int i = 0; i < 20; i++) enemiesPerWave[i] = 0;
	NormalizeMapXML();
	
	bCanOpenUnitMenu = false;

	if(mp_cinematic) 
	{
		inGameState = GSG_Cinematic;
	} 
	else 
	{
		inGameState = GSG_Defending;
		bCanOpenUnitMenu = true;
		bPause = false;
	}

	//How to play
	if(mp_howtoplaymaxpages != 0)
	{
		inGamePausedState = GSGP_HowToPlayIG;
		bPause = true;
	}

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_CGame, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_CGame, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotionCB, NULL);

	timer.SetTimeLimit(mp_timedefense * 1000);
	auxTimer.SetTimeLimit(2 * 1000); //delay between inGameStates

	bRenderAttackText = false;

	//Map pointer
	mapPointer.InitIso();
	mapPointer.SetCursorGrid(isoTerrainCursorGrid);

	if(dialogController->DialogExists(currentMap) && gameMode == GAMEMODE_CAMPAIGN)
	{
		bActivateDialog = true;
	}
	else
	{
		bActivateDialog = false;
	}

	if(currentMap == 1 && gameMode == GAMEMODE_CAMPAIGN)
	{
		bActiveTutorial = true;
	}
	else
	{
		bActiveTutorial = false;
		tutorial->SetSkipped(true);
	}

	tutorial->UnhideGUIElements();

	if(bShowTutorialLevel1 && bActiveTutorial)
	{
		tutorial->HideGUIElements();
	}

	unitManager.SetAStarHelper(&aStarHelper);
	unitManager.SetCursorGrid(isoTerrainCursorGrid);

	curr_wave = 0;
	extra_time = 0;
	wasted_coins = 0;
	last_mp_coins = mp_coins;

	time_before_fastButton = 4;

	bIsInvincible = false;
	damageFadeScreen->SetAlpha(0);

	//stop previous theme.
	s3eAudioStop();
	//then start the next
	if(bIsAudioPaused)
	{
		if(currentMap >= 1 && currentMap < 7)
			LastMusicStacked = Music_Forest_Ingame_01;
		else if(currentMap >= 7 && currentMap < 13)
			LastMusicStacked = Music_Forest_Ingame_01;
		else if(currentMap >= 13 && currentMap < 19)
			LastMusicStacked = Music_IceLand_Ingame_01;
		else if(currentMap >= 19 && currentMap < 25)
			LastMusicStacked = Music_Forest_Ingame_01;
		else
			LastMusicStacked = Music_Forest_Ingame_01;

		if(gameMode == GAMEMODE_CAMPAIGN && currentMap == 25)
			LastMusicStacked = Music_Boss;
	}
	else
	{
		if(gameMode == GAMEMODE_CAMPAIGN && currentMap == 25)
		{
			Utils::PlayBGM("bgm/sd_boss_base.mp3", 0);
		}
		else
		{
			int loopCount = 1;
			if((currentMap == 1 && bShowTutorialLevel1) || (dialogController->DialogExists(currentMap) && gameMode == GAMEMODE_CAMPAIGN))
				loopCount = 0;

			if(currentMap >= 1 && currentMap < 7)
				Utils::PlayBGM("bgm/sd_ingame_forest_01_theme_loop.mp3", loopCount);
			else if(currentMap >= 7 && currentMap < 13)
				Utils::PlayBGM("bgm/sd_ingame_mountain_01_theme_loop.mp3", loopCount);
			else if(currentMap >= 13 && currentMap < 19)
				Utils::PlayBGM("bgm/sd_ingame_iceland_01_theme_loop.mp3", loopCount);
			else if(currentMap >= 19 && currentMap < 25)
				Utils::PlayBGM("bgm/sd_ingame_fireland_01_theme_loop.mp3", loopCount);
			else
				Utils::PlayBGM("bgm/sd_ingame_forest_01_theme_loop.mp3", loopCount);
		}
		LastMusicStacked = Music_None;
	}

	timer_change_theme.SetTimeLimit(30000);

	for(num_waves = 0; mp_timetowaves[num_waves] != 0; num_waves++);
	num_waves++;

	bDoFadeOut = true;
	timeToFadeOut.Reset();
	timeToFadeOut.Start();

	bChangeTheme = true;

	if(unitMenu.GetIsActive()) 
		unitMenu.Close();

	if(unitManager.GetIsActive())
		unitManager.Close();

	xWindowOffset = 400;
	yWindowOffset = 100;
	
	//Fix UV coords.
	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		IsoTerrain* CurrentIsoTerrain = &isoTerrainArr[i];
		
		//Tile position in the original image (512x432 LG TV Version) (8 tiles x 9 tiles)
		float xPos = 512.0f * CurrentIsoTerrain->getUvMinX();
		float yPos = 432.0f * CurrentIsoTerrain->getUvMaxY();
		int xIdx = (int)(xPos / 64.0f);
		int yIdx = (int)(yPos / 48.0f);
		int originalIndex = xIdx + 8 * yIdx;

		//originalIndex = 9;

		//new image: 10 tiles x 7 tiles
		int newXIdx = originalIndex % 10;
		int newYIdx = (int)(originalIndex / 10.0f);
		float tileWidth = 96.0f;
		float tileHeight = 72.0f;
		
		//newYIdx = newXIdx = 0;

		float uvMinX = (terrainTextureData.textureXPos + newXIdx * tileWidth) / (float)terrainTextureData.atlasWidth;
		float uvMaxX = (terrainTextureData.textureXPos + (newXIdx + 1) * tileWidth) / (float)terrainTextureData.atlasWidth;
		float uvMinY = (terrainTextureData.textureYPos + newYIdx * tileHeight) / (float)terrainTextureData.atlasHeight;
		float uvMaxY = (terrainTextureData.textureYPos + (newYIdx + 1) * tileHeight) / (float)terrainTextureData.atlasHeight;
		
		CurrentIsoTerrain->setUvMaxX(uvMaxX);
		CurrentIsoTerrain->setUvMaxY(uvMaxY);
		CurrentIsoTerrain->setUvMinX(uvMinX);
		CurrentIsoTerrain->setUvMinY(uvMinY);
	}

	for(int i = 0; i < MAX_ISO_PROPS; i++)
	{
		IsoProp* CurrentIsoProp = &isoPropArr[i];
		float uvMinX, uvMinY, uvMaxX, uvMaxY;
		uvMinY = uvMinX = uvMaxX = uvMaxY = 0.0f;

		int propNum = CurrentIsoProp->getPropNum();
		if(propNum == -1) continue;
		
		if(propNum >= 31 && propNum <= 39)
		{
			uvMinX = (prop64_64Texture.textureXPos + CurrentIsoProp->getUvMinX() * 192.0f) / (float)prop64_64Texture.atlasWidth;
			uvMaxX = (prop64_64Texture.textureXPos + CurrentIsoProp->getUvMaxX() * 192.0f) / (float)prop64_64Texture.atlasWidth;
			uvMinY = (prop64_64Texture.textureYPos + CurrentIsoProp->getUvMinY() * 192.0f) / (float)prop64_64Texture.atlasHeight;
			uvMaxY = (prop64_64Texture.textureYPos + CurrentIsoProp->getUvMaxY() * 192.0f) / (float)prop64_64Texture.atlasHeight;
		}
		else if(propNum >= 40 && propNum <= 45)
		{
			uvMinX = (prop64_96Texture.textureXPos + CurrentIsoProp->getUvMinX() * 192.0f) / (float)prop64_96Texture.atlasWidth;
			uvMaxX = (prop64_96Texture.textureXPos + CurrentIsoProp->getUvMaxX() * 192.0f) / (float)prop64_96Texture.atlasWidth;
			uvMinY = (prop64_96Texture.textureYPos + CurrentIsoProp->getUvMinY() * 192.0f) / (float)prop64_96Texture.atlasHeight;
			uvMaxY = (prop64_96Texture.textureYPos + CurrentIsoProp->getUvMaxY() * 192.0f) / (float)prop64_96Texture.atlasHeight;
		}
		else if(propNum >= 46 && propNum <= 51)
		{
			uvMinX = (prop64_128Texture.textureXPos + CurrentIsoProp->getUvMinX() * 192.0f) / (float)prop64_128Texture.atlasWidth;
			uvMaxX = (prop64_128Texture.textureXPos + CurrentIsoProp->getUvMaxX() * 192.0f) / (float)prop64_128Texture.atlasWidth;
			uvMinY = (prop64_128Texture.textureYPos + CurrentIsoProp->getUvMinY() * 256.0f) / (float)prop64_128Texture.atlasHeight;
			uvMaxY = (prop64_128Texture.textureYPos + CurrentIsoProp->getUvMaxY() * 256.0f) / (float)prop64_128Texture.atlasHeight;
		}
		
		CurrentIsoProp->setUvMaxX(uvMaxX);
		CurrentIsoProp->setUvMaxY(uvMaxY);
		CurrentIsoProp->setUvMinX(uvMinX);
		CurrentIsoProp->setUvMinY(uvMinY);
	}

	movementMarkData = TextureHelper::GetTextureNamed("cursor_movement");
	movementMarkData.textureRotated = true;
	movementMarkDimension = TextureHelper::GetDimension(TextureHelper::GetRenderData(movementMarkData));
	movementMarkAtlasName = TextureHelper::GetAtlasName(movementMarkData);

	cannonWarningData = TextureHelper::GetTextureNamed("cursor_red_big");
	cannonWarningAtlas = TextureHelper::GetAtlasName(cannonWarningData);

	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			movementMarkMap[i][j] = 0;
		}
	}

	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			rangeTileMap[i][j] = 0;
			unavailableTileMap[i][j] = 0;
			cannonWarningTile[i][j] = 0;
			cannonWarningSkillType[i][j] = Skill_Type_None;
		}
	}

	//Enemy Spot Decal
	for(int i = 0; i < MAX_INITNODES; i++)
	{
		IsoTerrain* currTile = initNodes[i];
		if(currTile != NULL)
		{
			initNodesDirections[i] = 0;

			if(currTile->northNode && currTile->northNode->aStar_uniqueCost == ASTAR_WALKABLETILECOST && !IsInInitNodeList(currTile->northNode))
			{
				initNodesDirections[i] = 0;
				continue;
			}

			if(currTile->southNode && currTile->southNode->aStar_uniqueCost == ASTAR_WALKABLETILECOST && !IsInInitNodeList(currTile->southNode))
			{
				initNodesDirections[i] = 2;
				continue;
			}

			if(currTile->eastNode && currTile->eastNode->aStar_uniqueCost == ASTAR_WALKABLETILECOST && !IsInInitNodeList(currTile->eastNode))
			{
				initNodesDirections[i] = 3;
				continue;
			}

			if(currTile->westNode && currTile->westNode->aStar_uniqueCost == ASTAR_WALKABLETILECOST && !IsInInitNodeList(currTile->westNode))
			{
				initNodesDirections[i] = 1;
				continue;
			}
		}
	}

	enemySpotBlinkTime.Start();
	enemySpotFadeIn =  false;

	bEndingDialog = false;

	if(currentMap == 25)
	{
		InitBossBattle();
	}

	if(gameMode == GAMEMODE_SURVIVAL)
	{
		bBlockCamera = false;
		for(int i = 0; i < MAX_ISO_SELKNAM; i++)
		{
			isoSelknamArr[i].CleanIso();
		}

		survivalModeConfig->InitObject();
	}

	if(currentMap == 26 && gameMode == GAMEMODE_CAMPAIGN)
	{
		CleanPawns();
		isoSelknamArr[0].InitSelknam(SELKNAM_Archer_Base, false, 3);      isoSelknamArr[0].InitIsoPawn(isoTerrainAstarGrid[11][6]);
		isoSelknamArr[1].InitSelknam(SELKNAM_Archer_Poisoner, false, 3);  isoSelknamArr[1].InitIsoPawn(isoTerrainAstarGrid[12][5]);
		isoSelknamArr[2].InitSelknam(SELKNAM_Archer_Retarder, false, 3);  isoSelknamArr[2].InitIsoPawn(isoTerrainAstarGrid[13][5]);

		isoSelknamArr[3].InitSelknam(SELKNAM_Warrior_Base, false, 3);     isoSelknamArr[3].InitIsoPawn(isoTerrainAstarGrid[10][12]);
		isoSelknamArr[4].InitSelknam(SELKNAM_Warrior_Tank, false, 3);     isoSelknamArr[4].InitIsoPawn(isoTerrainAstarGrid[9][11]);
		isoSelknamArr[5].InitSelknam(SELKNAM_Warrior_Berserk, false, 3);  isoSelknamArr[5].InitIsoPawn(isoTerrainAstarGrid[8][11]);

		isoSelknamArr[6].InitSelknam(SELKNAM_Shaman_Healer, false, 3);    isoSelknamArr[6].InitIsoPawn(isoTerrainAstarGrid[4][2]);
		isoSelknamArr[7].InitSelknam(SELKNAM_Shaman_Might, false, 3);     isoSelknamArr[7].InitIsoPawn(isoTerrainAstarGrid[4][1]);
		isoSelknamArr[8].InitSelknam(SELKNAM_Shaman_Haste, false, 3);     isoSelknamArr[8].InitIsoPawn(isoTerrainAstarGrid[5][1]);

		isoSelknamArr[9].InitSelknam(SELKNAM_Bowler_Stuner, false, 3);    isoSelknamArr[9].InitIsoPawn(isoTerrainAstarGrid[12][13]);
		isoSelknamArr[10].InitSelknam(SELKNAM_Bowler_Rooter, false, 3);   isoSelknamArr[10].InitIsoPawn(isoTerrainAstarGrid[12][11]);
		isoSelknamArr[11].InitSelknam(SELKNAM_Bowler_Confuser, false, 3); isoSelknamArr[11].InitIsoPawn(isoTerrainAstarGrid[13][11]);

		dynamicCredits->InitObject();
	}

	bUnitBuffedAchievementChecked = false;

	currentGameSpeedValue = normalGameSpeed;
	bOnTripleVelocAchievement = false;
	nextWaveButton_x1->Enabled(false);
	nextWaveButton_x2->Enabled(false);
	nextWaveButton_x3->Enabled(false);
	bDeathOnlyByPoison = 0;

	bCaptainEvent01 = false;
	bCaptainEvent02 = false;
	bCaptainEvent03 = false;

	bIsActiveObj = true;

	Utils::FreezeCamera();

#ifdef STEAMREQUIRED
	if(gameMode == GAMEMODE_SURVIVAL)
	{
		g_SteamLeaderboards->FindLeaderboard("Survival");
	}
	else
		g_SteamLeaderboards->FindLeaderboard("Campaign_sd_");
	
#endif
}

bool CGame::IsInInitNodeList(IsoTerrain* node)
{
	for(int i = 0; i < MAX_INITNODES; i++)
	{
		if(initNodes[i])
		{
			if(initNodes[i] == node) return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void CGame::InitResources()
{
}

//Cursor fill grid
void CGame::CursorFillGrid()
{
	int tmpXNumTile = -1;
	int tmpYNumTile = -1;
	IsoTerrain *CurrentIsoTerrain = NULL;
	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		CurrentIsoTerrain = &isoTerrainArr[i];

		if(CurrentIsoTerrain && !CurrentIsoTerrain->getRender())
			continue;
		if(CurrentIsoTerrain == NULL)
			continue;
		tmpXNumTile = CurrentIsoTerrain->getXNumTile();
		tmpYNumTile = CurrentIsoTerrain->getYNumTile();
		if(CurrentIsoTerrain->getCanNavigate() 
			&& tmpXNumTile != -1
			&& tmpYNumTile != -1 )
		{
			isoTerrainCursorGrid[tmpXNumTile][tmpYNumTile] = CurrentIsoTerrain;
			if(CurrentIsoTerrain->getInitCursorPoint())
			{
				mapPointer.SetGrid(CurrentIsoTerrain);
			}
		}
	}
}

void CGame::AStarPropagateHelper()
{
	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		isoTerrainArr[i].aStarHelperRef = &aStarHelper;
	}
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		isoEnemyArr[i].aStarHelperRef = &aStarHelper;
		isoEnemyArr[i].gameRef = this;
	}
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoSelknamArr[i].aStarHelperRef = &aStarHelper;
		isoSelknamArr[i].gameRef = this;
	}
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		isoProjectileArr[i].SetGame(this);
	}
}
//A-STAR Fill Walkable grid
void CGame::AStarFillGrid()
{
	int tmpXNumTile = -1;
	int tmpYNumTile = -1;
	IsoTerrain *CurrentIsoTerrain = NULL;
	int numList = 0;
	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		CurrentIsoTerrain = &isoTerrainArr[i];

		if(CurrentIsoTerrain && !CurrentIsoTerrain->getRender())
			continue;

		if(CurrentIsoTerrain == NULL)
		{
			continue;
		}

		tmpXNumTile = CurrentIsoTerrain->getXNumTile();
		tmpYNumTile = CurrentIsoTerrain->getYNumTile();
		if(CurrentIsoTerrain->getIsWalkeable() 
			&& tmpXNumTile != -1
			&& tmpYNumTile != -1 )
		{
			isoTerrainAstarGrid[tmpXNumTile][tmpYNumTile] = CurrentIsoTerrain;
			isoTerrainAstarList[numList] = CurrentIsoTerrain;
			numList++;
		}
		else
		{
			if(tmpXNumTile != -1 && tmpYNumTile != -1)
			{
				isoTerrainAstarGrid[tmpXNumTile][tmpYNumTile] = CurrentIsoTerrain;
			}
		}
	}
	IwTrace(ASTAR, ("AStarFillGrid() called, done"));
}

void CGame::AStarCalcPathsForAllTiles()
{
	IsoTerrain *tileInit = NULL;
	IsoTerrain *tileFinal = NULL;
	IsoTerrain *resultList[ASTAR_LISTSMAX];
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
		resultList[i] = NULL;

	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			tileInit = isoTerrainAstarGrid[i][j];
			if(!tileInit)
				continue;
			for(int k = 0; k < MAX_ASTAROBJECTIVES; k++)
			{
				tileFinal = objectivesNodes[k];
				if(!tileFinal)
					continue;
				aStarHelper.AStarRetrievePathByGrid(isoTerrainAstarGrid, tileInit, tileFinal, resultList);
				//fill path on iso terrain paths by objetives
				if(resultList[0])
				{
					for(int l = 0; l < ASTAR_LISTSMAX; l++)
					{
						if(resultList[l])
							tileInit->isoPath[k][l] = resultList[l];
					}					
				}
			}
		}
	}
}

void CGame::EndObject()
{
	CleanMapProperties();
	CleanTerrainObjects();
	CleanPawns();
	bIsActiveObj = false;

	//Clean timers
	timer.Clear();
	auxTimer.Clear();
	waveTimer.Clear();

	//If gameState changed, then we need to end inGamePaused and InGame objects.
	inGamePausedState = GSGP_None;
	if(bPause)
		HandleUpdatePausedStates();

	inGameState = GSG_None;

	InitAndEndSubObjects();

	UnregisterDevices();

	s_Texture = NULL;

	if(unitMenu.GetIsActive()) 
		unitMenu.Close();

	xWindowOffset = yWindowOffset = 0;

	if(gameMode == GAMEMODE_SURVIVAL)
	{
		survivalModeConfig->End();
	}

	captainBoss->EndObject();
	dynamicCredits->EndObject();

	timer_change_theme.Reset();
	timer_change_theme.Stop();
}

void CGame::RegisterDevices()
{

}

void CGame::UnregisterDevices()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_CGame);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_CGame);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleSingleTouchMotionCB);
}

void CGame::InitAndEndSubObjects()
{
	if(lastInGameState != inGameState)
	{
		//Init States
		if(inGameState==GSG_Cinematic)
		{
			//init state
		}
		else if(inGameState==GSG_Defending)
		{
			auxTimer.Start();
		}
		else if(inGameState==GSG_Attack)
		{
			//init state
			bCanOpenUnitMenu = false;
			curr_wave = 0;
		}
		else if(inGameState==GSG_Win)
		{
			//init state
			endingwin->InitObject();

			//Achievement nº 6.
			//Invincible: Logra pasar una etapa del segundo mundo en adelante al triple de velocidad.	
			if(bOnTripleVelocAchievement)
			{
				achievementsHelper->SetAchievementCompletedById(5);
			}
			bOnTripleVelocAchievement = false;

		}
		else if(inGameState==GSG_Loose)
		{
			//init state
			survivalModeConfig->End();
			endingloose->InitObject();
		}

		//End States
		if(lastInGameState==GSG_Cinematic)
		{
			//end state
		}
		if(lastInGameState==GSG_Defending)
		{
			if(inGameState != GSG_Attack)
				bCanOpenUnitMenu = false;
			//end state
		}
		if(lastInGameState==GSG_Attack)
		{
			if(inGameState != GSG_Defending)
				bCanOpenUnitMenu = false;
			//end state
		}
		if(lastInGameState==GSG_Win)
		{
			//end state
			endingwin->EndObject();
		}
		if(lastInGameState==GSG_Loose)
		{
			//end state
			endingloose->EndObject();
		}

		lastInGameState = inGameState;
	}
}


#pragma region UPDATE_GAME
void CGame::Update()
{
	if(!bIsActiveObj)
		return;

#if defined DESKTOPPLATFORM
	if(Utils::IsControllerActive(Controller_KEYBOARD) && !bFreezeCamera
	   && (xbox360Controller.GetLastGameKeyPressed() == XBOX360_Button_Up 
	       || xbox360Controller.GetLastGameKeyPressed() == XBOX360_Button_Down
		   || xbox360Controller.GetLastGameKeyPressed() == XBOX360_Button_Left
		   || xbox360Controller.GetLastGameKeyPressed() == XBOX360_Button_Right))
	{
		cameraLerpTimer.Update();

		bMoveCameraToTile = true;

		if(bMoveCameraToTile
			&& mapPointer.getXNumTile() >= 0 && mapPointer.getXNumTile() < 18
			&& mapPointer.getYNumTile() >= 0 && mapPointer.getYNumTile() < 18
			&& lastCameraTerrainLerp != isoTerrainAstarGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()])
		{
			lastCameraTerrainLerp = isoTerrainAstarGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()];

			if(lastCameraTerrainLerp != NULL)
			{
				bMoveCameraToTile = false;
				lastCameraFinalXLerp = (int)lastCameraTerrainLerp->getPosX();
				lastCameraFinalYLerp = (int)lastCameraTerrainLerp->getPosY();
				lastCameraInitXLerp = Utils::GetCameraX();
				lastCameraInitYLerp = Utils::GetCameraY();
				cameraLerpTimer.Start(cameraSpeedConstant);
			}
		}

		if(cameraLerpTimer.GetLerpConstant() < 1)
		{
			Utils::MoveCamera(lastCameraInitXLerp, lastCameraInitYLerp, lastCameraFinalXLerp, lastCameraFinalYLerp, cameraLerpTimer.GetLerpConstant());

			if(CheckWindowOffset())
			{
				cameraLerpTimer.Finish();
			}
		}
		else
		{
			xWindowOffset = Utils::ToCameraXPos(lastCameraFinalXLerp);
			yWindowOffset = Utils::ToCameraYPos(lastCameraFinalYLerp);
			CheckWindowOffset();
		}
	}
#endif


#if defined DESKTOPPLATFORM
	if(!bPause 
		&& !bPauseGame 
		&& !unitMenu.GetIsActive() 
		&& (!tutorial->IsActive() || (tutorial->IsActive() && !tutorial->ClickeableFrame()))
		&& !unitManager.GetIsActive()
		&& (inGameState == GSG_Attack || inGameState == GSG_Defending)
		&& !dialogController->IsActive())
	{
		xbox360Controller.Update();
	}

	if(!bFreezeCamera && !bPause)
	{
		xWindowOffset += (int)(xbox360CameraSpeed * xbox360Controller.GetXRightAxis());
		yWindowOffset += (int)(xbox360CameraSpeed * xbox360Controller.GetYRightAxis());

		CheckWindowOffset();
	}
#endif

	float slowStopTimeParam = 15;
	float acc = (slowStopTimeParam) * 0.2f * 0.5f;
	int tmpXWindowsOffset = 0;
	int tmpYWindowsOffset = 0;
	int altCameraSpeed = 20;
	int altCameraSens = 4;
	int altCameraBorderLeft = 1 + altCameraSens;
	int altCameraBorderRight = currentGameWidth - 1 - altCameraSens;
	int altCameraBorderTop = 1 + altCameraSens;
	int altCameraBorderBottom = currentGameHeight - 1 - altCameraSens;
	
#if defined DESKTOPPLATFORM
	if (!bFreezeCamera && !bBlockCamera && !bPause && activeControllerType == Controller_MOUSE)
	{
		if (xMouse <= altCameraBorderLeft)
		{
			tmpXWindowsOffset = xWindowOffset;
			tmpXWindowsOffset -= altCameraSpeed;
			if (tmpXWindowsOffset <= xMinOffset)
				tmpXWindowsOffset = xMinOffset;
			xWindowOffset = tmpXWindowsOffset;

		}
		else if (xMouse >= altCameraBorderRight)
		{
			tmpXWindowsOffset = xWindowOffset;
			tmpXWindowsOffset += altCameraSpeed;
			if (tmpXWindowsOffset >= xMaxOffset)
				tmpXWindowsOffset = xMaxOffset;
			xWindowOffset = tmpXWindowsOffset;
		}

		if (yMouse <= altCameraBorderTop)
		{
			tmpYWindowsOffset = yWindowOffset;
			tmpYWindowsOffset -= altCameraSpeed;
			if (tmpYWindowsOffset <= yMinOffset)
				tmpYWindowsOffset = yMinOffset;
			yWindowOffset = tmpYWindowsOffset;

		}
		else if (yMouse >= altCameraBorderBottom)
		{
			tmpYWindowsOffset = yWindowOffset;
			tmpYWindowsOffset += altCameraSpeed;
			if (tmpYWindowsOffset >= yMaxOffset)
				tmpYWindowsOffset = yMaxOffset;
			yWindowOffset = tmpYWindowsOffset;
		}
	}
#endif

	if(bSlowStopEffect && !bBlockCamera && !bFreezeCamera)
	{
		timerSlowStop.Update();
		if(timerSlowStop.GetIsStop() || timerSlowStop.GetIsEnd())
		{
#if !defined DESKTOPPLATFORM
			xWindowOffset += (int)(fVec2SlowStop.x * slowStopTimeParam);
			yWindowOffset += (int)(fVec2SlowStop.y * slowStopTimeParam);
#endif
			bool bFinish = false;
			if(fVec2SlowStop.x > 0)
			{
				fVec2SlowStop.x -= acc;
				if(fVec2SlowStop.x <= 0) bFinish = true;
			}
			else
			{ 
				fVec2SlowStop.x += acc;
				if(fVec2SlowStop.x <= 0) bFinish = true;
			}

			if(fVec2SlowStop.x > 0)
			{
				fVec2SlowStop.x -= acc;
				if(fVec2SlowStop.x <= 0) bFinish = true;
			}
			else
			{ 
				fVec2SlowStop.x += acc;
				if(fVec2SlowStop.x <= 0) bFinish = true;
			}

			if(CheckWindowOffset() || bFinish)
			{
				bSlowStopEffect = false;
				timerSlowStop.Stop();
			}
			
			timerSlowStop.SetTimeLimit((int)slowStopTimeParam);
			timerSlowStop.Reset();
			timerSlowStop.Start();
			//IwTrace(TOUCHHELP, ("Moving..."));
		}
	}

	if(bMovingCamera)
	{
		uint64 t = (s3eTimerGetMs() - CGameRef->lastTouchMotionCBTime); 
		if(t >= 10)
		{
			CIwSVec2 diffVector = CIwSVec2(currentMousePosition.x - lastMousePositionRegistered.x, currentMousePosition.y - lastMousePositionRegistered.y);
			fVec2SlowStop = CIwFVec2(-diffVector.x / slowStopTimeParam, -diffVector.y / slowStopTimeParam);

			CGameRef->lastTouchMotionCBTime = s3eTimerGetMs();
			lastMousePositionRegistered = currentMousePosition;

			//IwTrace(TOUCHHELP, ("%f %f %d %d %d", CGameRef->fVec2SlowStop.x, CGameRef->fVec2SlowStop.y, t, -diffVector.x, -diffVector.y));
		}
	}

	loadingfade->Update();
	timeToFadeOut.Update();

	if(bPause && s3eAudioIsPlaying() && inGameState != GSG_Loose)
		s3eAudioPause();
	
	if(!bPause && !s3eAudioIsPlaying())
	{
		if(!bIsAudioPaused)
		{
			s3eAudioResume();
		}
	}

	//captain scripted events on defend
	if(currentMap==25 && captainBoss->GetIsActive())
	{
		if(timer_change_theme.GetCurrTime() >= 7000 && !bCaptainEvent01)
		{
			bCaptainEvent01 = true;
			Utils::PlaySoundFX(FX_Boss_comment_cycle01_moving_forward_01);
			captainBoss->PlayCaptainAnim(CaptainState_Attack);
		}
		else if(timer_change_theme.GetCurrTime() >= 17000 && !bCaptainEvent02)
		{
			bCaptainEvent02 = true;
			Utils::PlaySoundFX(FX_Boss_pain_02);
			captainBoss->PlayCaptainAnim(CaptainState_Attack);
		}
		//I like the number 27 and big numbers that contain this like 29127 :)
		else if(timer_change_theme.GetCurrTime() >= 29127 && !bCaptainEvent03)
		{
			bCaptainEvent03 = true;
			Utils::PlaySoundFX(FX_Boss_comment_cycle03_moving_forward_01);
			captainBoss->PlayCaptainAnim(CaptainState_Attack);
		}
	}

	if(timer_change_theme.GetIsEnd() && inGameState == GSG_Attack && bChangeTheme)
	{
		//IwTrace(NEXTTHEME, ("Change theme"));

		if(bIsAudioPaused)
		{
			if(currentMap >= 1 && currentMap < 7)
				LastMusicStacked = Music_Forest_Ingame_02;
			else if(currentMap >= 7 && currentMap < 13)
				LastMusicStacked = Music_Forest_Ingame_02;
			else if(currentMap >= 13 && currentMap < 19)
				LastMusicStacked = Music_IceLand_Ingame_02;
			else if(currentMap >= 19 && currentMap < 25)
				LastMusicStacked = Music_Forest_Ingame_02;
			else
				LastMusicStacked = Music_Forest_Ingame_02;
		}
		else
		{
			if(captainBoss->GetIsActive())
			{
				//Utils::PlayBGM("bgm/sd_boss_intro.mp3", 1);
				//bPlayBossSong = true;
			}
			else
			{
				if(currentMap >= 1 && currentMap < 7)
					Utils::PlayBGM("bgm/sd_ingame_forest_02_theme_loop.mp3", 0);
				else if(currentMap >= 7 && currentMap < 13)
					Utils::PlayBGM("bgm/sd_ingame_mountain_02_theme_loop.mp3", 0);
				else if(currentMap >= 13 && currentMap < 19)
					Utils::PlayBGM("bgm/sd_ingame_iceland_02_theme_loop.mp3", 0);
				else if(currentMap >= 19 && currentMap < 25)
					Utils::PlayBGM("bgm/sd_ingame_fireland_02_theme_loop.mp3", 0);
				else
					Utils::PlayBGM("bgm/sd_ingame_forest_02_theme_loop.mp3", 0);
			}
				
			LastMusicStacked = Music_None;
		}
			
		timer_change_theme.Reset();
		timer_change_theme.Stop();
		bChangeTheme = false;
	}

	//Ugly patch, when game is paused by the tutorial and the unitmenu is open. You need to click on the screen to enable it again.
	if(bUpdateClickEvents)
	{
		bUpdateClickEvents = false;
		s3ePointerEvent tempEvent;
		tempEvent.m_Pressed = 0;
		tempEvent.m_x = 0;
		tempEvent.m_y = 0;
		tempEvent.m_Button = S3E_POINTER_BUTTON_MIDDLEMOUSE;
		UpdateClickEvents(&tempEvent);
	}

	InitAndEndSubObjects();

	//logic loop
	if(!bPause)
		HandleUpdateGameStates(inGameState);
	if(inGameState==GSG_Cinematic)
	{
		//loop cinematic
		if(bPause)
			HandleUpdatePausedStates();
	}
	else if(inGameState==GSG_Defending)
	{
		//loop defending
		if(bPause)
			HandleUpdatePausedStates();
	}
	else if(inGameState==GSG_Attack)
	{
		//loop attack
		if(bPause)
			HandleUpdatePausedStates();
	}
	else if(inGameState==GSG_Win)
	{
		//loop win
		endingwin->Update();
	}
	else if(inGameState==GSG_Loose)
	{
		//loop loose
		endingloose->Update();
	}

	if(loadingfade->GetAlpha() == 0)
	{		
		if(currentMap != 25
			&& bRestoreSettingsWhenInitObjectIsCalled)
		{
			bRestoreSettingsWhenInitObjectIsCalled = false;
			bPauseGame = false;
			bBlockCamera = false;
		}

		fade->SetColor(0, 0, 0);
		loadingfade->SetColor(0, 0, 0);

		if(bActivateDialog && currentMap != 25)
		{
			dialogController->StartDialog(currentMap);
			bActivateDialog = false;
			bUnfreezeCameraAfterInitObject = false;
		}
		
		if(bShowTutorialLevel1 && bActiveTutorial && !dialogController->IsActive())
		{
			tutorial->SetGameRef(this);
			tutorial->HideGUIElements();
			tutorial->SetBasePosition(CIwSVec2((int)isoTerrainAstarGrid[15][3]->getPosX(), (int)isoTerrainAstarGrid[15][3]->getPosY()));
			tutorial->SetEnemySpotPosition(CIwSVec2((int)isoTerrainAstarGrid[0][7]->getPosX(), (int)isoTerrainAstarGrid[0][7]->getPosY()));
			tutorial->SetSkipped(false);
			tutorial->InitTutorial(Tutorial_Map1);
			bActiveTutorial = false;
			bUnfreezeCameraAfterInitObject = false;
		}

		if(bUnfreezeCameraAfterInitObject)
		{
			bUnfreezeCameraAfterInitObject = false;
			Utils::UnfreezeCamera();
		}
	}
	
	//TODO: Logic of game.
	if(loadingfade->GetAlpha() == 0)
	{
		CheckGameWinCondition();
	}

	//General updates.
	survivalModeConfig->Update();
	captainBoss->Update();
	dynamicCredits->Update();
	
	UpdateTimer();
	
	if(!bPause && !bPauseGame && !dialogController->IsActive())
	{
		CleanMapDeathPawns();
		UpdateIsoPawnsOnScreen();
		UpdateProjectiles();
		UpdateHUD();
		UpdateGameplayVariables();
		if(!bUnitBuffedAchievementChecked)
			UpdateCheckUnitBuffedAchievement();
	}
}

void CGame::UpdateCheckUnitBuffedAchievement()
{
	int myTotalUnitsBuffed = 0;
	//Update selknam on screen
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(isoSelknamArr[i].GetSelknamType() == SELKNAM_None)
			continue;
		if(!isoSelknamArr[i].getInitialized())
			continue;
		
		if(isoSelknamArr[i].has_skill_effect[(int)Skill_Type_Might] || 
			isoSelknamArr[i].has_skill_effect[(int)Skill_Type_Haste])
		{
			myTotalUnitsBuffed++;
		}
	}

	if (myTotalUnitsBuffed >= 9)
	{
		bUnitBuffedAchievementChecked = true;
		//Achievement nº 11.
		//All Buffed: Tener al menos 9 unidades bufeadas al mismo tiempo.
		achievementsHelper->SetAchievementCompletedById(10);
	}
}

void CGame::UpdateProjectiles()
{
	for(int i = 0; i <  MAX_ISO_PROJECTILE; i++)
	{
		if(isoProjectileArr[i].GetType() != Projectile_None)
		{
			isoProjectileArr[i].Update();
		}
	}
}

void CGame::HandleUpdateGameStates(InGameState _inGameState)
{
	if(_inGameState == GSG_Defending)
	{
		UpdateGameState_GSG_Defending();
	}
	else if(_inGameState == GSG_Attack)
	{
		UpdateGameState_GSG_Attack();
	}
}

void CGame::HandleUpdatePausedStates()
{
	if(lastInGamePausedState != inGamePausedState)
	{
		if(lastInGamePausedState == GSGP_MainMenuIG) mainmenuig->EndObject();
		else if(lastInGamePausedState == GSGP_AlmanacIG) almanacBase->EndObject();

		if(inGamePausedState == GSGP_MainMenuIG) mainmenuig->InitObject();
		else if(inGamePausedState == GSGP_AlmanacIG) almanacBase->InitObject();
		else if(inGamePausedState == GSGP_None) bPause = false;

		lastInGamePausedState = inGamePausedState;
	}

	if(inGamePausedState == GSGP_MainMenuIG) mainmenuig->Update();
	else if(inGamePausedState == GSGP_AlmanacIG) almanacBase->Update();
}

void CGame::UpdateGameState_GSG_Defending()
{
	//Once message is gone, countdown starts
	if(auxTimer.GetIsEnd() && timer.GetIsStop())
	{
		timer.Start();
		timer_change_theme.Reset();
		timer_change_theme.Start();
	} 

	//When defending time is over, the attack starts.
	if(timer.GetIsEnd()) 
	{
		InitAttackStage();
	}
}

void CGame::InitAttackStage()
{
	inGameState = GSG_Attack;
	bRenderAttackText = true;
	bCanOpenUnitMenu = false;
	auxTimer.Start();
	waveTimer.SetMaxTimeSecond(mp_timetowaves[0]);

	if(gameMode == GAMEMODE_SURVIVAL)
	{
		survivalModeConfig->Start();
	}
}

void CGame::UpdateGameState_GSG_Attack()
{
	if(!auxTimer.GetIsEnd())
		return;
	else 
	{
		bRenderAttackText = false;
		bCanOpenUnitMenu = true;
	}

	if(waveTimer.GetIsStop())
	{
		waveTimer.Start();
	}

	if(mp_timetowaves[curr_wave] != 0 && waveTimer.GetIsEnd())
	{
		waveTimer.Reset();
		curr_wave++;
		mp_coins += 7;
		waveTimer.SetMaxTimeSecond(mp_timetowaves[curr_wave]);
		waveTimer.Start();
	}

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		//Enemy Spawn
		IsoEnemy* CurrentEnemy = NULL;
		int initPathNum = 0;

		for(int i = 0; i < MAX_ISO_ENEMIES; i++)
		{
			if(bNumDebugSpawns <= 0 && bNumDebugSpawns != -1)
				break;

			CurrentEnemy = &isoEnemyArr[i];

			if(CurrentEnemy == NULL)
				continue;

			if(CurrentEnemy->getInitialized() || CurrentEnemy->GetEnemyType() == ENEMY_None)
				continue;

			if(CurrentEnemy->getWaveToSpawn() == curr_wave + 1)
			{
				if(!CurrentEnemy->bIsActiveObj && CurrentEnemy->getSecToSpawn() <= waveTimer.GetCurrTime())
				{
					initPathNum = CurrentEnemy->getInitPathNum() - 1;
					if(initNodes[initPathNum] != NULL)
					{
						//CurrentEnemy->setEnemyType(ENEMY_Soldier);
						CurrentEnemy->InitIsoPawn(initNodes[initPathNum]);
						
						//This is not part of the IA system, it's just a test feature.
						//bNumDebugSpawns must be set to a number greater than 1 in InitObject method()
						//Then in the stage will spawn only the first bNUmDebugSpawns enemies.
						//If bNumDebugSpawns = -1 then the game works as always.
						if(bNumDebugSpawns > 0)
						{
							bNumDebugSpawns--;
						}
						
						enemiesPerWave[CurrentEnemy->getWaveToSpawn() - 1]--;
						int index = (int)CurrentEnemy->GetEnemyType();
						isEnemyUnlocked[index - 1] = true;
					}
				}
			}
		}

		//Achievement nº 6 part 1.
		//Invincible: Logra pasar una etapa del segundo mundo en adelante al triple de velocidad.
		if((waveTimer.GetCurrTime() <= 3 && curr_wave == 0) 
			&& waveTimerSpeed == WTIMER_X3 && currentMap >= 7)
		{
			bOnTripleVelocAchievement = true;
		}
		else if(((waveTimer.GetCurrTime() > 3 && curr_wave == 0) || curr_wave > 0) && waveTimerSpeed != WTIMER_X3)
		{
			bOnTripleVelocAchievement = false;
		}
	}
}

void CGame::SpawnEnemyUnit(int node, int dest)
{
	if(initNodes[node] == NULL) return;

	//Enemy Spawn
	IsoEnemy* CurrentEnemy = NULL;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		CurrentEnemy = &isoEnemyArr[i];
		
		if(CurrentEnemy == NULL) continue;

		if(CurrentEnemy->getInitialized()) continue;

		CurrentEnemy->setEnemyType((IsoEnemyType)(rand() % 7 + 1));
		CurrentEnemy->setEnemyLevel((rand() % 3 + 1));

		//int spot = (rand() % 4 + 1);
		CurrentEnemy->setInitPathNum(node + 1);
		CurrentEnemy->setTargetNum(dest);

		CurrentEnemy->InitIsoPawn(initNodes[node]);

		return;
	}
}

void CGame::SpawnEnemyUnit(int node, int dest, IsoEnemyType type)
{
	if(initNodes[node] == NULL) return;

	//Enemy Spawn
	IsoEnemy* CurrentEnemy = NULL;
	for(int i = MAX_ISO_ENEMIES - 1; i >= 0; i--)
	{
		CurrentEnemy = &isoEnemyArr[i];

		if(CurrentEnemy == NULL) continue;

		if(CurrentEnemy->getInitialized()) continue;

		CurrentEnemy->setEnemyType(type);
		CurrentEnemy->setEnemyLevel(1);

		//int spot = (rand() % 4 + 1);
		CurrentEnemy->setInitPathNum(node + 1);
		CurrentEnemy->setTargetNum(dest);

		CurrentEnemy->InitIsoPawn(initNodes[node]);

		return;
	}
}

void CGame::UpdateTimer()
{
	enemySpotBlinkTime.Update();

	//Update timers
	if(!bPause && !bPauseGame && !dialogController->IsActive())
	{
		if(timer.GetIsStop()) timer.Resume();
		if(auxTimer.GetIsStop()) auxTimer.Resume();
		if(waveTimer.GetIsStop()) waveTimer.Resume();
		if(timer_change_theme.GetIsStop()) timer_change_theme.Resume();


		if(!bStopWaveTimer)
		{
			timer.Update();
		}
		else
		{
			timer.Stop();
		}

		auxTimer.Update();
		waveTimer.Update();
		timer_change_theme.Update();
	}
	else
	{
		timer.Stop();
		auxTimer.Stop();
		waveTimer.Stop();
		timer_change_theme.Stop();
	}
}

void CGame::UpdateHUD()
{
	//Map Pointer
	if(!unitMenu.GetIsActive() && !unitManager.GetIsActive())
		mapPointer.SetMovementConstraint(true);
	else
	{
		if(mapPointer.banishTime.GetIsEnd())
		{
			mapPointer.SetMovementConstraint(false);
		}
	}

	mapPointer.Update();
	unitMenu.Update();
	unitManager.Update();

	//Wave information
	sprintf(Number_Wave, "%s %i / %i", Wave_Text, curr_wave + 1, num_waves);

	//Timer
	if(inGameState == GSG_Defending)
	{
		sprintf(timer_text, "%s %i", TimeText, (int)(timer.GetCurrTime_Reversed() * 0.001));
		
		if(tutorial->GetTutorialName() != Tutorial_Map1)
		{
			if(7 > (int)(timer.GetCurrTime() * 0.001))
			{
				if(nextWaveButton_x1->IsActive())
					nextWaveButton_x1->Enabled(false);
			}
			else
			{
				if(!nextWaveButton_x1->IsActive() && !captainBoss->GetIsActive())
					nextWaveButton_x1->Enabled(true);
			}
		}
	}
	else if(inGameState == GSG_Attack) 
	{
		sprintf(timer_text, "%s %i", TimeText, waveTimer.GetCurrTime_Reversed());
		if(gameMode == GAMEMODE_SURVIVAL)
		{
			nextWaveButton_x1->Enabled(false);
		}
		//if(mp_timetowavesspawn[curr_wave] > waveTimer.GetCurrTime() || num_waves == (curr_wave + 1))
		//{
		//	if(nextWaveButton_x1->IsActive())
		//		nextWaveButton_x1->Enabled(false);
		//}
		//else
		//{
		//	if(!nextWaveButton_x1->IsActive())
		//		nextWaveButton_x1->Enabled(true);
		//}
	}

	sprintf(num_hearts_text, "%i", mp_lifes);
	sprintf(num_gold_text, "%i", mp_coins);

	damageFadeScreen->Update();
}

void CGame::UpdateGameplayVariables()
{
	//Update Wasted Coins

	if(last_mp_coins != mp_coins)
	{
		if(unitMenu.GetIsActive())
		{
			unitMenu.UpdateCoins(mp_coins);
			unitMenu.SetActiveOptions();
		}

		if (unitManager.GetIsActive())
		{
			unitManager.SetActiveOptions();
		}
	}

	if(last_mp_coins > mp_coins)
	{
		wasted_coins += last_mp_coins - mp_coins;
		last_mp_coins = mp_coins;
	}
	else if(last_mp_coins < mp_coins)
	{
		last_mp_coins = mp_coins;
	}

	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		isoTerrainArr[i].num_enemies_on_this_tile = 0;
	}

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		isoExplotion[i].timer.Update();
		isoPoisonOnDeath[i].timer.Update();
		isoHeal[i].timer.Update();
		isoMight[i].timer.Update();
		isoHaste[i].timer.Update();
	}

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		isoCoin[i].timer.Update();
	}

	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		inTerrainIsoCoin[i].timer.Update();
	}
}

void CGame::UpdateIsoPawnsOnScreen()
{
	int i = 0;
	//Update enemies on screen
	for(i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() == ENEMY_None)
			continue;
		if(!isoEnemyArr[i].getInitialized())
			continue;
		isoEnemyArr[i].Update();
	}

	//Update selknam on screen
	for(i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(isoSelknamArr[i].GetSelknamType() == SELKNAM_None)
			continue;
		if(!isoSelknamArr[i].getInitialized())
			continue;
		isoSelknamArr[i].Update();
	}

	IsoPawnRenderUpdateZ();
}
#pragma endregion UPDATE_GAME
#pragma region RENDER_GAME
void CGame::SetAtlasAsTexture(int atlasId)
{
	s_Texture = TextureHelper::GetCIwTexture(terrainData.atlasName);
	prop64_64 = TextureHelper::GetCIwTexture(prop64_64_Data.atlasName);
	prop64_96 = TextureHelper::GetCIwTexture(prop64_96_Data.atlasName);
	prop64_128 = TextureHelper::GetCIwTexture(prop64_128_Data.atlasName);
}

 void CGame::Render()
{
	if(!bIsActiveObj)
		return;

	//render loop
	HandleRenderGameStates(inGameState);
	
	if(inGameState==GSG_Cinematic)
	{
		//render loop cinematic	
		if(bPause)
			HandleRenderGamePausedStates();
	}
	else if(inGameState==GSG_Defending)
	{
		RenderGameState_GSG_Defending();
		if(bPause)
			HandleRenderGamePausedStates();
	}
	else if(inGameState==GSG_Attack)
	{
		RenderGameState_GSG_Attack();
		if(bPause)
			HandleRenderGamePausedStates();
	}
	else if(inGameState==GSG_Win)
	{
		//render loop win
		endingwin->Render();
	}
	else if(inGameState==GSG_Loose)
	{
		//render loop loose
		endingloose->Render();
	}

	dynamicCredits->Render();

	loadingfade->Render();

	if(bDoFadeOut && timeToFadeOut.GetIsEnd())
	{
		if(currentMap == 25 && gameMode == GAMEMODE_CAMPAIGN) //FINAL BOSS SCENARIO.
		{
			loadingfade->FadeOutWithTimer(finalBoss_time_fadeout, finalBoss_time_before_fadeout);
			captainBoss->SetEntrancePosition();
			captainBoss->EntranceScene();
		}
		else
		{
			loadingfade->FadeOut(FADE_OUT_SCREEN);	
		}

		bDoFadeOut = false;
	}

	//Temp Logs
	/*
	char debugString[256];
	int i = 0;
	
	int freeIsoSelknamSlots = 0;
	int totalIsoSelknamSlots = MAX_ISO_SELKNAM;
	for(i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(!isoSelknamArr[i].getRender())
			freeIsoSelknamSlots++;
	}

	int freeIsoEnemySlots = 0;
	int totalIsoEnemySlots = MAX_ISO_ENEMIES;
	for(i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(!isoEnemyArr[i].getRender())
			freeIsoEnemySlots++;
	}

	sprintf(debugString, "`x0000ff Selknam Slots %i , free: %i ", totalIsoSelknamSlots, freeIsoSelknamSlots);
	IwGxPrintString(5, 200, debugString, false);

	sprintf(debugString, "`x0000ff Enemy Slots %i , free: %i ", totalIsoEnemySlots, freeIsoEnemySlots);
	IwGxPrintString(5, 250, debugString, false);
	*/
}

void CGame::HandleRenderGameStates(InGameState _inGameState)
{
	background->Render();
	IwGxFlush();
	RenderMovementCursor();
	RenderTerrain();
	RenderIsoPawnsOnScreen();

	captainBoss->Render();

	//Restoring normal configuration for a 2D Isometric view right after rendering 3D Model.
	static CIwFMat view = CIwFMat::g_Identity;
	view = CIwFMat::g_Identity;
	IwGxSetViewMatrix(&view);
	IwGxSetModelMatrix(&view);

	if(captainBoss->GetIsVisible())
	{
		DrawCaptainBoss();
		captainBoss->RenderShipFragment(5);
	}

	RenderProjectiles();

	Utils::RenderExplotions();
	Utils::RenderCannonExplotions();

	IwGxFlush();
	RenderIsoPawnBarBuff();
	RenderIsoPawnBarLifes();
	RenderOthers();
	RenderHUD();

	captainBoss->RenderSpecialForeground();

	IwGxFlush();
}

void CGame::HandleRenderGamePausedStates()
{
	if(inGamePausedState == GSGP_MainMenuIG) mainmenuig->Render();
	else if(inGamePausedState == GSGP_AlmanacIG) almanacBase->Render();
}


void CGame::RenderMovementCursor()
{
	mapPointer.Render();
	unitManager.RenderMovementCursor();
}

void CGame::RenderHUD()
{
	if(dynamicCredits != NULL && dynamicCredits->GetIsActive() && !dynamicCredits->EasterEggActive())
	{
		return;
	}

	damageFadeScreen->Render();


	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_infocard.atlasName), 
		                       CIwSVec2(label_timeWave_posX + tutorial->OffsetWaveTimerGUI().x, label_timeWave_posY + tutorial->OffsetWaveTimerGUI().y),
							   label_infocard);

	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_hud_lives.atlasName), 
		                       CIwSVec2(label_heartGold_posX + tutorial->OffsetLifesGUI().x, label_heartGold_posY + tutorial->OffsetLifesGUI().y), 
							   label_hud_lives);

	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_hud_gold.atlasName), 
		                       CIwSVec2(label_heartGold_posX + TextureHelper::GetDimension(label_hud_lives).x + tutorial->OffsetGoldGUI().x, label_heartGold_posY + tutorial->OffsetGoldGUI().y),
							   label_hud_gold);

	IwGxLightingOn();

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		if(tutorial->GetTutorialName() == Tutorial_Map1)
		{
			char tmpChar[30];
			sprintf(tmpChar, "TIME: %d", tutorial->auxWaveTimer.GetCurrTime_Reversed() / 1000);

			Utils::RenderText(tmpChar,  
				CIwRect(label_timeWave_posX + 20 + tutorial->OffsetWaveTimerGUI().x, label_timeWave_posY + 65 + tutorial->OffsetWaveTimerGUI().y, 166, 90), 
				font_komikadisplaybold18, 
				0xff0da8d9, 
				IW_GX_FONT_ALIGN_LEFT, 
				IW_GX_FONT_ALIGN_TOP);

			Utils::RenderText("WAVE: ---", 
				CIwRect(label_timeWave_posX + 20 + tutorial->OffsetWaveTimerGUI().x, label_timeWave_posY + 65 + 25 + tutorial->OffsetWaveTimerGUI().y, 166, 90), 
				font_komikadisplaybold18, 
				0xff0da8d9, 
				IW_GX_FONT_ALIGN_LEFT, 
				IW_GX_FONT_ALIGN_TOP);
		}
		else
		{
			Utils::RenderText(timer_text,  
				CIwRect(label_timeWave_posX + 20 + tutorial->OffsetWaveTimerGUI().x, label_timeWave_posY + 65 + tutorial->OffsetWaveTimerGUI().y, 166, 90), 
				font_komikadisplaybold18, 
				0xff0da8d9, 
				IW_GX_FONT_ALIGN_LEFT, 
				IW_GX_FONT_ALIGN_TOP);

			Utils::RenderText(Number_Wave, 
				CIwRect(label_timeWave_posX + 20 + tutorial->OffsetWaveTimerGUI().x, label_timeWave_posY + 65 + 25 + tutorial->OffsetWaveTimerGUI().y, 166, 90), 
				font_komikadisplaybold18, 
				0xff0da8d9, 
				IW_GX_FONT_ALIGN_LEFT, 
				IW_GX_FONT_ALIGN_TOP);
		}

	}
	else if(gameMode == GAMEMODE_SURVIVAL)
	{
		if(inGameState == GSG_Defending)
		{
			Utils::RenderText(timer_text,  CIwRect(label_timeWave_posX + 20, label_timeWave_posY + 65, 166, 90), font_komikadisplaybold18, 0xff0da8d9, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
			Utils::RenderText(Number_Wave, CIwRect(label_timeWave_posX + 20, label_timeWave_posY + 65 + 25, 166, 90), font_komikadisplaybold18, 0xff0da8d9, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		}
		else
		{
			Utils::RenderText(survivalModeConfig->GetTimePlayed().c_str(),  CIwRect(label_timeWave_posX + 20, label_timeWave_posY + 65, 166, 50), font_komikadisplaybold18, 0xff0da8d9, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE);
		}
	}

	Utils::RenderText(num_hearts_text,
		              CIwRect(label_heartGold_posX + 50 + tutorial->OffsetLifesGUI().x, label_heartGold_posY + 45 + tutorial->OffsetLifesGUI().y, 64, 40), 
					  font_komikadisplaybold22, 
					  0xff0da8d9, 
					  IW_GX_FONT_ALIGN_CENTRE,
					  IW_GX_FONT_ALIGN_MIDDLE);

	Utils::RenderText(num_gold_text, 
					  CIwRect(label_heartGold_posX + 172 + tutorial->OffsetGoldGUI().x, label_heartGold_posY + 45 + tutorial->OffsetGoldGUI().y, 80, 40),
					  font_komikadisplaybold22, 
					  0xff0da8d9, 
					  IW_GX_FONT_ALIGN_CENTRE, 
					  IW_GX_FONT_ALIGN_MIDDLE);	

	IwGxLightingOff();

	if(inGameState == GSG_Attack && gameMode == GAMEMODE_CAMPAIGN && currentMap < 25)
	{
		if(nextWaveButton_x1->IsActive())
			nextWaveButton_x1->Render(tutorial->OffsetFastWaveGUI().x, tutorial->OffsetFastWaveGUI().y);
		else if(nextWaveButton_x2->IsActive())
			nextWaveButton_x2->Render(tutorial->OffsetFastWaveGUI().x, tutorial->OffsetFastWaveGUI().y);
		else if(nextWaveButton_x3->IsActive())
			nextWaveButton_x3->Render(tutorial->OffsetFastWaveGUI().x, tutorial->OffsetFastWaveGUI().y);
	}
	else
	{
		nextWaveButton_x1->Render(tutorial->OffsetFastWaveGUI().x, tutorial->OffsetFastWaveGUI().y);
	}

	pauseMenuButton->Render(tutorial->OffsetIngameMenuGUI().x, tutorial->OffsetIngameMenuGUI().y);
	almanacButton->Render(tutorial->OffsetAlmanacGUI().x, tutorial->OffsetAlmanacGUI().y);

	unitMenu.Render();
	unitManager.Render();	
}

void CGame::RenderIsoPawnBarBuff()
{
	IsoEnemy *CurrentIsoEnemy = NULL;
	IsoSelknam *CurrentIsoSelknam = NULL;
	CIwMaterial* pMat = NULL;
	float buff_uvMinX;
	float buff_uvMaxX;
	float buff_uvMinY;
	float buff_uvMaxY;
	s_Texture = TextureHelper::GetCIwTexture(buffBarAtlasName);

	pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetTexture(s_Texture);
	IwGxSetMaterial(pMat);
	numImagesBatched = 0;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		CurrentIsoEnemy = &isoEnemyArr[i];
		if(CurrentIsoEnemy && !CurrentIsoEnemy->getRender())
			continue;

		if(!CurrentIsoEnemy->getInitialized())
			continue;

		if(CurrentIsoEnemy->GetEnemyType() == ENEMY_None)
			continue;

		if(CurrentIsoEnemy->GetEnemyType() == ENEMY_Captain && !captainBoss->GetIsVisible())
			continue;

		int num_debuffs;
		Skill_Type list[MAX_SKILLS + 1];
		float offset_y = -67.0f;

		CurrentIsoEnemy->GetBuffList(list, &num_debuffs);
		
		for(int j = 0; j < num_debuffs; j++)
		{
			Utils::SetBuffImageUV(list[j], &buff_uvMaxX, &buff_uvMinX, &buff_uvMaxY, &buff_uvMinY);
			float offset_x = -28.0f + 20.0f * j;

			DrawQuadBatch(24, 24, 
				CurrentIsoEnemy->getPosX(), CurrentIsoEnemy->getPosY(), CurrentIsoEnemy->getPosZ(), 
				buff_uvMinX, buff_uvMaxX, buff_uvMinY, buff_uvMaxY,
				offset_x, offset_y);
		}
	}
	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(NULL);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();

	pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetTexture(s_Texture);
	IwGxSetMaterial(pMat);
	numImagesBatched = 0;
	float offset_y = -102.0f;
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		CurrentIsoSelknam = &isoSelknamArr[i];
		if(CurrentIsoSelknam && !CurrentIsoSelknam->getRender())
			continue;

		if(!CurrentIsoSelknam->getInitialized())
			continue;

		if(CurrentIsoSelknam->GetSelknamType() == SELKNAM_None)
			continue;

		int num_debuffs;
		Skill_Type list[MAX_SKILLS + 1];
		

		CurrentIsoSelknam->GetBuffList(list, &num_debuffs);
		for(int j = 0; j < num_debuffs; j++)
		{
			Utils::SetBuffImageUV(list[j], &buff_uvMaxX, &buff_uvMinX, &buff_uvMaxY, &buff_uvMinY);
			float offset_x = -36.0f + 24.0f * j;

			DrawQuadBatch(24, 24, 
				CurrentIsoSelknam->getPosX(), CurrentIsoSelknam->getPosY(), CurrentIsoSelknam->getPosZ(), 
				buff_uvMinX, buff_uvMaxX, buff_uvMinY, buff_uvMaxY,
				offset_x, offset_y);
		}
	}

	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(NULL);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();
}

void CGame::RenderOthers()
{
}

void CGame::RenderIsoPawnBarLifes()
{
	if(dialogController->IsActive() || (dynamicCredits->GetIsActive() && !dynamicCredits->EasterEggActive())) 
	{
		return;
	}

	numImagesBatched = 0;

	float barXOff = -38.0f;
	float barYOff = -76.0f;
	float barHeight = 4;
	float barWidth = 75.0f;

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	IwGxSetMaterial(pMat);
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(isoSelknamArr[i].GetSelknamType() == SELKNAM_None)
			continue;

		if(isoSelknamArr[i].GetIsDead())
			continue;

		float width = barWidth;
		DrawColorQuad(width + 2, barHeight + 2, isoSelknamArr[i].getPosX() - 1, isoSelknamArr[i].getPosY() - 1, isoSelknamArr[i].getPosZ(), 0xff101010, barXOff, barYOff);
	}
	IwGxSetColStream(s_ColorsBatch, 4 * numImagesBatched);
	IwGxSetVertStreamModelSpace(s_QuadBatch, 4 * numImagesBatched);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4 * numImagesBatched);

	numImagesBatched = 0;

	IwGxFlush();

	pMat = IW_GX_ALLOC_MATERIAL();
	IwGxSetMaterial(pMat);
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() == ENEMY_None)
			continue;

		if(isoEnemyArr[i].GetIsDead())
			continue;

		if(isoEnemyArr[i].GetEnemyType() == ENEMY_Captain)
			continue;

		float width = barWidth;

		DrawColorQuad(width + 2, barHeight + 2, isoEnemyArr[i].getPosX() - 1, isoEnemyArr[i].getPosY() - 1, isoEnemyArr[i].getPosZ(), 0xff101010, barXOff, barYOff);
	}
	IwGxSetColStream(s_ColorsBatch, 4 * numImagesBatched);
	IwGxSetVertStreamModelSpace(s_QuadBatch, 4 * numImagesBatched);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4 * numImagesBatched);
	IwGxFlush();

	numImagesBatched = 0;
	pMat = IW_GX_ALLOC_MATERIAL();
	IwGxSetMaterial(pMat);
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(isoSelknamArr[i].GetSelknamType() == SELKNAM_None)
			continue;
		float width = (barWidth * (float)isoSelknamArr[i].GetCurrentHealth()) / (float)isoSelknamArr[i].GetMaxHealth();
		
		int index = numImagesBatched * 4;
		float _width = width;
		float _height = barHeight;
		float _posX = isoSelknamArr[i].getPosX();
		float _posY = isoSelknamArr[i].getPosY();
		float _posZ = isoSelknamArr[i].getPosZ();
		float _offsetX = barXOff;
		float _offsetY = barYOff;

		_posX += _offsetX; 
		_posY += _offsetY;

		s_QuadBatch[index].x = _posX;
		s_QuadBatch[index].y = _posY;
		s_QuadBatch[index].z = _posZ;

		s_QuadBatch[index + 1].x = _posX;
		s_QuadBatch[index + 1].y = _posY + _height;
		s_QuadBatch[index + 1].z = _posZ;

		s_QuadBatch[index + 2].x = _posX + _width;
		s_QuadBatch[index + 2].y = _posY + _height;
		s_QuadBatch[index + 2].z = _posZ;

		s_QuadBatch[index + 3].x = _posX + _width;
		s_QuadBatch[index + 3].y = _posY;
		s_QuadBatch[index + 3].z = _posZ;

		s_ColorsBatch[index].Set(0xff0000ff);
		s_ColorsBatch[index + 1].Set(0xff252598);
		s_ColorsBatch[index + 2].Set(0xff252598);
		s_ColorsBatch[index + 3].Set(0xff0000ff);

		numImagesBatched++;
	}

	IwGxSetColStream(s_ColorsBatch, 4 * numImagesBatched);
	IwGxSetVertStreamModelSpace(s_QuadBatch, 4 * numImagesBatched);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4 * numImagesBatched);

	numImagesBatched = 0;

	IwGxFlush();

	pMat = IW_GX_ALLOC_MATERIAL();
	IwGxSetMaterial(pMat);
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() == ENEMY_None)
			continue;

		if(isoEnemyArr[i].GetEnemyType() == ENEMY_Captain)
			continue;

		float width = (barWidth * (float)isoEnemyArr[i].GetCurrentHealth()) / (float)isoEnemyArr[i].GetMaxHealth();

		int index = numImagesBatched * 4;
		float _width = width;
		float _height = barHeight;
		float _posX = isoEnemyArr[i].getPosX();
		float _posY = isoEnemyArr[i].getPosY();
		float _posZ = isoEnemyArr[i].getPosZ();
		float _offsetX = barXOff;
		float _offsetY = barYOff;

		_posX += _offsetX; 
		_posY += _offsetY;

		s_QuadBatch[index].x = _posX;
		s_QuadBatch[index].y = _posY;
		s_QuadBatch[index].z = _posZ;

		s_QuadBatch[index + 1].x = _posX;
		s_QuadBatch[index + 1].y = _posY + _height;
		s_QuadBatch[index + 1].z = _posZ;

		s_QuadBatch[index + 2].x = _posX + _width;
		s_QuadBatch[index + 2].y = _posY + _height;
		s_QuadBatch[index + 2].z = _posZ;

		s_QuadBatch[index + 3].x = _posX + _width;
		s_QuadBatch[index + 3].y = _posY;
		s_QuadBatch[index + 3].z = _posZ;

		s_ColorsBatch[index].Set(0xff0000ff);
		s_ColorsBatch[index + 1].Set(0xff252598);
		s_ColorsBatch[index + 2].Set(0xff252598);
		s_ColorsBatch[index + 3].Set(0xff0000ff);

		numImagesBatched++;

		//DrawColorQuad(width, barHeight, isoEnemyArr[i].getPosX(), isoEnemyArr[i].getPosY(), isoEnemyArr[i].getPosZ(), 0xff0000ff, barXOff, barYOff);
	}
	IwGxSetColStream(s_ColorsBatch, 4 * numImagesBatched);
	IwGxSetVertStreamModelSpace(s_QuadBatch, 4 * numImagesBatched);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4 * numImagesBatched);

	IwGxFlush();

	DrawCountdownDeathTimer();

	//-------------------------------//
	if(inGameState == GSG_Attack && captainBoss->GetIsActive())
	{
		//BOSS LIFE BAR
		numImagesBatched = 0;
		IwGxFlush();
		pMat = IW_GX_ALLOC_MATERIAL();
		IwGxSetMaterial(pMat);
		if(isoEnemyArr[0].GetEnemyType() == ENEMY_Captain && captainBoss->GetIsActive())
		{
			float bossHealthWidth = (bossBarLifeWidth * (float)isoEnemyArr[0].GetCurrentHealth()) / (float)isoEnemyArr[0].GetMaxHealth();

			//barra de fondo
			Utils::RenderColor(0xff333333, 0xff191919, CIwSVec2((int)bossBarLifeX + 12, (int)bossBarLifeY + 8), CIwSVec2((int)bossBarLifeWidth - 24, (int)bossBarLifeHeight - 16), true);
			IwGxFlush();
			//barra creciente
			Utils::RenderColor(0xff0000ff, 0xff252598, CIwSVec2((int)bossBarLifeX + 12, (int)bossBarLifeY + 8), CIwSVec2((int)bossHealthWidth - 24, (int)bossBarLifeHeight - 16), true);
			IwGxFlush();

			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(bossBarRenderData.atlasName), CIwSVec2((int)bossBarLifeX, (int)bossBarLifeY), bossBarRenderData);
		}
	}
	//-------------------------------//
}

void CGame::DrawCountdownDeathTimer()
{
	float barXOff = -38.0f;
	float barYOff = -74.0f;
	float barHeight = 4;
	float barWidth = 75.0f;
	
	IwGxLightingOn();
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(isoSelknamArr[i].GetSelknamType() == SELKNAM_None)
			continue;

		if(isoSelknamArr[i].GetIsDead())
			continue;

		if(!isoSelknamArr[i].has_skill_effect[(int)Skill_Type_CountdownDeath])
			continue;

		char str[100];
		sprintf(str, "%.2f", isoSelknamArr[i].GetCountdownDeathTimeLeft() * 0.001f);

		uint32 color = 0xffffffff;
		if(isoSelknamArr[i].GetCountdownDeathTimeLeft() <= 1200)
		{
			color = 0xff2525e7;
		}

		Utils::RenderText(str, CIwRect((int)(isoSelknamArr[i].getPosX() + barXOff - xWindowOffset), (int)(isoSelknamArr[i].getPosY() + barYOff - yWindowOffset), (int)(barWidth), 16), font_komikadisplay11, color, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP);
	}

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() == ENEMY_None)
			continue;

		if(isoEnemyArr[i].GetIsDead())
			continue;

		if(!isoEnemyArr[i].has_skill_effect[(int)Skill_Type_CountdownDeath])
			continue;

		char str[100];
		sprintf(str, "%.2f", isoEnemyArr[i].GetCountdownDeathTimeLeft() * 0.001f);

		uint32 color = 0xffffffff;
		if(isoEnemyArr[i].GetCountdownDeathTimeLeft() <= 1200)
		{
			color = 0xff2525e7;
		}

		Utils::RenderText(str, CIwRect((int)(isoEnemyArr[i].getPosX() + barXOff - xWindowOffset), (int)(isoEnemyArr[i].getPosY() + barYOff - yWindowOffset), (int)(barWidth), 16), font_komikadisplay11, color, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP);
	}
	IwGxLightingOff();

	if(captainBoss->GetIsActive())
	{
		captainBoss->DrawSpecialAttackCountdown();
	}

	IwGxFlush();
}

void CGame::RenderTerrain()
{
	IsoTerrain *CurrentIsoTerrain = NULL;
	int i = 0;
	int draws = 0;
	CIwMaterial* pMat = NULL;

	//DRAW TERRAIN
	SetAtlasAsTexture(getMPTileType());
	// Allocate a material from the IwGx global cache
	pMat = IW_GX_ALLOC_MATERIAL();
	// Set this as the active material
	pMat->SetTexture(s_Texture);
	IwGxSetMaterial(pMat);
	numImagesBatched = 0;
	for(i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		CurrentIsoTerrain = &isoTerrainArr[i];
		if(CurrentIsoTerrain && !CurrentIsoTerrain->getRender())
			continue;
		DrawQuadBatch(CurrentIsoTerrain->getWidth(), CurrentIsoTerrain->getHeight(), CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
			CurrentIsoTerrain->getUvMinX(), CurrentIsoTerrain->getUvMaxX(), CurrentIsoTerrain->getUvMinY(), CurrentIsoTerrain->getUvMaxY(),
			CurrentIsoTerrain->getOffsetX(), CurrentIsoTerrain->getOffsetY());

		CurrentIsoTerrain->UpdateAnimation();
		draws++;
	}
	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(NULL);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();

	//RANGE TILE MAP
	//DRAW MOVEMENT MARKS
	// Allocate a material from the IwGx global cache
	pMat = IW_GX_ALLOC_MATERIAL();
	// Set this as the active material
	pMat->SetTexture(TextureHelper::GetCIwTexture(rangeTileMapAtlasName));
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(pMat);
	numImagesBatched = 0;

	float uvX = 0.2f;
	int movementCursorXid = 4;
	int alpha = 0;

	for(i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			if(rangeTileMap[i][j] > 0 || rangeTileMap[i][j] == -1)
			{
				CurrentIsoTerrain = isoTerrainAstarGrid[i][j];
				if(CurrentIsoTerrain && !CurrentIsoTerrain->getRender())
					continue;

				if(CurrentIsoTerrain == NULL) continue;

				if(!CurrentIsoTerrain->getIsRamp())
				{
					movementCursorXid = 0;
				}
				else
				{
					if(CurrentIsoTerrain->getIsWalkeableFromWest())
					{
						if(!CurrentIsoTerrain->getRampConnOnWest())
						{
							movementCursorXid = 1;
						}
						else
						{
							movementCursorXid = 4;
						}
					}
					else
					{
						if(!CurrentIsoTerrain->getRampConnOnNorth())
						{
							movementCursorXid = 2;
						}
						else
						{
							movementCursorXid = 3;
						}
					}
				}

				float uvMinX, uvMinY, uvMaxX, uvMaxY;

				if(!rangeTileMapData.textureRotated)
				{
					uvMinY = (rangeTileMapData.textureYPos) / (float)rangeTileMapData.atlasHeight;
					uvMinX = (0.2f * movementCursorXid * rangeTileMapData.textureWidth + rangeTileMapData.textureXPos) / (float)rangeTileMapData.atlasWidth;
					uvMaxY = (rangeTileMapData.textureHeight + rangeTileMapData.textureYPos) / (float)rangeTileMapData.atlasHeight;
					uvMaxX = (0.2f * (movementCursorXid + 1) * rangeTileMapData.textureWidth  + rangeTileMapData.textureXPos) / (float)rangeTileMapData.atlasWidth;
				}
				else
				{
					uvMinY = (0.2f * movementCursorXid * rangeTileMapData.textureHeight + rangeTileMapData.textureYPos) / (float)rangeTileMapData.atlasHeight;
					uvMaxY = (0.2f * (movementCursorXid + 1) * rangeTileMapData.textureHeight + rangeTileMapData.textureYPos) / (float)rangeTileMapData.atlasHeight;
					uvMinX = (rangeTileMapData.textureXPos) / (float)rangeTileMapData.atlasWidth;
					uvMaxX = (rangeTileMapData.textureWidth + rangeTileMapData.textureXPos) / (float)rangeTileMapData.atlasWidth;
				}


				if(rangeTileMap[i][j] > 0)
				{
					int tmpTime = rangeTileMap[i][j];
					if(tmpTime > blinkMapTileTime)
					{
						tmpTime = rangeTileMap[i][j] - blinkMapTileTime;
					}

					if(tmpTime <= (int)(blinkMapTileTime * 0.5f))
					{
						alpha = (int)((255 / (blinkMapTileTime * 0.5f)) * tmpTime);
					}
					else
					{
						alpha = (int)(-(255 / (blinkMapTileTime * 0.5f)) * (tmpTime - blinkMapTileTime));
					}

					if(alpha < 0) alpha = 0;
					if(alpha > 255) alpha = 255;
				}
				else
				{
					alpha = 255;
				}

				DrawQuadBatch(64.0f * globalScale, 64.0f * globalScale, 
					CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
					uvMinX, uvMaxX, uvMinY, uvMaxY,
					-32.0f * globalScale, -32.0f * globalScale, 255, 255, 255, alpha, rangeTileMapData.textureRotated);

				if(rangeTileMap[i][j] > 0)
				{
					rangeTileMap[i][j] -= (int)deltaSystemTime;
					if(rangeTileMap[i][j] < 0)
					{
						rangeTileMap[i][j] = 0;
					}
				}
			}
		}
	}
	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(s_ColorsBatch);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();

	//DRAW MOVEMENT MARKS
	// Allocate a material from the IwGx global cache
	pMat = IW_GX_ALLOC_MATERIAL();
	// Set this as the active material
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(TextureHelper::GetCIwTexture(movementMarkAtlasName));
	IwGxSetMaterial(pMat);
	numImagesBatched = 0;

	uvX = 0.2f;
	float uvY = 0.2f;
	movementCursorXid = 4;
	int movementCursorYid = 0;

	for(i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			if(movementMarkMap[i][j] > 0)
			{
				CurrentIsoTerrain = isoTerrainAstarGrid[i][j];
				if(CurrentIsoTerrain && !CurrentIsoTerrain->getRender())
					continue;

				if(CurrentIsoTerrain == NULL) continue;

				if(!CurrentIsoTerrain->getIsRamp())
				{
					movementCursorYid = 0;
				}
				else
				{
					if(CurrentIsoTerrain->getIsWalkeableFromWest())
					{
						if(!CurrentIsoTerrain->getRampConnOnWest())
						{
							movementCursorYid = 1;
						}
						else
						{
							movementCursorYid = 3;
						}
					}
					else
					{
						if(!CurrentIsoTerrain->getRampConnOnNorth())
						{
							movementCursorYid = 2;
						}
						else
						{
							movementCursorYid = 4;
						}
					}
				}

				float uvMinX, uvMinY, uvMaxX, uvMaxY;

				if(movementMarkData.textureRotated)
				{
					movementCursorYid = 4 - movementCursorYid;

					movementCursorXid = 4;
					movementCursorYid = 4;
					//IwTrace(dsadsad, ("%d %d", movementMarkData.textureWidth, movementMarkData.textureHeight));
					//movementCursorXid = movementCursorYid = 0;
					/*
					uvMinX = ((uvX * movementCursorXid) * movementMarkData.textureHeight + movementMarkData.textureYPos) / (float)movementMarkData.atlasWidth;
					uvMinY = ((uvY * movementCursorYid) * movementMarkData.textureWidth  + movementMarkData.textureXPos) / (float)movementMarkData.atlasHeight;
					uvMaxX = ((uvX * (movementCursorXid + 1)) * movementMarkData.textureHeight + movementMarkData.textureYPos) / (float)movementMarkData.atlasWidth;
					uvMaxY = ((uvY * (movementCursorYid + 1)) * movementMarkData.textureWidth  + movementMarkData.textureXPos) / (float)movementMarkData.atlasHeight;
					*/
					uvMinX = ((uvX * movementCursorXid) * movementMarkData.textureWidth + movementMarkData.textureXPos) / (float)movementMarkData.atlasWidth;
					uvMinY = ((uvY * movementCursorYid) * movementMarkData.textureHeight  + movementMarkData.textureYPos) / (float)movementMarkData.atlasHeight;
					uvMaxX = ((uvX * (movementCursorXid + 1)) * movementMarkData.textureWidth + movementMarkData.textureXPos) / (float)movementMarkData.atlasWidth;
					uvMaxY = ((uvY * (movementCursorYid + 1)) * movementMarkData.textureHeight  + movementMarkData.textureYPos) / (float)movementMarkData.atlasHeight;
				}
				else
				{

					uvMinX = ((uvX * movementCursorXid) * movementMarkData.textureWidth + movementMarkData.textureXPos) / (float)movementMarkData.atlasWidth;
					uvMinY = ((uvY * movementCursorYid) * movementMarkData.textureHeight  + movementMarkData.textureYPos) / (float)movementMarkData.atlasHeight;
					uvMaxX = ((uvX * (movementCursorXid + 1)) * movementMarkData.textureWidth + movementMarkData.textureXPos) / (float)movementMarkData.atlasWidth;
					uvMaxY = ((uvY * (movementCursorYid + 1)) * movementMarkData.textureHeight  + movementMarkData.textureYPos) / (float)movementMarkData.atlasHeight;
				}

				if(movementMarkData.textureRotated)
				{
					DrawQuadBatch(48.0f * globalScale, 40.0f * globalScale, 
						CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
						uvMinX, uvMaxX, uvMinY, uvMaxY,
						-24.0f * globalScale, -18.0f * globalScale, 255, 255, 255,  255, movementMarkData.textureRotated);
				}
				else
				{
					DrawQuadBatch(48.0f * globalScale, 40.0f * globalScale, 
						CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
						uvMinX, uvMaxX, uvMinY, uvMaxY,
						-24.0f * globalScale, -18.0f * globalScale, 255, 255, 255,  255, movementMarkData.textureRotated);
				}
			}
		}
	}
	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(NULL);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();

	DrawUnavailableTiles();

	DrawEnterEnemySpots();
	DrawCannonWarnings();
}

void CGame::DrawUnavailableTiles()
{
	IsoTerrain *CurrentIsoTerrain = NULL;
	CIwMaterial* pMat = NULL;

	pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetTexture(TextureHelper::GetCIwTexture(unavailableTileMapAtlasName));
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(pMat);
	numImagesBatched = 0;

	float uvX = 0.2f;
	int movementCursorXid = 4;
	int alpha = 0;

	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			if(unavailableTileMap[i][j] > 0 || unavailableTileMap[i][j] == -1)
			{
				CurrentIsoTerrain = isoTerrainAstarGrid[i][j];
				if(CurrentIsoTerrain && !CurrentIsoTerrain->getRender())
					continue;

				if(CurrentIsoTerrain == NULL) continue;

				if(!CurrentIsoTerrain->getIsRamp())
				{
					movementCursorXid = 0;
				}
				else
				{
					if(CurrentIsoTerrain->getIsWalkeableFromWest())
					{
						if(!CurrentIsoTerrain->getRampConnOnWest())
						{
							movementCursorXid = 1;
						}
						else
						{
							movementCursorXid = 4;
						}
					}
					else
					{
						if(!CurrentIsoTerrain->getRampConnOnNorth())
						{
							movementCursorXid = 2;
						}
						else
						{
							movementCursorXid = 3;
						}
					}
				}

				float uvMinX, uvMinY, uvMaxX, uvMaxY;

				if(!unavailableTileMapData.textureRotated)
				{
					uvMinY = (unavailableTileMapData.textureYPos) / (float)unavailableTileMapData.atlasHeight;
					uvMinX = (0.2f * movementCursorXid * unavailableTileMapData.textureWidth + unavailableTileMapData.textureXPos) / (float)unavailableTileMapData.atlasWidth;
					uvMaxY = (unavailableTileMapData.textureHeight + unavailableTileMapData.textureYPos) / (float)unavailableTileMapData.atlasHeight;
					uvMaxX = (0.2f * (movementCursorXid + 1) * unavailableTileMapData.textureWidth  + unavailableTileMapData.textureXPos) / (float)unavailableTileMapData.atlasWidth;
				}
				else
				{
					uvMinY = (0.2f * movementCursorXid * unavailableTileMapData.textureHeight + unavailableTileMapData.textureYPos) / (float)unavailableTileMapData.atlasHeight;
					uvMaxY = (0.2f * (movementCursorXid + 1) * unavailableTileMapData.textureHeight + unavailableTileMapData.textureYPos) / (float)unavailableTileMapData.atlasHeight;
					uvMinX = (unavailableTileMapData.textureXPos) / (float)unavailableTileMapData.atlasWidth;
					uvMaxX = (unavailableTileMapData.textureWidth + unavailableTileMapData.textureXPos) / (float)unavailableTileMapData.atlasWidth;
				}


				if(unavailableTileMap[i][j] > 0)
				{
					int tmpTime = unavailableTileMap[i][j];
					if(tmpTime > blinkMapTileTime)
					{
						tmpTime = unavailableTileMap[i][j] - blinkMapTileTime;
					}

					if(tmpTime <= (int)(blinkMapTileTime * 0.5f))
					{
						alpha = (int)((255 / (blinkMapTileTime * 0.5f)) * tmpTime);
					}
					else
					{
						alpha = (int)(-(255 / (blinkMapTileTime * 0.5f)) * (tmpTime - blinkMapTileTime));
					}

					if(alpha < 0) alpha = 0;
					if(alpha > 255) alpha = 255;
				}
				else
				{
					alpha = 255;
				}

				DrawQuadBatch(64.0f * globalScale, 64.0f * globalScale, 
					CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
					uvMinX, uvMaxX, uvMinY, uvMaxY,
					-32.0f * globalScale, -32.0f * globalScale, 255, 255, 255, alpha, unavailableTileMapData.textureRotated);

				if(unavailableTileMap[i][j] > 0)
				{
					unavailableTileMap[i][j] -= (int)deltaSystemTime;
					if(unavailableTileMap[i][j] < 0)
					{
						unavailableTileMap[i][j] = 0;
					}
				}
			}
		}
	}
	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(s_ColorsBatch);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();
}

void CGame::DrawCannonWarnings()
{
	IsoTerrain *CurrentIsoTerrain = NULL;
	CIwMaterial* pMat = NULL;
	pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(TextureHelper::GetCIwTexture(cannonWarningAtlas));
	IwGxSetMaterial(pMat);

	numImagesBatched = 0;
	float uvX = 1.0f;
	float uvY = 1.0f;
	int movementCursorXid = 0;
	int movementCursorYid = 0;

	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			if(cannonWarningTile[i][j] > 0)
			{
				if(!Utils::GameIsPaused())
				{
					cannonWarningTile[i][j] -= (int)deltaSystemTime;
				}
				
				if(cannonWarningTile[i][j] <= 0)
				{
					captainBoss->Shoot(isoTerrainAstarGrid[i][j], cannonWarningSkillType[i][j]);
				}

				CurrentIsoTerrain = isoTerrainAstarGrid[i][j];
				if(CurrentIsoTerrain && !CurrentIsoTerrain->getRender())
					continue;

				if(CurrentIsoTerrain == NULL) continue;

				float uvMinX, uvMinY, uvMaxX, uvMaxY;

				if(cannonWarningData.textureRotated)
				{
					movementCursorYid = 4 - movementCursorYid;
					uvMinX = ((uvX * movementCursorXid) * cannonWarningData.textureHeight + cannonWarningData.textureYPos) / (float)cannonWarningData.atlasWidth;
					uvMinY = ((uvY * movementCursorYid) * cannonWarningData.textureWidth  + cannonWarningData.textureXPos) / (float)cannonWarningData.atlasHeight;
					uvMaxX = ((uvX * (movementCursorXid + 1)) * cannonWarningData.textureHeight + cannonWarningData.textureYPos) / (float)cannonWarningData.atlasWidth;
					uvMaxY = ((uvY * (movementCursorYid + 1)) * cannonWarningData.textureWidth  + cannonWarningData.textureXPos) / (float)cannonWarningData.atlasHeight;
				}
				else
				{
					uvMinX = ((uvX * movementCursorXid) * cannonWarningData.textureWidth + cannonWarningData.textureXPos) / (float)cannonWarningData.atlasWidth;
					uvMinY = ((uvY * movementCursorYid) * cannonWarningData.textureHeight  + cannonWarningData.textureYPos) / (float)cannonWarningData.atlasHeight;
					uvMaxX = ((uvX * (movementCursorXid + 1)) * cannonWarningData.textureWidth + cannonWarningData.textureXPos) / (float)cannonWarningData.atlasWidth;
					uvMaxY = ((uvY * (movementCursorYid + 1)) * cannonWarningData.textureHeight  + cannonWarningData.textureYPos) / (float)cannonWarningData.atlasHeight;
				}

				if(cannonWarningData.textureRotated)
				{
					DrawQuadBatch(48.0f * globalScale, 48.0f * globalScale, 
						CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
						uvMinY, uvMaxY, uvMinX, uvMaxX,
						-24.0f * globalScale, -36.0f * globalScale, 255, 255, 255,  255, cannonWarningData.textureRotated);
				}
				else
				{
					DrawQuadBatch(48.0f * globalScale, 48.0f * globalScale, 
						CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
						uvMinX, uvMaxX, uvMinY, uvMaxY,
						-24.0f * globalScale, -36.0f * globalScale, 255, 255, 255,  255, cannonWarningData.textureRotated);
				}
			}
		}
	}
	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(NULL);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();
}

void CGame::DrawCaptainBoss()
{
	if(!captainBoss->GetIsActive())
	{
		return;
	}

	IsoTerrain *CurrentIsoTerrain = NULL;
	int i = 0;
	int draws = 0;
	CIwMaterial* pMat = NULL;

	AtlasName atlas = captainBoss->GetAtlasByCaptainState(captainBoss->GetState());
	RenderData rData = captainBoss->GetRenderDataByCaptainState(captainBoss->GetState());

	pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetTexture(TextureHelper::GetCIwTexture(atlas));
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(pMat);
	numImagesBatched = 0;

	float uvMinY = rData.uvInfo.minY;
	float uvMaxY = rData.uvInfo.maxY;
	float uvMinX = rData.uvInfo.minX;
	float uvMaxX = rData.uvInfo.maxX;

	CIwFVec3 position = captainBoss->GetPosition();

	DrawQuadBatch((float)rData.width * 0.85f, (float)rData.height * 0.85f, 
		          position.x, position.y, position.z, 
				  uvMinX, uvMaxX, uvMinY, uvMaxY,
				  15, -55, 255, 255, 255, 255, rData.bRotate);

	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(s_ColorsBatch);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();
}

void CGame::DrawEnterEnemySpots()
{
	if(inGameState != GSG_Defending || dialogController->IsActive()) return;

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	// Set this as the active material
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(TextureHelper::GetCIwTexture(enemySpotAtlasName));
	IwGxSetMaterial(pMat);

	numImagesBatched = 0;

	float uvX = 0.25f;
	float uvY = 1.0f;

	int xIndex = 0;
	int yIndex = 0;

	for(int i = 0; i < MAX_INITNODES; i++)
	{
		IsoTerrain* CurrentIsoTerrain = initNodes[i];
		if(CurrentIsoTerrain != NULL)
		{
			xIndex = initNodesDirections[i];

			float uvMinX, uvMinY, uvMaxX, uvMaxY;

			if(enemySpotData.textureRotated)
			{
				uvMinX = ((uvX * xIndex) * enemySpotData.textureHeight + enemySpotData.textureYPos) / (float)enemySpotData.atlasHeight;
				uvMinY = ((uvY * yIndex) * enemySpotData.textureWidth  + enemySpotData.textureXPos) / (float)enemySpotData.atlasWidth;
				uvMaxX = ((uvX * (xIndex + 1)) * enemySpotData.textureHeight + enemySpotData.textureYPos) / (float)enemySpotData.atlasHeight;
				uvMaxY = ((uvY * (yIndex + 1)) * enemySpotData.textureWidth  + enemySpotData.textureXPos) / (float)enemySpotData.atlasWidth;
			}
			else
			{
				uvMinX = ((uvX * xIndex) * enemySpotData.textureWidth + enemySpotData.textureXPos) / (float)enemySpotData.atlasWidth;
				uvMinY = ((uvY * yIndex) * enemySpotData.textureHeight  + enemySpotData.textureYPos) / (float)enemySpotData.atlasHeight;
				uvMaxX = ((uvX * (xIndex + 1)) * enemySpotData.textureWidth + enemySpotData.textureXPos) / (float)enemySpotData.atlasWidth;
				uvMaxY = ((uvY * (yIndex + 1)) * enemySpotData.textureHeight  + enemySpotData.textureYPos) / (float)enemySpotData.atlasHeight;
			}

			CIwSVec2 enemySpotDimension = TextureHelper::GetDimension(TextureHelper::GetRenderData(enemySpotData));

			int currFade = 0;
			if(enemySpotFadeIn)
			{
				currFade = (int)((255 * enemySpotBlinkTime.GetCurrTime()) / (float)(enemySpotBlinkTime.GetTimeLimit()));
				if(currFade > 255) currFade = 255;
			}
			else
			{
				currFade = (int)(255 * (1 - (enemySpotBlinkTime.GetCurrTime() / (float)enemySpotBlinkTime.GetTimeLimit())));
				if(currFade < 0) currFade = 0;
			}

			if(enemySpotBlinkTime.GetIsEnd() || enemySpotBlinkTime.GetIsStop())
			{
				enemySpotFadeIn = !enemySpotFadeIn;
				enemySpotBlinkTime.Reset();
				enemySpotBlinkTime.Start();
			}

			if(enemySpotData.textureRotated)
			{
				DrawQuadBatch(48.0f * globalScale, 40.0f * globalScale, 
					CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
					uvMinY, uvMaxY, uvMinX, uvMaxX,
					-22.0f * globalScale, -20.0f * globalScale, 255, 255, 255, currFade, enemySpotData.textureRotated);
			}
			else
			{
				DrawQuadBatch(48.0f * globalScale, 40.0f * globalScale, 
					CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
					uvMinX, uvMaxX, uvMinY, uvMaxY,
					-22.0f * globalScale, -20.0f * globalScale, 255, 255, 255, currFade, enemySpotData.textureRotated);
			}
		}
	}

	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(s_ColorsBatch);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();
}

void CGame::RenderIsoPawnTexture(CIwTexture* texture, IsoPawn* pawn)
{
	numImagesBatched = 0;

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(texture);
	IwGxSetMaterial(pMat);
		
	int r = pawn->getTextureR();
	int g = pawn->getTextureG();
	int b = pawn->getTextureB();
	int a = pawn->getTextureAlpha();

	if(pawn->PlayingTakeDamageEffect() && !pawn->IsDebuffed())
	{
		r = 255;
		g = b = 125;
		a = 255;
	}

	DrawQuadBatch(pawn->getWidth()   , pawn->getHeight(),  pawn->getPosX()  ,   pawn->getPosY(),     pawn->getPosZ(),
		          pawn->getUvMinX()  , pawn->getUvMaxX(),  pawn->getUvMinY(),   pawn->getUvMaxY(),
		          pawn->getOffsetX() , pawn->getOffsetY(), r, g, b, a);

	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(s_ColorsBatch, numImagesBatched * 4);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();
}

void CGame::RenderIsoPawnsOnScreen()
{
	int size;
	Skill_Type list[5];
	IsoEnemy* CurrentIsoEnemy = NULL;

	for(int i = 0; i < currentIsoPawn; i++)
	{
		if(isoPawnRenderContainer[i].isoEnemy != NULL)
		{
			IsoEnemyType CurrentEnemyType = isoPawnRenderContainer[i].isoEnemy->GetEnemyType();

			if(CurrentEnemyType == ENEMY_None)
				continue;
			
			if(CurrentEnemyType != ENEMY_Captain)
			{
				RenderIsoPawnTexture(isoPawnRenderContainer[i].isoEnemy->GetTexture(), isoPawnRenderContainer[i].isoEnemy);
			}

			CurrentIsoEnemy = isoPawnRenderContainer[i].isoEnemy;

			CurrentIsoEnemy->GetDebuffList(list, &size);
			for(int i = 0; i < size; i++)
			{
				if(list[i] == Skill_Type_Root)
				{
					float offset_x = -32.0f * globalScale;
					float offset_y = -16.0f * globalScale;
					float uvMinX = rootRenderData.uvInfo.minX;
					float uvMaxX = rootRenderData.uvInfo.maxX;
					float uvMinY = rootRenderData.uvInfo.minY;
					float uvMaxY = rootRenderData.uvInfo.maxY;
					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(rootRenderData.atlasName));
					Utils::DrawQuadBatch((float)(rootRenderData.width * globalScale), (float)(rootRenderData.height * globalScale), 
						CurrentIsoEnemy->getPosX(), CurrentIsoEnemy->getPosY(), CurrentIsoEnemy->getPosZ(),
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();
				}
				else if(list[i] == Skill_Type_Confusion)
				{
					int idx = CurrentIsoEnemy->GetIsoConfusionIndex() % 5;
					int idy = CurrentIsoEnemy->GetIsoConfusionIndex() / 5;

					float offset_x = -16.0f * globalScale;
					float offset_y = (-16.0f - 48.0f) * globalScale;
					float uvMinX = 0.2f * idx;
					float uvMaxX = 0.2f * (idx + 1);
					float uvMinY = 0.5f * idy;
					float uvMaxY = 0.5f * (idy + 1);

					uvMinX = (disorientedData.textureXPos + uvMinX * 160.0f) / (float)disorientedData.atlasWidth;
					uvMaxX = (disorientedData.textureXPos + uvMaxX * 160.0f) / (float)disorientedData.atlasWidth;
					uvMinY = (disorientedData.textureYPos + uvMinY * 64.0f) / (float)disorientedData.atlasHeight;
					uvMaxY = (disorientedData.textureYPos + uvMaxY * 64.0f) / (float)disorientedData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(disorientedName));
					Utils::DrawQuadBatch(32.0f * globalScale, 32.0f * globalScale, 
						CurrentIsoEnemy->getPosX(), CurrentIsoEnemy->getPosY(), CurrentIsoEnemy->getPosZ(),
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();
				}
				else if(list[i] == Skill_Type_Stun)
				{
					int idx = CurrentIsoEnemy->GetIsoStunIndex() % 5;
					int idy = CurrentIsoEnemy->GetIsoStunIndex() / 5;

					float offset_x = -32.0f * globalScale;
					float offset_y = (-8.0f - 36.0f) * globalScale;
					float uvMinX = 0.2f * idx;
					float uvMaxX = 0.2f * (idx + 1);
					float uvMinY = 0.5f * idy;
					float uvMaxY = 0.5f * (idy + 1);

					uvMinX = (stunData.textureXPos + uvMinX * 320.0f) / (float)stunData.atlasWidth;
					uvMaxX = (stunData.textureXPos + uvMaxX * 320.0f) / (float)stunData.atlasWidth;
					uvMinY = (stunData.textureYPos + uvMinY * 32.0f) / (float)stunData.atlasHeight;
					uvMaxY = (stunData.textureYPos + uvMaxY * 32.0f) / (float)stunData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(stunName));
					Utils::DrawQuadBatch(64.0f * globalScale, 16.0f * globalScale, 
						CurrentIsoEnemy->getPosX(), CurrentIsoEnemy->getPosY(), CurrentIsoEnemy->getPosZ(),
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();
				}
			}

			if(CurrentIsoEnemy->GetCoinIndex() != -1)
			{
				if(isoCoin[CurrentIsoEnemy->GetCoinIndex()].index != -1)
				{
					int _idx = isoCoin[CurrentIsoEnemy->GetCoinIndex()].index % 5;
					int _idy = isoCoin[CurrentIsoEnemy->GetCoinIndex()].index / 5;

					float offset_x = (-16.0f) * globalScale;
					float offset_y = (-40.0f - 16.0f) * globalScale;
					float uvMinX = 0.2f * _idx;
					float uvMaxX = 0.2f * (_idx + 1);
					float uvMinY = 0.333f * _idy;
					float uvMaxY = 0.333f * (_idy + 1);

					uvMinX = (coinsData.textureXPos + uvMinX * 160.0f) / (float)coinsData.atlasWidth;
					uvMaxX = (coinsData.textureXPos + uvMaxX * 160.0f) / (float)coinsData.atlasWidth;
					uvMinY = (coinsData.textureYPos + uvMinY * 240.0f) / (float)coinsData.atlasHeight;
					uvMaxY = (coinsData.textureYPos + uvMaxY * 240.0f) / (float)coinsData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(coinsName));
					Utils::DrawQuadBatch(32.0f * globalScale, 80.0f * globalScale, 
						isoCoin[CurrentIsoEnemy->GetCoinIndex()]._x, isoCoin[CurrentIsoEnemy->GetCoinIndex()]._y, isoCoin[CurrentIsoEnemy->GetCoinIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();

					if(isoCoin[CurrentIsoEnemy->GetCoinIndex()].timer.GetIsEnd())
					{
						isoCoin[CurrentIsoEnemy->GetCoinIndex()].index++;	
						isoCoin[CurrentIsoEnemy->GetCoinIndex()].timer.Reset();
						isoCoin[CurrentIsoEnemy->GetCoinIndex()].timer.Start();
					}

					if(isoCoin[CurrentIsoEnemy->GetCoinIndex()].index > 14)
					{
						isoCoin[CurrentIsoEnemy->GetCoinIndex()].index = -1;
						isoCoin[CurrentIsoEnemy->GetCoinIndex()].timer.Stop();
						CurrentIsoEnemy->SetCoinIndex(-1);
					}
				}
			}
		}

		if(isoPawnRenderContainer[i].isoSelknam != NULL)
		{
			IsoSelknamType CurrentSelknamType = isoPawnRenderContainer[i].isoSelknam->GetSelknamType();

			if(CurrentSelknamType == SELKNAM_None)
				continue;

			IsoSelknam* CurrentIsoSelknam = isoPawnRenderContainer[i].isoSelknam;
			RenderIsoPawnTexture(CurrentIsoSelknam->GetTexture(), CurrentIsoSelknam);

			if(CurrentIsoSelknam->GetCoinIndex() != -1)
			{
				if(isoCoin[CurrentIsoSelknam->GetCoinIndex()].index != -1)
				{
					int _idx = isoCoin[CurrentIsoSelknam->GetCoinIndex()].index % 5;
					int _idy = isoCoin[CurrentIsoSelknam->GetCoinIndex()].index / 5;

					float offset_x = (-16.0f) * globalScale;
					float offset_y = (-40.0f - 16.0f) * globalScale;
					float uvMinX = 0.2f * _idx;
					float uvMaxX = 0.2f * (_idx + 1);
					float uvMinY = 0.333f * _idy;
					float uvMaxY = 0.333f * (_idy + 1);

					uvMinX = (coinsData.textureXPos + uvMinX * 160.0f) / (float)coinsData.atlasWidth;
					uvMaxX = (coinsData.textureXPos + uvMaxX * 160.0f) / (float)coinsData.atlasWidth;
					uvMinY = (coinsData.textureYPos + uvMinY * 240.0f) / (float)coinsData.atlasHeight;
					uvMaxY = (coinsData.textureYPos + uvMaxY * 240.0f) / (float)coinsData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(coinsName));
					Utils::DrawQuadBatch(32.0f * globalScale, 80.0f * globalScale, 
						isoCoin[CurrentIsoSelknam->GetCoinIndex()]._x, isoCoin[CurrentIsoSelknam->GetCoinIndex()]._y, isoCoin[CurrentIsoSelknam->GetCoinIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();

					if(isoCoin[CurrentIsoSelknam->GetCoinIndex()].timer.GetIsEnd())
					{
						isoCoin[CurrentIsoSelknam->GetCoinIndex()].index++;	
						isoCoin[CurrentIsoSelknam->GetCoinIndex()].timer.Reset();
						isoCoin[CurrentIsoSelknam->GetCoinIndex()].timer.Start();
					}

					if(isoCoin[CurrentIsoSelknam->GetCoinIndex()].index > 14)
					{
						isoCoin[CurrentIsoSelknam->GetCoinIndex()].index = -1;
						isoCoin[CurrentIsoSelknam->GetCoinIndex()].timer.Stop();
						CurrentIsoSelknam->SetCoinIndex(-1);
					}
				}
			}

			if(CurrentIsoSelknam->GetIsoHealIndex() != -1)
			{
				if(isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index != -1)
				{
					int _idx = isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index % 4;
					int _idy = isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index / 4;

					float offset_x = -32.0f * globalScale;
					float offset_y = -80.0f * globalScale;
					float uvMinX = 0.25f * _idx;
					float uvMaxX = 0.25f * (_idx + 1);
					float uvMinY = 0.25f * _idy;
					float uvMaxY = 0.25f * (_idy + 1);

					uvMinX = (healData.textureXPos + uvMinX * 256.0f) / (float)healData.atlasWidth;
					uvMaxX = (healData.textureXPos + uvMaxX * 256.0f) / (float)healData.atlasWidth;
					uvMinY = (healData.textureYPos + uvMinY * 512.0f) / (float)healData.atlasHeight;
					uvMaxY = (healData.textureYPos + uvMaxY * 512.0f) / (float)healData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(healName));
					Utils::DrawQuadBatch(64.0f * globalScale, 128.0f * globalScale, 
						isoHeal[CurrentIsoSelknam->GetIsoHealIndex()]._x, isoHeal[CurrentIsoSelknam->GetIsoHealIndex()]._y, isoHeal[CurrentIsoSelknam->GetIsoHealIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();

					if(isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].timer.GetIsEnd())
					{
						isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index++;
						isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].timer.Reset();
						isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].timer.Start();
					}

					if(isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index > 14)
					{
						isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index = -1;
						isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].timer.Stop();
						CurrentIsoSelknam->SetIsoHealIndex(-1);
					}
				}
			}

			if(CurrentIsoSelknam->GetIsoMightIndex() != -1)
			{
				if(isoMight[CurrentIsoSelknam->GetIsoMightIndex()].index != -1)
				{
					int _idx = isoMight[CurrentIsoSelknam->GetIsoMightIndex()].index % 4;
					int _idy = isoMight[CurrentIsoSelknam->GetIsoMightIndex()].index / 4;

					float offset_x = -32.0f * globalScale;
					float offset_y = -80.0f * globalScale;
					float uvMinX = 0.25f * _idx;
					float uvMaxX = 0.25f * (_idx + 1);
					float uvMinY = 0.25f * _idy;
					float uvMaxY = 0.25f * (_idy + 1);
					uvMinX = (healData.textureXPos + uvMinX * 256.0f) / (float)healData.atlasWidth;
					uvMaxX = (healData.textureXPos + uvMaxX * 256.0f) / (float)healData.atlasWidth;
					uvMinY = (healData.textureYPos + uvMinY * 512.0f) / (float)healData.atlasHeight;
					uvMaxY = (healData.textureYPos + uvMaxY * 512.0f) / (float)healData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(healName));
					Utils::DrawQuadBatch(64.0f * globalScale, 128.0f * globalScale, 
						isoMight[CurrentIsoSelknam->GetIsoMightIndex()]._x, isoMight[CurrentIsoSelknam->GetIsoMightIndex()]._y, isoMight[CurrentIsoSelknam->GetIsoMightIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y,
						225, 22, 22);
					Utils::RenderQuadBatch();

					if(isoMight[CurrentIsoSelknam->GetIsoMightIndex()].timer.GetIsEnd())
					{
						isoMight[CurrentIsoSelknam->GetIsoMightIndex()].index++;
						isoMight[CurrentIsoSelknam->GetIsoMightIndex()].timer.Reset();
						isoMight[CurrentIsoSelknam->GetIsoMightIndex()].timer.Start();
					}

					if(isoMight[CurrentIsoSelknam->GetIsoMightIndex()].index > 14)
					{
						isoMight[CurrentIsoSelknam->GetIsoMightIndex()].index = -1;
						isoMight[CurrentIsoSelknam->GetIsoMightIndex()].timer.Stop();
						CurrentIsoSelknam->SetIsoMightIndex(-1);
					}
				}
			}

			if(CurrentIsoSelknam->GetIsoHasteIndex() != -1)
			{
				if(isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].index != -1)
				{
					int _idx = isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].index % 4;
					int _idy = isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].index / 4;

					float offset_x = -32.0f * globalScale;
					float offset_y = -80.0f * globalScale;
					float uvMinX = 0.25f * _idx;
					float uvMaxX = 0.25f * (_idx + 1);
					float uvMinY = 0.25f * _idy;
					float uvMaxY = 0.25f * (_idy + 1);
					uvMinX = (healData.textureXPos + uvMinX * 256.0f) / (float)healData.atlasWidth;
					uvMaxX = (healData.textureXPos + uvMaxX * 256.0f) / (float)healData.atlasWidth;
					uvMinY = (healData.textureYPos + uvMinY * 512.0f) / (float)healData.atlasHeight;
					uvMaxY = (healData.textureYPos + uvMaxY * 512.0f) / (float)healData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(healName));
					Utils::DrawQuadBatch(64.0f * globalScale, 128.0f * globalScale, 
						isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()]._x, isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()]._y, isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y,
						1, 138, 243);
					Utils::RenderQuadBatch();

					if(isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].timer.GetIsEnd())
					{
						isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].index++;
						isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].timer.Reset();
						isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].timer.Start();
					}

					if(isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].index > 14)
					{
						isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].index = -1;
						isoHaste[CurrentIsoSelknam->GetIsoHasteIndex()].timer.Stop();
						CurrentIsoSelknam->SetIsoHasteIndex(-1);
					}
				}
			}

			if(CurrentIsoSelknam->GetIsoExplotionIndex() != -1)
			{
				if(isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].index != -1)
				{
					int _idx = isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].index % 5;
					int _idy = isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].index / 5;

					float offset_x = -32.0f * globalScale;
					float offset_y = -40.0f * globalScale;
					float uvMinX = 0.2f * _idx;
					float uvMaxX = 0.2f * (_idx + 1);
					float uvMinY = 0.1667f * _idy;
					float uvMaxY = 0.1667f * (_idy + 1);

					uvMinX = (expData.textureXPos + uvMinX * 320.0f) / (float)expData.atlasWidth;
					uvMaxX = (expData.textureXPos + uvMaxX * 320.0f) / (float)expData.atlasWidth;
					uvMinY = (expData.textureYPos + uvMinY * 480.0f) / (float)expData.atlasHeight;
					uvMaxY = (expData.textureYPos + uvMaxY * 480.0f) / (float)expData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(explosionName));
					Utils::DrawQuadBatch(64.0f * globalScale, 80.0f * globalScale, 
						isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()]._x, isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()]._y, isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();

					if(isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].timer.GetIsEnd())
					{
						isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].index++;
						isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].timer.Reset();
						isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].timer.Start();
					}

					if(isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].index >= 23)
					{
						isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].index = -1;
						isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].timer.Stop();
						CurrentIsoSelknam->SetIsoExplotionIndex(-1);
					}
				}
			}

			if(CurrentIsoSelknam->GetIsoPoisonOnDeathIndex() != -1)
			{
				if(isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index != -1)
				{
					int _idx = isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index % 5;
					int _idy = isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index / 5;

					float offset_x = -32.0f * globalScale;
					float offset_y = -40.0f * globalScale;
					float uvMinX = 0.2f * _idx;
					float uvMaxX = 0.2f * (_idx + 1);
					float uvMinY = 0.1667f * _idy;
					float uvMaxY = 0.1667f * (_idy + 1);
					uvMinX = (poisonData.textureXPos + uvMinX * 320.0f) / (float)poisonData.atlasWidth;
					uvMaxX = (poisonData.textureXPos + uvMaxX * 320.0f) / (float)poisonData.atlasWidth;
					uvMinY = (poisonData.textureYPos + uvMinY * 480.0f) / (float)poisonData.atlasHeight;
					uvMaxY = (poisonData.textureYPos + uvMaxY * 480.0f) / (float)poisonData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(poisonName));
					Utils::DrawQuadBatch(64.0f * globalScale, 80.0f * globalScale, 
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()]._x, isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()]._y, isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();

					//IwTrace(BOSSBATTLE, ("IsoPoisonOnDeath Index: %d", isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index));

					if(isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.GetIsEnd())
					{
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index++;
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.Reset();
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.Start();
					}


					if(isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index >= 23)
					{
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index = -1;
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.Stop();
						CurrentIsoSelknam->SetIsoPoisonOnDeathIndex(-1);
					}
				}
			}

			continue;
		}

		if(isoPawnRenderContainer[i].isoCoin != NULL)
		{
			CoinEffect* CurrentCoinEffect = isoPawnRenderContainer[i].isoCoin;

			if(CurrentCoinEffect->index != -1)
			{
				int _idx = CurrentCoinEffect->index % 5;
				int _idy = CurrentCoinEffect->index / 5;

				float offset_x = (-16.0f) * globalScale;
				float offset_y = (-40.0f - 16.0f) * globalScale;
				float uvMinX = 0.2f * _idx;
				float uvMaxX = 0.2f * (_idx + 1);
				float uvMinY = 0.333f * _idy;
				float uvMaxY = 0.333f * (_idy + 1);

				uvMinX = (coinsData.textureXPos + uvMinX * 160.0f) / (float)coinsData.atlasWidth;
				uvMaxX = (coinsData.textureXPos + uvMaxX * 160.0f) / (float)coinsData.atlasWidth;
				uvMinY = (coinsData.textureYPos + uvMinY * 240.0f) / (float)coinsData.atlasHeight;
				uvMaxY = (coinsData.textureYPos + uvMaxY * 240.0f) / (float)coinsData.atlasHeight;

				Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(coinsName));
				Utils::DrawQuadBatch(32.0f * globalScale, 80.0f * globalScale, 
					CurrentCoinEffect->_x, CurrentCoinEffect->_y, CurrentCoinEffect->_z,
					uvMinX, 
					uvMaxX, 
					uvMinY,
					uvMaxY,
					offset_x, 
					offset_y);
				Utils::RenderQuadBatch();

				if(CurrentCoinEffect->timer.GetIsEnd())
				{
					CurrentCoinEffect->index++;	
					CurrentCoinEffect->timer.Reset();
					CurrentCoinEffect->timer.Start();
				}

				if(CurrentCoinEffect->index > 14)
				{
					CurrentCoinEffect->index = -1;
					CurrentCoinEffect->timer.Stop();
				}
			}
		}

		if(isoPawnRenderContainer[i].isoProp != NULL)
		{
			int curr_num_prop;

			if(isoPawnRenderContainer[i].isoProp->getPropNum() >= 31 && isoPawnRenderContainer[i].isoProp->getPropNum() <= 39)
				curr_num_prop = 0; //64x64
			else if(isoPawnRenderContainer[i].isoProp->getPropNum() >= 40 && isoPawnRenderContainer[i].isoProp->getPropNum() <= 45)
				curr_num_prop = 1; //64x92
			else if(isoPawnRenderContainer[i].isoProp->getPropNum() >= 46 && isoPawnRenderContainer[i].isoProp->getPropNum() <= 51)
				curr_num_prop = 2; //64x128
			else
				continue;

			CIwMaterial* pMat = NULL;
			numImagesBatched = 0;

			if(curr_num_prop == 0)
			{
				pMat = IW_GX_ALLOC_MATERIAL();
				pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
				pMat->SetTexture(prop64_64);
				IwGxSetMaterial(pMat);
			}
			else if(curr_num_prop == 1)
			{
				pMat = IW_GX_ALLOC_MATERIAL();
				pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
				pMat->SetTexture(prop64_96);
				IwGxSetMaterial(pMat);
			}
			else if(curr_num_prop == 2)
			{
				pMat = IW_GX_ALLOC_MATERIAL();
				pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
				pMat->SetTexture(prop64_128);
				IwGxSetMaterial(pMat);
			}
			
			IsoProp* CurrentIsoSelknam = isoPawnRenderContainer[i].isoProp;
			DrawQuadBatch(CurrentIsoSelknam->getWidth(), CurrentIsoSelknam->getHeight(), CurrentIsoSelknam->getPosX()  , CurrentIsoSelknam->getPosY(), CurrentIsoSelknam->getPosZ(),
				CurrentIsoSelknam->getUvMinX()  , CurrentIsoSelknam->getUvMaxX(), CurrentIsoSelknam->getUvMinY(), CurrentIsoSelknam->getUvMaxY(),
				CurrentIsoSelknam->getOffsetX() , CurrentIsoSelknam->getOffsetY());

			IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
			IwGxSetColStream(s_ColorsBatch, numImagesBatched * 4);
			IwGxSetUVStream(s_UVsBatch);
			IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
			IwGxFlush();

			continue;
		}

		if(isoPawnRenderContainer[i].isoProjectile != NULL)
		{
			IsoProjectile* CurrentIsoProjectile = isoPawnRenderContainer[i].isoProjectile;
			int curr_num_projectile;

			if(CurrentIsoProjectile->GetType() == Projectile_Arrow)
				curr_num_projectile = 0;
			else if(CurrentIsoProjectile->GetType() == Projectile_Rock)
				curr_num_projectile = 1;
			else if(CurrentIsoProjectile->GetType() == Projectile_Cannon)
				curr_num_projectile = 2;
			else 
				continue;

			CIwMaterial* pMat = NULL;

			if(curr_num_projectile == 0)
			{
				pMat = IW_GX_ALLOC_MATERIAL();
				pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
				pMat->SetTexture(TextureHelper::GetCIwTexture(arrowProjectileName));
				IwGxSetMaterial(pMat);
			}
			else if(curr_num_projectile == 1)
			{
				pMat = IW_GX_ALLOC_MATERIAL();
				pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
				pMat->SetTexture(TextureHelper::GetCIwTexture(rockProjectileName));
				IwGxSetMaterial(pMat);
			}
			else if(curr_num_projectile == 2)
			{
				pMat = IW_GX_ALLOC_MATERIAL();
				pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
				pMat->SetTexture(TextureHelper::GetCIwTexture(cannonProjectileName));
				IwGxSetMaterial(pMat);
			}

			numImagesBatched = 0;
			CurrentIsoProjectile->RenderQuad(s_QuadBatch, s_UVsBatch, &numImagesBatched);
			IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
			IwGxSetColStream(s_ColorsBatch, numImagesBatched * 4);
			IwGxSetUVStream(s_UVsBatch);
			IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
			IwGxFlush();
			continue;
		}

		if(isoPawnRenderContainer[i].isoExplotion != NULL)
		{
			ExplotionEffect* effect = isoPawnRenderContainer[i].isoExplotion;

			if(effect->index == -1)
			{
				continue;
			}	

			int _idx = effect->index % 5;
			int _idy = effect->index / 5;

			float offset_x = -32.0f * globalScale;
			float offset_y = -40.0f * globalScale - 10.0f;
			float uvMinX = 0.2f * _idx;
			float uvMaxX = 0.2f * (_idx + 1);
			float uvMinY = 0.1667f * _idy;
			float uvMaxY = 0.1667f * (_idy + 1);

			uvMinX = (expData.textureXPos + uvMinX * 320.0f) / (float)expData.atlasWidth;
			uvMaxX = (expData.textureXPos + uvMaxX * 320.0f) / (float)expData.atlasWidth;
			uvMinY = (expData.textureYPos + uvMinY * 480.0f) / (float)expData.atlasHeight;
			uvMaxY = (expData.textureYPos + uvMaxY * 480.0f) / (float)expData.atlasHeight;

			Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(explosionName));
			Utils::DrawQuadBatch(64.0f * globalScale, 80.0f * globalScale, 
				effect->_x, effect->_y, effect->_z,
				uvMinX, 
				uvMaxX, 
				uvMinY,
				uvMaxY,
				offset_x, 
				offset_y);
			Utils::RenderQuadBatch();

			if(effect->timer.GetIsEnd())
			{
				effect->index++;
				effect->timer.Reset();
				effect->timer.Start();
			}

			if(effect->index > 23)
			{
				effect->index = -1;
				effect->bRender = false;
				effect->timer.Stop();
			}

			IwGxFlush();

			continue;
		}

		if(isoPawnRenderContainer[i].isoPoison != NULL)
		{
			PoisonOnDeathEffect* effect = isoPawnRenderContainer[i].isoPoison;

			if(effect->index == -1)
			{
				continue;
			}

			int _idx = effect->index % 5;
			int _idy = effect->index / 5;

			float offset_x = -32.0f * globalScale;
			float offset_y = -40.0f * globalScale - 10.0f;
			float uvMinX = 0.2f * _idx;
			float uvMaxX = 0.2f * (_idx + 1);
			float uvMinY = 0.1667f * _idy;
			float uvMaxY = 0.1667f * (_idy + 1);
			uvMinX = (poisonData.textureXPos + uvMinX * 320.0f) / (float)poisonData.atlasWidth;
			uvMaxX = (poisonData.textureXPos + uvMaxX * 320.0f) / (float)poisonData.atlasWidth;
			uvMinY = (poisonData.textureYPos + uvMinY * 480.0f) / (float)poisonData.atlasHeight;
			uvMaxY = (poisonData.textureYPos + uvMaxY * 480.0f) / (float)poisonData.atlasHeight;

			Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(poisonName));
			Utils::DrawQuadBatch(64.0f * globalScale, 80.0f * globalScale, 
				effect->_x, effect->_y, effect->_z,
				uvMinX, 
				uvMaxX, 
				uvMinY,
				uvMaxY,
				offset_x, 
				offset_y);
			Utils::RenderQuadBatch();

			if(effect->timer.GetIsEnd())
			{
				effect->index++;
				effect->timer.Reset();
				effect->timer.Start();
			}

			if(effect->index > 23)
			{
				effect->index = -1;
				effect->bRender = false;
				effect->timer.Stop();
			}

			IwGxFlush();

			continue;
		}
	}

	IwGxFlush();

	//SOLD SELKNAM COINS.
	/*
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(inTerrainIsoCoin[i].index != -1)
		{
			int _idx = inTerrainIsoCoin[i].index % 5;
			int _idy = inTerrainIsoCoin[i].index / 5;

			float offset_x = (-16.0f) * globalScale;
			float offset_y = (-40.0f - 16.0f) * globalScale;
			float uvMinX = 0.2f * _idx;
			float uvMaxX = 0.2f * (_idx + 1);
			float uvMinY = 0.333f * _idy;
			float uvMaxY = 0.333f * (_idy + 1);

			uvMinX = (coinsData.textureXPos + uvMinX * 160.0f) / (float)coinsData.atlasWidth;
			uvMaxX = (coinsData.textureXPos + uvMaxX * 160.0f) / (float)coinsData.atlasWidth;
			uvMinY = (coinsData.textureYPos + uvMinY * 240.0f) / (float)coinsData.atlasHeight;
			uvMaxY = (coinsData.textureYPos + uvMaxY * 240.0f) / (float)coinsData.atlasHeight;

			Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(coinsName));
			Utils::DrawQuadBatch(32.0f * globalScale, 80.0f * globalScale, 
				inTerrainIsoCoin[i]._x, inTerrainIsoCoin[i]._y, inTerrainIsoCoin[i]._z,
				uvMinX, 
				uvMaxX, 
				uvMinY,
				uvMaxY,
				offset_x, 
				offset_y);
			Utils::RenderQuadBatch();

			if(inTerrainIsoCoin[i].timer.GetIsEnd())
			{
				inTerrainIsoCoin[i].index++;	
				inTerrainIsoCoin[i].timer.Reset();
				inTerrainIsoCoin[i].timer.Start();
			}

			if(inTerrainIsoCoin[i].index > 14)
			{
				inTerrainIsoCoin[i].index = -1;
				inTerrainIsoCoin[i].timer.Stop();
			}
		}
	}
	*/
}

void CGame::RenderGameState_GSG_Attack()
{
	//render loop attack
	if(bRenderAttackText)
	{
		defense_stage_back->Render();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(graphic_defense_stage.atlasName), ingameMessagePos, &graphic_defense_stage);
		IwGxFlush();
	}
}

void CGame::RenderGameState_GSG_Defending()
{
}

void CGame::RenderProjectiles()
{
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
		if(isoProjectileArr[i].GetType() != Projectile_None)
			isoProjectileArr[i].RenderImpact();
}

void CGame::DrawQuad(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX, float _uvMaxX, float _uvMinY, float _uvMaxY, float _offsetX, float _offsetY)
{
	// Allocate a material from the IwGx global cache
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	// Set this as the active material
	pMat->SetTexture(s_Texture);
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(pMat);

	_posX += _offsetX; 
	_posY += _offsetY;

	s_Quad[0].x = _posX;
	s_Quad[0].y = _posY;
	s_Quad[0].z = _posZ;

	s_Quad[1].x = _posX;
	s_Quad[1].y = _posY + _height;
	s_Quad[1].z = _posZ;

	s_Quad[2].x = _posX + _width;
	s_Quad[2].y = _posY + _height;
	s_Quad[2].z = _posZ;

	s_Quad[3].x = _posX + _width;
	s_Quad[3].y = _posY;
	s_Quad[3].z = _posZ;

	IwGxSetVertStreamModelSpace(s_Quad, 4);	

	IwGxSetColStream(NULL);
	
	s_UVs[0] = CIwFVec2(_uvMinX, _uvMinY);
	s_UVs[1] = CIwFVec2(_uvMinX, _uvMaxY);
	s_UVs[2] = CIwFVec2(_uvMaxX, _uvMaxY);
	s_UVs[3] = CIwFVec2(_uvMaxX, _uvMinY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

}

void CGame::DrawQuadBatch(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX, float _uvMaxX, float _uvMinY, float _uvMaxY, float _offsetX, float _offsetY, int r, int g, int b, int alpha, bool bRotate)
{
	int index = numImagesBatched * 4;

	_posX += _offsetX; 
	_posY += _offsetY;

	if(!bRotate)
	{
		s_QuadBatch[index].x = _posX;
		s_QuadBatch[index].y = _posY;
		s_QuadBatch[index].z = _posZ;

		s_QuadBatch[index + 1].x = _posX;
		s_QuadBatch[index + 1].y = _posY + _height;
		s_QuadBatch[index + 1].z = _posZ;

		s_QuadBatch[index + 2].x = _posX + _width;
		s_QuadBatch[index + 2].y = _posY + _height;
		s_QuadBatch[index + 2].z = _posZ;

		s_QuadBatch[index + 3].x = _posX + _width;
		s_QuadBatch[index + 3].y = _posY;
		s_QuadBatch[index + 3].z = _posZ;
	}
	else
	{
		s_QuadBatch[index + 3].x = _posX;
		s_QuadBatch[index + 3].y = _posY;
		s_QuadBatch[index + 3].z = _posZ;

		s_QuadBatch[index].x = _posX;
		s_QuadBatch[index].y = _posY + _height;
		s_QuadBatch[index].z = _posZ;

		s_QuadBatch[index + 1].x = _posX + _width;
		s_QuadBatch[index + 1].y = _posY + _height;
		s_QuadBatch[index + 1].z = _posZ;

		s_QuadBatch[index + 2].x = _posX + _width;
		s_QuadBatch[index + 2].y = _posY;
		s_QuadBatch[index + 2].z = _posZ;
	}

	s_UVsBatch[index] = CIwFVec2(_uvMinX, _uvMinY);
	s_UVsBatch[index + 1] = CIwFVec2(_uvMinX, _uvMaxY);
	s_UVsBatch[index + 2] = CIwFVec2(_uvMaxX, _uvMaxY);
	s_UVsBatch[index + 3] = CIwFVec2(_uvMaxX, _uvMinY);
	
	s_ColorsBatch[index].Set(r, g, b, alpha);
	s_ColorsBatch[index + 1].Set(r, g, b, alpha);
	s_ColorsBatch[index + 2].Set(r, g, b, alpha);
	s_ColorsBatch[index + 3].Set(r, g, b, alpha);

	numImagesBatched++;
}

void CGame::DrawColorQuad(float _width, float _height, float _posX, float _posY, float _posZ, uint32 color, float _offsetX, float _offsetY)
{
	int index = numImagesBatched * 4;

	_posX += _offsetX; 
	_posY += _offsetY;

	s_QuadBatch[index].x = _posX;
	s_QuadBatch[index].y = _posY;
	s_QuadBatch[index].z = _posZ;

	s_QuadBatch[index + 1].x = _posX;
	s_QuadBatch[index + 1].y = _posY + _height;
	s_QuadBatch[index + 1].z = _posZ;

	s_QuadBatch[index + 2].x = _posX + _width;
	s_QuadBatch[index + 2].y = _posY + _height;
	s_QuadBatch[index + 2].z = _posZ;

	s_QuadBatch[index + 3].x = _posX + _width;
	s_QuadBatch[index + 3].y = _posY;
	s_QuadBatch[index + 3].z = _posZ;

	s_ColorsBatch[index].Set(color);
	s_ColorsBatch[index + 1].Set(color);
	s_ColorsBatch[index + 2].Set(color);
	s_ColorsBatch[index + 3].Set(color);

	numImagesBatched++;
}
#pragma endregion RENDER_GAME

//Transformation matrix
/*
xTile = mouseX * 1/(2*w) + mouseY * 1/(2h) - (f_x * h + f_y * w) / 2wh
yTile = mouseY * -1/(2*w) + mouseY * 1/(2h) + (f_x * h - f_y * w) / 2wh
w = Half of the width of a single tile
h = Half of the height of a single tile
f_x = tile (0,0) x coord
f_y = tile (0,0) y coord
*/
void CGame::UpdateMotionEvents(s3ePointerMotionEvent* event)
{

	if(bPauseGame)// || dialogController->IsActive())
		return;
	/*
	if(bPause)
		return;
		*/
	unitMenu.UpdateMotionEvents(event);
	unitManager.UpdateMotionEvents(event);
	mapPointer.UpdateMotionEvents(event);
	nextWaveButton_x1->HandleMotion(event);
	nextWaveButton_x2->HandleMotion(event);
	nextWaveButton_x3->HandleMotion(event);
	pauseMenuButton->HandleMotion(event);
	almanacButton->HandleMotion(event);
}

void CGame::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bPauseGame || dialogController->IsActive() || (dynamicCredits->GetIsActive() && !dynamicCredits->EasterEggActive()))
		return;

	if(bPause)
		return;

	if(event->m_Button == S3E_POINTER_BUTTON_RIGHTMOUSE)
	{
		if(event->m_Pressed)
		{
			if(unitManager.MovingSelknam())
				unitManager.Close();
		}

		return;
	}

	if(bCheatModeEnabled
	   && event->m_Pressed
	   && Utils::MouseOn(event->m_x, event->m_y, 0, 0, 50, 50))
	{
		inGameState = GSG_Win;
	}

	nextWaveButton_x1->HandleClick(event);
	nextWaveButton_x2->HandleClick(event);
	nextWaveButton_x3->HandleClick(event);
	
	if(!dynamicCredits->GetIsActive())
	{
		pauseMenuButton->HandleClick(event);
		almanacButton->HandleClick(event);
	}

	if(!nextWaveButton_x1->MouseOn(event->m_x, event->m_y) && 
		!nextWaveButton_x2->MouseOn(event->m_x, event->m_y) &&
		!nextWaveButton_x3->MouseOn(event->m_x, event->m_y) &&
		!pauseMenuButton->MouseOn(event->m_x, event->m_y) &&
		!almanacButton->MouseOn(event->m_x, event->m_y))
	{
		unitMenu.UpdateClickEvents(event);
		unitManager.UpdateClickEvents(event);
		
		mapPointer.UpdateGrid(event->m_x, event->m_y);
	
		if(bCanOpenUnitMenu && !event->m_Pressed && mapPointer.GetIsMouseOnAGrid() && !bMovingCamera)
		{
			if(unitMenu.bUnitMenuRecentlyClosedByTap)
			{
				unitMenu.bUnitMenuRecentlyClosedByTap = false;
				return;
			}
			
			if(unitManager.bUnitManagerRecentlyClosedByTap)
			{
				unitManager.bUnitManagerRecentlyClosedByTap = false;
				return;
			}

			IsoTerrain* CurrentGrid = isoTerrainCursorGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()];
			
			if(!unitManager.GetIsActive() && !unitMenu.GetIsActive())
			{
				CIwVec2 Tile = mapPointer.GetTileByMouseCoords((float)event->m_x, (float)event->m_y, CurrentGrid->getLayer());
				if(Tile.x != CurrentGrid->getXNumTile() || Tile.y != CurrentGrid->getYNumTile())
					return;
			}

			if(CurrentGrid->getCanPutUnit() && (tileToSelect == NULL || tileToSelect == CurrentGrid))
			{
				tileToSelect = NULL;

				if(CurrentGrid->CurrentIsoSelknam != NULL)
				{
					if(CurrentGrid->CurrentIsoSelknam->has_skill_effect[(int)Skill_Type_CountdownDeath])
					{
						CurrentGrid->CurrentIsoSelknam->RemoveCountdownDeathDebuff();
					}
					else
					{
						if(!unitManager.GetIsActive() && !unitMenu.GetIsActive())
						{
							if(bUnitMenuAttachedToMouseWhenOpened)
							{
								CIwSVec2 menuCoords = Utils::GetAdjustedCenter2DPointForGUI(currentMousePosition, unitMenu.GetWidth(), unitMenu.GetHeight());
								unitManager.SetPos(menuCoords.x, menuCoords.y);
							}

							unitManager.Open(CurrentGrid, &mp_coins);
						}
					}
				}
				else if(!unitMenu.GetIsActive() && !unitManager.GetIsActive())
				{
					if(bUnitMenuAttachedToMouseWhenOpened)
					{
						CIwSVec2 menuCoords = Utils::GetAdjustedCenter2DPointForGUI(currentMousePosition, unitMenu.GetWidth(), unitMenu.GetHeight());
						unitMenu.SetPos(menuCoords.x, menuCoords.y);
					}

					unitMenu.Open(mp_coins);
				}
			}
			
			if(CurrentGrid->getIntentionalCantPutUnit()
			   && !unitManager.GetIsUnitMenuBlocked())
			{
				//Mark all unavailable tiles.
				for(int i = 0; i < MAX_TILESROW; i++)
				{
					for(int j = 0; j < MAX_TILESROW; j++)
					{
						if(i >= 0 
							&& j >= 0 
							&& (i != CurrentGrid->getXNumTile() || j != CurrentGrid->getYNumTile()) 
							&& i < MAX_TILESROW && j < MAX_TILESROW
							&& isoTerrainAstarGrid[i][j] != NULL
							&& isoTerrainAstarGrid[i][j]->getIntentionalCantPutUnit())
						{
							unavailableTileMap[i][j] = blinkMapTileTime * 2;
						}
					}
				}
			}
		}

		if(mapPointer.GetIsMouseOnAGrid() && !bMovingCamera && !event->m_Pressed)
		{
			if(unitMenu.GetIsActive() || unitManager.GetIsActive())
			{
				mapPointer.ShowCursor(-1);					
			}
			else
			{
				mapPointer.ShowCursor(450);
			}
		}
	}
}

//Function triggered by button pressed.
void CGame::ResolveActionEvent()
{
	if(bPauseGame || dialogController->IsActive() || (dynamicCredits->GetIsActive() && !dynamicCredits->EasterEggActive()))
		return;

	if(bPause)
		return;

	if(bCanOpenUnitMenu && !bMovingCamera)
	{
		if(unitMenu.bUnitMenuRecentlyClosedByTap)
		{
			unitMenu.bUnitMenuRecentlyClosedByTap = false;
			return;
		}

		if(unitManager.bUnitManagerRecentlyClosedByTap)
		{
			unitManager.bUnitManagerRecentlyClosedByTap = false;
			return;
		}

		IsoTerrain* CurrentGrid = isoTerrainCursorGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()];

		if(CurrentGrid->getCanPutUnit() && (tileToSelect == NULL || tileToSelect == CurrentGrid))
		{
			tileToSelect = NULL;

			if(CurrentGrid->CurrentIsoSelknam != NULL)
			{
				if(CurrentGrid->CurrentIsoSelknam->has_skill_effect[(int)Skill_Type_CountdownDeath])
				{
					CurrentGrid->CurrentIsoSelknam->RemoveCountdownDeathDebuff();
				}
				else
				{
					if(!unitManager.GetIsActive() && !unitMenu.GetIsActive())
					{
						CIwSVec2 menuCoords = Utils::GetAdjustedCenter2DPointForGUI(CIwSVec2((int)CurrentGrid->getPos2D().x - xWindowOffset, (int)CurrentGrid->getPos2D().y - yWindowOffset), unitMenu.GetWidth(), unitMenu.GetHeight());

						unitManager.SetPos(menuCoords.x, menuCoords.y);

						unitManager.Open(CurrentGrid, &mp_coins);
					}
				}
			}
			else if(!unitMenu.GetIsActive() && !unitManager.GetIsActive())
			{
				CIwSVec2 menuCoords = Utils::GetAdjustedCenter2DPointForGUI(CIwSVec2((int)CurrentGrid->getPos2D().x - xWindowOffset, (int)CurrentGrid->getPos2D().y - yWindowOffset), unitMenu.GetWidth(), unitMenu.GetHeight());

				unitMenu.SetPos(menuCoords.x, menuCoords.y);

				unitMenu.Open(mp_coins);
			}
		}

		if(CurrentGrid->getIntentionalCantPutUnit()
			&& !unitManager.GetIsUnitMenuBlocked())
		{
			//Mark all unavailable tiles.
			for(int i = 0; i < MAX_TILESROW; i++)
			{
				for(int j = 0; j < MAX_TILESROW; j++)
				{
					if(i >= 0 
						&& j >= 0 
						&& (i != CurrentGrid->getXNumTile() || j != CurrentGrid->getYNumTile()) 
						&& i < MAX_TILESROW && j < MAX_TILESROW
						&& isoTerrainAstarGrid[i][j] != NULL
						&& isoTerrainAstarGrid[i][j]->getIntentionalCantPutUnit())
					{
						unavailableTileMap[i][j] = blinkMapTileTime * 2;
					}
				}
			}
		}
	}
}

void CGame::Debug()
{
}

void CGame::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	s_Quad[0] = CIwSVec2(position.x, position.y);
	s_Quad[1] = CIwSVec2(position.x, position.y + tex->GetHeight());
	s_Quad[2] = CIwSVec2(position.x + tex->GetWidth(), position.y + tex->GetHeight());
	s_Quad[3] = CIwSVec2(position.x + tex->GetWidth(), position.y);

	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	IwGxSetColStream(NULL);

	s_UVs[0] = CIwFVec2(0, 0);
	s_UVs[1] = CIwFVec2(0, 1);
	s_UVs[2] = CIwFVec2(1, 1);
	s_UVs[3] = CIwFVec2(1, 0);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
}

void CGame::GetInitNodes()
{
	int numInitNode = 0;
	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		if(isoTerrainArr[i].getIsInitPoint())
		{
			numInitNode = isoTerrainArr[i].getInitPointNum();//number 1 - 10
			numInitNode -= 1;
			if(numInitNode >= 10)
				IwAssertMsg(APPASSERT, numInitNode >= 10, ("GetInitNodes(), Illegal value"));
			initNodes[numInitNode] = &isoTerrainArr[i];
		}
	}
}

void CGame::SpawnSelknam(IsoSelknamType type)
{
	if(type == SELKNAM_Archer_Base)	            mp_coins -= SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].cost;
	else if(type == SELKNAM_Archer_Poisoner)    mp_coins -= SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].cost;
	else if(type == SELKNAM_Archer_Retarder)    mp_coins -= SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].cost;
	else if(type == SELKNAM_Bowler_Rooter)      mp_coins -= SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].cost;
	else if(type == SELKNAM_Bowler_Stuner)      mp_coins -= SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].cost;
	else if(type == SELKNAM_Bowler_Confuser)    mp_coins -= SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].cost;
	else if(type == SELKNAM_Shaman_Haste)       mp_coins -= SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].cost;
	else if(type == SELKNAM_Shaman_Healer)      mp_coins -= SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].cost;
	else if(type == SELKNAM_Shaman_Might)       mp_coins -= SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].cost;
	else if(type == SELKNAM_Warrior_Base)       mp_coins -= SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].cost;
	else if(type == SELKNAM_Warrior_Tank)       mp_coins -= SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].cost;
	else if(type == SELKNAM_Warrior_Berserk)    mp_coins -= SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].cost;

	if(tutorial->currBreakpoint == Tutorial_BreakPoint_DeployUnit)
	{
		tutorial->ExecuteBreakpoint();
	}
	
	for(int i = 0; i < MAX_ISO_SELKNAM; i++)
	{
		if(isoSelknamArr[i].GetSelknamType() == SELKNAM_None) 
		{
			isoSelknamArr[i].InitSelknam(type);
			isoSelknamArr[i].InitIsoPawn(isoTerrainAstarGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()]);

			int xTile = mapPointer.getXNumTile();
			int yTile = mapPointer.getYNumTile();
			int range = isoSelknamArr[i].GetRange();

			for(int i = yTile - range; i <= yTile + range; i++)
			{
				for(int j = xTile - range; j <= xTile + range; j++)
				{
					if(i > 0 && j > 0 && (i != yTile || j != xTile) && i < MAX_TILESROW && j < MAX_TILESROW)
					{
						rangeTileMap[j][i] = blinkMapTileTime * 2;
					}
				}
			}

			unitMenu.AllowOnlyButton(NULL);

			return;
		}
	}
}

void CGame::NextWaveBtnAction()
{
	//if(inGameState == GSG_Attack)
	//{
	//	if(mp_timetowavesspawn[curr_wave] > waveTimer.GetCurrTime())
	//	{
	//		return;
	//	}

	//	if(enemiesPerWave[curr_wave] > 0) return;

	//	if(mp_timetowaves[curr_wave] == 0)
	//		return;

	//	if(waveTimer.GetIsEnd() || waveTimer.GetIsStop())
	//		return;

	//	int CurExtraTime = waveTimer.GetCurrTime_Reversed();
	//	extra_time += CurExtraTime;

	//	//Al adelantar wave, que te de coins correspondientes al numero de segundos que queden, pero un maximo de 7 en caso de quedar mas de 7 segundos.
	//	if(CurExtraTime > 7) 
	//		mp_coins += 7;
	//	else
	//		mp_coins += CurExtraTime;

	//	FixMaxCoins();

	//	curr_wave++;

	//	waveTimer.Reset();
	//	waveTimer.SetTimeLimit(mp_timetowaves[curr_wave]);
	//	waveTimer.Start();
	//}
	//else 

	if(tutorial->GetTutorialName() == Tutorial_Map1)
	{
		tutorial->auxWaveTimer.SetCurrTime(tutorial->auxWaveTimer.GetTimeLimit());

		return;
	}

	if(inGameState == GSG_Defending)
	{
		inGameState = GSG_Attack;
		bRenderAttackText = true;
		bCanOpenUnitMenu = false;
		unitMenu.Close();
		auxTimer.Start();
		waveTimer.SetMaxTimeSecond(mp_timetowaves[0]);
		timer_change_theme.Stop();
		bChangeTheme = false;

		if(gameMode == GAMEMODE_SURVIVAL)
		{
			survivalModeConfig->Start();
			nextWaveButton_x1->Enabled(false);
		}

		if(bIsAudioPaused)
		{
			if(currentMap >= 1 && currentMap < 7)
				LastMusicStacked = Music_Forest_Ingame_02;
			else if(currentMap >= 7 && currentMap < 13)
				LastMusicStacked = Music_Forest_Ingame_02;
			else if(currentMap >= 13 && currentMap < 19)
				LastMusicStacked = Music_IceLand_Ingame_02;
			else if(currentMap >= 19 && currentMap < 25)
				LastMusicStacked = Music_Forest_Ingame_02;
			else
				LastMusicStacked = Music_Forest_Ingame_02;
		}
		else
		{
			if(captainBoss->GetIsActive())
			{
				//Utils::PlayBGM("bgm/sd_boss_intro.mp3", 1);
				//bPlayBossSong = true;
			}
			else
			{
				if(currentMap >= 1 && currentMap < 7)
					Utils::PlayBGM("bgm/sd_ingame_forest_02_theme_loop.mp3", 0);
				else if(currentMap >= 7 && currentMap < 13)
					Utils::PlayBGM("bgm/sd_ingame_mountain_02_theme_loop.mp3", 0);
				else if(currentMap >= 13 && currentMap < 19)
					Utils::PlayBGM("bgm/sd_ingame_iceland_02_theme_loop.mp3", 0);
				else if(currentMap >= 19 && currentMap < 25)
					Utils::PlayBGM("bgm/sd_ingame_fireland_02_theme_loop.mp3", 0);
				else
					Utils::PlayBGM("bgm/sd_ingame_forest_02_theme_loop.mp3", 0);
			}
				
			LastMusicStacked = Music_None;
		}
	}
	else if(inGameState == GSG_Attack)
	{
		if(gameMode == GAMEMODE_SURVIVAL || currentMap == 25)
			return;

		if(waveTimerSpeed == WTIMER_NORMAL)
		{
			waveTimerSpeed = WTIMER_X2;
			currentGameSpeedValue = doubleGameSpeed;
			nextWaveButton_x1->Enabled(false);
			nextWaveButton_x2->Enabled(true);
			nextWaveButton_x3->Enabled(false);
		}
		else if(waveTimerSpeed == WTIMER_X2)
		{
			waveTimerSpeed = WTIMER_X3;
			currentGameSpeedValue = tripleGameSpeed;
			nextWaveButton_x1->Enabled(false);
			nextWaveButton_x2->Enabled(false);
			nextWaveButton_x3->Enabled(true);
		}
		else if(waveTimerSpeed == WTIMER_X3)
		{
			waveTimerSpeed = WTIMER_NORMAL;
			currentGameSpeedValue = normalGameSpeed;
			nextWaveButton_x1->Enabled(true);
			nextWaveButton_x2->Enabled(false);
			nextWaveButton_x3->Enabled(false);
		}
	}
}

void CGame::SetWaveSpeed(TimerSpeed speed)
{
	if(speed == WTIMER_X2)
	{
		waveTimerSpeed = WTIMER_X2;
		currentGameSpeedValue = doubleGameSpeed;
		nextWaveButton_x1->Enabled(false);
		nextWaveButton_x2->Enabled(true);
		nextWaveButton_x3->Enabled(false);
	}
	else if(speed == WTIMER_X3)
	{
		waveTimerSpeed = WTIMER_X3;
		currentGameSpeedValue = tripleGameSpeed;
		nextWaveButton_x1->Enabled(false);
		nextWaveButton_x2->Enabled(false);
		nextWaveButton_x3->Enabled(true);
	}
	else if(speed == WTIMER_NORMAL)
	{
		waveTimerSpeed = WTIMER_NORMAL;
		currentGameSpeedValue = normalGameSpeed;
		nextWaveButton_x1->Enabled(true);
		nextWaveButton_x2->Enabled(false);
		nextWaveButton_x3->Enabled(false);
	}
}

void CGame::NextWaveSpeed()
{
	if(!nextWaveButton_x1->IsEnabled()
		&& !nextWaveButton_x2->IsEnabled()
		&& !nextWaveButton_x3->IsEnabled())
	{
		return;
	}

	if(inGameState == GSG_Defending) 
	{
		NextWaveBtnAction();

		return;
	}

	if(waveTimerSpeed != WTIMER_X3)
	{
		waveTimerSpeed = (TimerSpeed)((int)waveTimerSpeed + 1);
		SetWaveSpeed(waveTimerSpeed);
	}
}

void CGame::PrevWaveSpeed()
{
	if(!nextWaveButton_x1->IsEnabled()
		&& !nextWaveButton_x2->IsEnabled()
		&& !nextWaveButton_x3->IsEnabled())
	{
		return;
	}

	if(inGameState == GSG_Defending) 
	{
		NextWaveBtnAction();

		return;
	}

	if(waveTimerSpeed != WTIMER_NORMAL)
	{
		waveTimerSpeed = (TimerSpeed)((int)waveTimerSpeed - 1);
		SetWaveSpeed(waveTimerSpeed);
	}
}

void CGame::TakeDamage(int damage, IsoEnemy* inflictor)
{
	if(bIsInvincible)
		return;

	mp_lifes -= damage;

	if(mp_lifes > 0)
		damageFadeScreen->Fade(30, 0, 178);

	if(mp_lifes < 0) 
		mp_lifes = 0;

	

	if(mp_lifes == 0)
	{
		if(currentMap != 26)
		{
			inGameState = GSG_Loose;
			bPause = true;
			bIsInvincible = true;
			damageFadeScreen->FadeOut(255);
		}
		else
		{
			dialogController->StartDialog(52);
		}
	}

	Utils::PlaySoundFX(FX_LooseALive);
}

void CGame::StressTest()
{
	return;
	if(!stresstest_timer.GetIsEnd())
		return;

	if(stresstest_num_units <= 0)
		return;

	srand(2009);

	IsoEnemyType random_enemy[] = {ENEMY_Varon};

	for(int i = 0; i < 1; i++)
	{
		for(int j = 0; j < MAX_ISO_ENEMIES; j++)
		{
			IsoEnemy* CurrentEnemy = &isoEnemyArr[j];
			if(CurrentEnemy->GetEnemyType() != ENEMY_None)
				continue;

			CurrentEnemy->setEnemyType(random_enemy[(int)(rand() % 1)]);
			CurrentEnemy->setTargetNum(i + 1);
			CurrentEnemy->InitIsoPawn(initNodes[i]);
			stresstest_num_units--;

			if(stresstest_num_units <= 0)
				return;

			break;
		}
	}

	stresstest_timer.Reset();
	stresstest_timer.Start();
}

void CGame::SpawnRandomUnits(int num_units)
{	
	return;
	if(num_units >= MAX_ISO_SELKNAM)
		num_units = MAX_ISO_SELKNAM - 1;

	srand(2009);

	bool visited[MAX_TILESROW][MAX_TILESROW];
	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
			visited[i][j] = false;
	}

	IsoSelknamType randTypes[] = {SELKNAM_Warrior_Tank, SELKNAM_Archer_Base};// SELKNAM_Archer_Retarder,
	                              //SELKNAM_Shaman_Healer  , SELKNAM_Shaman_Might, SELKNAM_Shaman_Haste,
								  //SELKNAM_Warrior_Tank};//, SELKNAM_Shaman_Might};

	for(int i = 0; i < num_units; i++)
	{
		isoSelknamArr[i].InitSelknam(randTypes[rand() % 2]);

		while(1)
		{
			int xTile = (rand() % MAX_TILESROW);
			int yTile = (rand() % MAX_TILESROW);
			if(!visited[xTile][yTile])
			{
				visited[xTile][yTile] = true;
				isoSelknamArr[i].InitIsoPawn(isoTerrainAstarGrid[xTile][yTile]);
				isoSelknamArr[i].TakeDamage(30);
				break;
			}
		}
	}
}


void CGame::CleanMapDeathPawns()
{
	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			IsoTerrain* CurrentGrid = isoTerrainAstarGrid[i][j];
			if(CurrentGrid == NULL)
				continue;
			if(CurrentGrid->CurrentIsoSelknam != NULL)
			{
				if(CurrentGrid->CurrentIsoSelknam->GetSelknamType() == SELKNAM_None)
				{
					CurrentGrid->CurrentIsoSelknam = NULL;
				}
			}
		}
	}
}

void CGame::CheckGameWinCondition()
{
	if(currentMap >= 25) return;

	if(gameMode != GAMEMODE_CAMPAIGN || dialogController->IsActive())
	{
		return;
	}

	if(mp_lifes <= 0)
		return;

	
	if(!bIsActiveObj)
		return;
	
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() != ENEMY_None)
		{
			return;
		}
	}

	if(currentMap == 24)
	{
		fade->SetColor(255, 255, 255);
		loadingfade->SetColor(255, 255, 255);
		PerformWinGame();
	}
	else
	{

		if(dialogController->DialogExists(currentMap * 2) && !bEndingDialog)
		{
			bEndingDialog = true;
			dialogController->StartDialog(currentMap * 2);
		}
		else
		{
			PerformWinGame();
		}
	}
}

void CGame::PerformWinGame()
{
	endingwin->SetLifesOnLevel(mp_lifes);
	endingwin->SetCoinsBonus(mp_coins * 10);
	endingwin->SetWaveBonus(extra_time);
	inGameState = GSG_Win;
}

void CGame::RenderDebuffGraphics()
{

}

void CGame::NormalizeMapXML()
{
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() != ENEMY_None)
		{
			if(mp_timetowaves[isoEnemyArr[i].getWaveToSpawn() - 1] == 0) continue;

			if(isoEnemyArr[i].getSecToSpawn() > mp_timetowaves[isoEnemyArr[i].getWaveToSpawn() - 1])
			{
				isoEnemyArr[i].setEnemyType(ENEMY_None);
			}
			else
			{
				enemiesPerWave[isoEnemyArr[i].getWaveToSpawn() - 1]++;
			}
		}
	}
}

void CGame::FixMaxCoins()
{
	if(mp_coins >= 999)
	{
		mp_coins = 999;
	}
}

void CGame::InitBossBattle()
{
	if(currentMap == 25)
	{
		bBlockCamera = true;

		//Enemy Spawn
		IsoEnemy* CurrentEnemy = NULL;
		CurrentEnemy = &isoEnemyArr[0];

		if(CurrentEnemy == NULL) return;
		if(CurrentEnemy->getInitialized()) return;

		CurrentEnemy->setEnemyType(ENEMY_Captain);
		CurrentEnemy->setEnemyLevel(1);
		CurrentEnemy->setInitPathNum(0);
		CurrentEnemy->setTargetNum(0);
		CurrentEnemy->InitIsoPawn(isoTerrainAstarGrid[6][8]);
		captainBoss->SetEnemyObject(&isoEnemyArr[0]);

		captainBoss->Init();
	}
}

void CGame::AddTimeBonusByEnemyDie()
{
	int myBonusTime = 5;
	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		if(waveTimerSpeed == WTIMER_X2)
			myBonusTime = 8;
		else if(waveTimerSpeed == WTIMER_X3)
			myBonusTime = 11;
		extra_time += myBonusTime;
	}
}
