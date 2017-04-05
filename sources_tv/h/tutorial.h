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

#ifndef _TUTORIAL_H
#define _TUTORIAL_H

#include "IwGx.h"
#include "button.h"
#include "timer.h"
#include <string>
using namespace std;

class ButtonRenderData;
class RenderData;

typedef enum 
{
	Tutorial_None,
	Tutorial_Map1,
	Tutorial_Stars,
	Tutorial_UnitMenu
} TutorialName;

typedef enum
{
	TutorialFocus_None,
	TutorialFocus_Skip,
	TutorialFocus_Ok,
	TutorialFocus_Finish
} TutorialFocus;

class Tutorial
{
private:
	TutorialName CurTutorial;
	int CurTutorialStep;
	Button* SkipButton;
	Button* OkButton;
	Button* FinishButton;
	bool bRenderSkipButton;
	bool bRenderOkButton;
	bool bRenderFinishButton;
	TutorialFocus focus;
	bool bSkipped;
	void DrawMessage(string message);
	void InitDeviceHandler();
	void StopDeviceHandler();
	void SetFocus(TutorialFocus newFocus);
	bool bFirstOpen;
	Timer openTimer;
	ButtonRenderData *redButton;
	ButtonRenderData *greenButton;
	RenderData* arrow;

public:
	bool bMouseBlocked;
	bool bKeyboardBlocked;

	Tutorial();
	~Tutorial();
	void Update();
	void Render();
	void InitTutorial(TutorialName _TutorialName);
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);	
	void EndTutorial();
	void NextTutorialFrame();
	void FinishTutorial();
	void SetSkipped(bool b) { bSkipped = b; }
	void MouseHack();
	void ActiveButtonMouseHack();
};

#endif