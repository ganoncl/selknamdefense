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

#ifndef _SPRITE2D_H
#define _SPRITE2D_H

#include "IwTexture.h"
#include "IwGx.h"
#include "texturehelper.h"
#include "utils.h"

class Sprite2D
{
public:
	Sprite2D();
	Sprite2D(CIwTexture* image, float xPos = 0, float yPos = 0, float xVel = 0, float yVel = 0);
	Sprite2D(RenderData data,  CIwTexture* tex, float xPos = 0, float yPos = 0, float xVel = 0, float yVel = 0);
	~Sprite2D();
	void Update();
	void Render();

	CIwFVec3 GetPosition();
	CIwFVec3 GetVelocity();
	float GetWidth();
	float GetHeight();
	float GetScreenWidth();
	float GetScreenHeight();

	void SetPosition(float x, float y, float z = 1.0f);
	void SetVelocity(float x, float y = 0.0f, float z = 0.0f);

private:
	CIwFVec3 position;
	CIwFVec3 velocity;
	CIwTexture* image;
	CIwFVec2 uv[4];
	CIwFVec3 pos[4];
	float width;
	float height;
	float screenW;
	float screenH;
	RenderData data;
	bool bMode;
};

#endif