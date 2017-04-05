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
#include "localization.h"
#include "utils.h"
#include "texturehelper.h"
#if !defined DESKTOPPLATFORM
#include "s3eFlurry.h"
#endif


MenuIG *menuigRef = NULL;

//Mouse Motion
void HandleMotion_MIG(s3ePointerMotionEvent* event) 
{
	if(menuigRef == NULL || popUpState != GSP_None)
		return;
	
	menuigRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_MIG(s3ePointerEvent *event) 
{
	if(menuigRef == NULL || popUpState != GSP_None)
		return;

	menuigRef->UpdateClickEvents(event);
}

//Buttons functions
void MIG_OnClick_Resume()
{
	inGamePausedState = GSGP_None;
	mainMenuIGState = GSMIG_None;
}

void MIG_OnClick_Retry()
{
	popUpState = GSP_Retry;
}

void MIG_OnClick_Options()
{
	popUpState = GSP_MapSelection;
}

void MainMenuInGame_Options()
{
	mainMenuIGState = GSMIG_Options;
}

void MIG_OnClick_QuitGame()
{
	popUpState = GSP_ReturnToMainMenu;
}

#if defined DESKTOPPLATFORM
void MIG_Xbox360_Up()
{
	menuigRef->focusManager.ChangeFocus(Xbox360_Up);
}

void MIG_Xbox360_Down()
{
	menuigRef->focusManager.ChangeFocus(Xbox360_Down);
}

void MIG_Xbox360_Action()
{
	menuigRef->focusManager.ExecuteFocusedObject();
}
#endif

MenuIG::MenuIG()
{
	menuigRef = this;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu_long");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	CIwSVec2 buttonDimension = TextureHelper::GetDimension(buttonRenderData.state[0].Pressed);
	buttonWidth = buttonDimension.x;
	buttonHeight = buttonDimension.y;

	//Settings
	titlePos = CIwRect(500, 60, 150, 30);
	color = 0xffffffff;
	//Menu position
	btnX =  (int)((currentGameWidth - buttonWidth) * 0.5f);
	btnY = Utils::GetScaledInt(185);
	//y distance between two consecutive buttons
	margin = 10;
	btnH = (int)(buttonHeight);
	btnH = Utils::GetScaledInt(btnH);
	//End Settings

	//Set buttons
	resume = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 0);
	resume->OnClick(&MIG_OnClick_Resume);
	resume->SetStyle(NULL, 60, 5);

	howtoplay = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 1);
	howtoplay->SetStyle(NULL, 60, 5);

	retry = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 1);
	retry->OnClick(&MIG_OnClick_Retry);
	retry->SetStyle(NULL, 60, 5);

	worldMap = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 2);
	worldMap->OnClick(&MIG_OnClick_Options);
	worldMap->SetStyle(NULL, 60, 5);

	options = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 4);
	options->OnClick(&MainMenuInGame_Options);

	quitgame = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnX, btnY + (btnH + margin) * 5);
	quitgame->OnClick(&MIG_OnClick_QuitGame);
	quitgame->SetStyle(NULL, 60, 5);

	lastKeyPressed = s3eKeyFirst; //InvalidKey

	background = new Background(0, 0, 0, fade_menu - 50);

	//Text
	title = loc_ui_pause;
	resume->SetText(loc_ui_resume);
	howtoplay->SetText(loc_ui_howToPlay);
	retry->SetText(loc_ui_retry);
	worldMap->SetText(loc_ui_worldMap);
	options->SetText(loc_ui_settings);
	quitgame->SetText(loc_ui_quitGame);

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Up, &MIG_Xbox360_Up);
	xbox360Controller.SetCallback(Xbox360_Down, &MIG_Xbox360_Down);
	xbox360Controller.SetCallback(Xbox360_Action1, &MIG_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Action2, &MIG_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Action3, &MIG_OnClick_Resume);
	xbox360Controller.SetCallback(Xbox360_Cancel1, &MIG_OnClick_Resume);
	xbox360Controller.SetCallback(Xbox360_Cancel2, &MIG_OnClick_Resume);
#endif
}

MenuIG::~MenuIG()
{
	delete resume;
	delete howtoplay;
	delete retry;
	delete worldMap;
	delete quitgame;
	delete options;
	delete background;
}

void MenuIG::InitObject()
{
	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_MIG, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_MIG, NULL);

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		resume->SetPos(btnX, btnY + (btnH + margin) * 0);
		retry->SetPos(btnX, btnY + (btnH + margin) * 1);
		worldMap->SetPos(btnX, btnY + (btnH + margin) * 2);
		options->SetPos(btnX, btnY + (btnH + margin) * 3);
		quitgame->SetPos(btnX, btnY + (btnH + margin) * 4);
	}
	else
	{
		resume->SetPos(btnX, btnY + (btnH + margin) * 0);
		retry->SetPos(btnX, btnY + (btnH + margin) * 1);
		options->SetPos(btnX, btnY + (btnH + margin) * 2);
		quitgame->SetPos(btnX, btnY + (btnH + margin) * 3);
	}

#if defined DESKTOPPLATFORM
	if(focusManager.GetDefaultFocus() == options)
	{
		focusManager.SetInitialFocus(focusManager.GetDefaultFocus());
	}
	else
	{
		focusManager.SetInitialFocus(resume);
	}

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		focusManager.SetFocusDirection(Xbox360_Down, resume, retry);
		focusManager.SetFocusDirection(Xbox360_Down, retry, worldMap);
		focusManager.SetFocusDirection(Xbox360_Down, worldMap, options);
		focusManager.SetFocusDirection(Xbox360_Down, options, quitgame);

		focusManager.SetFocusDirection(Xbox360_Up, retry, resume);
		focusManager.SetFocusDirection(Xbox360_Up, worldMap, retry);
		focusManager.SetFocusDirection(Xbox360_Up, options, worldMap);
		focusManager.SetFocusDirection(Xbox360_Up, quitgame, options);
	}
	else
	{
		focusManager.SetFocusDirection(Xbox360_Down, resume, retry);
		focusManager.SetFocusDirection(Xbox360_Down, retry, options);
		focusManager.SetFocusDirection(Xbox360_Down, options, quitgame);

		focusManager.SetFocusDirection(Xbox360_Up, retry, resume);
		focusManager.SetFocusDirection(Xbox360_Up, options, retry);
		focusManager.SetFocusDirection(Xbox360_Up, quitgame, options);
	}
#endif
}

void MenuIG::EndObject()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_MIG);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_MIG);
	bIsActiveObj = false;
}

void MenuIG::Render()
{
	background->Render();

	//text
	IwGxLightingOn();
	Utils::RenderText(title, CIwRect(0, 0, currentGameWidth, 220), font_komikadisplaybold48, 0xff0da8d9, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true, 3);
	IwGxLightingOff();

	resume->Render(); 
	retry->Render();
	
	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		worldMap->Render();
	}

	options->Render();
	quitgame->Render();
}

void MenuIG::UpdateClickEvents(s3ePointerEvent *event) 
{
	resume->HandleClick(event);
	retry->HandleClick(event);

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		worldMap->HandleClick(event);
	}

	options->HandleClick(event);
	quitgame->HandleClick(event);
}

void MenuIG::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	resume->HandleMotion(event);
	retry->HandleMotion(event);

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		worldMap->HandleMotion(event);
	}

	options->HandleMotion(event);
	quitgame->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

void MenuIG::Update()
{
#if defined DESKTOPPLATFORM
	if(bIsActiveObj && popUpState == GSP_None)
	{
		xbox360Controller.Update();
	}
#endif
}
