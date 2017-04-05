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

#ifndef _MAINMENUBASE_H
#define _MAINMENUBASE_H

#include "s3eAudio.h"

#include "background.h"
#include "mm_credits.h"
#include "sprite2d.h"
#include "cloud.h"
#include "mm_trialversion.h"
#include "mm_final.h"
#include "texturehelper.h"

class MainMenu {
public:
	MainMenu();
	~MainMenu();
	bool bIsActiveObj;
	void Update();
	void Render();
	void InitObject();
	void EndObject();

private:
	//Main Screen
	Background* background;
	Background* foreground;
	Menu* menu;
	//Subscreens
	Credits* credits;
	//Popups
	PopUp* newGamePopUp;
	TrialVersion* TrialVersionMenu;
	FinalScreen* FinalScreenMenu;

	Cloud* clouds[2];
	RenderData cloud1RenderData;
	RenderData cloud2RenderData;
};

#endif