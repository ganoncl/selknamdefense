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

#ifndef _ASTARHELPER_H
#define _ASTARHELPER_H

#include "common.h"

class CGame;
class IsoTerrain;

class AStarHelper
{
public:
	AStarHelper();
	~AStarHelper();

	void AStarInitMapGetObjectiveNodes(CGame* gameRef);
	void AStarInitPreCalcAdjNodes(CGame* gameRef);

	void AStarDumpGrid(CGame* gameRef);

	//function to retrieve a path from a chunk of entire grid, specified by range from tileInit.
	bool AStarRetrievePathByChunk(IsoTerrain *tileList[][MAX_TILESROW], IsoTerrain *tileInit, IsoTerrain *tileFinal, IsoTerrain **resultList, int range = 1);
	//function to retrieve a path from a entire grid.
	bool AStarRetrievePathByGrid(IsoTerrain *tileList[][MAX_TILESROW], IsoTerrain *tileInit, IsoTerrain *tileFinal, IsoTerrain **resultList, int iMax = MAX_TILESROW, int jMax = MAX_TILESROW);
	//Astar functions.
	bool AStarDetermineNodeValue(IsoTerrain *_targetNode, IsoTerrain *_currentNode, IsoTerrain *_testingNode, IsoTerrain **_openList, IsoTerrain **_closedList);
	IsoTerrain *AStarGetSmallestFValueNode(IsoTerrain **_list);
	bool IsInAStarList(IsoTerrain **_list, IsoTerrain *_node);
	void AddToAStarList(IsoTerrain **_list, IsoTerrain *_node);
	void DumpAStarList(IsoTerrain **_list, bool _bIsOpenList);
	void RemoveFromAStarList(IsoTerrain **_list, IsoTerrain *_node);

	bool AStarRetrievePathByRectangle(IsoTerrain *tileList[][MAX_TILESROW], IsoTerrain *tileInit, IsoTerrain *tileFinal, IsoTerrain **resultList, int range = 1);

	IsoTerrain* initial_tile;
};



#endif