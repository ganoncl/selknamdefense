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
#include "astarhelper.h"
#include "isoobject.h"
#include "game.h"



#include "isoterrain.h"

IsoTerrain::IsoTerrain()
{
	CleanIso();
	timer_change_anim.SetTimeLimit(450);
}

IsoTerrain::~IsoTerrain()
{

}

void IsoTerrain::InitIso()
{
	SetIsoOffset(isoTerrainOffset);
}

void IsoTerrain::CleanIso()
{
	IsoObject::CleanIso();
	TileNum = -1;
	XNumTile = -1;
	YNumTile = -1;
	bIsWalkeable = false;
	bIsWalkeableFromNorth = false;
	bIsWalkeableFromSouth = false;
	bIsWalkeableFromEast = false;
	bIsWalkeableFromWest = false;
	bIsRamp = false;
	bRampConnOnNorth = false;
	bRampConnOnSouth = false;
	bRampConnOnEast = false;
	bRampConnOnWest = false;
	bIsInitPoint = false;
	InitPointNum = 0;
	bIsFinalPoint = false;
	FinalPointNum = 0;
	parentNode = NULL;
	northNode = NULL;
	eastNode = NULL;
	southNode = NULL;
	westNode = NULL;
	aStar_heuristicValue = 0;
	aStar_movementCost = 0;
	aStar_totalCost = 0;
	aStar_aditionalCost = 0;
	aStar_uniqueCost = 0;
	CleanPaths();
	CurrentIsoSelknam = NULL;
	bIsInitCursorPoint = false;
	bCanNavigate = false;
	bCanPutUnit = false;
	bCanPutUnitInitValue = false;
	bIsGreen = false;
	bIsWater_a = false;
	bIsWater_b = false;
	bIntentionalCantPutUnit = false;
	bCanEnemyStopHere = false;
	width = tileMapWidthImage;
	height = tileMapHeightImage;
	timer_change_anim.Reset();
	timer_change_anim.Stop();
	num_locks = 0;
	num_enemies_on_this_tile = 0;
}

void IsoTerrain::CleanPaths()
{
	for(int i = 0; i < MAX_ASTAROBJECTIVES; i++)
	{
		for(int j = 0; j < ASTAR_LISTSMAX; j++)
		{
			isoPath[i][j] = NULL;
		}
	}
}

void IsoTerrain::SetTileNum(int tileNum)
{
	float tmpUvMinX = 0.f;
	float tmpUvMaxX = 0.f;
	float tmpUvMinY = 0.f;
	float tmpUvMaxY = 0.f;
	float uvOffset = 0.125f;
	float uvOffsetY = 0.1111f;
	float myX=0.f;
	float myY=0.f;
	int resto = 0;
	int tmpTileNum = 0;

	tileNum = tileNum - 51;

	TileNum = tileNum;
	
	tmpTileNum = tileNum;

	tmpTileNum--;

	resto = tmpTileNum / 8;
	myY = float(resto);
	//IwTrace(APPLOG, ("InitMap() called, myY %f", myY));
	myX = (resto * 8.f);
	myX = tmpTileNum - myX;
	//IwTrace(APPLOG, ("InitMap() called, myX %f", myX));

	tmpUvMinX = myX * uvOffset;
	tmpUvMaxX = (myX + 1) * uvOffset;

	tmpUvMinY = myY * uvOffsetY;
	tmpUvMaxY = (myY + 1) * uvOffsetY;
	
	setUvMinX(tmpUvMinX);
	setUvMaxX(tmpUvMaxX);
	setUvMinY(tmpUvMinY);
	setUvMaxY(tmpUvMaxY);
}


void IsoTerrain::DetectAdjacentPlaneNodes(IsoTerrain **isoTerrainArray, int _count)
{
	int adjancentNodesNum = 0;
	int maxVXExpectedDist = (int)isoFormulaWValue;
	int maxVYExpectedDist = (int)tileDiffLayer;
	int dx = -1;
	int dy = -1;
	int myX = int(getIsoPosX());
	int myY = int(getIsoPosY());
	int myZ = int(getIsoPosZ());
	int otherX = -1;
	int otherY = -1;
	int otherZ = -1;
	
	//retrieve list of adjacent of plane nodes
	for(int i = 0; i < _count; i++)
	{
		//filter if null
		if(!isoTerrainArray[i])
		{
			continue;
		}
		bool bIsNotPlane = false;
		if(!isoTerrainArray[i]->getIsWalkeableFromNorth()
			|| !isoTerrainArray[i]->getIsWalkeableFromSouth()
			|| !isoTerrainArray[i]->getIsWalkeableFromEast()
			|| !isoTerrainArray[i]->getIsWalkeableFromWest())
		{
			bIsNotPlane = true;
		}

		if(!bIsNotPlane && (isoTerrainArray[i]->getLayer() != getLayer()))
			continue;

		otherX = int(isoTerrainArray[i]->getIsoPosX());
		otherY = int(isoTerrainArray[i]->getIsoPosY());
		otherZ = int(isoTerrainArray[i]->getIsoPosZ());
		
		if(!bIsNotPlane && myZ != otherZ)
			continue;

		dx = abs(myX - otherX);
		dy = abs(myY - otherY);
		if((dx <= maxVXExpectedDist) && (dy <= maxVYExpectedDist))
		{
			if(dx!=0 && dy!=0)
			{
				
				dx = myX - otherX;
				dy = myY - otherY;
				//determine if the node is north, south, east or west
				//IwTrace(APPLOG, ("dx %i dy %i", dx, dy));
				//west node
				if(dx > 0 && dy > 0)
				{
					if(isoTerrainArray[i]->getIsWalkeableFromEast())
					{
						if(isoTerrainArray[i]->getIsRamp())
						{
							if( (isoTerrainArray[i]->getRampConnOnEast() && (getLayer() + 1 == isoTerrainArray[i]->getLayer())) ||
								(!isoTerrainArray[i]->getRampConnOnEast() && (getLayer() == isoTerrainArray[i]->getLayer()))   )
							{
								westNode = isoTerrainArray[i];
							}
						}
						else westNode = isoTerrainArray[i];
					}
				}
				//south node
				else if(dx > 0 && dy < 0)
				{
					if(isoTerrainArray[i]->getIsWalkeableFromNorth())
					{
						if(isoTerrainArray[i]->getIsRamp())
						{
							if( (isoTerrainArray[i]->getRampConnOnNorth() && (getLayer() + 1== isoTerrainArray[i]->getLayer())) ||
								(!isoTerrainArray[i]->getRampConnOnNorth() && (getLayer() == isoTerrainArray[i]->getLayer()))   )
							{
								southNode = isoTerrainArray[i];
							}
						}
						else southNode = isoTerrainArray[i];
					}
				}
				//east node
				else if(dx < 0 && dy < 0)
				{
					if(isoTerrainArray[i]->getIsWalkeableFromWest())
					{
						if(isoTerrainArray[i]->getIsRamp())
						{
							if( (isoTerrainArray[i]->getRampConnOnWest() && (getLayer() + 1 == isoTerrainArray[i]->getLayer())) ||
								(!isoTerrainArray[i]->getRampConnOnWest() && (getLayer() == isoTerrainArray[i]->getLayer()))   )
							{
								eastNode = isoTerrainArray[i];
							}
						}
						else eastNode = isoTerrainArray[i];
					}
				}
				//north node
				else if(dx < 0 && dy > 0)
				{
					if(isoTerrainArray[i]->getIsWalkeableFromSouth())
					{
						if(isoTerrainArray[i]->getIsRamp())
						{
							if( (isoTerrainArray[i]->getRampConnOnSouth() && (getLayer() + 1 == isoTerrainArray[i]->getLayer())) ||
								(!isoTerrainArray[i]->getRampConnOnSouth() && (getLayer() == isoTerrainArray[i]->getLayer()))   )
							{
								northNode = isoTerrainArray[i];
							}
						}
						else 
						{
							northNode = isoTerrainArray[i];
						}
					}
					//IwTrace(APPLOG, ("Get Adjacency on north"));
				}
			}
		}
	}
}

void IsoTerrain::DetectAdjacentRampNodes(IsoTerrain **isoTerrainArray, int _count)
{
	int adjancentNodesNum = 0;
	int maxVXExpectedDist = (int)isoFormulaWValue;
	int maxVYExpectedDist = (int)tileDiffLayer;
	int dx = -1;
	int dy = -1;
	int myX = int(getIsoPosX());
	int myY = int(getIsoPosY());
	int myZ = int(getIsoPosZ());
	int otherX = -1;
	int otherY = -1;
	int otherZ = -1;

	//retrieve list of adjacent of plane nodes
	for(int i = 0; i < _count; i++)
	{
		//filter if null
		if(!isoTerrainArray[i])
			continue;
		//use only plane nodes of the same Z.
		else if(!isoTerrainArray[i]->getIsWalkeableFromNorth()
			|| !isoTerrainArray[i]->getIsWalkeableFromSouth()
			|| !isoTerrainArray[i]->getIsWalkeableFromEast()
			|| !isoTerrainArray[i]->getIsWalkeableFromWest())
		{
			if(!isoTerrainArray[i]->getIsRamp())
				continue;
		}

		otherX = int(isoTerrainArray[i]->getIsoPosX());
		otherY = int(isoTerrainArray[i]->getIsoPosY());

	
		dx = abs(myX - otherX);
		dy = abs(myY - otherY);
		if((dx <= maxVXExpectedDist) && (dy <= maxVYExpectedDist))
		{
			if(dx!=0 && dy!=0)
			{
				dx = myX - otherX;
				dy = myY - otherY;
				//determine if the node is north, south, east or west
				//IwTrace(APPLOG, ("dx %i dy %i", dx, dy));
				//west node
				if(dx > 0 && dy > 0 && getIsWalkeableFromWest())
				{
					if((getRampConnOnWest() && (getLayer() - 1 == isoTerrainArray[i]->getLayer())) ||
					   (!getRampConnOnWest() && (getLayer() == isoTerrainArray[i]->getLayer()   )) ||
					   (isoTerrainArray[i]->getIsRamp() && !getRampConnOnWest() && (isoTerrainArray[i]->getLayer() - 1 == getLayer())))// ||
					   //(isoTerrainArray[i]->getIsRamp() && !getRampConnOnWest() && !isoTerrainArray[i]->getRampConnOnEast() && (getLayer() == isoTerrainArray[i]->getLayer())) ||
					   //(isoTerrainArray[i]->getIsRamp() && getRampConnOnWest() && isoTerrainArray[i]->getRampConnOnEast() && (getLayer() == isoTerrainArray[i]->getLayer())))
					{
						westNode = isoTerrainArray[i];
					}
					//IwTrace(APPLOG, ("Get Adjacency on west"));
				}
				//south node
				else if(dx > 0 && dy < 0 && getIsWalkeableFromSouth())
				{
					if((getRampConnOnSouth() && (getLayer() - 1 == isoTerrainArray[i]->getLayer())) ||
						(!getRampConnOnSouth() && (getLayer() == isoTerrainArray[i]->getLayer()   )) ||
						(isoTerrainArray[i]->getIsRamp() && !getRampConnOnSouth() && (isoTerrainArray[i]->getLayer() - 1 == getLayer())))// ||
						//(isoTerrainArray[i]->getIsRamp() && !getRampConnOnSouth() && !isoTerrainArray[i]->getRampConnOnNorth() && (getLayer() == isoTerrainArray[i]->getLayer())) ||
						//(isoTerrainArray[i]->getIsRamp() && getRampConnOnSouth() && isoTerrainArray[i]->getRampConnOnNorth() && (getLayer() == isoTerrainArray[i]->getLayer())))
					{
						southNode = isoTerrainArray[i];
					}
					//IwTrace(APPLOG, ("Get Adjacency on south"));
				}
				//east node
				else if(dx < 0 && dy < 0 && getIsWalkeableFromEast())
				{
					if((getRampConnOnEast() && (getLayer() - 1 == isoTerrainArray[i]->getLayer())) ||
						(!getRampConnOnEast() && (getLayer() == isoTerrainArray[i]->getLayer()   )) ||
						(isoTerrainArray[i]->getIsRamp() && !getRampConnOnEast() && (isoTerrainArray[i]->getLayer() - 1 == getLayer()))) //||
						//(isoTerrainArray[i]->getIsRamp() && !getRampConnOnEast() && !isoTerrainArray[i]->getRampConnOnWest() && (getLayer() == isoTerrainArray[i]->getLayer())) ||
						//(isoTerrainArray[i]->getIsRamp() && getRampConnOnEast() && isoTerrainArray[i]->getRampConnOnWest() && (getLayer() == isoTerrainArray[i]->getLayer())))
					{
						eastNode = isoTerrainArray[i];
					}
					//IwTrace(APPLOG, ("Get Adjacency on east"));
				}
				//north node
				else if(dx < 0 && dy > 0 && getIsWalkeableFromNorth())
				{
					if((getRampConnOnNorth() && (getLayer() - 1 == isoTerrainArray[i]->getLayer()))  ||
						(!getRampConnOnNorth() && (getLayer() == isoTerrainArray[i]->getLayer()   )) ||
						(isoTerrainArray[i]->getIsRamp() && !getRampConnOnNorth() && (isoTerrainArray[i]->getLayer() - 1 == getLayer()))) //||
						//(isoTerrainArray[i]->getIsRamp() && !getRampConnOnNorth() && !isoTerrainArray[i]->getRampConnOnSouth() && (getLayer() == isoTerrainArray[i]->getLayer())) ||
						//(isoTerrainArray[i]->getIsRamp() && getRampConnOnNorth() && isoTerrainArray[i]->getRampConnOnSouth() && (getLayer() == isoTerrainArray[i]->getLayer())))
					{
						northNode = isoTerrainArray[i];
					}
					//IwTrace(APPLOG, ("Get Adjacency on north"));
				}
			}
		}
	}
}

void IsoTerrain::CalculateFValue()
{
	aStar_totalCost = aStar_movementCost + aStar_heuristicValue;
}

void IsoTerrain::UpdateAnimation()
{
	if(!getIsWater_a() && !getIsWater_b())
		return;

	timer_change_anim.Update();

	if(timer_change_anim.GetIsStop())
	{
		timer_change_anim.Reset();
		timer_change_anim.Start();
	}

	if(!timer_change_anim.GetIsEnd()) 
		return;

	if(getIsWater_b())
	{
		setUvMinY(getUvMinY() - (9 / 128.0f));
		setUvMaxY(getUvMaxY() - (9 / 128.0f));
		setUvMinX(9 * (3 / 32.0f));
		setUvMaxX(10 * (3 / 32.0f));
		setIsWater_a(true);
		setIsWater_b(false);
	}
	else if(getIsWater_a())
	{
		setUvMinY(getUvMinY() + (9 / 128.0f));
		setUvMaxY(getUvMaxY() + (9 / 128.0f));
		setUvMinX(0 * (3 / 32.0f));
		setUvMaxX(1 * (3 / 32.0f));
		setIsWater_a(false);
		setIsWater_b(true);
	}

	timer_change_anim.Reset();
	timer_change_anim.Start();
};