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

#ifndef _STOREBUTTON_H
#define _STOREBUTTON_H

#include "button.h"

typedef enum
{
	ABILITY_AVAILABLE,
	ABILITY_LEARNED,
	ABILITY_LOCKED,
} btnStoreState;

class StoreButton : public Button {
public:
	StoreButton(CIwTexture* tex, int x, int y, int w = 0);
	~StoreButton();

	void Render();

	//Variables that holds a way to identify the button.
	void SetID(const char* name); //For example, name = "Warrior"
	void SetID(int id); //For example, id = 0
	int GetIDByNumber();
	const char* GetIDByName();

	void AddBonus(const char* text, int x, int y, int w = 100, int h = 30);

	void SetAbility(btnStoreState state);
	btnStoreState state;


private:
	int id;
	const char* id_name;

	int numLines;
	const char* text[10];
	CIwRect textPos[10];
	CIwGxFont* ptextFont;	
};

#endif