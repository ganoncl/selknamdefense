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

#include "IwGx.h"
#include "IwGxFont.h"
#include "s3e.h"
#include "common.h"
#include "button.h"
#include "mm_menu.h"
#include "parserhelper.h"

//MOUSE && KEYBOARD HANDLE 
#define NUM_FOCUS_ELEMENTS  4

Menu *mainRef = NULL;

int32 KeyboardHandler(void* sys, void*)
{
	if(mainRef != NULL && mainRef->bBlockDeviceHandler)
	{
		return -1;
	}

	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(!mainRef->bKeyboardBlocked)
		mainRef->UpdateKeyboardEvents(event);
	return 0;
}

//Mouse Motion
void HandleMotion(s3ePointerMotionEvent* event) 
{
	if(mainRef == NULL)
		return;

	if(mainRef != NULL && mainRef->bBlockDeviceHandler)
	{
		return;
	}
	

	if(!mainRef->bMouseBlocked)
		mainRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick(s3ePointerEvent *event) 
{
	if(mainRef == NULL)
		return;

	if(mainRef != NULL && mainRef->bBlockDeviceHandler)
	{
		return;
	}

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
				mainRef->MouseHack();
			else
				mainRef->ActiveButtonHack();
		}

		return;
	}

	if(!mainRef->bMouseBlocked)
		mainRef->UpdateClickEvents(event);
}

//Buttons functions
void loadGameRollOn() 
{
	mainRef->ChangeFocus(FOCUS_LOADGAME);
}

void loadGameFunc()
{
	gameState = GS_MapSelection;
}

void newGameRollOn()
{
	mainRef->ChangeFocus(FOCUS_NEWGAME);
}

void newGameAction()
{
	if(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed)
		popUpState = GSP_NewGame;
	else
	{
		gameState = GS_MapSelection;
		NewGame_State = NOT_EMPTY_GAME;
		//ParserHelper::SaveGameData();
	}
}

void howtoplayRollOn()
{
	//mainRef->ChangeFocus(FOCUS_HOWTOPLAY);
}

void howtoplayFunc()
{
	//mainMenuState = GSM_HowToPlay;
	gameState = GS_ExtraMapSelection;
}

/*
void optionsRollOn()
{
	mainRef->ChangeFocus(FOCUS_OPTIONS);
}

void optionsAction()
{
	mainMenuState = GSM_Options;
}
*/

void creditsRollOn()
{
	mainRef->ChangeFocus(FOCUS_CREDITS);
}

void creditsAction()
{
	mainMenuState = GSM_Credits;
}

void quitgameAction()
{
	//popUpState = GSP_QuitGame;
	ParserHelper::SaveGameData();
	s3eDeviceRequestQuit();
}

void quitgameRollOn()
{
	mainRef->ChangeFocus(FOCUS_QUITGAME);
}


Menu::Menu()
{
	mainRef = this;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	int texWidth  = TextureHelper::GetDimension(buttonRenderData.state[0].RollOn).x;
	int texHeight = TextureHelper::GetDimension(buttonRenderData.state[0].RollOn).y;

	//Title
	menuTitle = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_title"));
	CIwSVec2 menuTitleDimension = TextureHelper::GetDimension(menuTitle);
	int xTitle = (int)((IwGxGetScreenWidth() - menuTitleDimension.x) * 0.5f);
	int yTitle = 30;
	titlePosition = CIwSVec2(xTitle, yTitle);

	////Steam
	//steamPromoData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("steam_promo"));
	//steamPromoPos = CIwSVec2(15, 604);

	//More Games
	moreGamesData= TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("more_games_qr"));
	moreGamesPos = CIwSVec2(10, 560);
	
	//Settings
	//Menu position
	btnX =  (int)((IwGxGetScreenWidth() - texWidth) * 0.5f);
	btnY = 200;
	//y distance between two consecutive buttons
	margin = 10;
	btnH = (int)(texHeight);
	//End Settings

	//Set buttons
	loadgame =  new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 0);
	loadgame->OnClick(&loadGameFunc);
	loadgame->OnRollOn(&loadGameRollOn);
	//loadgame->SetStyle(font_komikadisplaybold18, 10, 0);
	//loadgame->SetStyle(NULL, 60, 5);
	loadgame->IsFocused(true);
	//Focus = FOCUS_LOADGAME;

	newgame =  new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 1);
	newgame->OnRollOn(&newGameRollOn);
	newgame->OnClick(&newGameAction);
	newgame->SetStyle(NULL, 60, 5);
	//newgame->Enabled(false);

	howtoplay =  new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 2);
	howtoplay->OnRollOn(&howtoplayRollOn);
	howtoplay->OnClick(&howtoplayFunc);
	howtoplay->SetStyle(NULL, 60, 5);
	/*
	options = new Button(tex, btnX, btnY + (btnH + margin) * 3);
	options->OnRollOn(&optionsRollOn);
	options->OnClick(&optionsAction);
	options->SetStyle(NULL, 60, 5);
	*/
	credits =  new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 2);
	credits->OnRollOn(&creditsRollOn);
	credits->OnClick(&creditsAction);
	credits->SetStyle(NULL, 60, 5);
	//credits->Enabled(false);

	quitgame = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 3);
	quitgame->OnRollOn(&quitgameRollOn);
	quitgame->OnClick(&quitgameAction);
	quitgame->SetStyle(NULL, 60, 5);
	
	SetMenuPositionWithResumeButton(NewGame_State == NOT_EMPTY_GAME);
	if(NewGame_State == NOT_EMPTY_GAME)
		Focus = FOCUS_LOADGAME;
	else
		Focus = FOCUS_NEWGAME;

	bIsOn = false;

	lastKeyPressed = s3eKeyFirst; //InvalidKey
	bMouseBlocked = false;
	bKeyboardBlocked = false;

	Timer_BlockAfterFadeOut.SetTimeLimit(150);
	bStartTimer = true;
	bDoJustOnceInWhileGame = true;
	bBlockDeviceHandler = true;

	//Text
	loadgame->SetText("PLAY GAME");
	newgame->SetText("NEW GAME");
	howtoplay->SetText("HOW TO PLAY");
	credits->SetText("CREDITS");
	quitgame->SetText("QUIT GAME");

}

Menu::~Menu()
{
	delete loadgame;
	delete newgame;
	delete howtoplay;
	delete credits;
	delete quitgame;
}

void Menu::InitObject()
{
	bIsActiveObj = true;
	bFirstFocus = true;
	if(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed)
		ChangeFocus(FOCUS_LOADGAME);
	else
		ChangeFocus(FOCUS_NEWGAME);

	SetMenuPositionWithResumeButton(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed);

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler, NULL);
	bIsOn = true;
	bMouseBlocked = false;
	bKeyboardBlocked = false;
}

void Menu::EndObject()
{
	ChangeFocus(FOCUS_NONE);

	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler);
	bIsActiveObj = false;
}

void Menu::Render()
{
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(menuTitle.atlasName), titlePosition, menuTitle);
	//Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(steamPromoData.atlasName), steamPromoPos, steamPromoData);
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(moreGamesData.atlasName), moreGamesPos, moreGamesData);


	IwGxFlush();

	if(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed)
		loadgame->Render(); 
	newgame->Render();
	credits->Render();
	quitgame->Render();

	IwGxFlush();
}

void Menu::UpdateClickEvents(s3ePointerEvent *event) 
{
	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	if(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed) loadgame->HandleClick(event);
	newgame->HandleClick(event);
	credits->HandleClick(event);
	quitgame->HandleClick(event);
}

void Menu::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed) loadgame->HandleMotion(event);
	newgame->HandleMotion(event);
	credits->HandleMotion(event);
	quitgame->HandleMotion(event);
}

bool Menu::ChangeFocus(FocusState newFocus)
{
	if(newFocus == Focus)
		return false;

	if(newFocus == FOCUS_LOADGAME) 
	{
		if(!loadgame->IsActive())
			return false;
		loadgame->IsFocused(true, !bFirstFocus);
	}
	else if(newFocus == FOCUS_NEWGAME)
	{
		if(!newgame->IsActive())
			return false;
		newgame->IsFocused(true, !bFirstFocus);
	}
	else if(newFocus == FOCUS_CREDITS) 
	{
		if(!credits->IsActive())
			return false;
		credits->IsFocused(true, !bFirstFocus);
	}
	else if(newFocus == FOCUS_QUITGAME) 
	{
		if(!quitgame->IsActive())
			return false;
		quitgame->IsFocused(true, !bFirstFocus);
	}

	if(Focus == FOCUS_LOADGAME) loadgame->IsFocused(false);
	else if(Focus == FOCUS_NEWGAME) newgame->IsFocused(false);
	else if(Focus == FOCUS_CREDITS) credits->IsFocused(false);
	else if(Focus == FOCUS_QUITGAME) quitgame->IsFocused(false);

	Focus = newFocus;
	bFirstFocus = false;
	return true;
}

void Menu::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(event->m_Pressed) 
	{
		lastKeyPressed = event->m_Key;
		bMouseBlocked = true;
	
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter) 
		{
			if(Focus == FOCUS_LOADGAME) loadgame->SetStatus(3);
			else if(Focus == FOCUS_NEWGAME) newgame->SetStatus(3);
			else if(Focus == FOCUS_CREDITS) credits->SetStatus(3);
			else if(Focus == FOCUS_QUITGAME) quitgame->SetStatus(3);
		}	
		else if(lastKeyPressed == s3eKeyAbsUp)
		{
			//Search for a non-inactive button in order to be focused.
			int currState = (int)mainRef->Focus;
			for(int i = currState - 1; i >= 1; i--) 
			{
				FocusState newFocus = (FocusState)(i);
				
				if(newFocus == FOCUS_LOADGAME && !(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed))
					continue;

				if(ChangeFocus(newFocus))
					break;
			}
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			//Search for a non-inactive button in order to be focused.
			int currState = (int)mainRef->Focus;
			for(int i = currState + 1; i < NUM_FOCUS_ELEMENTS + 1; i++)
			{
				FocusState newFocus = (FocusState)(i);
				
				if(newFocus == FOCUS_LOADGAME && !(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed))
					continue;

				if(ChangeFocus(newFocus))
					break;
			}
		}
		else if(lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack) 
		{
			ChangeFocus(FOCUS_QUITGAME);
			quitgame->SetStatus(3);
		}
		else
		{
			if(Focus == FOCUS_LOADGAME) loadgame->SetStatus(1);
			else if(Focus == FOCUS_NEWGAME) newgame->SetStatus(1);
			else if(Focus == FOCUS_CREDITS) credits->SetStatus(1);
		}
	}
	else
	{
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter)
		{
			if(Focus == FOCUS_LOADGAME) loadgame->Execute();
			else if(Focus == FOCUS_NEWGAME) newgame->Execute();
			else if(Focus == FOCUS_CREDITS) credits->Execute();
			else if(Focus == FOCUS_QUITGAME) 
			{
				quitgameAction();
			}
		}
		else if(lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack) 
		{
			quitgameAction();
		}
		bMouseBlocked = false;
		lastKeyPressed = s3eKeyFirst;
	}
}

void Menu::Off()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler);
	bIsOn = false;
}

void Menu::On()
{
	bStartTimer = true;

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler, NULL);
	bIsOn = true;
	
	if(Focus == FOCUS_LOADGAME) loadgame->SetStatus(1);
	else if(Focus == FOCUS_NEWGAME) newgame->SetStatus(1);
	else if(Focus == FOCUS_CREDITS) credits->SetStatus(1);
	else if(Focus == FOCUS_QUITGAME) quitgame->SetStatus(1);

	bMouseBlocked = false;
	bKeyboardBlocked = false;
	ChangeFocus(FOCUS_NONE);
	if(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed)
		ChangeFocus(FOCUS_LOADGAME);
	else
		ChangeFocus(FOCUS_NEWGAME);

	SetMenuPositionWithResumeButton(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed);
}

void Menu::SetMenuPositionWithResumeButton(bool bWithResume)
{
	int offset = 0;
	int offset_height = 0;
	if(!bWithResume)
	{
		offset = -1;
		offset_height = 50;
	}

	if(bWithResume) 
		loadgame->SetPos(btnX, btnY + (btnH + margin) * 0 + offset + offset_height);

	newgame->SetPos(btnX, btnY + (btnH + margin)   * (1 + offset) + offset_height);
	credits->SetPos(btnX, btnY + (btnH + margin)   * (2 + offset) + offset_height);
	quitgame->SetPos(btnX, btnY + (btnH + margin)  * (3 + offset) + offset_height);
}

void Menu::MouseHack()
{
	if(Focus == FOCUS_LOADGAME) loadgame->Execute();
	else if(Focus == FOCUS_NEWGAME)	newgame->Execute();
	else if(Focus == FOCUS_CREDITS)	credits->Execute();
	else if(Focus == FOCUS_QUITGAME) quitgameAction();
}

void Menu::ActiveButtonHack()
{
	if(Focus == FOCUS_LOADGAME) loadgame->SetStatus(3);
	else if(Focus == FOCUS_NEWGAME) newgame->SetStatus(3);
	else if(Focus == FOCUS_CREDITS) credits->SetStatus(3);
	else if(Focus == FOCUS_QUITGAME) quitgame->SetStatus(3);
}

void Menu::Update()
{
	//IwTrace(TEXHELP, ("Menu Update"));
	if(bDoJustOnceInWhileGame)
	{
		//IwTrace(TEXHELP, ("bDoJustOnceInWhileGame"));

		Timer_BlockAfterFadeOut.Update();

		if(fade->GetAlpha() <= 80 && bStartTimer)
		{
			bStartTimer = false;
			Timer_BlockAfterFadeOut.Reset();
			Timer_BlockAfterFadeOut.Start();
		}

		if(Timer_BlockAfterFadeOut.GetIsEnd())
		{
			bBlockDeviceHandler = false;
			bDoJustOnceInWhileGame = false;
		}
	}
}