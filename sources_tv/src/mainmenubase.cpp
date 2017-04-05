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

MainMenu::MainMenu() 
{
	//Main menu
	background = new Background(1);
	foreground = new Background(0);
	menu = new Menu();

	//Subscreens
	credits = new Credits();
	TrialVersionMenu = new TrialVersion();
	FinalScreenMenu = new FinalScreen();
	//options = new Options();

	mainMenuState = GSM_MainMenu;
	lastMainMenuState = mainMenuState;

	cloud1RenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_cloud1"));
	cloud2RenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_cloud2"));

	//There is a black vertical line if original UV is used.
	cloud1RenderData.uvInfo.minX *= 1.001f;

	clouds[0] = new Cloud(cloud1RenderData, TextureHelper::GetCIwTexture(cloud1RenderData.atlasName));
	clouds[0]->SetVelocity(-0.1f);
	clouds[1] = new Cloud(cloud2RenderData, TextureHelper::GetCIwTexture(cloud2RenderData.atlasName));
	clouds[1]->SetVelocity(-0.2f);
}

MainMenu::~MainMenu()
{
	delete menu;
	delete credits;
	delete background;
	delete foreground;
	delete clouds[0];
	delete clouds[1];
	delete TrialVersionMenu;
	delete FinalScreenMenu;
}

void MainMenu::Render()
{
	
	foreground->Render();
	IwGxFlush();

	clouds[0]->Render();
	clouds[1]->Render();
	background->Render();

	if(mainMenuState == GSM_MainMenu) menu->Render();
	else if(mainMenuState == GSM_Credits) credits->Render();
	else if(mainMenuState == GSM_TrialVersion) TrialVersionMenu->Render();
	else if(mainMenuState == GSM_FinalScreen) FinalScreenMenu->Render();
}

void MainMenu::Update()
{
	if(lastMainMenuState != mainMenuState)
	{
		if(lastMainMenuState == GSM_MainMenu) menu->Off();
		else if(lastMainMenuState == GSM_Credits) credits->Off();
		else if(lastMainMenuState == GSM_TrialVersion) TrialVersionMenu->EndObject();
		else if(lastMainMenuState == GSM_FinalScreen) FinalScreenMenu->EndObject();

		if(mainMenuState == GSM_MainMenu) menu->On();
		else if(mainMenuState == GSM_Credits) credits->On();
		else if(mainMenuState == GSM_TrialVersion) TrialVersionMenu->InitObject();
		else if(mainMenuState == GSM_FinalScreen) FinalScreenMenu->InitObject();

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
	else if(mainMenuState == GSM_FinalScreen) FinalScreenMenu->Update();
	else if(mainMenuState == GSM_MainMenu) menu->Update();

	//Sprites
	clouds[0]->Update();
	clouds[1]->Update();
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

	clouds[0]->SetPosition(300, 250, 1.0f);
	clouds[1]->SetPosition(100, 100, 1.0f);

	if(!bUseOldAudioSystem)
	{
		Utils::PlayAudio(Music_MainMenu);
	}
	else
	{
		if(bIsAudioPaused)
		{
			LastMusicStacked = Music_MainMenu;
		}
		else
		{
			s3eAudioPlay("bgm/sd_mainmenu_theme_loop.mp3", 0);
			LastMusicStacked = Music_None;
		}
	}
}

void MainMenu::EndObject()
{
	mainMenuState = GSM_None;
	
	Update();

	bIsActiveObj = false;
}

