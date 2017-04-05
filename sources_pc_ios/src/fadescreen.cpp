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

#include "common.h"
#include "fadescreen.h"

FadeScreen::FadeScreen(int r, int g, int b) 
{
	_fadeAlpha = 0;
	fSpeed = 30;
	bFade = false;
	bFadeIn = false;
	this->r = r;
	this->g = g;
	this->b = b;

	bCompleteFade = false;
	
	bFadeOutWithTimer = bFadeInWithTimer = false;
}

FadeScreen::~FadeScreen() 
{

}

void FadeScreen::Update() 
{
	if(!bFade) return;

	idleTimer.Update();
	fadeOutTimer.Update();
	fadeInTimer.Update();

	//if(!fadeInTimer.GetIsEnd() && !fadeInTimer.GetIsStop())
	//{
	//	IwTrace(BOSSBATTLE, ("fadeInTimer.GetCurrentTime(): %d", fadeInTimer.GetCurrTime()));
	//}

	if(bCompleteFade)
	{
		if(_fadeAlpha >= maxAlpha)
		{
			bFadeIn = false;
			bCompleteFade = false;
		}
	}

	if(bFadeIn)
	{	
		if(bFadeInWithTimer)
		{
			if(fadeInTimer.GetIsEnd())
			{
				bFade = false;
				_fadeAlpha = maxAlpha;
				fadeInTimer.Stop();
			}
			else
			{
				_fadeAlpha = (int)(255 * (fadeInTimer.GetCurrTime() / (float)(fadeInTimer.GetTimeLimit()))); 
			}
		}
		else
		{
			if(_fadeAlpha + fSpeed >= maxAlpha)
			{
				_fadeAlpha = maxAlpha;
				if(!bCompleteFade) 
					bFade = false;
			}
			else
			{
				_fadeAlpha += fSpeed;
			}
		}
	} 
	else
	{
		if(bFadeOutWithTimer)
		{
			if(idleTimer.GetIsEnd())
			{
				fadeOutTimer.Reset();
				fadeOutTimer.Start();
				idleTimer.Stop();
			}

			if(fadeOutTimer.GetIsEnd())
			{
				bFade = false;
				_fadeAlpha = minAlpha;
				fadeOutTimer.Stop();
			}
			else
			{
				_fadeAlpha = (int)(255 * (1 - (fadeOutTimer.GetCurrTime() / (float)(fadeOutTimer.GetTimeLimit())))); 
			}
		}
		else
		{
			if(_fadeAlpha - fSpeed > minAlpha)
			{
				_fadeAlpha -= fSpeed;
			}
			else
			{
				_fadeAlpha = minAlpha;
				bFade = false;
			}
		}
	}
}

void FadeScreen::Render()
{
	if(!bFade && _fadeAlpha == 0) return;

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(pMat);
	static CIwSVec2 xy3[4];
	int xPos = currentGameWidth;
	int yPos = currentGameHeight;
	xy3[0].x = 0,	      xy3[0].y = 0;
	xy3[1].x = 0,		  xy3[1].y = yPos;
	xy3[2].x = xPos, xy3[2].y = yPos;
	xy3[3].x = xPos, xy3[3].y = 0;
	IwGxSetVertStreamScreenSpace(xy3, 4);

	static CIwColour cols[4];
	cols[0].Set(r, g, b, _fadeAlpha);
	cols[1].Set(r, g, b, _fadeAlpha);
	cols[2].Set(r, g, b, _fadeAlpha);
	cols[3].Set(r, g, b, _fadeAlpha);

	IwGxSetColStream(cols, 4);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	//IwGxFlush();
}

int FadeScreen::GetAlpha()
{
	return _fadeAlpha;
}

void FadeScreen::SetAlpha(int alpha)
{
	_fadeAlpha = alpha;
}

void FadeScreen::FadeIn(int speed, int maxAlpha)
{
	this->maxAlpha = maxAlpha;
	fSpeed = (int)((float)speed * FPSMULTIPLIER);
	bFadeIn = true;
	bFade = true;
	bFadeInWithTimer = false;
}

void FadeScreen::FadeInWithTimer(int time)
{
	bFadeInWithTimer = true;
	bFadeIn = true;
	bFade = true;
	this->maxAlpha = 255;

	fadeInTimer.SetTimeLimit(time);
	fadeInTimer.Reset();
	fadeInTimer.Start();
}

void FadeScreen::FadeOutWithTimer(int time, int timeBeforeFadeOut)
{
	bFadeOutWithTimer = true;
	bFadeIn = false;
	bFade = true;
	this->minAlpha = 0;

	idleTimer.SetTimeLimit(timeBeforeFadeOut);
	idleTimer.Reset();
	idleTimer.Start();
	
	fadeOutTimer.SetTimeLimit(time);
}

void FadeScreen::FadeOut(int speed, int minAlpha)
{
	bFadeOutWithTimer = false;
	this->minAlpha = minAlpha;
	fSpeed = (int)((float)speed * FPSMULTIPLIER);
	bFadeIn = false;
	bFade = true;
}

void FadeScreen::Fade(int speed, int minAlpha, int maxAlpha)
{
	this->minAlpha = minAlpha;
	this->maxAlpha = maxAlpha;
	fSpeed = (int)((float)speed * FPSMULTIPLIER);
	bFade = true;
	bCompleteFade = true;
	bFadeIn = true;
}

void FadeScreen::SetColor(int r, int g, int b)
{
	this->r = r;
	this->g = g;
	this->b = b;
}