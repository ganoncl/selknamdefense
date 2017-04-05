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
#include "mm_popup.h"
#include "parserhelper.h"
#include "texturehelper.h"

//Keyboard && mouse events
//Custom functions
void onCancelAction()
{
	if(popUpState == GSP_ResetStore)
	{
		bPausedStore = false;
	}

	popUpState = GSP_None;
}

void onAcceptAction()
{
	if(popUpState == GSP_NewGame)
	{
		ParserHelper::ReturnDataToDefault();
		NewGame_State = NOT_EMPTY_GAME;
		gameState = GS_MapSelection;
		popUpState = GSP_None;
	}
	else if(popUpState == GSP_NewGame_Error)
	{
		mainMenuState = GSM_MainMenu;
		popUpState = GSP_None;
	} 
	else if(popUpState == GSP_QuitGame)
	{
		if(!bQuittingGame)
		{
			bQuittingGame = true;
			ParserHelper::SaveGameData();
			s3eDeviceRequestQuit();
		}
	}
	else if(popUpState == GSP_Retry)
	{
		gameState = GS_Retry;
		popUpState = GSP_None;
	}
	else if(popUpState == GSP_ReturnToMainMenu)
	{
		gameState = GS_MainMenu;
		popUpState = GSP_None;
	}
	else if(popUpState == GSP_QuitGameLooseEnding)
	{
		gameState = GS_MainMenu;
		popUpState = GSP_None;
	}
	else if(popUpState == GSP_MapSelection)
	{
		if(!bExtraMapMode)
			gameState = GS_MapSelection;
		else 
			gameState = GS_ExtraMapSelection;
		popUpState = GSP_None;
	}
	else if(popUpState == GSP_ResetStore)
	{
		bResetStoreStats = true;
		bPausedStore     = false;
		popUpState       = GSP_None;
	}
}

//Keyboard
int32 KeyboardHandler_pop(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(!popup->bKeyboardBlocked)
		popup->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotion_pop(s3ePointerMotionEvent* event) 
{
	if(popup == NULL)
		return;

	if(!popup->bMouseBlocked)
		popup->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_pop(s3ePointerEvent *event) 
{
	if(popup == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
			{
				popup->MouseHack();
			}
			else
			{
				popup->ActiveButtonMouseHack();
			}
		}

		return;
	}

	if(!popup->bMouseBlocked)
		popup->UpdateClickEvents(event);
}

void OnAcceptMotion()
{
	popup->ChangeFocus(POPUP_FOCUS_ACCEPT);
}

void OnCancelMotion()
{
	popup->ChangeFocus(POPUP_FOCUS_CANCEL);
}

PopUp::PopUp()
{
	panelWidth = 14 * 40;
	panelHeight = 6 * 40;

	TextureData buttonData = TextureHelper::GetTextureNamed("btn_popup");
	button = new ButtonRenderData();
	button->numStates = 1;
	button->state[0].RollOut  = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	button->state[0].RollOn   = TextureHelper::GetRenderData(buttonData, 1, 3, 1);
	button->state[0].Pressed  = TextureHelper::GetRenderData(buttonData, 1, 3, 2);
	button->state[0].Inactive = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	int buttonWidth = button->state[0].Pressed.width;
	int buttonHeight = button->state[0].Pressed.height;

	//Title and text font
	pfont = font_komikadisplaybold25;

	//General Settings
	boxWidth = buttonWidth * 2 + 100;
	boxHeight = (int)buttonHeight + 150;
	boxPosX = (int)((IwGxGetScreenWidth() - boxWidth) * 0.5f);
	boxPosY = (int)((IwGxGetScreenHeight() - boxHeight) * 0.5f);
	btnAcceptPosX = boxPosX + 20;
	btnAcceptPosY = boxPosY + boxHeight - 10 - (int)(buttonHeight);
	btnCancelPosX = boxPosX + 30 + buttonWidth + 40;
	btnCancelPosY = boxPosY + boxHeight - 10 - (int)(buttonHeight);
	titleX = 40;
	titleY = 10;
	titleW = boxWidth;
	titleH = boxHeight;
	textX = 10;
	textY = 50;
	textW = boxWidth;
	textH = boxHeight;

	//Setting semi-transparent black background	
	int xPos = IwGxGetScreenWidth();
	int yPos = IwGxGetScreenHeight();
	xy3[0].x = 0,	      xy3[0].y = 0;
	xy3[1].x = 0,		  xy3[1].y = yPos;
	xy3[2].x = xPos,      xy3[2].y = yPos;
	xy3[3].x = xPos,      xy3[3].y = 0;
	
	alpha = 200; //transparency degree

	//Container box settings
	boxPos[0].x = boxPosX,					boxPos[0].y = boxPosY;
	boxPos[1].x = boxPosX,					boxPos[1].y = boxPosY + boxHeight;
	boxPos[2].x = boxPosX + boxWidth,       boxPos[2].y = boxPosY + boxHeight;
	boxPos[3].x = boxPosX + boxWidth,       boxPos[3].y = boxPosY;


	_x = (int)(IwGxGetScreenWidth() * 0.5 - panelWidth * 0.5f);
	_y = (int)(IwGxGetScreenHeight() * 0.5 - panelHeight * 0.5f);

	//Buttons
	btnAccept = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, _x + 70, _y + panelHeight - 20);
	btnAccept->OnClick(&onAcceptAction);
	btnAccept->OnRollOn(&OnAcceptMotion);
	btnCancel = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, _x + panelWidth - buttonWidth - 70, _y + panelHeight - 20);
	btnCancel->OnClick(&onCancelAction);
	btnCancel->OnRollOn(&OnCancelMotion);
	
	btnAccept->SetStyle(font_komikadisplaybold18, 0, -15);
	btnCancel->SetStyle(font_komikadisplaybold18, 0, -15);

	btnAccept->SetText("ACCEPT");
	btnCancel->SetText("CANCEL");

	//text
	title = NULL;
	text = NULL;

	bIsErrorMode = false;
	lastKeyPressed = s3eKeyFirst;
	bMouseBlocked = false;
	bKeyboardBlocked = false;

	focus = POPUP_FOCUS_NONE;

	background = new Background(0,0,0,fade_menu);
}

PopUp::~PopUp()
{
	delete btnAccept;
	delete btnCancel;
	delete background;
	delete button;
}

void PopUp::Render()
{
	background->Render();

	Utils::RenderPannelA(12, 4, _x, _y);
	IwGxFlush();
	
	IwGxLightingOn();

	if(text != NULL)
	{
		CIwGxFontPreparedData data;

		IwGxFontSetRect(CIwRect(_x + 45 - 1, _y + 1, panelWidth - 90, panelHeight));
		IwGxFontSetFont(font_komikadisplaybold22);
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
		IwGxFontSetCol(0xff000000);
		IwGxFontPrepareText(data, text);
		IwGxFontDrawText(data);

		IwGxFontSetRect(CIwRect(_x + 45, _y, panelWidth - 90, panelHeight));
		IwGxFontSetFont(font_komikadisplaybold22);
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
		IwGxFontSetCol(0xff00e0f7);
		IwGxFontPrepareText(data, text);
		IwGxFontDrawText(data);
	}
	//font	
	IwGxLightingOff();
	
	IwGxFlush();
	//buttons
	btnAccept->Render();
	
	if(!bIsErrorMode) 
		btnCancel->Render();
}

void PopUp::On()
{
	if(popUpState == GSP_NewGame)
	{
		bIsErrorMode = false;
		title = "New Game";
		text = "Saved data will be eliminated, Do you want to continue?";
	}
	else if(popUpState == GSP_QuitGame)
	{
		bIsErrorMode = false;
		title = "Quit Game";
		text = "The game will be closed.\nAre you sure?";
	}
	else if(popUpState == GSP_NewGame_Error)
	{
		bIsErrorMode = true;
		title = "Error";
		text = "This feature is not implemented yet.";
	}
	else if(popUpState == GSP_Retry)
	{
		bIsErrorMode = false;
		title = "Retry";
		text = "Are you sure you want to start again?";
	}
	else if(popUpState == GSP_ReturnToMainMenu)
	{
		bIsErrorMode = false;
		title = "Quit Game";
		text = "Are you sure you want to return to Main Menu?";
	}
	else if(popUpState == GSP_QuitGameLooseEnding)
	{
		bIsErrorMode = false;
		title = "Quit Game";
		text = "Are you sure you want to return to Main Menu?";
	}
	else if(popUpState == GSP_MapSelection)
	{
		bIsErrorMode = false;
		text = "Are you sure you want to return to Map Selection?";
	}
	else if(popUpState == GSP_ResetStore)
	{
		bIsErrorMode = false;
		text = "The upgrades will be deleted but the stars will remain. Do you want to continue?";
	}

	ChangeFocus(POPUP_FOCUS_NONE);
	ChangeFocus(POPUP_FOCUS_ACCEPT);

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_pop, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_pop, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_pop, NULL);
}

void PopUp::Off()
{
	title = NULL;
	text = NULL;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_pop);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_pop);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_pop);
}

void PopUp::UpdateClickEvents(s3ePointerEvent *event)
{
	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	btnAccept->HandleClick(event);
	btnCancel->HandleClick(event);
}

void PopUp::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	btnAccept->HandleMotion(event);
	btnCancel->HandleMotion(event);
}

void PopUp::ChangeFocus(PopUpFocusState newFocus)
{
	if(newFocus == focus)
		return;

	if(focus == POPUP_FOCUS_ACCEPT) btnAccept->IsFocused(false);
	else if(focus == POPUP_FOCUS_CANCEL) btnCancel->IsFocused(false);

	if(newFocus == POPUP_FOCUS_ACCEPT) btnAccept->IsFocused(true);
	else if(newFocus == POPUP_FOCUS_CANCEL) btnCancel->IsFocused(true);

	focus = newFocus;
}

void PopUp::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(event->m_Pressed)
	{
		bMouseBlocked = true;
		//lastKeyPressed = event->m_Key;
		if((event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter) && focus == POPUP_FOCUS_ACCEPT)
			btnAccept->SetStatus(3);
		else if((event->m_Key == s3eKeyEsc || event->m_Key == s3eKeyBack || event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter) && focus == POPUP_FOCUS_CANCEL) 
			btnCancel->SetStatus(3);
		else if(event->m_Key == s3eKeyAbsLeft)
		{
			if(!bIsErrorMode && focus == POPUP_FOCUS_CANCEL)
			{
				ChangeFocus(POPUP_FOCUS_ACCEPT);
			}
		} 
		else if(event->m_Key == s3eKeyAbsRight)
		{
			if(!bIsErrorMode && focus == POPUP_FOCUS_ACCEPT)
			{
				ChangeFocus(POPUP_FOCUS_CANCEL);
			}
		}
		else
		{
			if(focus == POPUP_FOCUS_CANCEL) btnCancel->SetStatus(1);
			else if(focus == POPUP_FOCUS_ACCEPT) btnAccept->SetStatus(1);
		}
	}
	else
	{
		if(event->m_Key == s3eKeyEsc || event->m_Key == s3eKeyBack)
		{
			if(!bIsErrorMode) btnCancel->Execute();
		}
		else if(event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter)
		{
			if(focus == POPUP_FOCUS_ACCEPT) btnAccept->Execute();
			else if(focus == POPUP_FOCUS_CANCEL) btnCancel->Execute();
		}

		//lastKeyPressed = s3eKeyFirst;
		bMouseBlocked = false;
	}
}

void PopUp::MouseHack()
{
	if(focus == POPUP_FOCUS_ACCEPT) btnAccept->Execute();
	else if(focus == POPUP_FOCUS_CANCEL) btnCancel->Execute();
}

void PopUp::ActiveButtonMouseHack()
{
	if(focus == POPUP_FOCUS_ACCEPT)
		btnAccept->SetStatus(3);

	if(focus == POPUP_FOCUS_CANCEL)
		btnCancel->SetStatus(3);
}