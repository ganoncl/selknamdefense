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

#include "IwResManager.h"
#include "IwTextParserITX.h"
#include "s3e.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "common.h"
#include "main.h"
#include "splashscreen.h"
#include "game.h"
#include "button.h"
#include "mm_menu.h"
#include "mm_popup.h"
#include "mainmenubase.h"
#include "mapselectbase.h"
#include "musichandler.h"
#include "IwSound.h"
#include "extramap.h"
#include "cursor.h"
#include "tutorial.h"
#include "texturehelper.h"
#include "utils.h"

MusicHandler* musicHandler = NULL;

bool bMouseBlocked = false;
bool bKeyboardBlocked = false;

s3eKeyboardEvent lastKeyEvent;

//cheat to change between any game states
int32 KeyboardHandler_main(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;
	
	//howtoRef->UpdateKeyboardEvents(event);
	if(event->m_Pressed)
	{
		if(event->m_Key == s3eKey1)
		{
			mainMenuState = GSM_TrialVersion;
		}
#ifdef IW_DEBUG
		if(event->m_Key == s3eKey5)
		{
			mainMenuState = GSM_TrialVersion;
		}
		else if(event->m_Key == s3eKey4)
		{
			mainMenuState = GSM_FinalScreen;
		}
		else if(event->m_Key == s3eKey6)
		{
			inGameState = GSG_Win;
		}
#endif
	}
	
	lastKeyEvent.m_Pressed = event->m_Pressed;
	lastKeyEvent.m_Key = event->m_Key;

	//The code above is just for test, this code is important so don't delete it.
	//If a button is pressed, then the mouse icon hides.
	if(event->m_Pressed)
	{
		//This means that the cursor will not render.
		//When the mouse starting to move again, this variable will be true.
		bMouseActive = false;
	}

	return 0;
}


//Mouse Motion
void HandleMotion_Main(s3ePointerMotionEvent* event) 
{
	if(bMouseBlocked)
		return;

	if(event == NULL)
		return;
	
	//return;
	if(bBlockOnQuit)
		return;

	if(gameState == GS_Game || gameState == GS_MainMenu || gameState == GS_MapSelection)
	{
		if(gameState != GS_SplashScreen && musicHandler != NULL && ENABLE_SOUND_BUTTONS)
			musicHandler->UpdateMotionEvents(event);
	}
}

//Mouse handle
void HandleClick_Main(s3ePointerEvent *event) 
{
	if(bMouseBlocked)
		return;

	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	
	if(gameState != GS_SplashScreen && musicHandler != NULL && ENABLE_SOUND_BUTTONS)
		musicHandler->UpdateClickEvents(event);
	
}


// updates per second
#define UPS 35

// throttle frame time at 10 fps (i.e. the game will slow down rather
// than having very low frame rate)
#define MAX_UPDATES 6

int GetUpdateFrame()
{
    return (int)(s3eTimerGetMs() / (1000/UPS));
}

void InitApplication()
{
	
	// Init my Application
	IwGxInit();
	// create the CIwResManager singleton
	IwResManagerInit();
	IwGxFontInit();
#ifdef IW_BUILD_RESOURCES
	// load available resource templates by parsing the resource templates file
	IwGetTextParserITX()->ParseFile("templates.itx");
#endif
	IwGxSetColClear(0x00, 0x00, 0x00, 0x00);
	// Turn all lighting off
	IwGxLightingOff();
	s3ePointerSetInt(S3E_POINTER_HIDE_CURSOR, 1);
#ifdef IW_DEBUG
	IwTraceSetFile("IwTrace.txt");
	IwTraceClearFile();
	s3ePointerSetInt(S3E_POINTER_HIDE_CURSOR, 0);
#endif

	IwSoundInit();
	s3eSoundSetInt(S3E_SOUND_DEFAULT_FREQ, 44100);
	#ifdef IW_BUILD_RESOURCES
		IwGetResManager()->AddHandler(new CIwResHandlerWAV);
	#endif
	//SCREEN FIX
		isoFormulaXOffset = (IwGxGetScreenWidth() * 0.5f);
		isoFormulaYOffset = mapYOffset;
}

void LoadFXSounds()
{
	Arrow_Shooting    = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("arrow_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Click          = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("click_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Explosion      = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("explosion_01",   IW_SOUND_RESTYPE_SPEC);
	FX_Knife_Sword_01 = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("knife_sword_01", IW_SOUND_RESTYPE_SPEC);
	FX_RollOver_02    = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("rollover_02",    IW_SOUND_RESTYPE_SPEC);
	FX_Shot_01        = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("shot_01",        IW_SOUND_RESTYPE_SPEC);
	FX_Star_Gained_02 = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("star_gained_02", IW_SOUND_RESTYPE_SPEC);  
	FX_MetalImpact    = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("metal_impact",   IW_SOUND_RESTYPE_SPEC);  
	FX_MetalImpact2   = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("metal_impact2",  IW_SOUND_RESTYPE_SPEC);
	FX_Coins          = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("coins",          IW_SOUND_RESTYPE_SPEC);
	FX_Heal           = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("heal",           IW_SOUND_RESTYPE_SPEC);
	FX_Bubbles        = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("bubbles",        IW_SOUND_RESTYPE_SPEC);
	FX_Silence		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("silence",        IW_SOUND_RESTYPE_SPEC);
}

void AddLoadedResourcesToArray(string myNewResource)
{
	IwTrace(NEWLOADING, ("AddLoadedResourcesToArray(): myNewResource: %s", myNewResource.c_str()));
	strcpy(totalResourcesLoaded[totalResourcesLoadedNum], myNewResource.c_str());
	IwTrace(NEWLOADING, ("AddLoadedResourcesToArray(): myNewResource: %s", totalResourcesLoaded[totalResourcesLoadedNum]));
	totalResourcesLoadedNum++;
}

void LoadTextures()
{
	static CIwFMat view = CIwFMat::g_Identity;

	commonResources[0] = "atlas_menu_0.group";
	commonResources[1] = "atlas_selknam_archer_0.group";
	commonResources[2] = "atlas_selknam_bowler_0.group";
	commonResources[3] = "atlas_selknam_melee_0.group";
	commonResources[4] = "atlas_enemy_0.group";
	commonResources[5] = "atlas_enemy_2.group";
	commonResources[6] = "atlas_gamebackground_0.group";
	commonResources[7] = "atlas_common_trial.group";
	commonResources[8] = "atlas_common_0.group";
	commonResources[9] = "atlas_common_1.group";
	commonResources[10] = "atlas_common_2.group";
	commonResources[11] = "atlas_worldmap.group";
	commonResources[12] = "atlas_terrain.group";
	commonResources[13] = "atlas_menu_1.group";
	commonResources[14] = "atlas_selknam_mage.group";
	commonResources[15] = "atlas_enemy_1.group";
	commonResources[16] = "atlas_props.group";
	commonResources[17] = "atlas_fx.group";
	commonResources[18] = "atlas_selknam_archer_1.group";
	commonResources[19] = "atlas_selknam_bowler_1.group";
	commonResources[20] = "atlas_selknam_melee_1.group";
	commonResources[21] = "game_sounds_fx.group";

	bool bCommonResourcesLoaded = false;


	int qnt_resources = 0;
	int maxQnt_resources = commonResourcesNum;
	int dots = 0;
	string str = "LOADING...";
	string ver = "";
	if(bIsFullVersion)
		ver += "FULL  VERSION : ";
	else
		ver += "TRIAL  VERSION : ";
	ver += version_number;
	//ver += "     DATE : ";
	//ver += version_date;
	float loadingBarWidth = 327.0f;
	float loadingBarHeight = 28.0f;
	float curBarLoadedLength = 0.0f;
	float chunkSizeBar = 0.0f;
	float loadingBarX = (IwGxGetScreenWidth() - loadingBarWidth) * 0.5f;
	float loadingBarY = (IwGxGetScreenHeight() - loadingBarHeight) * 0.5f;
	int percent = 0;
	char percent_str[5];
	sprintf(percent_str, "%d%%", percent);

	chunkSizeBar = loadingBarWidth / maxQnt_resources;

	IwGxSetOrtho(0, 1280, 0, 720, 1.0f, 512.0f);				
	IwGxSetViewMatrix(&view);
	IwGxSetModelMatrix(&view);

	while(qnt_resources <= maxQnt_resources && !s3eDeviceCheckQuitRequest())
	{
		curBarLoadedLength = chunkSizeBar * (qnt_resources);
		percent = (int)((qnt_resources * 100) / maxQnt_resources);
		sprintf(percent_str, "%d%%", percent);

		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
		//barra de fondo
		Utils::RenderColor(0xff333333, 0xff191919, CIwSVec2((int)loadingBarX, (int)loadingBarY), CIwSVec2((int)loadingBarWidth, (int)loadingBarHeight), true);
		IwGxFlush();
		//barra creciente
		Utils::RenderColor(0xff0EA317, 0xff08610D, CIwSVec2((int)loadingBarX, (int)loadingBarY), CIwSVec2((int)curBarLoadedLength, (int)loadingBarHeight), true);
		IwGxFlush();
		IwGxLightingOn();
		Utils::RenderText(percent_str, CIwRect(740, -30, 1280, 720), font_komikadisplay18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT,   IW_GX_FONT_ALIGN_MIDDLE);
		Utils::RenderText(str.c_str(), CIwRect(560, 50, 1280, 720), font_komikadisplaybold22, 0xffffffff, IW_GX_FONT_ALIGN_LEFT,   IW_GX_FONT_ALIGN_MIDDLE);
		Utils::RenderText(ver.c_str()   , CIwRect(32, 720 - 22 - 40, 1280, 720), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		IwGxLightingOff();

		if(!bCommonResourcesLoaded)
		{
			int tmpNum = qnt_resources;
			IwGetResManager()->LoadGroup(commonResources[tmpNum]);
			if(tmpNum == commonResourcesNum - 1)
				bCommonResourcesLoaded = true;
			AddLoadedResourcesToArray(Utils::GetUnformattedLoadedGroup(commonResources[tmpNum]));
			IwTrace(NEWLOADING, ("LOADING COMMON: tmpNum: %i, qtn_resources: %i, group to load: %s", tmpNum, qnt_resources, commonResources[tmpNum]));
		}
				
		qnt_resources++;
		
		IwGxFlush();
		IwGxSwapBuffers();

		//Yield required.
		s3eDeviceYield(1);
	}
}

void DestroyGroup(string str, int idx)
{
	//Yield required.
	s3eDeviceYield(1);
}

void UnLoadTextures()
{
	for(int i = 0; i < totalResourcesLoadedNum; i++)
	{
		IwTrace(NEWLOADING, ("UNLOAD: i: %i, group to unload: %s", i, totalResourcesLoaded[i]));
		IwGetResManager()->DestroyGroup(totalResourcesLoaded[i]);
		s3eDeviceYield(1);
	}
}


void TerminateApplication()
{
	s3ePointerSetInt(S3E_POINTER_HIDE_CURSOR, 0);
	IwSoundTerminate();
	//Yield required.
	s3eDeviceYield(1);
	IwGxFontTerminate();
	//Yield required.
	s3eDeviceYield(1);
	IwResManagerTerminate();
	//Yield required.
	s3eDeviceYield(1);
	//Terminate
	IwGxTerminate();
	//Yield required.
	s3eDeviceYield(1);
}

void LogsOnScreen()
{
	if(LAST_KEY_PRESSED)
	{
		char debugString[256];

		//sprintf(debugString, "last event Pressed %i , Keynum %i, GamePaused: %i \n1: SplashScreen\n2: MainMenu\n3: Game\n4: Map Selection\n5: Win Ending\n6: Loose Ending\n7: How to play\n8: Menu IG", lastKeyEvent.m_Pressed, lastKeyEvent.m_Key, bPause);
		sprintf(debugString, "`x0000ff Last key Pressed %i ", lastKeyEvent.m_Key);
		IwGxPrintString(5, 200, debugString, false);
	}

	/*
	Utils::RenderColor(0xff000000, CIwSVec2(0,0), CIwSVec2(1280, 32));
	IwGxFlush();
	Utils::RenderColor(0xff000000, CIwSVec2(0,0), CIwSVec2(32, 720));
	IwGxFlush();
	Utils::RenderColor(0xff000000, CIwSVec2(1280-32,0), CIwSVec2(32, 720));
	IwGxFlush();
	Utils::RenderColor(0xff000000, CIwSVec2(0, 720 - 22), CIwSVec2(1280, 22));
	IwGxFlush();
	*/
}

void InitializeFontsReferences()
{
	IwGetResManager()->LoadGroup("fonts.group");

	font_komikadisplaybold25 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplaybold25", "CIwGxFont");
	font_komikadisplay11     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay11", "CIwGxFont");
	font_komikadisplay14     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay14", "CIwGxFont");
	font_komikadisplay18     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay18", "CIwGxFont");
	font_komikadisplaybold18     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay18bold", "CIwGxFont");
	font_komikadisplay22     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay22", "CIwGxFont");
	font_komikadisplaybold22     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay22bold", "CIwGxFont");
	font_komikadisplaybold48     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay48bold", "CIwGxFont");
	font_komikadisplay28		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay28", "CIwGxFont");
}

void TerminateFontsReferences()
{
	font_komikadisplaybold25 = NULL;
	font_komikadisplay11     = NULL;
	font_komikadisplay14     = NULL;
	font_komikadisplay18     = NULL;
	font_komikadisplaybold18 = NULL;
	font_komikadisplay22     = NULL;
	font_komikadisplaybold22 = NULL;
	font_komikadisplaybold48 = NULL;
	font_komikadisplay28     = NULL;
}

// "main" is the S3E entry point
int main()
{
	bFxSoundMuted = true;

	//Init
	InitApplication();

	//Yield required.
	s3eDeviceYield(1);

	InitializeFontsReferences();

	//Yield required.
	s3eDeviceYield(1);

	LoadTextures();


	if(s3eDeviceCheckQuitRequest())
	{
		TerminateFontsReferences();
		//Yield required.
		s3eDeviceYield(1);
		UnLoadTextures();
		//Yield required.
		s3eDeviceYield(1);
		TerminateApplication();
		return 0;
	}

	LoadFXSounds();

	atlas_common_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_0")->GetResNamed("atlas_common_0", IW_GX_RESTYPE_TEXTURE);
	atlas_common_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_1")->GetResNamed("atlas_common_1", IW_GX_RESTYPE_TEXTURE);
	atlas_common_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_2")->GetResNamed("atlas_common_2", IW_GX_RESTYPE_TEXTURE);
	atlas_common_trial = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_trial")->GetResNamed("atlas_common_trial_0", IW_GX_RESTYPE_TEXTURE);
	atlas_menu_01_0   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_menu_0")->GetResNamed("atlas_menu_01_0", IW_GX_RESTYPE_TEXTURE);
	atlas_menu_01_1   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_menu_0")->GetResNamed("atlas_menu_01_1", IW_GX_RESTYPE_TEXTURE);
	atlas_menu_02_0   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_menu_1")->GetResNamed("atlas_menu_02_0", IW_GX_RESTYPE_TEXTURE);
	atlas_gamebackground_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_gamebackground_0")->GetResNamed("atlas_gamebackground_0", IW_GX_RESTYPE_TEXTURE);
	atlas_worldmap = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_worldmap")->GetResNamed("atlas_worldmap_0", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_01_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_0")->GetResNamed("atlas_enemy_01_1", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_01_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_0")->GetResNamed("atlas_enemy_01_2", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_02_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_0")->GetResNamed("atlas_enemy_02_1", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_02_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_0")->GetResNamed("atlas_enemy_02_2", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_03_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_1")->GetResNamed("atlas_enemy_03_1", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_04_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_1")->GetResNamed("atlas_enemy_04_1", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_04_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_1")->GetResNamed("atlas_enemy_04_2", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_05_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_2")->GetResNamed("atlas_enemy_05_1", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_06_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_2")->GetResNamed("atlas_enemy_06_1", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_07_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_2")->GetResNamed("atlas_enemy_07_1", IW_GX_RESTYPE_TEXTURE);
	atlas_enemy_07_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_enemy_2")->GetResNamed("atlas_enemy_07_2", IW_GX_RESTYPE_TEXTURE);
	atlas_selknam_archer_01_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_archer_0")->GetResNamed("atlas_selknam_archer_01_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_archer_01_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_archer_0")->GetResNamed("atlas_selknam_archer_01_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_archer_02_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_archer_0")->GetResNamed("atlas_selknam_archer_02_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_archer_02_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_archer_0")->GetResNamed("atlas_selknam_archer_02_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_archer_03_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_archer_1")->GetResNamed("atlas_selknam_archer_03_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_archer_03_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_archer_1")->GetResNamed("atlas_selknam_archer_03_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_melee_01_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_melee_0")->GetResNamed("atlas_selknam_melee_01_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_melee_01_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_melee_0")->GetResNamed("atlas_selknam_melee_01_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_melee_02_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_melee_0")->GetResNamed("atlas_selknam_melee_02_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_melee_02_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_melee_0")->GetResNamed("atlas_selknam_melee_02_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_melee_03_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_melee_1")->GetResNamed("atlas_selknam_melee_03_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_melee_03_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_melee_1")->GetResNamed("atlas_selknam_melee_03_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_mage_01_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_mage")->GetResNamed("atlas_selknam_mage_01_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_mage_01_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_mage")->GetResNamed("atlas_selknam_mage_01_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_mage_02_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_mage")->GetResNamed("atlas_selknam_mage_02_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_mage_02_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_mage")->GetResNamed("atlas_selknam_mage_02_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_mage_03_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_mage")->GetResNamed("atlas_selknam_mage_03_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_mage_03_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_mage")->GetResNamed("atlas_selknam_mage_03_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_bowler_01_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_bowler_0")->GetResNamed("atlas_selknam_bowler_01_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_bowler_01_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_bowler_0")->GetResNamed("atlas_selknam_bowler_01_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_bowler_02_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_bowler_1")->GetResNamed("atlas_selknam_bowler_03_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_bowler_02_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_bowler_1")->GetResNamed("atlas_selknam_bowler_03_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_bowler_03_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_bowler_0")->GetResNamed("atlas_selknam_bowler_02_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_selknam_bowler_03_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_selknam_bowler_0")->GetResNamed("atlas_selknam_bowler_02_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_terrain_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_terrain")->GetResNamed("atlas_terrain_0", IW_GX_RESTYPE_TEXTURE); 
	atlas_props_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_props")->GetResNamed("atlas_props_0", IW_GX_RESTYPE_TEXTURE); 
	atlas_props_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_props")->GetResNamed("atlas_props_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_props_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_props")->GetResNamed("atlas_props_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_fx_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_fx")->GetResNamed("atlas_fx_0", IW_GX_RESTYPE_TEXTURE); 
	atlas_fx_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_fx")->GetResNamed("atlas_fx_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_fx_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_fx")->GetResNamed("atlas_fx_2", IW_GX_RESTYPE_TEXTURE); 

	TextureHelper textureHelper;
	textureHelper.RetrieveAllTextures();

	ParserHelper::CheckIfSaveDataExists();
	ParserHelper::RetrieveGameData();

	// fade effect
	fade = new FadeScreen(0,0,0);

	// popup system
	popup = new PopUp();

	// create splash screen object
	SplashScreen *sScreen = new SplashScreen(5, 3, 15);

    // create game object
    CGame *pGame = new CGame;

	// create main menu object
	MainMenu *menu = new MainMenu;

	// create map selection object
	MapSelectionBase* mapselection = new MapSelectionBase();

	ExtraMaps* extramaps = new ExtraMaps();

	Cursor* cursor = new Cursor();
	cursor->InitCursor();

	Utils::UtilsInit();

	bFxSoundMuted = false;

	musicHandler = new MusicHandler(1100, 610);

	tutorial = new Tutorial(); 


	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_main, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Main, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_Main, NULL);


	for(int i = 0; i < MAX_FX_SOUNDS; i++)
	{
		fx_sound_limit[i][1] = 0;
		fx_sound_limit[i][0] = 1;
	}

	//s3eAudioSetInt(S3E_AUDIO_VOLUME, MUSIC_VOLUME);

	static CIwFMat view = CIwFMat::g_Identity;
    
	int currentUpdate = GetUpdateFrame();
    int nextUpdate = currentUpdate; 

	gameState = GS_SplashScreen;

	RenderData waterMarkRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_logo_badgames"));
	waterMarkRenderData.width = waterMarkRenderData.height = 256;
	CIwSVec2 waterMarkPosition = CIwSVec2((int)((IwGxGetScreenWidth() - waterMarkRenderData.width) * .5f), (int)((IwGxGetScreenHeight() - waterMarkRenderData.height) * .5f));
  
	// to exit correctly, applications should poll for quit requests
    while(!s3eDeviceCheckQuitRequest())
    {
        // run logic at a fixed frame rate (defined by UPS)

        // block until the next frame (don't render unless at
        // least one update has occurred)
        while(!s3eDeviceCheckQuitRequest())
        {
            nextUpdate = GetUpdateFrame();
            if( nextUpdate != currentUpdate )
                break;
            s3eDeviceYield(1);
        }

        // execute update steps
        int frames = nextUpdate - currentUpdate;
        frames = MIN(MAX_UPDATES, frames);
        while(frames--)
        {
			//init and end objects calls
			if(lastGameState != gameState)
			{
				//Init Objects
				if(gameState==GS_SplashScreen)
					sScreen->InitObject();
				else if(gameState==GS_Game)
					pGame->InitObject();
				else if(gameState==GS_MainMenu)
					menu->InitObject();
				else if(gameState==GS_MapSelection)
					mapselection->InitObject();
				else if(gameState==GS_ExtraMapSelection)
					extramaps->InitObject();
				else if(gameState==GS_Retry)
					fade->FadeIn(255);

				//End Objects
				if(lastGameState==GS_SplashScreen)
					sScreen->EndObject();
				else if(lastGameState==GS_Game)
					pGame->EndObject();
				else if(lastGameState==GS_MainMenu)
					menu->EndObject();
				else if(lastGameState==GS_MapSelection)
					mapselection->EndObject();
				else if(lastGameState==GS_ExtraMapSelection)
					extramaps->EndObject();
				else if(lastGameState==GS_Retry)
					fade->FadeOut(255);

				lastGameState = gameState;
			}

			//PopUp handler
			if(lastPopUpState != popUpState)
			{
				if(popUpState == GSP_None)
					popup->Off();
				else
					popup->On();
				lastPopUpState = popUpState;
			}

			fade->Update();
			IwGetSoundManager()->Update();	
			//FXSoundManager->Update();
			Utils::UtilsUpdate();
			//logic loop
			if(gameState==GS_SplashScreen)
				sScreen->Update();
			else if(gameState==GS_Game)
				pGame->Update();
			else if(gameState==GS_MainMenu)
				menu->Update();
			else if(gameState==GS_MapSelection)
				mapselection->Update();
			else if(gameState == GS_Retry)
				gameState = GS_Game;
			else if(gameState==GS_ExtraMapSelection)
				extramaps->Update();
			tutorial->Update();
        }
        currentUpdate = nextUpdate;


		

        //render
		// Clear the screen
		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
		//common render.
		IwGxSetOrtho(0, 1280, 0, 720, 1.0f, 512.0f);				
		IwGxSetViewMatrix(&view);
		IwGxSetModelMatrix(&view);

		//render per active object
		if(gameState==GS_SplashScreen)
			sScreen->Render();
		else if(gameState==GS_Game)
			pGame->Render();
		else if(gameState==GS_MainMenu)
			menu->Render();
		else if(gameState==GS_MapSelection)
			mapselection->Render();
		else if(gameState==GS_ExtraMapSelection)
			extramaps->Render();
	
		IwGxFlush();

		tutorial->Render();
		
		if(popUpState != GSP_None) 
			popup->Render();
		
		if(gameState != GS_SplashScreen && ENABLE_SOUND_BUTTONS && musicHandler != NULL)
			musicHandler->Render();
		
		if(bWaterMark)
		{
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(waterMarkRenderData.atlasName), waterMarkRenderData, waterMarkPosition, 255, 255, 255, 20);
		}

		cursor->Render();

		fade->Render();

		LogsOnScreen();
		
		IwGxFlush();
		// Swap buffers
		IwGxSwapBuffers();
        // S3E applications should yield frequently
        s3eDeviceYield();

		accumulated_frames++;
		if(accumulated_frames >= 15)
		{
			accumulated_frames = 0;
			for(int i = 0; i < MAX_FX_SOUNDS; i++)
				fx_sound_limit[i][1] = 0;
		}
			
    }

	/*
	if(!bIsFullVersion)
	{
		ParserHelper::ReturnDataToDefault();
	}
	*/

	bBlockOnQuit = true;

	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_main);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Main);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_Main);

	TerminateFontsReferences();
	
	cursor->EndCursor();
	delete cursor;

	if(sScreen->bIsActiveObj)
		sScreen->EndObject();
	delete sScreen;
    // clear up game object
	if(pGame->bIsActiveObj)
		pGame->EndObject();
    delete pGame;
	
	if(menu->bIsActiveObj)
		menu->EndObject();
	delete menu;

	if(mapselection->bIsActiveObj)
		mapselection->EndObject();
	delete mapselection;

	

	delete fade;

	delete popup;

	delete extramaps;
	

	

	//delete musicHandler;

	UnLoadTextures();
	//Yield required.
	s3eDeviceYield(1);
	// End application
	
	delete musicHandler;
	//musicHandler = NULL;
	tutorial->EndTutorial();
	delete tutorial;
	//delete FXSoundManager;
	
	TerminateApplication();

    return 0;
}