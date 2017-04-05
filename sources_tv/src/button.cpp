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

#include "IwGx.h"
#include "IwGxFont.h"
#include "IwResManager.h"
#include "s3e.h"
#include "common.h"
#include "button.h"
#include "utils.h"

void Button::HandleMotion(s3ePointerMotionEvent* event) 
{
	if(!bIsActive)
		return;
	
	//IwTrace(TEXHELP, ("HandleMotion"));
	if(MouseOn(event->m_x, event->m_y))
	{
		if(btnState != BTN_PRESSED) 
		{
			btnState = BTN_ROLLON;
			if(OnRollOnFunc != NULL)
			{
				OnRollOnFunc();
			}
		}
	} 
	else 
	{
		if(!bIsFocused && btnState != BTN_ROLLOUT) btnState = BTN_ROLLOUT;
		else if(bIsFocused && btnState == BTN_PRESSED) btnState = BTN_ROLLON;
		bPlayRollOverSound = true;
	}
}

void Button::HandleClick(s3ePointerEvent *event) 
{
	if(!bIsActive)
		return;

	//Button only allows must respond to the click of the left button of the mouse
	if(event->m_Button != S3E_POINTER_BUTTON_LEFTMOUSE)
	{
		return;
	}

	if(MouseOn(event->m_x, event->m_y))
	{	
		if(event->m_Pressed)
		{
			//IwTrace(CLICK, ("Click"));
			lastBtnPressed = event->m_Button;
			if(lastBtnPressed ==  S3E_POINTER_BUTTON_LEFTMOUSE)
			{
				if(btnState != BTN_PRESSED) 
				{
					btnState = BTN_PRESSED;
				}
			}
		}
		else
		{
			if(btnState == BTN_PRESSED && lastBtnPressed ==  S3E_POINTER_BUTTON_LEFTMOUSE)
			{
				Utils::PlayFXSound(FX_Sound_CLICK);
				if(bIsOnClickEnabled && OnClickFunc != NULL)
				{	
					OnClickFunc();
				}
				if(btnState != BTN_ROLLOUT)
					btnState = BTN_ROLLOUT;
			}
			if(btnState != BTN_ROLLON) btnState = BTN_ROLLON;
		}
	} 
}

bool Button::MouseOn(int x, int y) 
{
	int w, h;
	if(bRotatedTexture)
	{
		w = _height;
		h = _width;
	}
	else
	{
		w = _width;
		h = _height;
	}
	//IwTrace(TEXHELP, ("MOUSE ON!: %d %d", w, h));
	return (x >= xPos && 
		x <= (xPos + w) && 
		y >= yPos && 
		y <= (yPos + (int)(h)));
}

Button::Button(CIwTexture *tex, int xPos, int yPos, int w)
{
	newButtonClass = false;
	bRotatedTexture = false;

	this->tex = tex;
	
	if(w == 0)
	{
		if(tex != NULL)
			_width = tex->GetWidth();
	}
	else
	{
		_width = w;
	}

	if(tex != NULL)
		_height = (int)(tex->GetHeight() * 0.25f);

	this->xPos = xPos;
	this->yPos = yPos;

	//Button position.
	xy3[0].x = xPos,	      xy3[0].y = yPos;
	xy3[1].x = xPos,		  xy3[1].y = yPos + (int)(_height);
	xy3[2].x = xPos + _width, xy3[2].y = yPos + (int)(_height);
	xy3[3].x = xPos + _width, xy3[3].y = yPos;

	btnState = BTN_ROLLOUT;
	bIsActive = true;

	//UV coords
	SetImage(0);
	if(w == 0) 
	{
		//whole image
		_uvMinX = 0;
		_uvMaxX = 1;
	}
	else
	{
		_uvMinX = 0;
		_uvMaxX = ((float)w / (float)tex->GetWidth());
	}
	SetImage(0);

	//Init Font
	title = NULL;
    pfont = font_komikadisplaybold25;
	marginX = 5;
	marginY = 5;
	textColor = 0xff000000;

	//Functions
	bIsOnClickEnabled = true;
	OnClickFunc = NULL;
	OnRollOnFunc = NULL;

	//Focus
	bIsFocused = false;

	bPlayRollOverSound = true;
}

Button::Button(CIwTexture *tex, ButtonRenderData* data, int x, int y)
{
	newButtonClass = true;

	this->tex = tex;
	buttonRenderData = data;
	bRotatedTexture = data->state[0].RollOut.bRotate;

	_width = data->state[0].RollOut.width;
	_height = data->state[0].RollOut.height;

	xPos = x;
	yPos = y;

	btnState = BTN_ROLLOUT;
	bIsActive = true;

	//UV coords
	SetImage(0);

	//Init Font
	title = NULL;
	pfont = font_komikadisplaybold25;
	marginX = 5;
	marginY = 5;
	textColor = 0xff000000;

	//Functions
	bIsOnClickEnabled = true;
	OnClickFunc = NULL;
	OnRollOnFunc = NULL;

	//Focus
	bIsFocused = false;

	bPlayRollOverSound = true;
}

Button::~Button()
{

}

void Button::SetPos(int x, int y)
{
	this->xPos = x;
	this->yPos = y;

	//Button position.
	xy3[0].x = xPos,	      xy3[0].y = yPos;
	xy3[1].x = xPos,		  xy3[1].y = yPos + (int)(_height);
	xy3[2].x = xPos + _width, xy3[2].y = yPos + (int)(_height);
	xy3[3].x = xPos + _width, xy3[3].y = yPos;
}

void Button::SetText(const char* title) 
{
	this->title = title;
}

void Button::SetUVCoords() {
	if(btnState == BTN_ROLLOUT)
	{
		//Imagen 1
		s_UV[0] = CIwFVec2(uvMinX, 0);
		s_UV[1] = CIwFVec2(uvMinX, 0.25f);
		s_UV[2] = CIwFVec2(uvMaxX, 0.25f);
		s_UV[3] = CIwFVec2(uvMaxX, 0);
	}
	else if(btnState == BTN_ROLLON)
	{
		//Imagen 2
		s_UV[0] = CIwFVec2(uvMinX, 0.25f);
		s_UV[1] = CIwFVec2(uvMinX, 0.50f);
		s_UV[2] = CIwFVec2(uvMaxX, 0.50f);
		s_UV[3] = CIwFVec2(uvMaxX, 0.25f);
	}
	else if(btnState == BTN_PRESSED)
	{
		//Imagen 3
		s_UV[0] = CIwFVec2(uvMinX, 0.50f);
		s_UV[1] = CIwFVec2(uvMinX, 0.75f);
		s_UV[2] = CIwFVec2(uvMaxX, 0.75f);
		s_UV[3] = CIwFVec2(uvMaxX, 0.50f);
	}
	else if(btnState == BTN_INACTIVE) 
	{
		//Imagen 4
		s_UV[0] = CIwFVec2(uvMinX, 0.75f);
		s_UV[1] = CIwFVec2(uvMinX, 1.00f);
		s_UV[2] = CIwFVec2(uvMaxX, 1.00f);
		s_UV[3] = CIwFVec2(uvMaxX, 0.75f);
	}
}

void Button::Render()
{
	if(newButtonClass)
	{
		RenderData curData;
		if(btnState == BTN_ROLLON) curData = buttonRenderData->state[GetImageID()].RollOn;
		else if(btnState == BTN_ROLLOUT) curData = buttonRenderData->state[GetImageID()].RollOut;
		else if(btnState == BTN_PRESSED) curData = buttonRenderData->state[GetImageID()].Pressed;
		else if(btnState == BTN_INACTIVE) curData = buttonRenderData->state[GetImageID()].Inactive;

		Utils::RenderSingleTexture(tex, CIwSVec2(xPos, yPos), curData);
		IwGxFlush();
	}
	else
	{
		//Button texture
		CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
		//pMat->SetColAmbient(0xfff0f0f0);
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		pMat->SetTexture(tex);
		IwGxSetMaterial(pMat);
		IwGxSetVertStreamScreenSpace(xy3, 4);
		SetUVCoords();
		IwGxSetUVStream(s_UV);
		IwGxSetColStream(NULL);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
	}

	//Font
	if(title != NULL)
	{
		IwGxLightingOn();

		int w, h;
		if(bRotatedTexture)
		{
			w = _height;
			h = _width;
		}
		else
		{
			w = _width;
			h = _height;
		}

		CIwGxFontPreparedData data;
		CIwColour myFontColor;

		//Shadow part
		IwGxFontSetRect(CIwRect(xPos - 1, yPos - marginY + 1, w, h));
		IwGxFontSetFont(pfont);
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
		myFontColor.r = 51;
		myFontColor.g = 51;
		myFontColor.b = 51;
		myFontColor.a = 255;
		IwGxFontSetCol(myFontColor);
		IwGxFontPrepareText(data, title);
		IwGxFontDrawText(data);
		//end shadow part



		IwGxFontSetRect(CIwRect(xPos, yPos - marginY, w, h));
		IwGxFontSetFont(pfont);
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);

		if(btnState == BTN_ROLLOUT)
			IwGxFontSetCol(0xff0da8d9);
		else if(btnState == BTN_ROLLON)
			IwGxFontSetCol(0xff00e0f7);
		else if(btnState == BTN_PRESSED)
			IwGxFontSetCol(0xff00e0f7);
		else if(btnState == BTN_INACTIVE)
			IwGxFontSetCol(0xffaaa8a7);

		
		IwGxFontPrepareText(data, title);
		IwGxFontDrawText(data);
		IwGxLightingOff();
	}
}

void Button::Enabled(bool bIsEnabled) {
	if(!bIsEnabled) 
	{
		btnState = BTN_INACTIVE;
		bIsActive = false;
	}
	else
	{
		btnState = BTN_ROLLOUT;
		bIsActive = true;
	}
}

void Button::OnClick(function func) 
{
	OnClickFunc = func;
}

void Button::OnRollOn(function func) 
{
	OnRollOnFunc = func;
}

void Button::IsFocused(bool bIsFocused, bool playSound)
{
	this->bIsFocused = bIsFocused;
	if(bIsFocused)
	{
		btnState = BTN_ROLLON;
		if(bPlayRollOverSound && playSound) {
			Utils::PlayFXSound(FX_Sound_ROLLOVER02);
			bPlayRollOverSound = false;
		}
	}
	else
	{
		bPlayRollOverSound = true;
		if(bIsActive) btnState = BTN_ROLLOUT;
		else btnState = BTN_INACTIVE;
	}
}

bool Button::IsActive()
{
	return bIsActive;
}

void Button::Execute()
{
	Utils::PlayFXSound(FX_Sound_CLICK);
	if(bIsActive && OnClickFunc != NULL && bIsOnClickEnabled)
		OnClickFunc();
	SetStatus(2);
}

void Button::SetStatus(int status) 
{
	//1 - ROLLON
	//2 - ROLLOUT
	//3 - PRESSED
	//4 - INACTIVE
	btnState = (ButtonState)status;
}

void Button::SetStyle(CIwGxFont* pfont, int marginX, int marginY, uint32 color)
{
	if(pfont != NULL)
		this->pfont = pfont;

	this->marginX = marginX;
	this->marginY = marginY;
	this->textColor = color;
}

void Button::SetImage(int id)
{
	_id = id;
	uvMinX = id * _uvMaxX;
	uvMaxX = id * _uvMaxX + _uvMaxX;
}

void Button::FunctionOnClickEnabled(bool bEnabled)
{
	bIsOnClickEnabled = bEnabled;
}


int Button::GetWidth()
{
	return this->_width;
}

int Button::GetHeight()
{
	return this->_height;
}

int Button::GetX()
{
	return this->xPos;
}

int Button::GetY()
{
	return this->yPos;
}

CIwVec2 Button::GetCenter()
{
	 return CIwVec2(xPos + (int)(_width * 0.5f), yPos + (int)(_height * 0.5f));
}