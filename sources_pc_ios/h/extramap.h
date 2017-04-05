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

#ifndef _EXTRAMAP_H
#define _EXTRAMAP_H

#include "IwGx.h"
#include "Button.h"
#include "common.h"
#include <dirent.h>
#include <string>
using namespace std;

#define MAX_MAPS_PER_PAGE 20

class ExtraMaps
{
public:
	ExtraMaps();
	~ExtraMaps();
	void InitObject();
	void EndObject();
	void Update();
	void Render();

	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	bool bMouseBlocked;
	bool bKeyboardBlocked;
	s3eKey lastKeyPressed;

	int map_selected;

	void NextPage();
	void PrevPage();

private:
	//extra maps directory
	struct dirent *dirp;
	DIR *dp;

	int num_extramaps;
	int current_page;
	int num_map_current_page;
	int margin_x;
	int margin_y;
	int _x;
	int _y;
	int map_button_width;
	int map_button_height;
	int num_vertical_map_buttons;
	int num_horizontal_map_buttons;
	int remaining_maps;
	string map_list_name[MAX_EXTRA_MAPS];

	Button* map_button[MAX_MAPS_PER_PAGE];
	Button* next_page_button;
	Button* prev_page_button;

	void UpdateButtons();
};

#endif