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

#ifndef _SLIDER_H
#define _SLIDER_H

#include "common.h"
#include "focuseableobject.h"

class Slider : public FocuseableObject
{
public:
	Slider();
	~Slider();
	void Render();
	void SetPosition(int _x, int _y);
	void UpdateClickEvents(s3ePointerEvent* event);
	void UpdateMotionsEvents(s3ePointerMotionEvent* event);
	void SetOnChangeValueCallback(void (*fnc)(float f));
	void SetSliderProgress(float f);
	float GetSliderProgress();
	void EnableFocus();
	void UnableFocus();
	bool GetIsFocused();
	void ForceOnChangeValue();
	bool HandleFocusMotion(s3ePointerMotionEvent* event);

private:
	void (*onChangeValue)(float newValue);
	int x;
	int y;
	int lineW;
	int lineH;
	int stepperX;
	int stepperY;
	int stepperH;
	int stepperW;
	int maxValue;
	bool bPressed;
	RenderData lineRD;
	RenderData linehRD;
	RenderData stepperRD;
	RenderData stepperhRD;
	bool bIsFocused;
};

#endif