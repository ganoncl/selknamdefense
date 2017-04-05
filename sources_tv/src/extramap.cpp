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

#include "extramap.h"

ExtraMaps* ExtraMap_Reference = NULL;

//Keys handler
int32 KeyboardHandler_ExtraMap(void* sys, void*)
{
	/*
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(ExtraMap_Reference == NULL)
		return -1;

	if(!ExtraMap_Reference->bKeyboardBlocked)
		ExtraMap_Reference->UpdateKeyboardEvents(event);
	*/
	return 0;
}

//Mouse Motion
void HandleMotion_ExtraMap(s3ePointerMotionEvent* event) 
{
	/*
	if(ExtraMap_Reference == NULL)
		return;

	if(!ExtraMap_Reference->bMouseBlocked)
		ExtraMap_Reference->UpdateMotionEvents(event);
	*/
}


//Mouse handler
void HandleClick_ExtraMap(s3ePointerEvent *event) 
{
	/*
	if(ExtraMap_Reference == NULL)
		return;

	if(!ExtraMap_Reference->bMouseBlocked)
		ExtraMap_Reference->UpdateClickEvents(event);
	*/
}

void SelectExtraMap()
{
	/*
	currentMap = ExtraMap_Reference->map_selected + 1;
	IwTrace(EXTRAMAP, ("Amount of won stars: %d", Stars_By_ExtraLevel[currentMap - 1]));
	gameState = GS_Game;
	*/
}

void PrevPage_Function() 
{
	//ExtraMap_Reference->PrevPage();
}

void NextPage_Function()
{
	//ExtraMap_Reference->NextPage();
}

ExtraMaps::ExtraMaps()
{
	ExtraMap_Reference = this;
	/*
	//Settings
	dp                         = opendir("extramaps");
	//Space between map buttons
	margin_x                   = 10;
	margin_y                   = 10;
	//Position of the first map button (Top-Left Corner)
	_x                         = 100;
	_y                         = 100;
	num_vertical_map_buttons   = 5;
	num_horizontal_map_buttons = 4;

	if(dp == NULL)
		IwTrace(EXTRAMAP, ("No se encontro el directorio."));
	
	IwTrace(EXTRAMAP, ("Listing extra maps:"));
	num_extramaps = 0;
	while(dirp = readdir(dp))
	{
		IwTrace(EXTRAMAP, ("%s", dirp->d_name));
		map_list_name[num_extramaps++] = dirp->d_name;
	}

	closedir(dp);

	current_page = 0;

	CIwTexture* map_button_texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("mapselection00")->GetResNamed("MapSelectButton", IW_GX_RESTYPE_TEXTURE);
	map_button_width               = (int)(map_button_texture->GetWidth() * 0.33f);
	map_button_height              = (int)(map_button_texture->GetHeight() * 0.25f);
	for(int i = 0; i < MAX_MAPS_PER_PAGE; i++)
	{
		map_button[i] = new Button(map_button_texture, 0, 0, 96);
		map_button[i]->OnClick(&SelectExtraMap);
	}

	CIwTexture* normal_button_texture = (CIwTexture*)IwGetResManager()->GetGroupNamed("common_ui")->GetResNamed("button", IW_GX_RESTYPE_TEXTURE);
	prev_page_button = new Button(normal_button_texture, 100, 500);
	next_page_button = new Button(normal_button_texture, 700, 500);
	prev_page_button->OnClick(&PrevPage_Function);
	next_page_button->OnClick(&NextPage_Function);

	bMouseBlocked = false;
	bKeyboardBlocked = false;
	*/
}

ExtraMaps::~ExtraMaps()
{
	/*
	for(int i = 0; i < MAX_MAPS_PER_PAGE; i++)
		delete map_button[i];
	delete prev_page_button;
	delete next_page_button;
	*/
}

void ExtraMaps::InitObject()
{
	/*
	s3eAudioStop();
	bExtraMapMode = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_ExtraMap, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_ExtraMap, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_ExtraMap, NULL);

	current_page = 0;
	int k = 0;
	for(int i = 0; i < num_horizontal_map_buttons; i++)
	{
		for(int j = 0; j < num_vertical_map_buttons; j++)
		{
			map_button[k++]->SetPos(_x + j * (map_button_width + margin_x), _y + i * (map_button_height + margin_y));
		}
	}

	UpdateButtons();
	*/
}

void ExtraMaps::EndObject()
{
	/*
	//bExtraMapMode = false;
	IwTrace(EXTRAMAP, ("Desactivando device handling"));
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_ExtraMap);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_ExtraMap);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_ExtraMap);
	*/
}

void ExtraMaps::Update()
{
	/*
	remaining_maps = num_extramaps - MAX_MAPS_PER_PAGE * (current_page);
	num_map_current_page = min(remaining_maps, MAX_MAPS_PER_PAGE);
	*/
}

void ExtraMaps::Render()
{
	/*
	for(int i = 0; i < num_map_current_page; i++)
		map_button[i]->Render();
	prev_page_button->Render();
	next_page_button->Render();
	
	IwGxFlush();
	*/
}

void ExtraMaps::UpdateClickEvents(s3ePointerEvent *event)
{
	/*
	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	for(int i = 0; i < num_map_current_page; i++)
		map_button[i]->HandleClick(event);
	prev_page_button->HandleClick(event);
	next_page_button->HandleClick(event);
	*/
}

void ExtraMaps::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	/*
	for(int i = 0; i < num_map_current_page; i++)
	{
		map_button[i]->HandleMotion(event);
		if(map_button[i]->MouseOn(event->m_x, event->m_y))
		{
			map_selected = MAX_MAPS_PER_PAGE * current_page + i;
		}
	}
	prev_page_button->HandleMotion(event);
	next_page_button->HandleMotion(event);
	*/
}

void ExtraMaps::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	/*
	if(event->m_Pressed)
	{
		bMouseBlocked = true;
		lastKeyPressed = event->m_Key;
	}
	else
	{
		bMouseBlocked = false;
		if(event->m_Key == s3eKeyEsc || event->m_Key == s3eKeyBack) 
		{
			gameState = GS_MainMenu;
		}
	}
	*/
}

void ExtraMaps::NextPage()
{
	/*
	current_page++;
	UpdateButtons();
	*/
}

void ExtraMaps::PrevPage()
{
	/*
	current_page--;
	UpdateButtons();
	*/
}

void ExtraMaps::UpdateButtons()
{
	/*
	remaining_maps = num_extramaps - MAX_MAPS_PER_PAGE * (current_page);
	IwTrace(EXTRAMAP, ("LOOPEANDO"));

	if(current_page == 0)
		prev_page_button->FunctionOnClickEnabled(false);
	else
		prev_page_button->FunctionOnClickEnabled(true);
	
	if(remaining_maps <= MAX_MAPS_PER_PAGE)
	{
		next_page_button->FunctionOnClickEnabled(false);
	}
	else
		next_page_button->FunctionOnClickEnabled(true);
		*/

}