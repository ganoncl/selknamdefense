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
#include "texturehelper.h"

#define NUM_LEVELS 24

typedef enum
{
	MS_FOCUS_None,
	MS_FOCUS_Level,
	MS_FOCUS_Store,
	MS_FOCUS_Back,
} MS_Focus;

struct FocusPosition {
	int up;
	int down;
	int left;
	int right;
};

struct MapButton {
	Button* map_btn;
	int level;
	FocusPosition pos;
};

class MapSelection
{
public:
	MapSelection();
	~MapSelection();
	void Render();
	void InitObject();
	void EndObject();
	bool bIsActiveObj;

	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	bool bMouseBlocked;
	bool bKeyboardBlocked;
	s3eKey lastKeyPressed;

	void ChangeFocus(int i);
	void ChangeButtonFocus(MS_Focus newFocus);
	//CIwVec2 levelFocus;
	int level_focus;
	
	MS_Focus focus;

	int num_horizontal;
	int num_vertical;

	MapButton map_button[NUM_LEVELS];

	void MouseHack();
	void ActiveButtonMouseHack();

private:
	CIwGxFont* pfont;
	const char* title;
	CIwRect titlePos;
	uint32 titleColor;

	char starsCollected_Text[100];
	CIwRect starsCollected_Position;
	uint32 starsCollected_Color;

	//Button* btnLevels[10][10]; 


	Button* btnStore;
	Button* btnBack;

	CIwSVec2 starsCounter_position;

	//Stars per level
	CIwTexture* star_texture;
	CIwSVec2 stars_position[24][3];
	CIwSVec2 predefined_position[3][3];

	void SetMapButtons();
	bool isValid(int position);

	RenderData singleStarRenderData;
	ButtonRenderData buttonRenderData;
	ButtonRenderData btn_mapselect;
	RenderData starCounter;
};

#endif
