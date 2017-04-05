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

#ifndef _MENUIG_H
#define _MENUIG_H

#include "IwGx.h"
#include "s3e.h"
#include "button.h"
#include "background.h"
#include "texturehelper.h"

typedef enum
{
	Focus_MIG_None,
	Focus_MIG_Resume,
	//Focus_MIG_HowToPlay,
	Focus_MIG_Retry,
	Focus_MIG_Options,
	Focus_MIG_QuitGame,
} Focus_MIG;

class MenuIG
{
public:
	MenuIG();
	~MenuIG();
	void InitObject();
	void EndObject();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	bool ChangeFocus(Focus_MIG newFocus);
	Focus_MIG Focus;
	bool bIsActiveObj;

	bool bMouseBlocked;
	bool bKeyboardBlocked;

	Background* background;

	void MouseHack();
	void ActiveButtonMouseHack();
	
private:
	CIwGxFont* pfont;

	const char* title;
	CIwRect titlePos;
	uint32 color;

	CIwResGroup* pGroup;
	Button* resume;
	Button* howtoplay;
	Button* retry;
	Button* options;
	Button* quitgame;
	CIwFVec2	s_UV[4];
	s3eKey lastKeyPressed;

	ButtonRenderData buttonRenderData;
	int buttonWidth;
	int buttonHeight;
};

#endif