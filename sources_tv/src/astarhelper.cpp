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

#include "IwGx.h"
#include "common.h"
#include "astarhelper.h"
#include "stdio.h"
#include "isoterrain.h"
#include "game.h"
#include "limits.h"

AStarHelper::AStarHelper()
{

}

AStarHelper::~AStarHelper()
{

}

void AStarHelper::AStarInitMapGetObjectiveNodes(CGame* gameRef)
{
	int myObjectivesNum = 0;
	IsoTerrain *IsoObjetive = NULL;
	
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		IsoObjetive = gameRef->isoTerrainAstarList[i];
		if(IsoObjetive && IsoObjetive->getIsFinalPoint())
		{
			myObjectivesNum = IsoObjetive->getFinalPointNum() - 1;
			gameRef->objectivesNodes[myObjectivesNum] = IsoObjetive;
		}
	}

	IwTrace(ASTAR, ("AStarInitMapGetObjectiveNodes() called, done"));
}

void AStarHelper::AStarInitPreCalcAdjNodes(CGame* gameRef)
{
	IsoTerrain *CurrentIsoTerrain = NULL;

	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		CurrentIsoTerrain = gameRef->isoTerrainAstarList[i];
		if(!CurrentIsoTerrain)
			continue;

		//IwTrace(APPLOG, ("InitMap() called, CurrentIsoTerrain->getIsWalkeable() %i", CurrentIsoTerrain->getIsWalkeable()));
		if(CurrentIsoTerrain->getIsWalkeableFromNorth()
			&& CurrentIsoTerrain->getIsWalkeableFromSouth()
			&& CurrentIsoTerrain->getIsWalkeableFromEast()
			&& CurrentIsoTerrain->getIsWalkeableFromWest())
		{
			CurrentIsoTerrain->DetectAdjacentPlaneNodes(gameRef->isoTerrainAstarList, ASTAR_LISTSMAX);
		}
		else
		{
			CurrentIsoTerrain->DetectAdjacentRampNodes(gameRef->isoTerrainAstarList, ASTAR_LISTSMAX);
		}
	}

	IwTrace(ASTAR, ("AStarInitPreCalcAdjNodes() called, done"));
}

void AStarHelper::AStarDumpGrid(CGame* gameRef)
{
	int tmpXNumTile = -1;
	int tmpYNumTile = -1;
	CIwFVec3 tmpPos;
	IsoTerrain *myTerrain = NULL;

	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			myTerrain = gameRef->isoTerrainAstarGrid[j][i];
			if(!myTerrain)
				continue;
			if(j >= 0 && j <= 2 && i >= 15 && i <= 17 )
			{
				tmpPos = myTerrain->getPosition();
				IwTrace(ASTAR, ("AStarDumpGrid() called,************************************"));
				IwTrace(ASTAR, ("AStarDumpGrid() called, isoTerrainAstarGrid[%i][%i], position X=%.f, Y=%.f, Z=%.f", j, i, tmpPos.x, tmpPos.y, tmpPos.z));
				if(myTerrain->northNode)
				{
					tmpPos = myTerrain->northNode->getPosition();
					tmpXNumTile = myTerrain->northNode->getXNumTile();
					tmpYNumTile = myTerrain->northNode->getYNumTile();
					IwTrace(ASTAR, ("AStarDumpGrid() called, Current NORTH Node is in position X=%.f, Y=%.f, Z=%.f, grid[%i][%i]", tmpPos.x, tmpPos.y, tmpPos.z, tmpXNumTile, tmpYNumTile));
				}
				else
				{
					IwTrace(ASTAR, ("AStarDumpGrid() called, no NORTH node "));
				}
				if(myTerrain->southNode)
				{
					tmpPos = myTerrain->southNode->getPosition();
					tmpXNumTile = myTerrain->southNode->getXNumTile();
					tmpYNumTile = myTerrain->southNode->getYNumTile();
					IwTrace(ASTAR, ("AStarDumpGrid() called, Current SOUTH Node is in position X=%.f, Y=%.f, Z=%.f, grid[%i][%i]", tmpPos.x, tmpPos.y, tmpPos.z, tmpXNumTile, tmpYNumTile));
				}
				else
				{
					IwTrace(ASTAR, ("AStarDumpGrid() called, no SOUTH node "));
				}
				if(myTerrain->eastNode)
				{
					tmpPos = myTerrain->eastNode->getPosition();
					tmpXNumTile = myTerrain->eastNode->getXNumTile();
					tmpYNumTile = myTerrain->eastNode->getYNumTile();
					IwTrace(ASTAR, ("AStarDumpGrid() called, Current EAST Node is in position X=%.f, Y=%.f, Z=%.f, grid[%i][%i]", tmpPos.x, tmpPos.y, tmpPos.z, tmpXNumTile, tmpYNumTile));
				}
				else
				{
					IwTrace(ASTAR, ("AStarDumpGrid() called, no EAST node "));
				}
				if(myTerrain->westNode)
				{
					tmpPos = myTerrain->westNode->getPosition();
					tmpXNumTile = myTerrain->westNode->getXNumTile();
					tmpYNumTile = myTerrain->westNode->getYNumTile();
					IwTrace(ASTAR, ("AStarDumpGrid() called, Current WEST Node is in position X=%.f, Y=%.f, Z=%.f, grid[%i][%i]", tmpPos.x, tmpPos.y, tmpPos.z, tmpXNumTile, tmpYNumTile));
				}
				else
				{
					IwTrace(ASTAR, ("AStarDumpGrid() called, no WEST node "));
				}
			}
			else
			{
				//IwTrace(APPLOG, ("AStarDumpGrid() called, isoTerrainAstarGrid[%i][%i] NULL ", j, i));
			}
		}
	}
}

bool AStarHelper::AStarRetrievePathByChunk(IsoTerrain *tileList[][MAX_TILESROW], IsoTerrain *tileInit, IsoTerrain *tileFinal, IsoTerrain **resultList, int range)
{
	IsoTerrain *chunkTileList[MAX_TILESROW][MAX_TILESROW];
	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
			chunkTileList[i][j] = NULL;
	}

	for(int i = 0; i < ASTAR_LISTSMAX; i++)
		resultList[i] = NULL;
	
	if(!tileInit || !tileFinal || (tileInit==tileFinal))
		return false;

	IsoTerrain *myTile = NULL;

	int xTileNum = tileInit->getXNumTile();
	int yTileNum = tileInit->getYNumTile();
	
	int xTileNumMin = xTileNum - range;
	if(xTileNumMin < 0){ xTileNumMin = 0;}
	int xTileNumMax = xTileNum + range + 1;
	if(xTileNumMax > MAX_TILESROW){ xTileNumMax = MAX_TILESROW;}
	
	int yTileNumMin = yTileNum - range;
	if(yTileNumMin < 0){ yTileNumMin = 0;}
	int yTileNumMax = yTileNum + range + 1;
	if(yTileNumMax > MAX_TILESROW){ yTileNumMax = MAX_TILESROW;}

	bool bTileInitOnChunk = false;
	bool bTileFinalOnChunk = false;

	for(int i = xTileNumMin; i < xTileNumMax; i++)
	{
		int i_chunk = 0;
		for(int j = yTileNumMin; j < yTileNumMax; j++)
		{
			int j_chunk = 0;
			myTile = tileList[i][j];
			if(myTile)
			{
				if(myTile == tileInit){bTileInitOnChunk = true;}
				if(myTile == tileFinal){bTileFinalOnChunk = true;}
				chunkTileList[i_chunk][j_chunk] = myTile;
			}
			j_chunk++;
		}
		i_chunk++;
	}
	if(!bTileInitOnChunk || !bTileFinalOnChunk)
		return false;

	bool bFoundTarget = false;

	return AStarRetrievePathByGrid(chunkTileList, tileInit, tileFinal, resultList, xTileNumMax, yTileNumMax);
}

bool AStarHelper::AStarRetrievePathByGrid(IsoTerrain *tileList[][MAX_TILESROW], IsoTerrain *tileInit, IsoTerrain *tileFinal, IsoTerrain **resultList, int iMax, int jMax)
{
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
		resultList[i] = NULL;

	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
		{
			if(tileList[i][j] != NULL)
			{
				tileList[i][j]->aStar_movementCost = 0;
			}
		}
	}

	if(!tileInit || !tileFinal || (tileInit==tileFinal))
	{
		return false;
	}
	initial_tile = tileInit;

	IsoTerrain *myTile = NULL;

	//heuristic;
	int heuristicValue = 0;
	int dx = -1;
	int dy = -1;

	for(int i = 0; i < iMax; i++)
	{
		for(int j = 0; j < jMax; j++)
		{
			myTile = tileList[i][j];
			if(!myTile)
				continue;
			dx = abs(tileFinal->getXNumTile() - myTile->getXNumTile());
			dx = abs(tileFinal->getYNumTile() - myTile->getYNumTile());
			heuristicValue = (dx + dy);
			myTile->aStar_heuristicValue = heuristicValue;
			
		}
	}
	IwTrace(ASTAR, ("AStarRetrievePathByGrid() called, heuristicValue done"));

	//prepare astar data
	IsoTerrain *checkingNode = tileInit;
	IsoTerrain *openList[ASTAR_LISTSMAX];
	IsoTerrain *closedList[ASTAR_LISTSMAX];
	bool bFoundTarget = false;

	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		openList[i] = NULL;
		closedList[i] = NULL;
	}
	for(int i = 0; i < iMax; i++)
	{
		for(int j = 0; j < jMax; j++)
		{
			if(tileList[i][j])
				tileList[i][j]->parentNode = NULL;
		}
	}
	
	//calcule a-star path
	int numIterations = 0;
	while(!bFoundTarget)
	{
		if(checkingNode == NULL) break;

		//IwTrace(APPLOG, ("AStarRetrievePathByGrid() called, iteration for terrain[%i][%i]", j, i ))
		if(checkingNode->northNode)
		{
			bFoundTarget = AStarDetermineNodeValue(tileFinal, checkingNode, checkingNode->northNode, openList, closedList);
		}
		if(checkingNode->eastNode && !bFoundTarget)
		{
			bFoundTarget = AStarDetermineNodeValue(tileFinal, checkingNode, checkingNode->eastNode, openList, closedList);
		}
		if(checkingNode->southNode && !bFoundTarget)
		{
			bFoundTarget = AStarDetermineNodeValue(tileFinal, checkingNode, checkingNode->southNode, openList, closedList);
		}
		if(checkingNode->westNode && !bFoundTarget)
		{
			bFoundTarget = AStarDetermineNodeValue(tileFinal, checkingNode, checkingNode->westNode, openList, closedList);
		}

		if(!bFoundTarget)
		{
			AddToAStarList(closedList, checkingNode);
			RemoveFromAStarList(openList, checkingNode);

			bool bOpenListEmpty = true;
			for(int i = 0; i < ASTAR_LISTSMAX; i++)
			{
				if(openList[i] != NULL)
					bOpenListEmpty = false;
			}

			if(bOpenListEmpty)
				break;

			//search the node with smallest F value
			checkingNode = AStarGetSmallestFValueNode(openList);
		}
		numIterations++;
	}

	//if found target fill the path on resultList
	if(bFoundTarget)
	{
		int numNodes = -1;
		int totalNodesPath = 0;
		IsoTerrain *tileFinalTmp = tileFinal;
		while(tileFinal->parentNode)
		{
			tileFinal = tileFinal->parentNode;
			numNodes++;
		}
		totalNodesPath = numNodes + 1;
		tileFinal = tileFinalTmp;
		while(tileFinal->parentNode)
		{
			resultList[numNodes] = tileFinal;
			tileFinal = tileFinal->parentNode;
			numNodes--;
		}
	}

	IwTrace(ASTAR, ("AStarRetrievePathByGrid() called, bFoundTarget %i, numIterations %i", bFoundTarget, numIterations));

	return bFoundTarget;
}

bool AStarHelper::AStarDetermineNodeValue(IsoTerrain *_targetNode, IsoTerrain *_currentNode, IsoTerrain *_testingNode, IsoTerrain **_openList, IsoTerrain **_closedList)
{
	//Closed List: List of nodes that have been checked.
	//Open List: List of nodes that need to be checked.
	if(_testingNode == NULL)
		return false;
	//target node founded.. a-star finished
	if(_testingNode == _targetNode)
	{
		_targetNode->parentNode = _currentNode;
		return true;
	}

	int diffx = abs(_currentNode->getXNumTile() - initial_tile->getXNumTile());
	int diffy = abs(_currentNode->getYNumTile() - initial_tile->getYNumTile());

	//check if testing node is on closed list
	if(!IsInAStarList(_closedList, _testingNode))
	{
		if(IsInAStarList(_openList, _testingNode))
		{
			//int newCost = _currentNode->aStar_movementCost + ASTAR_MOVECOST;
			int newCost = _currentNode->aStar_movementCost + _currentNode->getAStarUniqueCost();

			if(diffx <= 1 && diffy <= 1)
			{
				newCost += _currentNode->aStar_aditionalCost;
			}

			if(newCost < _testingNode->aStar_movementCost)
			{
				_testingNode->parentNode = _currentNode;
				_testingNode->aStar_movementCost = newCost;
				_testingNode->CalculateFValue();
			}
		}
		else
		{
			//int newCost = _currentNode->aStar_movementCost + ASTAR_MOVECOST;
			int newCost = _currentNode->aStar_movementCost + _currentNode->getAStarUniqueCost();

			if(diffx <= 1 && diffy <= 1)
			{
				newCost += _currentNode->aStar_aditionalCost;
			}

			_testingNode->parentNode = _currentNode;
			_testingNode->aStar_movementCost = newCost;
			_testingNode->CalculateFValue();
			AddToAStarList(_openList, _testingNode);
		}
	}
	return false;
}

IsoTerrain *AStarHelper::AStarGetSmallestFValueNode(IsoTerrain **_list)
{
	int smallestValue = INT_MAX;
	IsoTerrain *smallestNode = NULL;
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		if(_list[i] && _list[i]->aStar_totalCost < smallestValue)
		{
			smallestValue = _list[i]->aStar_totalCost;
			smallestNode = _list[i];
		}
	}
	return smallestNode;
}

bool AStarHelper::IsInAStarList(IsoTerrain **_list, IsoTerrain *_node)
{
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		if(_list[i] && _list[i] == _node)
		{
			return true;
		}
	}
	return false;
}

void AStarHelper::AddToAStarList(IsoTerrain **_list, IsoTerrain *_node)
{
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		if(!_list[i])
		{
			_list[i] = _node;
			return;
		}
	}
}

void AStarHelper::RemoveFromAStarList(IsoTerrain **_list, IsoTerrain *_node)
{
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		if(_list[i] && _list[i] == _node)
		{
			_list[i] = NULL;
		}
	}
}

void AStarHelper::DumpAStarList(IsoTerrain **_list, bool _bIsOpenList)
{
	for(int i = 0; i < ASTAR_LISTSMAX; i++)
	{
		if(_list[i])
		{
			IwTrace(ASTAR, ("DumpAStarList() called, node[%i][%i] OpenList %i", _list[i]->getXNumTile(), _list[i]->getYNumTile(), _bIsOpenList));
		}
	}
}

bool AStarHelper::AStarRetrievePathByRectangle(IsoTerrain *tileList[][MAX_TILESROW], IsoTerrain *tileInit, IsoTerrain *tileFinal, IsoTerrain **resultList, int range)
{
	IsoTerrain *rectangleTileList[MAX_TILESROW][MAX_TILESROW];
	for(int i = 0; i < MAX_TILESROW; i++)
	{
		for(int j = 0; j < MAX_TILESROW; j++)
			rectangleTileList[i][j] = NULL;
	}

	for(int i = 0; i < ASTAR_LISTSMAX; i++)
		resultList[i] = NULL;

	if(!tileInit || !tileFinal || (tileInit==tileFinal))
		return false;
	
	CIwVec2 InitTile = CIwVec2(tileInit->getXNumTile(), tileInit->getYNumTile());
	CIwVec2 FinalTile = CIwVec2(tileFinal->getXNumTile(), tileFinal->getYNumTile());

	CIwVec2 StartTile = CIwVec2(min(InitTile.x, FinalTile.x), min(InitTile.y, FinalTile.y));
	CIwVec2 EndTile   = CIwVec2(max(InitTile.x, FinalTile.x), max(InitTile.y, FinalTile.y));

	StartTile.x -= range; if(StartTile.x < 0) StartTile.x = 0;
	StartTile.y -= range; if(StartTile.y < 0) StartTile.y = 0;
	EndTile.x   += range; if(EndTile.x >= MAX_TILESROW) EndTile.x = MAX_TILESROW - 1;
	EndTile.y   += range; if(EndTile.y >= MAX_TILESROW) EndTile.y = MAX_TILESROW - 1;

	int TilesRows = EndTile.y - StartTile.y;
	int TilesCols = EndTile.x - StartTile.x;

	IwTrace(APPLOG, ("%i %i", StartTile.x, StartTile.y));
	IwTrace(APPLOG, ("%i %i", EndTile.x, EndTile.y));

	for(int i = 0; i < TilesCols; i++)
	{
		for(int j = 0; j < TilesRows; j++) 
		{
			rectangleTileList[i][j] = tileList[StartTile.x + i][StartTile.y + j];
			//IwTrace(APPLOG, ("(%i, %i)", rectangleTileList[i][j]->getXNumTile(), rectangleTileList[i][j]->getYNumTile()));
		}
	}

	bool bFoundTarget = false;

	return AStarRetrievePathByGrid(rectangleTileList, tileInit, tileFinal, resultList, TilesCols, TilesRows);
}