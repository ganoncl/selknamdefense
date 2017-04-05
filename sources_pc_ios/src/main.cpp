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
#include "s3eWindow.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "common.h"
#include "localization.h"
#include "main.h"
#include "splashscreen.h"
#include "game.h"
#include "button.h"
#include "mm_menu.h"
#include "mm_popup.h"
#include "mainmenubase.h"
#include "mapselectbase.h"
#include "IwSound.h"
#include "extramap.h"
#include "cursor.h"
#include "tutorial.h"
#include "survivalmodeconfig.h"
#include "timer.h"
#include "IwResHandlerSKEL.h"
#include "IwResHandlerANIM.h"
#include "IwResHandlerSKIN.h"
#include "IwResHandlerGEO.h"
#include "IwAnim.h"
#include "IwAnimCore.h"
#include "IwAnimSkel.h"
#include "IwGraphics.h"
#include "IwAnimPlayer.h"
#include "IwGraphics.h"
#include "IwGxPrint.h"
#include "IwModelBuilder.h"
#include "IwResManager.h"
#include "IwTextParserITX.h"
#include "s3eKeyboard.h"
#include "mm_settings.h"
#if !defined DESKTOPPLATFORM
#include "IwBilling.h"
#include "s3eOSReadString.h"
#include "s3eFlurry.h"
#include "s3eKiip.h"
#include "s3eChartBoost.h"
#include "s3eIOSBackgroundAudio.h"
#include "s3eIOSIAd.h"
//#include "LeadboltExt.h"
#endif
#if defined GAMECENTER
#include "s3eIOSGameCenter.h"
#endif
#include "model3d.h"
#include "captainboss.h"
#include "achievementshelper.h"
#include "leaderboardshelper.h"
#if defined DESKTOPPLATFORM
#include "s3eDialog.h"
#include "h/GamePad.h"
#endif

#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif

//Global Statistics.
Timer perMinuteTimer;

bool bMouseBlocked = false;
bool bKeyboardBlocked = false;

s3eKeyboardEvent lastKeyEvent;

int GroupLoaded[60];

int clickPressed = 0;

SplashScreen     *sScreen      = NULL;
CGame            *pGame        = NULL;
MainMenu         *mainmenu         = NULL;
MapSelectionBase *mapselection = NULL;
ExtraMaps        *extramaps    = NULL;
Cursor           *cursor       = NULL;

Timer closeGameTimer;

// throttle frame time at 10 fps (i.e. the game will slow down rather
// than having very low frame rate)
#define MAX_UPDATES 12

#pragma region DEVICE_CALLBACKS
//When the app is suspended/resumed by the device.
int32 DeviceSuspendCallback_Main(void* sys, void* userData)
{
	if(gameState == GS_Game
	   && !tutorial->IsActive()
	   && !dialogController->IsActive())
	{
		bPause = true;
		inGamePausedState = GSGP_MainMenuIG;
	}
#if !defined DESKTOPPLATFORM
	if(s3eIOSBackgroundAudioIsPlaying() == S3E_TRUE)
	{
		s3eAudioStop();
		IwGetSoundManager()->StopAll();
	}
#endif
	return 0;
}

//cheat to change between any game states
int32 KeyboardHandler_main(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(event->m_Pressed)
	{
		currKeyPressed = event->m_Key;
		//IwTrace(Xbox360Debug, ("%d", (int)currKeyPressed));
	}
	else
	{
		currKeyPressed = s3eKeyFirst;
	}
	
	//howtoRef->UpdateKeyboardEvents(event);
	if(event->m_Pressed)
	{
#ifdef CHEATSENABLED
		if(event->m_Key == s3eKey0)
		{
			if(achievementsHelper)
			{
				achievementsHelper->ResetAllAchievements();
			}
		}
		else if(event->m_Key == s3eKey8)
		{
			if(saveDataHelper)
				saveDataHelper->ResetGameDataCampaign();
		}
		else if(event->m_Key == s3eKey9)
		{
			if(saveDataHelper)
				saveDataHelper->ResetGameDataSurvival();
		}
		else if(event->m_Key == s3eKey1)
		{
			if(gameState == GS_SplashScreen)
			{
				gameDataCampaign.SetGameDataCampaignFullProgress();
				saveDataHelper->SaveGameDataCampaign();
			}
		}
		else if(event->m_Key == s3eKey2)
		{
			if(gameState == GS_SplashScreen)
			{
				gameDataSurvival.SetGameDataSurvivalAvailable();
				saveDataHelper->SaveGameDataSurvival();
			}

			//perform mask score
			int tmpRawScore = 0;
			LBCampaignScore myTmpLBScore;
			myTmpLBScore.level = 22;
			myTmpLBScore.totalScore = 57823;
			tmpRawScore = Utils::MaskLeadeboardScore(myTmpLBScore);
			IwTrace(MASK, ("Intermedia() ,rawValue %i",tmpRawScore));
			myTmpLBScore = Utils::UnMaskLeadeboardScore(tmpRawScore);
		}
		else if(event->m_Key == s3eKey3)
		{
			if(gameState == GS_Game && pGame)
			{
				pGame->setMPCoins(pGame->getMPCoins() + 10);
			}
		}
		else if(event->m_Key == s3eKey4)
		{
			//if(gameState == GS_MapSelection && mapSelectionState == MS_MapSelection)
			//{
			//	mapSelectionState = MS_Store;
			//}
			currentGameSpeedValue = 10.5f;
		}
		else if(event->m_Key == s3eKey5)
		{
			if(gameState == GS_Game && inGameState == GSG_Attack && pGame)
			{
				pGame->PerformWinGame();
			}
		}
		else if(event->m_Key == s3eKey6)
		{
			//captainBoss->SetActive(false);
			//gameState = GS_Retry;
			//inGameState = GSG_DynamicCredits;
			//currentMap = 26;
			if(pGame && gameState == GS_Game)
				pGame->SetIsInvincible(true);
		}
		else if(event->m_Key == s3eKey7)
		{
			//if(currentMap==26 && pGame && pGame->dynamicCredits)
			//{
			//	pGame->dynamicCredits->EasterEgg_Init();
			//}
			if(pGame && gameState == GS_Game)
				pGame->SetBreakEnemySpawn(true);
		}

#endif
		/*if(event->m_Key == s3eKey1)
		{
		if(pGame)
		pGame->SetWaveTimerSpeed(WTIMER_NORMAL);
		}
		else if(event->m_Key == s3eKey2)
		{
		if(pGame)
		pGame->SetWaveTimerSpeed(WTIMER_X2);
		}
		else if(event->m_Key == s3eKey3)
		{
		if(pGame)
		pGame->SetWaveTimerSpeed(WTIMER_X3);
		}*/
		/*
		if(event->m_Key == s3eKey5)
		{
			captainBoss->SetActive(false);
			gameState = GS_Retry;
			inGameState = GSG_DynamicCredits;
			currentMap = 26;
		}
		else if(event->m_Key == s3eKey6)
		{
			inGameState = GSG_Win;
		}
		else if(event->m_Key == s3eKey0)
		{
			//Utils::MoveCamera(100, CIwFVec2(0, 0), CIwFVec2(130 - xWindowOffset, 50 - yWindowOffset));
		}
		else if(event->m_Key == s3eKey9)
		{
			mainMenuState = GSM_TrialVersion;
		}
		else if(event->m_Key == s3eKey1)
		{
			inGameState = GSG_Loose;
		}
		else if(event->m_Key == s3eKey2)
		{
			captainBoss->DestroyCannons();
			//Utils::DrawCannonExplotion(605, 390, 1);
			//Utils::DrawCannonExplotion(695, 340, 1);
		}
		else if(event->m_Key == s3eKey3)
		{
			captainBoss->PlayEndingScene();
		}
		else if(event->m_Key == s3eKey4)
		{
			captainBoss->SetVisible(!captainBoss->GetIsVisible());
		}
		*/
	}
	
	lastKeyEvent.m_Pressed = event->m_Pressed;
	lastKeyEvent.m_Key = event->m_Key;

	////The code above is just for test, this code is important so don't delete it.
	////If a button is pressed, then the mouse icon hides.
	//if(event->m_Pressed)
	//{
	//	//This means that the cursor will not render.
	//	//When the mouse starting to move again, this variable will be true.
	//	bMouseActive = false;
	//}

	return 0;
}

int lastXMouse = -1;
int lastYMouse = -1;
int accumulatedMouseMovement = 0;

//Mouse Motion
void HandleMotion_Main(s3ePointerMotionEvent* event) 
{
	xMouse = event->m_x;
	yMouse = event->m_y;

	if(bMouseBlocked)
		return;

	if(event == NULL)
		return;
	
	//return;
	if(bBlockOnQuit)
		return;

	if(dialogController != NULL)
	{
		dialogController->UpdateMotionEvents(event);
	}

	if(lastXMouse == -1 || lastYMouse == -1)
	{
		lastXMouse = xMouse;
		lastYMouse = yMouse;
	}

	if(lastXMouse != xMouse || lastYMouse != yMouse)
	{
		if(activeControllerType != Controller_MOUSE)
		{
			int diffX = abs(lastXMouse - xMouse);
			int diffY = abs(lastYMouse - yMouse);
			int diff = diffX + diffY;

			accumulatedMouseMovement += diff;

			if(accumulatedMouseMovement > mouseDetectionSensibility)
			{
				activeControllerType = Controller_MOUSE;
				accumulatedMouseMovement = 0;
			}
		}

		lastXMouse = xMouse;
		lastYMouse = yMouse;
	}
}

//Mouse handle
void HandleClick_Main(s3ePointerEvent *event) 
{
	xMouse = event->m_x;
	yMouse = event->m_y;

	if(event->m_Pressed) clickPressed = 1;
	else clickPressed = 0;

	if(bMouseBlocked)
		return;

	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;
	
	if(dialogController != NULL)
	{
		dialogController->UpdateClickEvents(event);
	}
}
#pragma endregion

#pragma region MAIN_FUNCTIONS
int GetUpdateFrame()
{
    return (int)(s3eTimerGetMs() / (1000/UPS));
}

Achievement_t g_Achievements[] = 
{
	_ACH_ID( SD_ACHIEVEMENT_00, "First Approach" ),
	_ACH_ID( SD_ACHIEVEMENT_01, "Good Strategy" ),
	_ACH_ID( SD_ACHIEVEMENT_02, "Selknam Authority" ),
	_ACH_ID( SD_ACHIEVEMENT_03, "Honorable Tank" ),
	_ACH_ID( SD_ACHIEVEMENT_04, "It's about money" ),
	_ACH_ID( SD_ACHIEVEMENT_05, "Invincible" ),
	_ACH_ID( SD_ACHIEVEMENT_06, "A new challenge" ),
	_ACH_ID( SD_ACHIEVEMENT_07, "It's about time" ),
	_ACH_ID( SD_ACHIEVEMENT_08, "Super poisoner" ),
	_ACH_ID( SD_ACHIEVEMENT_09, "Debuffer" ),
	_ACH_ID( SD_ACHIEVEMENT_10, "OverBuffed" ),
	_ACH_ID( SD_ACHIEVEMENT_11, "Selknam Mastery" ),
	_ACH_ID( SD_ACHIEVEMENT_12, "What happened?" ),
	_ACH_ID( SD_ACHIEVEMENT_13, "Epic finale" ),
	_ACH_ID( SD_ACHIEVEMENT_14, "Big Secret" ),
};

#ifdef GAMECENTER
void InitializeGameCenter()
{
	s3eBool myResult = s3eIOSGameCenterAvailable();
	if(myResult == 0)
		bGameCenterAvailable = false;
	else
		bGameCenterAvailable = true;
	IwTrace(GAMECENTER, ("InitializeGameCenter() called, bGameCenterAvailable %i", bGameCenterAvailable));

}

static void AuthenticationCallbackGameCenter(s3eIOSGameCenterError* error, void* userData)
{
	IwTrace(GAMECENTER, ("AuthenticationCallback() called, bGameCenterAuthenticated"));
	if (*error != S3E_IOSGAMECENTER_ERR_NONE)
	{
		bGameCenterAuthenticated = false;
		IwTrace(GAMECENTER, ("AuthenticationCallback() called, Error on Authentication"));
		return;
	}

	bGameCenterAuthenticated = true;
	const char* alias = s3eIOSGameCenterGetString(S3E_IOSGAMECENTER_LOCAL_PLAYER_ALIAS);

	if (strlen(alias) > 0)
		strcpy(gameCenterUserName, alias);
	else
	{
		strcpy(gameCenterUserName, "!user alias error!");
		bGameCenterAuthenticated = false;
	}
}

// Authenticate local player - prerequisite for other funcs to be used
static void AuthenticateLocalHandlerGameCenter()
{
	// Start new attempt
	if (!bGameCenterAuthenticated)
		bGameCenterAuthenticated = s3eIOSGameCenterGetInt(S3E_IOSGAMECENTER_LOCAL_PLAYER_IS_AUTHENTICATED) ? true : false;

	IwTrace(GAMECENTER, ("AuthenticateLocalHandler() called, bGameCenterAuthenticated %i", bGameCenterAuthenticated));

	if (bGameCenterAuthenticated)
		return;

	s3eIOSGameCenterAuthenticate(AuthenticationCallbackGameCenter, NULL);
}

#endif

void InitializeSteam_Step0()
{
#ifdef STEAMREQUIRED
	bSteamAvailable = SteamAPI_Init();
	IwTrace(STEAMWORKS, ("InitializeSteam_Step0() called, bSteamAvailable %i", bSteamAvailable));

	if(!bSteamAvailable)
		return;

	bSteam_LoggedOn = SteamUser()->BLoggedOn();
	IwTrace(STEAMWORKS, ("InitializeSteam_Step0() called, bSteam_LoggedOn %i", bSteam_LoggedOn));

	steam_PersonalName = SteamFriends()->GetPersonaName();
	IwTrace(STEAMWORKS, ("InitializeSteam_Step0() called, steam_PersonalName %s", steam_PersonalName));

	steamAppId = SteamUtils()->GetAppID();
	IwTrace(STEAMWORKS, ("InitializeSteam_Step0() called, steamAppId %i", steamAppId));
#endif
}


void InitializeSteam_Step1()
{
	// Initialize SteamAPI, if this fails we bail out since we depend on Steam for lots of stuff.
	// You don't necessarily have to though if you write your code to check whether all the Steam
	// interfaces are NULL before using them and provide alternate paths when they are unavailable.
	//
	// This will also load the in-game steam overlay dll into your process.  That dll is normally
	// injected by steam when it launches games, but by calling this you cause it to always load,
	// even when not launched via steam.

#ifdef STEAMREQUIRED
	if(!bSteamAvailable)
		return;

	g_SteamAchievements = new CSteamAchievements(g_Achievements, 15);

	g_SteamLeaderboards = new CSteamLeaderboards();

	steam_AppAllLanguages = SteamApps()->GetAvailableGameLanguages();
	steam_AppCurrentLanguage = SteamApps()->GetCurrentGameLanguage();
	if(strcmp(steam_AppCurrentLanguage, "english") == 0)
		gameLang = GAMELANG_INT;
	else if(strcmp(steam_AppCurrentLanguage, "spanish") == 0)
		gameLang = GAMELANG_ESN;

	IwTrace(STEAMWORKS, ("InitializeSteam_Step1() called, steam_AppAllLanguages %s", steam_AppAllLanguages));
	IwTrace(STEAMWORKS, ("InitializeSteam_Step1() called, steam_AppCurrentLanguage %s", steam_AppCurrentLanguage));

#endif
}

void TerminateSteam()
{
#ifdef STEAMREQUIRED
	if(bSteamAvailable)
	{
		if(saveDataHelper)
			saveDataHelper->RemoveSteamRemoteStorageRef();
		// Shutdown Steam
		SteamAPI_Shutdown();
		// Delete the SteamAchievements object
		if(g_SteamAchievements!=NULL)
			delete g_SteamAchievements;
		if(g_SteamLeaderboards!=NULL)
			delete g_SteamLeaderboards;
	}
#endif
}

void GetPlatformData()
{
	currentDeviceOS = (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS);
	currentDeviceLanguage = (s3eDeviceLanguage)s3eDeviceGetInt(S3E_DEVICE_LANGUAGE);
	currentDeviceID = s3eDeviceGetString(S3E_DEVICE_ID);

	if((strcmp(currentDeviceID, "iPhone3,1") == 0) || (strcmp(currentDeviceID, "iPhone3,2") == 0) || (strcmp(currentDeviceID, "iPhone3,3") == 0) ||
		(strcmp(currentDeviceID, "iPhone4,1") == 0) || (strcmp(currentDeviceID, "iPhone5,1") == 0) || (strcmp(currentDeviceID, "iPhone5,2") == 0) ||
		(strcmp(currentDeviceID, "iPhone5,3") == 0) || (strcmp(currentDeviceID, "iPhone5,4") == 0) || (strcmp(currentDeviceID, "iPhone6,1") == 0) ||
		(strcmp(currentDeviceID, "iPhone6,2") == 0) || (strcmp(currentDeviceID, "iPod4,1") == 0) || (strcmp(currentDeviceID, "iPod5,1") == 0))
		bUseHDImages = false;



	IwTrace(PLATFORMDATA, ("GetPlatformData() called, currentDeviceOS %i", currentDeviceOS));
	IwTrace(PLATFORMDATA, ("GetPlatformData() called, currentDeviceLanguage %i", currentDeviceLanguage));
	IwTrace(PLATFORMDATA, ("GetPlatformData() called, currentDeviceID %s", currentDeviceID));
}

void LocalizeGame()
{
#if !defined DESKTOPPLATFORM
	gameLang = GAMELANG_INT;
	if(currentDeviceLanguage == S3E_DEVICE_LANGUAGE_SPANISH)
		gameLang = GAMELANG_ESN;
#endif
	LocalizationSystem::RetrieveLocalization(gameLang);
}

void SetInGameCameraOffset()
{
	int myXDiff = (int)currentGameWidth - 1024;
	int myYDiff = (int)currentGameHeight - 768;
	xMinOffset = -140;
	xMaxOffset = 755 - myXDiff;
	yMinOffset = -70;
	yMaxOffset = 240 - myYDiff;
}

#if defined DESKTOPPLATFORM
void HandleScreenOnDesktopPlatform()
{
	if (!s3eWindowAvailable())
		return;

#ifdef STEAMREQUIRED
	if(!bSteamAvailable)
		return;
	else if(bSteamAvailable && steamAppId != 306260)
		return;
#endif
	if(strcmp(currentDeviceID, "Windows Simulator") == 0)
		return;
	
	//Get the number of display modes
	displayNumModes = 0;
	s3eWindowGetDisplayModes(NULL, &displayNumModes);
	IwTrace(SCREENDATA, ("HandleScreenOnDesktopPlatform() called, number of display modes, displayNumModes: %i", displayNumModes));

	//Retrieve all possible display modes
	displayModes = new s3eWindowDisplayMode[displayNumModes];
	s3eWindowGetDisplayModes(displayModes, &displayNumModes);

	int desiredDisplayWidth = 0;
	int desiredDisplayHeight = 0;

	s3eBool bFullScreenSuccess = false;

	for (int i = 0; i < displayNumModes; i++)
	{
		IwTrace(SCREENDATA, ("HandleScreenOnDesktopPlatform() called, display available %i, %i x %i , bpp %i" , i, displayModes[i].m_Width, displayModes[i].m_Height, displayModes[i].m_Bpp));
	}

	//Set Screen as procedural check
	//Check for 1280 x 720 Any
	if(!bFullScreenSuccess)
	{
		for (int i = 0; i < displayNumModes; i++)
		{
			if(displayModes[i].m_Width == 1280 && displayModes[i].m_Height == 720)
			{
				desiredDisplayWidth = displayModes[i].m_Width;
				desiredDisplayHeight = displayModes[i].m_Height;
				g_FullscreenMode = displayModes[i];
				s3eWindowResize(desiredDisplayWidth, desiredDisplayHeight);
				bFullScreenSuccess = s3eWindowIsFullscreenResolutionSupported(desiredDisplayWidth, desiredDisplayHeight);
				//bFullScreenSuccess = s3eWindowSetFullscreen(&g_FullscreenMode);
				IwTrace(SCREENDATA, ("HandleScreenOnDesktopPlatform() try to set %i x %i result: %i" ,displayModes[i].m_Width, displayModes[i].m_Height, bFullScreenSuccess));
			}
		}
	}

	//Check for 1280 x 800 OSX
	if(currentDeviceOS == S3E_OS_ID_OSX && !bFullScreenSuccess)
	{
		for (int i = 0; i < displayNumModes; i++)
		{
			if(displayModes[i].m_Width == 1280 && displayModes[i].m_Height == 800)
			{
				desiredDisplayWidth = displayModes[i].m_Width;
				desiredDisplayHeight = displayModes[i].m_Height;
				g_FullscreenMode = displayModes[i];
				s3eWindowResize(desiredDisplayWidth, desiredDisplayHeight);
				bFullScreenSuccess = s3eWindowIsFullscreenResolutionSupported(desiredDisplayWidth, desiredDisplayHeight);
				//bFullScreenSuccess = s3eWindowSetFullscreen(&g_FullscreenMode);
				IwTrace(SCREENDATA, ("HandleScreenOnDesktopPlatform() try to set %i x %i result: %i" ,displayModes[i].m_Width, displayModes[i].m_Height, bFullScreenSuccess));
			}
		}
	}

	//Check for 1152 x 720 OSX
	if(currentDeviceOS == S3E_OS_ID_OSX && !bFullScreenSuccess)
	{
		for (int i = 0; i < displayNumModes; i++)
		{
			if(displayModes[i].m_Width == 1152 && displayModes[i].m_Height == 720)
			{
				desiredDisplayWidth = displayModes[i].m_Width;
				desiredDisplayHeight = displayModes[i].m_Height;
				g_FullscreenMode = displayModes[i];
				s3eWindowResize(desiredDisplayWidth, desiredDisplayHeight);
				bFullScreenSuccess = s3eWindowIsFullscreenResolutionSupported(desiredDisplayWidth, desiredDisplayHeight);
				//bFullScreenSuccess = s3eWindowSetFullscreen(&g_FullscreenMode);
				IwTrace(SCREENDATA, ("HandleScreenOnDesktopPlatform() try to set %i x %i result: %i" ,displayModes[i].m_Width, displayModes[i].m_Height, bFullScreenSuccess));
			}
		}
	}

	//Check for 1024 x 768 Any
	if(!bFullScreenSuccess)
	{
		for (int i = 0; i < displayNumModes; i++)
		{
			if(displayModes[i].m_Width == 1024 && displayModes[i].m_Height == 768)
			{
				desiredDisplayWidth = displayModes[i].m_Width;
				desiredDisplayHeight = displayModes[i].m_Height;
				g_FullscreenMode = displayModes[i];
				s3eWindowResize(desiredDisplayWidth, desiredDisplayHeight);
				bFullScreenSuccess = s3eWindowIsFullscreenResolutionSupported(desiredDisplayWidth, desiredDisplayHeight);
				//bFullScreenSuccess = s3eWindowSetFullscreen(&g_FullscreenMode);
				IwTrace(SCREENDATA, ("HandleScreenOnDesktopPlatform() try to set %i x %i result: %i" ,displayModes[i].m_Width, displayModes[i].m_Height, bFullScreenSuccess));
			}
		}
	}

	displayWidthSelected = desiredDisplayWidth;
	displayHeightSelected = desiredDisplayHeight;
}
#endif

void SetCurrentResolutionData()
{
	currentDisplayWidth = IwGxGetDisplayWidth();
	currentDisplayHeight = IwGxGetDisplayHeight();
	currentScreenWidth = IwGxGetScreenWidth();
	currentScreenHeight = IwGxGetScreenHeight();
	currentDeviceWidth = IwGxGetDeviceWidth();
	currentDeviceHeight = IwGxGetDeviceHeight();
	currentSurfaceWidth = s3eSurfaceGetInt(S3E_SURFACE_DEVICE_WIDTH);
	currentSurfaceHeight = s3eSurfaceGetInt(S3E_SURFACE_DEVICE_HEIGHT);

	currentGameWidth = currentSurfaceWidth;
	currentGameHeight = currentSurfaceHeight;

	//secure check
	if(currentGameHeight > currentGameWidth)
	{
		currentGameWidth = currentSurfaceHeight;
		currentGameHeight = currentSurfaceWidth;
	}
#if defined DESKTOPPLATFORM
	if(displayWidthSelected != 0 && displayHeightSelected!=0)
	{
		currentGameWidth = displayWidthSelected;
		currentGameHeight = displayHeightSelected;
	}

#endif

	if(currentGameHeight == 540)
		gameHeight = HEIGHT_540;
	else if(currentGameHeight == 600)
		gameHeight = HEIGHT_600;
	else if(currentGameHeight == 640)
		gameHeight = HEIGHT_640;
	else if(currentGameHeight == 720)
		gameHeight = HEIGHT_720;
	else if(currentGameHeight == 768)
		gameHeight = HEIGHT_768;
	else if(currentGameHeight == 800)
		gameHeight = HEIGHT_800;


	IwTrace(PLATFORMDATA, ("SetCurrentResolutionData() called, currentDisplaySize %i x %i", currentDisplayWidth, currentDisplayHeight));
	IwTrace(PLATFORMDATA, ("SetCurrentResolutionData() called, currentScreenSize %i x %i", currentScreenWidth, currentScreenHeight));
	IwTrace(PLATFORMDATA, ("SetCurrentResolutionData() called, currentDeviceSize %i x %i", currentDeviceWidth, currentDeviceHeight));
	IwTrace(PLATFORMDATA, ("SetCurrentResolutionData() called, currentSurfaceSize %i x %i", currentSurfaceWidth, currentSurfaceHeight));
}

void ApplySavedSettingsOnInit()
{
	s3eAudioSetInt(S3E_AUDIO_VOLUME, currentAudioVolume);
	currentAudioVolume = s3eAudioGetInt(S3E_AUDIO_VOLUME);
	IwTrace(SELKNAMAUDIO, ("InitApplication() called, currentMusicVolume %i", currentAudioVolume));

	s3eSoundSetInt(S3E_SOUND_VOLUME, currentSoundVolume);
	currentSoundVolume = s3eSoundGetInt(S3E_SOUND_VOLUME);
	IwTrace(SELKNAMAUDIO, ("InitApplication() called, currentSoundFXVolume %i", currentSoundVolume));

#if !defined DESKTOPPLATFORM
	if(bAllMuted)
	{
		s3eAudioSetInt(S3E_AUDIO_VOLUME, 0);
		currentAudioVolume = s3eAudioGetInt(S3E_AUDIO_VOLUME);

		s3eSoundSetInt(S3E_SOUND_VOLUME, 0);
		currentSoundVolume = s3eSoundGetInt(S3E_SOUND_VOLUME);
	}
#endif

	if(strcmp(currentDeviceID, "Windows Simulator") == 0)
		return;
#if defined DESKTOPPLATFORM
	if(bIsFullScreenOption)
		s3eWindowSetFullscreen(&g_FullscreenMode);
	else
		s3eWindowSetFullscreen(NULL);
#endif
}

void InitApplication()
{
	
	// Init my Application
	s3eDeviceYield(1);
	IwGxInit();

	GetPlatformData();

	//Screen Handle
#if defined DESKTOPPLATFORM
	HandleScreenOnDesktopPlatform();
#endif

	saveDataHelper = new SaveDataHelper();
	saveDataHelper->LoadGameLocalData();

	ApplySavedSettingsOnInit();

	SetCurrentResolutionData();

	//Hack to avoid Landscape bug:
	IwGxTickUpdate();
	IwGxFlush();
	IwGxSwapBuffers();

	SetInGameCameraOffset();

	if(bLandscapeHack)
		s3eSurfaceSetInt( S3E_SURFACE_DEVICE_ORIENTATION_LOCK, S3E_SURFACE_LANDSCAPE );

	s3eDeviceYield(1);

	IwGraphicsInit();
	IwAnimInit();
	// create the CIwResManager singleton
	IwResManagerInit();

	IwGxFontInit();
#ifdef IW_BUILD_RESOURCES
	// load available resource templates by parsing the resource templates file
	IwGetTextParserITX()->ParseFile("templates.itx");

	if (IwGetResManager()->m_Flags & CIwResManager::BUILDRES_F )
		IwGetTextParserITX()->ParseFile("resource_template.itx");
#endif
	IwGxSetColClear(0x00, 0x00, 0x00, 0x00);
	// Turn all lighting off
	IwGxLightingOff();
#if defined DESKTOPPLATFORM
	s3ePointerSetInt(S3E_POINTER_HIDE_CURSOR, 1);
#endif
	if(strcmp(currentDeviceID, "Windows Simulator") == 0)
		s3ePointerSetInt(S3E_POINTER_HIDE_CURSOR, 0);
#ifdef IW_DEBUG
	IwTraceSetFile("IwTrace.txt");
	IwTraceClearFile();
#endif

	IwSoundInit();
	s3eSoundSetInt(S3E_SOUND_DEFAULT_FREQ, 44100);
	#ifdef IW_BUILD_RESOURCES
		IwGetResManager()->AddHandler(new CIwResHandlerWAV);
		//IwGetResManager()->AddHandler(new CIwResHandlerGEO);
	#endif

	//SCREEN FIX
	isoFormulaXOffset = 576;
	isoFormulaYOffset = mapYOffset;

	//AUDIO OPTION
	if(bNoMusic)
	{
		s3eAudioSetInt(S3E_AUDIO_VOLUME, 0);
	}
#if defined DESKTOPPLATFORM
	s3eWindowSetTitle("Selknam Defense");
#endif
}

void LoadFXSounds()
{
	FX_Arrow_Shooting_01    = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("arrow_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Arrow_Shooting_02    = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("arrow_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Click_01          = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("click_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Click_02          = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("click_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Explosion      = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("explosion_01",   IW_SOUND_RESTYPE_SPEC);
	FX_Knife_Sword_01 = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("knife_sword_01", IW_SOUND_RESTYPE_SPEC);
	FX_Knife_Sword_02 = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("knife_sword_02", IW_SOUND_RESTYPE_SPEC);
	FX_Knife_Sword_03 = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("knife_sword_03", IW_SOUND_RESTYPE_SPEC);
	FX_RollOver_02    = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("rollover_02",    IW_SOUND_RESTYPE_SPEC);
	FX_Shot_01        = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("shot_01",        IW_SOUND_RESTYPE_SPEC);
	FX_Star_Gained_02 = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("star_gained_02", IW_SOUND_RESTYPE_SPEC);  
	FX_MetalImpact_01    = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("metal_impact",   IW_SOUND_RESTYPE_SPEC);  
	FX_MetalImpact_02   = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("metal_impact2",  IW_SOUND_RESTYPE_SPEC);
	FX_Coins          = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("coins",          IW_SOUND_RESTYPE_SPEC);
	FX_Heal           = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("heal",           IW_SOUND_RESTYPE_SPEC);
	FX_BuffOn		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("buff_on",        IW_SOUND_RESTYPE_SPEC);
	FX_Bubbles        = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("bubbles",        IW_SOUND_RESTYPE_SPEC);
	FX_Silence		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("silence",        IW_SOUND_RESTYPE_SPEC);
	FX_Punch_01		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("punch_01",        IW_SOUND_RESTYPE_SPEC);
	FX_Punch_02		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("punch_02",        IW_SOUND_RESTYPE_SPEC);
	FX_Punch_03		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("punch_03",        IW_SOUND_RESTYPE_SPEC);
	FX_Punch_04		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("punch_04",        IW_SOUND_RESTYPE_SPEC);
	FX_Punch_05		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("punch_05",        IW_SOUND_RESTYPE_SPEC);
	FX_LooseALive		  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("lose_a_live",        IW_SOUND_RESTYPE_SPEC);
	FX_Sell_Unit	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("sell_unit",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Baron	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_baron",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Convict	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_convict",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Pirate	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_pirate",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Rabbit	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_rabbit",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Scientist	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_scientist",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Soldier	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_soldier",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Tank	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_tank",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Archer_Deploy_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_archer_deploy_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Archer_Deploy_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_archer_deploy_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Archer_Select_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_archer_select_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Archer_Select_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_archer_select_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Bowler_Deploy_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_bowler_deploy_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Bowler_Deploy_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_bowler_deploy_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Bowler_Select_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_bowler_select_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Bowler_Select_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_bowler_select_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Mage_Deploy_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_mage_deploy_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Mage_Deploy_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_mage_deploy_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Mage_Select_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_mage_select_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Mage_Select_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_mage_select_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Warrior_Deploy_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_warrior_deploy_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Warrior_Deploy_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_warrior_deploy_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Warrior_Select_01	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_warrior_select_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Selknam_Warrior_Select_02	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("selknam_warrior_select_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Selknam_Archer	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_selknam_archer",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Selknam_Bowler	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_selknam_bowler",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Selknam_Mage	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_selknam_mage",       IW_SOUND_RESTYPE_SPEC);
	FX_Dying_Selknam_Warrior	  = (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("dying_selknam_warrior",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_angry_after_paincycle= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_angry_after_paincycle",       IW_SOUND_RESTYPE_SPEC);
	FX_boss_comment_cycle01_backward_01= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle01_backward_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle01_moving_forward_01= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle01_moving_forward_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle01_prefire= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle01_prefire",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle02_backward_01= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle02_backward_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle02_moving_forward_01= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle02_moving_forward_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle02_prefire= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle02_prefire",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle03_backward_01= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle03_backward_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle03_backward_02= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle03_backward_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle03_moving_forward_01= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle03_moving_forward_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_comment_cycle03_prefire= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_comment_cycle03_prefire",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_dying= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_dying",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_pain_01= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_pain_01",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_pain_02= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_pain_02",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_pain_03= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_pain_03",       IW_SOUND_RESTYPE_SPEC);
	FX_Cannon_ship= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("cannon_ship",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_appear_explosion= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_appear_explosion",       IW_SOUND_RESTYPE_SPEC);
	FX_Boss_laugh= (CIwSoundSpec*)IwGetResManager()->GetGroupNamed("game_sounds_fx")->GetResNamed("boss_laugh",       IW_SOUND_RESTYPE_SPEC);


	//for(int i = 0; i < MAX_FX_SOUNDS; i++)
	//{
	//	fx_sound_limit[i][1] = 0;
	//	fx_sound_limit[i][0] = 1;
	//}
}

bool bIAPRegistered = false;

int32 maxMemUsed = 0;

void LogsOnScreen()
{
	//char debugString0[256];
	//sprintf(debugString0, "`x0000ff bGameCenterAuthenticated %i ", bGameCenterAuthenticated);
	//IwGxPrintString(5, 200, debugString0, false);
	//sprintf(debugString0, "`x0000ff bItemPurchasedSuccess %i ", bItemPurchasedSuccess);
	//IwGxPrintString(5, 225, debugString0, false);
	//sprintf(debugString0, "`x0000ff bItemRestoredSuccess %i ", bItemRestoredSuccess);
	//IwGxPrintString(5, 250, debugString0, false);
	//sprintf(debugString0, "`x0000ff bIwBillingIosAvailable %i ", bIwBillingIosAvailable);
	//IwGxPrintString(5, 275, debugString0, false);

	//return;
	//char debugString0[256];

	//// Displaying Heap information
	//int32 g_CurrentHeap = s3eMemoryGetInt(S3E_MEMORY_HEAP);  // Current Heap
	//int32 heapSize = s3eMemoryGetInt(S3E_MEMORY_SIZE); // Total size of the current heap
	//int32 heapFree = s3eMemoryGetInt(S3E_MEMORY_FREE); // Total amount of free space left on the current heap
	//int32 heapUsed = s3eMemoryGetInt(S3E_MEMORY_USED); // Free memory minus heap size

	//if(heapUsed > maxMemUsed)
	//	maxMemUsed = heapUsed;

	//sprintf(debugString0, "`x0000ffNumber Heaps : %i\nCurrent Heap : %d\nHeap size    : %d\nFree memory  : %d\nUsed memory  : %d\nMax Used memory  : %d",
	//	1, g_CurrentHeap, heapSize, heapFree, heapUsed, maxMemUsed);
	//IwGxPrintString(5, 150, debugString0, false);

	//char debugString0[256];
	//sprintf(debugString0, "`x0000ff bKiipAvailable %i ", bKiipAvailable);
	//IwGxPrintString(5, 200, debugString0, false);

	//return;

	
	//if(LAST_KEY_PRESSED)
	//{
	//	char debugString[256];
	//	sprintf(debugString, "`x0000ff Last key Pressed %i ", lastKeyEvent.m_Key);
	//	IwGxPrintString(5, 150, debugString, false);
	//}

	//char debugString1[256];
	//char debugString2[256];

	//sprintf(debugString0, "`x0000ff bLoadAllResources %i ", bLoadAllResources);
	//IwGxPrintString(5, 200, debugString0, false);
	//sprintf(debugString0, "`x0000ff bUseHDImages %i ", bUseHDImages);
	//IwGxPrintString(5, 225, debugString0, false);
	//sprintf(debugString0, "`x0000ff Steam LoggedOn %i ", bSteam_LoggedOn);
	//IwGxPrintString(5, 225, debugString0, false);
	//sprintf(debugString1, "`x0000ff Steam Personal Name: %s ", steam_PersonalName);
	//IwGxPrintString(5, 250, debugString1, false);
	//sprintf(debugString1, "`x0000ff Steam App Id %i ", steamAppId);
	//IwGxPrintString(5, 275, debugString1, false);
	//sprintf(debugString2, "`x0000ff Steam All Languages %s ", steam_AppAllLanguages);
	//IwGxPrintString(5, 300, debugString2, false);
	//sprintf(debugString2, "`x0000ff Steam Current Language %s ", steam_AppCurrentLanguage);
	//IwGxPrintString(5, 325, debugString2, false);

	//sprintf(debugString0, "`x0000ff numcurrent %i ", sound_arrows_numcurrent);
	//IwGxPrintString(5, 200, debugString0, false);

	//sprintf(debugString0, "`x0000ff milisecstamp %i ", sound_arrows_milisecstamp);
	//IwGxPrintString(5, 225, debugString0, false);

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

	bLoadAllResources = false;
#ifdef IW_BUILD_RESOURCES
	bLoadAllResources = true;
#endif

	commonResources[0] = "atlas_bkg_menu_others.group";
	commonResources[1] = "atlas_boss.group";
	commonResources[2] = "atlas_common_textures/atlas_common_textures_0.group";
	commonResources[3] = "atlas_common_textures/atlas_common_textures_1.group";
	commonResources[4] = "atlas_common_textures/atlas_common_textures_2.group";
	commonResources[5] = "atlas_common_textures/atlas_common_textures_3.group";
	commonResources[6] = "atlas_common_textures/atlas_common_textures_4.group";
	commonResources[7] = "atlas_small_textures/atlas_small_textures.group";
	commonResources[8] = "atlas_common_trial_0.group";
	commonResources[9] = "atlas_fx.group";
	commonResources[10] = "atlas_props.group";
	commonResources[11] = "atlas_terrain.group";
	commonResources[12] = "game_sounds_fx.group";
	commonResources[13] = "ship.group";
	commonResources[14] = "atlas_others/atlas_others.group";

	bool bCommonResourcesLoaded = false;

	nonHDImagesResources[0] = "atlas_bkg_small_0.group";
	nonHDImagesResources[1] = "atlas_bkg_small_1.group";
	nonHDImagesResources[2] = "game_selknams/atlas_selknam_archer_0.group";
	nonHDImagesResources[3] = "game_selknams/atlas_selknam_archer_1.group";
	nonHDImagesResources[4] = "game_selknams/atlas_selknam_bowler_0.group";
	nonHDImagesResources[5] = "game_selknams/atlas_selknam_bowler_1.group";
	nonHDImagesResources[6] = "game_selknams/atlas_selknam_mage.group";
	nonHDImagesResources[7] = "game_selknams/atlas_selknam_melee_0.group";
	nonHDImagesResources[8] = "game_selknams/atlas_selknam_melee_1.group";
	nonHDImagesResources[9] = "game_enemies/atlas_enemy_0.group";
	nonHDImagesResources[10] = "game_enemies/atlas_enemy_1.group";
	nonHDImagesResources[11] = "game_enemies/atlas_enemy_2.group";

	bool bNonHDImagesResourcesLoaded = false;

	hDImagesResources[0] = "atlas_bkg_gameplay.group";
	hDImagesResources[1] = "atlas_bkg_menu_foreground.group";
	hDImagesResources[2] = "atlas_bkg_menu_sky.group";
	hDImagesResources[3] = "atlas_bkg_worldmap.group";
	hDImagesResources[4] = "game_selknams_hd/atlas_hd_selknam_melee_1.group";
	hDImagesResources[5] = "game_selknams_hd/atlas_hd_selknam_melee_2.group";
	hDImagesResources[6] = "game_selknams_hd/atlas_hd_selknam_melee_3.group";
	hDImagesResources[7] = "game_selknams_hd/atlas_hd_selknam_mage_1.group";
	hDImagesResources[8] = "game_selknams_hd/atlas_hd_selknam_mage_2.group";
	hDImagesResources[9] = "game_selknams_hd/atlas_hd_selknam_mage_3.group";
	hDImagesResources[10] = "game_selknams_hd/atlas_hd_selknam_bowler_1.group";
	hDImagesResources[11] = "game_selknams_hd/atlas_hd_selknam_bowler_2.group";
	hDImagesResources[12] = "game_selknams_hd/atlas_hd_selknam_bowler_3.group";
	hDImagesResources[13] = "game_selknams_hd/atlas_hd_selknam_archer_1.group";
	hDImagesResources[14] = "game_selknams_hd/atlas_hd_selknam_archer_2.group";
	hDImagesResources[15] = "game_selknams_hd/atlas_hd_selknam_archer_3.group";
	hDImagesResources[16] = "game_enemies_hd/atlas_hd_enemy_1.group";
	hDImagesResources[17] = "game_enemies_hd/atlas_hd_enemy_2.group";
	hDImagesResources[18] = "game_enemies_hd/atlas_hd_enemy_3.group";
	hDImagesResources[19] = "game_enemies_hd/atlas_hd_enemy_4.group";
	hDImagesResources[20] = "game_enemies_hd/atlas_hd_enemy_5.group";
	hDImagesResources[21] = "game_enemies_hd/atlas_hd_enemy_6.group";
	hDImagesResources[22] = "game_enemies_hd/atlas_hd_enemy_7.group";

	bool bHDImagesResourcesLoaded = false;

	int qnt_resources = 0;
	int maxQnt_resources = commonResourcesNum;
	if(!bUseHDImages)
		maxQnt_resources = commonResourcesNum + nonHDImagesResourcesNum;
	else
		maxQnt_resources = commonResourcesNum + hDImagesResourcesNum;

#ifdef IW_BUILD_RESOURCES
	maxQnt_resources = commonResourcesNum + nonHDImagesResourcesNum + hDImagesResourcesNum;
#endif

	int dots = 0;
	string ver = "";
	//if(bIsFullVersion)
	//	ver += loc_splash_fullVersion;
	//else
	//	ver += loc_splash_freeVersion;
	ver += loc_splash_version;
	ver += " ";
	ver += version_number;

	//build data
	//ver = "BUILD 04 2014.07.11";

	CIwSVec2 barSize = CIwSVec2(327, 28);
	//barSize = Utils::GetScaledSize(barSize);
	float barWidth = (float)barSize.x;
	float barHeight = (float)barSize.y;
	float curBarLoadedLength = 0.0f;
	float chunkSizeBar = 0.0f;
	float loadingBarX = (currentGameWidth - barWidth) * 0.5f;
	float loadingBarY = (currentGameHeight - barSize.y) * 0.5f;
	int currentLoadingStringHeight = Utils::GetScaledInt(407);
	int percent = 0;
	char percent_str[5];
	sprintf(percent_str, "%d%%", percent);

	chunkSizeBar = barWidth / maxQnt_resources;

	//clean groups loaded..
	for(int i = 0; i < 60; i++)
		GroupLoaded[i] = -1;

	IwGxSetOrtho(0, currentGameWidth, 0, currentGameHeight, 1.0f, 512.0f);				
	IwGxSetViewMatrix(&view);
	IwGxSetModelMatrix(&view);

	//Special for loading bar
	IwGetResManager()->LoadGroup("bar_loading.group");
	CIwTexture* loadingBarTex = (CIwTexture*)IwGetResManager()->GetGroupNamed("bar_loading")->GetResNamed("bar_loading", IW_GX_RESTYPE_TEXTURE);

	while(qnt_resources <= maxQnt_resources && !s3eDeviceCheckQuitRequest())
	{
		LogsOnScreen();
		curBarLoadedLength = chunkSizeBar * (qnt_resources);
		percent = (int)((qnt_resources * 100) / maxQnt_resources);
		sprintf(percent_str, "%d%%", percent);

		IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
		
		//barra de fondo
		Utils::RenderColor(0xff333333, 0xff191919, CIwSVec2((int)loadingBarX, (int)loadingBarY), CIwSVec2((int)barWidth, (int)barHeight), true);
		IwGxFlush();
		//barra creciente
		Utils::RenderColor(0xff0EA317, 0xff08610D, CIwSVec2((int)loadingBarX, (int)loadingBarY), CIwSVec2((int)curBarLoadedLength, (int)barHeight), true);
		IwGxFlush();
		Utils::RenderSingleTexture(loadingBarTex, CIwSVec2((int)loadingBarX - 7, (int)loadingBarY - 5));
		IwGxFlush();
		IwGxLightingOn();

		int textX = (int)(loadingBarX + barWidth);
		int textY = (int)(loadingBarY - 45);

		Utils::RenderText(percent_str, CIwRect(textX - 30, textY, 200, 50), font_komikadisplay18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT,   IW_GX_FONT_ALIGN_MIDDLE);

		Utils::RenderText(loc_splash_loading, CIwRect(0, currentLoadingStringHeight, currentGameWidth, 500), font_komikadisplaybold25, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE,   IW_GX_FONT_ALIGN_TOP);
		//Utils::RenderText(ver.c_str()   , CIwRect(32, currentGameHeight - 50, 1280, 720), font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		IwGxLightingOff();
		
		
		if(bLoadAllResources)
		{
			if(!bCommonResourcesLoaded)
			{
				int tmpNum = qnt_resources;
				IwGetResManager()->LoadGroup(commonResources[tmpNum]);
				if(tmpNum == commonResourcesNum - 1)
					bCommonResourcesLoaded = true;
				AddLoadedResourcesToArray(Utils::GetUnformattedLoadedGroup(commonResources[tmpNum]));
				IwTrace(NEWLOADING, ("LOADING COMMON: tmpNum: %i, qtn_resources: %i, group to load: %s", tmpNum, qnt_resources, commonResources[tmpNum]));
			}
			else if(bCommonResourcesLoaded && !bHDImagesResourcesLoaded)
			{
				int tmpNum = qnt_resources - commonResourcesNum;
				IwGetResManager()->LoadGroup(hDImagesResources[tmpNum]);
				if(tmpNum == hDImagesResourcesNum - 1)
					bHDImagesResourcesLoaded = true;
				AddLoadedResourcesToArray(Utils::GetUnformattedLoadedGroup(hDImagesResources[tmpNum]));
				IwTrace(NEWLOADING, ("LOADING HD: tmpNum: %i, qtn_resources: %i, group to load: %s", tmpNum, qnt_resources, hDImagesResources[tmpNum]));
			}
			else if(bCommonResourcesLoaded && bHDImagesResourcesLoaded && !bNonHDImagesResourcesLoaded)
			{
				int tmpNum = qnt_resources - commonResourcesNum - hDImagesResourcesNum;
				IwGetResManager()->LoadGroup(nonHDImagesResources[tmpNum]);
				if(tmpNum == nonHDImagesResourcesNum - 1)
					bNonHDImagesResourcesLoaded = true;
				AddLoadedResourcesToArray(Utils::GetUnformattedLoadedGroup(nonHDImagesResources[tmpNum]));
				IwTrace(NEWLOADING, ("LOADING NONHD: tmpNum: %i, qtn_resources: %i, group to load: %s", tmpNum, qnt_resources, nonHDImagesResources[tmpNum]));
			}
		}
		else
		{
			if(!bCommonResourcesLoaded)
			{
				int tmpNum = qnt_resources;
				IwGetResManager()->LoadGroup(commonResources[tmpNum]);
				if(tmpNum == commonResourcesNum - 1)
					bCommonResourcesLoaded = true;
				AddLoadedResourcesToArray(Utils::GetUnformattedLoadedGroup(commonResources[tmpNum]));
				IwTrace(NEWLOADING, ("LOADING COMMON: tmpNum: %i, qtn_resources: %i, group to load: %s", tmpNum, qnt_resources, commonResources[tmpNum]));
			}
			else if(bUseHDImages && bCommonResourcesLoaded && !bHDImagesResourcesLoaded)
			{
				int tmpNum = qnt_resources - commonResourcesNum;
				IwGetResManager()->LoadGroup(hDImagesResources[tmpNum]);
				if(tmpNum == hDImagesResourcesNum - 1)
					bHDImagesResourcesLoaded = true;
				AddLoadedResourcesToArray(Utils::GetUnformattedLoadedGroup(hDImagesResources[tmpNum]));
				IwTrace(NEWLOADING, ("LOADING HD: tmpNum: %i, qtn_resources: %i, group to load: %s", tmpNum, qnt_resources, hDImagesResources[tmpNum]));
			}
			else if(!bUseHDImages && bCommonResourcesLoaded && !bNonHDImagesResourcesLoaded)
			{
				int tmpNum = qnt_resources - commonResourcesNum;
				IwGetResManager()->LoadGroup(nonHDImagesResources[tmpNum]);
				if(tmpNum == nonHDImagesResourcesNum - 1)
					bNonHDImagesResourcesLoaded = true;
				AddLoadedResourcesToArray(Utils::GetUnformattedLoadedGroup(nonHDImagesResources[tmpNum]));
				IwTrace(NEWLOADING, ("LOADING NONHD: tmpNum: %i, qtn_resources: %i, group to load: %s", tmpNum, qnt_resources, nonHDImagesResources[tmpNum]));
			}
		}

		qnt_resources++;

		IwGxFlush();
		IwGxSwapBuffers();

#ifdef STEAMREQUIRED
		if(bSteamAvailable)
			SteamAPI_RunCallbacks();
#endif

		//Yield required.
		s3eDeviceYield(1);
	}
	loadingBarTex = NULL;
	IwGetResManager()->DestroyGroup("bar_loading");
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

	simpleIniApi.Reset();

	IwAnimTerminate();
	IwGraphicsTerminate();

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

void InitializeFontsReferences()
{
	IwGetResManager()->LoadGroup("fonts.group");

	font_komikadisplaybold25     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplaybold25", "CIwGxFont");
	font_komikadisplay11         = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay11"    , "CIwGxFont");
	font_komikadisplay14         = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay14"    , "CIwGxFont");
	font_komikadisplay18         = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay18"    , "CIwGxFont");
	font_komikadisplaybold18     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay18bold", "CIwGxFont");
	font_komikadisplay22		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay22"    , "CIwGxFont");
	font_komikadisplaybold22     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay22bold", "CIwGxFont");
	font_komikadisplaybold48     = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay48bold", "CIwGxFont");
	font_komikadisplay28		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("komikadisplay28"    , "CIwGxFont");
	font_SmartKid12      		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("SmartKid12"         , "CIwGxFont");
	font_SmartKid16      		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("SmartKid16"         , "CIwGxFont");
	font_SmartKid18      		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("SmartKid18"         , "CIwGxFont");
	font_SmartKid20      		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("SmartKid20"         , "CIwGxFont");
	font_SmartKid22      		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("SmartKid22"         , "CIwGxFont");
	font_SmartKid24      		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("SmartKid24"         , "CIwGxFont");
	font_EpsonJap18      		 = (CIwGxFont*)IwGetResManager()->GetGroupNamed("fonts")->GetResNamed("epson_jap18"         , "CIwGxFont");
				
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
	font_SmartKid12          = NULL;
	font_SmartKid16          = NULL;
	font_SmartKid18          = NULL;
	font_SmartKid20          = NULL;
	font_SmartKid22          = NULL;
	font_SmartKid24          = NULL;
	font_EpsonJap18          = NULL;
}

void InitShadersReferences()
{
	if(s3eDeviceCheckQuitRequest())
		return;
	shader_font_a = (CIwGxShaderTechnique*)IwGetResManager()->GetGroupNamed("shaders")->GetResNamed("ShaderFont_A", "CIwGxShaderTechnique");
	// Get the parameter we're interested in
	//CIwGxShaderUniform* param = shader_font_a->GetParam("colour");
	//if( param )
	//{
	//	float val[4] = { 0.0, 1.0, 0.0, 1.0 };
	//	param->Set(CIwGxShaderUniform::VEC4, 0, val);
	//}

	shader_blur = (CIwGxShaderTechnique*)IwGetResManager()->GetGroupNamed("shaders")->GetResNamed("shader_blur", "CIwGxShaderTechnique");
	shader_bicubic = (CIwGxShaderTechnique*)IwGetResManager()->GetGroupNamed("shaders")->GetResNamed("shader_bicubic", "CIwGxShaderTechnique");

	
}

void TerminateShadersReferences()
{
	shader_font_a = NULL;
	shader_blur = NULL;
	shader_bicubic = NULL;
}

#if !defined DESKTOPPLATFORM
void ReadyiOSBillingCallback(void* caller, void* data)
{
	bIwBillingIosAvailable = true;
	//IwBilling::QueryProducts(&removeAds_ProductID, 1);
}

// Create a callback that will be called when product information is available
void ProductInfoAvailableCallback(void* userdata, IwBilling::CIwBillingInfoAvailableData* data)
{
	// Display product information
	removeAds_Data_ProductID = data->ProductID;
	removeAds_Data_Title = data->Title;
	removeAds_Data_Desc = data->Description;
	removeAds_Data_Price = data->Price;
}

void InitializeBillingSystem()
{
	bool tmpBillingValue = false;
	if (IwBilling::isAvailable(IwBilling::BILLING_VENDOR_APPLE))
	{
		tmpBillingValue = true;
	}

	if(!tmpBillingValue)
		return;

	if (IwBilling::Init(ReadyiOSBillingCallback))
	{
		// Set callback
		IwBilling::setInfoAvailableCallback(ProductInfoAvailableCallback);
	} 

}

void TerminateBillingSystem()
{
	IwBilling::Terminate();
}


void InitializeFlurryAnalytics()
{
	if (s3eFlurryAvailable())
	{
		bFlurryOn = true;
		//if(bIsFullVersion)
		//	s3eFlurryStartSession("G5JXHDC3YJSS9V7WDQTJ");
		//else
		//	s3eFlurryStartSession("2MVBP3MC2JD3XKQ72SP9");
		s3eFlurryStartSession("G5JXHDC3YJSS9V7WDQTJ");
		s3eFlurrySetUserID(s3eDeviceGetString(S3E_DEVICE_UNIQUE_ID));
		s3eFlurryLogEvent("event_UserLoggedIn");

		currentDeviceOS = (s3eDeviceOSID)s3eDeviceGetInt(S3E_DEVICE_OS);
		currentDeviceLanguage = (s3eDeviceLanguage)s3eDeviceGetInt(S3E_DEVICE_LANGUAGE);
		currentDeviceID = s3eDeviceGetString(S3E_DEVICE_ID);

		string tmpString = "Full";
		if(!bIsFullVersion)
			tmpString = "Free";
		string tmpValue = "Version|" + tmpString;
		s3eFlurryLogEventParams("event_Version", tmpValue.c_str());

		//register current device.
		tmpString = currentDeviceID;
		tmpValue = "Device|" + tmpString;
		s3eFlurryLogEventParams("event_Device", tmpValue.c_str());
	}
}

void TerminateFlurryAnalytics()
{
	if(bFlurryOn)
	{
		s3eFlurryCountPageView();
		s3eFlurryLogError("error", "OMG!");
		s3eFlurryEndSession();
		bFlurryOn = false;
	}
}


void InitializeChartBoost()
{
	if (s3eChartBoostAvailable() == S3E_TRUE)
	{
		s3eChartBoostSetAppID("5411eda4c26ee4135aadc60a");
		s3eChartBoostSetAppSignature("3f20db5d549af92194ca03bb479023ec87c963cc");
		s3eChartBoostStartSession();
		s3eChartBoostCacheInterstitial("Game Over");
		s3eChartBoostCacheInterstitial("Main Menu");
		s3eChartBoostCacheMoreApps();
		
		bChartBoostAvailable = true;
	}
}

void TerminateChartBoost()
{
	if(bChartBoostAvailable)
	{
		bChartBoostAvailable = false;
	}
}


void InitializeKiip()
{
	if (s3eKiipAvailable())
	{
		s3eResult myResult = s3eKiipInitWithKeys("32c11f97dd85e796582c80ab3ee17eea", "cdc811ac45ee48076dbb2e99a7634a02");
		bKiipAvailable = false;
		if(myResult == S3E_RESULT_SUCCESS)
		{
			bKiipAvailable = true;
			s3eKiipSetAutorotate(true);
		}
	}
}

void TerminateKiip()
{
	if(bKiipAvailable)
	{
		bKiipAvailable = false;
	}
}

void InitializeiAd()
{
	if (s3eIOSIAdAvailable())
	{
		biAdAvailable = true;
	}
}

void TerminateiAd()
{
	if(biAdAvailable)
	{
		biAdAvailable = false;
	}
}


void InitializeAds()
{
	if(bIsFullVersion)
		return;
	//if(LeadboltExtAvailable())
	//{
	//	setLandscapeMode("1");
	//	bLeadbolt = true;
	//}
}

void TerminateAds()
{
	//if(bLeadbolt)
	//{
	//	bLeadbolt = false;
	//}
}
#endif

void InitializeGlobalVariables()
{
	if(s3eDeviceCheckQuitRequest())
		return;

	atlas_bkg_menu_others   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_bkg_menu_others")->GetResNamed("bkg_menu_others", IW_GX_RESTYPE_TEXTURE);
	
	if(bUseHDImages)
	{
		atlas_bkg_gameplay   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_bkg_gameplay")->GetResNamed("bkg_gameplay", IW_GX_RESTYPE_TEXTURE);
		atlas_bkg_menu_sky   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_bkg_menu_sky")->GetResNamed("bkg_menu_sky", IW_GX_RESTYPE_TEXTURE);
		atlas_bkg_worldmap   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_bkg_worldmap")->GetResNamed("bkg_worldmap", IW_GX_RESTYPE_TEXTURE);
		atlas_bkg_menu_foreground   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_bkg_menu_foreground")->GetResNamed("bkg_menu_foreground", IW_GX_RESTYPE_TEXTURE);
	}
	else
	{
		atlas_bkg_small_0   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_bkg_small_0")->GetResNamed("bkg_small_0", IW_GX_RESTYPE_TEXTURE);
		atlas_bkg_small_1   = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_bkg_small_1")->GetResNamed("bkg_small_1", IW_GX_RESTYPE_TEXTURE);
	}

	atlas_boss     = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_boss")->GetResNamed("atlas_enemy_boss_1", IW_GX_RESTYPE_TEXTURE);
	atlas_common_textures_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_textures_0")->GetResNamed("atlas_common_textures_0", IW_GX_RESTYPE_TEXTURE);
	atlas_common_textures_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_textures_1")->GetResNamed("atlas_common_textures_1", IW_GX_RESTYPE_TEXTURE);
	atlas_common_textures_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_textures_2")->GetResNamed("atlas_common_textures_2", IW_GX_RESTYPE_TEXTURE);
	atlas_common_textures_3 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_textures_3")->GetResNamed("atlas_common_textures_3", IW_GX_RESTYPE_TEXTURE);
	atlas_common_textures_4 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_textures_4")->GetResNamed("atlas_common_textures_4", IW_GX_RESTYPE_TEXTURE);
	atlas_small_textures = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_small_textures")->GetResNamed("atlas_small_textures", IW_GX_RESTYPE_TEXTURE);
	atlas_common_trial_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_common_trial_0")->GetResNamed("atlas_common_trial_0", IW_GX_RESTYPE_TEXTURE);

	if(bUseHDImages)
	{
		atlas_hd_selknam_archer_1			= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_archer_1")->GetResNamed("atlas_selknam_archer_1", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_archer_2			= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_archer_2")->GetResNamed("atlas_selknam_archer_2", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_archer_3			= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_archer_3")->GetResNamed("atlas_selknam_archer_3", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_bowler_1_01		= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_bowler_1")->GetResNamed("atlas_selknam_bowler_1_01", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_bowler_1_02		= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_bowler_1")->GetResNamed("atlas_selknam_bowler_1_02", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_bowler_2_01		= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_bowler_3")->GetResNamed("atlas_selknam_bowler_3_01", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_bowler_2_02		= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_bowler_3")->GetResNamed("atlas_selknam_bowler_3_02", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_bowler_3_01		= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_bowler_2")->GetResNamed("atlas_selknam_bowler_2_01", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_bowler_3_02		= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_bowler_2")->GetResNamed("atlas_selknam_bowler_2_02", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_mage_1				= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_mage_1")->GetResNamed("atlas_selknam_mage_1", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_mage_2				= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_mage_2")->GetResNamed("atlas_selknam_mage_2", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_mage_3				= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_mage_3")->GetResNamed("atlas_selknam_mage_3", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_melee_1			= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_melee_1")->GetResNamed("atlas_selknam_melee_1", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_melee_2			= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_melee_2")->GetResNamed("atlas_selknam_melee_2", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_selknam_melee_3			= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_selknam_melee_3")->GetResNamed("atlas_selknam_melee_3", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_1					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_1")->GetResNamed("atlas_enemy_1", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_2					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_2")->GetResNamed("atlas_enemy_2", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_3					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_3")->GetResNamed("atlas_enemy_3", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_4_01					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_4")->GetResNamed("atlas_enemy_4_01", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_4_02					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_4")->GetResNamed("atlas_enemy_4_02", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_5					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_5")->GetResNamed("atlas_enemy_5", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_6					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_6")->GetResNamed("atlas_enemy_6", IW_GX_RESTYPE_TEXTURE);
		atlas_hd_enemy_7					= (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_hd_enemy_7")->GetResNamed("atlas_enemy_7", IW_GX_RESTYPE_TEXTURE);
	}
	else
	{
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
	}

	atlas_terrain_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_terrain")->GetResNamed("terrain_combined_01_02", IW_GX_RESTYPE_TEXTURE); 
	atlas_terrain_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_terrain")->GetResNamed("terrain_combined_03_04", IW_GX_RESTYPE_TEXTURE); 
	atlas_props_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_props")->GetResNamed("atlas_props_0", IW_GX_RESTYPE_TEXTURE); 
	atlas_props_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_props")->GetResNamed("atlas_props_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_props_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_props")->GetResNamed("atlas_props_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_fx_0 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_fx")->GetResNamed("atlas_fx_0", IW_GX_RESTYPE_TEXTURE); 
	atlas_fx_1 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_fx")->GetResNamed("atlas_fx_1", IW_GX_RESTYPE_TEXTURE); 
	atlas_fx_2 = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_fx")->GetResNamed("atlas_fx_2", IW_GX_RESTYPE_TEXTURE); 
	atlas_others = (CIwTexture*)IwGetResManager()->GetGroupNamed("atlas_others")->GetResNamed("atlas_others", IW_GX_RESTYPE_TEXTURE); 
}

static CIwFMat view;

RenderData waterMarkRenderData;
CIwSVec2 waterMarkPosition;

int currentUpdate;
int nextUpdate;

void CreateObjects()
{
	LoadFXSounds();
	survivalModeConfig = new SurvivalModeConfig();
	fade               = new FadeScreen(0,0,0);
	popup              = new PopUp();
	sScreen            = new SplashScreen(5, 2, 15);
	pGame              = new CGame;
	mainmenu               = new MainMenu;
	mapselection       = new MapSelectionBase();
	extramaps          = new ExtraMaps();
	cursor             = new Cursor();
	tutorial           = new Tutorial(); 
	dialogController   = new DialogController();
	almanacBase        = new AlmanacBase();
	settingsMenu       = new SettingsMenu();
	achievementsHelper = new AchievementsHelper();
	leaderboardsHelper = new LeaderboardsHelper();
	
#ifdef STEAMREQUIRED
	achievementsHelper->RetrieveSteamAchievementInterfaceRef(g_SteamAchievements);
	leaderboardsHelper->RetrieveSteamLeaderboardsInterfaceRef(g_SteamLeaderboards);
#endif

	cursor->InitCursor();
	Utils::UtilsInit();
	bFxSoundMuted = false;

	if(saveDataHelper)
	{
#ifdef STEAMREQUIRED
		saveDataHelper->RetrieveSteamRemoteStorageRef();
#endif
		saveDataHelper->CheckGameDataFlag();
		saveDataHelper->LoadGameDataCampaign();
		saveDataHelper->LoadGameDataSurvival();
	}
}

void DestroyObjects()
{
#if !defined DESKTOPPLATFORM
	TerminateAds();
	TerminateFlurryAnalytics();
	TerminateBillingSystem();
	TerminateKiip();
	InitializeiAd();
	TerminateChartBoost();
#endif

	TerminateFontsReferences();

	cursor->EndCursor();
	sScreen->EndObject();
	pGame->EndObject();
	mainmenu->EndObject();
	mapselection->EndObject();
	tutorial->EndTutorial();

	//TerminateShadersReferences();
	delete saveDataHelper;
	delete survivalModeConfig;
	delete fade;
	delete popup;
	delete sScreen;
	delete pGame;
	delete mainmenu;
	delete mapselection;
	delete extramaps;
	delete cursor;
	delete tutorial;
	delete dialogController;
	delete almanacBase;
	delete settingsMenu;
	delete achievementsHelper;
	delete leaderboardsHelper;
#if defined DESKTOPPLATFORM
	if(displayModes)
		delete displayModes;
#endif
	UnLoadTextures();
	s3eDeviceYield(1);
}

void GameInit()
{
	perMinuteTimer.SetTimeLimit(60000);

	bFxSoundMuted = true;

#ifdef STEAMREQUIRED
	InitializeSteam_Step0();
#endif

#ifdef GAMECENTER
	InitializeGameCenter();
#endif
	
	//Init
	InitApplication();

	//Yield required.
	s3eDeviceYield(1);

#ifdef STEAMREQUIRED
	InitializeSteam_Step1();

	//Check Steam Errors and close the game.
	if(!bSteamAvailable)
	{
		s3eWindowSetFullscreen(NULL);
		s3ePointerSetInt(S3E_POINTER_HIDE_CURSOR, 0);
		s3eDialogAlertInfo info;
		info.m_Title = "Steam Error";
		info.m_Message = "Unable to load Steam";
		info.m_Button[0] = "Accept";
		s3eDialogAlertBox(&info);
		s3eDeviceRequestQuit();
	}
	//Check correct steam app id steam
	else if(bSteamAvailable && steamAppId != 306260)
	{
		s3eWindowSetFullscreen(NULL);
		s3ePointerSetInt(S3E_POINTER_HIDE_CURSOR, 0);
		s3eDialogAlertInfo info;
		info.m_Title = "Steam Error";
		info.m_Message = "Unable to load game, Id Error";
		info.m_Button[0] = "Accept";
		s3eDialogAlertBox(&info);
		s3eDeviceRequestQuit();
	}

	//GAME STATS
	if(g_SteamAchievements)
		g_SteamAchievements->StoreGlobalStats_SteamID("event_UserLoggedIn");
#endif

	LocalizeGame();

	InitializeFontsReferences();


	//Yield required.
	for (int i = 0; i < 10; i++)
	{
		IwGxTickUpdate();
		IwGxFlush();
		IwGxSwapBuffers();
		s3eDeviceYield(1);
		if(bLandscapeHack)
			s3eSurfaceSetInt( S3E_SURFACE_DEVICE_ORIENTATION_LOCK, S3E_SURFACE_LANDSCAPE );
	}

#if !defined DESKTOPPLATFORM
	if(s3eIOSBackgroundAudioAvailable() == S3E_TRUE)
		biOSBackgroundAudioAvailable = true;

	InitializeBillingSystem();
	InitializeFlurryAnalytics();
	if(!bIsFullVersion)
	{
		InitializeAds();
		InitializeKiip();
		InitializeiAd();
		InitializeChartBoost();
	}
#endif
	LoadTextures();

#if !defined DESKTOPPLATFORM
#ifdef GAMECENTER
	if(bGameCenterAvailable)
		AuthenticateLocalHandlerGameCenter();
#endif
#endif

	InitializeGlobalVariables();

	TextureHelper textureHelper;
	textureHelper.RetrieveAllTextures();

	//InitShadersReferences();
}

void RegisterDevices()
{
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_main, NULL);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Main, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_Main, NULL);
	s3eDeviceRegister(S3E_DEVICE_PAUSE, DeviceSuspendCallback_Main, 0);
}

void InitAndEndObjects()
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
			mainmenu->InitObject();
		else if(gameState==GS_MapSelection)
			mapselection->InitObject();
		else if(gameState==GS_ExtraMapSelection)
			extramaps->InitObject();
		else if(gameState==GS_Retry)
		{
			fade->FadeIn(255);
			pGame->EndObject();
		}

		//End Objects
		if(lastGameState==GS_SplashScreen)
			sScreen->EndObject();
		else if(lastGameState==GS_Game)
			pGame->EndObject();
		else if(lastGameState==GS_MainMenu)
			mainmenu->EndObject();
		else if(lastGameState==GS_MapSelection)
			mapselection->EndObject();
		else if(lastGameState==GS_ExtraMapSelection)
			extramaps->EndObject();
		else if(lastGameState==GS_Retry)
		{
			pGame->InitObject();
			fade->FadeOut(255);
		}

		lastGameState = gameState;
	}
}

void HandlePopupSystem()
{
	//PopUp handler
	if(lastPopUpState != popUpState)
	{
		if(popUpState == GSP_None)
			popup->Off();
		else
			popup->On();
		lastPopUpState = popUpState;
	}

	popup->Update();
}

void UpdateGameStats()
{
	//Update Statistics.
	perMinuteTimer.Update();
	if(perMinuteTimer.GetIsEnd() || perMinuteTimer.GetIsStop())
	{
		//IwTrace(STATS, ("Num Selknam Killed By Minute: %d", tempNumSelknamKilledByMinute));
		numSelknamKilledByMinute = tempNumSelknamKilledByMinute;
		tempNumSelknamKilledByMinute = 0;
		perMinuteTimer.Reset();
		perMinuteTimer.Start();
	}
}

void UpdateObjects()
{
#ifdef STEAMREQUIRED
	if(bSteamAvailable)
		SteamAPI_RunCallbacks();
#endif

#if defined DESKTOPPLATFORM
	if(bIsXbox360ControllerEnabled)
	{
		GamePad_Update(0.0f);
	}
#endif
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
		mainmenu->Update();
	else if(gameState==GS_MapSelection)
		mapselection->Update();
	else if(gameState == GS_Retry)
	{
		if(fade->GetAlpha() >= 255)
			gameState = GS_Game;
	}
	else if(gameState==GS_ExtraMapSelection)
		extramaps->Update();
	tutorial->Update();
	dialogController->Update();
}

void SetScreenSettings()
{
	//render
	// Clear the screen
	IwGxClear(IW_GX_COLOUR_BUFFER_F | IW_GX_DEPTH_BUFFER_F);
	//common render.
	float widthFloat = (float)currentGameWidth;
	float heightFloat = (float)currentGameHeight;
	iwfixed width = (iwfixed)widthFloat;
	iwfixed height = (iwfixed)heightFloat;

	IwGxSetOrtho(xWindowOffset, width + xWindowOffset, yWindowOffset, height + yWindowOffset, 1.0f, 512.0f);				
	//IwGxSetOrtho(0, width + 0, 0, height + 0, 1.0f, 512.0f);				
	IwGxSetViewMatrix(&view);
	IwGxSetModelMatrix(&view);
}

void RenderObjects()
{
	//render per active object
	if(gameState==GS_SplashScreen)
		sScreen->Render();
	else if(gameState==GS_Game)
		pGame->Render();
	else if(gameState==GS_MainMenu)
		mainmenu->Render();
	else if(gameState==GS_MapSelection)
		mapselection->Render();
	else if(gameState==GS_ExtraMapSelection)
		extramaps->Render();

	IwGxFlush();

	tutorial->Render();

	if(popUpState != GSP_None) 
		popup->Render();

	dialogController->Render();

	if (bWaterMark)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(waterMarkRenderData.atlasName), waterMarkRenderData, waterMarkPosition, 255, 255, 255, 20);
	}
#if defined DESKTOPPLATFORM
	if (activeControllerType == Controller_MOUSE)
		cursor->Render();
#endif

	fade->Render();
}

void SettingEndRendering()
{
	IwGxFlush();
	// Swap buffers
	IwGxSwapBuffers();
	// S3E applications should yield frequently
	s3eDeviceYield();

	//accumulated_frames++;
	//if(accumulated_frames >= 15)
	//{
	//	accumulated_frames = 0;
	//	for(int i = 0; i < MAX_FX_SOUNDS; i++)
	//		fx_sound_limit[i][1] = 0;
	//}
}

void UnregisterDevices()
{
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_main);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Main);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_Main);
	s3eDeviceUnRegister(S3E_DEVICE_PAUSE, DeviceSuspendCallback_Main);
}

bool CheckUnexpectedAppQuit()
{
	if(s3eDeviceCheckQuitRequest())
	{
		TerminateFontsReferences();
		//Yield required.
		s3eDeviceYield(1);
		UnLoadTextures();
		//Yield required.
		s3eDeviceYield(1);
#ifdef STEAMREQUIRED
		TerminateSteam();
#endif
		TerminateApplication();

		return true;
	}

	return false;
}

void SetWaterMark()
{
	waterMarkRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_logo_badgames"));
	waterMarkRenderData.width = waterMarkRenderData.height = 128;
	waterMarkPosition = CIwSVec2(0, currentGameHeight - waterMarkRenderData.height);
}

void SetInitialVariables()
{
	view = CIwFMat::g_Identity;
	int currentUpdate = GetUpdateFrame();
	int nextUpdate = currentUpdate;
	gameState = GS_SplashScreen;
}
#pragma endregion

// "main" is the S3E entry point
int main()
{
	GameInit();
	if(CheckUnexpectedAppQuit())
	{
		return 0;
	}
	
	CreateObjects();
	RegisterDevices();
	SetInitialVariables();
	SetWaterMark();
	closeGameTimer.SetTimeLimit(20);
	bool bCloseGame = false;

    while(!s3eDeviceCheckQuitRequest())
    {
        while(!s3eDeviceCheckQuitRequest())
        {
            nextUpdate = GetUpdateFrame();
            if( nextUpdate != currentUpdate )
                break;
            s3eDeviceYield(1);
        }

		if(!bIAPRegistered && mainmenu != NULL && bIwBillingIosAvailable)
		{
			if(mainmenu->menu!=NULL)
			{
				mainmenu->menu->RegisterIAPCallbacks();
				bIAPRegistered = true;
			}
		}

		if(bQuittingGame && popUpState == GSP_None && !bCloseGame)
		{
			closeGameTimer.Reset();
			closeGameTimer.Start();
			bCloseGame = true;
		}

		if(bCloseGame && closeGameTimer.GetIsEnd())
		{
			bCloseGame = false;
			if(saveDataHelper)
				saveDataHelper->SaveGameDataCampaign();
			s3eDeviceRequestQuit();
		}

        int frames = nextUpdate - currentUpdate;
        frames = MIN(MAX_UPDATES, frames);
        while(frames--)
        {
			//global system timer delta detection.
			deltaSystemTime = s3eTimerGetMs() - lastSystemTime;
			lastSystemTime = s3eTimerGetMs();

			InitAndEndObjects();
			HandlePopupSystem();
			UpdateGameStats();
			UpdateObjects();

			closeGameTimer.Update();
        }
        currentUpdate = nextUpdate;

		SetScreenSettings();
		RenderObjects();
		LogsOnScreen();
		SettingEndRendering();
    }

#if defined DESKTOPPLATFORM
	if(s3eWindowIsFullscreen() == S3E_TRUE)
	{
		s3eWindowSetFullscreen(NULL);
	}
#endif

	bBlockOnQuit = true;
#ifdef STEAMREQUIRED
	TerminateSteam();
#endif
	DestroyObjects();
	TerminateApplication();

    return 0;
}