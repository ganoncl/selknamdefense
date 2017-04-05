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

#ifndef _MAINMENUPOPUP_H
#define _MAINMENUPOPUP_H
#include "button.h"
#include "background.h"

#if defined DESKTOPPLATFORM
class Xbox360Controller;
class FocusManager;
#endif

class RenderData;

typedef void (*function)();

class PopUp {
public:
	PopUp();
	~PopUp();
	void Render();
	void OnAccept(function func);
	void On();
	void Off();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void Update();

	//Error popup mode
	bool bIsErrorMode;
	bool bAdviceMode;

	bool bMouseBlocked;
	bool bKeyboardBlocked;

#if defined DESKTOPPLATFORM
	Xbox360Controller* xbox360Controller;
	FocusManager* focusManager;
#endif

private:
	Background* background;

	Button* btnAccept;
	Button* btnCancel;
	const char* text;
	CIwGxFont* pfont;
	function onAcceptFunc;

	//background settings
	CIwSVec2 xy3[4];
	int alpha;

	//box container settings
	CIwSVec2 boxPos[4];
	int boxPosX;
	int boxPosY;
	int boxWidth;
	int boxHeight;
	int btnAcceptPosX;
	int btnAcceptPosY;
	int btnCancelPosX;
	int btnCancelPosY;
	int titleX;
	int titleY;
	int titleW;
	int titleH;
	int textX;
	int textY;
	int textW;
	int textH;
	int buttonWidth;

	s3eKey lastKeyPressed;

	int _x;
	int _y;
	int panelWidth, panelHeight;
	ButtonRenderData* button;

	bool bNeedCheckInternetConn;
};

#endif