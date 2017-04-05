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
#include "localization.h"
#include "texturehelper.h"

TrialVersion* TrialVersionReference = NULL;

//Keyboard handler
int32 KeyboardHandlerTrialVersion(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	TrialVersionReference->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotionTrialVersion(s3ePointerMotionEvent* event) 
{
	if(TrialVersionReference == NULL)
		return;

	TrialVersionReference->UpdateMotionEvents(event);
}

//Mouse handle
void HandleClickTrialVersion(s3ePointerEvent *event) 
{
	if(TrialVersionReference == NULL)
		return;

	TrialVersionReference->UpdateClickEvents(event);
}

void TrialVersionContinueAction()
{
	TrialVersionReference->ContinueAction();
}

TrialVersion::TrialVersion()
{
	bIsActive = false;

	TrialVersionReference = this;
	menuTitleRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_title"));
	menuTitleRenderData.width = Utils::GetScaledInt(menuTitleRenderData.width);
	menuTitleRenderData.height = Utils::GetScaledInt(menuTitleRenderData.height);

	featuresRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_features"));
	featuresRenderData.width = Utils::GetScaledInt(featuresRenderData.width);
	featuresRenderData.height = Utils::GetScaledInt(featuresRenderData.height);

	framesRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_maps_sample"));
	framesRenderData.width = (int)((float)Utils::GetScaledInt(framesRenderData.width) * 0.7f);
	framesRenderData.height = (int)((float)Utils::GetScaledInt(framesRenderData.height) * 0.7f);

	thanksRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_thanks_trial"));
	thanksRenderData.width = Utils::GetScaledInt(thanksRenderData.width);
	thanksRenderData.height = Utils::GetScaledInt(thanksRenderData.height);

	menuTitlePos = CIwSVec2((int)((currentGameWidth - menuTitleRenderData.width) * 0.5f), Utils::GetScaledInt(30));
	thanksPos = CIwSVec2(Utils::GetFixedScaledInt(180), Utils::GetScaledInt(170));
	framesPos = CIwSVec2(Utils::GetFixedScaledInt(30), Utils::GetScaledInt(315));
	featuresPos = CIwSVec2(Utils::GetFixedScaledInt(700), Utils::GetScaledInt(450));

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

	ContinueButton = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, Utils::GetFixedScaledInt(340), currentGameHeight - Utils::GetScaledInt(110));
	ContinueButton->SetText(loc_ui_continue);
	ContinueButton->OnClick(&TrialVersionContinueAction);

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, &TrialVersionContinueAction);
	xbox360Controller.SetCallback(Xbox360_Action2, &TrialVersionContinueAction);
	xbox360Controller.SetCallback(Xbox360_Action3, &TrialVersionContinueAction);
#endif
}

TrialVersion::~TrialVersion()
{
	delete TrialVersionFade;
	delete ContinueButton;
}

void TrialVersion::InitObject()
{
	bIsActive = true;

	_Timer.Reset();
	_Timer.Start();
	TrialVersionFade->FadeOut(255);
	bRenderFrames = false;
	ImagesFadeAmount = 0;

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTrialVersion, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerTrialVersion, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTrialVersion, NULL);

#if defined DESKTOPPLATFORM
	focusManager.SetInitialFocus(ContinueButton);
#endif
}

void TrialVersion::EndObject()
{
	bIsActive = false;

	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTrialVersion);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerTrialVersion);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTrialVersion);

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(NULL);
#endif
}

void TrialVersion::Update()
{
	if(!bIsActive)
		return;

#if defined DESKTOPPLATFORM
	if(ImagesFadeAmount == 255)
		xbox360Controller.Update();
#endif

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
	if(!bIsActive)
		return;

	TrialVersionFade->Render();
	Utils::RenderSingleTexture(atlas_bkg_menu_others, menuTitlePos, menuTitleRenderData);
	IwGxFlush();
	if(bRenderFrames)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(framesRenderData.atlasName), framesRenderData, framesPos, 255, 255, 255, ImagesFadeAmount);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(featuresRenderData.atlasName), featuresRenderData, featuresPos, 255, 255, 255, ImagesFadeAmount);
		if(ImagesFadeAmount == 255)
			ContinueButton->Render();
	}
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(thanksRenderData.atlasName), thanksPos, thanksRenderData);
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