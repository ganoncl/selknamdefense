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

#ifndef _ALMANAC_H
#define _ALMANAC_H

#include "IwGx.h"
#include "IwGxFont.h"
#include "s3e.h"
#include "button.h"
#include "background.h"
#include <string>
#include "focuseableobject.h"
#include "utils.h"
using namespace std;

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

class PortraitFocuseableObject : public FocuseableObject {
public:
	PortraitFocuseableObject()
	{
		bIsFocused = false;
		bLocked = true;
		i = j = -1;
		w = h = x = y = 0;
		onSelect = NULL;
	}

	void EnableFocus()
	{
		bIsFocused = true;
	}

	void UnableFocus()
	{
		bIsFocused = false;
	}

	void ExecuteFunction()
	{
		if(onSelect != NULL)
			onSelect(i, j);
	}

	bool HandleFocusMotion(s3ePointerMotionEvent* event)
	{
		return Utils::MouseOn(event->m_x, event->m_y, x, y, w, h);
	}

	bool bLocked;
	bool bIsFocused;
	int i, j;
	int w, h, x, y;

	void (*onSelect)(int i, int j);
};

class Almanac
{
public:
	Almanac();
	~Almanac();
	void Render();
	void On();
	void Off();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void Update();
	void ClosePopup();
	void OpenPopup(int i, int j);

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;

	void CreateFocus(int i, int j);
	bool ValidPortraitPos(int i, int j);

	int lastRealI;
	int lastRealJ;
#endif

	Button* btnClose;
	bool bViewingDetailedInfo;

private:
	void SetParagraph(const char* title, const char* firstPar);
	void SetImageAndText(int i, int j);
	bool MouseOn(int x, int y, int xPos, int yPos, int _width, int _height);
	int realImageI, realImageJ;
	Background* moreInfoBackground;
	RenderData portraitArray[8][5];
	PortraitFocuseableObject portraitObject[4][3];
	int disabledPortrait[4][3];
	int xPortrait, yPortrait;
	
	CIwTexture* selknamRealImage[12];
	
	char detailedInfoTitle[50];
	char detailedInfoText[500];
	int pannelBPosX,pannelBPosY,w,h;

	//Buttons
	Button* btnBack;
	int btnBackPosX;
	int btnBackPosY;

	int widthSelknamAlmanacPannel;
	int heightSelknamAlmanacPannel;

	ButtonRenderData buttonRenderData;
	RenderData graphic_logo_marmalade;
	ButtonRenderData* button;

	int pannelBLengthX, pannelBLengthY;

	int renderPanelNumWidth;
	int renderPanelNumHeight;

	int pannelXPos;
	int pannelYPos;

	int buttonsXOffset;
	int buttonsYOffset;
};

#endif