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

#include "slider.h"
#include "texturehelper.h"
#include "utils.h"
#include <iostream>
using namespace std;

Slider::Slider()
{
	x = y = stepperX = stepperY = 0;
	lineRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("settings_line"));
	linehRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("settings_line_h"));
	lineW = TextureHelper::GetDimension(lineRD).x;
	lineH = TextureHelper::GetDimension(lineRD).y;
	stepperRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("settings_stepper"));	
	stepperhRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("settings_stepper_h"));	
	stepperW = TextureHelper::GetDimension(stepperRD).x;
	stepperH = (int)(TextureHelper::GetDimension(stepperRD).y * 0.5f);
	bPressed = false;
	maxValue = lineW - stepperW;
	onChangeValue = NULL;
	bIsFocused = false;
}

Slider::~Slider()
{

}

void Slider::Render()
{
	if(bIsFocused)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(linehRD.atlasName), CIwSVec2(x, y), linehRD);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(stepperhRD.atlasName), CIwSVec2(stepperX, stepperY), stepperhRD);
	}
	else
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(lineRD.atlasName), CIwSVec2(x, y), lineRD);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(stepperRD.atlasName), CIwSVec2(stepperX, stepperY), stepperRD);
	}
}

void Slider::SetPosition(int _x, int _y)
{
	x = _x;
	y = _y;
	stepperX = _x;
	stepperY = _y - (int)(stepperH * 0.5f);
}

void Slider::UpdateClickEvents(s3ePointerEvent* event)
{
	bPressed = event->m_Pressed && Utils::MouseOn(event->m_x, event->m_y, stepperX, stepperY + (int)(stepperH * 0.5f), stepperW, stepperH);

	if(event->m_Pressed
	   && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE
	   && Utils::MouseOn(event->m_x, event->m_y, x, y, lineW, lineH))
	{
		stepperX = min(max(event->m_x - (int)(stepperW * 0.5f), x), x + lineW - stepperW);
		ForceOnChangeValue();
	}
}

void Slider::UpdateMotionsEvents(s3ePointerMotionEvent* event)
{
	if(bPressed)
	{
		stepperX = min(max(event->m_x - (int)(stepperW * 0.5f), x), x + lineW - stepperW);
		
		if(onChangeValue != NULL)
			onChangeValue(GetSliderProgress());
	}
}

bool Slider::HandleFocusMotion(s3ePointerMotionEvent* event)
{
	return Utils::MouseOn(event->m_x, event->m_y, x, y, lineW, lineH);
}

void Slider::ForceOnChangeValue()
{
	if(onChangeValue != NULL)
		onChangeValue(GetSliderProgress());
}

float Slider::GetSliderProgress()
{
	return (stepperX - x) / (float)(maxValue);
}

void Slider::SetOnChangeValueCallback(void (*fnc)(float f))
{
	onChangeValue = fnc;
}

void Slider::SetSliderProgress(float f)
{
	stepperX = (int)(maxValue * f + x);
}

void Slider::EnableFocus()
{
	bIsFocused = true;
}

void Slider::UnableFocus()
{
	bIsFocused = false;
}

bool Slider::GetIsFocused()
{
	return bIsFocused;
}