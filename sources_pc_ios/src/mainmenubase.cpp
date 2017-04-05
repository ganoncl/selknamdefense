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

#include "IwGx.h"
#include "IwGxFont.h"
#include "s3e.h"

#include "common.h"
#include "button.h"
#include "mm_menu.h"
#include "mm_popup.h"
#include "mm_final.h"
#include "mainmenubase.h"
#include "mm_almanac.h"
#include "mm_settings.h"
#if !defined DESKTOPPLATFORM
#include "s3eChartBoost.h"
#include "s3eIOSIAd.h"
#endif

MainMenu::MainMenu() 
{
	//Main menu
	background = new Background(1);
	foreground = new Background(0);
	menu = new Menu();

	//Subscreens
	credits = new Credits();
	options = new OptionsMenu();
	TrialVersionMenu = new TrialVersion();
	FinalScreenMenu = new FinalScreen();
	playMenu = new PlayMenu();
	menuLeaderboards = new MenuLeaderboards();

	mainMenuState = GSM_MainMenu;
	lastMainMenuState = mainMenuState;

	CIwSVec2 myNewSize;

	cloud1RenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_cloud1"));
	myNewSize = CIwSVec2((int16)cloud1RenderData.width, (int16)cloud1RenderData.height);
	myNewSize = Utils::GetScaledSize(myNewSize);
	cloud1RenderData.width = myNewSize.x;
	cloud1RenderData.height = myNewSize.y;

	cloud2RenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_cloud2"));
	myNewSize = CIwSVec2((int16)cloud2RenderData.width, (int16)cloud2RenderData.height);
	myNewSize = Utils::GetScaledSize(myNewSize);
	cloud2RenderData.width = myNewSize.x;
	cloud2RenderData.height = myNewSize.y;

	boatRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_boat"));
	myNewSize = CIwSVec2((int16)boatRenderData.width, (int16)boatRenderData.height);
	myNewSize = Utils::GetScaledSize(myNewSize);
	boatRenderData.width = myNewSize.x;
	boatRenderData.height = myNewSize.y;

	clouds[0] = new Cloud(cloud1RenderData, TextureHelper::GetCIwTexture(cloud1RenderData.atlasName));
	clouds[0]->SetVelocity(-0.10f);
	clouds[1] = new Cloud(cloud2RenderData, TextureHelper::GetCIwTexture(cloud2RenderData.atlasName));
	clouds[1]->SetVelocity(-0.20f);
	clouds[2] = new Cloud(boatRenderData, TextureHelper::GetCIwTexture(boatRenderData.atlasName));
	clouds[2]->SetVelocity(0.05f);
	
	bShowingiAd = false;
}

MainMenu::~MainMenu()
{
	delete menu;
	delete credits;
	delete background;
	delete foreground;
	delete clouds[0];
	delete clouds[1];
	delete clouds[2];
	delete TrialVersionMenu;
	delete FinalScreenMenu;
	delete playMenu;
	delete menuLeaderboards;
	delete options;
}

void MainMenu::Render()
{
	
	foreground->Render();
	IwGxFlush();

	clouds[0]->Render();
	clouds[1]->Render();
	clouds[2]->Render();
	background->Render();

	if(mainMenuState == GSM_MainMenu) menu->Render();
	else if(mainMenuState == GSM_Credits) credits->Render();
	else if(mainMenuState == GSM_TrialVersion) TrialVersionMenu->Render();
	else if(mainMenuState == GSM_FinalScreen) FinalScreenMenu->Render();
	else if(mainMenuState == GSM_Almanac) almanacBase->Render();
	else if(mainMenuState == GSM_Play) playMenu->Render();
	else if(mainMenuState == GSM_Leaderboards) menuLeaderboards->Render();
	else if(mainMenuState == GSM_Options) options->Render();
	else if(mainMenuState == GSM_Settings) settingsMenu->Render();
}

void MainMenu::Update()
{
	if(lastMainMenuState != mainMenuState)
	{
		if(lastMainMenuState == GSM_MainMenu) menu->Off();
		else if(lastMainMenuState == GSM_Credits) credits->Off();
		else if(lastMainMenuState == GSM_TrialVersion) TrialVersionMenu->EndObject();
		else if(lastMainMenuState == GSM_FinalScreen) FinalScreenMenu->EndObject();
		else if(lastMainMenuState == GSM_Almanac) almanacBase->EndObject();
		else if(lastMainMenuState == GSM_Play) playMenu->EndObject();
		else if(lastMainMenuState == GSM_Leaderboards) menuLeaderboards->EndObject();
		else if(lastMainMenuState == GSM_Options) options->EndObject();
		else if(lastMainMenuState == GSM_Settings) settingsMenu->EndObject();

		if(mainMenuState == GSM_MainMenu) menu->On();
		else if(mainMenuState == GSM_Credits) credits->On();
		else if(mainMenuState == GSM_TrialVersion) TrialVersionMenu->InitObject();
		else if(mainMenuState == GSM_FinalScreen) FinalScreenMenu->InitObject();
		else if(mainMenuState == GSM_Almanac) almanacBase->InitObject();
		else if(mainMenuState == GSM_Play) playMenu->InitObject();
		else if(mainMenuState == GSM_Leaderboards) menuLeaderboards->InitObject();
		else if(mainMenuState == GSM_Options) options->InitObject();
		else if(mainMenuState == GSM_Settings) settingsMenu->InitObject();
 
		lastMainMenuState = mainMenuState;
	}

	if(popUpState == GSP_None && mainMenuState == GSM_MainMenu)
	{
		if(!menu->bIsOn) menu->On();
	}
	else if(menu->bIsOn) 
	{
		menu->Off();
	}
	
	if(mainMenuState == GSM_TrialVersion) TrialVersionMenu->Update();
	else if(mainMenuState == GSM_FinalScreen)  FinalScreenMenu->Update();
	else if(mainMenuState == GSM_MainMenu)     menu->Update();
	else if(mainMenuState == GSM_Almanac)      almanacBase->Update();
	else if(mainMenuState == GSM_Play) playMenu->Update();
	else if(mainMenuState == GSM_Leaderboards) menuLeaderboards->Update();
	else if(mainMenuState == GSM_Credits) credits->Update();
	else if(mainMenuState == GSM_Options) options->Update();
	else if(mainMenuState == GSM_Settings) settingsMenu->Update();
		 
	//Sprites
	clouds[0]->Update();
	clouds[1]->Update();
	clouds[2]->Update();
}

void MainMenu::InitObject()
{
	bIsActiveObj = true;
	if(bShowTrialVersionFinal)
	{
		mainMenuState = GSM_TrialVersion;
		bShowTrialVersionFinal = false;
	}
	else
	{
		if(mainMenuState == GSM_None)
			mainMenuState = GSM_MainMenu;
	}

	CIwSVec2 myScaledPos;

	myScaledPos = CIwSVec2(Utils::GetFixedScaledInt(300), Utils::GetScaledInt(250));
	clouds[0]->SetPosition(myScaledPos.x, myScaledPos.y, 1.0f);

	myScaledPos = CIwSVec2(Utils::GetFixedScaledInt(100), Utils::GetScaledInt(100));
	clouds[1]->SetPosition(myScaledPos.x, myScaledPos.y, 1.0f);

	myScaledPos = CIwSVec2(Utils::GetFixedScaledInt(320), Utils::GetScaledInt(595));
	if(NewGame_State == NOT_EMPTY_GAME)
		myScaledPos = CIwSVec2(Utils::GetFixedScaledInt(320), Utils::GetScaledInt(547));
	clouds[2]->SetPosition(myScaledPos.x, myScaledPos.y, 1.0f);

	s3eAudioStop();
	if(bIsAudioPaused)
	{
		LastMusicStacked = Music_MainMenu;
	}
	else
	{
		Utils::PlayBGM("bgm/sd_mainmenu_theme_loop.mp3", 0);
		LastMusicStacked = Music_None;
	}

	//set current map on default 1
	currentMap = 1;

#if !defined DESKTOPPLATFORM

	if(!bIsFullVersion && bChartBoostAvailable)
	{
		showCharBoostOnMainMenuCount++;
		if(showCharBoostOnMainMenuCount == 4)
		{
			s3eChartBoostShowInterstitial("Main Menu");
			showCharBoostOnMainMenuCount = 0;
		}
	}
	else if(!bIsFullVersion && biAdAvailable)
	{
		//Show iAd
		s3eIOSIAdStart();
		s3eIOSIAdSetInt(S3E_IOSIAD_BANNER_SHOW, 1);
		bShowingiAd = true;
	}

	if(!bIsFullVersion && s3eSocketGetInt(S3E_SOCKET_NETWORK_AVAILABLE) == 0)
	{
		popUpState = GSP_NOINTERNET;
	}

#endif
}

void MainMenu::EndObject()
{
	mainMenuState = GSM_None;
	
	Update();

	bIsActiveObj = false;
#if !defined DESKTOPPLATFORM
	if(bShowingiAd)
	{
		//Show iAd
		s3eIOSIAdSetInt(S3E_IOSIAD_BANNER_SHOW, 0);
		s3eIOSIAdStop();
		bShowingiAd = false;
	}
#endif
}

