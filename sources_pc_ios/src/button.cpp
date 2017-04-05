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
#include "button.h"
#include "utils.h"
#include "common.h"
#include "localization.h"

void Button::EnableFocus()
{
	IsFocused(true);
}

void Button::ExecuteOnFocused()
{
	if(onFocused != NULL)
		onFocused();
}

void Button::UnableFocus()
{
	IsFocused(false);
}

void Button::ExecuteFunction()
{
	Execute(false);
}

bool Button::HandleFocusMotion(s3ePointerMotionEvent* event)
{
	if(bIsButtonAnim)
		return false;
	return MouseOn(event->m_x, event->m_y);
}

void Button::HandleMotion(s3ePointerMotionEvent* event) 
{
	if(bIsButtonAnim)
		return;
	if(!bIsActive) //|| (buttonInMouseOver != NULL && buttonInMouseOver != this)
		return;

	//if(bDebugData)
	//{
	//	IwTrace(MOUSEDATA, ("HandleMotion, Moved to %d, %d", event->m_x, event->m_y));
	//}
	
	if(!MouseOn(event->m_x, event->m_y)) 
	{
		//btnState = BTN_ROLLOUT;
		if(!bIsFocused && btnState != BTN_ROLLOUT)
		{		
			btnState = BTN_ROLLOUT;
			//buttonInMouseOver = NULL;
		}
		bPlayRollOverSound = true;
	}
	else if(MouseOn(event->m_x, event->m_y))
	{
#if defined DESKTOPPLATFORM
		if (btnState != BTN_PRESSED)
		{
			btnState = BTN_ROLLON;
			//buttonInMouseOver = this;
			if (OnRollOnFunc != NULL)
			{
				OnRollOnFunc();
			}
		}
#endif
	}
}

void Button::HandleClick(s3ePointerEvent *event) 
{
	if(bIsButtonAnim)
		return;

	if(bDebugData)
	{
		IwTrace(MOUSEDATA, ("HandleClick called, %s detected at %d, %d", event->m_Pressed ? "Press" : "Release", event->m_x, event->m_y));
	}

	if(!bIsActive)// || (buttonInMouseOver != NULL && buttonInMouseOver != this))
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
				//Utils::PlayFXSound(FX_Sound_CLICK);
				Utils::PlaySoundFX(FX_Click_01);
				if(bIsOnClickEnabled)
				{	
					if(OnClickFunc != NULL)
					{
						OnClickFunc();
					}

					if(OnClickFuncWithData != NULL)
					{
						OnClickFuncWithData(OnClickData);
					}
				}

				if(bIsFocused)
				{
					btnState = BTN_ROLLON;
				}
				else if(btnState != BTN_ROLLOUT)
				{
					btnState = BTN_ROLLOUT;
				}
			}

			if(bIsFocused)
			{
				btnState = BTN_ROLLON;
			}
			else if(btnState != BTN_ROLLOUT)
			{
				btnState = BTN_ROLLOUT;
			}
		}
	}

	if(!event->m_Pressed)
	{
		if(bIsFocused)
		{
			btnState = BTN_ROLLON;
		}
		else if(btnState != BTN_ROLLOUT)
		{
			btnState = BTN_ROLLOUT;
		}
	}
}

bool Button::MouseOn(int x, int y) 
{
	int w, h;
	int xMax, yMax;
	bool bResult = false;

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

	xMax = xPos + w;
	yMax = yPos + h;

	int xDiff = (int)(w * 0.05f);
	int yDiff = (int)(h * 0.05f);

	xMax -= xDiff;
	yMax -= yDiff;
	int tmpXPos = xPos + xDiff;
	int tmpYPos = yPos + yDiff;

	if((x >= tmpXPos && x <= xMax) && (y >= tmpYPos && y <= yMax))
		bResult = true;

	return bResult;
}

Button::Button(CIwTexture *tex, ButtonRenderData* data, int x, int y)
{
	this->tex = tex;
	buttonRenderData = data;
	bRotatedTexture = data->state[0].RollOut.bRotate;
	onFocused = NULL;
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
	marginY = 4;

	//Functions
	bIsOnClickEnabled = true;
	OnClickFunc = NULL;
	OnRollOnFunc = NULL;
	OnClickFuncWithData = NULL;
	OnClickData = NULL;

	//Focus
	bIsFocused = false;

	bPlayRollOverSound = true;

	bDebugData = false;

	btnTextOffsetByState = 2;

	buttonAlphaValue = 255;

	bIsButtonAnim = false;
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

CIwSVec2 Button::GetPos()
{
	return CIwSVec2(this->xPos, this->yPos);
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

void Button::Render(int xOff, int yOff)
{
	RenderData curData = buttonRenderData->state[GetImageID()].RollOut;
	if(btnState == BTN_ROLLON) curData = buttonRenderData->state[GetImageID()].RollOn;
	//else if(btnState == BTN_ROLLOUT) curData = buttonRenderData->state[GetImageID()].RollOut;
	else if(btnState == BTN_PRESSED) curData = buttonRenderData->state[GetImageID()].Pressed;
	else if(btnState == BTN_INACTIVE) curData = buttonRenderData->state[GetImageID()].Inactive;

	Utils::RenderSingleTexture(tex, CIwSVec2(xPos + xOff, yPos + yOff), curData, NO_ROTATE, buttonAlphaValue);
	IwGxFlush();

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

		int myBtnTextOffsetByState = 0;
		if(btnState == BTN_ROLLOUT)
			myBtnTextOffsetByState = 0;
		else if(btnState == BTN_ROLLON)
			myBtnTextOffsetByState = btnTextOffsetByState;
		else if(btnState == BTN_PRESSED)
			myBtnTextOffsetByState = btnTextOffsetByState;
		else if(btnState == BTN_INACTIVE)
			myBtnTextOffsetByState = 0;



		CIwGxFontPreparedData data;
		CIwColour myFontColor;

		//Shadow part
		IwGxFontSetRect(CIwRect(xPos - 1, yPos - marginY + 1 - myBtnTextOffsetByState, w, h));
		IwGxFontSetFont(pfont);
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);
		myFontColor.r = 51;
		myFontColor.g = 51;
		myFontColor.b = 51;
		myFontColor.a = buttonAlphaValue;
		IwGxFontSetCol(myFontColor);
		IwGxFontPrepareText(data, title);
		IwGxFontDrawText(data);
		//end shadow part


		IwGxFontSetRect(CIwRect(xPos, yPos - marginY - myBtnTextOffsetByState, w, h));
		IwGxFontSetFont(pfont);
		IwGxFontSetAlignmentHor(IW_GX_FONT_ALIGN_CENTRE);
		IwGxFontSetAlignmentVer(IW_GX_FONT_ALIGN_MIDDLE);

		if(btnState == BTN_ROLLOUT)
		{
			myFontColor.r = 217;
			myFontColor.g = 168;
			myFontColor.b = 13;
			myFontColor.a = buttonAlphaValue;
			IwGxFontSetCol(myFontColor);
		}
		else if(btnState == BTN_ROLLON)
		{
			myFontColor.r = 247;
			myFontColor.g = 224;
			myFontColor.b = 0;
			myFontColor.a = buttonAlphaValue;
			IwGxFontSetCol(myFontColor);
		}
		else if(btnState == BTN_PRESSED)
		{
			myFontColor.r = 247;
			myFontColor.g = 224;
			myFontColor.b = 0;
			myFontColor.a = buttonAlphaValue;
			IwGxFontSetCol(myFontColor);
		}
		else if(btnState == BTN_INACTIVE)
		{
			myFontColor.r = 167;
			myFontColor.g = 168;
			myFontColor.b = 170;
			myFontColor.a = buttonAlphaValue;
			IwGxFontSetCol(myFontColor);
		}

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

void Button::OnClick(functionWithData func, void* data)
{
	OnClickFuncWithData = func;
	OnClickData = data;
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
			//Utils::PlaySoundFX(FX_RollOver_02);
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

void Button::Execute(bool bChangeStatus)
{
	//IwTrace(Xbox360Debug, ("Button Exec."));
	//Utils::PlayFXSound(FX_Sound_CLICK);
	//Utils::PlaySelknamSound(FX_Click);

	if(bIsActive && bIsOnClickEnabled)
	{
		if(OnClickFunc != NULL)
		{
			OnClickFunc();
		}

		if(OnClickFuncWithData != NULL)
		{
			OnClickFuncWithData(OnClickData);
		}
	}

	if(bChangeStatus)
		SetStatus(2);
}

void Button::SetStatus(int status) 
{
	if(bIsActive)
	{
		//1 - ROLLON
		//2 - ROLLOUT
		//3 - PRESSED
		//4 - INACTIVE
		//if(status == 1) return;
		btnState = (ButtonState)status;
	}
}

void Button::SetStyle(CIwGxFont* pfont, int marginX, int marginY)
{
	if(pfont != NULL)
		this->pfont = pfont;

	this->marginX = marginX;
	this->marginY = marginY;
}

void Button::SetFont(CIwGxFont* pfont)
{
	this->pfont = pfont;
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