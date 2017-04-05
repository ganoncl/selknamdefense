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


#include "mmig_menu.h"
#include "common.h"
#include "utils.h"

//MOUSE && KEYBOARD HANDLE 
#define NUM_FOCUS_MENUIG_ELEMENTS  4

MenuIG *menuigRef = NULL;

int32 KeyboardHandler_MIG(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(!menuigRef->bKeyboardBlocked)
		menuigRef->UpdateKeyboardEvents(event);
	return 0;
}

//Mouse Motion
void HandleMotion_MIG(s3ePointerMotionEvent* event) 
{
	if(menuigRef == NULL)
		return;
	
	if(!menuigRef->bMouseBlocked)
		menuigRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_MIG(s3ePointerEvent *event) 
{
	if(menuigRef == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE && popUpState == GSP_None)
		{
			if(!event->m_Pressed)
			{
				menuigRef->MouseHack();
			}
			else
			{
				menuigRef->ActiveButtonMouseHack();
			}
		}

		return;
	}

	if(!menuigRef->bMouseBlocked)
		menuigRef->UpdateClickEvents(event);
}

//Buttons functions
void MIG_OnMotion_Resume() 
{
	menuigRef->ChangeFocus(Focus_MIG_Resume);
}

void MIG_OnClick_Resume()
{
	IwTrace(POINTER, ("Resume"));
	inGamePausedState = GSGP_None;
	mainMenuIGState = GSMIG_None;
}

void MIG_OnMotion_HowToPlay()
{
	//menuigRef->ChangeFocus(Focus_MIG_HowToPlay);
}

void MIG_OnClick_HowToPlay()
{
	bPause = true;
	inGamePausedState = GSGP_HowToPlayIG;
}

void MIG_OnMotion_Retry()
{
	menuigRef->ChangeFocus(Focus_MIG_Retry);
}

void MIG_OnClick_Retry()
{
	popUpState = GSP_Retry;
}

void MIG_OnMotion_Options()
{
	menuigRef->ChangeFocus(Focus_MIG_Options);
}

void MIG_OnClick_Options()
{
	//mainMenuIGState = GSMIG_Options;
	//gameState = GS_MapSelection;
	popUpState = GSP_MapSelection;
}

void MIG_OnMotion_QuitGame()
{
	menuigRef->ChangeFocus(Focus_MIG_QuitGame);
}

void MIG_OnClick_QuitGame()
{
	popUpState = GSP_ReturnToMainMenu;
}


MenuIG::MenuIG()
{
	menuigRef = this;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	buttonWidth = TextureHelper::GetDimension(buttonRenderData.state[0].RollOut).x;
	buttonHeight = TextureHelper::GetDimension(buttonRenderData.state[0].RollOut).y;

	pfont = font_komikadisplaybold25;

	//Settings
	titlePos = CIwRect(500, 60, 150, 30);
	color = 0xffffffff;
	//Menu position
	int btnX =  (int)((IwGxGetScreenWidth() - buttonWidth) * 0.5f);
	int btnY = 200;
	//y distance between two consecutive buttons
	int margin = 10;
	int btnH = (int)(buttonHeight);
	//End Settings

	//Set buttons
	resume = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 0);
	resume->OnClick(&MIG_OnClick_Resume);
	resume->OnRollOn(&MIG_OnMotion_Resume);
	resume->SetStyle(NULL, 60, 5);
	resume->IsFocused(true);
	Focus = Focus_MIG_Resume;

	howtoplay = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 1);
	howtoplay->OnRollOn(&MIG_OnMotion_HowToPlay);
	howtoplay->OnClick(&MIG_OnClick_HowToPlay);
	howtoplay->SetStyle(NULL, 60, 5);

	retry = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 1);
	retry->OnRollOn(&MIG_OnMotion_Retry);
	retry->OnClick(&MIG_OnClick_Retry);
	retry->SetStyle(NULL, 60, 5);

	options = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 2);
	options->OnRollOn(&MIG_OnMotion_Options);
	options->OnClick(&MIG_OnClick_Options);
	options->SetStyle(NULL, 60, 5);
	//options->SetStyle(font_komikadisplaybold18, 60, 5);

	quitgame = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 3);
	quitgame->OnRollOn(&MIG_OnMotion_QuitGame);
	quitgame->OnClick(&MIG_OnClick_QuitGame);
	quitgame->SetStyle(NULL, 60, 5);

	lastKeyPressed = s3eKeyFirst; //InvalidKey
	bMouseBlocked = false;
	bKeyboardBlocked = false;

	background = new Background(0, 0, 0, fade_menu - 50);

	//Text
	title = "PAUSE";
	resume->SetText("RESUME");
	howtoplay->SetText("HOW TO PLAY");
	retry->SetText("RETRY");
	options->SetText("WORLD MAP");
	quitgame->SetText("QUIT GAME");
}

MenuIG::~MenuIG()
{
	delete resume;
	delete howtoplay;
	delete retry;
	delete options;
	delete quitgame;
	delete background;
}

void MenuIG::InitObject()
{
	bIsActiveObj = true;
	ChangeFocus(Focus_MIG_Resume);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_MIG, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_MIG, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_MIG, NULL);
	bMouseBlocked = false;
	bKeyboardBlocked = false;
}

void MenuIG::EndObject()
{
	ChangeFocus(Focus_MIG_None);

	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_MIG);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_MIG);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_MIG);
	bIsActiveObj = false;
}

void MenuIG::Render()
{
	background->Render();

	//text
	IwGxLightingOn();
	Utils::RenderText(title, CIwRect(0, 0, 1280, 220), font_komikadisplaybold48, 0xff0da8d9, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true, 2);
	IwGxLightingOff();

	resume->Render(); 
	//howtoplay->Render();
	retry->Render();
	options->Render();
	quitgame->Render();
}

void MenuIG::UpdateClickEvents(s3ePointerEvent *event) 
{
	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	resume->HandleClick(event);
	//howtoplay->HandleClick(event);
	retry->HandleClick(event);
	options->HandleClick(event);
	quitgame->HandleClick(event);
}

void MenuIG::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	resume->HandleMotion(event);
	//howtoplay->HandleMotion(event);
	retry->HandleMotion(event);
	options->HandleMotion(event);
	quitgame->HandleMotion(event);
}

bool MenuIG::ChangeFocus(Focus_MIG newFocus)
{
	if(newFocus == Focus)
		return false;

	if(newFocus == Focus_MIG_Resume) 
	{
		if(!resume->IsActive())
			return false;
		resume->IsFocused(true);
	}
	/*
	else if(newFocus == Focus_MIG_HowToPlay)
	{
		if(!howtoplay->IsActive())
			return false;
		howtoplay->IsFocused(true);
	}
	*/
	else if(newFocus == Focus_MIG_Retry)
	{
		if(!retry->IsActive())
			return false;
		retry->IsFocused(true);
	}
	else if(newFocus == Focus_MIG_Options) 
	{
		if(!options->IsActive())
			return false;
		options->IsFocused(true);
	}
	else if(newFocus == Focus_MIG_QuitGame) 
	{
		if(!quitgame->IsActive())
			return false;
		quitgame->IsFocused(true);
	}

	if(Focus == Focus_MIG_Resume) resume->IsFocused(false);
	//else if(Focus == Focus_MIG_HowToPlay) howtoplay->IsFocused(false);
	else if(Focus == Focus_MIG_Retry) retry->IsFocused(false);
	else if(Focus == Focus_MIG_Options) options->IsFocused(false);
	else if(Focus == Focus_MIG_QuitGame) quitgame->IsFocused(false);

	Focus = newFocus;
	return true;
}

void MenuIG::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(event->m_Pressed) 
	{
		lastKeyPressed = event->m_Key;
		bMouseBlocked = true;
	
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter) 
		{
			if(Focus == Focus_MIG_Resume) resume->SetStatus(3);
			//else if(Focus == Focus_MIG_HowToPlay) howtoplay->SetStatus(3);
			else if(Focus == Focus_MIG_Retry) retry->SetStatus(3);
			else if(Focus == Focus_MIG_Options) options->SetStatus(3);
			else if(Focus == Focus_MIG_QuitGame) quitgame->SetStatus(3);
		}	
		else if(lastKeyPressed == s3eKeyAbsUp)
		{
			//Search for a non-inactive button in order to be focused.
			int currState = (int)Focus;
			for(int i = currState - 1; i >= 1; i--) 
			{
				Focus_MIG newFocus = (Focus_MIG)(i);
				if(ChangeFocus(newFocus))
					break;
			}
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			//Search for a non-inactive button in order to be focused.
			int currState = (int)Focus;
			for(int i = currState + 1; i < NUM_FOCUS_MENUIG_ELEMENTS + 1; i++)
			{
				Focus_MIG newFocus = (Focus_MIG)(i);
				if(ChangeFocus(newFocus))
					break;
			}
		}
		else if(lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack) 
		{
			inGamePausedState = GSGP_None;
		}
		else
		{
			if(Focus == Focus_MIG_Resume) resume->SetStatus(1);
			//else if(Focus == Focus_MIG_HowToPlay) howtoplay->SetStatus(1);
			else if(Focus == Focus_MIG_Retry) retry->SetStatus(1);
			else if(Focus == Focus_MIG_Options) options->SetStatus(1);
			else if(Focus == Focus_MIG_QuitGame) quitgame->SetStatus(1);
		}
	}
	else
	{
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter)
		{
			if(Focus == Focus_MIG_Resume) resume->Execute();
			//else if(Focus == Focus_MIG_HowToPlay) howtoplay->Execute();
			else if(Focus == Focus_MIG_Retry) retry->Execute();
			else if(Focus == Focus_MIG_Options) options->Execute();
			else if(Focus == Focus_MIG_QuitGame) 
			{
				quitgame->Execute();
				quitgame->SetStatus(1);
			}
		}
		bMouseBlocked = false;
		lastKeyPressed = s3eKeyFirst;
	}
}

void MenuIG::Update()
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
}

void MenuIG::MouseHack()
{
	if(Focus == Focus_MIG_Resume) resume->Execute();
	//else if(Focus == Focus_MIG_HowToPlay) howtoplay->Execute();
	else if(Focus == Focus_MIG_Retry) retry->Execute();
	else if(Focus == Focus_MIG_Options) options->Execute();
	else if(Focus == Focus_MIG_QuitGame) 
	{
		quitgame->Execute();
		quitgame->SetStatus(1);
	}
}

void MenuIG::ActiveButtonMouseHack()
{
	if(Focus == Focus_MIG_Resume) resume->SetStatus(3);
	//else if(Focus == Focus_MIG_HowToPlay) howtoplay->SetStatus(3);
	else if(Focus == Focus_MIG_Retry) retry->SetStatus(3);
	else if(Focus == Focus_MIG_Options) options->SetStatus(3);
	else if(Focus == Focus_MIG_QuitGame) quitgame->SetStatus(3);
}