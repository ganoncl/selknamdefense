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

#include "endingloose.h"

EndingLoose *looseRef = NULL;

int32 KeyboardHandler_loose(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(!looseRef->bKeyboardBlocked)
		looseRef->UpdateKeyboardEvents(event);
	return 0;
}

//Mouse Motion
void HandleMotion_loose(s3ePointerMotionEvent* event) 
{
	if(looseRef == NULL)
		return;
	
	if(!looseRef->bMouseBlocked)
		looseRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_loose(s3ePointerEvent *event) 
{
	if(looseRef == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
			{
				looseRef->MouseHack();
			}
			else
			{
				looseRef->ActiveButtonMouseHack();
			}
		}

		return;
	}

	if(!looseRef->bMouseBlocked)
		looseRef->UpdateClickEvents(event);
}

void Loose_OnClick_PlayAgain()
{
	gameState = GS_Retry;
	bPause = false;
}

void Loose_OnClick_MapSelect()
{
	if(!bExtraMapMode)
		gameState = GS_MapSelection;
	else
		gameState = GS_ExtraMapSelection;
	inGameState = GSG_None;
}

void Loose_OnClick_QuitGame()
{
	popUpState = GSP_QuitGameLooseEnding;
}

void Loose_OnMotion_PlayAgain()
{
	looseRef->ChangeFocus(Loose_Focus_PlayAgain);
}

void Loose_OnMotion_MapSelect()
{
	looseRef->ChangeFocus(Loose_Focus_MapSelect);
}

void Loose_OnMotion_QuitGame()
{
	looseRef->ChangeFocus(Loose_Focus_QuitGame);
}

EndingLoose::EndingLoose()
{
	looseRef = this;

	//Background color
	cols[0].Set(0xbfb77b49);   
	cols[1].Set(0xbfb77b49);
	cols[2].Set(0xbfb77b49);
	cols[3].Set(0xbfb77b49);

	//Set font
	pfont = font_komikadisplaybold25;
	textColor = 0xffffffff;
	titlePos = CIwRect(0, 100, 1280, 30);
	title = "DEFEAT";

	//Position and size
	_x = 140;
	_y = 60;
	_width = 1000;
	_height = 600;
	pos[0] = CIwSVec2(_x, _y);
	pos[1] = CIwSVec2(_x, _y + _height);
	pos[2] = CIwSVec2(_x + _width, _y + _height);
	pos[3] = CIwSVec2(_x + _width, _y);

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	int btnTexWidth = buttonRenderData.state[0].RollOut.width;

	int buttonX = (int)((IwGxGetScreenWidth() - btnTexWidth) * 0.5f);

	btnPlayAgain = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 500, 200);
	btnPlayAgain->OnClick(&Loose_OnClick_PlayAgain);
	btnPlayAgain->OnRollOn(&Loose_OnMotion_PlayAgain);
	btnMapSelect = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 500, 300);
	btnMapSelect->OnClick(&Loose_OnClick_MapSelect);
	btnMapSelect->OnRollOn(&Loose_OnMotion_MapSelect);
	btnQuitGame = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 500, 400);
	btnQuitGame->OnRollOn(&Loose_OnMotion_QuitGame);
	btnQuitGame->OnClick(&Loose_OnClick_QuitGame);

	bIsActiveObj = true;
	bMouseBlocked = false;
	bKeyboardBlocked = false;

	defeatRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_defeat"));
	defeatDimension  = TextureHelper::GetDimension(defeatRenderData);

	defeat_position = CIwSVec2((int)((IwGxGetScreenWidth() - defeatDimension.x) * 0.5f),
		(int)((IwGxGetScreenHeight() - defeatDimension.y) * 0.5f));

	background = new Background(0,0,0,170);

	btnPlayAgain->SetText("PLAY AGAIN");
	btnMapSelect->SetText("MAP SELECT");
	btnQuitGame->SetText("QUIT GAME");
}

EndingLoose::~EndingLoose()
{
	delete btnPlayAgain;
	delete btnMapSelect;
	delete btnQuitGame;
	delete background;
}

void EndingLoose::Update()
{
	if(popUpState != GSP_None)
	{
		bMouseBlocked = true;
		bKeyboardBlocked = true;
	}
	else
	{
		bMouseBlocked = false;
		bKeyboardBlocked = false;
	}

	time_defeat_picture.Update();

	if(time_defeat_picture.GetIsEnd())
	{
		bShowDefeatPicture = false;
		//s3eAudioStop();
	}
}

void EndingLoose::Render()
{
	if(!bShowDefeatPicture)
	{
		background->Render();

		//Text
		IwGxLightingOn();
		Utils::RenderText(title, CIwRect(titlePos.x - 4, titlePos.y + 4, titlePos.w, titlePos.h), font_komikadisplaybold48, 0xff000000);
		Utils::RenderText(title, titlePos, font_komikadisplaybold48, 0xff00e0f7);
		IwGxLightingOff();

		btnPlayAgain->Render();
		btnMapSelect->Render();
		btnQuitGame->Render();
	}
	else
	{
		background->Render();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(defeatRenderData.atlasName), defeat_position, defeatRenderData);
		if(time_defeat_picture.GetIsStop())
		{
			time_defeat_picture.Reset();
			time_defeat_picture.Start();
		}
	}
}

void EndingLoose::InitObject()
{
	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_loose, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_loose, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_loose, NULL);
	ChangeFocus(Loose_Focus_PlayAgain);

	if(bUseOldAudioSystem)
	{
		s3eAudioStop();

		if(!SOUND_MUSIC_MUTE)
		{
			if(bIsAudioPaused)
			{
				LastMusicStacked = Music_Loose;
			}
			else
			{
				s3eAudioPlay("bgm/sd_fanfare_lose_theme.mp3", 1);
				LastMusicStacked = Music_None;
			}
		}
	}
	else
	{
		Utils::PlayAudio(Music_Loose);
	}
	
	time_defeat_picture.SetTimeLimit(8000);
	time_defeat_picture.Reset();
	bShowDefeatPicture = true;

	IwGetSoundManager()->StopAll();
}

void EndingLoose::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_loose);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_loose);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_loose);
	bShowDefeatPicture = true;
}

void EndingLoose::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bShowDefeatPicture) return;

	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	btnPlayAgain->HandleClick(event);
	btnMapSelect->HandleClick(event);
	btnQuitGame->HandleClick(event);
}

void EndingLoose::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bShowDefeatPicture) return;

	btnPlayAgain->HandleMotion(event);
	btnMapSelect->HandleMotion(event);
	btnQuitGame->HandleMotion(event);
}

void EndingLoose::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(bShowDefeatPicture) return;

	if(event->m_Pressed) bMouseBlocked = true;
	else bMouseBlocked = false;

	if(event->m_Pressed)
	{
		lastKeyPressed = event->m_Key;

		int tmpFocus = (int)focus;

		if(lastKeyPressed == s3eKeyAbsUp)
		{
			if(tmpFocus - 1 > 0) ChangeFocus((Loose_Focus)(tmpFocus - 1));
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			if(tmpFocus + 1 <= 3) ChangeFocus((Loose_Focus)(tmpFocus + 1));
		} 
		else if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter)
		{
			if(focus == Loose_Focus_PlayAgain) btnPlayAgain->SetStatus(3);
			else if(focus == Loose_Focus_MapSelect) btnMapSelect->SetStatus(3);
			else if(focus == Loose_Focus_QuitGame) btnQuitGame->SetStatus(3);
		}
		else if(lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack) 
		{
			ChangeFocus(Loose_Focus_QuitGame);
		}
		else
		{
			if(focus == Loose_Focus_PlayAgain) btnPlayAgain->SetStatus(1);
			else if(focus == Loose_Focus_MapSelect) btnMapSelect->SetStatus(1);
			else if(focus == Loose_Focus_QuitGame) btnQuitGame->SetStatus(1);
		}
	}
	else
	{
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter)
		{
			if(focus == Loose_Focus_PlayAgain) btnPlayAgain->Execute();
			else if(focus == Loose_Focus_MapSelect) btnMapSelect->Execute();
			else if(focus == Loose_Focus_QuitGame) 
			{
				btnQuitGame->Execute();
				btnQuitGame->IsFocused(true);
				bShowDefeatPicture = true;
			}
		}
		else if(lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack)
		{
			btnQuitGame->Execute();
			btnQuitGame->IsFocused(true);
			bShowDefeatPicture = true;
		}
		lastKeyPressed = s3eKeyFirst;
	}
}

void EndingLoose::ChangeFocus(Loose_Focus newFocus)
{
	if(focus == newFocus)
		return;

	if(focus == Loose_Focus_PlayAgain) btnPlayAgain->IsFocused(false);
	else if(focus == Loose_Focus_MapSelect) btnMapSelect->IsFocused(false);
	else if(focus == Loose_Focus_QuitGame) btnQuitGame->IsFocused(false);

	if(newFocus == Loose_Focus_PlayAgain) btnPlayAgain->IsFocused(true);
	else if(newFocus == Loose_Focus_MapSelect) btnMapSelect->IsFocused(true);
	else if(newFocus == Loose_Focus_QuitGame) btnQuitGame->IsFocused(true);

	focus = newFocus;
}


void EndingLoose::MouseHack()
{
	if(focus == Loose_Focus_PlayAgain) btnPlayAgain->Execute();
	else if(focus == Loose_Focus_MapSelect) btnMapSelect->Execute();
	else if(focus == Loose_Focus_QuitGame) 
	{
		btnQuitGame->Execute();
		btnQuitGame->IsFocused(true);
		bShowDefeatPicture = true;
	}
}

void EndingLoose::ActiveButtonMouseHack()
{
	if(focus == Loose_Focus_PlayAgain) btnPlayAgain->SetStatus(3);
	else if(focus == Loose_Focus_MapSelect) btnMapSelect->SetStatus(3);
	else if(focus == Loose_Focus_QuitGame) btnQuitGame->SetStatus(3);
}