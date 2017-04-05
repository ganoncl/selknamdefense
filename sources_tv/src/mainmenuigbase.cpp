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
}

void MainMenuIG::Update()
{
	if(lastMainMenuIGState != mainMenuIGState)
	{
		if(lastMainMenuIGState == GSMIG_MainMenuIG) menuig->EndObject();

		if(mainMenuIGState == GSMIG_MainMenuIG) menuig->InitObject();

		lastMainMenuIGState = mainMenuIGState;
	}
		
	if(mainMenuIGState == GSMIG_MainMenuIG) menuig->Update();
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
}
	

