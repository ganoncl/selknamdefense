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

#include "texturehelper.h"

typedef enum
{
	FOCUS_NONE,
	FOCUS_LOADGAME,
	FOCUS_NEWGAME,
	//FOCUS_HOWTOPLAY,
	//FOCUS_OPTIONS,
	FOCUS_CREDITS,
	FOCUS_QUITGAME
} FocusState;

class Menu {
public:
	Menu();
	~Menu();
	void InitObject();
	void EndObject();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	bool ChangeFocus(FocusState newFocus);
	void On();
	void Off();
	FocusState Focus;
	bool bIsActiveObj;
	bool bIsOn;

	bool bMouseBlocked;
	bool bBlockDeviceHandler;
	bool bKeyboardBlocked;
	Timer Timer_BlockAfterFadeOut;
	bool bStartTimer;
	bool bDoJustOnceInWhileGame;
	
	void SetMenuPositionWithResumeButton(bool bWithResume);
	
	void MouseHack();
	void ActiveButtonHack();
private:
	CIwResGroup* pGroup;
	Button* loadgame;
	Button* newgame;
	Button* howtoplay;
	Button* options;
	Button* credits;
	Button* quitgame;
	CIwFVec2	s_UV[4];
	s3eKey lastKeyPressed;
	//Title
	CIwTexture* imgTitle;
	CIwSVec2 posTitle[4];
	CIwFVec2 uvTitle[4];

	int btnX;
	int btnY;
	int btnH;
	int margin;

	bool bFirstFocus;
	ButtonRenderData buttonRenderData;

	RenderData menuTitle;
	CIwSVec2 titlePosition;

	RenderData steamPromoData;
	CIwSVec2 steamPromoPos;

	RenderData moreGamesData;
	CIwSVec2 moreGamesPos;
};

#endif