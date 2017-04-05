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

#include "mm_final.h"
#include "texturehelper.h"

FinalScreen* FinalScreenReference = NULL;

//Keyboard handler
int32 KeyboardHandlerFinalScreen(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	//if(!storeRef->bKeyboardBlocked)
	FinalScreenReference->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotionFinalScreen(s3ePointerMotionEvent* event) 
{
	if(FinalScreenReference == NULL)
		return;

	//if(!storeRef->bMouseBlocked)
	FinalScreenReference->UpdateMotionEvents(event);
}

//Mouse handle
void HandleClickFinalScreen(s3ePointerEvent *event) 
{
	if(FinalScreenReference == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
			{
				FinalScreenReference->MouseHack();
			}
			else
			{
				FinalScreenReference->ActiveButtonMouseHack();
			}
		}

		return;
	}

	//if(!storeRef->bMouseBlocked)
	FinalScreenReference->UpdateClickEvents(event);
}

void FinalScreenContinueAction()
{
	FinalScreenReference->ContinueAction();
}

void FinalScreenCreditsAction()
{
	FinalScreenReference->CreditsAction();
}

FinalScreen::FinalScreen()
{
	FinalScreenReference = this;
	thankData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_thanks_full"));
	menuTitle = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_title"));
	FinalScreenFade = new FadeScreen();
	_Timer.SetTimeLimit(3000);
	bRenderFrames = false;


	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	ContinueButton = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 280, 510);
	ContinueButton->SetText("CONTINUE");
	ContinueButton->OnClick(&FinalScreenContinueAction);
	CreditsButton = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 680, 510);
	CreditsButton->SetText("CREDITS");
	CreditsButton->OnClick(&FinalScreenCreditsAction);
	bKeyboard = true;
	bMouse = true;
	focus = FSFocus_None;
	lastKeyPressed = s3eKeyFirst;
}

FinalScreen::~FinalScreen()
{
	ThanksTexture = NULL;
	delete FinalScreenFade;
	delete ContinueButton;
	delete CreditsButton;
}

void FinalScreen::InitObject()
{
	_Timer.Reset();
	_Timer.Start();
	FinalScreenFade->FadeOut(255);
	bRenderFrames = false;
	ImagesFadeAmount = false;
	bKeyboard = true;
	bMouse = true;
	ContinueButton->IsFocused(false);
	CreditsButton->IsFocused(false);

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionFinalScreen, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerFinalScreen, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickFinalScreen, NULL);

	focus = FSFocus_None;
	SetFocus(FSFocus_Credits);

	lastKeyPressed = s3eKeyFirst;
}

void FinalScreen::EndObject()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionFinalScreen);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerFinalScreen);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickFinalScreen);
}

void FinalScreen::Update()
{
	_Timer.Update();
	FinalScreenFade->Update();

	if(_Timer.GetIsEnd() && !_Timer.GetIsStop())
	{
		_Timer.Stop();
		FinalScreenFade->FadeIn(5, 175);
		bRenderFrames = true;
	}

	if(bRenderFrames)
	{
		ImagesFadeAmount += 5;
		if(ImagesFadeAmount > 255)
			ImagesFadeAmount = 255;
	}
}

void FinalScreen::Render()
{
	FinalScreenFade->Render();
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(menuTitle.atlasName), CIwSVec2((int)((1280 - menuTitle.width) * 0.5f) - 7,30), menuTitle);
	if(bRenderFrames)
	{
		if(ImagesFadeAmount == 255)
		{
			ContinueButton->Render();
			CreditsButton->Render();
		}
	}
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(thankData.atlasName), CIwSVec2(250, 220), thankData);
}

void FinalScreen::UpdateClickEvents(s3ePointerEvent *event)
{
	if(!bMouse)
		return;

	if(event->m_Pressed)
	{
		bKeyboard = false;
	}
	else
	{
		bKeyboard = true;
	}

	if(ImagesFadeAmount == 255)
	{
		ContinueButton->HandleClick(event);
		CreditsButton->HandleClick(event);
	}
}

void FinalScreen::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!bMouse)
		return;

	if(ImagesFadeAmount == 255)
	{
		ContinueButton->HandleMotion(event);

		if(ContinueButton->MouseOn(event->m_x, event->m_y))
		{
			SetFocus(FSFocus_Continue);
		}

		CreditsButton->HandleMotion(event);
		
		if(CreditsButton->MouseOn(event->m_x, event->m_y))
		{
			SetFocus(FSFocus_Credits);
		}
	}
}

void FinalScreen::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(ImagesFadeAmount != 255)
		return;

	if(!bKeyboard)
		return;

	if(event->m_Pressed)
	{
		bMouse = false;
	}
	else
	{
		bMouse = true;
	}

	if(event->m_Pressed)
	{
		if(event->m_Key == s3eKeyAbsRight)
		{
			SetFocus(FSFocus_Credits);
		}
		else if(event->m_Key == s3eKeyAbsLeft)
		{
			SetFocus(FSFocus_Continue);
		}
		else if(event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter)
		{
			if(focus == FSFocus_Continue)
				ContinueButton->SetStatus(3);
			else if(focus == FSFocus_Credits)
				CreditsButton->SetStatus(3);
		}
		lastKeyPressed = event->m_Key;
	}
	else
	{
		if(lastKeyPressed == s3eKeyAbsOk || lastKeyPressed == s3eKeyEnter)
		{
			if(focus == FSFocus_Continue)
				mainMenuState = GSM_MainMenu;
			else if(focus == FSFocus_Credits)
				mainMenuState = GSM_Credits;
		}
		lastKeyPressed = s3eKeyFirst;
	}
}

void FinalScreen::SetFocus(FSFocus newFocus)
{
	if(focus == newFocus)
	{
		return;
	}

	if(focus == FSFocus_Continue)
		ContinueButton->IsFocused(false);
	else if(focus == FSFocus_Credits)
		CreditsButton->IsFocused(false);

	if(newFocus == FSFocus_Continue)
		ContinueButton->IsFocused(true);
	else if(newFocus == FSFocus_Credits)
		CreditsButton->IsFocused(true);

	focus = newFocus;
}

void FinalScreen::ContinueAction()
{
	mainMenuState = GSM_MainMenu;
}

void FinalScreen::CreditsAction()
{
	mainMenuState = GSM_Credits;
}

void FinalScreen::MouseHack()
{
	if(focus == FSFocus_Continue)
		mainMenuState = GSM_MainMenu;
	else if(focus == FSFocus_Credits)
		mainMenuState = GSM_Credits;
}

void FinalScreen::ActiveButtonMouseHack()
{
	if(focus == FSFocus_Continue)
		ContinueButton->SetStatus(3);
	else if(focus == FSFocus_Credits)
		CreditsButton->SetStatus(3);
}