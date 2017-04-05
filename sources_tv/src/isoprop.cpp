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
#include "isoobject.h"
#include "game.h"



#include "isoprop.h"

IsoProp::IsoProp()
{
	CleanIso();
}

IsoProp::~IsoProp()
{

}

void IsoProp::InitIso()
{
	//SetIsoOffset(isoTerrainOffset);
}

void IsoProp::CleanIso()
{
	IsoObject::CleanIso();
	PropNum = -1;
	XNumTile = -1;
	YNumTile = -1;
}

void IsoProp::SetPropNum(int propNum)
{
	float tmpUvMinX = 0.f;
	float tmpUvMaxX = 0.f;
	float tmpUvMinY = 0.f;
	float tmpUvMaxY = 0.f;
	float uvOffset = 0.f;
	float myX=0.f;
	float myY=0.f;
	int resto = 0;
	int tmpPropNum = 0;

	//64x64 props
	//prop nums
	//86, 87, 88
	//89, 90, 91
	//92, 93, 94

	//96x192 props
	//prop nums
	//95,  96,  97,  98
	//99, 100, 101, 102

	PropNum = propNum;	
	tmpPropNum = propNum;
	
	//handle uvs 64x64 props
	if(propNum >= 31 && propNum <= 39)
	{
		tmpPropNum = tmpPropNum - 31;
		resto = tmpPropNum / 3;
		myY = float(resto);
		myX = (resto * 3.f);
		myX = tmpPropNum - myX;

		uvOffset = 1.0f / 3.0f;
		tmpUvMinX = myX * uvOffset;
		tmpUvMaxX = (myX + 1) * uvOffset;

		tmpUvMinY = myY * uvOffset;
		tmpUvMaxY = (myY + 1) * uvOffset;


		setUvMinX(tmpUvMinX);
		setUvMaxX(tmpUvMaxX);
		setUvMinY(tmpUvMinY);
		setUvMaxY(tmpUvMaxY);
	}

	//handle uvs 96x192 props
	//if(propNum >= 86 && propNum <= 94)
	//{
	//	tmpPropNum = tmpPropNum - 86;
	//	resto = tmpPropNum / 3;
	//	myY = float(resto);
	//	myX = (resto * 3.f);
	//	myX = tmpPropNum - myX;

	//	uvOffset = 1.0f / 3.0f;
	//	tmpUvMinX = myX * uvOffset;
	//	tmpUvMaxX = (myX + 1) * uvOffset;

	//	tmpUvMinY = myY * uvOffset;
	//	tmpUvMaxY = (myY + 1) * uvOffset;


	//	setUvMinX(tmpUvMinX);
	//	setUvMaxX(tmpUvMaxX);
	//	setUvMinY(tmpUvMinY);
	//	setUvMaxY(tmpUvMaxY);
	//}


	//CODIGO DE PRUEBA
	IsoOffset offset;

	if(propNum >= 31 && propNum <= 39)
	{
		offset.offsetX = -32.0f;
		offset.offsetY = -32.0f;
		SetIsoOffset(offset);
	}
	else if(propNum >= 40 && propNum <= 45)
	{
		//setWidth(64.0f);
		//setHeight(64.0f);

		tmpPropNum = tmpPropNum - 40;
		resto = tmpPropNum / 3;
		myY = float(resto);
		myX = (resto * 3.f);
		myX = tmpPropNum - myX;

		uvOffset = 1.0f / 3.0f;
		tmpUvMinX = myX * uvOffset;
		tmpUvMaxX = (myX + 1) * uvOffset;

		tmpUvMinY = myY * 0.5f;
		tmpUvMaxY = (myY + 1) * 0.5f;


		setUvMinX(tmpUvMinX);
		setUvMaxX(tmpUvMaxX);
		setUvMinY(tmpUvMinY);
		setUvMaxY(tmpUvMaxY);


		/*
		int aux = getPropNum() - 95;

		setUvMinX(0.0f);
		setUvMaxX(1.0f);
		setUvMinY(0.0f);
		setUvMaxY(1.0f);
		*/
		/*
		setUvMinX(0.25f * (aux % 4));
		setUvMaxX(0.25f + 0.25f * (aux % 4));
		setUvMinY(((aux >= 4) ? 1 : 0) * 0.5f);
		setUvMaxY(((aux >= 4) ? 1 : 0) * 0.5f + 0.5f);
		*/

		offset.offsetX = -36.0f;
		offset.offsetY = -64.0f;
		SetIsoOffset(offset);
	}
	else if(propNum >= 46 && propNum <= 51)
	{

		tmpPropNum = tmpPropNum - 46;
		resto = tmpPropNum / 3;
		myY = float(resto);
		myX = (resto * 3.f);
		myX = tmpPropNum - myX;

		uvOffset = 1.0f / 3.0f;
		tmpUvMinX = myX * uvOffset;
		tmpUvMaxX = (myX + 1) * uvOffset;

		tmpUvMinY = myY * 0.5f;
		tmpUvMaxY = (myY + 1) * 0.5f;


		setUvMinX(tmpUvMinX);
		setUvMaxX(tmpUvMaxX);
		setUvMinY(tmpUvMinY);
		setUvMaxY(tmpUvMaxY);

		offset.offsetX = -32.0f;
		offset.offsetY = -96.0f;
		SetIsoOffset(offset);
	}
}