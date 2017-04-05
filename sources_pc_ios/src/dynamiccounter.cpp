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

#include "dynamiccounter.h"

DynamicCounter::DynamicCounter()
{
	srand(2013);
}

DynamicCounter::~DynamicCounter()
{

}

void DynamicCounter::SetTime(int time)
{
	timer.SetTimeLimit(time);
}

void DynamicCounter::SetValue(int value_int)
{
	this->value_int = value_int;
	num_digits = (int)(log10(value_int) + 1);
}

void DynamicCounter::SetTextStyle(CIwRect position, CIwGxFont* font, uint32 color, IwGxFontAlignHor align)
{
	this->position = position;
	this->font     = font;
	this->color    = color;
	this->align    = align;
}

void DynamicCounter::Update()
{
	timer.Update();
	if(!timer.GetIsEnd() && !timer.GetIsStop())
	{
		if(value_int != 0)
		{
			value_string[0] = '\0';
			for(int i = 0; i < num_digits; i++)
			{
				sprintf(value_string, "%s%d", value_string, rand() % 10);
			}
		}
	}
	else
	{
		sprintf(value_string, "%d", value_int);
	}
}

void DynamicCounter::Render()
{
	Utils::RenderText(value_string, position, font, color, align);
}

void DynamicCounter::InitAnimation()
{
	if(timer.GetIsEnd() || timer.GetIsStop())
	{
		timer.Start();
		if(value_int == 0)
		{
			sprintf(value_string, "0");
		}
	}
}

bool DynamicCounter::AnimationEnded()
{
	return timer.GetIsEnd();
}