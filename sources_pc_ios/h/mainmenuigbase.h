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

#ifndef _MAINMENUIGBASE_H
#define _MAINMENUIGBASE_H

#include "mmig_menu.h"
#include "almanac_base.h"

class MainMenuIG
{
public:
	MainMenuIG();
	~MainMenuIG();
	void Update();
	void Render();
	void InitObject();
	void EndObject();
	bool bIsActiveObj;

private:
	MenuIG* menuig;
};

#endif