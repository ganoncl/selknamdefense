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

#include "common.h"
#include "localization.h"
#include "mm_credits.h"
#include "utils.h"
#include "texturehelper.h"

Credits* creditsRef = NULL;

//Mouse Motion
void HandleMotion_c(s3ePointerMotionEvent* event) 
{
	if(creditsRef == NULL)
		return;

	creditsRef->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClick_c(s3ePointerEvent *event) 
{
	if(creditsRef == NULL)
		return;

	creditsRef->UpdateClickEvents(event);
}

void btnBackAction_c()
{
	mainMenuState = GSM_Options;
}

#if defined DESKTOPPLATFORM
void Credits_Xbox360_A()
{
	creditsRef->focusManager.ExecuteFocusedObject();
}

void Credits_Xbox360_Directional()
{
	creditsRef->focusManager.ChangeFocus(XBOX360_Button_Left);
}
#endif

Credits::Credits()
{
	bIsActive = false;

	creditsRef = this;

	RenderPanelNumWidth = 18;
	RenderPanelNumHeight = 9;
	// Credits Pannel
	widthCreditsPannel = (RenderPanelNumWidth + 2) * 40;
	heightCreditsPannel = (RenderPanelNumHeight + 2) * 40;
	PannelPosX = (int)(currentGameWidth * 0.5f) - (int)(widthCreditsPannel * 0.5f);
	PannelPosY = (int)(currentGameHeight * 0.5f) - (int)(heightCreditsPannel * 0.5f) - Utils::GetScaledInt(30);

	creditsTitleYPos = PannelPosY + 35;
	badGamePeopleInitYPos = PannelPosY + 85;
	contactMailInitYPos = PannelPosY + 210;
	specialThanksTitleYPos = PannelPosY + 245 + (25 * 0);
	specialThanksPeople1YPos = PannelPosY + 245 + (25 * 1);
	specialThanksPeople2YPos = PannelPosY + 245 + (25 * 2);
	specialThanksPeople3YPos = PannelPosY + 245 + (25 * 3);
	textureMarmaladeYPos = PannelPosY + 340;
	madeWithMarmaladeYPos = PannelPosY + 370; 

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

	background = new Background(0, 0, 0, fade_menu);

	//Back button
	btnBackPosX = (int)((currentGameWidth - btnTexWidth) * 0.5f);
	btnBackPosY = currentGameHeight - Utils::GetScaledInt(110);
	numMembers = 5;

	rol[0] = loc_credits_title0;
	rol[1] = loc_credits_title1;
	rol[2] = loc_credits_title2;
	rol[3] = loc_credits_title3;
	rol[4] = loc_credits_title4;

	name[0] = loc_credits_name0;
	name[1] = loc_credits_name1;
	name[2] = loc_credits_name2;
	name[3] = loc_credits_name3;
	name[4] = loc_credits_name4;

	//Imagen2
	graphic_logo_marmalade = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_logo_marmalade"));
	
	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnBackPosX, btnBackPosY);
	btnBack->OnClick(&btnBackAction_c);

	//Text
	btnBack->SetText(loc_ui_back);

	//Mail text and rect
	strMail        = loc_credits_contact;
	rectMail       = CIwRect(0, contactMailInitYPos, currentGameWidth, 30);
	colorMail      = 0xff00edf8;

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(XBOX360_Button_A, &Credits_Xbox360_A);
	xbox360Controller.SetCallback(XBOX360_Button_X, &Credits_Xbox360_A);
	xbox360Controller.SetCallback(Xbox360_Action3, &Credits_Xbox360_A);
	xbox360Controller.SetCallback(XBOX360_Button_B, &btnBackAction_c);

	xbox360Controller.SetCallback(XBOX360_Button_Left, &Credits_Xbox360_Directional);
	xbox360Controller.SetCallback(XBOX360_Button_Right, &Credits_Xbox360_Directional);
	xbox360Controller.SetCallback(XBOX360_Button_Up, &Credits_Xbox360_Directional);
	xbox360Controller.SetCallback(XBOX360_Button_Down, &Credits_Xbox360_Directional);

	focusManager.SetDefaultFocus(btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Left, btnBack, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Right, btnBack, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Up, btnBack, btnBack);
	focusManager.SetFocusDirection(XBOX360_Button_Down, btnBack, btnBack);
#endif
}

Credits::~Credits()
{
	delete btnBack;
	delete background;
	creditsRef = NULL;
}

void Credits::Render()
{
	background->Render();

	Utils::RenderPannelA(RenderPanelNumWidth, RenderPanelNumHeight, PannelPosX, PannelPosY);

	IwGxLightingOn();
	Utils::RenderText(loc_ui_credits, CIwRect(PannelPosX, creditsTitleYPos, widthCreditsPannel, 50), font_komikadisplaybold25, 0xff00edf8, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP, true, 2); 
	int space_y = 25;
	
	for (int i = 0; i < numMembers; i++)
	{
		Utils::RenderText(rol[i] , CIwRect(PannelPosX, badGamePeopleInitYPos + i * space_y,  (int)(widthCreditsPannel * 0.5f), 300), font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP, true, 1); 
	}	

	for(int i = 0; i < numMembers; i++)
		Utils::RenderText(name[i] , CIwRect(PannelPosX + (int)(widthCreditsPannel * 0.5f) + 40, badGamePeopleInitYPos + i * space_y, (int)(widthCreditsPannel * 0.5f) - 50, 300), 
									font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP, true, 1); 
	
	
	IwGxFlush();

	//Contact mail.
	Utils::RenderText(strMail.c_str(),
		              rectMail,
					  font_komikadisplay14,
					  colorMail,
					  IW_GX_FONT_ALIGN_CENTRE,
					  IW_GX_FONT_ALIGN_MIDDLE,
					  true,
					  2);

	Utils::RenderText(loc_credits_title5 , CIwRect(PannelPosX, 
		                                         specialThanksTitleYPos, 
												 widthCreditsPannel, 300), 
		              font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP, true, 1); 
	Utils::RenderText(loc_credits_name5 , CIwRect(PannelPosX, 
		specialThanksPeople1YPos, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP, true, 1); 
	Utils::RenderText(loc_credits_name6 , CIwRect(PannelPosX, 
		specialThanksPeople2YPos, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP, true, 1); 
	Utils::RenderText(loc_credits_name7 , CIwRect(PannelPosX, 
		specialThanksPeople3YPos, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP, true, 1); 


	Utils::RenderText(loc_credits_marmalade , CIwRect(PannelPosX + 230 + 95, 
		madeWithMarmaladeYPos, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP, true, 1); 
	IwGxLightingOff();
	
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(graphic_logo_marmalade.atlasName), CIwSVec2(PannelPosX + 230, textureMarmaladeYPos), graphic_logo_marmalade);

	//Buttons
	btnBack->Render();
}

void Credits::On()
{
	bIsActive = true;

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_c, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_c, NULL);

#if defined DESKTOPPLATFORM
	focusManager.UnableAllFocus();
	focusManager.SetInitialFocus(btnBack);
#endif
}

void Credits::Off()
{
	bIsActive = false;

	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_c);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_c);

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(NULL);
#endif
}

void Credits::UpdateClickEvents(s3ePointerEvent *event)
{
	btnBack->HandleClick(event);
}

void Credits::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	btnBack->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

void Credits::Update()
{
	if(!bIsActive)
		return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}
