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

#ifndef _TEXTUREHELPER_H
#define _TEXTUREHELPER_H

#include "common.h"

class CGame;
class IsoTerrain;

class TextureHelper
{
public:
	TextureHelper();
	~TextureHelper();

	void RetrieveTextureDataFromFile(string fileName);
	void RetrieveAllTextures();
	static TextureData GetTextureNamed(string fileName);
	static UVInfo GetUVInfo(TextureData data);
	static UVInfo GetUVInfo(int width, int height, int offX, int offY, TextureData data);
	static UVInfo GetUVInfo(TextureData data, int cols, int rows, int index);
	static RenderData GetRenderData(TextureData data);
	static RenderData GetRenderData(int width, int height, int offX, int offY, TextureData data);
	static RenderData GetRenderData(TextureData data, int cols, int rows, int index);
	static void GetRenderData(RenderData* output, TextureData data);
	static CIwSVec2 GetDimension(RenderData data);
	static CIwTexture* GetCIwTexture(AtlasName name);
	static AtlasName GetAtlasName(TextureData data);

	/* Swap UV's values to simulate a mirror image */
	static RenderData GetMirrorRenderData(RenderData data, MirrorImage mirror);
};



#endif