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

#ifndef _CLOUD_H
#define _CLOUD_H

#include "IwTexture.h"
#include "IwGx.h"
#include "sprite2d.h"
#include "texturehelper.h"

class Cloud : public Sprite2D
{
public:
	Cloud(RenderData data, CIwTexture* tex, float xPos = 0, float yPos = 0, float xVel = 0, float yVel = 0);
	~Cloud();
	void Update();
};

#endif