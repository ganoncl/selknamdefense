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

#ifndef _ALMANACENEMY_H
#define _ALMANACENEMY_H

#include "IwGx.h"
#include "common.h"
#include "localization.h"
#include "texturehelper.h"
#include "utils.h"

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

class EnemyPortrait : public FocuseableObject
{
public:
	EnemyPortrait()
	{
		bIsFocused = false;
		x = y = w = h = 0;
		onClick = NULL;
		i = j = -1;
	}

	void ExecuteFunction()
	{
		if(onClick != NULL)
			onClick(i, j);
	}

	void EnableFocus()
	{
		bIsFocused = true;
	}

	void UnableFocus()
	{
		bIsFocused = false;
	}

	bool HandleFocusMotion(s3ePointerMotionEvent* event)
	{
		return Utils::MouseOn(event->m_x, event->m_y, x, y, w, h);
	}

	int x, y, w, h;
	bool bIsFocused;
	int i, j;
	void (*onClick)(int i, int j);
};

class AlmanacEnemy
{
public:
	AlmanacEnemy();
	~AlmanacEnemy();
	void InitObject();
	void EndObject();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void ClosePopup();
	void SetImageAndText(int i, int j);

	bool bViewingDetailedInfo;

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

private:
	CIwSVec2 GetPortraitPosition(int i, int j);

	void SetPortraitEnemy(int index, string name);
	void SetParagraph(const char* title, const char* firstPar);
	void SetIndexToEnemyType(IsoEnemyType type, int index);

	int GetIndexOfPortrait(int portraitIndex);

	bool bIsActive;

	Background* moreInfoBackground;
	
	char detailedInfoTitle[50];
	char detailedInfoText[500];
	ButtonRenderData* button;
	Button* btnClose;
	int pannelBLengthX, pannelBLengthY;
	int pannelBPosX,pannelBPosY,w,h;

	ButtonRenderData buttonRenderData;
	Button* btnBack;
	int btnBackPosX;
	int btnBackPosY;

	int pannelUnit;
	int pannelWidth;
	int pannelHeight;
	int pannelXPos;
	int pannelYPos;
	int pannelLengthX, pannelLengthY;

	RenderData backgroundButtonData;
	AtlasName backgroundButtonAtlasName;
	CIwSVec2 backgroundButtonDimension;

	RenderData backgroundFocusButtonData;
	AtlasName backgroundFocusButtonAtlasName;
	CIwSVec2 backgroundFocusButtonDimension;

	RenderData enemyPortrait[MAX_ENEMIES];
	AtlasName  enemyPortraitAtlasName[MAX_ENEMIES];
	CIwTexture* enemyRealImage[MAX_ENEMIES];

	EnemyPortrait portraits[2][4];
	
	int enemyIindex, enemyJIndex;

	int indexToEnemyType[MAX_ENEMIES];

	int buttonEnemyPortraitWidth;
	int buttonEnemyPortraitHeight;

	int buttonEnemyPortraitXOffset;
	int buttonEnemyPortraitYOffset;
};

#endif