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

#include "endingwin.h"
#include "common.h"
#include "parserhelper.h"
#include "utils.h"
#include "fadescreen.h"

EndingWin *winRef = NULL;

int32 KeyboardHandler_win(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(!winRef->bKeyboardBlocked)
		winRef->UpdateKeyboardEvents(event);
	return 0;
}

//Mouse Motion
void HandleMotion_win(s3ePointerMotionEvent* event) 
{
	if(winRef == NULL)
		return;
	
	if(!winRef->bMouseBlocked)
		winRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_win(s3ePointerEvent *event) 
{
	if(winRef == NULL)
		return;

	if(!winRef->bMouseBlocked)
		winRef->UpdateClickEvents(event);
}

void Win_OnClick_Continue()
{
	if(!bIsFullVersion && currentMap == 7)
	{
		gameState = GS_MainMenu;
		mainMenuState = GSM_TrialVersion;
		inGameState = GSG_None;
		bShowTrialVersionFinal = true;
		return;
	}

	//Game complete.
	if(currentMap == 24)
	{
		gameState = GS_MainMenu;
		mainMenuState = GSM_FinalScreen;
		inGameState = GSG_None;
		return;
	}

	if(!bExtraMapMode)
		gameState = GS_MapSelection;
	else
		gameState = GS_ExtraMapSelection;
	inGameState = GSG_None;
}

EndingWin::EndingWin()
{
	winRef = this;
	
	//Background color
	cols[0].Set(0xbfb77b49);   
	cols[1].Set(0xbfb77b49);
	cols[2].Set(0xbfb77b49);
	cols[3].Set(0xbfb77b49);

	//Set font
	pfont = font_komikadisplaybold25;

	label_star_big = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_star_big"));

	TextureData tdBackground = TextureHelper::GetTextureNamed("pannel_credits");
	rdBackground = TextureHelper::GetRenderData(tdBackground);
	panelWidth = 20 * 40;
	panelPosition = CIwSVec2((int)((IwGxGetScreenWidth() - panelWidth) * 0.5f),
		(int)((IwGxGetScreenHeight() - (13 * 40)) * 0.5f));

	//Position and size
	_x = 140;
	_y = 60;
	_width = 1000;
	_height = 600;
	pos[0] = CIwSVec2(_x, _y);
	pos[1] = CIwSVec2(_x, _y + _height);
	pos[2] = CIwSVec2(_x + _width, _y + _height);
	pos[3] = CIwSVec2(_x + _width, _y);

	background = new Background(0,0,0, 170);

	//Text properties
	textColor = 0xffffffff;
	int text1_pos = panelPosition.x;
	int text1_w   = (int)(panelWidth * 0.5f);
	titlePos = CIwRect(panelPosition.x, 160, panelWidth, 30);
	timePos  = CIwRect(text1_pos, 230, text1_w, 30);
	starsPos = CIwRect(text1_pos, 230 + 50, text1_w, 30);
	levelPos = CIwRect(text1_pos, 230 + 100, text1_w, 30);
	coinsPos = CIwRect(text1_pos, 230 + 150, text1_w, 30);
	totalPos = CIwRect(text1_pos, 230 + 200, text1_w, 30);

	timePos_n  = CIwRect(650, 230, 250, 30);
	starsPos_n = CIwRect(650, 230 + 50, 250, 30);
	levelPos_n = CIwRect(650, 230 + 100, 250, 30);
	coinsPos_n = CIwRect(650, 230 + 150, 250, 30);
	totalPos_n = CIwRect(650, 230 + 200, 250, 30);

	title = "VICTORY";
	SetTimeBonus(0);
	SetStarsBonus(3);
	SetLevelBonus(1);
	SetCoinsBonus(2222);
	SetLifes(8);
	UpdateInfo();

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	int btnContinueWidth = buttonRenderData.state[0].RollOut.width;
	btnContinue = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 500, 500);
	btnContinue->OnClick(&Win_OnClick_Continue);
	btnContinue->SetText("CONTINUE");

	bIsActiveObj = true;

	bMouseBlocked = false;
	bKeyboardBlocked = false;

	stars_speed = 10;
	level_speed = 10;
	coins_speed = 10;

	float w = panelWidth * 0.33f + 55;
	float h = 420;

	star_position[0][0] = CIwSVec2((int16)(panelPosition.x + w - 40 + 80), (int16)h);

	star_position[1][0] = CIwSVec2((int16)(panelPosition.x + w - 40 + 40), (int16)h);
	star_position[1][1] = CIwSVec2((int16)(panelPosition.x + w - 40 + 80 + 40), (int16)h);

	star_position[2][0] = CIwSVec2((int16)(panelPosition.x + w - 40), (int16)h);
	star_position[2][1] = CIwSVec2((int16)(panelPosition.x + w - 40 + 80), (int16)h);
	star_position[2][2] = CIwSVec2((int16)(panelPosition.x + w - 40 + 160), (int16)h);

	num_stars = 0;
	bBlockContinueButton = false;

	heart_counter.SetTextStyle(timePos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
	coins_counter.SetTextStyle(starsPos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
	wave_counter.SetTextStyle(levelPos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
	total_counter.SetTextStyle(coinsPos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

	heart_counter.SetTime(750);
	coins_counter.SetTime(1000);
	wave_counter.SetTime(1250);
	total_counter.SetTime(1500);

	TextureData VictoryData = TextureHelper::GetTextureNamed("graphic_victory");
	VictoryRenderData = TextureHelper::GetRenderData(VictoryData);
	VictoryPosition = CIwSVec2((int)((IwGxGetScreenWidth() - VictoryData.textureWidth) * 0.5f), (int)((IwGxGetScreenHeight() - VictoryData.textureHeight) * 0.5f));
}

EndingWin::~EndingWin()
{
	delete btnContinue;
	delete background;
}

void EndingWin::Update()
{
	heart_counter.Update();
	coins_counter.Update();
	wave_counter.Update();
	total_counter.Update();

	star_timer.Update();
	time_victory_picture.Update();
	if(time_victory_picture.GetIsEnd() && bShowVictoryPicture) {
		bShowVictoryPicture = false;
		heart_counter.InitAnimation();
		coins_counter.InitAnimation();
		wave_counter.InitAnimation();
		total_counter.InitAnimation();
	}

	
	if(!bShowVictoryPicture)
	{
		UpdateInfo();
		if(total_counter.AnimationEnded())
		{
			bRenderStars = true;
		}

		if(current_stars == num_stars)
		{
			bBlockContinueButton = false;
		}
	}
	
}

void EndingWin::Render()
{
	if(!bShowVictoryPicture)
	{
		background->Render();
		Utils::RenderPannelA(18, 11, panelPosition.x, panelPosition.y);
		//Text
		IwGxLightingOn();
		Utils::RenderText(title, CIwRect(titlePos.x - 4, titlePos.y + 4, titlePos.w, titlePos.h), font_komikadisplaybold48, 0xff000000);
		Utils::RenderText(title, titlePos, font_komikadisplaybold48, 0xff00e0f7);
		//Utils::RenderText(timeText, timePos, pfont, textColor,   IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText("Heart Bonus : ", timePos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);
		Utils::RenderText("Coins Bonus : ", starsPos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);
		Utils::RenderText("Wave Bonus : ", levelPos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);
		Utils::RenderText("Total : ", coinsPos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);
		heart_counter.Render();
		coins_counter.Render();
		wave_counter.Render();
		total_counter.Render();
		IwGxLightingOff();

		IwGxFlush();

		/*
		Utils::RenderText(levelText_n, timePos_n, pfont, textColor, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(coinsText_n, starsPos_n, pfont, textColor, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(starsText_n, levelPos_n, pfont, textColor, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(totalText_n, coinsPos_n, pfont, textColor, IW_GX_FONT_ALIGN_LEFT);
		*/
		
		if(bRenderStars)
		{
			if(star_timer.GetIsStop() && current_stars < num_stars)
			{
				star_timer.Start();
			}

			if(star_timer.GetIsEnd() && current_stars < num_stars)
			{
				//FX_Star_Gained_02->Play();
				Utils::PlayFXSound(FX_Sound_STARGAINED02);
				current_stars++;
				if(current_stars < num_stars)
				{
					star_timer.Reset();
					star_timer.Start();
				}
			}

			if(current_stars != 0 && num_stars != 0)
			{
				if(current_stars > 3)
					current_stars = 3;
				for(int i = 0; i < current_stars; i++)
				{
					Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_star_big.atlasName), star_position[num_stars - 1][i], label_star_big);
				}
			}
		}

		if(!bBlockContinueButton)
			btnContinue->Render();
	}
	else
	{
		background->Render();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(VictoryRenderData.atlasName), VictoryPosition, VictoryRenderData);
		if(time_victory_picture.GetIsStop())
		{
			time_victory_picture.Reset();
			time_victory_picture.Start();
		}
	}
}

void EndingWin::InitObject()
{
	//DEBUG
	//SetCoinsBonus(200 * 10);
	//SetLifes(8 * 1000);
	//SetLifes(8);
	//SetStarsBonus(20 *100); //Aceleracion de las oleadas. * 100
	//DEBUG
	bShowTutorialLevel1 = false;
	bBlockContinueButton = true;
	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_win, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_win, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_win, NULL);
	
	btnContinue->IsFocused(true);

	if(bUseOldAudioSystem)
	{
		s3eAudioStop();
		if(!SOUND_MUSIC_MUTE)
		{
			if(bIsAudioPaused)
			{
				LastMusicStacked = Music_Win;
			}
			else
			{
				s3eAudioPlay("bgm/sd_fanfare_win_theme.mp3", 1);
				LastMusicStacked = Music_None;
			}
		}
	}
	else
	{
		Utils::PlayAudio(Music_Win);
	}

	bShowVictoryPicture = true;
	time_victory_picture.Reset();
	time_victory_picture.Stop();
	time_victory_picture.SetTimeLimit(8000);

	
	levelBonus = lifes * 1000;
	current_coins_value = 0;
	current_level_value = 0;
	current_stars_value = 0;

	stars_speed = starsBonus / 100;
	level_speed = levelBonus / 140;
	coins_speed = coinsBonus / 180;

	starsBonus *= 10;

	if(!bExtraMapMode)
	{
		if(MapStatus_State[currentMap - 1] == CURRENT_PLAYABLE)
		{
			MapStatus_State[currentMap - 1] = PLAYABLE;
			MapStatus_State[currentMap] = CURRENT_PLAYABLE;
		}

		if(lifes == 4)
		{
			if(Stars_By_Level[currentMap - 1] < 3)
				Stars_By_Level[currentMap - 1] = 3;
			num_stars = 3;
		}
		else if(lifes == 3)
		{
			if(Stars_By_Level[currentMap - 1] < 2)
				Stars_By_Level[currentMap - 1] = 2;
			num_stars = 2;
		}
		else if(lifes == 2)
		{
			if(Stars_By_Level[currentMap - 1] < 1)
				Stars_By_Level[currentMap - 1] = 1;
			num_stars = 1;
		}
		else if(lifes == 1)
		{
			num_stars = 0;
		}
	}
	else
	{
		if(lifes == 4)
		{
			if(Stars_By_ExtraLevel[currentMap - 1] < 3)
				Stars_By_ExtraLevel[currentMap - 1] = 3;
			num_stars = 3;
		}
		else if(lifes == 3)
		{
			if(Stars_By_ExtraLevel[currentMap - 1] < 2)
				Stars_By_ExtraLevel[currentMap - 1] = 2;
			num_stars = 2;
		}
		else if(lifes == 2)
		{
			if(Stars_By_ExtraLevel[currentMap - 1] < 1)
				Stars_By_ExtraLevel[currentMap - 1] = 1;
			num_stars = 1;
		}
		else if(lifes == 1)
		{
			num_stars = 0;
		}
	}

	current_stars = 0;
	bRenderStars = false;

	star_timer.SetTimeLimit(100);
	star_timer.Reset();
	star_timer.Stop();

	heart_counter.SetValue(levelBonus);
	coins_counter.SetValue(coinsBonus);
	wave_counter.SetValue(starsBonus);
	total_counter.SetValue(coinsBonus + levelBonus + starsBonus);

	IwGetSoundManager()->StopAll();
}

void EndingWin::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_win);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_win);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_win);

	ParserHelper::SaveGameData();
}

void EndingWin::SetTimeBonus(int score)
{
	this->timeBonus = score;
}

void EndingWin::SetStarsBonus(int score)
{
	this->starsBonus = score;
}

void EndingWin::SetLevelBonus(int score)
{
	this->levelBonus = score;
}

void EndingWin::SetCoinsBonus(int score)
{
	this->coinsBonus = score;	
}

void EndingWin::UpdateInfo()
{
	total = 0 + current_coins_value + current_level_value + current_stars_value;
	sprintf(timeText_n, "%d", 0);
	sprintf(starsText_n, "%d", current_stars_value);
	sprintf(levelText_n, "%d", current_level_value);
	sprintf(coinsText_n, "%d", current_coins_value);
	sprintf(totalText_n, "%d", total);
	if(total == coinsBonus + levelBonus + starsBonus)
	{
		if(num_stars != 0)
			bRenderStars = true;
		//bBlockContinueButton = false;
	}
}

void EndingWin::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bShowVictoryPicture)
		return;

	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	if(!bBlockContinueButton)
		btnContinue->HandleClick(event);
}

void EndingWin::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bShowVictoryPicture)
		return;

	if(!bBlockContinueButton)
		btnContinue->HandleMotion(event);
}

void EndingWin::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(bShowVictoryPicture)
		return;

	if(event->m_Pressed) bMouseBlocked = true;
	else bMouseBlocked = false;

	if(event->m_Pressed)
	{
		lastKeyPressed = event->m_Key;
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter || lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack)
		{
			if(!bBlockContinueButton)
				btnContinue->SetStatus(3);
		}
		else
		{
			if(!bBlockContinueButton)
				btnContinue->SetStatus(1);
		}
	} 
	else
	{
		
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter || lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack)
		{
			if(!bBlockContinueButton)
				btnContinue->Execute();
		}
		
		lastKeyPressed = s3eKeyFirst;
	}
}