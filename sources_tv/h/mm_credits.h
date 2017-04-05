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
#include "texturehelper.h"
using namespace std;

typedef enum
{
	C_FOCUS_NONE,
	C_FOCUS_BACK,
} CreditsFocus;

class Credits
{
public:
	Credits();
	~Credits();
	void Render();
	void On();
	void Off();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	void ChangeFocus(CreditsFocus newFocus);
	void MouseHack();
	void ActiveButtonMouseHack();
	CreditsFocus focus;

	bool bMouseBlocked;
	bool bKeyboardBlocked;

private:
	Background* background;

	CIwGxFont* pfont;
	const char* screenTitle; 
	int numMembers; //Nº names that appears on credits window.
	CIwRect textPos[4][2];
	const char* rol[5];
	const char* name[5];

	CIwFVec2 s_UV[4];
	CIwSVec2 img1Pos[4];
	CIwSVec2 img2Pos[4];
	int img1X;
	int img1Y;
	int img2X;
	int img2Y;

	//Container
	CIwSVec2 xy3[4];
	CIwColour cols[4];
	int containerW;
	int containerH;
	int containerX;
	int containerY;
	
	//Buttons
	Button* btnBack;
	int btnBackPosX;
	int btnBackPosY;

	//Text, title
	int titleX;
	int titleY;
	int titleW;
	int titleH;
	uint32 textColor;
	uint32 titleColor;

	s3eKey lastKeyPressed;

	//Mail text and rect
	string  strMail;
	CIwRect rectMail;
	uint32  colorMail;
	bool    bHasShadowMail;

	RenderData rdCreditsPannel;
	int widthCreditsPannel;

	ButtonRenderData buttonRenderData;
	RenderData graphic_logo_marmalade;
};

#endif