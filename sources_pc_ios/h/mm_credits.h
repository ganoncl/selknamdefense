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

#ifndef _CREDITS_H
#define _CREDITS_H

#include "IwGx.h"
#include "IwGxFont.h"
#include "s3e.h"
#include "button.h"
#include "background.h"
#include <string>
#include "game_window.h"
using namespace std;

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

class Credits : public GameWindow
{
public:
	Credits();
	~Credits();
	void Render();
	void On();
	void Off();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void Update();

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

private:
	Background* background;

	bool bIsActive;

	int numMembers; //Nº names that appears on credits window.
	const char* rol[5];
	const char* name[5];

	int PannelPosX;
	int PannelPosY;

	//Buttons
	Button* btnBack;
	int btnBackPosX;
	int btnBackPosY;

	//Mail text and rect
	string  strMail;
	CIwRect rectMail;
	uint32  colorMail;

	int widthCreditsPannel;
	int heightCreditsPannel;

	ButtonRenderData buttonRenderData;
	RenderData graphic_logo_marmalade;

	int RenderPanelNumWidth;
	int RenderPanelNumHeight;

	int creditsTitleYPos;
	int badGamePeopleInitYPos;
	int contactMailInitYPos;
	int specialThanksTitleYPos;
	int specialThanksPeople1YPos;
	int specialThanksPeople2YPos;
	int specialThanksPeople3YPos;
	int madeWithMarmaladeYPos;
	int textureMarmaladeYPos;
};

#endif