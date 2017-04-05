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

#include "IwGx.h"
#include "common.h"
#include "texturehelper.h"
#include "stdio.h"
#include "isoterrain.h"
#include "game.h"

TextureHelper::TextureHelper()
{

}

TextureHelper::~TextureHelper()
{

}

void TextureHelper::RetrieveTextureDataFromFile(string fileName)
{
	//map properties
	string tmpAtlasFileName = "";
	int tmpAtlasWidth = -1;
	int tmpAtlasHeight = -1;

	int textureNums = 0;
	string tmpTextureFileName = "";
	int tmpTextureXPos = -1;
	int tmpTextureYPos = -1;
	int tmpTextureWidth = -1;
	int tmpTextureHeight = -1;
	bool tmpbTextureRotated = false;

	string name = "tbn/" + fileName;

	s3eFile* fileHandle = s3eFileOpen(name.c_str(), "rb");
	if(!fileHandle)
		return;
	int32 fileSize = s3eFileGetSize(fileHandle);
	int dataIntToRead = 0;
	char dataByteRead = 0;
	char dataCharRead = 0;
	bool dataBoolToRead = 0;
	int stringCount = 0;
	int readerPos = 0;

	while (readerPos < fileSize)
	{
		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		stringCount = dataIntToRead;
		//IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, stringCount %i", stringCount));
		readerPos += sizeof(int);

		for(int i = 0; i < stringCount; i++)
		{
			s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
			dataCharRead = dataByteRead;
			readerPos += sizeof(char);
			tmpAtlasFileName += dataCharRead;
		}
		//IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpAtlasFileName %s", tmpAtlasFileName));

		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpAtlasWidth = dataIntToRead;
		IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpAtlasWidth %i", tmpAtlasWidth));
		readerPos += sizeof(int);

		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		tmpAtlasHeight = dataIntToRead;
		IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpAtlasHeight %i", tmpAtlasHeight));
		readerPos += sizeof(int);

		s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
		textureNums = dataIntToRead;
		IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, textureNums %i", textureNums));
		readerPos += sizeof(int);

		for(int i = 0; i < textureNums; i++)
		{
			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			stringCount = dataIntToRead;
			IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, stringCount %i", stringCount));
			readerPos += sizeof(int);

			tmpTextureFileName = "";

			for(int j = 0; j < stringCount; j++)
			{
				s3eFileRead(&dataByteRead, sizeof(char), 1, fileHandle);
				dataCharRead = dataByteRead;
				readerPos += sizeof(char);
				tmpTextureFileName += dataCharRead;
			}
			//IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpTextureFileName %s", tmpTextureFileName));

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpTextureXPos = dataIntToRead;
			IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpTextureXPos %i", tmpTextureXPos));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpTextureYPos = dataIntToRead;
			IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpTextureYPos %i", tmpTextureYPos));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpTextureWidth = dataIntToRead;
			IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpTextureWidth %i", tmpTextureWidth));
			readerPos += sizeof(int);

			s3eFileRead(&dataIntToRead, sizeof(int), 1, fileHandle);
			tmpTextureHeight = dataIntToRead;
			IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpTextureHeight %i", tmpTextureHeight));
			readerPos += sizeof(int);

			s3eFileRead(&dataBoolToRead, sizeof(bool), 1, fileHandle);
			tmpbTextureRotated = dataBoolToRead;
			IwTrace(TEXTURE_PARSER, ("RetrieveTextureDataFromFile() called, tmpbRotated %i", tmpbTextureRotated));
			readerPos += sizeof(bool);

			TextureData myNewTextureData;
			sprintf(myNewTextureData.textureFileName, "%s", tmpTextureFileName.c_str());
			sprintf(myNewTextureData.atlasFileName, "%s", tmpAtlasFileName.c_str());
			myNewTextureData.atlasWidth     = tmpAtlasWidth;
			myNewTextureData.atlasHeight    = tmpAtlasHeight;
			myNewTextureData.textureXPos    = tmpTextureXPos;
			myNewTextureData.textureYPos    = tmpTextureYPos;
			myNewTextureData.textureWidth   = tmpTextureWidth;
			myNewTextureData.textureHeight  = tmpTextureHeight;
			myNewTextureData.textureRotated = tmpbTextureRotated;

			IwTrace(DATATEX, ("RETRIEVING: %s %s", myNewTextureData.textureFileName, myNewTextureData.atlasFileName));

			TextureDataList[currentTextureId] = myNewTextureData;
			currentTextureId++;
		}
	}
	s3eFileClose(fileHandle);
}

void TextureHelper::RetrieveAllTextures()
{
	RetrieveTextureDataFromFile("bkg_gameplay.tbn");
	RetrieveTextureDataFromFile("bkg_menu_foreground.tbn");
	RetrieveTextureDataFromFile("bkg_menu_others.tbn");
	RetrieveTextureDataFromFile("bkg_menu_sky.tbn");
	RetrieveTextureDataFromFile("bkg_worldmap.tbn");
	RetrieveTextureDataFromFile("bkg_small_0.tbn");
	RetrieveTextureDataFromFile("bkg_small_1.tbn");
	RetrieveTextureDataFromFile("atlas_common_textures_0.tbn");
	RetrieveTextureDataFromFile("atlas_common_textures_1.tbn");
	RetrieveTextureDataFromFile("atlas_common_textures_2.tbn");
	RetrieveTextureDataFromFile("atlas_common_textures_3.tbn");
	RetrieveTextureDataFromFile("atlas_common_textures_4.tbn");
	RetrieveTextureDataFromFile("atlas_small_textures.tbn");
	RetrieveTextureDataFromFile("atlas_common_trial_0.tbn");
	RetrieveTextureDataFromFile("atlas_fx_0.tbn");
	RetrieveTextureDataFromFile("atlas_fx_1.tbn");
	RetrieveTextureDataFromFile("atlas_fx_2.tbn");
	RetrieveTextureDataFromFile("atlas_props_0.tbn");
	RetrieveTextureDataFromFile("atlas_props_1.tbn");
	RetrieveTextureDataFromFile("atlas_props_2.tbn");
	RetrieveTextureDataFromFile("terrain_combined_01_02.tbn");
	RetrieveTextureDataFromFile("terrain_combined_03_04.tbn");
	RetrieveTextureDataFromFile("atlas_enemy_boss_1.tbn");
	RetrieveTextureDataFromFile("atlas_others.tbn");
}

CIwSVec2 TextureHelper::GetDimension(RenderData data)
{
	CIwSVec2 ret;
	if(data.bRotate)
	{
		ret.x = data.height;
		ret.y = data.width;
	}
	else
	{
		ret.x = data.width;
		ret.y = data.height;
	}
	return ret;
}

TextureData TextureHelper::GetTextureNamed(string fileName)
{
	bool bTextureRetrieved = false;
	for(int i = 0; i < currentTextureId; i++)
	{
		string tmpString = TextureDataList[i].textureFileName;
		if(tmpString == fileName)
		{
			bTextureRetrieved = true;
			return TextureDataList[i];
		}
	}
	IwAssertMsg(APPASSERT, bTextureRetrieved == false, ("TextureHelper::GetTextureNamed() No texture"));
	return TextureData(); //Empty Struct.
}

UVInfo TextureHelper::GetUVInfo(TextureData data)
{
	UVInfo ret;

	ret.minX =  data.textureXPos                       / (float)data.atlasWidth;
	ret.maxX = (data.textureXPos + data.textureWidth)  / (float)data.atlasWidth;
	ret.minY =  data.textureYPos                       / (float)data.atlasHeight;
	ret.maxY = (data.textureYPos + data.textureHeight) / (float)data.atlasHeight;

	return ret;
}

UVInfo TextureHelper::GetUVInfo(TextureData data, int cols, int rows, int index)
{
	UVInfo ret;
	int newIndex = index;

	if(data.textureRotated)
	{
		int tmpCol = cols - (index / rows) - 1;
		int tmpRow = index % rows;

		newIndex = tmpCol + tmpRow * cols;
	}

	int xIndex = newIndex % cols;
	int yIndex = (int)(newIndex / (float)cols);

	int xPos = data.textureXPos + data.textureWidth * xIndex;
	int yPos = data.textureYPos + data.textureHeight * yIndex;

	ret.minX =  xPos                       / (float)data.atlasWidth;
	ret.maxX = (xPos + data.textureWidth)  / (float)data.atlasWidth;
	ret.minY =  yPos                       / (float)data.atlasHeight;
	ret.maxY = (yPos + data.textureHeight) / (float)data.atlasHeight;

	return ret;
}

RenderData TextureHelper::GetRenderData(TextureData data)
{
	RenderData ret;
	ret.bRotate = data.textureRotated;
	ret.uvInfo = TextureHelper::GetUVInfo(data);
	ret.width = data.textureWidth;
	ret.height = data.textureHeight;
	ret.atlasName = TextureHelper::GetAtlasName(data);
	return ret;
}

RenderData TextureHelper::GetRenderData(TextureData data, int cols, int rows, int index, float myScale)
{
	RenderData ret;
	ret.bRotate = data.textureRotated;

	if(data.textureRotated)
	{
		int tmp = cols;
		cols = rows;
		rows = tmp;
	}

	data.textureWidth = (int)(data.textureWidth / (float)cols);
	data.textureHeight = (int)(data.textureHeight / (float)rows);

	ret.uvInfo = TextureHelper::GetUVInfo(data, cols, rows, index);
	
	ret.width = (int)((float)data.textureWidth * myScale);
	ret.height = (int)((float)data.textureHeight * myScale);
	ret.atlasName = TextureHelper::GetAtlasName(data);

	return ret;
}

UVInfo TextureHelper::GetUVInfo(int width, int height, int offX, int offY, TextureData data)
{
	UVInfo ret;

	int xPos = data.textureXPos + offX;
	int yPos = data.textureYPos + offY;

	ret.minX =  xPos                       / (float)data.atlasWidth;
	ret.maxX = (xPos + width)  / (float)data.atlasWidth;
	ret.minY =  yPos                       / (float)data.atlasHeight;
	ret.maxY = (yPos + height) / (float)data.atlasHeight;

	return ret;
}

RenderData TextureHelper::GetRenderData(int width, int height, int offX, int offY, TextureData data)
{
	RenderData ret;
	ret.bRotate = data.textureRotated;

	data.textureWidth = width;
	data.textureHeight = height;

	ret.uvInfo = TextureHelper::GetUVInfo(width, height, offX, offY, data);

	ret.width = data.textureWidth;
	ret.height = data.textureHeight;
	ret.atlasName = TextureHelper::GetAtlasName(data);
	return ret;
}

void TextureHelper::GetRenderData(RenderData* output, TextureData data)
{
	output->bRotate = data.textureRotated;
	output->uvInfo = TextureHelper::GetUVInfo(data);
	output->width = data.textureWidth;
	output->height = data.textureHeight;
	output->atlasName = TextureHelper::GetAtlasName(data);
}

CIwTexture* TextureHelper::GetCIwTexture(AtlasName name)
{
	if(name == ATLAS_BKG_GAMEPLAY)         return atlas_bkg_gameplay;
	else if(name == ATLAS_BKG_MENU_FOREGROUND)         return atlas_bkg_menu_foreground;
	else if(name == ATLAS_BKG_MENU_OTHERS)         return atlas_bkg_menu_others;
	else if(name == ATLAS_BKG_MENU_SKY)         return atlas_bkg_menu_sky;
	else if(name == ATLAS_BKG_WORLDMAP)         return atlas_bkg_worldmap;
	else if(name == ATLAS_BKG_SMALL_0)         return atlas_bkg_small_0;
	else if(name == ATLAS_BKG_SMALL_1)         return atlas_bkg_small_1;
	else if(name == ATLAS_COMMON_TEXTURES_0)         return atlas_common_textures_0;
	else if(name == ATLAS_COMMON_TEXTURES_1)         return atlas_common_textures_1;
	else if(name == ATLAS_COMMON_TEXTURES_2)         return atlas_common_textures_2;
	else if(name == ATLAS_COMMON_TEXTURES_3)         return atlas_common_textures_3;
	else if(name == ATLAS_COMMON_TEXTURES_4)         return atlas_common_textures_4;
	else if(name == ATLAS_SMALL_TEXTURES)         return atlas_small_textures;
	else if(name == ATLAS_COMMON_TRIAL_0)   return atlas_common_trial_0;
	else if(name == ATLAS_TERRAIN_0) return atlas_terrain_0;
	else if(name == ATLAS_TERRAIN_1) return atlas_terrain_1;
	else if(name == ATLAS_PROPS_0) return atlas_props_0;
	else if(name == ATLAS_PROPS_1) return atlas_props_1;
	else if(name == ATLAS_PROPS_2) return atlas_props_2;
	else if(name == ATLAS_FX_0) return atlas_fx_0;
	else if(name == ATLAS_FX_1) return atlas_fx_1;
	else if(name == ATLAS_FX_2) return atlas_fx_2;
	else if(name == ATLAS_HD_SELKNAM_ARCHER_1) return atlas_hd_selknam_archer_1;
	else if(name == ATLAS_HD_SELKNAM_ARCHER_2) return atlas_hd_selknam_archer_2;
	else if(name == ATLAS_HD_SELKNAM_ARCHER_3) return atlas_hd_selknam_archer_3;
	else if(name == ATLAS_HD_SELKNAM_BOWLER_1_01) return atlas_hd_selknam_bowler_1_01;
	else if(name == ATLAS_HD_SELKNAM_BOWLER_1_02) return atlas_hd_selknam_bowler_1_02;
	else if(name == ATLAS_HD_SELKNAM_BOWLER_2_01) return atlas_hd_selknam_bowler_2_01;
	else if(name == ATLAS_HD_SELKNAM_BOWLER_2_02) return atlas_hd_selknam_bowler_2_02;
	else if(name == ATLAS_HD_SELKNAM_BOWLER_3_01) return atlas_hd_selknam_bowler_3_01;
	else if(name == ATLAS_HD_SELKNAM_BOWLER_3_02) return atlas_hd_selknam_bowler_3_02;
	else if(name == ATLAS_HD_SELKNAM_MAGE_1) return atlas_hd_selknam_mage_1;
	else if(name == ATLAS_HD_SELKNAM_MAGE_2) return atlas_hd_selknam_mage_2;
	else if(name == ATLAS_HD_SELKNAM_MAGE_3) return atlas_hd_selknam_mage_3;
	else if(name == ATLAS_HD_SELKNAM_MELEE_1) return atlas_hd_selknam_melee_1;
	else if(name == ATLAS_HD_SELKNAM_MELEE_2) return atlas_hd_selknam_melee_2;
	else if(name == ATLAS_HD_SELKNAM_MELEE_3) return atlas_hd_selknam_melee_3;
	else if(name == ATLAS_HD_ENEMY_1) return atlas_hd_enemy_1;
	else if(name == ATLAS_HD_ENEMY_2) return atlas_hd_enemy_2;
	else if(name == ATLAS_HD_ENEMY_3) return atlas_hd_enemy_3;
	else if(name == ATLAS_HD_ENEMY_4_01) return atlas_hd_enemy_4_01;
	else if(name == ATLAS_HD_ENEMY_4_02) return atlas_hd_enemy_4_02;
	else if(name == ATLAS_HD_ENEMY_5) return atlas_hd_enemy_5;
	else if(name == ATLAS_HD_ENEMY_6) return atlas_hd_enemy_6;
	else if(name == ATLAS_HD_ENEMY_7) return atlas_hd_enemy_7;
	else if(name == ATLAS_SELKNAM_ARCHER_01_1) return atlas_selknam_archer_01_1;
	else if(name == ATLAS_SELKNAM_ARCHER_01_2) return atlas_selknam_archer_01_2;
	else if(name == ATLAS_SELKNAM_ARCHER_02_1) return atlas_selknam_archer_02_1;
	else if(name == ATLAS_SELKNAM_ARCHER_02_2) return atlas_selknam_archer_02_2;
	else if(name == ATLAS_SELKNAM_ARCHER_03_1) return atlas_selknam_archer_03_1;
	else if(name == ATLAS_SELKNAM_ARCHER_03_2) return atlas_selknam_archer_03_2;
	else if(name == ATLAS_SELKNAM_BOWLER_01_1) return atlas_selknam_bowler_01_1;
	else if(name == ATLAS_SELKNAM_BOWLER_01_2) return atlas_selknam_bowler_01_2;
	else if(name == ATLAS_SELKNAM_BOWLER_02_1) return atlas_selknam_bowler_02_1;
	else if(name == ATLAS_SELKNAM_BOWLER_02_2) return atlas_selknam_bowler_02_2;
	else if(name == ATLAS_SELKNAM_BOWLER_03_1) return atlas_selknam_bowler_03_1;
	else if(name == ATLAS_SELKNAM_BOWLER_03_2) return atlas_selknam_bowler_03_2;
	else if(name == ATLAS_SELKNAM_MAGE_01_1) return atlas_selknam_mage_01_1;
	else if(name == ATLAS_SELKNAM_MAGE_01_2) return atlas_selknam_mage_01_2;
	else if(name == ATLAS_SELKNAM_MAGE_02_1) return atlas_selknam_mage_02_1;
	else if(name == ATLAS_SELKNAM_MAGE_02_2) return atlas_selknam_mage_02_2;
	else if(name == ATLAS_SELKNAM_MAGE_03_1) return atlas_selknam_mage_03_1;
	else if(name == ATLAS_SELKNAM_MAGE_03_2) return atlas_selknam_mage_03_2;
	else if(name == ATLAS_SELKNAM_MELEE_01_1) return atlas_selknam_melee_01_1;
	else if(name == ATLAS_SELKNAM_MELEE_01_2) return atlas_selknam_melee_01_2;
	else if(name == ATLAS_SELKNAM_MELEE_02_1) return atlas_selknam_melee_02_1;
	else if(name == ATLAS_SELKNAM_MELEE_02_2) return atlas_selknam_melee_02_2;
	else if(name == ATLAS_SELKNAM_MELEE_03_1) return atlas_selknam_melee_03_1;
	else if(name == ATLAS_SELKNAM_MELEE_03_2) return atlas_selknam_melee_03_2;
	else if(name == ATLAS_ENEMY_01_1)       return atlas_enemy_01_1;
	else if(name == ATLAS_ENEMY_01_2)       return atlas_enemy_01_2;
	else if(name == ATLAS_ENEMY_02_1)       return atlas_enemy_02_1;
	else if(name == ATLAS_ENEMY_02_2)       return atlas_enemy_02_2;
	else if(name == ATLAS_ENEMY_03_1)       return atlas_enemy_03_1;
	else if(name == ATLAS_ENEMY_04_1)       return atlas_enemy_04_1;
	else if(name == ATLAS_ENEMY_04_2)       return atlas_enemy_04_2;
	else if(name == ATLAS_ENEMY_05_1)       return atlas_enemy_05_1;
	else if(name == ATLAS_ENEMY_06_1)       return atlas_enemy_06_1;
	else if(name == ATLAS_ENEMY_07_1)       return atlas_enemy_07_1;
	else if(name == ATLAS_ENEMY_07_2)       return atlas_enemy_07_2;
	else if(name == ATLAS_BOSS) return atlas_boss;
	else if(name == ATLAS_OTHERS && atlas_others) return atlas_others;
	
	return NULL;
}

AtlasName TextureHelper::GetAtlasName(TextureData data)
{
	if(      strcmp(data.atlasFileName, "bkg_gameplay")           == 0)	   return ATLAS_BKG_GAMEPLAY;
	else  if(strcmp(data.atlasFileName, "bkg_menu_foreground")           == 0)      return ATLAS_BKG_MENU_FOREGROUND;
	else  if(strcmp(data.atlasFileName, "bkg_menu_others")           == 0)      return ATLAS_BKG_MENU_OTHERS;
	else  if(strcmp(data.atlasFileName, "bkg_menu_sky")           == 0)      return ATLAS_BKG_MENU_SKY;
	else  if(strcmp(data.atlasFileName, "bkg_worldmap")           == 0)      return ATLAS_BKG_WORLDMAP;
	else  if(strcmp(data.atlasFileName, "bkg_small_0")           == 0)      return ATLAS_BKG_SMALL_0;
	else  if(strcmp(data.atlasFileName, "bkg_small_1")           == 0)      return ATLAS_BKG_SMALL_1;
	else  if(strcmp(data.atlasFileName, "atlas_common_textures_0")           == 0)      return ATLAS_COMMON_TEXTURES_0;
	else  if(strcmp(data.atlasFileName, "atlas_common_textures_1")           == 0)      return ATLAS_COMMON_TEXTURES_1;
	else  if(strcmp(data.atlasFileName, "atlas_common_textures_2")           == 0)      return ATLAS_COMMON_TEXTURES_2;
	else  if(strcmp(data.atlasFileName, "atlas_common_textures_3")           == 0)      return ATLAS_COMMON_TEXTURES_3;
	else  if(strcmp(data.atlasFileName, "atlas_common_textures_4")           == 0)      return ATLAS_COMMON_TEXTURES_4;
	else  if(strcmp(data.atlasFileName, "atlas_small_textures")           == 0)      return ATLAS_SMALL_TEXTURES;
	else  if(strcmp(data.atlasFileName, "atlas_common_trial_0")     == 0)      return ATLAS_COMMON_TRIAL_0;
	else  if(strcmp(data.atlasFileName, "terrain_combined_01_02")	== 0)      return ATLAS_TERRAIN_0;
	else  if(strcmp(data.atlasFileName, "terrain_combined_03_04")	== 0)      return ATLAS_TERRAIN_1;
	else  if(strcmp(data.atlasFileName, "atlas_props_0")	        == 0)      return ATLAS_PROPS_0;
	else  if(strcmp(data.atlasFileName, "atlas_props_1")	        == 0)      return ATLAS_PROPS_1;
	else  if(strcmp(data.atlasFileName, "atlas_props_2")	        == 0)      return ATLAS_PROPS_2;
	else  if(strcmp(data.atlasFileName, "atlas_fx_0")			    == 0)      return ATLAS_FX_0;
	else  if(strcmp(data.atlasFileName, "atlas_fx_1")	            == 0)      return ATLAS_FX_1;
	else  if(strcmp(data.atlasFileName, "atlas_fx_2")	            == 0)      return ATLAS_FX_2;
	else  if(strcmp(data.atlasFileName, "atlas_enemy_boss_1")	    == 0)      return ATLAS_BOSS;
	else  if(strcmp(data.atlasFileName, "atlas_others")	    == 0)      return ATLAS_OTHERS;

	return ATLAS_NONE;
}

RenderData TextureHelper::GetMirrorRenderData(RenderData data, MirrorImage mirror)
{
	RenderData dataToReturn;
	dataToReturn = data;

	if(mirror == MIRROR_HORIZONTAL)
	{
		Utils::SwapFloat(&dataToReturn.uvInfo.minY, &dataToReturn.uvInfo.maxY);
	}
	else if(mirror == MIRROR_VERTICAL)
	{
		Utils::SwapFloat(&dataToReturn.uvInfo.minX, &dataToReturn.uvInfo.maxX);
	}
	else if(mirror == MIRROR_DIAGONAL)
	{
		Utils::SwapFloat(&dataToReturn.uvInfo.minX, &dataToReturn.uvInfo.maxX);
		Utils::SwapFloat(&dataToReturn.uvInfo.minY, &dataToReturn.uvInfo.maxY);
	}

	return dataToReturn;
}