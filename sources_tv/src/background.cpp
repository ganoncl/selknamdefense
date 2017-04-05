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

#include "math.h"
#include "background.h"
#include "utils.h"
#include "texturehelper.h"
//Set a background only color
Background::Background(int r, int g, int b, int a, int width, int height, float x, float y)
{
	numImage = -1;

	onlyColor = true;

	this->_x = x;
	this->_y = y;
	this->_z = 10;

	if(width == -1) screenW = IwGxGetScreenWidth();
	else screenW = width;

	if(height == -1) screenH = IwGxGetScreenHeight();
	else screenH = height;

	this->r = r;
	this->g = g;
	this->b = b;
	this->a = a;
}

//Set a texture and mosaic mode
Background::Background(CIwTexture* tex, int width, int height, float x, float y)
{
	numImage = -1;
	onlyColor = false;

	this->_x = x;
	this->_y = y;
	this->_z = 1.0f;

	if(width == -1) screenW = IwGxGetScreenWidth();
	else screenW = width;

	if(height == -1) screenH = IwGxGetScreenHeight();
	else screenH = height;
	
	this->tex = tex;

	if(tex->GetWidth() != 0 && tex->GetHeight() != 0)
	{
		int blockWidth = tex->GetWidth();
		int blockHeight = tex->GetHeight();
		float numHorizontalBlocks = (float)ceil((float)screenW / (float)blockWidth);
		float numVerticalBlocks = (float)ceil((float)screenH / (float)blockHeight);

		numBlocks = 0;
		for(int i = 0; i < numVerticalBlocks; i++)
		{
			for(int j = 0; j < numHorizontalBlocks; j++)
			{
				int index = numBlocks * 4;
				pos[index].x = j * blockWidth + _x;
				pos[index].y = i * blockHeight + _y;
				pos[index].z = _z;
				pos[index + 1].x = j * blockWidth + _x;
				pos[index + 1].y = (i + 1) * blockHeight + _y;
				pos[index + 1].z = _z;
				pos[index + 2].x = (j + 1) * blockWidth + _x;
				pos[index + 2].y = (i + 1) * blockHeight + _y;
				pos[index + 2].z = _z;
				pos[index + 3].x = (j + 1) * blockWidth + _x;
				pos[index + 3].y = i * blockHeight + _y;
				pos[index + 3].z = _z; 
				s_UV[index] = CIwFVec2(0, 0);
				s_UV[index + 1] = CIwFVec2(0, 1);
				s_UV[index + 2] = CIwFVec2(1, 1);
				s_UV[index + 3] = CIwFVec2(1, 0);
				numBlocks++;
			}
		}
	}
}

//Set a texture and mosaic mode
Background::Background(int numImage)
{
	this->numImage = numImage;

	if(numImage == 0)
	{
		numData = 6;
		for(int i = 0; i < 6;  i++) backgroundData[i] = new RenderData();
		RenderData tmp;
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky-0-0")); Utils::CopyRenderData(backgroundData[0], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky-0-1")); Utils::CopyRenderData(backgroundData[1], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky-1-0")); Utils::CopyRenderData(backgroundData[2], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky-1-1")); Utils::CopyRenderData(backgroundData[3], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky-2-0")); Utils::CopyRenderData(backgroundData[4], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky-2-1")); Utils::CopyRenderData(backgroundData[5], tmp);
	}
	else if(numImage == 3)
	{
		numData = 6;
		for(int i = 0; i < 6;  i++) backgroundData[i] = new RenderData();
		RenderData tmp;
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay01")); Utils::CopyRenderData(backgroundData[0], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay02")); Utils::CopyRenderData(backgroundData[1], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay03")); Utils::CopyRenderData(backgroundData[2], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay04")); Utils::CopyRenderData(backgroundData[3], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay05")); Utils::CopyRenderData(backgroundData[4], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay06")); Utils::CopyRenderData(backgroundData[5], tmp);
	}
	else if(numImage == 1)
	{
		numData = 6;
		for(int i = 0; i < 6;  i++) backgroundData[i] = new RenderData();
		RenderData tmp;
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground-0-0")); Utils::CopyRenderData(backgroundData[0], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground-0-1")); Utils::CopyRenderData(backgroundData[1], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground-1-0")); Utils::CopyRenderData(backgroundData[2], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground-1-1")); Utils::CopyRenderData(backgroundData[3], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground-2-0")); Utils::CopyRenderData(backgroundData[4], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground-2-1")); Utils::CopyRenderData(backgroundData[5], tmp);
	}
	else if(numImage == 2)
	{
		numData = 6;
		for(int i = 0; i < 6;  i++) backgroundData[i] = new RenderData();
		RenderData tmp;
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap-0-0")); Utils::CopyRenderData(backgroundData[0], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap-0-1")); Utils::CopyRenderData(backgroundData[1], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap-1-0")); Utils::CopyRenderData(backgroundData[2], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap-1-1")); Utils::CopyRenderData(backgroundData[3], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap-2-0")); Utils::CopyRenderData(backgroundData[4], tmp);
		tmp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap-2-1")); Utils::CopyRenderData(backgroundData[5], tmp);
	}
}

Background::~Background()
{
	for(int i = 0; i < 6; i++)
	{
		texBackground[i] = NULL;
	}

	for(int i = 0; i < numData; i++) delete backgroundData[i];
} 

void Background::Render()
{
	//Game background.
	if(numImage == 3)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[0]->atlasName), CIwSVec2(0, 0),     backgroundData[0]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[1]->atlasName), CIwSVec2(512, 0),   backgroundData[1]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[2]->atlasName), CIwSVec2(1024, 0),   backgroundData[2]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[3]->atlasName), CIwSVec2(512, 512),  backgroundData[3]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[4]->atlasName), CIwSVec2(0, 512),    backgroundData[4]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[5]->atlasName), CIwSVec2(1024, 512), backgroundData[5]);
		IwGxFlush();

		return;
	}
	
	//Menu foreground.
	if(numImage == 1)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[0]->atlasName), CIwSVec2(0, 0),     backgroundData[0]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[1]->atlasName), CIwSVec2(0, 512),   backgroundData[1]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[2]->atlasName), CIwSVec2(512, 0),   backgroundData[2]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[3]->atlasName), CIwSVec2(512, 512),  backgroundData[3]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[4]->atlasName), CIwSVec2(1024, 0),    backgroundData[4]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[5]->atlasName), CIwSVec2(1024, 512), backgroundData[5]);
		IwGxFlush();

		return;
	}

	//Worldmap
	if(numImage == 2)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[0]->atlasName), CIwSVec2(0, 0),     backgroundData[0]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[1]->atlasName), CIwSVec2(0, 512),   backgroundData[1]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[2]->atlasName), CIwSVec2(512, 0),   backgroundData[2]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[3]->atlasName), CIwSVec2(512, 512),  backgroundData[3]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[4]->atlasName), CIwSVec2(1024, 0),    backgroundData[4]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[5]->atlasName), CIwSVec2(1024, 512), backgroundData[5]);
		IwGxFlush();

		return;
	}

	//Menu background.
	if(numImage == 0)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[0]->atlasName), CIwSVec2(0, 0),     backgroundData[0]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[1]->atlasName), CIwSVec2(0, 512),   backgroundData[1]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[2]->atlasName), CIwSVec2(512, 0),   backgroundData[2]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[3]->atlasName), CIwSVec2(512, 512),  backgroundData[3]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[4]->atlasName), CIwSVec2(1024, 0),    backgroundData[4]);
		IwGxFlush();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[5]->atlasName), CIwSVec2(1024, 512), backgroundData[5]);
		IwGxFlush();

		return;
	}

	if(numImage != -1)
	{
		Utils::RenderSingleTexture(texBackground[0], CIwSVec2(0, 0));
		IwGxFlush();
		Utils::RenderSingleTexture(texBackground[1], CIwSVec2(512, 0));
		IwGxFlush();
		Utils::RenderSingleTexture(texBackground[2], CIwSVec2(1024, 0));
		IwGxFlush();
		Utils::RenderSingleTexture(texBackground[3], CIwSVec2(0, 512));
		IwGxFlush();
		Utils::RenderSingleTexture(texBackground[4], CIwSVec2(512, 512));
		IwGxFlush();
		Utils::RenderSingleTexture(texBackground[5], CIwSVec2(1024, 512));
		IwGxFlush();

		return;
	}

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);

	if(onlyColor)
	{
		IwGxSetMaterial(pMat);
		static CIwFVec3 xy3[4];
		xy3[0].x = _x,			    xy3[0].y = _y;
		xy3[1].x = _x,				xy3[1].y = screenH + _y;
		xy3[2].x = screenW + _x,	xy3[2].y = screenH + _y;
		xy3[3].x = screenW + _x,    xy3[3].y = _y;
		xy3[0].z = _z;
		xy3[1].z = _z;
		xy3[2].z = _z;
		xy3[3].z = _z;
		IwGxSetVertStreamModelSpace(xy3, 4);
		static CIwColour cols[4];
		cols[0].Set(r, g, b, a);
		cols[1].Set(r, g, b, a);
		cols[2].Set(r, g, b, a);
		cols[3].Set(r, g, b, a);
		IwGxSetColStream(cols, 4);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	}
	else
	{
		pMat->SetTexture(tex);
		IwGxSetMaterial(pMat);
		IwGxSetVertStreamModelSpace(pos, 4 * numBlocks);
		IwGxSetColStream(NULL);
		IwGxSetUVStream(s_UV);
		IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4 * numBlocks);
	}
}