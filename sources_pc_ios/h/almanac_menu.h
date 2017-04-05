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

#ifndef _ALMANACMENU_H
#define _ALMANACMENU_H

#include "common.h"
#include "localization.h"
#include "utils.h"
#include "texturehelper.h"

#if defined DESKTOPPLATFORM
#include "focusmanager.h"
#include "xbox360controller.h"
#endif

class AlmanacMenu
{
public:
	AlmanacMenu();
	~AlmanacMenu();
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
	int pannelLengthX;
	int pannelLengthY;
	int pannelWidth;
	int pannelHeight;
	int pannelYPos;
	int pannelSelknamXPos;
	int pannelEnemyXPos;
	int selknamIconXPos, selknamIconYPos;
	int enemyIconXPos, enemyIconYPos;
	int pannelUnit;

	RenderData selknamIcon, enemyIcon;
	AtlasName selknamIconAtlasName, enemyIconAtlasName;
	CIwSVec2 selknamIconDimension, enemyIconDimension;

	ButtonRenderData buttonRenderData;
	Button* btnBack;
	int btnBackPosX;
	int btnBackPosY;

	Button* viewSelknamBtn;
	Button* viewEnemiesBtn;
};

#endif