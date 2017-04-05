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

#include "almanac_base.h"

AlmanacBase::AlmanacBase()
{
	background = new Background(0, 0, 0, fade_menu);
	almanacSelknam = new Almanac();
	almanacMenu = new AlmanacMenu();
	almanacEnemy = new AlmanacEnemy();

	titleHolderLength = 10; //32 pixels each unit.
}

AlmanacBase::~AlmanacBase()
{
	delete background;
	delete almanacSelknam;
	delete almanacMenu;
	delete almanacEnemy;
}

void AlmanacBase::InitObject()
{
	lastAlmanacState = ALMANAC_NONE;
	curAlmanacState  = ALMANAC_MENU;
}

void AlmanacBase::EndObject()
{
	almanacMenu->EndObject();
	almanacSelknam->Off();
	almanacEnemy->EndObject();
}

void AlmanacBase::Update()
{
	if(lastAlmanacState != curAlmanacState)
	{
		if(lastAlmanacState == ALMANAC_MENU)
		{
			almanacMenu->EndObject();
		}
		else if(lastAlmanacState == ALMANAC_SELKNAM)
		{
			almanacSelknam->Off();
		}
		else if(lastAlmanacState == ALMANAC_ENEMY)
		{
			almanacEnemy->EndObject();
		}

		if(curAlmanacState == ALMANAC_MENU)
		{
			almanacMenu->InitObject();
		}
		else if(curAlmanacState == ALMANAC_SELKNAM)
		{
			almanacSelknam->On();
		}
		else if(curAlmanacState == ALMANAC_ENEMY)
		{
			almanacEnemy->InitObject();
		}

		lastAlmanacState = curAlmanacState;
	}

	if(curAlmanacState == ALMANAC_MENU)
		almanacMenu->Update();
	else if(curAlmanacState == ALMANAC_SELKNAM)
		almanacSelknam->Update();
	else if(curAlmanacState == ALMANAC_ENEMY)
		almanacEnemy->Update();
}

void AlmanacBase::Render()
{
	background->Render();

	if(currentGameHeight > 600)
	{
		Utils::RenderLabelTitle(titleHolderLength, (int)((currentGameWidth - (128 + titleHolderLength * 32)) * 0.5f), -50);
		IwGxLightingOn();
		Utils::RenderText(loc_ui_almanac, CIwRect((int)((currentGameWidth - (128 + titleHolderLength * 32)) * 0.5f), 45, 438, 66), font_komikadisplay28, 0xff00d9fc);
		IwGxLightingOff();
	}

	if(curAlmanacState == ALMANAC_MENU)
	{
		almanacMenu->Render();
	}
	else if(curAlmanacState == ALMANAC_SELKNAM)
	{
		almanacSelknam->Render();
	}
	else if(curAlmanacState == ALMANAC_ENEMY)
	{
		almanacEnemy->Render();
	}
}