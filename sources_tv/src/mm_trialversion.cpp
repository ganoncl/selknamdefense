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

#include "mm_trialversion.h"
#include "texturehelper.h"

TrialVersion* TrialVersionReference = NULL;

//Keyboard handler
int32 KeyboardHandlerTrialVersion(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	//if(!storeRef->bKeyboardBlocked)
		TrialVersionReference->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotionTrialVersion(s3ePointerMotionEvent* event) 
{
	if(TrialVersionReference == NULL)
		return;

	//if(!storeRef->bMouseBlocked)
		TrialVersionReference->UpdateMotionEvents(event);
}

//Mouse handle
void HandleClickTrialVersion(s3ePointerEvent *event) 
{
	if(TrialVersionReference == NULL)
		return;

	//if(!storeRef->bMouseBlocked)
		TrialVersionReference->UpdateClickEvents(event);
}

void TrialVersionContinueAction()
{
	TrialVersionReference->ContinueAction();
}

TrialVersion::TrialVersion()
{
	TrialVersionReference = this;
	menuTitle = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_title"));
	four = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_features"));
	FramesRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_maps_sample"));
	ThanksRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_thanks_trial"));
	TrialVersionFade = new FadeScreen();
	_Timer.SetTimeLimit(3000);
	bRenderFrames = false;
	ImagesFadeAmount = 0;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	ContinueButton = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 510, 610);
	ContinueButton->SetText("CONTINUE");
	ContinueButton->OnClick(&TrialVersionContinueAction);
}

TrialVersion::~TrialVersion()
{
	delete TrialVersionFade;
	delete ContinueButton;
}

void TrialVersion::InitObject()
{
	_Timer.Reset();
	_Timer.Start();
	TrialVersionFade->FadeOut(255);
	bRenderFrames = false;
	ContinueButton->IsFocused(true);
	ImagesFadeAmount = 0;

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTrialVersion, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerTrialVersion, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTrialVersion, NULL);
}

void TrialVersion::EndObject()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTrialVersion);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerTrialVersion);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTrialVersion);
}

void TrialVersion::Update()
{
	_Timer.Update();
	TrialVersionFade->Update();

	if(_Timer.GetIsEnd() && !_Timer.GetIsStop())
	{
		_Timer.Stop();
		TrialVersionFade->FadeIn(5, 175);
		bRenderFrames = true;
	}

	if(bRenderFrames)
	{
		ImagesFadeAmount += 5;
		if(ImagesFadeAmount > 255)
			ImagesFadeAmount = 255;
	}
}

void TrialVersion::Render()
{
	TrialVersionFade->Render();
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(menuTitle.atlasName), CIwSVec2((int)((1280 - menuTitle.width) * 0.5f) - 7,30), menuTitle);
	if(bRenderFrames)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(FramesRenderData.atlasName), FramesRenderData, CIwSVec2(30, 200), 255, 255, 255, ImagesFadeAmount);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(four.atlasName), four, CIwSVec2(840, 480), 255, 255, 255, ImagesFadeAmount);
		if(ImagesFadeAmount == 255)
			ContinueButton->Render();
	}
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(ThanksRenderData.atlasName), CIwSVec2(250, 220), ThanksRenderData);
}

void TrialVersion::UpdateClickEvents(s3ePointerEvent *event)
{
	if(ImagesFadeAmount == 255)
		ContinueButton->HandleClick(event);
}

void TrialVersion::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(ImagesFadeAmount == 255)
		ContinueButton->HandleMotion(event);
}

void TrialVersion::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(event->m_Pressed)
	{
		if(event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter)
		{
			if(ImagesFadeAmount == 255)
				mainMenuState = GSM_MainMenu;
		}
	}
}

void TrialVersion::ContinueAction()
{
	mainMenuState = GSM_MainMenu;
}