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

#ifndef _ISO_PROP_H_
#define _ISO_PROP_H_

#include "isoobject.h"

class IsoProp : public IsoObject
{
protected:
	int PropNum;
	int XNumTile;
	int YNumTile;

public:
	IsoProp();
	~IsoProp();

	int getPropNum() const { return PropNum; }

	void setXNumTile(const int _XNumTile){ XNumTile = _XNumTile; }
	int getXNumTile() const	{ return XNumTile; }
	void setYNumTile(const int _YNumTile){ YNumTile = _YNumTile; }
	int getYNumTile() const	{ return YNumTile; }

	virtual void InitIso();
	virtual void CleanIso();

	void SetPropNum(int propNum);
};

#endif // _ISO_PROP_H_
