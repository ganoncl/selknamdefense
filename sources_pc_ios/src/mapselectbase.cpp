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

#include "common.h"
#include "mapselectbase.h"
#include "background.h"
#include "parserhelper.h"

MapSelectionBase::MapSelectionBase()
{
	mapselection = new MapSelection();
	store = new Store();
	background = new Background(2);
}

MapSelectionBase::~MapSelectionBase()
{
	delete mapselection;
	delete store;
	delete background;
}

void MapSelectionBase::Update()
{
	if(lastMapSelectionState != mapSelectionState)
	{
		if(lastMapSelectionState == MS_MapSelection) mapselection->EndObject();
		else if(lastMapSelectionState == MS_Store) store->EndObject();

		if(mapSelectionState == MS_MapSelection) mapselection->InitObject();
		else if(mapSelectionState == MS_Store) store->InitObject();

		lastMapSelectionState = mapSelectionState;
	}

	if(mapSelectionState == MS_Store) store->Update();
	else if(mapSelectionState == MS_MapSelection) mapselection->Update();
}

void MapSelectionBase::Render()
{
	background->Render();

	if(mapSelectionState == MS_MapSelection) mapselection->Render();
	else if(mapSelectionState == MS_Store) store->Render();
}

void MapSelectionBase::InitObject()
{
	bIsActiveObj = true;
	mapSelectionState = MS_MapSelection;

	s3eAudioStop();
	if(bIsAudioPaused)
	{
		LastMusicStacked = Music_Store;
	}
	else
	{
		Utils::PlayBGM("bgm/sd_store_theme_loop.mp3", 0);
		LastMusicStacked = Music_None;
	}
}

void MapSelectionBase::EndObject()
{
	bIsActiveObj = false;
	
	mapSelectionState = MS_None;
	Update();

	if(mapselection->bIsActiveObj)
		mapselection->EndObject();
	if(store->bIsActiveObj)
		store->EndObject();
}

