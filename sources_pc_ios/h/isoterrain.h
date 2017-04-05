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

#ifndef _ISO_TERRAIN_H_
#define _ISO_TERRAIN_H_

#include "astarhelper.h"
#include "isoobject.h"
#include "timer.h"

class IsoSelknam;

class IsoTerrain : public IsoObject
{
protected:
	int TileNum;
	int XNumTile;
	int YNumTile;
	// pre-calculated data
	bool bIsWalkeable;
	bool bIsWalkeableFromNorth;
	bool bIsWalkeableFromSouth;
	bool bIsWalkeableFromEast;
	bool bIsWalkeableFromWest;
	bool bIsRamp;
	bool bRampConnOnNorth;
	bool bRampConnOnSouth;
	bool bRampConnOnEast;
	bool bRampConnOnWest;
	bool bIsInitPoint;
	int InitPointNum;
	bool bIsFinalPoint;
	int FinalPointNum;

	bool bIsInitCursorPoint;
	bool bCanNavigate;
	bool bCanPutUnit;
	bool bCanPutUnitInitValue;
	bool bIntentionalCantPutUnit;
	bool bCanEnemyStopHere;

	bool bIsGreen;
	bool bIsWater_a;
	bool bIsWater_b;

public:
	IsoTerrain();
	~IsoTerrain();

	int getTileNum() const { return TileNum; }

	void setXNumTile(const int _XNumTile){ XNumTile = _XNumTile; }
	int getXNumTile() const	{ return XNumTile; }
	void setYNumTile(const int _YNumTile){ YNumTile = _YNumTile; }
	int getYNumTile() const	{ return YNumTile; }

	void setIsWalkeable(const bool _bIsWalkeable) { bIsWalkeable = _bIsWalkeable; }
	bool getIsWalkeable() const { return bIsWalkeable; }

	void setIsWalkeableFromNorth(const bool _bIsWalkeableFromNorth)	{ bIsWalkeableFromNorth = _bIsWalkeableFromNorth; }
	bool getIsWalkeableFromNorth() const { return bIsWalkeableFromNorth; }

	void setIsWalkeableFromSouth(const bool _bIsWalkeableFromSouth) { bIsWalkeableFromSouth = _bIsWalkeableFromSouth; }
	bool getIsWalkeableFromSouth() const { return bIsWalkeableFromSouth; }

	void setIsWalkeableFromEast(const bool _bIsWalkeableFromEast) { bIsWalkeableFromEast = _bIsWalkeableFromEast; }
	bool getIsWalkeableFromEast() const { return bIsWalkeableFromEast; }

	void setIsWalkeableFromWest(const bool _bIsWalkeableFromWest) { bIsWalkeableFromWest = _bIsWalkeableFromWest; }
	bool getIsWalkeableFromWest() const { return bIsWalkeableFromWest; }

	void setIsRamp(const bool _bIsRamp) { bIsRamp = _bIsRamp; }
	bool getIsRamp() const { return bIsRamp; }

	void setRampConnOnNorth(const bool _bRampConnOnNorth) { bRampConnOnNorth = _bRampConnOnNorth; }
	bool getRampConnOnNorth() const { return bRampConnOnNorth; }

	void setRampConnOnSouth(const bool _bRampConnOnSouth) { bRampConnOnSouth = _bRampConnOnSouth; }
	bool getRampConnOnSouth() const { return bRampConnOnSouth; }

	void setRampConnOnEast(const bool _bRampConnOnEast) { bRampConnOnEast = _bRampConnOnEast; }
	bool getRampConnOnEast() const { return bRampConnOnEast; }

	void setRampConnOnWest(const bool _bRampConnOnWest) { bRampConnOnWest = _bRampConnOnWest; }
	bool getRampConnOnWest() const { return bRampConnOnWest; }

	void setIsInitPoint(const bool _bIsInitPoint) { bIsInitPoint = _bIsInitPoint; }
	bool getIsInitPoint() const { return bIsInitPoint; }

	void setInitPointNum(const int _InitPointNum) { InitPointNum = _InitPointNum; }
	int getInitPointNum() const { return InitPointNum; }

	void setIsFinalPoint(const bool _bIsFinalPoint) { bIsFinalPoint = _bIsFinalPoint; }
	bool getIsFinalPoint() const { return bIsFinalPoint; }

	void setFinalPointNum(const int _FinalPointNum) { FinalPointNum = _FinalPointNum; }
	int getFinalPointNum() const { return FinalPointNum; }

	void setInitCursorPoint(const bool _bIsInitCursorPoint) { bIsInitCursorPoint = _bIsInitCursorPoint; }
	bool getInitCursorPoint() const { return bIsInitCursorPoint; }

	void setCanNavigate(const bool _bCanNavigate) { bCanNavigate = _bCanNavigate; }
	bool getCanNavigate() const { return bCanNavigate; }

	void setCanPutUnit(const bool _bCanPutUnit) { bCanPutUnit = _bCanPutUnit; }
	bool getCanPutUnit() const { return bCanPutUnit; }

	bool GetCanPutUnitInitValue() const { return bCanPutUnitInitValue; }
	void SetCanPutUnitInitValue(bool val) { bCanPutUnitInitValue = val; }

	void setAStarUniqueCost(const int _aStar_uniqueCost) { aStar_uniqueCost = _aStar_uniqueCost; }
	int getAStarUniqueCost() const { return aStar_uniqueCost; }

	void setIsGreen(const bool _bIsGreen) { bIsGreen = _bIsGreen; }
	bool getIsGreen() const { return bIsGreen; }

	void setIntentionalCantPutUnit(const bool _bIntentionalCantPutUnit) { bIntentionalCantPutUnit = _bIntentionalCantPutUnit; }
	bool getIntentionalCantPutUnit() const { return bIntentionalCantPutUnit; }

	void setIsWater_a(const bool _bIsWater_a) { bIsWater_a = _bIsWater_a; }
	bool getIsWater_a() const { return bIsWater_a; }

	void setIsWater_b(const bool _bIsWater_b) { bIsWater_b = _bIsWater_b; }
	bool getIsWater_b() const { return bIsWater_b; }

	void setEnemyCanStopHere(bool _bEnemyCanStopHere) { bCanEnemyStopHere = _bEnemyCanStopHere; };
	bool getEnemyCanStopHere() { return bCanEnemyStopHere; };

	virtual void InitIso();
	virtual void CleanIso();

	void SetTileNum(int tileNum);
	void DetectAdjacentPlaneNodes(IsoTerrain **isoTerrainArray, int _count);
	void DetectAdjacentRampNodes(IsoTerrain **isoTerrainArray, int _count);
	//A-Star
	AStarHelper *aStarHelperRef;
	void CalculateFValue();
	void CalculateFValue(int _numObjective);

	int aStar_heuristicValue;
	int aStar_movementCost;
	int aStar_totalCost;
	int aStar_aditionalCost;
	int aStar_uniqueCost;
	
	//path to objective
	IsoTerrain *isoPath[MAX_ASTAROBJECTIVES][ASTAR_LISTSMAX];
	void CleanPaths();

	IsoTerrain *parentNode;
	// adjacent nodes
	IsoTerrain *northNode;
	IsoTerrain *eastNode;
	IsoTerrain *southNode;
	IsoTerrain *westNode;

	//Temporal
	IsoSelknam* CurrentIsoSelknam;

	GameTimer timer_change_anim;
	void UpdateAnimation();

	int num_locks;
	int num_enemies_on_this_tile;
};

#endif // _ISO_TERRAIN_H_
