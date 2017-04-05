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

#ifndef _MAP_POINTER_H
#define _MAP_POINTER_H

#include "IwGx.h"
#include "s3e.h"
#include "isoobject.h"
#include "isoterrain.h"
#include "texturehelper.h"
#include "timer.h"

//Sprite showing the focus position on the map
#define DELAY_TIME 120
#define DECREASE_DELAY_FACTOR 10

enum MapPointerState
{
	MAP_POINTER_RED,
	MAP_POINTER_BLUE
};

class MapPointer : public IsoObject
{
public:
	MapPointer();
	~MapPointer();
	virtual void InitIso();
	virtual void Update();
	virtual void CleanIso();
	void Render();
	void SetState(MapPointerState mapPointerState);
	int getXNumTile() { return xNumTile; };
	int getYNumTile() { return yNumTile; };
	void SetGrid(IsoTerrain* tile);
	void SetContinuousMovement(bool bLoop);
	void SetIsActive(bool bIsActive) { this->bIsActive = bIsActive; };
	bool GetIsMouseOnAGrid() { return bIsMouseOnAGrid; };
	CIwVec2 GetTileByMouseCoords(float mouseX, float mouseY, int layer);
	void SetCursorGrid(IsoTerrain *CursorGrid[MAX_TILESROW][MAX_TILESROW]);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateClickEvents(s3ePointerEvent *event);
	void KeyboardHandler(s3eKeyboardEvent* event);
	IsoTerrain* findAvailableGridForPointer(int despX, int despY);
	bool GetIsActive() { return bIsActive; };
	IsoTerrain* GetIsoTerrain();

	void SetMovementCursor(bool b)
	{
		bShowMovementCursor = b;
	}

private:
	int xNumTile;
	int yNumTile;
	//delay between the action of change pointer position, miliseconds
	bool bFirstMove;
	bool bLoop;
	int64 delay; 
	int64 vel_delay;
	int64 time;
	int64 currTime;
	int64 lastTime;
	bool bIsActive;
	bool bIsMouseOnAGrid;

	float pointerAdjustX[10];
	float pointerAdjustY[10];

	IsoTerrain *CursorGrid[MAX_TILESROW][MAX_TILESROW];

	bool bKeyRightPressed;
	bool bKeyLeftPressed;
	bool bKeyUpPressed;
	bool bKeyDownPressed;
	s3eKey lastKeyPressed;

	Timer delay_movement;
	bool bMoveOnce;
	bool bSetMovement;

	float uvX;
	float uvY;

	Timer animation_delay;
	int current_uvX;

	float _h;
	float delta_off_x;
	float delta_off_y;

	bool bIsBlueState;
	TextureData cursor_map_data;
	AtlasName cursor_map_atlasname;

	TextureData cursor_movement_data;
	AtlasName cursor_movement_atlasname;

	bool bShowMovementCursor;
	int movementCursorXid, movementCursorYid;
	bool bCanPutUnit;
	CIwColour redColor[4];
};

#endif