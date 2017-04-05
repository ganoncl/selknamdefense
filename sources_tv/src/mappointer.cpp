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

#include "mappointer.h"
#include "timer.h"
#include "utils.h"

MapPointer::MapPointer()
{
	setWidth(64);
	setHeight(32);
	setUvMaxX(0.5f);
	setUvMaxY(1.0f);
	setUvMinX(0.0f);
	setUvMinY(0.0f);
	bLoop = false;
	delay = DELAY_TIME;
	vel_delay = DECREASE_DELAY_FACTOR;
	bFirstMove = true;
	bIsActive = true;
	bIsMouseOnAGrid = false;

	cursor_map_data = TextureHelper::GetTextureNamed("cursor_map");
	cursor_map_atlasname = TextureHelper::GetAtlasName(cursor_map_data);
	
	cursor_movement_data = TextureHelper::GetTextureNamed("cursor_movement");
	cursor_movement_data.textureRotated = true;
	cursor_movement_atlasname = TextureHelper::GetAtlasName(cursor_movement_data);
	bShowMovementCursor = false;
	movementCursorXid = movementCursorYid = 0;

	for(int i = 0; i < 10; i++)
	{
		pointerAdjustX[i] = (float)(-((608 * 16) + (80 - i * 16) * 32) / (2 * 16 * 32));
		pointerAdjustY[i] = (float)( ((608 * 16) - (80 - i * 16) * 32) / (2 * 16 * 32));
	}

	//Offset
	pointerAdjustX[0] -= (float)1.3; 
	pointerAdjustY[0] -= (float)0.4;

	pointerAdjustX[1] -= (float)2.0;
	pointerAdjustY[1] -= (float)0.0;

	pointerAdjustX[2] -= (float)1.4;
	pointerAdjustY[2] -= (float)0.6;

	pointerAdjustX[3] -= (float)2.0; 
	pointerAdjustY[3] -= (float)0.0;

	pointerAdjustX[4] -= (float)1.2;
	pointerAdjustY[4] -= (float)0.6;

	pointerAdjustX[5] -= (float)1.8;
	pointerAdjustY[5] -= (float)0.0;

	pointerAdjustX[6] -= (float)1.4;
	pointerAdjustY[6] -= (float)0.6;

	pointerAdjustX[7] -= (float)1.8;
	pointerAdjustY[7] -= (float)0.0;

	pointerAdjustX[8] -= (float)1.4;
	pointerAdjustY[8] -= (float)0.6;

	pointerAdjustX[9] -= (float)2.2;
	pointerAdjustY[9] -= (float)0.0;

	bKeyDownPressed = false;
	bKeyLeftPressed = false;
	bKeyRightPressed = false;
	bKeyUpPressed = false;

	delay_movement.SetTimeLimit(DELAY_BEFORE_MOUSE_BEGIN_TO_MOVE);

	uvX = 0.25f;
	//uvY = 0.1667f;
	uvY = 0.1f;

	animation_delay.SetTimeLimit(100);

	
	setUvMaxX(uvX * 0);
	setUvMaxY(uvY * 2);
	setUvMinX(uvX * 1);
	setUvMinY(uvY * 1);
	
	/*
	setUvMaxX(uvX * 0);
	setUvMaxY(uvY * 1);
	setUvMinX(uvX * 1);
	setUvMinY(uvY * 0);
	*/
	current_uvX = 0;

	SetState(MAP_POINTER_BLUE);

	redColor[0].Set(255, 0, 0);
	redColor[1].Set(255, 0, 0);
	redColor[2].Set(255, 0, 0);
	redColor[3].Set(255, 0, 0);
}

MapPointer::~MapPointer()
{

}

void MapPointer::InitIso()
{
	delay = DELAY_TIME;
	bLoop = false;
	bFirstMove = true;
	bSetMovement = false;
	bMoveOnce = false;
}

void MapPointer::Update()
{
	if(!bIsActive)
		return;

	animation_delay.Update();
	if(animation_delay.GetIsStop() || animation_delay.GetIsEnd())
	{
		animation_delay.Reset();
		animation_delay.Start();
		current_uvX++;

		
		movementCursorXid++;
		if(movementCursorXid > 4) movementCursorXid = 0;
		
		/*
		movementCursorYid++;
		if(movementCursorYid > 4) movementCursorYid = 0;
		*/
		if(bIsBlueState)
		{
			if(current_uvX >= 3) current_uvX = 0;
		}
		else if(current_uvX >= 4) current_uvX = 0;
		//current_uvX = 0;
		
		setUvMaxX(uvX * (current_uvX + 1));
		setUvMinX(uvX * current_uvX);
		
		IwTrace(POINTERTRACE, ("CURRENT_UVX: %d", current_uvX));
	}
	

	delay_movement.Update();
	//IwTrace(APPLOG, ("POINTER TIME: %d", delay_movement.GetCurrTime_Reversed()));

	if(delay_movement.GetIsEnd()) {
		bSetMovement = true;
		//IwTrace(APPLOG, ("Ready to move..."));
	}

	if(bSetMovement || bMoveOnce)
	{
		if(bKeyRightPressed)     SetGrid(findAvailableGridForPointer(0, -1));
		else if(bKeyLeftPressed) SetGrid(findAvailableGridForPointer(0, 1));
		else if(bKeyUpPressed)   SetGrid(findAvailableGridForPointer(-1, 0));
		else if(bKeyDownPressed) SetGrid(findAvailableGridForPointer(1, 0));
		bMoveOnce = false;
	}

	
	if(gameState == GS_Game && GetIsoTerrain() != NULL)
		SetGrid(GetIsoTerrain());


	if(!bLoop)
		return;

	lastTime = currTime;
	currTime = s3eTimerGetMs();
	time += currTime - lastTime;

}

void MapPointer::CleanIso()
{
	bCanPutUnit = true;
}

void MapPointer::Render()
{
	if(!bShowMovementCursor)
	{
		CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		pMat->SetTexture(TextureHelper::GetCIwTexture(cursor_map_atlasname));
		IwGxSetMaterial(pMat);

		float offsetX = 32.0f + delta_off_x;
		float offsetY = 16.0f + delta_off_y;

		static CIwFVec3 s_Quad[4];
		if(!cursor_map_data.textureRotated)
		{
			s_Quad[0].x = getPosX() - offsetX;
			s_Quad[0].y = getPosY() - offsetY;
			s_Quad[0].z = getPosZ();

			s_Quad[1].x = getPosX() - offsetX;
			s_Quad[1].y = getPosY() + (_h - offsetY);
			s_Quad[1].z = getPosZ();

			s_Quad[2].x = getPosX() + (getWidth() - offsetX);
			s_Quad[2].y = getPosY() + (_h - offsetY);
			s_Quad[2].z = getPosZ();

			s_Quad[3].x = getPosX() + (getWidth() - offsetX);
			s_Quad[3].y = getPosY() - offsetY ;
			s_Quad[3].z = getPosZ();
		}
		else
		{
			s_Quad[3].x = getPosX() - offsetX;
			s_Quad[3].y = getPosY() - offsetY;
			s_Quad[3].z = getPosZ();

			s_Quad[0].x = getPosX() - offsetX;
			s_Quad[0].y = getPosY() + (_h - offsetY);
			s_Quad[0].z = getPosZ();

			s_Quad[1].x = getPosX() + (getWidth() - offsetX);
			s_Quad[1].y = getPosY() + (_h - offsetY);
			s_Quad[1].z = getPosZ();

			s_Quad[2].x = getPosX() + (getWidth() - offsetX);
			s_Quad[2].y = getPosY() - offsetY;
			s_Quad[2].z = getPosZ();
		}

		IwGxSetVertStreamModelSpace(s_Quad, 4);
		/*
		int alpha = (int)(255 * (1 - banishTime.GetCurrTime() / (float)banishTime.GetTimeLimit()));

		static CIwColour tmpColor[4];
		tmpColor[0].Set(255, 255, 255, alpha);
		tmpColor[1].Set(255, 255, 255, alpha);
		tmpColor[2].Set(255, 255, 255, alpha);
		tmpColor[3].Set(255, 255, 255, alpha);
		*/
		IwGxSetColStream(NULL);

		static CIwFVec2 s_UVs[4];

		float uvMinX = ((getUvMinX()) * cursor_map_data.textureWidth + cursor_map_data.textureXPos) / (float)cursor_map_data.atlasWidth;
		float uvMinY = ((getUvMinY()) * cursor_map_data.textureHeight  + cursor_map_data.textureYPos) / (float)cursor_map_data.atlasHeight;
		float uvMaxX = ((getUvMaxX()) * cursor_map_data.textureWidth + cursor_map_data.textureXPos) / (float)cursor_map_data.atlasWidth;
		float uvMaxY = ((getUvMaxY()) * cursor_map_data.textureHeight  + cursor_map_data.textureYPos) / (float)cursor_map_data.atlasHeight;

		s_UVs[0] = CIwFVec2(uvMinX, uvMinY);
		s_UVs[1] = CIwFVec2(uvMinX, uvMaxY);
		s_UVs[2] = CIwFVec2(uvMaxX, uvMaxY);
		s_UVs[3] = CIwFVec2(uvMaxX, uvMinY);

		IwGxSetUVStream(s_UVs);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
	}
	else
	{
		CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		pMat->SetTexture(TextureHelper::GetCIwTexture(cursor_movement_atlasname));
		IwGxSetMaterial(pMat);

		float offsetX = 24.0f;
		float offsetY = 16.0f;
		float _height = 40.0f;
		float _width = 48.0f;

		static CIwFVec3 s_Quad[4];
		if(!cursor_movement_data.textureRotated)
		{
			s_Quad[0].x = getPosX() - offsetX;
			s_Quad[0].y = getPosY() - offsetY;
			s_Quad[0].z = getPosZ();

			s_Quad[1].x = getPosX() - offsetX;
			s_Quad[1].y = getPosY() + _height - offsetY;
			s_Quad[1].z = getPosZ();

			s_Quad[2].x = getPosX() + _width - offsetX;
			s_Quad[2].y = getPosY() + _height - offsetY;
			s_Quad[2].z = getPosZ();

			s_Quad[3].x = getPosX() + _width - offsetX;
			s_Quad[3].y = getPosY() - offsetY;
			s_Quad[3].z = getPosZ();
		}
		else
		{
			s_Quad[3].x = getPosX() - offsetX;
			s_Quad[3].y = getPosY() - offsetY;
			s_Quad[3].z = getPosZ();

			s_Quad[0].x = getPosX() - offsetX;
			s_Quad[0].y = getPosY() + _height - offsetY;
			s_Quad[0].z = getPosZ();

			s_Quad[1].x = getPosX() + _width - offsetX;
			s_Quad[1].y = getPosY() + _height - offsetY;
			s_Quad[1].z = getPosZ();

			s_Quad[2].x = getPosX() + _width - offsetX;
			s_Quad[2].y = getPosY() - offsetY;
			s_Quad[2].z = getPosZ();
		}

		IwGxSetVertStreamModelSpace(s_Quad, 4);

		if(bCanPutUnit)
		{
			IwGxSetColStream(NULL);
		}
		else
		{
			IwGxSetColStream(redColor, 4);
		}
		

		float uvX = 0.2f;
		float uvY = 0.2f;

		static CIwFVec2 s_UVs[4];
		/*
		float uvMinX = ((uvX * movementCursorYid) * cursor_movement_data.textureHeight + cursor_movement_data.textureYPos) / (float)cursor_movement_data.atlasHeight;
		float uvMinY = ((uvY * movementCursorXid) * cursor_movement_data.textureWidth  + cursor_movement_data.textureXPos) / (float)cursor_movement_data.atlasWidth;
		float uvMaxX = ((uvX * (movementCursorYid + 1)) * cursor_movement_data.textureHeight + cursor_movement_data.textureYPos) / (float)cursor_movement_data.atlasHeight;
		float uvMaxY = ((uvY * (movementCursorXid + 1)) * cursor_movement_data.textureWidth  + cursor_movement_data.textureXPos) / (float)cursor_movement_data.atlasWidth;
		*/
		float uvMinX, uvMinY, uvMaxX, uvMaxY;

		if(cursor_movement_data.textureRotated)
		{
			movementCursorYid = 4 - movementCursorYid;
			//IwTrace(dsadsad, ("%d %d", movementMarkData.textureWidth, movementMarkData.textureHeight));
			//movementCursorXid = movementCursorYid = 0;
			uvMinX = ((uvX * movementCursorXid) * cursor_movement_data.textureHeight + cursor_movement_data.textureYPos) / (float)cursor_movement_data.atlasWidth;
			uvMinY = ((uvY * movementCursorYid) * cursor_movement_data.textureWidth  + cursor_movement_data.textureXPos) / (float)cursor_movement_data.atlasHeight;
			uvMaxX = ((uvX * (movementCursorXid + 1)) * cursor_movement_data.textureHeight + cursor_movement_data.textureYPos) / (float)cursor_movement_data.atlasWidth;
			uvMaxY = ((uvY * (movementCursorYid + 1)) * cursor_movement_data.textureWidth  + cursor_movement_data.textureXPos) / (float)cursor_movement_data.atlasHeight;
		}
		else
		{
			uvMinX = ((uvX * movementCursorXid) * cursor_movement_data.textureWidth + cursor_movement_data.textureXPos) / (float)cursor_movement_data.atlasWidth;
			uvMinY = ((uvY * movementCursorYid) * cursor_movement_data.textureHeight  + cursor_movement_data.textureYPos) / (float)cursor_movement_data.atlasHeight;
			uvMaxX = ((uvX * (movementCursorXid + 1)) * cursor_movement_data.textureWidth + cursor_movement_data.textureXPos) / (float)cursor_movement_data.atlasWidth;
			uvMaxY = ((uvY * (movementCursorYid + 1)) * cursor_movement_data.textureHeight  + cursor_movement_data.textureYPos) / (float)cursor_movement_data.atlasHeight;
		}

		s_UVs[0] = CIwFVec2(uvMinY, uvMinX);
		s_UVs[1] = CIwFVec2(uvMinY, uvMaxX);
		s_UVs[2] = CIwFVec2(uvMaxY, uvMaxX);
		s_UVs[3] = CIwFVec2(uvMaxY, uvMinX);

		IwGxSetUVStream(s_UVs);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
	}
}

void MapPointer::SetState(MapPointerState mapPointerState)
{
	if(mapPointerState == MAP_POINTER_RED)
	{
		bCanPutUnit = false;
		if(!GetIsoTerrain()->getIsRamp())
		{
			//Normal tile
			setUvMaxY(uvY * 6);
			setUvMinY(uvY * 5 + uvY * (16.0f / 48.0f));
			_h = 32.0f;
			delta_off_x = 0.0f;
			delta_off_y = 0.0f;

			movementCursorYid = 0;
			//movementCursorXid = 0;
		}
		else
		{
			_h = 43.0f;
			if(GetIsoTerrain()->getIsWalkeableFromWest())
			{
				if(!GetIsoTerrain()->getRampConnOnWest())
				{
					setUvMaxY(uvY * 7 - uvY * (5.0f / 48.0f));
					setUvMinY(uvY * 6);
					delta_off_x = 0.0f;
					delta_off_y = 0.0f;
					movementCursorYid = 1;
					//movementCursorXid = 1;
				}
				else
				{
					_h = 48.0f - 16.0f - 6.7f;
					setUvMaxY(uvY * 10 - uvY * (16.0f / 48.0f));
					setUvMinY(uvY * 9 + uvY * (6.7f / 48.0f));
					delta_off_x = 0.0f;
					delta_off_y = -6.7f;
					movementCursorYid = 3;
					//movementCursorXid = 3;
				}
			}
			else
			{
				if(!GetIsoTerrain()->getRampConnOnNorth())
				{
					setUvMaxY(uvY * 8 - uvY * (5.0f / 48.0f));
					setUvMinY(uvY * 7);// + uvY * (16.0f / 48.0f));
					delta_off_x = 0.0f;
					delta_off_y = 0.0f;
					movementCursorYid = 2;
					//movementCursorXid = 2;
				}
				else
				{
					_h = 48.0f - 16.0f - 6.7f;
					setUvMaxY(uvY * 9 - uvY * (16.0f / 48.0f));
					setUvMinY(uvY * 8 + uvY * (6.7f / 48.0f));// + uvY * (16.0f / 48.0f));
					delta_off_x = 0.0f;
					delta_off_y = -6.7f;
					movementCursorYid = 4;
					//movementCursorXid = 4;
				}
			}
		}
		bIsBlueState = false;
	}
	else if(mapPointerState == MAP_POINTER_BLUE)
	{
		bCanPutUnit = true;
		_h = 32.0f;
		//setUvMaxX(uvX * 0);
		setUvMaxY(uvY * 1);
		//setUvMinX(uvX * 1);
		setUvMinY(uvY * (16.0f / 48.0f));
		delta_off_x = 0.0f;
		delta_off_y = 0.0f;
		bIsBlueState = true;
		movementCursorYid = 0;
		//movementCursorXid = 0;
	}
}

void MapPointer::SetGrid(IsoTerrain* tile)
{
	if(!bIsActive)
		return;
	
	bIsMouseOnAGrid = false;

	if(bLoop)
	{ 
		if(!bFirstMove)
		{
			if(time < delay)
				return;
			else
			{
				delay -= vel_delay;
				if(delay < 0) delay = 0;
				time = 0;
				currTime = s3eTimerGetMs();
				lastTime = s3eTimerGetMs();
			}
		}
		else 
		{
			bFirstMove = false;
			time = 0;
			currTime = s3eTimerGetMs();
			lastTime = s3eTimerGetMs();
		}
	}

	if(tile == NULL)
		return;

	if(!tile->getCanNavigate())
		return;
	//IwTrace(APPLOG, ("Moving pointer...(%i %i)", tile->getXNumTile(), tile->getYNumTile()));
	setPosition(tile->getPosX(), tile->getPosY(), tile->getPosZ() - 0.005f);
	xNumTile = tile->getXNumTile();
	yNumTile = tile->getYNumTile();
	if(tile->getCanPutUnit())
		SetState(MAP_POINTER_BLUE);
	else
		SetState(MAP_POINTER_RED);

	bIsMouseOnAGrid = true;
}

void MapPointer::SetContinuousMovement(bool bLoop) { 
	this->bLoop = bLoop; 
	this->bFirstMove = true;
	this->delay = DELAY_TIME;
}

//SCREEN FIX
CIwVec2 MapPointer::GetTileByMouseCoords(float mouseX, float mouseY, int layer)
{
	float newIsoXPos = 0;
	float newIsoYPos = 0;
	mouseY += layer * tileDiffLayer - tileMapHeightBase;
	Utils::GetIsoSpaceCoord(mouseX, mouseY, isoFormulaXOffset, isoFormulaYOffset, &newIsoXPos, &newIsoYPos);
	int xTile = (int)ceilf(newIsoXPos);
	int yTile = (int)ceilf(newIsoYPos);
	return CIwVec2(xTile, yTile);
}

void MapPointer::SetCursorGrid(IsoTerrain *CursorGrid[][MAX_TILESROW])
{
	for(int i = 0; i < MAX_TILESROW; i++)
		for(int j = 0; j < MAX_TILESROW; j++)
			this->CursorGrid[j][i] = CursorGrid[j][i];
}

void MapPointer::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	int32 mouseX = event->m_x;
	int32 mouseY = event->m_y;

	for(int i = 9; i >= 0; i--)
	{
		CIwVec2 Tile = GetTileByMouseCoords((float)mouseX, (float)mouseY, i);
		int xTile = Tile.x;
		int yTile = Tile.y;

		if(xTile < 0 || yTile < 0 || xTile >= MAX_TILESROW || yTile >= MAX_TILESROW)
			continue;

		if(CursorGrid[xTile][yTile] == NULL)
			continue;

		if(CursorGrid[xTile][yTile]->getLayer() != i)
			continue;

		SetGrid(CursorGrid[xTile][yTile]);
		return;
	}
}

void MapPointer::KeyboardHandler(s3eKeyboardEvent* event)
{
	/*
	if(event->m_Pressed && bBlockMousePointer)
	{
		return;
	}
	else
	{
		if(!event->m_Pressed)
		{
			bBlockMousePointer = false;
		}
	}
	*/

	if(event->m_Pressed)
	{
		if(delay_movement.GetIsStop())
		{
			delay_movement.Reset();
			delay_movement.Start();
			IwTrace(APPLOG, ("Timer..."));
		}

		lastKeyPressed = event->m_Key;
		if(lastKeyPressed == s3eKeyAbsRight)
		{
			SetContinuousMovement(true);
			bKeyRightPressed = true;
			bMoveOnce = true;
		}
		else if(lastKeyPressed == s3eKeyAbsLeft)
		{
			SetContinuousMovement(true);
			bKeyLeftPressed = true;
			bMoveOnce = true;
		}
		else if(lastKeyPressed == s3eKeyAbsUp)
		{
			SetContinuousMovement(true);
			bKeyUpPressed = true;
			bMoveOnce = true;
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			SetContinuousMovement(true);
			bKeyDownPressed = true;
			bMoveOnce = true;
		} 
	}
	else
	{
		SetContinuousMovement(false);
		bKeyRightPressed = false;
		bKeyLeftPressed = false;
		bKeyDownPressed = false;
		bKeyUpPressed = false;
		lastKeyPressed = s3eKeyFirst;
		bSetMovement = false;
		delay_movement.Reset();
		delay_movement.Stop();
	}
}

IsoTerrain *MapPointer::findAvailableGridForPointer(int despX, int despY)
{
	int xTile = getXNumTile() + despX;
	int yTile = getYNumTile() + despY;
	//IwTrace(APPLOG, ("%i %i", xTile, yTile));
	if(xTile >= MAX_TILESROW || yTile >= MAX_TILESROW || xTile < 0 || yTile < 0)
		return NULL;

	if(CursorGrid[xTile][yTile] != NULL)
		return CursorGrid[xTile][yTile];

	IsoTerrain* tempTile = NULL;
	do 
	{
		xTile += despX;
		yTile += despY;
		if(CursorGrid[xTile][yTile] != NULL)
			return CursorGrid[xTile][yTile];
	} 
	while(xTile < MAX_TILESROW - 1 && yTile < MAX_TILESROW - 1 && xTile > 0 && yTile > 0);

	return NULL;
}

IsoTerrain* MapPointer::GetIsoTerrain()
{
	//if(GetIsMouseOnAGrid())
	//{
	if(getXNumTile() >= 0 && getXNumTile() < MAX_TILESROW && getYNumTile() >= 0 && getXNumTile() < MAX_TILESROW)
		return CursorGrid[getXNumTile()][getYNumTile()];
	//}

	return NULL;
}