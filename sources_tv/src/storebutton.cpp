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

#include "storebutton.h"
#include "common.h"

StoreButton::StoreButton(CIwTexture* tex, int x, int y, int w) : Button(tex, x, y , w)
{
	CIwResGroup* pGroup = IwGetResManager()->GetGroupNamed("mapselection");
	ptextFont = font_komikadisplaybold25;
	numLines = 0;

	id = -1;
	id_name = "No name";
}

StoreButton::~StoreButton()
{

}

void StoreButton::Render()
{
	Button::Render();

	IwGxLightingOn();
	IwGxFontSetFont(ptextFont);
	IwGxFontSetCol(0xff808080);
	for(int i = 0; i < numLines; i++)
	{
		IwGxFontSetRect(textPos[i]);
		IwGxFontDrawText(text[i]);
	}
	IwGxLightingOff();
	IwGxFlush();
}

void StoreButton::AddBonus(const char* text, int x, int y, int w, int h)
{
	this->text[numLines] = text;
	this->textPos[numLines] = CIwRect(this->GetX() + x, this->GetY() + y, w, h);
	numLines++;
}

void StoreButton::SetAbility(btnStoreState state)
{
	this->state = state;
	if(state == ABILITY_AVAILABLE)
	{
		this->SetImage(0);
		this->FunctionOnClickEnabled(true);
	}
	else if(state == ABILITY_LEARNED)
	{
		this->SetImage(1);
		this->FunctionOnClickEnabled(false);
	}
	else if(state == ABILITY_LOCKED)
	{
		this->SetImage(2);
		this->FunctionOnClickEnabled(false);
	}
}

void StoreButton::SetID(int id)
{
	this->id = id;
}

void StoreButton::SetID(const char* name)
{
	this->id_name = name;
}

int StoreButton::GetIDByNumber()
{
	return this->id;
}

const char* StoreButton::GetIDByName()
{
	return this->id_name;
}
