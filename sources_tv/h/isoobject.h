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

#ifndef _ISO_OBJECT_H_
#define _ISO_OBJECT_H_

#include "Math.h"
#include "IwGx.h"
#include "common.h"

class IsoObject
{
	/// Properties
protected:
	//render properties.
	bool bRender;
	CIwFVec3 position;
	CIwFVec3 isoPosition;
	int layer;
	float width;
	float height;
	float offsetX;
	float offsetY;
	float uvMinX;
	float uvMaxX;
	float uvMinY;
	float uvMaxY;
public:
	bool bIsActiveObj;
	IsoObject();
	~IsoObject();
	void setRender(const bool _bRender)	{ bRender = _bRender;}
	bool getRender() const	{ return bRender;}

	void setPosition(float _posX, float _posY, float _posZ)
	{
		position.x = _posX;
		position.y = _posY;
		position.z = _posZ;
	}
	CIwFVec3 getPosition() const {	return position; }

	void setPos2D(float _posX, float _posY)
	{
		position.x = _posX;
		position.y = _posY;
	}

	CIwFVec2 getPos2D() const	{ return CIwFVec2(position.x, position.y); }

	void setPosX(float _posX) { position.x = _posX;	}
	float getPosX() const { return position.x; }

	void setPosY(float _posY) { position.y = _posY;	}
	float getPosY() const { return position.y; }

	void setPosZ(float _posZ) {	position.z = _posZ;	}
	float getPosZ() const { return position.z; }

	void setIsoPosition();
	CIwFVec3 getIsoPosition() const {	return isoPosition; }

	CIwFVec3 getIsoPos3D();
	CIwFVec2 getIsoPos2D();
	float getIsoPosX();
	float getIsoPosY();
	float getIsoPosZ();

	void setLayer(int _layer) {	layer = _layer; }
	int getLayer() const { return layer; }

	void setSize(float _width, float _height)
	{
		width = _width;
		height = _height;
	}
	void setWidth(float _width) { width = _width; }
	float getWidth() const { return width; }

	void setHeight(float _height) { height = _height; }
	float getHeight() const { return height; }

	float getOffsetX() const { return offsetX; }
	float getOffsetY() const { return offsetY; }

	void setUvMinX(float _uvMinX) { uvMinX = _uvMinX; }
	float getUvMinX() const { return uvMinX; }

	void setUvMinY(float _uvMinY) {	uvMinY = _uvMinY; }
	float getUvMinY() const { return uvMinY; }

	void setUvMaxX(float _uvMaxX) { uvMaxX = _uvMaxX; }
	float getUvMaxX() const { return uvMaxX; }

	void setUvMaxY(float _uvMaxY) { uvMaxY = _uvMaxY;}
	float getUvMaxY() const { return uvMaxY; }
		
	virtual void InitIso();
	virtual void Update();
	virtual void CleanIso();

	void SetIsoOffset(IsoOffset _newIsoOffset);

	float VGetDistance(float _x1, float _x2, float _y1, float _y2);
};


#endif // _ISO_OBJECT_H_