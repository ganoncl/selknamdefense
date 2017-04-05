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

bool order_by_z(IsoPawnRenderContainer A, IsoPawnRenderContainer B)
{
	return A._z < B._z;
}

CGame* CGameRef = NULL;

//Keys handler
int32 KeyboardHandler_CGame(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;
	
	if(CGameRef == NULL)
		return -1;

	if(!CGameRef->bKeyboardBlocked)
		CGameRef->KeyboardHandler(event);

	return 0;
}

//Mouse Motion
void HandleMotion_CGame(s3ePointerMotionEvent* event) 
{
	if(CGameRef == NULL)
		return;

	if(!CGameRef->bMouseBlocked)
		CGameRef->UpdateMotionEvents(event);
}

//Mouse handle
void HandleClick_CGame(s3ePointerEvent *event) 
{
	if(CGameRef == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
			{
				CGameRef->MouseHack();
			}
			else
			{
				CGameRef->ActiveButtonMouseHack();
			}
		}

		return;
	}

	if(!CGameRef->bMouseBlocked)
		CGameRef->UpdateClickEvents(event);
}

void CGame_NextWave_Action()
{
	CGameRef->ForceNextWave();
}

#pragma region UnitMenu_Button_Functions
void UnitMenu_ArcherBaseButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Archer_Base);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_ARCHER);
}

void UnitMenu_ArcherPoisonerButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Archer_Poisoner);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_ARCHER);
}

void UnitMenu_ArcherRetarderButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Archer_Retarder);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_ARCHER);
}

void UnitMenu_BowlerStunerButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Bowler_Stuner);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_BOWLER);
}

void UnitMenu_BowlerRooterButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Bowler_Rooter);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_BOWLER);
}

void UnitMenu_BowlerConfuserButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Bowler_Confuser);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_BOWLER);
}

void UnitMenu_MageMightButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Shaman_Might);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_MAGE);
}

void UnitMenu_MageHasteButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Shaman_Haste);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_MAGE);
}

void UnitMenu_MageHealerButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Shaman_Healer);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_MAGE);
}

void UnitMenu_WarriorTankButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Warrior_Tank);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_TANK);
}

void UnitMenu_WarriorBerserkerButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Warrior_Berserk);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_TANK);
}

void UnitMenu_WarriorBaseButton_Action()
{
	CGameRef->SpawnSelknam(SELKNAM_Warrior_Base);
	CGameRef->unitMenu.CloseWithDelay(300, UMB_TANK);
}

void UnitMenu_OKButton_Action()
{
	CGameRef->unitMenu.Close();
}
#pragma endregion UnitMenu_Button_Functions

CGame::CGame()
{
	loadingfade = new FadeScreen();
	timeToFadeOut.SetTimeLimit(TIME_BLACKSCREEN);

	parserHelper.RetrievePawnsAttributesFromFile();
	parserHelper.RetrievePawnsAnimsFromFile();
	CleanMapProperties();
	CleanTerrainObjects();
	CleanPawns();
	AStarPropagateHelper();
	IsoPawnRenderInit();

	arrowProjectileName = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("fx_proyectile_arrow"));
	rockProjectileName  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("fx_proyectile_stone"));
	rootRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fx_root"));

	expData = TextureHelper::GetTextureNamed("fx_explotion");	        explosionName   = TextureHelper::GetAtlasName(expData);
	healData = TextureHelper::GetTextureNamed("fx_heal");	            healName        =  TextureHelper::GetAtlasName(healData);
	poisonData = TextureHelper::GetTextureNamed("fx_poison");           poisonName      = TextureHelper::GetAtlasName(poisonData);
	stunData = TextureHelper::GetTextureNamed("fx_stun");               stunName        = TextureHelper::GetAtlasName(stunData);
	disorientedData = TextureHelper::GetTextureNamed("fx_disoriented"); disorientedName = TextureHelper::GetAtlasName(disorientedData);
	coinsData = TextureHelper::GetTextureNamed("fx_coins");             coinsName       = TextureHelper::GetAtlasName(coinsData);

	//InGame Windows
	mainmenuig = new MainMenuIG();
	endingwin = new EndingWin();
	endingloose = new EndingLoose();

	CGameRef = this;

	//General
	pFont = font_komikadisplaybold25;
	graphic_defense_stage = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_defense_stage"));
	ingameMessagePos = CIwSVec2((int)((IwGxGetScreenWidth() - graphic_defense_stage.width) * 0.5f), 
		(int)((IwGxGetScreenHeight() - graphic_defense_stage.height) * 0.5f));

	defense_stage_back = new Background(0,0,0, 170);

	TextureData nextWave = TextureHelper::GetTextureNamed("btn_nextwave");
	nextWaveData.numStates = 1;
	nextWaveData.state[0].RollOut  = TextureHelper::GetRenderData(nextWave, 1, 4, 0);
	nextWaveData.state[0].RollOn   = TextureHelper::GetRenderData(nextWave, 1, 4, 1);
	nextWaveData.state[0].Pressed  = TextureHelper::GetRenderData(nextWave, 1, 4, 2);
	nextWaveData.state[0].Inactive = TextureHelper::GetRenderData(nextWave, 1, 4, 3);

	nextWaveButton = new Button(TextureHelper::GetCIwTexture(nextWaveData.state[0].RollOut.atlasName), &nextWaveData, 35, 610);
	nextWaveButton->OnClick(&CGame_NextWave_Action);

	//GSG_Defending
	DefenseText = "Defense Stage";
	DefenseTextPos = CIwRect(580, 330, 200, 200);
	bRenderDefenseText = false;

	//GSG_Attack
	AttackText = "Attack Stage";
	AttackTextPos = CIwRect(580, 330, 200, 200);
	bRenderAttackText = false;

	//Timer
	TimeText = "TIME: ";
	TimeTextPos = CIwRect(15, 20, 250, 200);

	Wave_Text = "WAVE: ";
	Wave_Text_Position = CIwRect(15, 50, 250, 250);
	num_hearts_position = CIwRect(1000, 20, 200, 200);
	num_gold_position = CIwRect(1130, 20, 200, 200);

	bPause = false;

	bMouseBlocked = false;
	bKeyboardBlocked = false;

	//Unit Menu
	unitMenu.SetPos(600, 300);
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

	unitManager.SetPos(600, 300);

	damageFadeScreen = new FadeScreen(238,0,0);
	unitManager.SetWorldMapPointer(&mapPointer);	

	label_infocard = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_infocard"));
	label_hud_lives_gold = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_hud_lives_gold"));

	background = new Background(3);
	bIsKeyPressed = false;
	bPrintOutput = false;

	bChangeTheme = true;

	buffBarTextureData = TextureHelper::GetTextureNamed("fx_buffbar");
	buffBarAtlasName   = TextureHelper::GetAtlasName(buffBarTextureData);

	TextureData colorData = TextureHelper::GetTextureNamed("label_tv_colors");
	redButton = TextureHelper::GetRenderData(colorData, 1, 4, 0);

	rangeTileMapData = TextureHelper::GetTextureNamed("graphic_range_decal");
	rangeTileMapAtlasName = TextureHelper::GetAtlasName(rangeTileMapData);

	unitManager.gameRef = this;

	blinkMapTileTime = 500;
}


CGame::~CGame()
{
	if(s_Texture)
		delete s_Texture;

	//InGame Windows
	delete mainmenuig;
	delete endingwin;
	delete endingloose;
	delete nextWaveButton;
	delete damageFadeScreen;
	delete background;
	delete defense_stage_back;

	prop64_64         = NULL;
	prop64_96         = NULL;
	prop64_128        = NULL;
	heart_texture     = NULL;

	delete loadingfade;
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
		isoExplotion[i].timer.SetTimeLimit(50);
		isoExplotion[i].bRender = false;
		isoHaste[i].timer.SetTimeLimit(50);
		isoHeal[i].timer.SetTimeLimit(50);
		isoMight[i].timer.SetTimeLimit(50);
		isoPoisonOnDeath[i].timer.SetTimeLimit(50);
		isoPoisonOnDeath[i].bRender = false;
	}

	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		isoCoin[i].index = -1;
		isoCoin[i].timer.SetTimeLimit(25);
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
		isoPawnRenderContainer[currentIsoPawn]._z            = isoPropArr[i].getPosZ();
		currentIsoPawn++;
	}
	 
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
	{
		IwTrace(APPLOG, ("CURRNET ISO PAWN TOTAL: %d", currentIsoPawn));
		
		isoPawnRenderContainer[currentIsoPawn].isoEnemy      = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProp       = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoSelknam    = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoProjectile = &isoProjectileArr[i];
		isoPawnRenderContainer[currentIsoPawn].isoPoison     = NULL;
		isoPawnRenderContainer[currentIsoPawn].isoExplotion  = NULL;
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
		isoPawnRenderContainer[currentIsoPawn]._z            = isoExplotion[i]._z;
		currentIsoPawn++;
	}
	
	IwTrace(APPLOG, ("CURRNET ISO PAWN TOTAL: %d", currentIsoPawn)); 
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
		
	}

	sort(isoPawnRenderContainer, isoPawnRenderContainer + currentIsoPawn, order_by_z);

	if(bPrintOutput)
	{
		IwTrace(APPLOG, ("Lista de impresion:"));
		for(int i = 0; i < currentIsoPawn; i++)
		{
			//if(     isoPawnRenderContainer[i].isoEnemy      != NULL && isoPawnRenderContainer[i].isoEnemy->getInitialized()) IwTrace(APPLOG, ("Enemy"));
			if(isoPawnRenderContainer[i].isoProjectile != NULL && isoPawnRenderContainer[i].isoProjectile->GetType() != Projectile_None) IwTrace(APPLOG, ("Projectile"));
			//else if(isoPawnRenderContainer[i].isoProp       != NULL) IwTrace(APPLOG, ("Prop"));
			else if(isoPawnRenderContainer[i].isoSelknam    != NULL && isoPawnRenderContainer[i].isoSelknam->getInitialized()) IwTrace(APPLOG, ("Selknam"));
		}
		bPrintOutput = false;
	}

}

void CGame::InitObject()
{
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
	//AStarCalcPathsForAllTiles();
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
	//nextWaveButton->Enabled(false);
	NormalizeMapXML();
	
	//IsoPawnRenderInit();
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
	

	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_CGame, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_CGame, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_CGame, NULL);

	//mp_timedefense = 1;
	timer.SetTimeLimit(mp_timedefense * 1000);
	auxTimer.SetTimeLimit(2 * 1000); //delay between inGameStates

	bRenderDefenseText = false;
	bRenderAttackText = false;

	//Map pointer
	mapPointer.InitIso();
	mapPointer.SetCursorGrid(isoTerrainCursorGrid);

	bMouseBlocked = false;
	bKeyboardBlocked = false;

	if(currentMap == 1)
	{
		bActiveTutorial = true;
	}
	else
	{
		bActiveTutorial = false;
		tutorial->SetSkipped(true);
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

	if(!bUseOldAudioSystem)
	{
		Utils::PlayAudio(Music_Ingame_01);
	}
	else
	{
		//stop previous theme.
		s3eAudioStop();
		//then start the next
		if(!SOUND_MUSIC_MUTE)
		{
			if(bIsAudioPaused)
			{
				LastMusicStacked = Music_Ingame_01;
			}
			else
			{
				if(currentMap == 1 && bShowTutorialLevel1)
					s3eAudioPlay("bgm/sd_ingame_01_theme_loop.mp3", 0);
				else
					s3eAudioPlay("bgm/sd_ingame_01_theme_loop.mp3", 1);
				LastMusicStacked = Music_None;
			}
		}
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

	//Fix UV coords.
	for(int i = 0; i < MAX_TERRAIN_TILES; i++)
	{
		IsoTerrain* CurrentIsoTerrain = &isoTerrainArr[i];
		float uvMinX = (terrainTextureData.textureXPos + CurrentIsoTerrain->getUvMinX() * 512.0f) / (float)terrainTextureData.atlasWidth;
		float uvMaxX = (terrainTextureData.textureXPos + CurrentIsoTerrain->getUvMaxX() * 512.0f) / (float)terrainTextureData.atlasWidth;
		float uvMinY = (terrainTextureData.textureYPos + CurrentIsoTerrain->getUvMinY() * 432.0f) / (float)terrainTextureData.atlasHeight;
		float uvMaxY = (terrainTextureData.textureYPos + CurrentIsoTerrain->getUvMaxY() * 432.0f) / (float)terrainTextureData.atlasHeight;
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
	movementMarkAtlasName = TextureHelper::GetAtlasName(movementMarkData);

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
		}
	}

	deltaTime = 0;
	lastTime = s3eTimerGetMs();
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
			continue;

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
	Update();

	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_CGame);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_CGame);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_CGame);
	s_Texture = NULL;

	if(unitMenu.GetIsActive()) 
		unitMenu.Close();
}

#pragma region UPDATE_GAME
void CGame::Update()
{
	deltaTime = s3eTimerGetMs() - lastTime;
	lastTime = s3eTimerGetMs();

	if(bPause || bPauseGame)
	{
		bMouseBlocked = false;
		bKeyboardBlocked = false;
	}

	loadingfade->Update();
	timeToFadeOut.Update();

	if(bUseOldAudioSystem)
	{
		if(bPause && s3eAudioIsPlaying() && inGameState != GSG_Loose)
			s3eAudioPause();

		if(!bPause && !s3eAudioIsPlaying() && !SOUND_MUSIC_MUTE)
		{
			if(!bIsAudioPaused)
			{
				s3eAudioResume();
			}
		}
	}
	else
	{
		if(bPause && s3eAudioIsPlaying() && inGameState != GSG_Loose)
		{
			LastMusicStacked = Music_None;

			if(inGameState == GSG_Attack)
			{
				CurrentMusicPlaying = Music_Ingame_02;
			}
			else if(inGameState == GSG_Defending)
			{
				CurrentMusicPlaying = Music_Ingame_01;
			}

			LastMusicPosition = s3eAudioGetInt(S3E_AUDIO_POSITION);
			s3eAudioStop();
		}

		if(!bPause && !s3eAudioIsPlaying() && !SOUND_MUSIC_MUTE)
		{
			if(!bIsAudioPaused)
			{
				if(inGameState == GSG_Attack)
				{
					s3eAudioPlay("bgm/sd_ingame_02_theme_loop.mp3");
				}
				else if(inGameState == GSG_Defending)
				{
					s3eAudioPlay("bgm/sd_ingame_01_theme_loop.mp3");
				}
				s3eAudioSetInt(S3E_AUDIO_POSITION, LastMusicPosition);
				LastMusicPosition = 0;
			}
		}
	}

	if(timer_change_theme.GetIsEnd() && inGameState == GSG_Attack && bChangeTheme)
	{
		IwTrace(NEXTTHEME, ("Change theme"));
		if(!SOUND_MUSIC_MUTE)
		{
			if(bIsAudioPaused)
			{
				LastMusicStacked = Music_Ingame_02;
			}
			else
			{
				s3eAudioPlay("bgm/sd_ingame_02_theme_loop.mp3", 0);
				LastMusicStacked = Music_None;
				CurrentMusicPlaying = Music_Ingame_02;
			}
			
			timer_change_theme.Reset();
			timer_change_theme.Stop();
			bChangeTheme = false;
		}
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

	//TODO: Logic of game.
	CheckGameWinCondition();

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
		}
		else if(inGameState==GSG_Loose)
		{
			//init state
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
		if(bShowTutorialLevel1 && bActiveTutorial)
		{
			tutorial->SetSkipped(false);
			tutorial->InitTutorial(Tutorial_Map1);
			bActiveTutorial = false;
		}
	}
	
	//General updates.
	UpdateTimer();
	if(!bPause && !bPauseGame)
	{
		CleanMapDeathPawns();
		UpdateIsoPawnsOnScreen();
		UpdateProjectiles();
		UpdateHUD();
		UpdateGameplayVariables();
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

		if(inGamePausedState == GSGP_MainMenuIG) mainmenuig->InitObject();
		else if(inGamePausedState == GSGP_None) bPause = false;

		lastInGamePausedState = inGamePausedState;
	}

	if(inGamePausedState == GSGP_MainMenuIG) mainmenuig->Update();
}

void CGame::UpdateGameState_GSG_Defending()
{
	//Once message is gone, countdown starts
	if(auxTimer.GetIsEnd() && timer.GetIsStop())
	{
		timer.Start();
		timer_change_theme.Reset();
		timer_change_theme.Start();
		//bRenderDefenseText = false;
		//bCanOpenUnitMenu = true;
	} 

	//When defending time is over, the attack starts.
	if(timer.GetIsEnd()) 
	{
		inGameState = GSG_Attack;
		bRenderAttackText = true;
		bCanOpenUnitMenu = false;
		//unitMenu.Close();
		auxTimer.Start();
		waveTimer.SetTimeLimit(mp_timetowaves[0] * 1000);
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
		waveTimer.SetTimeLimit(mp_timetowaves[curr_wave] * 1000);
		waveTimer.Start();
	}
	//Enemy Spawn
	IsoEnemy* CurrentEnemy = NULL;
	int initPathNum = 0;

	/*
	int temp = 0;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() != ENEMY_None)
			temp++;
	}

	IwTrace(APPLOG, ("ENEMY COUNT: %i", temp));
	*/


	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		CurrentEnemy = &isoEnemyArr[i];

		if(CurrentEnemy == NULL)
			continue;

		if(CurrentEnemy->getInitialized() || CurrentEnemy->GetEnemyType() == ENEMY_None)
			continue;

		if(CurrentEnemy->getWaveToSpawn() == curr_wave + 1)
		{
			if(!CurrentEnemy->bIsActiveObj && CurrentEnemy->getSecToSpawn() <= (int)(waveTimer.GetCurrTime()*0.001))
			{
				initPathNum = CurrentEnemy->getInitPathNum() - 1;
				//IwTrace(APPLOG, ("Init Path: %i", CurrentEnemy->getSecToSpawn()));
				if(initNodes[initPathNum] != NULL)
				{
					//IwTrace(APPLOG, ("LAYER INIT NODE: %i", initNodes[0]->getLayer()));//, initNodes[0]->getYNumTile()));
					CurrentEnemy->InitIsoPawn(initNodes[initPathNum]);
					enemiesPerWave[CurrentEnemy->getWaveToSpawn() - 1]--;
				}
			}
		}
	}
}

void CGame::UpdateTimer()
{
	//Update timers
	if(!bPause && !bPauseGame)
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
	/*
	if(nextWaveButton->IsActive()) IwTrace(SELKNAM, ("WAVE ACTIVO"));
	else IwTrace(SELKNAM, ("WAVE NO ACTIVO"));
	*/
	//Map Pointer
	if(!unitMenu.GetIsActive() && !unitManager.GetIsActive())
		mapPointer.SetIsActive(true);
	else
		mapPointer.SetIsActive(false);

	mapPointer.Update();
	unitMenu.Update();
	unitManager.Update();

	//Wave information
	sprintf(Number_Wave, "%s %i / %i", Wave_Text, curr_wave + 1, num_waves);

	/*
	if(nextWaveButton->IsActive() && (inGameState != GSG_Attack || mp_timetowaves[curr_wave] == 0 || time_before_fastButton > waveTimer.GetCurrTime()))
		nextWaveButton->Enabled(false);
	else if(!nextWaveButton->IsActive() && inGameState == GSG_Attack && mp_timetowaves[curr_wave] != 0 && time_before_fastButton <= waveTimer.GetCurrTime())
		nextWaveButton->Enabled(true);
	*/
	//Timer
	if(inGameState == GSG_Defending)
	{
		sprintf(timer_text, "%s %i", TimeText, (int)(timer.GetCurrTime_Reversed() * 0.001));
		if(7 > (int)(timer.GetCurrTime() * 0.001))
		{
			if(nextWaveButton->IsActive())
				nextWaveButton->Enabled(false);
		}
		else
		{
			if(!nextWaveButton->IsActive())
				nextWaveButton->Enabled(true);
		}
	}
	else if(inGameState == GSG_Attack) 
	{
		sprintf(timer_text, "%s %i", TimeText, (int)(waveTimer.GetCurrTime_Reversed() * 0.001));
		if(mp_timetowavesspawn[curr_wave] > (int)(waveTimer.GetCurrTime() * 0.001) || num_waves == (curr_wave + 1))
		{
			if(nextWaveButton->IsActive())
				nextWaveButton->Enabled(false);
		}
		else
		{
			if(!nextWaveButton->IsActive())
				nextWaveButton->Enabled(true);
		}
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

	loadingfade->Render();

	if(bDoFadeOut && timeToFadeOut.GetIsEnd())
	{
		loadingfade->FadeOut(FADE_OUT_SCREEN);	
		bDoFadeOut = false;
	}
}

void CGame::HandleRenderGameStates(InGameState _inGameState)
{
	background->Render();
	IwGxFlush();
	RenderMovementCursor();
	RenderTerrain();
	RenderIsoPawnsOnScreen();
	//RenderDebuffGraphics();
	RenderProjectiles();
	IwGxFlush();
	RenderIsoPawnBarBuff();
	RenderIsoPawnBarLifes();
	RenderOthers();
	RenderHUD();
	IwGxFlush();
}

void CGame::HandleRenderGamePausedStates()
{
	if(inGamePausedState == GSGP_MainMenuIG) mainmenuig->Render();
}


void CGame::RenderMovementCursor()
{
	mapPointer.Render();
	unitManager.RenderMovementCursor();
}

void CGame::RenderHUD()
{
	damageFadeScreen->Render();


	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_infocard.atlasName), CIwSVec2(40, 0), label_infocard);
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_hud_lives_gold.atlasName), CIwSVec2(984, 0), label_hud_lives_gold);

	IwGxLightingOn();
	Utils::RenderText(timer_text,  CIwRect(60, 65, 166, 90), font_komikadisplaybold18, 0xff0da8d9, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
	Utils::RenderText(Number_Wave, CIwRect(60, 65 + 25, 166, 90), font_komikadisplaybold18, 0xff0da8d9, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
	Utils::RenderText(num_hearts_text, CIwRect(984 + 50, 45, 64, 40), font_komikadisplaybold22, 0xff0da8d9, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE);
	Utils::RenderText(num_gold_text, CIwRect(984 + 172, 45, 80, 40), font_komikadisplaybold22, 0xff0da8d9, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE);	
	IwGxLightingOff();
	/*
	RenderText(num_hearts_text, num_hearts_position, 0xffffffff);
	RenderText(num_gold_text, num_gold_position, 0xffffffff);
	RenderText(timer_text, TimeTextPos, 0xffffffff);
	RenderText(Number_Wave, Wave_Text_Position, 0xffffffff);
	*/

	nextWaveButton->Render();

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

		int num_debuffs;
		Skill_Type list[MAX_SKILLS + 1];
		float offset_y = -67.0f;

		CurrentIsoEnemy->GetBuffList(list, &num_debuffs);
		
		for(int j = 0; j < num_debuffs; j++)
		{
			Utils::SetBuffImageUV(list[j], &buff_uvMaxX, &buff_uvMinX, &buff_uvMaxY, &buff_uvMinY);
			float offset_x = -28.0f + 20.0f * j;

			DrawQuadBatch(16, 16, 
				CurrentIsoEnemy->getPosX(), CurrentIsoEnemy->getPosY(), CurrentIsoEnemy->getPosZ(), 
				buff_uvMinX, buff_uvMaxX, buff_uvMinY, buff_uvMaxY,
				offset_x, offset_y);
		}

		/*
		if(CurrentIsoEnemy->GetLastDebuffReceived() != Skill_Type_None)
		{
			Utils::SetBuffImageUV(CurrentIsoEnemy->GetLastDebuffReceived(), &buff_uvMaxX, &buff_uvMinX, &buff_uvMaxY, &buff_uvMinY);
			float offset_x = -28.0f + 20.0f * 2;
			DrawQuadBatch(16, 16, 
				CurrentIsoEnemy->getPosX(), CurrentIsoEnemy->getPosY(), CurrentIsoEnemy->getPosZ(), 
				buff_uvMinX, buff_uvMaxX, buff_uvMinY, buff_uvMaxY,
				offset_x, offset_y);
		}
		*/
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
	float offset_y = -67.0f;
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
		//IwTrace(APPLOG, ("Numero de buffs: %i", num_debuffs));
		for(int j = 0; j < num_debuffs; j++)
		{
			Utils::SetBuffImageUV(list[j], &buff_uvMaxX, &buff_uvMinX, &buff_uvMaxY, &buff_uvMinY);
			float offset_x = -28.0f + 16.0f * j;

			DrawQuadBatch(16, 16, 
				CurrentIsoSelknam->getPosX(), CurrentIsoSelknam->getPosY(), CurrentIsoSelknam->getPosZ(), 
				buff_uvMinX, buff_uvMaxX, buff_uvMinY, buff_uvMaxY,
				offset_x, offset_y);
		}

		/*
		if(CurrentIsoSelknam->GetLastDebuffReceived() != Skill_Type_None)
		{
			Utils::SetBuffImageUV(CurrentIsoSelknam->GetLastDebuffReceived(), &buff_uvMaxX, &buff_uvMinX, &buff_uvMaxY, &buff_uvMinY);
			float offset_x = -28.0f + 16.0f * 3;
			DrawQuadBatch(16, 16, 
				CurrentIsoSelknam->getPosX(), CurrentIsoSelknam->getPosY(), CurrentIsoSelknam->getPosZ(), 
				buff_uvMinX, buff_uvMaxX, buff_uvMinY, buff_uvMaxY,
				offset_x, offset_y);
		}
		*/
	}

	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(NULL);
	IwGxSetUVStream(s_UVsBatch);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched * 4);
	IwGxFlush();
}

void CGame::RenderOthers()
{
	if(bRenderColorButtons)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(redButton.atlasName), CIwSVec2(70, 678), redButton);
	}
}

void CGame::RenderIsoPawnBarLifes()
{
	numImagesBatched = 0;

	float barXOff = -28.0f;
	float barYOff = -48.0f;
	float barHeight = 4;
	float barWidth = 60.0f;

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
							movementCursorXid = 3;
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
							movementCursorXid = 4;
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

				//int alpha = 255;
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

				DrawQuadBatch(64.0f, 64.0f, 
					CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
					uvMinX, uvMaxX, uvMinY, uvMaxY,
					-32.0f, -32.0f, 255, 255, 255, alpha, rangeTileMapData.textureRotated);

				if(rangeTileMap[i][j] > 0)
				{
					rangeTileMap[i][j] -= (int)deltaTime;
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

				movementCursorYid = 4 - movementCursorYid;
				uvMinX = ((uvX * movementCursorXid) * movementMarkData.textureHeight + movementMarkData.textureYPos) / (float)movementMarkData.atlasWidth;
				uvMinY = ((uvY * movementCursorYid) * movementMarkData.textureWidth  + movementMarkData.textureXPos) / (float)movementMarkData.atlasHeight;
				uvMaxX = ((uvX * (movementCursorXid + 1)) * movementMarkData.textureHeight + movementMarkData.textureYPos) / (float)movementMarkData.atlasWidth;
				uvMaxY = ((uvY * (movementCursorYid + 1)) * movementMarkData.textureWidth  + movementMarkData.textureXPos) / (float)movementMarkData.atlasHeight;


				DrawQuadBatch(48.0f, 40.0f, 
					CurrentIsoTerrain->getPosX(), CurrentIsoTerrain->getPosY(), CurrentIsoTerrain->getPosZ(), 
					uvMinY, uvMaxY, uvMinX, uvMaxX,
					-24.0f, -18.0f, 255, 255, 255,  255, true);
			}
		}
	}
	IwGxSetVertStreamModelSpace(s_QuadBatch, numImagesBatched * 4);
	IwGxSetColStream(NULL);
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

	DrawQuadBatch(pawn->getWidth()   , pawn->getHeight(),  pawn->getPosX()  ,   pawn->getPosY(),     pawn->getPosZ(),
		pawn->getUvMinX()  , pawn->getUvMaxX(),  pawn->getUvMinY(),   pawn->getUvMaxY(),
		pawn->getOffsetX() , pawn->getOffsetY(), pawn->getTextureR(), pawn->getTextureG(), pawn->getTextureB(), pawn->getTextureAlpha());

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
			RenderIsoPawnTexture(isoPawnRenderContainer[i].isoEnemy->GetTexture(), isoPawnRenderContainer[i].isoEnemy);
			CurrentIsoEnemy = isoPawnRenderContainer[i].isoEnemy;

			CurrentIsoEnemy->GetDebuffList(list, &size);
			for(int i = 0; i < size; i++)
			{
				if(list[i] == Skill_Type_Root)
				{
					float offset_x = -32.0f;
					float offset_y = -16.0f;
					float uvMinX = rootRenderData.uvInfo.minX;
					float uvMaxX = rootRenderData.uvInfo.maxX;
					float uvMinY = rootRenderData.uvInfo.minY;
					float uvMaxY = rootRenderData.uvInfo.maxY;
					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(rootRenderData.atlasName));
					Utils::DrawQuadBatch((float)rootRenderData.width, (float)rootRenderData.height, 
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

					float offset_x = -16.0f;
					float offset_y = -16.0f - 48.0f;
					float uvMinX = 0.2f * idx;
					float uvMaxX = 0.2f * (idx + 1);
					float uvMinY = 0.5f * idy;
					float uvMaxY = 0.5f * (idy + 1);

					uvMinX = (disorientedData.textureXPos + uvMinX * 160.0f) / (float)disorientedData.atlasWidth;
					uvMaxX = (disorientedData.textureXPos + uvMaxX * 160.0f) / (float)disorientedData.atlasWidth;
					uvMinY = (disorientedData.textureYPos + uvMinY * 64.0f) / (float)disorientedData.atlasHeight;
					uvMaxY = (disorientedData.textureYPos + uvMaxY * 64.0f) / (float)disorientedData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(disorientedName));
					Utils::DrawQuadBatch(32.0f, 32.0f, 
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

					float offset_x = -32.0f;
					float offset_y = -8.0f - 36.0f;
					float uvMinX = 0.2f * idx;
					float uvMaxX = 0.2f * (idx + 1);
					float uvMinY = 0.5f * idy;
					float uvMaxY = 0.5f * (idy + 1);

					uvMinX = (stunData.textureXPos + uvMinX * 320.0f) / (float)stunData.atlasWidth;
					uvMaxX = (stunData.textureXPos + uvMaxX * 320.0f) / (float)stunData.atlasWidth;
					uvMinY = (stunData.textureYPos + uvMinY * 32.0f) / (float)stunData.atlasHeight;
					uvMaxY = (stunData.textureYPos + uvMaxY * 32.0f) / (float)stunData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(stunName));
					Utils::DrawQuadBatch(64.0f, 16.0f, 
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

					float offset_x = -16.0f;
					float offset_y = -40.0f - 16.0f;
					float uvMinX = 0.2f * _idx;
					float uvMaxX = 0.2f * (_idx + 1);
					float uvMinY = 0.333f * _idy;
					float uvMaxY = 0.333f * (_idy + 1);

					uvMinX = (coinsData.textureXPos + uvMinX * 160.0f) / (float)coinsData.atlasWidth;
					uvMaxX = (coinsData.textureXPos + uvMaxX * 160.0f) / (float)coinsData.atlasWidth;
					uvMinY = (coinsData.textureYPos + uvMinY * 240.0f) / (float)coinsData.atlasHeight;
					uvMaxY = (coinsData.textureYPos + uvMaxY * 240.0f) / (float)coinsData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(coinsName));
					Utils::DrawQuadBatch(32.0f, 80.0f, 
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

			if(CurrentIsoSelknam->GetIsoHealIndex() != -1)
			{
				if(isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index != -1)
				{
					int _idx = isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index % 4;
					int _idy = isoHeal[CurrentIsoSelknam->GetIsoHealIndex()].index / 4;

					float offset_x = -32.0f;
					float offset_y = -80.0f;
					float uvMinX = 0.25f * _idx;
					float uvMaxX = 0.25f * (_idx + 1);
					float uvMinY = 0.25f * _idy;
					float uvMaxY = 0.25f * (_idy + 1);

					uvMinX = (healData.textureXPos + uvMinX * 256.0f) / (float)healData.atlasWidth;
					uvMaxX = (healData.textureXPos + uvMaxX * 256.0f) / (float)healData.atlasWidth;
					uvMinY = (healData.textureYPos + uvMinY * 512.0f) / (float)healData.atlasHeight;
					uvMaxY = (healData.textureYPos + uvMaxY * 512.0f) / (float)healData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(healName));
					Utils::DrawQuadBatch(64.0f, 128.0f, 
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

					float offset_x = -32.0f;
					float offset_y = -80.0f;
					float uvMinX = 0.25f * _idx;
					float uvMaxX = 0.25f * (_idx + 1);
					float uvMinY = 0.25f * _idy;
					float uvMaxY = 0.25f * (_idy + 1);
					uvMinX = (healData.textureXPos + uvMinX * 256.0f) / (float)healData.atlasWidth;
					uvMaxX = (healData.textureXPos + uvMaxX * 256.0f) / (float)healData.atlasWidth;
					uvMinY = (healData.textureYPos + uvMinY * 512.0f) / (float)healData.atlasHeight;
					uvMaxY = (healData.textureYPos + uvMaxY * 512.0f) / (float)healData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(healName));
					Utils::DrawQuadBatch(64.0f, 128.0f, 
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

					float offset_x = -32.0f;
					float offset_y = -80.0f;
					float uvMinX = 0.25f * _idx;
					float uvMaxX = 0.25f * (_idx + 1);
					float uvMinY = 0.25f * _idy;
					float uvMaxY = 0.25f * (_idy + 1);
					uvMinX = (healData.textureXPos + uvMinX * 256.0f) / (float)healData.atlasWidth;
					uvMaxX = (healData.textureXPos + uvMaxX * 256.0f) / (float)healData.atlasWidth;
					uvMinY = (healData.textureYPos + uvMinY * 512.0f) / (float)healData.atlasHeight;
					uvMaxY = (healData.textureYPos + uvMaxY * 512.0f) / (float)healData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(healName));
					Utils::DrawQuadBatch(64.0f, 128.0f, 
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

					float offset_x = -32.0f;
					float offset_y = -40.0f;
					float uvMinX = 0.2f * _idx;
					float uvMaxX = 0.2f * (_idx + 1);
					float uvMinY = 0.1667f * _idy;
					float uvMaxY = 0.1667f * (_idy + 1);

					uvMinX = (expData.textureXPos + uvMinX * 320.0f) / (float)expData.atlasWidth;
					uvMaxX = (expData.textureXPos + uvMaxX * 320.0f) / (float)expData.atlasWidth;
					uvMinY = (expData.textureYPos + uvMinY * 480.0f) / (float)expData.atlasHeight;
					uvMaxY = (expData.textureYPos + uvMaxY * 480.0f) / (float)expData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(explosionName));
					Utils::DrawQuadBatch(64.0f, 80.0f, 
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

					if(isoExplotion[CurrentIsoSelknam->GetIsoExplotionIndex()].index > 23)
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

					float offset_x = -32.0f;
					float offset_y = -40.0f;
					float uvMinX = 0.2f * _idx;
					float uvMaxX = 0.2f * (_idx + 1);
					float uvMinY = 0.1667f * _idy;
					float uvMaxY = 0.1667f * (_idy + 1);
					uvMinX = (poisonData.textureXPos + uvMinX * 320.0f) / (float)poisonData.atlasWidth;
					uvMaxX = (poisonData.textureXPos + uvMaxX * 320.0f) / (float)poisonData.atlasWidth;
					uvMinY = (poisonData.textureYPos + uvMinY * 480.0f) / (float)poisonData.atlasHeight;
					uvMaxY = (poisonData.textureYPos + uvMaxY * 480.0f) / (float)poisonData.atlasHeight;

					Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(poisonName));
					Utils::DrawQuadBatch(64.0f, 80.0f, 
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()]._x, isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()]._y, isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()]._z,
						uvMinX, 
						uvMaxX, 
						uvMinY,
						uvMaxY,
						offset_x, 
						offset_y);
					Utils::RenderQuadBatch();

					if(isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.GetIsEnd())
					{
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index++;
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.Reset();
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.Start();
					}


					if(isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index > 23)
					{
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].index = -1;
						isoPoisonOnDeath[CurrentIsoSelknam->GetIsoPoisonOnDeathIndex()].timer.Stop();
						CurrentIsoSelknam->SetIsoPoisonOnDeathIndex(-1);
					}
				}
			}

			continue;
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

			int _idx = effect->index % 5;
			int _idy = effect->index / 5;

			float offset_x = -32.0f;
			float offset_y = -40.0f;
			float uvMinX = 0.2f * _idx;
			float uvMaxX = 0.2f * (_idx + 1);
			float uvMinY = 0.1667f * _idy;
			float uvMaxY = 0.1667f * (_idy + 1);

			uvMinX = (expData.textureXPos + uvMinX * 320.0f) / (float)expData.atlasWidth;
			uvMaxX = (expData.textureXPos + uvMaxX * 320.0f) / (float)expData.atlasWidth;
			uvMinY = (expData.textureYPos + uvMinY * 480.0f) / (float)expData.atlasHeight;
			uvMaxY = (expData.textureYPos + uvMaxY * 480.0f) / (float)expData.atlasHeight;

			Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(explosionName));
			Utils::DrawQuadBatch(64.0f, 80.0f, 
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

			float offset_x = -32.0f;
			float offset_y = -40.0f;
			float uvMinX = 0.2f * _idx;
			float uvMaxX = 0.2f * (_idx + 1);
			float uvMinY = 0.1667f * _idy;
			float uvMaxY = 0.1667f * (_idy + 1);
			uvMinX = (poisonData.textureXPos + uvMinX * 320.0f) / (float)poisonData.atlasWidth;
			uvMaxX = (poisonData.textureXPos + uvMaxX * 320.0f) / (float)poisonData.atlasWidth;
			uvMinY = (poisonData.textureYPos + uvMinY * 480.0f) / (float)poisonData.atlasHeight;
			uvMaxY = (poisonData.textureYPos + uvMaxY * 480.0f) / (float)poisonData.atlasHeight;

			Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(poisonName));
			Utils::DrawQuadBatch(64.0f, 80.0f, 
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
	//render loop defending
	/*
	if(bRenderDefenseText)
	{
		//Message InGame
		RenderSingleTexture(ingameMessageTex, ingameMessagePos);
		RenderText(DefenseText, DefenseTextPos, 0xffffffff);
		IwGxFlush();
	}
	*/
}

void CGame::RenderProjectiles()
{
	for(int i = 0; i < MAX_ISO_PROJECTILE; i++)
		if(isoProjectileArr[i].GetType() != Projectile_None)
			isoProjectileArr[i].Render();
}

void CGame::AttachPawnTextureToMaterial(IsoSelknamType SelknamType)
{
	if(SelknamType == SELKNAM_Archer_Base)          s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_archers00")->GetResNamed("archer1",   IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Archer_Poisoner) s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_archers00")->GetResNamed("archer2",   IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Warrior_Base)    s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_warriors00")->GetResNamed("melee1",    IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Archer_Retarder) s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_archers01")->GetResNamed("archer3",   IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Shaman_Healer)   s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_shamans")->GetResNamed("mage2",     IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Shaman_Might)    s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_shamans")->GetResNamed("mage3",     IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Shaman_Haste)    s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_shamans")->GetResNamed("mage1",     IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Warrior_Berserk) s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_warriors01")->GetResNamed("melee3",    IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Warrior_Tank)    s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_warriors00")->GetResNamed("melee2",    IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Bowler_Rooter)   s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_bowlers00")->GetResNamed("boleador2", IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Bowler_Stuner)   s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_bowlers00")->GetResNamed("boleador1", IW_GX_RESTYPE_TEXTURE);
	else if(SelknamType == SELKNAM_Bowler_Confuser) s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_bowlers01")->GetResNamed("boleador3", IW_GX_RESTYPE_TEXTURE);

	IwGxFlush();
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(s_Texture);
	IwGxSetMaterial(pMat);
}

void CGame::AttachPawnTextureToMaterial(IsoEnemyType EnemyType)
{
	if(EnemyType == ENEMY_Soldier)        s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_enemies_one00")->GetResNamed("enemy_1", IW_GX_RESTYPE_TEXTURE);
	else if(EnemyType == ENEMY_Convict)   s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_enemies_one01")->GetResNamed("enemy_3", IW_GX_RESTYPE_TEXTURE);
	else if(EnemyType == ENEMY_Varon)     s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_enemies_one00")->GetResNamed("enemy_2", IW_GX_RESTYPE_TEXTURE);
	else if(EnemyType == ENEMY_Scientist) s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_enemies_one01")->GetResNamed("enemy_4", IW_GX_RESTYPE_TEXTURE);
	else if(EnemyType == ENEMY_Tank)      s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_enemies_two")->GetResNamed("enemy_5", IW_GX_RESTYPE_TEXTURE);
	else if(EnemyType == ENEMY_Rabbit)    s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_enemies_two")->GetResNamed("enemy_6", IW_GX_RESTYPE_TEXTURE);
	else if(EnemyType == ENEMY_Pirate)    s_Texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("game_enemies_two")->GetResNamed("enemy_7", IW_GX_RESTYPE_TEXTURE);

	IwGxFlush();
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(s_Texture);
	IwGxSetMaterial(pMat);
}

void CGame::DrawQuad(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX, float _uvMaxX, float _uvMinY, float _uvMaxY, float _offsetX, float _offsetY)
{
	// Allocate a material from the IwGx global cache
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	// Set this as the active material
	pMat->SetTexture(s_Texture);
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	//pMat->SetColAmbient(0xfff0f0f0);
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

	// End drawing
	//IwGxFlush();
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

void CGame::KeyboardHandler(s3eKeyboardEvent* event)
{
	if(bPauseGame)
		return;

	if(bPause) 
		return;

	if(inGameState == GSG_Win)
		return;

	if(event->m_Pressed) bMouseBlocked = true;
	else bMouseBlocked = false;

	if(!unitMenu.GetIsActive() && !unitManager.GetIsActive())
	{
		mapPointer.KeyboardHandler(event);
	}
	unitMenu.KeyboardHandler(event);
	unitManager.KeyboardHandler(event);

	if(bBlockKeyboardOnce)
	{
		bBlockKeyboardOnce = false;
		return;
	}

	if(event->m_Pressed)
	{
		bIsKeyPressed = true;
		lastKeyPressed = event->m_Key;
		if(lastKeyPressed == s3eKeyEnter || lastKeyPressed == s3eKeyAbsOk)
		{
			if(bCanOpenUnitMenu && isoTerrainCursorGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()]->getCanPutUnit())
			{
				IsoTerrain* CurrentGrid = isoTerrainCursorGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()];
				//Checks if a unit is placed on isoTerrain
				if(CurrentGrid->CurrentIsoSelknam != NULL)
				{
					if(!unitManager.GetIsActive())
						unitManager.OpenWithKeyboard(CurrentGrid, &mp_coins);
				}
				else if(!unitMenu.GetIsActive())
				{
					unitMenu.OpenWithKeyboard(mp_coins);
				}
			}
		}
		else if(lastKeyPressed == s3eKeyAbsASK)
		{
			//Set the wave button to pressed state.
			if(nextWaveButton->IsActive())
			{
				nextWaveButton->SetStatus(2);
			}
		}
	}
	else
	{
		bIsKeyPressed = false;
		lastKeyPressed = s3eKeyFirst;
	}

	if(bIsKeyPressed)
	{
		if(lastKeyPressed == s3eKeyAbsBSK)
		{
			if(nextWaveButton->IsActive())
			{
				nextWaveButton->Execute();				
			}
		}
		else if(lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack) 
		{
			if(!unitManager.GetIsActive() && !bPause)
			{
				bPause = true;
				inGamePausedState = GSGP_MainMenuIG;
				bMouseBlocked = false;
			}
		}

		bIsKeyPressed = false;
		lastKeyPressed = s3eKeyFirst;
	}
}

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

	if(bPauseGame)
		return;

	if(bPause)
		return;

	//IwTrace(POINTER, ("Motion"));
	unitMenu.UpdateMotionEvents(event);
	unitManager.UpdateMotionEvents(event);
	mapPointer.UpdateMotionEvents(event);
	nextWaveButton->HandleMotion(event);
}

void CGame::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bPauseGame)
		return;

	if(bPause)
		return;

	nextWaveButton->HandleClick(event);

	if(bCanOpenUnitMenu && 
		event->m_Pressed && event->m_Button ==  S3E_POINTER_BUTTON_LEFTMOUSE && mapPointer.GetIsMouseOnAGrid())
	{
		IsoTerrain* CurrentGrid = isoTerrainCursorGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()];

		if(!unitManager.GetIsActive() && !unitMenu.GetIsActive())
		{
			CIwVec2 Tile = mapPointer.GetTileByMouseCoords((float)event->m_x, (float)event->m_y, CurrentGrid->getLayer());
			if(Tile.x != CurrentGrid->getXNumTile() || Tile.y != CurrentGrid->getYNumTile())
				return;
		}

		if(CurrentGrid->getCanPutUnit())
		{
			//Replaces "true" with selknam condition...
			if(CurrentGrid->CurrentIsoSelknam != NULL)
			{
				if(!unitManager.GetIsActive() && !unitMenu.GetIsActive())
					unitManager.Open(CurrentGrid, &mp_coins);
			}
			else if(!unitMenu.GetIsActive() && !unitManager.GetIsActive())
			{
				mapPointer.SetIsActive(false);
				unitMenu.Open(mp_coins);
			}
		}
	}

	unitMenu.UpdateClickEvents(event);
	unitManager.UpdateClickEvents(event);
}

void CGame::RenderText(const char* text, CIwRect position, uint32 color)
{
	IwGxLightingOn();
	IwGxFontSetFont(pFont);
	IwGxFontSetCol(color);
	IwGxFontSetRect(position);
	IwGxFontDrawText(text);
	IwGxLightingOff();
}

void CGame::Debug()
{
	//DEBUG: Path from a particular point to an objective.
	/*
	IwTrace(APPLOG, ("AVAILABLE OBJECTIVES"));
	for(int i = 0; objectivesNodes[i] != NULL; i++)
		IwTrace(APPLOG, ("%i: (%i, %i)", i, objectivesNodes[i]->getXNumTile(), objectivesNodes[i]->getYNumTile()));

	int obj = 3;
	IsoTerrain* tmp = isoTerrainAstarGrid[0][0];
	IwTrace(APPLOG, ("PATH TO (%i, %i) FROM (%i, %i)", objectivesNodes[obj]->getXNumTile(), objectivesNodes[obj]->getYNumTile(), tmp->getXNumTile(), tmp->getYNumTile()));
	if(tmp->path[obj][0] == NULL)
		IwTrace(APPLOG, ("OBJECTIVE CANT BE REACHED"));
	for(int i = 0; tmp->path[obj][i] != NULL; i++)
		IwTrace(APPLOG, ("%i %i", tmp->path[obj][i]->getXNumTile(), tmp->path[obj][i]->getYNumTile()));
	IwTrace(APPLOG, ("END"));
	*/
	/*
	IsoTerrain* tmp = isoTerrainAstarGrid[8][6];
	IwTrace(APPLOG, ("Grid (%i, %i) information:", tmp->getXNumTile(), tmp->getYNumTile()));
	IwTrace(APPLOG, ("Layer: %i", tmp->getLayer()));
	IwTrace(APPLOG, ("Position: (%f, %f, %f)", tmp->getPosX(), tmp->getPosY(), tmp->getPosZ()));
	if(tmp->northNode) IwTrace(APPLOG, ("North: (%i, %i)", tmp->northNode->getXNumTile(), tmp->northNode->getYNumTile()));
	if(tmp->westNode)  IwTrace(APPLOG, ("West : (%i, %i)", tmp->westNode->getXNumTile(),  tmp->westNode->getYNumTile()));
	if(tmp->southNode) IwTrace(APPLOG, ("South: (%i, %i)", tmp->southNode->getXNumTile(), tmp->southNode->getYNumTile()));
	if(tmp->eastNode)  IwTrace(APPLOG, ("East : (%i, %i)", tmp->eastNode->getXNumTile(),  tmp->eastNode->getYNumTile()));
	*/
	//DEBUG

	//Enemies
	///*isoEnemyArr[0].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[0].SetGrid(isoTerrainAstarGrid[17][17]);
	//isoEnemyArr[0].GoToObjective(0);
	//isoEnemyArr[1].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[1].SetGrid(isoTerrainAstarGrid[1][1]);
	//isoEnemyArr[1].GoToObjective(0);
	//isoEnemyArr[2].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[2].SetGrid(isoTerrainAstarGrid[12][1]);
	//isoEnemyArr[2].GoToObjective(2);
	//isoEnemyArr[3].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[3].SetGrid(isoTerrainAstarGrid[13][2]);
	//isoEnemyArr[3].GoToObjective(0);
	//isoEnemyArr[4].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[4].SetGrid(isoTerrainAstarGrid[11][2]);
	//isoEnemyArr[4].GoToObjective(2);
	//isoEnemyArr[5].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[5].SetGrid(isoTerrainAstarGrid[10][1]);
	//isoEnemyArr[5].GoToObjective(0);
	//isoEnemyArr[6].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[6].SetGrid(isoTerrainAstarGrid[6][2]);
	//isoEnemyArr[6].GoToObjective(1);
	//isoEnemyArr[7].InitEnemy(ENEMY_Soldier);
	//isoEnemyArr[7].SetGrid(isoTerrainAstarGrid[5][1]);
	//isoEnemyArr[7].GoToObjective(0);*/
	//End Enemies
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
			//IwTrace(APPLOG, ("GetInitNodes() called, numInitNode %i", numInitNode));
			if(numInitNode >= 10)
				IwAssertMsg(APPASSERT, numInitNode >= 10, ("GetInitNodes(), Illegal value"));
			initNodes[numInitNode] = &isoTerrainArr[i];
			//IwTrace(APPLOG, ("INIT NODES %i %i", initNodes[numInitNode]->getXNumTile(), initNodes[numInitNode]->getYNumTile()));
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

			return;
		}
	}
}

void CGame::ForceNextWave()
{
	if(inGameState == GSG_Attack)
	{
		if(mp_timetowavesspawn[curr_wave] > (int)(waveTimer.GetCurrTime() * 0.001))
		{
			//IwTrace(WAVETIME, ("Minimum time to next wave is not reached yet."));
			return;
		}

		if(enemiesPerWave[curr_wave] > 0) return;

		if(mp_timetowaves[curr_wave] == 0)
			return;

		if(waveTimer.GetIsEnd() || waveTimer.GetIsStop())
			return;

		int CurExtraTime = (int)(waveTimer.GetCurrTime_Reversed() * 0.001);
		extra_time += CurExtraTime;

		//Al adelantar wave, que te de coins correspondientes al numero de segundos que queden, pero un maximo de 7 en caso de quedar mas de 7 segundos.
		if(CurExtraTime > 7) 
			mp_coins += 7;
		else
			mp_coins += CurExtraTime;

		curr_wave++;

		waveTimer.Reset();
		waveTimer.SetTimeLimit(mp_timetowaves[curr_wave] * 1000);
		waveTimer.Start();
	}
	else if(inGameState == GSG_Defending)
	{
		inGameState = GSG_Attack;
		bRenderAttackText = true;
		bCanOpenUnitMenu = false;
		unitMenu.Close();
		auxTimer.Start();
		waveTimer.SetTimeLimit(mp_timetowaves[0] * 1000);
		timer_change_theme.Stop();
		bChangeTheme = false;

		if(bUseOldAudioSystem)
		{
			if(!SOUND_MUSIC_MUTE)
			{
				if(bIsAudioPaused)
				{
					LastMusicStacked = Music_Ingame_02;
				}
				else
				{
					s3eAudioPlay("bgm/sd_ingame_02_theme_loop.mp3", 0);
					LastMusicStacked = Music_None;
				}
			}
		}
		else
		{
			Utils::PlayAudio(Music_Ingame_02);
		}
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
		inGameState = GSG_Loose;
		bPause = true;
		bIsInvincible = true;
		damageFadeScreen->FadeOut(255);
	}
}

void CGame::StressTest()
{
	return;
	if(!stresstest_timer.GetIsEnd())
		return;

	if(stresstest_num_units <= 0)
		return;

	srand(2009);

	/*
	IsoEnemyType random_enemy[] = {ENEMY_Soldier, 
		                           ENEMY_Convict,
								   ENEMY_Varon, 
								   ENEMY_Scientist, 
								   ENEMY_Tank, 
								   ENEMY_Rabbit};
	*/

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

	/*
	IsoSelknamType randTypes[] = {SELKNAM_Archer_Poisoner, SELKNAM_Archer_Retarder, SELKNAM_Archer_Base, 
		                          SELKNAM_Shaman_Healer  , SELKNAM_Shaman_Might   , SELKNAM_Shaman_Haste,
								  SELKNAM_Bowler_Stuner  , SELKNAM_Bowler_Rooter, SELKNAM_Bowler_Tmp3,
								  SELKNAM_Warrior_Base   , SELKNAM_Warrior_Offensive, SELKNAM_Warrior_Tank};
	*/
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
	if(mp_lifes <= 0)
		return;

	
	if(!bIsActiveObj)
		return;
	
	//int cnt = 0;
	for(int i = 0; i < MAX_ISO_ENEMIES; i++)
	{
		if(isoEnemyArr[i].GetEnemyType() != ENEMY_None)
		{
			return;
			//cnt++;
		}
	}

	//IwTrace(SELKNAM, ("%d enemies remaining.", cnt));

	//if(cnt > 0) return;

	IwTrace(SELKNAM, ("COINS: %d, LIFES: %d, WAVE: %d", mp_coins * 10, mp_lifes * 1000, 0));
	endingwin->SetCoinsBonus(mp_coins * 10);
	endingwin->SetLifes(mp_lifes);
	endingwin->SetStarsBonus(extra_time); //Aceleracion de las oleadas. * 100

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
			//IwTrace(SELKNAM, ("%d %d %d", isoEnemyArr[i].getSecToSpawn(), isoEnemyArr[i].getWaveToSpawn(), mp_timetowaves[isoEnemyArr[i].getWaveToSpawn() - 1]));
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

void CGame::MouseHack()
{
	if(bPause)
		return;

	if(bCanOpenUnitMenu && isoTerrainCursorGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()]->getCanPutUnit())
	{
		IsoTerrain* CurrentGrid = isoTerrainCursorGrid[mapPointer.getXNumTile()][mapPointer.getYNumTile()];
		//Checks if a unit is placed on isoTerrain
		if(CurrentGrid->CurrentIsoSelknam != NULL)
		{
			if(!unitManager.GetIsActive())
				unitManager.OpenWithKeyboard(CurrentGrid, &mp_coins);
		}
		else if(!unitMenu.GetIsActive())
		{
			unitMenu.OpenWithKeyboard(mp_coins);
		}
	}
}

void CGame::ActiveButtonMouseHack()
{

}