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

#include "almanac_menu.h"
#if !defined DESKTOPPLATFORM
#include "s3eFlurry.h"
#endif

AlmanacMenu* almanacMenuRef = NULL;

//Mouse Motion
void HandleMotion_AlmanacMenu(s3ePointerMotionEvent* event) 
{
	if(almanacMenuRef == NULL || popUpState != GSP_None)
		return;

	almanacMenuRef->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClick_AlmanacMenu(s3ePointerEvent *event) 
{
	if(almanacMenuRef == NULL || popUpState != GSP_None)
		return;

	almanacMenuRef->UpdateClickEvents(event);
}


void btnBackAction_AlmanacMenu()
{
	if(inGamePausedState == GSGP_AlmanacIG)
	{
		inGamePausedState = GSGP_None;
	}
	else if(mainMenuIGState == GSMIG_AlmanacIG)
	{
		mainMenuIGState = GSMIG_MainMenuIG;
	}
	else
	{
		mainMenuState = GSM_MainMenu;
	}		
}

void viewSelknamBtn_AlmanacMenu()
{
	curAlmanacState = ALMANAC_SELKNAM;
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpValue = "Place|Selknams";
		s3eFlurryLogEventParams("event_ShowAlmanac", tmpValue.c_str());
	}
#endif
}

void viewEnemyBtn_AlmanacMenu()
{
	curAlmanacState = ALMANAC_ENEMY;
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpValue = "Place|Enemies";
		s3eFlurryLogEventParams("event_ShowAlmanac", tmpValue.c_str());
	}
#endif
}

#if defined DESKTOPPLATFORM
void AlmanacMenu_Xbox360_A()
{
	almanacMenuRef->focusManager.ExecuteFocusedObject();
}

void AlmanacMenu_Xbox360_Right()
{
	almanacMenuRef->focusManager.ChangeFocus(XBOX360_Button_Right);
}

void AlmanacMenu_Xbox360_Left()
{
	almanacMenuRef->focusManager.ChangeFocus(XBOX360_Button_Left);
}

void AlmanacMenu_Xbox360_Down()
{
	almanacMenuRef->focusManager.ChangeFocus(XBOX360_Button_Down);
}

void AlmanacMenu_Xbox360_Up()
{
	almanacMenuRef->focusManager.ChangeFocus(XBOX360_Button_Up);
}
#endif

AlmanacMenu::AlmanacMenu()
{
	almanacMenuRef = this;

	pannelUnit = 40;
	pannelLengthX     = 7;
	pannelLengthY     = 7;
	pannelWidth       = pannelLengthX * pannelUnit + pannelUnit * 2;
	pannelHeight      = pannelLengthY * pannelUnit + pannelUnit * 2;
	pannelYPos        = (int)((currentGameHeight - pannelHeight) * 0.5f);

	int margin = 20;

	if(currentGameHeight <= 640)
		margin = 40;

	if(currentGameHeight <= 600)
		pannelYPos = pannelYPos - 25;

	pannelSelknamXPos = (int)(currentGameWidth * 0.33f - pannelWidth * 0.5f) - margin;
	pannelEnemyXPos   = (int)(currentGameWidth * 0.66f - pannelWidth * 0.5f) + margin;

	TextureData selknamIconData = TextureHelper::GetTextureNamed("selknam_warrior");
	selknamIcon = TextureHelper::GetRenderData(selknamIconData);
	selknamIconAtlasName = TextureHelper::GetAtlasName(selknamIconData);
	selknamIcon.width  = 210;
	selknamIcon.height = 210;
	selknamIconDimension = TextureHelper::GetDimension(selknamIcon);

	TextureData enemyIconData = TextureHelper::GetTextureNamed("enemy_soldier");
	enemyIcon = TextureHelper::GetRenderData(enemyIconData);
	enemyIconAtlasName = TextureHelper::GetAtlasName(enemyIconData);
	enemyIcon.width  = 250;
	enemyIcon.height = 250;
	enemyIconDimension = TextureHelper::GetDimension(enemyIcon);

	selknamIconXPos = (int)(pannelSelknamXPos + (pannelWidth - selknamIconDimension.x) * 0.5f); 
	selknamIconYPos = pannelYPos + pannelUnit;

	enemyIconXPos = (int)(pannelEnemyXPos + (pannelWidth - enemyIconDimension.x) * 0.5f); 
	enemyIconYPos = pannelYPos + pannelUnit - 30;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	CIwSVec2 buttonDimension = TextureHelper::GetDimension(buttonRenderData.state[0].Pressed);
	int btnTexWidth = buttonDimension.x;
	int btnTexHeight = buttonDimension.y;

	btnBackPosX = (int)((currentGameWidth - btnTexWidth) * 0.5f);
	btnBackPosY = currentGameHeight - Utils::GetScaledInt(110);

	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnBackPosX, btnBackPosY);
	btnBack->OnClick(&btnBackAction_AlmanacMenu);
	btnBack->SetText(loc_ui_back);

	int tempSelknamX = pannelSelknamXPos + (int)((pannelWidth - btnTexWidth) * 0.5f);
	int tempEnemyX   = pannelEnemyXPos   + (int)((pannelWidth - btnTexWidth) * 0.5f);
	int tmpY = pannelYPos + pannelHeight - btnTexHeight - 20;

	viewSelknamBtn = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, tempSelknamX, tmpY);
	viewSelknamBtn->OnClick(&viewSelknamBtn_AlmanacMenu);
	viewSelknamBtn->SetText(loc_ui_selknams);

	viewEnemiesBtn = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, tempEnemyX, tmpY);
	viewEnemiesBtn->OnClick(&viewEnemyBtn_AlmanacMenu);
	viewEnemiesBtn->SetText(loc_ui_enemies);

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(XBOX360_Button_A, &AlmanacMenu_Xbox360_A);
	xbox360Controller.SetCallback(XBOX360_Button_X, &AlmanacMenu_Xbox360_A);
	xbox360Controller.SetCallback(XBOX360_Button_B, &btnBackAction_AlmanacMenu);
	xbox360Controller.SetCallback(XBOX360_Button_Y, &btnBackAction_AlmanacMenu);
	xbox360Controller.SetCallback(XBOX360_Button_Down, &AlmanacMenu_Xbox360_Down);
	xbox360Controller.SetCallback(XBOX360_Button_Left, &AlmanacMenu_Xbox360_Left);
	xbox360Controller.SetCallback(XBOX360_Button_Right, &AlmanacMenu_Xbox360_Right);
	xbox360Controller.SetCallback(XBOX360_Button_Up, &AlmanacMenu_Xbox360_Up);

	focusManager.SetDefaultFocus(btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Down, viewSelknamBtn, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Down, viewEnemiesBtn, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Right, viewSelknamBtn, viewEnemiesBtn);
	focusManager.SetFocusDirection(XBOX360_Button_Left, viewEnemiesBtn, viewSelknamBtn);
	focusManager.SetFocusDirection(XBOX360_Button_Left, btnBack, viewSelknamBtn);
	focusManager.SetFocusDirection(XBOX360_Button_Right, btnBack, viewEnemiesBtn);
	focusManager.SetFocusDirection(XBOX360_Button_Up, btnBack, viewSelknamBtn);
#endif
}

AlmanacMenu::~AlmanacMenu()
{
	delete viewSelknamBtn;
	delete viewEnemiesBtn;
	delete btnBack;
}

void AlmanacMenu::InitObject()
{
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_AlmanacMenu, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_AlmanacMenu, NULL);

#if defined DESKTOPPLATFORM
	focusManager.UnableAllFocus();
	focusManager.SetInitialFocus(focusManager.GetDefaultFocus());
#endif
}

void AlmanacMenu::EndObject()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_AlmanacMenu);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_AlmanacMenu);
}

void AlmanacMenu::Render()
{
	Utils::RenderPannelA(pannelLengthX, pannelLengthY, pannelSelknamXPos, pannelYPos);
	Utils::RenderPannelA(pannelLengthX, pannelLengthY, pannelEnemyXPos, pannelYPos);

	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(selknamIconAtlasName), CIwSVec2(selknamIconXPos, selknamIconYPos), selknamIcon);
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(enemyIconAtlasName)  , CIwSVec2(enemyIconXPos  , enemyIconYPos)  , enemyIcon);

	viewSelknamBtn->Render();
	viewEnemiesBtn->Render();
	btnBack->Render();
}

void AlmanacMenu::UpdateClickEvents(s3ePointerEvent *event)
{
	viewSelknamBtn->HandleClick(event);
	viewEnemiesBtn->HandleClick(event);

	btnBack->HandleClick(event);
}

void AlmanacMenu::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	viewSelknamBtn->HandleMotion(event);
	viewEnemiesBtn->HandleMotion(event);

	btnBack->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

void AlmanacMenu::Update()
{
	if(popUpState != GSP_None) 
		return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}