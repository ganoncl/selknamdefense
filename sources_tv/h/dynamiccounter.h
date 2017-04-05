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

#ifndef _DYNAMICCOUNTER_H
#define _DYNAMICCOUNTER_H

#include "IwGx.h"
#include "timer.h"
#include "IwGxFont.h"
#include "math.h"
#include "utils.h"

class DynamicCounter
{
public:
	DynamicCounter();
	~DynamicCounter();
	void SetTime(int time); //miliseconds
	void SetValue(int value_int);
	void SetTextStyle(CIwRect position, CIwGxFont* font, uint32 color, IwGxFontAlignHor align);
	void Update();
	void Render();
	void InitAnimation();
	bool AnimationEnded();

private:
	CIwRect position;
	CIwGxFont* font;
	uint32 color;
	IwGxFontAlignHor align;
	Timer timer;
	int value_int;
	int num_digits;
	char value_string[100];
};

#endif