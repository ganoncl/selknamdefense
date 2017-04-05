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

#ifndef _ALMANACBASE_H
#define _ALMANACBASE_H

#include "IwGx.h"
#include "utils.h"
#include "common.h"
#include "localization.h"
#include "mm_almanac.h"
#include "almanac_menu.h"
#include "almanac_enemy.h"
#include "game_window.h"

class AlmanacBase : public GameWindow
{
public:
	AlmanacBase();
	~AlmanacBase();
	void InitObject();
	void EndObject();
	void Render();
	void Update();

private:
	Background* background; //Black semi-transparent background.
	
	//Title
	int    titleHolderLength;

	Almanac* almanacSelknam;
	AlmanacMenu* almanacMenu;
	AlmanacEnemy* almanacEnemy;
};

#endif