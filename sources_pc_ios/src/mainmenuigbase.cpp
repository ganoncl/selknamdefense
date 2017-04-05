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

#include "common.h"
#include "mainmenuigbase.h"
#include "mm_settings.h"

MainMenuIG::MainMenuIG()
{
	menuig = new MenuIG();
}

MainMenuIG::~MainMenuIG()
{
	delete menuig;
}

void MainMenuIG::Render()
{
	if(mainMenuIGState == GSMIG_MainMenuIG) menuig->Render();
	else if(mainMenuIGState == GSMIG_AlmanacIG) almanacBase->Render();
	else if(mainMenuIGState == GSMIG_Options) settingsMenu->Render();
}

void MainMenuIG::Update()
{
	if(lastMainMenuIGState != mainMenuIGState)
	{
		if(lastMainMenuIGState == GSMIG_MainMenuIG) menuig->EndObject();
		else if(lastMainMenuIGState == GSMIG_AlmanacIG) almanacBase->EndObject();
		else if(lastMainMenuIGState == GSMIG_Options) settingsMenu->EndObject();

		if(mainMenuIGState == GSMIG_MainMenuIG) menuig->InitObject();
		else if(mainMenuIGState == GSMIG_AlmanacIG) almanacBase->InitObject();
		else if(mainMenuIGState == GSMIG_Options) settingsMenu->InitObject();

		lastMainMenuIGState = mainMenuIGState;
	}
		
	if(mainMenuIGState == GSMIG_MainMenuIG) menuig->Update();
	else if(mainMenuIGState == GSMIG_AlmanacIG) almanacBase->Update();
	else if(mainMenuIGState == GSMIG_Options) settingsMenu->Update();
}

void MainMenuIG::InitObject()
{
	bIsActiveObj = true;
	mainMenuIGState = GSMIG_MainMenuIG;
}

void MainMenuIG::EndObject()
{
	bIsActiveObj = false;

	mainMenuIGState = GSMIG_None;
	Update();

	if(mainMenuIGState == GSMIG_MainMenuIG) menuig->EndObject();
	else if(mainMenuIGState == GSMIG_AlmanacIG) almanacBase->EndObject();
	else if(mainMenuIGState == GSMIG_Options) settingsMenu->EndObject();
}
	

