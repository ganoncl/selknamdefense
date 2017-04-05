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

#include "ms_mapselect.h"
#include "background.h"
#include "common.h"
#include "utils.h"

MapSelection* mapselRef = NULL;

//Keys handler
int32 KeyboardHandler_msel(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(mapselRef == NULL)
		return -1;

	if(!mapselRef->bKeyboardBlocked)
		mapselRef->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotion_msel(s3ePointerMotionEvent* event) 
{
	if(mapselRef == NULL)
		return;

	if(!mapselRef->bMouseBlocked)
		mapselRef->UpdateMotionEvents(event);
}


//Mouse handler
void HandleClick_msel(s3ePointerEvent *event) 
{
	if(mapselRef == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
			{
				mapselRef->MouseHack();
			}
			else
			{
				mapselRef->ActiveButtonMouseHack();
			}
		}

		return;
	}

	if(!mapselRef->bMouseBlocked)
		mapselRef->UpdateClickEvents(event);
}

//Custom functions
void SelectLevel_OnClick()
{
	currentMap = mapselRef->map_button[mapselRef->level_focus].level;
	if((!bIsFullVersion && currentMap <= 7) || bIsFullVersion)
	{
		gameState = GS_Game;
		bFirstMapWasPlayed = true;
	}
}

void MS_Store_OnClick()
{
	mapSelectionState = MS_Store;
}

void MS_Back_OnClick()
{
	gameState = GS_MainMenu;
}

//Custom on motion functions
void MS_Store_OnMotion()
{
	mapselRef->ChangeButtonFocus(MS_FOCUS_Store);
}

void MS_Back_OnMotion()
{
	mapselRef->ChangeButtonFocus(MS_FOCUS_Back);
}

MapSelection::MapSelection()
{
	mapselRef = this;
	bMouseBlocked = false;
	bKeyboardBlocked = false;

	TextureData btn_mapselect_data = TextureHelper::GetTextureNamed("btn_mapselect");
	btn_mapselect.numStates = 2;

	btn_mapselect.state[0].RollOut  = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 0);
	btn_mapselect.state[0].RollOn   = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 1);
	btn_mapselect.state[0].Pressed  = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 0);
	btn_mapselect.state[0].Inactive = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 0);

	btn_mapselect.state[1].RollOut  = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 0);
	btn_mapselect.state[1].RollOn   = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 1);
	btn_mapselect.state[1].Pressed  = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 0);
	btn_mapselect.state[1].Inactive = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 0);

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	//Stars collected text
	starCounter = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_infocard_star"));
	starCounter.width = 196;
	starCounter.height = 139;
	if(starCounter.bRotate)
	{
		starCounter.width = 139;
		starCounter.height = 196;
	}
	starsCounter_position = CIwSVec2(980, 0);
	CIwSVec2 starCounterDimension = TextureHelper::GetDimension(starCounter);

	starsCollected_Position = CIwRect(starsCounter_position.x - 15, starsCounter_position.y + 22, starCounterDimension.x, starCounterDimension.y);
	starsCollected_Color = 0xffffffff;

	singleStarRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_star_little"));

	//Title
	pfont = font_komikadisplaybold25;
	titlePos = CIwRect(500, 20, 300, 30);
	titleColor = 0xffffffff;

	for(int i = 0; i < NUM_LEVELS; i++) 
	{
		map_button[i].map_btn = new Button(TextureHelper::GetCIwTexture(btn_mapselect.state[0].RollOut.atlasName), &btn_mapselect, 0, 0);
		map_button[i].map_btn->OnClick(&SelectLevel_OnClick);
		map_button[i].level = -1;
		map_button[i].pos.down = map_button[i].pos.up = map_button[i].pos.left = map_button[i].pos.right = -1;
	}

	//Button Positions
	map_button[0].map_btn->SetPos(196, 375);
	map_button[1].map_btn->SetPos(233, 242);
	map_button[2].map_btn->SetPos(203, 148);
	map_button[3].map_btn->SetPos(111, 82);
	map_button[4].map_btn->SetPos(186, 25);
	map_button[5].map_btn->SetPos(288, 55);
	map_button[6].map_btn->SetPos(400, 80);
	map_button[7].map_btn->SetPos(480, 60);
	map_button[8].map_btn->SetPos(410, 180);
	map_button[9].map_btn->SetPos(380, 275);
	map_button[10].map_btn->SetPos(490, 336);
	map_button[11].map_btn->SetPos(380, 430);
	map_button[12].map_btn->SetPos(490, 440);
	map_button[13].map_btn->SetPos(600, 500);
	map_button[14].map_btn->SetPos(674, 438);
	map_button[15].map_btn->SetPos(766, 533);
	map_button[16].map_btn->SetPos(890, 481);
	map_button[17].map_btn->SetPos(997, 463);
	map_button[18].map_btn->SetPos(1057, 348);
	map_button[19].map_btn->SetPos(938, 378);
	map_button[20].map_btn->SetPos(775, 391);
	map_button[21].map_btn->SetPos(678, 297);
	map_button[22].map_btn->SetPos(706, 172);
	map_button[23].map_btn->SetPos(829, 278);

	//levels
	map_button[0].level = 1;
	map_button[1].level = 2;
	map_button[2].level = 3;
	map_button[3].level = 4;
	map_button[4].level = 5;
	map_button[5].level = 6;
	map_button[6].level = 7;
	map_button[7].level = 8;
	map_button[8].level = 9;
	map_button[9].level = 10;
	map_button[10].level = 11;
	map_button[11].level = 12;
	map_button[12].level = 13;
	map_button[13].level = 14;
	map_button[14].level = 15;
	map_button[15].level = 16;
	map_button[16].level = 17;
	map_button[17].level = 18;
	map_button[18].level = 19;
	map_button[19].level = 20;
	map_button[20].level = 21;
	map_button[21].level = 22;
	map_button[22].level = 23;
	map_button[23].level = 24;

	//focus
	//up down left right
	map_button[0].pos.right = 11;
	map_button[0].pos.up = 1;
	map_button[0].pos.down = 25;
	
	map_button[1].pos.down = 0;
	map_button[1].pos.right = 9;
	map_button[1].pos.up = 2;
	
	map_button[2].pos.left = 3;
	map_button[2].pos.right = 8;
	map_button[2].pos.down = 1;
	map_button[2].pos.up = 3;
	
	map_button[3].pos.down = 2;
	map_button[3].pos.right = 4;
	map_button[3].pos.up = 4;
	
	map_button[4].pos.left = 3;
	map_button[4].pos.right = 5;
	map_button[4].pos.down = 3;
	
	map_button[5].pos.right = 6;
	map_button[5].pos.left = 4;
	
	map_button[6].pos.down = 8;
	map_button[6].pos.right = 7;
	map_button[6].pos.left = 5;
	
	map_button[7].pos.right = 22;
	map_button[7].pos.left = 6;
	
	map_button[8].pos.up = 6;
	map_button[8].pos.down = 9;
	map_button[8].pos.left = 2;
	
	map_button[9].pos.up = 8;
	map_button[9].pos.right = 10;
	map_button[9].pos.down = 10;
	map_button[9].pos.left = 1;

	map_button[10].pos.right = 8;
	map_button[10].pos.up = 9;
	map_button[10].pos.down = 11;
	map_button[10].pos.left = 9;
	
	map_button[11].pos.right = 12;
	map_button[11].pos.up = 10;
	map_button[11].pos.left = 0;
	map_button[11].pos.down = 25;
	
	map_button[12].pos.right = 13;
	map_button[12].pos.left = 11;
	map_button[12].pos.up = 10;
	map_button[12].pos.down = 25;
	
	map_button[13].pos.right = 14;
	map_button[13].pos.up = 13;
	map_button[13].pos.left = 12;
	map_button[13].pos.down = 26;

	map_button[14].pos.left = 13;
	map_button[14].pos.up = 20;
	map_button[14].pos.right = 15;
	
	map_button[15].pos.right = 16;
	map_button[15].pos.up = 14;
	map_button[15].pos.left = 14;
	map_button[15].pos.down = 26;
	
	map_button[16].pos.left = 15;
	map_button[16].pos.right = 17;
	map_button[16].pos.down = 26;

	map_button[17].pos.up = 19;
	map_button[17].pos.left = 16;
	map_button[17].pos.down = 26;
	
	map_button[18].pos.down = 17;
	map_button[18].pos.left = 19;
	
	map_button[19].pos.right = 18;
	map_button[19].pos.up = 23;
	map_button[19].pos.down = 17;
	map_button[19].pos.left = 20;
	
	map_button[20].pos.right = 19;
	map_button[20].pos.up = 21;
	map_button[20].pos.down = 15;
	map_button[20].pos.left = 14;
	
	map_button[21].pos.right = 23;
	map_button[21].pos.up = 22;
	map_button[21].pos.down = 20;
	map_button[21].pos.left = 10;
	
	map_button[22].pos.right = 23;
	map_button[22].pos.down = 21;
	map_button[22].pos.left = 7;
	
	map_button[23].pos.up = 22;
	map_button[23].pos.right = 19;
	map_button[23].pos.down = 20;
	map_button[23].pos.left = 21;

	for(int i = 0; i < NUM_LEVELS; i++)
	{
		int level = map_button[i].level;
		if(MapStatus_State[level - 1] == PLAYABLE)
		{
			//Stage was completed can be played yet.
			map_button[i].map_btn->SetImage(1);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
		else if(MapStatus_State[level - 1] == NOT_PLAYABLE)
		{
			//Stage locked, use column 3 on png image and disable the click function.
			map_button[i].map_btn->SetImage(2);
			map_button[i].map_btn->FunctionOnClickEnabled(false);
		}
		else if(MapStatus_State[level - 1] == CURRENT_PLAYABLE)
		{
			//Stage is not completed yet.
			map_button[i].map_btn->SetImage(0);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
	}

	/*
	//Example draw.
	num_vertical = 4;
	num_horizontal = 6;

	int btnLevelW = 95;
	int btnLevelH = 75;
	int btnLevelMargin = 40;
	for(int i = 0; i < num_vertical; i++)
	{
		for(int j = 0; j < num_horizontal; j++)
		{
			btnLevels[i][j] = new Button(btnLevelTex, 150 + btnLevelW * j + btnLevelMargin * j, 100 + btnLevelH * i + btnLevelMargin * i, 96); 
			btnLevels[i][j]->OnClick(&SelectLevel_OnClick);

			int level = i * num_horizontal + j;
			if(MapStatus_State[level] == PLAYABLE)
			{
				//Stage was completed can be played yet.
				btnLevels[i][j]->SetImage(1);
				btnLevels[i][j]->FunctionOnClickEnabled(true);
			}
			else if(MapStatus_State[level] == NOT_PLAYABLE)
			{
				//Stage locked, use column 3 on png image and disable the click function.
				btnLevels[i][j]->SetImage(2);
				btnLevels[i][j]->FunctionOnClickEnabled(false);
			}
			else if(MapStatus_State[level] == CURRENT_PLAYABLE)
			{
				//Stage is not completed yet.
				btnLevels[i][j]->SetImage(0);
				btnLevels[i][j]->FunctionOnClickEnabled(true);
			}
		}
	}
	*/

	
	//levelFocus = CIwVec2(0, 0);
	level_focus = 0;
	focus = MS_FOCUS_Level;
	int correction_x = 200;
	btnStore = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 940 - correction_x, 610);
	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 30 + correction_x, 610);
	btnStore->OnClick(&MS_Store_OnClick);
	btnBack->OnClick(&MS_Back_OnClick);
	btnStore->OnRollOn(&MS_Store_OnMotion);
	btnBack->OnRollOn(&MS_Back_OnMotion);

	predefined_position[0][0] = CIwSVec2(40, 20);
	predefined_position[1][0] = CIwSVec2(30, 20);
	predefined_position[1][1] = CIwSVec2(50, 20);
	predefined_position[2][0] = CIwSVec2(25, 30);
	predefined_position[2][1] = CIwSVec2(40, 15);
	predefined_position[2][2] = CIwSVec2(55, 30);

	//btnStore->SetStyle(font_komikadisplaybold25, 0, 0);

	title = "Map Selection";
	btnStore->SetText("UPGRADES");
	btnBack->SetText("BACK");
}

MapSelection::~MapSelection()
{
	/*
	for(int i = 0; i < num_vertical; i++)
		for(int j = 0; j < num_horizontal; j++)
			delete btnLevels[i][j];
	*/

	for(int i = 0; i < NUM_LEVELS; i++)
		delete map_button[i].map_btn;

	delete btnBack;
	delete btnStore;
}

void MapSelection::Render()
{
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(starCounter.atlasName), starsCounter_position, starCounter);
	
	IwGxFlush();

	if(bIsFullVersion)
	{
		for(int i = 0; i < NUM_LEVELS; i++)
		{
			if(map_button[i].map_btn->GetImageID() != 2)
			{
				map_button[i].map_btn->Render();
			}
		}
	}
	else
	{
		for(int i = 0; i < 7; i++)
		{
			if(map_button[i].map_btn->GetImageID() != 2)
			{
				map_button[i].map_btn->Render();
			}
		}
	}


	IwGxFlush();
	for(int i = 0; i < 24; i++)
	{
		for(int j = 0; j < Stars_By_Level[i]; j++)
		{
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(singleStarRenderData.atlasName), stars_position[i][j], singleStarRenderData);
		}
	}

	btnStore->Render();
	btnBack->Render();

	IwGxLightingOn();
	Utils::RenderText(starsCollected_Text, starsCollected_Position, font_komikadisplaybold18);
	int size = NUM_LEVELS;
	if(!bIsFullVersion) size = 7;
	for(int i = 0; i < size; i++)
	{
		if(map_button[i].map_btn->GetImageID() != 2)
		{
			char tmp[10];
			sprintf(tmp, "%d", map_button[i].level);
			Utils::RenderText(tmp, CIwRect(map_button[i].map_btn->GetX(), map_button[i].map_btn->GetY(), map_button[i].map_btn->GetWidth() - 12, map_button[i].map_btn->GetHeight() + 4), font_komikadisplaybold22, 0xff00edf8, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_BOTTOM, true, 2);
		}
	}
	IwGxLightingOff();
}

void MapSelection::SetMapButtons()
{
	for(int i = 0; i < NUM_LEVELS; i++)
	{
		int level = map_button[i].level;
		if(MapStatus_State[level - 1] == PLAYABLE)
		{
			//Stage was completed can be played yet.
			map_button[i].map_btn->SetImage(1);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
		else if(MapStatus_State[level - 1] == NOT_PLAYABLE)
		{
			//Stage locked, use column 3 on png image and disable the click function.
			map_button[i].map_btn->SetImage(2);
			map_button[i].map_btn->FunctionOnClickEnabled(false);
		}
		else if(MapStatus_State[level - 1] == CURRENT_PLAYABLE)
		{
			//Stage is not completed yet.
			map_button[i].map_btn->SetImage(0);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
	}

	int current_stars = 0;
	int num_levels = sizeof(Stars_By_Level) / 4;
	for(int i = 0; i < num_levels; i++) current_stars += Stars_By_Level[i];
	sprintf(starsCollected_Text, "%d / %d", current_stars, Max_Stars);

	if(bShowTutorialStars)
	{
		if(current_stars >= 3)
		{
			tutorial->SetSkipped(false);
			tutorial->InitTutorial(Tutorial_Stars);
			bShowTutorialStars = false;
		}
	}

	//stars per level
	for(int i = 0; i < NUM_LEVELS; i++)
	{
		for(int j = 0; j < Stars_By_Level[i]; j++)
		{
			stars_position[i][j] = predefined_position[Stars_By_Level[map_button[i].level - 1] - 1][j];
			stars_position[i][j].x += map_button[i].map_btn->GetX();
			stars_position[i][j].y += map_button[i].map_btn->GetY();
		}
	}

}

void MapSelection::InitObject()
{
	bExtraMapMode = false;
	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_msel, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_msel, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_msel, NULL);
	btnBack->IsFocused(false);
	btnStore->IsFocused(false);
	focus = MS_FOCUS_Level;
	bMouseBlocked = false;
	bKeyboardBlocked = false;

	SetMapButtons();
	focus = MS_FOCUS_None;
	bool bMapFounded = false;
	for(int i = 0; i < 24; i++)
	{
		if(MapStatus_State[i] == CURRENT_PLAYABLE)
		{
			ChangeFocus(i);
			bMapFounded = true;
			break;
		}
	}

	if(!bMapFounded)
	{
		ChangeFocus(0);
	}

	/*
	if(bUseOldAudioSystem)
	{
		//then start the next
		if(!SOUND_MUSIC_MUTE && !s3eAudioIsPlaying())
		{
			if(bIsAudioPaused)
			{
				LastMusicStacked = Music_Store;
				IwTrace(DEB, ("Music_Store"));
			}
			else
			{
				s3eAudioPlay("bgm/sd_store_theme_loop.mp3", 0);
				LastMusicStacked = Music_None;
			}
		}
	}
	*/
}

void MapSelection::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_msel);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_msel);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_msel);
}

void MapSelection::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bPauseMapSelection)
		return;

	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;
	
	int size = NUM_LEVELS;
	if(!bIsFullVersion) size = 7;

	for(int i = 0; i < size; i++)
	{
		if(map_button[i].map_btn->GetImageID() != 2)
			map_button[i].map_btn->HandleClick(event);
	}

	btnStore->HandleClick(event);
	btnBack->HandleClick(event);
}

void MapSelection::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bPauseMapSelection)
		return;

	int size = NUM_LEVELS;
	if(!bIsFullVersion) size = 7;

	for(int i = 0; i < size; i++)
	{
		if(map_button[i].map_btn->GetImageID() != 2)
		{
			map_button[i].map_btn->HandleMotion(event);
			if(map_button[i].map_btn->MouseOn(event->m_x, event->m_y))
			{
				ChangeFocus(i);
				btnBack->IsFocused(false);
				btnStore->IsFocused(false);
			}
		}
	}

	btnStore->HandleMotion(event);
	btnBack->HandleMotion(event);
}

void MapSelection::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(bPauseMapSelection)
		return;

	if(event->m_Pressed)
	{
		bMouseBlocked = true;
		lastKeyPressed = event->m_Key;

		if(event->m_Key == s3eKeyAbsRight) 
		{
			if(focus == MS_FOCUS_Level)
			{
				if(isValid(map_button[level_focus].pos.right))
					ChangeFocus(map_button[level_focus].pos.right);
				else
				{
					ChangeFocus(26);
				}
			}
			else if(focus == MS_FOCUS_Back)
			{
				btnStore->IsFocused(true);
				btnBack->IsFocused(false);
				focus = MS_FOCUS_Store;
			}
		}
		else if(event->m_Key == s3eKeyAbsLeft)
		{
			if(focus == MS_FOCUS_Level)
			{
				if(isValid(map_button[level_focus].pos.left))
					ChangeFocus(map_button[level_focus].pos.left);
			}
			else if(focus == MS_FOCUS_Store)
			{
				btnStore->IsFocused(false);
				btnBack->IsFocused(true);
				focus = MS_FOCUS_Back;
			}
		}
		else if(event->m_Key == s3eKeyAbsUp)
		{
			if(focus == MS_FOCUS_Level)
			{
				if(isValid(map_button[level_focus].pos.up))
					ChangeFocus(map_button[level_focus].pos.up);
			}
			else if(focus == MS_FOCUS_Back || focus == MS_FOCUS_Store)
			{
				btnStore->IsFocused(false);
				btnBack->IsFocused(false);
				//focus = MS_FOCUS_Level;
				ChangeFocus(level_focus);
			}
		}
		else if(event->m_Key == s3eKeyAbsDown)
		{
			if(focus == MS_FOCUS_Level && isValid(map_button[level_focus].pos.down))
				ChangeFocus(map_button[level_focus].pos.down);
		}
		else if(event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter)
		{
			
			if(focus == MS_FOCUS_Level)
				map_button[level_focus].map_btn->SetStatus(3);
			else if(focus == MS_FOCUS_Back)
				btnBack->SetStatus(3);
			else if(focus == MS_FOCUS_Store)
				btnStore->SetStatus(3);
		}
		else if(event->m_Key == s3eKeyEsc || event->m_Key == s3eKeyBack) 
		{
			gameState = GS_MainMenu;
		}
		else
		{
			if(focus == MS_FOCUS_Level);
				//map_button[level_focus].map_btn->Execute();
			else if(focus == MS_FOCUS_Back)
				btnBack->SetStatus(1);
			else if(focus == MS_FOCUS_Store)
				btnStore->SetStatus(1);
		}
	}
	else
	{
		bMouseBlocked = false;
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter)
		{
			if(focus == MS_FOCUS_Level)
			{
				//btnLevels[levelFocus.y][levelFocus.x]->Execute();
				//btnLevels[levelFocus.y][levelFocus.x]->SetStatus(1);
				map_button[level_focus].map_btn->Execute();
			}
			else if(focus == MS_FOCUS_Back)
			{
				btnBack->Execute();
			}
			else if(focus == MS_FOCUS_Store)
			{
				btnStore->Execute();
			}
		}
		//CHEAT: MUST BE DELETED BEFORE FINAL RELEASE!!!!!!!!
#ifdef IW_DEBUG
		else if(event->m_Key == s3eKey2)
		{
			for(int i = 0; i < 24; i++) 
				MapStatus_State[i] = PLAYABLE;
			SetMapButtons();
		}
#endif
	}
}

bool MapSelection::isValid(int position)
{
	//IwTrace(VALID, ("Position: %d State: %d", position, map_button[position].map_btn->GetState()));
	if(position == 26 || position == 25) return true;
	if(position < 0) return false;
	if(map_button[position].map_btn == NULL) return false;
	if(!bIsFullVersion && position >= 7) return false;
	return map_button[position].map_btn->GetImageID() != 2;
}

void MapSelection::ChangeFocus(int i)
{
	if(i == 25)
	{
		if(map_button[level_focus].map_btn != NULL) 
			map_button[level_focus].map_btn->IsFocused(false);

		btnBack->IsFocused(true);
		focus = MS_FOCUS_Back;
		return;
	}

	if(i == 26)
	{
		if(map_button[level_focus].map_btn != NULL) 
			map_button[level_focus].map_btn->IsFocused(false);

		btnStore->IsFocused(true);
		focus = MS_FOCUS_Store;
		return;
	}


	if(i >= NUM_LEVELS || i < 0)
		return;

	//btnLevels[levelFocus.y][levelFocus.x]->IsFocused(false);
	//btnLevels[i][j]->IsFocused(true);

	if(level_focus != i || focus != MS_FOCUS_Level)
	{
		if(map_button[level_focus].map_btn != NULL) 
			map_button[level_focus].map_btn->IsFocused(false);

		//map_button[i].map_btn->IsFocused(true);

		level_focus = i;
		focus = MS_FOCUS_Level;

		map_button[i].map_btn->IsFocused(true);
	}
	
	//map_button[i].map_btn->IsFocused(true);
}

void MapSelection::ChangeButtonFocus(MS_Focus newFocus)
{
	//btnLevels[levelFocus.y][levelFocus.x]->IsFocused(false);
	if(newFocus == focus) 
		return;
	
	map_button[level_focus].map_btn->IsFocused(false);

	btnBack->IsFocused(false);
	btnStore->IsFocused(false);

	if(newFocus == MS_FOCUS_Store) btnStore->IsFocused(true);
	else if(newFocus == MS_FOCUS_Back) btnBack->IsFocused(true);

	//levelFocus.y = num_vertical - 1;
	focus = newFocus;
	
}

void MapSelection::MouseHack()
{
	if(focus == MS_FOCUS_Level)
	{
		//btnLevels[levelFocus.y][levelFocus.x]->Execute();
		//btnLevels[levelFocus.y][levelFocus.x]->SetStatus(1);
		map_button[level_focus].map_btn->Execute();
	}
	else if(focus == MS_FOCUS_Back)
	{
		btnBack->Execute();
	}
	else if(focus == MS_FOCUS_Store)
	{
		btnStore->Execute();
	}
}

void MapSelection::ActiveButtonMouseHack()
{
	if(focus == MS_FOCUS_Level)
		map_button[level_focus].map_btn->SetStatus(3);
	else if(focus == MS_FOCUS_Back)
		btnBack->SetStatus(3);
	else if(focus == MS_FOCUS_Store)
		btnStore->SetStatus(3);
}