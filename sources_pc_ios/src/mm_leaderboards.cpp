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

#include "mm_leaderboards.h"
#include "localization.h"

MenuLeaderboards* menuLeaderboardsReference = NULL;

//Mouse Motion
void MenuLeaderboards_HandleMotion(s3ePointerMotionEvent* event) 
{
	if(menuLeaderboardsReference == NULL)
		return;

	menuLeaderboardsReference->UpdateMotionEvents(event);
}


//Mouse handle
void MenuLeaderboards_HandleClick(s3ePointerEvent *event) 
{
	if(menuLeaderboardsReference == NULL)
		return;

	menuLeaderboardsReference->UpdateClickEvents(event);
}

void MenuLeaderboards_Back()
{
	mainMenuState = GSM_MainMenu;
}

void MenuLeaderboards_OnListLoaded()
{
	menuLeaderboardsReference->OnLoadedList();
}

#if defined DESKTOPPLATFORM
void MenuLeaderboards_XBOX360_A()
{
	menuLeaderboardsReference->focusManager.ExecuteFocusedObject();
}

void MenuLeaderboards_XBOX360_Up()
{
	menuLeaderboardsReference->focusManager.ChangeFocus(XBOX360_Button_Up);
}

void MenuLeaderboards_XBOX360_Down()
{
	menuLeaderboardsReference->focusManager.ChangeFocus(XBOX360_Button_Down);
}

void MenuLeaderboards_XBOX360_Left()
{
	menuLeaderboardsReference->focusManager.ChangeFocus(XBOX360_Button_Left);
}

void MenuLeaderboards_XBOX360_Right()
{
	menuLeaderboardsReference->focusManager.ChangeFocus(XBOX360_Button_Right);
}
#endif

MenuLeaderboards::MenuLeaderboards()
{
	menuLeaderboardsReference = this;

	SetPannelProperties();

	lboardList = new LboardList(xPannel, yPannel);
	
	titleXPos = (int)(currentGameWidth * 0.5f) - 224;//Utils::GetFixedScaledInt(260);
	titleYPos = Utils::GetScaledInt(-50);

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

	//Back button
	btnBackPosX = (int)((currentGameWidth - btnTexWidth) * 0.5f);
	btnBackPosY = currentGameHeight - Utils::GetScaledInt(110);

	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnBackPosX, btnBackPosY);
	btnBack->OnClick(&MenuLeaderboards_Back);

	//Text
	btnBack->SetText(loc_ui_back);

	label_star_big = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_star_big"));
	label_star_big.width = (int)(label_star_big.width * 0.75f);
	label_star_big.height = (int)(label_star_big.height * 0.75f);

	lboardList->SetOnLoadListCallback(&MenuLeaderboards_OnListLoaded);

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(XBOX360_Button_Up, &MenuLeaderboards_XBOX360_Up);
	xbox360Controller.SetCallback(XBOX360_Button_Down, &MenuLeaderboards_XBOX360_Down);
	xbox360Controller.SetCallback(XBOX360_Button_Left, &MenuLeaderboards_XBOX360_Left);
	xbox360Controller.SetCallback(XBOX360_Button_Right, &MenuLeaderboards_XBOX360_Right);
	xbox360Controller.SetCallback(XBOX360_Button_A, &MenuLeaderboards_XBOX360_A);
	xbox360Controller.SetCallback(XBOX360_Button_X, &MenuLeaderboards_XBOX360_A);
	xbox360Controller.SetCallback(XBOX360_Button_B, &MenuLeaderboards_Back);
	xbox360Controller.SetCallback(XBOX360_Button_Y, &MenuLeaderboards_Back);
#endif
}

MenuLeaderboards::~MenuLeaderboards()
{
	delete btnBack;
	delete lboardList;
}

void MenuLeaderboards::SetPannelProperties()
{
	RenderPanelNumWidth = 18;
	RenderPanelNumHeight = 9;
	widthCreditsPannel = (RenderPanelNumWidth + 2) * 40;
	heightCreditsPannel = (RenderPanelNumHeight + 2) * 40;
	xPannel = (int)(currentGameWidth * 0.5f) - (int)(widthCreditsPannel * 0.5f);
	yPannel = (int)(currentGameHeight * 0.5f) - (int)(heightCreditsPannel * 0.5f) + 30 - Utils::GetScaledInt(30);
}

void MenuLeaderboards::InitObject()
{
	SetIsActive(true);
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)MenuLeaderboards_HandleMotion, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)MenuLeaderboards_HandleClick, NULL);

#ifdef STEAMREQUIRED
	if(g_SteamLeaderboards == NULL)
	{
		popUpState = GSP_LEADERBOARD_ERROR;
	}
#endif

	lboardList->LoadLeaderboard(FilterLBName_Campaign);

#if defined DESKTOPPLATFORM
	//focusManager.ClearFocusDirections();
	focusManager.ChangeFocus(btnBack);
	/*
	focusManager.SetFocusDirection(XBOX360_Button_Up, btnBack, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Down, btnBack, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Left, btnBack, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Right, btnBack, btnBack);
	*/
#endif
}

void MenuLeaderboards::EndObject()
{
	SetIsActive(false);
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)MenuLeaderboards_HandleMotion);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)MenuLeaderboards_HandleClick);
}

void MenuLeaderboards::Render()
{
	if(!GetIsActive()) return;

	if(currentGameHeight >= 640)
	{
		Utils::RenderLabelTitle(10, titleXPos, titleYPos);

		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_star_big.atlasName), CIwSVec2(titleXPos + 25, titleYPos + 100), label_star_big);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_star_big.atlasName), CIwSVec2(titleXPos + 365, titleYPos + 100), label_star_big);
	}

	lboardList->Render();

	IwGxLightingOn();
	if(currentGameHeight >= 640)
		Utils::RenderText(loc_leaderboards_title, CIwRect(titleXPos + 5, titleYPos + 95, 438, 66), font_komikadisplaybold25, 0xff00d9fc);
	IwGxLightingOff();

	//Buttons
	btnBack->Render();
}

void MenuLeaderboards::Update()
{
	if(popUpState != GSP_None) return;

	lboardList->Update();

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}

void MenuLeaderboards::UpdateClickEvents(s3ePointerEvent *event)
{
	if(popUpState != GSP_None) return;
	
	lboardList->UpdateClickEvents(event);

	btnBack->HandleClick(event);
}

void MenuLeaderboards::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(popUpState != GSP_None) return;

	lboardList->UpdateMotionEvents(event);

	btnBack->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

void MenuLeaderboards::OnLoadedList()
{
#if defined DESKTOPPLATFORM
	focusManager.ClearFocusDirections();

	int size = lboardList->GetEntryListSize();

	for(int i = 0; i < size - 1; i++)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Down, lboardList->GetEntryAt(i), lboardList->GetEntryAt(i + 1));
	}

	for(int i = size - 1; i > 0; i--)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Up, lboardList->GetEntryAt(i), lboardList->GetEntryAt(i - 1));
	}

	focusManager.SetFocusDirection(XBOX360_Button_Up, lboardList->GetGUILBNameFilter(), lboardList->GetGUIFilter());
	focusManager.SetFocusDirection(XBOX360_Button_Down, lboardList->GetGUIFilter(), lboardList->GetGUILBNameFilter());

	if(size > 0)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Up, btnBack, lboardList->GetEntryAt(size - 1));
		focusManager.SetFocusDirection(XBOX360_Button_Down, lboardList->GetEntryAt(size - 1), btnBack);
		focusManager.SetFocusDirection(XBOX360_Button_Up, lboardList->GetEntryAt(0), lboardList->GetGUILBNameFilter());
		focusManager.SetFocusDirection(XBOX360_Button_Down, lboardList->GetGUILBNameFilter(), lboardList->GetEntryAt(0));
	}
	else
	{
		focusManager.SetFocusDirection(XBOX360_Button_Up, btnBack, lboardList->GetGUILBNameFilter());
		focusManager.SetFocusDirection(XBOX360_Button_Down, lboardList->GetGUILBNameFilter(), btnBack);
	}

	if(lboardList->ExistsNextPage())
	{
		for(int i = 0; i < size; i++)
		{
			focusManager.SetFocusDirection(XBOX360_Button_Right, lboardList->GetEntryAt(i), lboardList->GetNextPageButton());
		}

		focusManager.SetFocusDirection(XBOX360_Button_Right, btnBack, lboardList->GetNextPageButton());
	}

	if(lboardList->ExistsPrevPage())
	{
		for(int i = 0; i < size; i++)
		{
			focusManager.SetFocusDirection(XBOX360_Button_Left, lboardList->GetEntryAt(i), lboardList->GetPrevPageButton());
		}

		focusManager.SetFocusDirection(XBOX360_Button_Left, btnBack, lboardList->GetPrevPageButton());
	}

#endif
}
