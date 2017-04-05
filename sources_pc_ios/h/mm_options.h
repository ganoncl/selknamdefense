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

#ifndef _OPTIONS_MENU_H
#define _OPTIONS_MENU_H

#include "game_window.h"
#include "texturehelper.h"
#include "button.h"

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

#if defined DESKTOPPLATFORM
#define OPTIONS_NUM_BUTTONS 3
#endif
#if !defined DESKTOPPLATFORM
#define OPTIONS_NUM_BUTTONS 4
#endif

class OptionsMenu : public GameWindow
{
public:
	OptionsMenu();
	~OptionsMenu();

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
	RenderData menuTitle;
	CIwSVec2 titlePosition;
	ButtonRenderData buttonRenderData;
	Button* buttonList[OPTIONS_NUM_BUTTONS];
};

#endif