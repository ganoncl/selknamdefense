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

#include "mm_playmenu.h"
#include "IwGx.h"
#include "IwGxFont.h"
#include "s3e.h"
#include "common.h"
#include "localization.h"
#include "button.h"
#include "mm_menu.h"
#include "parserhelper.h"
#include "texturehelper.h"
#include "s3eOSExec.h"
#include "survivalmodeconfig.h"
#if !defined DESKTOPPLATFORM
#include "iOSNativeEDK.h"
//#include "LeadboltExt.h"
#include "s3eFlurry.h"
#endif


PlayMenu *playMenuReference = NULL;

//Mouse Motion
void PlayMenu_HandleMotion(s3ePointerMotionEvent* event) 
{
	if(playMenuReference == NULL)
		return;

	playMenuReference->UpdateMotionEvents(event);
}

//Mouse Click
void PlayMenu_HandleClick(s3ePointerEvent *event) 
{
	if(playMenuReference == NULL)
		return;

	playMenuReference->UpdateClickEvents(event);
}

void PlayMenu_PlayCampaign()
{
	gameState = GS_MapSelection;
}

void PlayMenu_NewCampaign()
{
	if(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed)
	{
		popUpState = GSP_NewGame;
	}
	else
	{
		if(saveDataHelper)
			saveDataHelper->ResetGameDataCampaign();
		gameState = GS_MapSelection;
		NewGame_State = NOT_EMPTY_GAME;
		if(saveDataHelper)
			saveDataHelper->SaveGameDataCampaign();
	}
}

void PlayMenu_Survival()
{
//	if(!bIsFullVersion)
//	{
//		popUpState = GSP_SURVIVALDISABLED;
//#if !defined DESKTOPPLATFORM
//		//ADS
//		//if(bLeadbolt)
//		//{
//		//	loadAd(leadboltAdNumber);
//		//}
//#endif
//		return;
//	}

	if(survivalModeConfig->bIsSurvivalAvailable)
	{
		gameState = GS_Game;
		gameMode = GAMEMODE_SURVIVAL;
	}
	else
	{
		popUpState = GSP_SURVIVALDISABLED;
	}
}

void PlayMenu_Back()
{
	mainMenuState = GSM_MainMenu;
}

#if defined DESKTOPPLATFORM
void PlayMenu_Xbox360_A()
{
	playMenuReference->focusManager.ExecuteFocusedObject();
}

void PlayMenu_Xbox360_Up()
{
	playMenuReference->focusManager.ChangeFocus(XBOX360_Button_Up);
}

void PlayMenu_Xbox360_Down()
{
	playMenuReference->focusManager.ChangeFocus(XBOX360_Button_Down);
}
#endif

void PlayMenu_FacebookOnClick()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpValue = "Place|Menu";
		s3eFlurryLogEventParams("event_FacebookClicked", tmpValue.c_str());
	}
	Social_Facebook(loc_socialMenu);
#endif
}

void PlayMenu_TwitterOnClick()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpValue = "Place|Menu";
		s3eFlurryLogEventParams("event_TwitterClicked", tmpValue.c_str());
	}
	Social_Twitter(loc_socialMenu);
#endif
}

PlayMenu::PlayMenu()
{
	playMenuReference = this;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu_long");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	CIwSVec2 buttonDimension = TextureHelper::GetDimension(buttonRenderData.state[0].RollOn);
	int texWidth = buttonDimension.x;
	int texHeight = buttonDimension.y;

	//Title
	menuTitle = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_title"));
	CIwSVec2 menuTitleDimension = TextureHelper::GetDimension(menuTitle);
	int xTitle = (int)((currentGameWidth - menuTitleDimension.x) * 0.5f);
	int yTitle = Utils::GetScaledInt(30);;
	titlePosition = CIwSVec2(xTitle, yTitle);

	//Settings
	//Menu position
	btnXPos =  (int)((currentGameWidth - texWidth) * 0.5f);

	//default btn vertical layout: btnY first Button pos , margin, distance between buttons.
#if defined DESKTOPPLATFORM
	btnYPos = Utils::GetScaledInt(230);
	if (currentGameHeight <= 600)
		btnYPos = Utils::GetScaledInt(240);
	btnDistance = Utils::GetScaledInt(85);
#endif
#if !defined DESKTOPPLATFORM
	btnYPos = Utils::GetScaledInt(245);
	if (currentGameHeight <= 600)
		btnYPos = Utils::GetScaledInt(265);
	btnDistance = Utils::GetScaledInt(98);
#endif

	//Set buttons
	//Play Campaign
	buttonList[0].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 0);
	buttonList[0].button->OnClick(&PlayMenu_PlayCampaign);
	buttonList[0].button->SetText(loc_ui_playCampaign);//loc_ui_playCampaign);

	//New Campaign
	buttonList[1].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 1);
	buttonList[1].button->OnClick(&PlayMenu_NewCampaign);
	buttonList[1].button->SetText(loc_ui_newCampaign);//loc_ui_almanac);

	//Survival
	buttonList[2].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 2);
	buttonList[2].button->OnClick(&PlayMenu_Survival);
	buttonList[2].button->SetText(loc_ui_survival);

	//Back
	buttonList[3].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 3);
	buttonList[3].button->OnClick(&PlayMenu_Back);
	buttonList[3].button->SetText(loc_ui_back);

#if !defined DESKTOPPLATFORM
	facebookButtonRD.numStates = 1;
	TextureData facebookData = TextureHelper::GetTextureNamed("button_social");
	facebookButtonRD.state[0].RollOn = TextureHelper::GetRenderData(facebookData, 2, 2, 1);
	facebookButtonRD.state[0].RollOut = TextureHelper::GetRenderData(facebookData, 2, 2, 1);
	facebookButtonRD.state[0].Pressed = TextureHelper::GetRenderData(facebookData, 2, 2, 3);
	facebookButtonRD.state[0].Inactive = TextureHelper::GetRenderData(facebookData, 2, 2, 1);

	facebookButtonRD.state[0].RollOn.width = facebookButtonRD.state[0].RollOn.height = 64;
	facebookButtonRD.state[0].RollOut.width = facebookButtonRD.state[0].RollOut.height = 64;
	facebookButtonRD.state[0].Pressed.width = facebookButtonRD.state[0].Pressed.height = 64;
	facebookButtonRD.state[0].Inactive.width = facebookButtonRD.state[0].Inactive.height = 64;

	int currentSocialBtnXPos = Utils::GetScaledInt(32);

	facebookButton = new Button(TextureHelper::GetCIwTexture(facebookButtonRD.state[0].RollOn.atlasName), &facebookButtonRD, currentSocialBtnXPos, currentGameHeight - Utils::GetScaledInt(110));
	facebookButton->OnClick(&PlayMenu_FacebookOnClick);

	twitterButtonRD.numStates = 1;
	TextureData twitterData = TextureHelper::GetTextureNamed("button_social");
	twitterButtonRD.state[0].RollOn = TextureHelper::GetRenderData(twitterData, 2, 2, 0);
	twitterButtonRD.state[0].RollOut = TextureHelper::GetRenderData(twitterData, 2, 2, 0);
	twitterButtonRD.state[0].Pressed = TextureHelper::GetRenderData(twitterData, 2, 2, 2);
	twitterButtonRD.state[0].Inactive = TextureHelper::GetRenderData(twitterData, 2, 2, 0);

	twitterButtonRD.state[0].RollOn.width = twitterButtonRD.state[0].RollOn.height = 64;
	twitterButtonRD.state[0].RollOut.width = twitterButtonRD.state[0].RollOut.height = 64;
	twitterButtonRD.state[0].Pressed.width = twitterButtonRD.state[0].Pressed.height = 64;
	twitterButtonRD.state[0].Inactive.width = twitterButtonRD.state[0].Inactive.height = 64;


	twitterButton = new Button(TextureHelper::GetCIwTexture(twitterButtonRD.state[0].RollOn.atlasName), &twitterButtonRD, currentSocialBtnXPos + 64 + 10, currentGameHeight - Utils::GetScaledInt(110));
	twitterButton->OnClick(&PlayMenu_TwitterOnClick);
#endif
#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, &PlayMenu_Xbox360_A);
	xbox360Controller.SetCallback(Xbox360_Action2, &PlayMenu_Xbox360_A);
	xbox360Controller.SetCallback(Xbox360_Action3, &PlayMenu_Xbox360_A);
	xbox360Controller.SetCallback(Xbox360_Cancel1, &PlayMenu_Back);
	xbox360Controller.SetCallback(Xbox360_Cancel2, &PlayMenu_Back);
	xbox360Controller.SetCallback(Xbox360_Up, &PlayMenu_Xbox360_Up);
	xbox360Controller.SetCallback(Xbox360_Down, &PlayMenu_Xbox360_Down);
#endif
}

PlayMenu::~PlayMenu()
{
#if !defined DESKTOPPLATFORM
	delete facebookButton;
	delete twitterButton;
#endif
}

void PlayMenu::InitObject()
{
	SetIsActive(true);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)PlayMenu_HandleMotion, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)PlayMenu_HandleClick, NULL);
	
	SetMenuPositionWithResumeButton(NewGame_State == NOT_EMPTY_GAME && bFirstMapWasPlayed);
}

void PlayMenu::EndObject()
{
	SetIsActive(false);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)PlayMenu_HandleMotion);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)PlayMenu_HandleClick);

	for(int i = 0; i < PLAYMENU_MAX_NUMBER_BUTTONS; i++) buttonList[i].button->SetStatus(2);

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(NULL);
#endif
}

void PlayMenu::Render()
{
	Utils::RenderSingleTexture(atlas_bkg_menu_others, titlePosition, menuTitle);

	for(int i = 0; i < PLAYMENU_MAX_NUMBER_BUTTONS; i++) buttonList[i].Render();

#if !defined DESKTOPPLATFORM
	facebookButton->Render();
	twitterButton->Render();
#endif
}

void PlayMenu::Update()
{
	if(popUpState != GSP_None)
	{ 
		if(GetIsActive())
			EndObject();
	}
	else  
	{
		if(!GetIsActive())
			InitObject();
	}

#if defined DESKTOPPLATFORM
	if(GetIsActive())
	{
		xbox360Controller.Update();
	}
#endif
}

void PlayMenu::UpdateClickEvents(s3ePointerEvent *event)
{
	for(int i = 0; i < PLAYMENU_MAX_NUMBER_BUTTONS; i++) buttonList[i].HandleClick(event);

#if !defined DESKTOPPLATFORM
	facebookButton->HandleClick(event);
	twitterButton->HandleClick(event);
#endif
}

void PlayMenu::UpdateMotionEvents(s3ePointerMotionEvent* event) 
{
	for(int i = 0; i < PLAYMENU_MAX_NUMBER_BUTTONS; i++) buttonList[i].HandleMotion(event);
#if !defined DESKTOPPLATFORM
	twitterButton->HandleMotion(event);
	facebookButton->HandleMotion(event);
#endif
#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

void PlayMenu::SetMenuPositionWithResumeButton(bool bWithResume)
{
	int newGameYOffset = Utils::GetScaledInt(85);
#if !defined DESKTOPPLATFORM
	newGameYOffset = Utils::GetScaledInt(98);
#endif

	if(bWithResume)
		newGameYOffset = 0;

	for(int i = 0; i < PLAYMENU_MAX_NUMBER_BUTTONS; i++) buttonList[i].button->SetPos(btnXPos, btnYPos + (btnDistance * i) - newGameYOffset);

	buttonList[0].bRender = bWithResume;

#if defined DESKTOPPLATFORM
	focusManager.ClearFocusDirections();
	focusManager.SetFocusDirection(XBOX360_Button_Down, buttonList[0].button, buttonList[1].button);
	focusManager.SetFocusDirection(XBOX360_Button_Down, buttonList[1].button, buttonList[2].button);
	focusManager.SetFocusDirection(XBOX360_Button_Down, buttonList[2].button, buttonList[3].button);

	if(bWithResume)
		focusManager.SetFocusDirection(XBOX360_Button_Up, buttonList[1].button, buttonList[0].button);
	focusManager.SetFocusDirection(XBOX360_Button_Up, buttonList[2].button, buttonList[1].button);
	focusManager.SetFocusDirection(XBOX360_Button_Up, buttonList[3].button, buttonList[2].button);

	if(bWithResume)
	{
		if(focusManager.GetLastObjectFocused() == NULL || focusManager.GetLastObjectFocused() == buttonList[3].button)
			focusManager.SetInitialFocus(buttonList[0].button);
		else
			focusManager.SetInitialFocus(focusManager.GetLastObjectFocused());
	}
	else
	{
		if(focusManager.GetLastObjectFocused() == NULL || focusManager.GetLastObjectFocused() == buttonList[3].button)
			focusManager.SetInitialFocus(buttonList[1].button);
		else
			focusManager.SetInitialFocus(focusManager.GetLastObjectFocused());
	}
#endif
}