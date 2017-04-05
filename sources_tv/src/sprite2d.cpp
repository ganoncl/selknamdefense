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

#include "sprite2d.h"

Sprite2D::Sprite2D()
{
	this->image = NULL;
}

Sprite2D::Sprite2D(CIwTexture* image, float xPos, float yPos, float xVel, float yVel)
{
	this->image = image;
	this->position = CIwFVec3(xPos, yPos, 1.0f);
	this->velocity = CIwFVec3(xVel, yVel, 0);
	width = (float)image->GetWidth();
	height = (float)image->GetHeight();
	screenW = (float)IwGxGetScreenWidth();
	screenH = (float)IwGxGetScreenHeight();
	uv[0] = CIwFVec2(0.0f, 0.0f);
	uv[1] = CIwFVec2(0.0f, 1.0f);
	uv[2] = CIwFVec2(1.0f, 1.0f);
	uv[3] = CIwFVec2(1.0f, 0.0f);
}

Sprite2D::Sprite2D(RenderData data, CIwTexture* tex, float xPos, float yPos, float xVel, float yVel)
{
	image = tex;
	bMode = true;
	this->position = CIwFVec3(xPos, yPos, 0.0f);
	this->velocity = CIwFVec3(xVel, yVel, 0);

	CIwSVec2 tmpDimension = TextureHelper::GetDimension(data);

	width = (float)tmpDimension.x;
	height = (float)tmpDimension.y;
	screenW = (float)IwGxGetScreenWidth();
	screenH = (float)IwGxGetScreenHeight();
	this->data = data;

	uv[0] = CIwFVec2(data.uvInfo.minX, data.uvInfo.minY);
	uv[1] = CIwFVec2(data.uvInfo.minX, data.uvInfo.maxY);
	uv[2] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.maxY);
	uv[3] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.minY);
}

Sprite2D::~Sprite2D()
{

}

void Sprite2D::Render()
{
	if(bMode)
	{
		Utils::RenderSingleTextureFloat(image, CIwFVec2(position.x, position.y), data);
	}
	else
	{
		CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
		pMat->SetTexture(image);
		IwGxSetMaterial(pMat);
		IwGxSetVertStreamModelSpace(pos, 4);
		IwGxSetUVStream(uv);
		IwGxSetColStream(NULL);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
	}
}

void Sprite2D::Update()
{
	position.x += velocity.x;
	position.y += velocity.y;
	position.z += velocity.z;
	
	SetPosition(position.x, position.y, position.z);
}

CIwFVec3 Sprite2D::GetPosition()
{
	return position;
}

CIwFVec3 Sprite2D::GetVelocity()
{
	return velocity;
}

void Sprite2D::SetPosition(float x, float y, float z)
{
	this->position = CIwFVec3(x, y, z);

	pos[0] = CIwFVec3(x, y, z);
	pos[1] = CIwFVec3(x, y + GetHeight(), z);
	pos[2] = CIwFVec3(x + GetWidth(), y + GetHeight(), z);
	pos[3] = CIwFVec3(x + GetWidth(), y, z);
}

void Sprite2D::SetVelocity(float x, float y, float z)
{
	this->velocity = CIwFVec3(x, y, z);
}

float Sprite2D::GetWidth()
{
	return this->width;
}

float Sprite2D::GetHeight()
{
	return this->height;
}

float Sprite2D::GetScreenWidth()
{
	return screenW;
}

float Sprite2D::GetScreenHeight()
{
	return screenH;
}