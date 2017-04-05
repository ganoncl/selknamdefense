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

#ifndef _MENU_H
#define _MENU_H

#include "xbox360controller.h"
#include "game_window.h"
#include "focusmanager.h"

#define MENU_NUMBER_BUTTONS 5

class Menu : public GameWindow {
public:
	Menu();
	~Menu();
	void InitObject();
	void EndObject();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void On();
	void Off();

	void RegisterIAPCallbacks();

	bool bIsActiveObj;
	bool bIsOn;
	bool bBlockDeviceHandler;
	bool bStartTimer;
	bool bDoJustOnceInWhileGame;
	Timer Timer_BlockAfterFadeOut;
	
	FocusManager focusManager;
	ButtonWrapper buttonList[MENU_NUMBER_BUTTONS];

private:
#if defined DESKTOPPLATFORM	
	Xbox360Controller xbox360Controller;
#endif

	CIwResGroup* pGroup;

	bool bDoEnterAnimation;
	
	CIwSVec2 buttonInitPos[MENU_NUMBER_BUTTONS];
	CIwSVec2 buttonFinalPos[MENU_NUMBER_BUTTONS];
	int buttonAccel[MENU_NUMBER_BUTTONS];
	uint8 buttonCurrentAlpha[MENU_NUMBER_BUTTONS];
	Timer buttonAnimTimer[MENU_NUMBER_BUTTONS];
	bool bMoveButton[MENU_NUMBER_BUTTONS];
	bool bApplyAlphaAnimButton[MENU_NUMBER_BUTTONS];

	s3eKey lastKeyPressed;
	
	int btnXPos;
	int btnYPos;
	int btnDistance;
	int newGameYOffset;

	ButtonRenderData buttonRenderData;
	RenderData menuTitle;
	CIwSVec2 titlePosition;

#if !defined DESKTOPPLATFORM
	ButtonRenderData facebookButtonRD;
	Button* facebookButton;

	ButtonRenderData twitterButtonRD;
	Button* twitterButton;
#endif

	void ConfigureXBOX360Callbacks();
};

#endif