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

	FinalScreenReference->UpdateClickEvents(event);
}

void FinalScreenContinueAction()
{
	FinalScreenReference->ContinueAction();
}

FinalScreen::FinalScreen()
{
	bIsActive = false;

	FinalScreenReference = this;
	thankData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_thanks_full"));
	thankData.width = Utils::GetScaledInt(thankData.width);
	thankData.height = Utils::GetScaledInt(thankData.height);
	menuTitle = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_title"));
	FinalScreenFade = new FadeScreen();
	_Timer.SetTimeLimit(3000);
	bRenderFrames = false;

	thanksXPos = Utils::GetFixedScaledInt(180);

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	ContinueButton = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, Utils::GetFixedScaledInt(340), Utils::GetScaledInt(510));
	ContinueButton->SetText(loc_ui_continue);
	ContinueButton->OnClick(&FinalScreenContinueAction);

	bKeyboard = true;
	bMouse = true;
	lastKeyPressed = s3eKeyFirst;

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, &FinalScreenContinueAction);
	xbox360Controller.SetCallback(Xbox360_Action2, &FinalScreenContinueAction);
	xbox360Controller.SetCallback(Xbox360_Action3, &FinalScreenContinueAction);
#endif
}

FinalScreen::~FinalScreen()
{
	delete FinalScreenFade;
	delete ContinueButton;
}

void FinalScreen::InitObject()
{
	bIsActive = true;
	_Timer.Reset();
	_Timer.Start();
	FinalScreenFade->FadeOut(255);
	bRenderFrames = false;
	ImagesFadeAmount = false;
	bKeyboard = true;
	bMouse = true;
	ContinueButton->IsFocused(false);

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionFinalScreen, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickFinalScreen, NULL);

	lastKeyPressed = s3eKeyFirst;

	fade->FadeOut();

#if defined DESKTOPPLATFORM
	focusManager.SetInitialFocus(ContinueButton);
#endif
}

void FinalScreen::EndObject()
{
	bIsActive = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionFinalScreen);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickFinalScreen);

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(NULL);
#endif
}

void FinalScreen::Update()
{
	if(!bIsActive)
		return;

#if defined DESKTOPPLATFORM
	if(ImagesFadeAmount == 255)
		xbox360Controller.Update();
#endif

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
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(menuTitle.atlasName), CIwSVec2((int)((currentGameWidth - menuTitle.width) * 0.5f), 30), menuTitle);
	if(bRenderFrames)
	{
		if(ImagesFadeAmount == 255)
		{
			ContinueButton->Render();
		}
	}

	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(thankData.atlasName), CIwSVec2(thanksXPos, 220), thankData);
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
	}
}

void FinalScreen::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!bMouse)
		return;

	if(ImagesFadeAmount == 255)
	{
		ContinueButton->HandleMotion(event);
	}
}

void FinalScreen::ContinueAction()
{
	mainMenuState = GSM_MainMenu;
}
