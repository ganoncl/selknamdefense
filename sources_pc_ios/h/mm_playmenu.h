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

#ifndef _PLAYMENU_H
#define _PLAYMENU_H

#include "game_window.h"
#include "button.h"
#include "common.h"
#if defined DESKTOPPLATFORM
#include "focusmanager.h"
#include "xbox360controller.h"
#endif

#define PLAYMENU_MAX_NUMBER_BUTTONS 4

class PlayMenu : public GameWindow
{
public:
	PlayMenu();
	~PlayMenu();

	void InitObject();
	void EndObject();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

private:
	void SetMenuPositionWithResumeButton(bool bWithResume);

	ButtonWrapper buttonList[PLAYMENU_MAX_NUMBER_BUTTONS];

	ButtonRenderData buttonRenderData;
	RenderData menuTitle;
	CIwSVec2 titlePosition;

	int btnXPos;
	int btnYPos;
	int btnDistance;

#if !defined DESKTOPPLATFORM
	ButtonRenderData facebookButtonRD;
	Button* facebookButton;

	ButtonRenderData twitterButtonRD;
	Button* twitterButton;
#endif
};

#endif