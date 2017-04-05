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

	backgroundData = NULL;

	this->_x = x;
	this->_y = y;
	this->_z = 1;

	if(width == -1) screenW = currentGameWidth;
	else screenW = width;

	if(height == -1) screenH = currentGameHeight;
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

	backgroundData = NULL;

	this->_x = x;
	this->_y = y;
	this->_z = 0;

	if(width == -1) screenW = currentGameWidth;
	else screenW = width;

	if(height == -1) screenH = currentGameHeight;
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

	RenderData myTmpRenderData;

	backgroundData = new RenderData();

	if(numImage == 0 || numImage == 1 || numImage == 2)
	{
		CIwSVec2 myNewSize;

		if(numImage == 0)
		{
			if(bUseHDImages)
				myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky_wide"));
			else
				myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_sky_wide_small"));
		}
		else if(numImage == 1)
		{
			if(bUseHDImages)
				myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground_wide"));
			else
				myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_foreground_wide_small"));
		}
		else if(numImage == 2)
		{
			if(bUseHDImages)
				myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap_wide"));
			else
				myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("worldmap_wide_small"));
		}

		myNewSize = CIwSVec2((int16)myTmpRenderData.width, (int16)myTmpRenderData.height);
		myNewSize = CIwSVec2(Utils::GetScaledInt(myTmpRenderData.width, myTmpRenderData.height), (int16)currentGameHeight);
		myTmpRenderData.width = myNewSize.x;
		myTmpRenderData.height = myNewSize.y;
			
		int tmpWidth = myTmpRenderData.width;
		if(myTmpRenderData.bRotate)
			tmpWidth = myTmpRenderData.height;
		xFullScreenPos = (int16)((currentGameWidth * 0.5f) - (tmpWidth * 0.5f));

	}
	else if(numImage == 3)
	{
		myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay"));
		if(!bUseHDImages)
		{
			myTmpRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("fondogameplay_small"));
			myTmpRenderData.width = 1920;
			myTmpRenderData.height = 1080;
		}
	}

	backgroundData[0].atlasName = myTmpRenderData.atlasName;
	backgroundData[0].bRotate = myTmpRenderData.bRotate;
	backgroundData[0].width = myTmpRenderData.width;
	backgroundData[0].height = myTmpRenderData.height;
	backgroundData[0].uvInfo = myTmpRenderData.uvInfo;

}

Background::~Background()
{
	if(backgroundData!=NULL)
		delete backgroundData;
}

void Background::Render()
{
	if(numImage != -1 && backgroundData != NULL)
	{
		if(numImage == 0 || numImage == 1 || numImage == 2)
		{
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData->atlasName), CIwSVec2(xFullScreenPos, 0), backgroundData[0]);
		}
		else if(numImage == 3)
		{
			int xOff = (int)(xWindowOffset) + 140;
			int yOff = (int)(yWindowOffset) + 70;
			
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData->atlasName), CIwSVec2(0 - xOff, 0 - yOff), backgroundData[0]);
			//Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData->atlasName), CIwSVec2(512 - xOff, 512 - yOff), backgroundData[2]);
			/*IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[1]->atlasName), CIwSVec2(1024 - xOff, 0 - yOff), backgroundData[1]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[3]->atlasName), CIwSVec2(512 - xOff, 0 - yOff), backgroundData[3]);
			IwGxFlush();
			
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[4]->atlasName), CIwSVec2(0 - xOff, 512 - yOff), backgroundData[4]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[5]->atlasName), CIwSVec2(0 - xOff, 0 - yOff), backgroundData[5]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[6]->atlasName),  CIwSVec2(512 - xOff, 1024 - yOff), backgroundData[6]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[8]->atlasName), CIwSVec2(1536 - xOff, 512 - yOff), backgroundData[8]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[7]->atlasName), CIwSVec2(1024 - xOff, 1024 - yOff), backgroundData[7]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[10]->atlasName), CIwSVec2(1536 - xOff, 0 - yOff), backgroundData[10]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[11]->atlasName), CIwSVec2(1536 - xOff, 1024 - yOff), backgroundData[11]);
			IwGxFlush();
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundData[9]->atlasName), CIwSVec2(0 - xOff, 1024 - yOff), backgroundData[9]);
			IwGxFlush();*/
		}

		return;
	}

	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);

	if(onlyColor)
	{
		Utils::RenderColor(r, g, b, a, CIwSVec2((int)_x, (int)_y), CIwSVec2(screenW, screenH));
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