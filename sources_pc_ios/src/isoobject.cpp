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

#include "utils.h"
#include "common.h"
#include "isoobject.h"

IsoObject::IsoObject()
{
	CleanIso();
}

IsoObject::~IsoObject()
{

}

void IsoObject::InitIso()
{
	bIsActiveObj = true;
	bRender = true;
}

void IsoObject::Update()
{

}

void IsoObject::CleanIso()
{
	bRender = false;
	position.x = -1;
	position.y = -1;
	position.z = -1;
	isoPosition.x = -1;
	isoPosition.y = -1;
	isoPosition.z = -1;
	layer = -1;
	width = 64;
	height = 64;
	offsetX = -1;
	offsetY = -1;
	uvMinX = -1;
	uvMaxX = -1;
	uvMinY = -1;
	uvMaxY = -1;
	bIsActiveObj = false;
}

CIwFVec3 IsoObject::getIsoPos3D()
{
	CIwFVec3 myVect3D;
	float myOffsetY = 0;
	myOffsetY = float(layer) * 24.0f;
	float modifiedY = position.y + myOffsetY;
	myVect3D.x = position.x;
	myVect3D.y = modifiedY;
	myVect3D.z = myOffsetY;

	return myVect3D;
}

CIwFVec2 IsoObject::getIsoPos2D()
{
	CIwFVec2 myVect2D;
	float myOffsetY = 0;
	myOffsetY = float(layer) * 24.0f;
	float modifiedY = position.y + myOffsetY;
	myVect2D.x = position.x;
	myVect2D.y = modifiedY;

	return myVect2D;
}

float IsoObject::getIsoPosX()
{
	return position.x;
}

float IsoObject::getIsoPosY()
{
	float myOffsetY = 0;
	myOffsetY = float(layer) * 24.0f;
	float modifiedY = position.y + myOffsetY;

	return modifiedY;
}

float IsoObject::getIsoPosZ()
{
	int myOffsetY = 0;
	myOffsetY = layer * 24;
	return float(myOffsetY);
}

void IsoObject::SetIsoOffset(IsoOffset _newIsoOffset)
{
	offsetX = _newIsoOffset.offsetX;
	offsetY = _newIsoOffset.offsetY;
}

float IsoObject::VGetDistance(float _x1, float _x2, float _y1, float _y2)
{
	float dx, dy;
	dx = (_x2 - _x1);
	dy = (_y2 - _y1);
	return((dx * dx) - (dy * dy));
}

void IsoObject::setIsoPosition()
{
	float myPosX = position.x;
	float myPosY = position.y + ((layer - 1) * tileDiffLayer); //layer * tileDiffLayer - tileMapHeightBase;

	float newIsoXPos = 0;
	float newIsoYPos = 0;

	Utils::GetIsoSpaceCoord(myPosX - xWindowOffset, myPosY - yWindowOffset, mapXOffset + mapTileUpperCornerX - xWindowOffset, mapYOffset - yWindowOffset, &newIsoXPos, &newIsoYPos);

	newIsoXPos *= 100;
	newIsoYPos *= 100;

	int isoXInt = (int)newIsoXPos;
	int isoYInt = (int)newIsoYPos;

	isoPosition.x = (float)isoXInt;
	isoPosition.y = (float)isoYInt;
	isoPosition.z = (float)layer;
	//IwTrace(ISOPOSITION, ("setIsoPosition() called, myPosX %f, myPosY %f", myPosX, myPosY));
	//IwTrace(ISOPOSITION, ("setIsoPosition() called, isoPosition.x %f, isoPosition.y %f , isoPosition.z %f", isoPosition.x, isoPosition.y, isoPosition.z));
}

