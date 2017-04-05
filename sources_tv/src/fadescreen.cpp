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
}

FadeScreen::~FadeScreen() 
{

}

void FadeScreen::Update() 
{
	if(!bFade) return;

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

void FadeScreen::Render()
{
	if(!bFade && _fadeAlpha == 0) return;

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	IwGxSetMaterial(pMat);
	static CIwSVec2 xy3[4];
	int xPos = IwGxGetScreenWidth();
	int yPos = IwGxGetScreenHeight();
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
	fSpeed = speed;
	bFadeIn = true;
	bFade = true;
}

void FadeScreen::FadeOut(int speed, int minAlpha)
{
	this->minAlpha = minAlpha;
	fSpeed = speed;
	bFadeIn = false;
	bFade = true;
}

void FadeScreen::Fade(int speed, int minAlpha, int maxAlpha)
{
	this->minAlpha = minAlpha;
	this->maxAlpha = maxAlpha;
	fSpeed = speed;
	bFade = true;
	bCompleteFade = true;
	bFadeIn = true;
}