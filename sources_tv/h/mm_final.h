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
#include "utils.h"

typedef enum
{
	FSFocus_None,
	FSFocus_Continue,
	FSFocus_Credits
} FSFocus;

class FinalScreen
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
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	void ContinueAction();
	void CreditsAction();
	void SetFocus(FSFocus newFocus);
	void MouseHack();
	void ActiveButtonMouseHack();
private:
	CIwTexture* ThanksTexture;
	FadeScreen* FinalScreenFade;
	Timer _Timer;
	Button* ContinueButton;
	Button* CreditsButton;
	bool bRenderFrames;
	int ImagesFadeAmount;
	FSFocus focus;
	bool bKeyboard;
	bool bMouse;
	s3eKey lastKeyPressed;

	ButtonRenderData buttonRenderData;
	RenderData menuTitle;
	RenderData thankData;
};

#endif