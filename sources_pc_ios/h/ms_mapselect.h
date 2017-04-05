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

#ifndef _MAPSELECTION_H
#define _MAPSELECTION_H

#include "button.h"
#include "background.h"
#include "utils.h"
#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

#define NUM_LEVELS 25

struct MapButton {
	Button* map_btn;
	int level;
};

class MapSelection
{
public:
	MapSelection();
	~MapSelection();
	void Render();
	void InitObject();
	void EndObject();
	void Update();
	bool bIsActiveObj;

	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	s3eKey lastKeyPressed;
	int num_horizontal;
	int num_vertical;

	MapButton map_button[NUM_LEVELS];
	Button* btnUpgrade;

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;

	void LinkMapButtons(int source, int up, int down, int left, int right);
#endif

private:
	CIwGxFont* pfont;
	CIwRect titlePos;
	uint32 titleColor;

	char starsCollected_Text[100];
	CIwRect starsCollected_Position;
	uint32 starsCollected_Color;

	float xModifier;
	float yModifier;

	
	Button* btnBack;

	CIwSVec2 starsCounter_position;

	//Stars per level
	CIwSVec2 stars_position[25][3];
	CIwSVec2 predefined_position[3][3];

	void SetMapButtons();
	bool isValid(int position);

	RenderData singleStarRenderData;
	ButtonRenderData buttonRenderData;
	ButtonRenderData btn_mapselect;
	RenderData starCounter;

	CIwGxFont *font_MapNumber;

	int singleStarEasterEggOffsetXPos;
	int singleStarEasterEggOffsetYPos;

};

#endif
