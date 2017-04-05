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
#include "localization.h"
#include "button.h"
#include "parserhelper.h"
#include "savedatahelper.h"
#include "texturehelper.h"
#include "mm_popup.h"
#if !defined DESKTOPPLATFORM
//#include "LeadboltExt.h"
#endif

#if defined DESKTOPPLATFORM
#include "focusmanager.h"
#include "xbox360controller.h"
#endif

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
		if(saveDataHelper)
			saveDataHelper->ResetGameDataCampaign();
		NewGame_State = NOT_EMPTY_GAME;
		gameState = GS_MapSelection;
		popUpState = GSP_None;
		if(saveDataHelper)
			saveDataHelper->SaveGameDataCampaign();
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
			//if(saveDataHelper)
			//  saveDataHelper->SavePersistentData();
			//s3eDeviceRequestQuit();
		}
		popUpState = GSP_None;
	}
	else if(popUpState == GSP_LEADERBOARD_ERROR)
	{
		popUpState = GSP_None;
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
#if !defined DESKTOPPLATFORM
		//ADS
		//if(bLeadbolt)
		//{
		//	loadAd(leadboltAdNumber);
		//}
#endif
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
#if !defined DESKTOPPLATFORM
		//ADS
		//if(bLeadbolt)
		//{
		//	loadAd(leadboltAdNumber);
		//}
#endif
	}
	else if(popUpState == GSP_ResetStore)
	{
		bResetStoreStats = true;
		bPausedStore     = false;
		popUpState       = GSP_None;
	}
	else if(popUpState == GSP_SOCIALNETWORKPUBLISH)
	{
		popUpState = GSP_None;
	}
	else if(popUpState == GSP_NOINTERNET)
	{
		popUpState       = GSP_None;
	}
}

//Mouse Motion
void HandleMotion_pop(s3ePointerMotionEvent* event) 
{
	if(popup == NULL)
		return;

	popup->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_pop(s3ePointerEvent *event) 
{
	if(popup == NULL)
		return;

	popup->UpdateClickEvents(event);
}

#if defined DESKTOPPLATFORM
void Popup_Xbox360_A()
{
	popup->focusManager->ExecuteFocusedObject();
}

void Popup_Xbox360_Left()
{
	popup->focusManager->ChangeFocus(XBOX360_Button_Left);
}

void Popup_Xbox360_Right()
{
	popup->focusManager->ChangeFocus(XBOX360_Button_Right);
}
#endif

PopUp::PopUp()
{
	TextureData buttonData = TextureHelper::GetTextureNamed("btn_popup_long");
	button = new ButtonRenderData();
	button->numStates = 1;
	button->state[0].RollOut  = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	button->state[0].RollOn   = TextureHelper::GetRenderData(buttonData, 1, 3, 1);
	button->state[0].Pressed  = TextureHelper::GetRenderData(buttonData, 1, 3, 2);
	button->state[0].Inactive = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	buttonWidth = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).x;
	int buttonHeight = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).y;

	//Title and text font
	pfont = font_komikadisplaybold25;

	//General Settings
	boxWidth = buttonWidth * 2 + 100;
	boxHeight = (int)(buttonHeight * 0.25f) + 150;
	boxPosX = (int)((currentGameWidth - boxWidth) * 0.5f);
	boxPosY = (int)((currentGameHeight - boxHeight) * 0.5f);
	btnAcceptPosX = boxPosX + 20;
	btnAcceptPosY = boxPosY + boxHeight - 10 - (int)(buttonHeight * 0.25f);
	btnCancelPosX = boxPosX + 30 + buttonWidth + 40;
	btnCancelPosY = boxPosY + boxHeight - 10 - (int)(buttonHeight * 0.25f);
	titleX = 40;
	titleY = 10;
	titleW = boxWidth;
	titleH = boxHeight;
	textX = 10;
	textY = 50;
	textW = boxWidth;
	textH = boxHeight;

	//Setting semi-transparent black background	
	int xPos = currentGameWidth;
	int yPos = currentGameHeight;
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

	panelWidth = 14 * 40;
	panelHeight = 6 * 40;

	_x = (int)(currentGameWidth * 0.5 - panelWidth * 0.5f);
	_y = (int)(currentGameHeight * 0.5 - panelHeight * 0.5f);

	//Buttons
	btnAccept = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, _x + 70, _y + panelHeight - 20);
	btnAccept->OnClick(&onAcceptAction);
	btnCancel = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, _x + panelWidth - buttonWidth - 70, _y + panelHeight - 20);
	btnCancel->OnClick(&onCancelAction);
	
	btnAccept->SetStyle(font_komikadisplaybold18, 0, -17);
	btnAccept->SetTextOffsetValueByState(1);
	btnCancel->SetStyle(font_komikadisplaybold18, 0, -17);
	btnCancel->SetTextOffsetValueByState(1);

	btnAccept->SetText(loc_ui_accept);
	btnCancel->SetText(loc_ui_cancel);

	text = NULL;

	bIsErrorMode = false;
	lastKeyPressed = s3eKeyFirst;
	bMouseBlocked = false;
	bKeyboardBlocked = false;
	bAdviceMode = false;

	background = new Background(0,0,0,fade_menu);

#if defined DESKTOPPLATFORM
	xbox360Controller = new Xbox360Controller();
	focusManager = new FocusManager();

	xbox360Controller->SetCallback(Xbox360_Action1, &Popup_Xbox360_A);
	xbox360Controller->SetCallback(Xbox360_Action2, &Popup_Xbox360_A);
	xbox360Controller->SetCallback(Xbox360_Action3, &Popup_Xbox360_A);
	xbox360Controller->SetCallback(Xbox360_Cancel1, &onCancelAction);
	xbox360Controller->SetCallback(Xbox360_Cancel2, &onCancelAction);
	xbox360Controller->SetCallback(Xbox360_Left, &Popup_Xbox360_Left);
	xbox360Controller->SetCallback(Xbox360_Right, &Popup_Xbox360_Right);
#endif
}

PopUp::~PopUp()
{
	delete btnAccept;
	delete btnCancel;
	delete background;
	delete button;
#if defined DESKTOPPLATFORM
	delete xbox360Controller;
	delete focusManager;
#endif
}

void PopUp::Render()
{
	background->Render();
	//Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelPopupRenderData->atlasName), CIwSVec2(_x, _y), pannelPopupRenderData);
	Utils::RenderPannelA(12, 4, _x, _y);
	IwGxFlush();	
	
	IwGxLightingOn();

	if(text != NULL)
	{
		Utils::RenderText(text,  CIwRect(_x + 45, _y, panelWidth - 90, panelHeight), font_komikadisplaybold22, 0xff00e0f7, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE);
	}
	//font	
	IwGxLightingOff();
	
	IwGxFlush();
	//buttons

	if(bAdviceMode)
		return;

	if(!bIsErrorMode) 
	{
		btnAccept->Render();
	}
	
	btnCancel->Render();
}

void PopUp::On()
{
	bAdviceMode = false;
	if(popUpState == GSP_NewGame)
	{
		bIsErrorMode = false;
		text = loc_popup_savedData;
	}
	else if(popUpState == GSP_LEADERBOARD_ERROR)
	{
		bIsErrorMode = true;
		text = loc_popup_leaderboarderror;
	}
	else if(popUpState == GSP_QuitGame)
	{
		bIsErrorMode = false;
		text = loc_popup_quitGame;
	}
	else if(popUpState == GSP_NewGame_Error)
	{
		bIsErrorMode = true;
		text = loc_popup_noFeature;
	}
	else if(popUpState == GSP_Retry)
	{
		bIsErrorMode = false;
		text = loc_popup_retry;
	}
	else if(popUpState == GSP_ReturnToMainMenu)
	{
		bIsErrorMode = false;
		text = loc_popup_returnToMenu;
	}
	else if(popUpState == GSP_QuitGameLooseEnding)
	{
		bIsErrorMode = false;
		text = loc_popup_returnToMenu;
	}
	else if(popUpState == GSP_MapSelection)
	{
		bIsErrorMode = false;
		text = loc_popup_returnToWorldMap;
	}
	else if(popUpState == GSP_ResetStore)
	{
		bIsErrorMode = false;
		text = loc_popup_upgrades;
	}
	else if(popUpState == GSP_SURVIVALDISABLED)
	{
		bIsErrorMode = true;
		text = loc_popup_survival;
		//if(!bIsFullVersion)
		//	text = loc_popup_trial;
	}
	else if(popUpState == GSP_SOCIALNETWORKPUBLISH)
	{
		bIsErrorMode = true;
		text = loc_popup_thanks;
	}
	else if(popUpState == GSP_SOCIALNETWORKPUBLISHFAIL)
	{
		bIsErrorMode = true;
		text = loc_popup_error;
	}
	else if(popUpState == GSP_NOINTERNET)
	{
		bAdviceMode = true;
		text = loc_popup_PopupInternetConnection;
		bNeedCheckInternetConn = true;
	}
	
	if(bAdviceMode)
		return;

	if(!bIsErrorMode)
	{
		btnAccept->SetPos(_x + 70, _y + panelHeight - 20);
		btnCancel->SetPos( _x + panelWidth - buttonWidth - 70, _y + panelHeight - 20);
	}
	else
	{
		btnCancel->SetPos( _x + (int)((panelWidth - buttonWidth) * 0.5f), _y + panelHeight - 20);
	}

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_pop, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_pop, NULL);

#if defined DESKTOPPLATFORM
	focusManager->ClearFocusDirections();
	if(!bIsErrorMode)
	{
		//focusManager->SetDefaultFocus(btnCancel);
		focusManager->SetFocusDirection(XBOX360_Button_Left, btnCancel, btnAccept);
		focusManager->SetFocusDirection(XBOX360_Button_Right, btnAccept, btnCancel);
	}
	else
	{
		//focusManager->SetDefaultFocus(btnCancel);
	}

	//if(activeControllerType == Controller_XBOX360)
	//{
		focusManager->ChangeFocus(btnCancel);
	//}
#endif
}

void PopUp::Off()
{
	text = NULL;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_pop);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_pop);

#if defined DESKTOPPLATFORM
	focusManager->ChangeFocus(NULL);
#endif
}

void PopUp::UpdateClickEvents(s3ePointerEvent *event)
{
	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	if(!bIsErrorMode)
	{
		btnAccept->HandleClick(event);
	}

	btnCancel->HandleClick(event);
}

void PopUp::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!bIsErrorMode)
	{
		btnAccept->HandleMotion(event);
	}
	
	btnCancel->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager->UpdateMotionEvents(event);
#endif
}

void PopUp::Update()
{
#if defined DESKTOPPLATFORM
	if(popUpState != GSP_None)
		xbox360Controller->Update();
#endif
	if(bNeedCheckInternetConn)
	{
		if(s3eSocketGetInt(S3E_SOCKET_NETWORK_AVAILABLE) == 1)
		{
			popUpState = GSP_None;
			bNeedCheckInternetConn = false;
		}
	}
}