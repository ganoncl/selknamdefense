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

#include "cloud.h"

Cloud::Cloud(RenderData data, CIwTexture* tex, float xPos, float yPos, float xVel, float yVel) : Sprite2D(data, tex, xPos, yPos, xVel, yVel)
{

}

Cloud::~Cloud()
{

}

void Cloud::Update()
{
	CIwFVec3 currentVeloc = GetVelocity();
	if(currentVeloc.x < 0)
	{
		if(GetPosition().x <= -GetWidth())
			SetPosition(GetScreenWidth(), GetPosition().y);
	}
	else
	{
		if(GetPosition().x >= currentGameHeight)
			SetPosition(0, GetPosition().y);
	}
	
	
	Sprite2D::Update();
}
