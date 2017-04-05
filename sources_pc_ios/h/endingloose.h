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

#ifndef _ENDINGLOOSE_H
#define _ENDINGLOOSE_H

#include "button.h"
#include "common.h"
#include "background.h"
#include "utils.h"
#include "time.h"

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

class EndingLoose
{
public:
	EndingLoose();
	~EndingLoose();
	void Update();
	void Render();
	void InitObject();
	void EndObject();
	bool bIsActiveObj;

	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	s3eKey lastKeyPressed;

	Timer time_defeat_picture;
	Background* background;

	CIwSVec2 defeat_position;

	bool bShowDefeatPicture;

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

private:
	Button* btnRetry;
	Button* btnMapSelect;
	Button* btnQuitGame;
	int buttonX;

	//Background
	CIwColour cols[4];
	CIwSVec2 pos[4];
	int _width;
	int _height;
	int _x;
	int _y;

	CIwGxFont* pfont;
	uint32 textColor;
	CIwRect titlePos;
	const char* title;

	ButtonRenderData buttonRenderData;
	RenderData defeatRenderData;
	CIwSVec2 defeatDimension;

	//Survival Mode
	bool bNewRecord;
	int panelWidth, panelHeight, panelX, panelY;
	char strNewRecord[100];
	char strBestRecord[100];

#if !defined DESKTOPPLATFORM
	ButtonRenderData facebookButtonRD;
	Button* facebookButton;

	ButtonRenderData twitterButtonRD;
	Button* twitterButton;
#endif

	bool bShowingiAd;
};

#endif
