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
#include "mm_menu.h"
#include "parserhelper.h"
#include "texturehelper.h"
#include "s3eOSExec.h"
#include "savedatahelper.h"
#if !defined DESKTOPPLATFORM
#include "IwBilling.h"
#include "iOSNativeEDK.h"
//#include "LeadboltExt.h"
#include "s3eFlurry.h"
#include "s3eIOSIAd.h"
#endif

#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif
#ifdef GAMECENTER
#include "s3eIOSGameCenter.h"
#endif

Menu *mainRef = NULL;

//Mouse Motion
void HandleMotion(s3ePointerMotionEvent* event) 
{
	if(mainRef == NULL)
		return;

	mainRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick(s3ePointerEvent *event) 
{
	if(mainRef == NULL)
		return;

	mainRef->UpdateClickEvents(event);
}

//Buttons functions
void MainMenu_PlayGame()
{
	mainMenuState = GSM_Play;
}

void MainMenu_Options()
{
	mainMenuState = GSM_Options;
}

void MainMenu_Almanac()
{
	mainMenuState = GSM_Almanac;
#if !defined DESKTOPPLATFORM
	//ADS
	//if(bLeadbolt)
	//{
	//	loadAd(leadboltAdNumber);
	//}
	if(bFlurryOn)
	{
		string tmpValue = "Place|Menu";
		s3eFlurryLogEventParams("event_ShowAlmanac", tmpValue.c_str());
	}
#endif
}

void MainMenu_Leaderboards()
{
#ifdef GAMECENTER
	if(bGameCenterAvailable && bGameCenterAuthenticated)
		s3eIOSGameCenterLeaderboardShowGUI("campaignsd", S3E_IOSGAMECENTER_PLAYER_SCOPE_ALL_TIME);
	return;
#endif
	mainMenuState = GSM_Leaderboards;
}

void MainMenu_QuitGame()
{
	popUpState = GSP_QuitGame;
}
#if !defined DESKTOPPLATFORM
void ErrorCallback(void* userdata, IwBilling::CIwBillingErrorData* data)
{
	bErrorOnLastPurchase = true;
	popUpState = GSP_SOCIALNETWORKPUBLISHFAIL;
}

void ReceiptAvailableCallback(void* userdata, IwBilling::CIwBillingReceiptAvailableData* data)
{
	if (data->Restored)
		bItemRestoredSuccess = true;
	else
		bItemPurchasedSuccess = true;

	if(bItemRestoredSuccess || bItemPurchasedSuccess)
	{
		bIsFullVersion = true;
		if(bIsFullVersion)
		{
			mainRef->buttonList[4].bRender = false;
		}

		if(saveDataHelper)
			saveDataHelper->SaveGameLocalData();
	}
	char* receipt = new char[data->ReceiptLength + 1];
	memcpy(receipt, data->Receipt, data->ReceiptLength);
	receipt[data->ReceiptLength] = 0;
	//AddStatus("Product ID: %s", data->ProductID);
	//if (receipt != NULL)
	//	AddStatus("Receipt: %s", receipt);
	//if (data->TransactionID != NULL)
	//	AddStatus("TransactionID: %s", data->TransactionID);

	//if (data->PurchaseToken != NULL)
	//{
	//	g_PurchaseToken = data->PurchaseToken;
	//	g_ConsumeButton->m_Enabled = true;
	//}

	//s3eDebugTracePrintf("===================================================");
	//s3eDebugTracePrintf("Receipt available");
	//s3eDebugTracePrintf("Product ID: %s", data->ProductID);
	//if (receipt != NULL)
	//	s3eDebugTracePrintf("Receipt: %s", receipt);
	//if (data->TransactionID != NULL)
	//	s3eDebugTracePrintf("TransactionID: %s", data->TransactionID);
	//s3eDebugTracePrintf("===================================================");

	IwBilling::FinishTransaction(data->FinaliseData);
	delete [] receipt;
}

void RefundCallback(void* userdata, IwBilling::CIwBillingRefundData* data)
{
	bItemRefundSuccess = true;
	//AddStatus("Product was refunded %s", data->ProductID);
	//s3eDebugTracePrintf("===================================================");
	//s3eDebugTracePrintf("Product was refunded %s", data->ProductID);
	//s3eDebugTracePrintf("===================================================");

	IwBilling::FinishTransaction(data->FinaliseData);

	bIsFullVersion = false;
	if(saveDataHelper)
		saveDataHelper->SaveGameLocalData();
}

//void ConsumeCallback(void* userdata, void* data)
//{
//	AddStatus("Product was consumed %s", g_PurchaseToken.c_str());
//	g_ConsumeButton->m_Enabled = false;
//}

void MainMenu_RemoveAds()
{
	IwBilling::PurchaseProduct(removeAds_ProductID);
}

void MainMenu_FacebookOnClick()
{
	if(bFlurryOn)
	{
		string tmpValue = "Place|Menu";
		s3eFlurryLogEventParams("event_FacebookClicked", tmpValue.c_str());
	}
	Social_Facebook(loc_socialMenu);

}

void MainMenu_TwitterOnClick()
{
	if(bFlurryOn)
	{
		string tmpValue = "Place|Menu";
		s3eFlurryLogEventParams("event_TwitterClicked", tmpValue.c_str());
	}
	Social_Twitter(loc_socialMenu);
}
#endif
#if defined DESKTOPPLATFORM
//XBOX 360 CALLBACK FUNCTIONS
void MainMenu_XBOX360_ButtonA()
{
	mainRef->focusManager.ExecuteFocusedObject();
}

void MainMenu_XBOX360_ButtonDown()
{
	mainRef->focusManager.ChangeFocus(XBOX360_Button_Down);
}

void MainMenu_XBOX360_ButtonUp()
{
	mainRef->focusManager.ChangeFocus(XBOX360_Button_Up);
}
#endif

Menu::Menu()
{
	mainRef = this;

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
	//Play Game
	buttonList[0].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 0);
	buttonList[0].button->OnClick(&MainMenu_PlayGame);
	buttonList[0].button->SetText(loc_ui_playGame);

	//Alamanac
	buttonList[1].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 1);
	buttonList[1].button->OnClick(&MainMenu_Almanac);
	buttonList[1].button->SetText(loc_ui_almanac);

	//Leaderboards
	buttonList[2].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 2);
	buttonList[2].button->OnClick(&MainMenu_Leaderboards);
	buttonList[2].button->SetText(loc_ui_leaderboards);
#ifdef GAMECENTER
	buttonList[2].button->SetText(loc_ui_leaderboards);
#endif
	
	//Credits
	buttonList[3].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 3);
	buttonList[3].button->OnClick(&MainMenu_Options);
	buttonList[3].button->SetText(loc_ui_options);

	//QuitGame
	buttonList[4].button = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance)* 4);
	buttonList[4].button->OnClick(&MainMenu_QuitGame);
	buttonList[4].button->SetText(loc_ui_exitGame);
#if defined DESKTOPPLATFORM
	buttonList[4].bRender = true;
#endif
#if !defined DESKTOPPLATFORM
	buttonList[4].bRender = false;
#endif


#if !defined DESKTOPPLATFORM
	if(!bIsFullVersion)
	{
		buttonList[4].button->OnClick(&MainMenu_RemoveAds);
		buttonList[4].button->SetText(loc_ui_removeAds);
		buttonList[4].bRender = true;
	}
	else
	{
		buttonList[4].bRender = false;
	}
#endif

	//set enter animation
	bDoEnterAnimation = true;
	for(int i = 0; i < MENU_NUMBER_BUTTONS; i++)
	{
		buttonInitPos[i] = CIwSVec2(btnXPos, btnYPos + (btnDistance) * i - 100);
		buttonFinalPos[i] = CIwSVec2(btnXPos, btnYPos + (btnDistance) * i);
		buttonCurrentAlpha[i] = 0;
		buttonAccel[i] = 1000;

		buttonList[i].button->SetPos(buttonInitPos[i].x, buttonInitPos[i].y);
		buttonList[i].button->SetAlphaValue(buttonCurrentAlpha[i]);
		buttonList[i].button->SetButtonOnAnim(true);
		buttonAnimTimer[i].SetTimeLimit(100 + 200 * i);
		bMoveButton[i] = false;
		bApplyAlphaAnimButton[i] = false;
	}

	//SetMenuPositionWithResumeButton(NewGame_State == NOT_EMPTY_GAME);

	bIsOn = false;

	lastKeyPressed = s3eKeyFirst; //InvalidKey

	Timer_BlockAfterFadeOut.SetTimeLimit(150);
	bStartTimer = true;
	bDoJustOnceInWhileGame = true;
	bBlockDeviceHandler = true;

#if !defined DESKTOPPLATFORM
		facebookButtonRD.numStates = 1;
		TextureData facebookData = TextureHelper::GetTextureNamed("button_social");
		facebookButtonRD.state[0].RollOn   = TextureHelper::GetRenderData(facebookData, 2, 2, 1);
		facebookButtonRD.state[0].RollOut  = TextureHelper::GetRenderData(facebookData, 2, 2, 1);
		facebookButtonRD.state[0].Pressed  = TextureHelper::GetRenderData(facebookData, 2, 2, 3);
		facebookButtonRD.state[0].Inactive = TextureHelper::GetRenderData(facebookData, 2, 2, 1);

		facebookButtonRD.state[0].RollOn.width   = facebookButtonRD.state[0].RollOn.height   = 64;
		facebookButtonRD.state[0].RollOut.width  = facebookButtonRD.state[0].RollOut.height  = 64;
		facebookButtonRD.state[0].Pressed.width  = facebookButtonRD.state[0].Pressed.height  = 64;
		facebookButtonRD.state[0].Inactive.width = facebookButtonRD.state[0].Inactive.height = 64;

		int currentSocialBtnXPos = Utils::GetScaledInt(32);

		facebookButton = new Button(TextureHelper::GetCIwTexture(facebookButtonRD.state[0].RollOn.atlasName), &facebookButtonRD, currentSocialBtnXPos, currentGameHeight - Utils::GetScaledInt(110));
		facebookButton->OnClick(&MainMenu_FacebookOnClick);

		twitterButtonRD.numStates = 1;
		TextureData twitterData = TextureHelper::GetTextureNamed("button_social");
		twitterButtonRD.state[0].RollOn   = TextureHelper::GetRenderData(twitterData, 2, 2, 0);
		twitterButtonRD.state[0].RollOut  = TextureHelper::GetRenderData(twitterData, 2, 2, 0);
		twitterButtonRD.state[0].Pressed  = TextureHelper::GetRenderData(twitterData, 2, 2, 2);
		twitterButtonRD.state[0].Inactive = TextureHelper::GetRenderData(twitterData, 2, 2, 0);

		twitterButtonRD.state[0].RollOn.width   = twitterButtonRD.state[0].RollOn.height   = 64;
		twitterButtonRD.state[0].RollOut.width  = twitterButtonRD.state[0].RollOut.height  = 64;
		twitterButtonRD.state[0].Pressed.width  = twitterButtonRD.state[0].Pressed.height  = 64;
		twitterButtonRD.state[0].Inactive.width = twitterButtonRD.state[0].Inactive.height = 64;


		twitterButton = new Button(TextureHelper::GetCIwTexture(twitterButtonRD.state[0].RollOn.atlasName), &twitterButtonRD, currentSocialBtnXPos + 64 + 10, currentGameHeight - Utils::GetScaledInt(110));
		twitterButton->OnClick(&MainMenu_TwitterOnClick);
#endif

#if defined DESKTOPPLATFORM
	ConfigureXBOX360Callbacks();
#endif
}

void Menu::ConfigureXBOX360Callbacks()
{
#if defined DESKTOPPLATFORM
	//XBOX 360 MAIN MENU CONFIGURATION
	xbox360Controller.SetCallback(XBOX360_Button_A, &MainMenu_XBOX360_ButtonA);
	xbox360Controller.SetCallback(XBOX360_Button_X, &MainMenu_XBOX360_ButtonA);
	xbox360Controller.SetCallback(Xbox360_Action3, &MainMenu_XBOX360_ButtonA);
	xbox360Controller.SetCallback(XBOX360_Button_Down, &MainMenu_XBOX360_ButtonDown);
	xbox360Controller.SetCallback(XBOX360_Button_Up, &MainMenu_XBOX360_ButtonUp);

	focusManager.SetFocusDirection(XBOX360_Button_Down, buttonList[0].button, buttonList[1].button);
	focusManager.SetFocusDirection(XBOX360_Button_Down, buttonList[1].button, buttonList[2].button);
	focusManager.SetFocusDirection(XBOX360_Button_Down, buttonList[2].button, buttonList[3].button);
	focusManager.SetFocusDirection(XBOX360_Button_Down, buttonList[3].button, buttonList[4].button);

	focusManager.SetFocusDirection(XBOX360_Button_Up, buttonList[1].button, buttonList[0].button);
	focusManager.SetFocusDirection(XBOX360_Button_Up, buttonList[2].button, buttonList[1].button);
	focusManager.SetFocusDirection(XBOX360_Button_Up, buttonList[3].button, buttonList[2].button);
	focusManager.SetFocusDirection(XBOX360_Button_Up, buttonList[4].button, buttonList[3].button);
#endif

}

void Menu::RegisterIAPCallbacks()
{
#if !defined DESKTOPPLATFORM
	// Set up callbacks
	IwBilling::setErrorCallback(ErrorCallback, 0);
	IwBilling::setReceiptAvailableCallback(ReceiptAvailableCallback, 0);
	IwBilling::setRefundCallback(RefundCallback, 0);
#endif
}

Menu::~Menu()
{
#if !defined DESKTOPPLATFORM
	delete facebookButton;
	delete twitterButton;
#endif
}

void Menu::InitObject()
{
	bIsActiveObj = true;
	Utils::RegisterMouseCallbacks(&HandleClick, &HandleMotion);
	bIsOn = true;
}

void Menu::EndObject()
{
	Utils::UnRegisterMouseCallbacks(&HandleClick, &HandleMotion);
	bIsActiveObj = false;
}

void Menu::Render()
{
	Utils::RenderSingleTexture(atlas_bkg_menu_others, titlePosition, menuTitle);

	IwGxFlush();

	for(int i = 0; i < MENU_NUMBER_BUTTONS; i++) buttonList[i].Render();

#if !defined DESKTOPPLATFORM
	facebookButton->Render();
	twitterButton->Render();
#endif
	IwGxFlush();
}

void Menu::UpdateClickEvents(s3ePointerEvent *event) 
{
	for(int i = 0; i < MENU_NUMBER_BUTTONS; i++)
	{
		buttonList[i].HandleClick(event);
	}
#if !defined DESKTOPPLATFORM
	facebookButton->HandleClick(event);
	twitterButton->HandleClick(event);
#endif
}

void Menu::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	for(int i = 0; i < MENU_NUMBER_BUTTONS; i++) 
	{
		buttonList[i].HandleMotion(event);
	}

#if !defined DESKTOPPLATFORM
	twitterButton->HandleMotion(event);
	facebookButton->HandleMotion(event);
#endif

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

void Menu::Off()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick);
	bIsOn = false;

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(NULL);
#endif
}

void Menu::On()
{
	bStartTimer = true;

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick, NULL);
	bIsOn = true;

	for(int i = 0; i < MENU_NUMBER_BUTTONS; i++) buttonList[i].button->SetStatus(2);

#if !defined DESKTOPPLATFORM
	facebookButton->SetStatus(2);
	twitterButton->SetStatus(2);
#endif
#if defined DESKTOPPLATFORM
	if(focusManager.GetLastObjectFocused() == NULL)
	{
		focusManager.ChangeFocus(buttonList[0].button);
	}
	else
	{
		focusManager.ChangeFocus(focusManager.GetLastObjectFocused());
	}
#endif

	//Animation Check
	//Check Enter buttons animation
	if(bDoEnterAnimation)
	{
		bDoEnterAnimation = false;
		for(int i = 0; i < MENU_NUMBER_BUTTONS; i++)
		{
			IwTrace(MYTIMER, (" InitObject buttonAnimTimer Start i %i", i));
			buttonList[i].button->SetPos(buttonInitPos[i].x, buttonInitPos[i].y);
			buttonList[i].button->SetAlphaValue(buttonCurrentAlpha[i]);
			buttonList[i].button->SetButtonOnAnim(true);
			buttonAccel[i] = 1000;
			buttonAnimTimer[i].Reset();
			buttonAnimTimer[i].Start();
		}
	}
	else
	{
		for(int i = 0; i < MENU_NUMBER_BUTTONS; i++)
		{
			buttonList[i].button->SetPos(buttonFinalPos[i].x, buttonFinalPos[i].y);
			buttonList[i].button->SetAlphaValue(255);
			buttonList[i].button->SetButtonOnAnim(false);
			buttonAccel[i] = 0;
		}
	}
}

void Menu::Update()
{
#if defined DESKTOPPLATFORM
	if(popUpState == GSP_None && !bBlockDeviceHandler)
		xbox360Controller.Update();
#endif
	if(bDoJustOnceInWhileGame)
	{
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

	//Check Enter buttons animation
	for(int i = 0; i < MENU_NUMBER_BUTTONS; i++)
	{
		//IwTrace(MYTIMER, ("buttonAnimTimer i %i", i));
		buttonAnimTimer[i].Update();
		if(buttonAnimTimer[i].GetIsEnd())
		{
			buttonAnimTimer[i].Stop();
			bMoveButton[i] = true;
			bApplyAlphaAnimButton[i] = true;
		}
		if(bMoveButton[i])
		{
			if(buttonList[i].button->GetPos().y >= buttonFinalPos[i].y)
			{
				bMoveButton[i] = false;
				buttonList[i].button->SetPos(buttonFinalPos[i].x, buttonFinalPos[i].y);
				buttonList[i].button->SetButtonOnAnim(false);
			}
			else
			{
				buttonAccel[i] -= 50;
				if(buttonAccel[i] <= 0)
					buttonAccel[i] = 100;
				float tmpMyVeloc = buttonAccel[i] * 0.01f;
				int tmpYPos = buttonList[i].button->GetPos().y + (int)tmpMyVeloc;
				buttonList[i].button->SetPos(buttonList[i].button->GetPos().x, tmpYPos);
			}
		}
		if(bApplyAlphaAnimButton[i])
		{
			if(buttonList[i].button->GetAlphaValue() >= 255)
			{
				bApplyAlphaAnimButton[i] = false;
				buttonList[i].button->SetAlphaValue(255);
			}
			else
			{
				uint8 tmpAlpha = buttonList[i].button->GetAlphaValue() + 5;
				buttonList[i].button->SetAlphaValue(tmpAlpha);
			}
		}
	}
}