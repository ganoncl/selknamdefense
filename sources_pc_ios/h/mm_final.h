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

#ifndef _FINAL_H
#define _FINAL_H

#include "IwTexture.h"
#include "IwGx.h"
#include "s3e.h"
#include "fadescreen.h"
#include "timer.h"
#include "common.h"
#include "localization.h"
#include "utils.h"
#include "game_window.h"

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

class FinalScreen : public GameWindow
{
public:
	FinalScreen();
	~FinalScreen();
	void Update();
	void Render();
	void InitObject();
	void EndObject();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void ContinueAction();

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

private:
	FadeScreen* FinalScreenFade;
	Timer _Timer;
	Button* ContinueButton;
	bool bRenderFrames;
	int ImagesFadeAmount;
	bool bKeyboard;
	bool bMouse;
	s3eKey lastKeyPressed;

	ButtonRenderData buttonRenderData;
	RenderData menuTitle;
	RenderData thankData;
	int thanksXPos;
	bool bIsActive;
};

#endif