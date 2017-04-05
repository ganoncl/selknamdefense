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

#include "s3e.h"
#include "s3eOSReadString.h"
#include "IwGx.h"
#include "stdlib.h"
#include "time.h"
#include "stdio.h"
#include "IwColour.h"
#include "s3ePointer.h"
#include "parserhelper.h"
#include "savedatahelper.h"
#include "common.h"
#include "splashscreen.h"
#include "utils.h"
#include "IwGx.h"

SplashScreen *sScreenRef = NULL;

void SplashScreen_SkipImage()
{
	if(!sScreenRef)
		return;

	sScreenRef->SkipImage();
}

//need to be outside from class.
void HandleClickBtn(s3ePointerMotionEvent* event) 
{
	if(!sScreenRef)
		return;

	sScreenRef->SkipImage();
	//IwTrace(ASDFG, ("MOUSE pOS Xpos %i, Ypos %i", xMouse, yMouse));
#ifdef CHEATSENABLED
	//can enable cheats.
	if(xMouse>=0 && xMouse<50 && yMouse>=0 && yMouse<50)
	{
		gameDataCampaign.SetGameDataCampaignFullProgress();
		saveDataHelper->SaveGameDataCampaign();
		gameDataSurvival.SetGameDataSurvivalAvailable();
		saveDataHelper->SaveGameDataSurvival();
	}
#endif
}

int32 KeyboardHandler_splashscreen(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;
	if(event->m_Pressed)
	{
		if(!sScreenRef)
			return 0;
		sScreenRef->SkipImage();
	}

	return 0;
}

//delay: Tiempo entre imagenes. (Segundos)
//Skip: Tiempo minimo entre imagenes. (Segundos)
SplashScreen::SplashScreen(uint8 delay, uint8 skip, int fadeSpeed)
{
	nextImageTime = 1;
	numTextures = 0;
	skipTime = 0;
	fSpeed = fadeSpeed;

	sScreenRef = this;

	AddRes("graphic_logo_badgames");
	//screens
	//AddRes("lg");
	//AddRes("bad_games");

	AuxTimer.SetTimeLimit(1000);
}

SplashScreen::~SplashScreen() {

}

void SplashScreen::InitObject() 
{
	bIsActiveObj = true;
	
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickBtn, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_splashscreen, NULL);
	
	initialTime = (int)(s3eTimerGetMs() * 0.001);
	currText = 0;
	bSplashOver = false;
	bSkip = false;

	if(numTextures > 0)
	{
		_width = textures[0].textureWidth;
		_height = textures[0].textureHeight;

		uvInfo = TextureHelper::GetUVInfo(textures[0]);
	}

	fade->SetAlpha(255);
	fade->FadeOut();

	//Music Settings
	Utils::PlayBGM("bgm/sd_blank.mp3", 1);
	AuxTimer.Reset();
	AuxTimer.Start();
	bPlaySilence = true;

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(XBOX360_Button_A, &SplashScreen_SkipImage);
	xbox360Controller.SetCallback(XBOX360_Button_X, &SplashScreen_SkipImage);
	xbox360Controller.SetCallback(XBOX360_Button_B, &SplashScreen_SkipImage);
	xbox360Controller.SetCallback(XBOX360_Button_Y, &SplashScreen_SkipImage);
#endif
}

void SplashScreen::EndObject() 
{
	sScreenRef = NULL;
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickBtn);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_splashscreen);

	bIsActiveObj = false;
}


void SplashScreen::AddRes(string resName)
{
	textures[numTextures] = TextureHelper::GetTextureNamed(resName);

	numTextures++;
}

void SplashScreen::SkipImage()
{
	int currTime = (int)(s3eTimerGetMs() * 0.001);
	if(currTime - initialTime >= skipTime) bSkip = true;
}

void SplashScreen::Update()
{
	AuxTimer.Update();

	if(AuxTimer.GetIsEnd() && bPlaySilence)
	{
		Utils::PlaySoundFX(FX_Silence);
		AuxTimer.Stop();
		bPlaySilence = false;
	}

	int currTime = (int)(s3eTimerGetMs() * 0.001);
	if(bSkip || ((currTime - initialTime) >= (nextImageTime)))
	{
		fade->FadeIn(fSpeed);
		if(fade->GetAlpha() >= 255)
		{
			if(currText < numTextures - 1)
			{
				currText++;
				if(currText < numTextures)
				{
					_width = textures[currText].textureWidth;
					_height = textures[currText].textureHeight;

					uvInfo = TextureHelper::GetUVInfo(textures[currText]);
					bSkip = false;
				}
				initialTime = (int)(s3eTimerGetMs() * 0.001);
				fade->FadeOut(fSpeed);
			} 
			else 
			{
				bSplashOver = true;
				fade->FadeOut(fSpeed);
			}
		}
	}

	if(SplashOver())
		gameState=GS_MainMenu;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}

bool SplashScreen::SplashOver() 
{
	return bSplashOver;
}

void SplashScreen::Render()
{
	//int xPos = int((currentGameWidth - _width) * 0.5f);
	//int yPos = int((currentGameHeight - _height) * 0.5f);

	//Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(TextureHelper::GetAtlasName(textures[currText])), CIwSVec2(xPos, yPos), CIwSVec2(_width, _height), uvInfo.minX, uvInfo.minY, uvInfo.maxX, uvInfo.maxY);
	
}