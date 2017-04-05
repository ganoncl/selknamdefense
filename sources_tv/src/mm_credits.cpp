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
#include "mm_credits.h"
#include "utils.h"

Credits* cRef = NULL;

int32 KeyboardHandler_c(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(!cRef->bKeyboardBlocked)
		cRef->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotion_c(s3ePointerMotionEvent* event) 
{
	if(cRef == NULL)
		return;

	if(!cRef->bMouseBlocked)
		cRef->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClick_c(s3ePointerEvent *event) 
{
	if(cRef == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
			{
				cRef->MouseHack();
			}
			else
			{
				cRef->ActiveButtonMouseHack();
			}
		}
	}

	if(!cRef->bMouseBlocked)
		cRef->UpdateClickEvents(event);
}

void btnBackAction_c()
{
	mainMenuState = GSM_MainMenu;
}

void btnBackMotion_c()
{
	cRef->ChangeFocus(C_FOCUS_BACK);
}

Credits::Credits()
{
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

	background = new Background(0,0,0,fade_menu);

	int screenH = IwGxGetScreenHeight();
	
	//Settings
	//Container
	containerW = 960;
	containerH = 480;
	containerX = 100;
	containerY = 100;
	uint32 containerColor = 0xffffffff;
	//Back button
	btnBackPosX = (int)((IwGxGetScreenWidth() - btnTexWidth) * 0.5f);
	btnBackPosY = IwGxGetScreenHeight() - (int)(btnTexHeight) - 10;
	//title
	screenTitle = "Credits";
	titleX = containerX + (int)((containerW - 20) * 0.5f);
	titleY = containerY + 10;
	titleW = containerW;
	titleH = 50;
	titleColor = 0xff0000ae; //If you want to use 0xae0000 for example, then you need to write titleColor = 0xff0000ae
	//text
	textColor = 0xff000000;
	numMembers = 5;

	rol[0] = "Team Lead & Programmer";
	rol[1] = "Game Design / Art";
	rol[2] = "Game Design / Concept";
	rol[3] = "Senior Programmer";
	rol[4] = "Music Production / Composer";

	name[0] = "Sebastian Gana";
	name[1] = "Rodrigo Alarcon";
	name[2] = "Walter Berendsen";
	name[3] = "Javier Gonzalez";
	name[4] = "Matias Castro";
	//Imagen1
	widthCreditsPannel = 20 * 40;
	img1X = (int)(IwGxGetScreenWidth() * 0.5f) - (int)(widthCreditsPannel * 0.5f);
	img1Y = (int)(IwGxGetScreenHeight() * 0.5f) - (int)((13 * 40) * 0.5f);
	//Imagen2
	graphic_logo_marmalade = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_logo_marmalade"));
	img2X = containerX + widthCreditsPannel + 250;
	img2Y = containerY + 150 + titleH + 10;
	//End Settings
	
	int img1W = widthCreditsPannel;
	int img1H = widthCreditsPannel;
	int img2W = graphic_logo_marmalade.height;
	int img2H = graphic_logo_marmalade.width;

	img1Pos[0].x = img1X;              img1Pos[0].y = img1Y;
	img1Pos[1].x = img1X;              img1Pos[1].y = img1Y + img1H;
	img1Pos[2].x = img1X + img1W;	   img1Pos[2].y = img1Y + img1H;
	img1Pos[3].x = img1X + img1W;      img1Pos[3].y = img1Y;

	img2Pos[0].x = img2X;              img2Pos[0].y = img2Y;
	img2Pos[1].x = img2X;              img2Pos[1].y = img2Y + img2H;
	img2Pos[2].x = img2X + img2W;	   img2Pos[2].y = img2Y + img2H;
	img2Pos[3].x = img2X + img2W;      img2Pos[3].y = img2Y;

	s_UV[0] = CIwFVec2(0, 0);
	s_UV[1] = CIwFVec2(0, 1);
	s_UV[2] = CIwFVec2(1, 1);
	s_UV[3] = CIwFVec2(1, 0);

	xy3[0].x = containerX;              xy3[0].y = containerY;
	xy3[1].x = containerX;              xy3[1].y = containerY + containerH;
	xy3[2].x = containerX + containerW; xy3[2].y = containerY + containerH;
	xy3[3].x = containerX + containerW; xy3[3].y = containerY;
	cols[0].Set(containerColor);
	cols[1].Set(containerColor);
	cols[2].Set(containerColor);
	cols[3].Set(containerColor);
	
	pfont = font_komikadisplaybold25;
	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnBackPosX, btnBackPosY);
	btnBack->OnClick(&btnBackAction_c);
	btnBack->OnRollOn(&btnBackMotion_c);

	cRef = this;

	lastKeyPressed = s3eKeyFirst;
	bMouseBlocked = false;
	bKeyboardBlocked = true;

	//Text
	btnBack->SetText("BACK");

	//Mail text and rect
	strMail        = "Contact us: contact@bad-games.com";
	rectMail       = CIwRect(0, containerY + 245, 1280, 30);
	colorMail      = 0xff00edf8;
	bHasShadowMail = true;;
}

Credits::~Credits()
{
	delete btnBack;
	delete background;
	cRef = NULL;
}

void Credits::Render()
{
	background->Render();
	Utils::RenderPannelA(18, 11, img1X, img1Y);

	IwGxLightingOn();
	Utils::RenderText("CREDITS", CIwRect(img1X, img1Y, widthCreditsPannel, 140), font_komikadisplaybold25, 0xff00edf8, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true, 2); 
	int space_y = 25;
	
	for (int i = 0; i < numMembers; i++)
	{
		Utils::RenderText(rol[i] , CIwRect(img1X, img1Y + 120 + i * space_y,  (int)(widthCreditsPannel * 0.5f), 300), font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP); 
	}	

	for(int i = 0; i < numMembers; i++)
		Utils::RenderText(name[i] , CIwRect(img1X + (int)(widthCreditsPannel * 0.5f) + 40, img1Y + 120 + i * space_y, (int)(widthCreditsPannel * 0.5f) - 50, 300), 
									font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP); 
	
	
	IwGxFlush();

	//Contact mail.
	Utils::RenderText(strMail.c_str(),
		              rectMail,
					  font_komikadisplay14,
					  colorMail,
					  IW_GX_FONT_ALIGN_CENTRE,
					  IW_GX_FONT_ALIGN_MIDDLE,
					  bHasShadowMail,
					  1);

	int offsetY = 10;
	Utils::RenderText("SPECIAL THANKS" , CIwRect(img1X, 
		                                         370 + offsetY, 
												 widthCreditsPannel, 300), 
		              font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP); 
	Utils::RenderText("Leo Benaducci" , CIwRect(img1X, 
		395 + offsetY, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP); 
	Utils::RenderText("Luis Santander" , CIwRect(img1X, 
		420 + offsetY, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP); 
	Utils::RenderText("ACE Team members" , CIwRect(img1X, 
		445 + offsetY, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP); 


	Utils::RenderText("wwww.madewithmarmalade.com" , CIwRect(img1X, 
		555 + offsetY, 
		widthCreditsPannel, 300), 
		font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP); 
	IwGxLightingOff();
	
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(graphic_logo_marmalade.atlasName), CIwSVec2(img1X + (int)(widthCreditsPannel * 0.5f) - (int)(graphic_logo_marmalade.height * 0.5f), 475 + offsetY), graphic_logo_marmalade);

	//Buttons
	btnBack->Render();
}

void Credits::On()
{
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_c, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_c, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_c, NULL);
	btnBack->IsFocused(true);
	focus = C_FOCUS_BACK;
	bMouseBlocked = false;
	bKeyboardBlocked = false;
}

void Credits::Off()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_c);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_c);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_c);
}

void Credits::UpdateClickEvents(s3ePointerEvent *event)
{
	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	btnBack->HandleClick(event);
}

void Credits::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	btnBack->HandleMotion(event);
}

void Credits::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(event->m_Pressed)
	{
		bMouseBlocked = true;
		lastKeyPressed = event->m_Key;
		if(event->m_Key == s3eKeyEsc || event->m_Key == s3eKeyBack) mainMenuState = GSM_MainMenu;
		else if(event->m_Key == s3eKeyAbsDown)
		{
			btnBack->IsFocused(true);
			focus = C_FOCUS_BACK;
		}
		else if(event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter)
		{
			if(focus == C_FOCUS_BACK)
				btnBack->SetStatus(3);
		}
		else
		{
			if(focus == C_FOCUS_BACK)
				btnBack->SetStatus(1);
		}
	}
	else
	{
		if((lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter) && focus == C_FOCUS_BACK)
		{
			btnBack->Execute();
		}
		bMouseBlocked = false;
	}
}

void Credits::ChangeFocus(CreditsFocus newFocus)
{
	if(newFocus == C_FOCUS_BACK) btnBack->IsFocused(true);
	focus = newFocus;
}

void Credits::MouseHack()
{
	btnBack->Execute();
}

void Credits::ActiveButtonMouseHack()
{
	if(focus == C_FOCUS_BACK)
		btnBack->SetStatus(3);
}