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

#include "utils.h"
#include "animhelper.h"
#include "timer.h"
#include "IwGx.h"
#include "common.h"
#include "s3eWindow.h"
#if !defined DESKTOPPLATFORM
#include "s3eIOSBackgroundAudio.h"
#endif
#include <algorithm>
using namespace std;

#define MAX_UTIL_EXPLOTIONS 10
float explotionInfo[MAX_UTIL_EXPLOTIONS][3]; //(x, y, z, currAnimationIndex)
Timer explotionTimer[MAX_UTIL_EXPLOTIONS];
int explotionIndex[MAX_UTIL_EXPLOTIONS];

float cannonexplotionInfo[MAX_UTIL_EXPLOTIONS][3]; //(x, y, z, currAnimationIndex)
Timer cannonexplotionTimer[MAX_UTIL_EXPLOTIONS];
int cannonexplotionIndex[MAX_UTIL_EXPLOTIONS];

//image pool
const int	MaxImages_util=1024;

CIwFVec3	s_Quad_util[4];
CIwColour	s_Colors_util[4];
CIwFVec2    s_UVs_util[4];

//batching method
int numImagesBatched_util = 0;
CIwFVec3	s_QuadBatch_util[4 * MaxImages_util];
CIwColour	s_ColorsBatch_util[4 * MaxImages_util];
CIwFVec2    s_UVsBatch_util[4 * MaxImages_util];

int anim_list[15];
int anim_time_list[15];

//Timer playfxsound_timer;

//RenderLabelTitle variables.
RenderData label_title_center_renderdata, label_title_left_renderdata, label_title_right_renderdata;
AtlasName label_title_center_atlasname, label_title_left_atlasname, label_title_right_atlasname;

RenderData pannelCornerUpLeftRD[NUM_PANELS];
RenderData pannelCornerUpRightRD[NUM_PANELS];
RenderData pannelCornerDownLeftRD[NUM_PANELS];
RenderData pannelCornerDownRightRD[NUM_PANELS];
RenderData pannelBorderUpRD[NUM_PANELS];
RenderData pannelBorderDownRD[NUM_PANELS];
RenderData pannelBorderLeftRD[NUM_PANELS];
RenderData pannelBorderRightRD[NUM_PANELS];
RenderData pannelCenterRD[NUM_PANELS];

AtlasName pannelCornerUpLeftAN[NUM_PANELS];
AtlasName pannelCornerUpRightAN[NUM_PANELS];
AtlasName pannelCornerDownLeftAN[NUM_PANELS];
AtlasName pannelCornerDownRightAN[NUM_PANELS];
AtlasName pannelBorderUpAN[NUM_PANELS];
AtlasName pannelBorderDownAN[NUM_PANELS];
AtlasName pannelBorderLeftAN[NUM_PANELS];
AtlasName pannelBorderRightAN[NUM_PANELS];
AtlasName pannelCenterAN[NUM_PANELS];

RenderData barLeftCornerRD[NUM_BARS];
RenderData barRightCornerRD[NUM_BARS];
RenderData barCenterRD[NUM_BARS];

AtlasName barLeftCornerAN[NUM_BARS];
AtlasName barRightCornerAN[NUM_BARS];
AtlasName barCenterAN[NUM_BARS];

RenderData pannel_tube_renderdata;
AtlasName pannel_tube_atlasname;

TextureData util_explotion_textureData;
AtlasName  util_explotion_atlasname;

TextureData util_cannonexplotion_textureData;
AtlasName  util_cannonexplotion_atlasname;

TextureData cursorMapTD;
AtlasName cursorMapAtlasName;

TextureData yellowCircleTD;
AtlasName yellowCircleAtlasName;

bool bCameraMovingByUtilMethod;
Timer cameraMovingByUtilMethodTimer;
float tIncrement;
CIwFVec2 aVector, bVector;
float cameraMovementVar;
int xInitialOffset, yInitialOffset;

void Utils::CopyRenderData(RenderData* destination, RenderData source)
{
	destination->bRotate = source.bRotate;
	destination->height = source.height;
	destination->width = source.width;
	destination->uvInfo = source.uvInfo;
	destination->atlasName = source.atlasName;
}

void Utils::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, int r, int g, int b, int alpha)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	s_Quad[0] = CIwSVec2(position.x, position.y);
	s_Quad[1] = CIwSVec2(position.x, position.y + tex->GetHeight());
	s_Quad[2] = CIwSVec2(position.x + tex->GetWidth(), position.y + tex->GetHeight());
	s_Quad[3] = CIwSVec2(position.x + tex->GetWidth(), position.y);

	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	s_Color[0].Set(r, g, b, alpha);
	s_Color[1].Set(r, g, b, alpha);
	s_Color[2].Set(r, g, b, alpha);
	s_Color[3].Set(r, g, b, alpha);

	IwGxSetColStream(s_Color);

	s_UVs[0] = CIwFVec2(0, 0);
	s_UVs[1] = CIwFVec2(0, 1);
	s_UVs[2] = CIwFVec2(1, 1);
	s_UVs[3] = CIwFVec2(1, 0);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, float _uvMinX, float _uvMinY, float _uvMaxX, float _uvMaxY)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	s_Quad[0] = CIwSVec2(position.x, position.y);
	s_Quad[1] = CIwSVec2(position.x, position.y + dimension.y);
	s_Quad[2] = CIwSVec2(position.x + dimension.x, position.y + dimension.y);
	s_Quad[3] = CIwSVec2(position.x + dimension.x, position.y);

	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	IwGxSetColStream(NULL);

	s_UVs[0] = CIwFVec2(_uvMinX, _uvMinY);
	s_UVs[1] = CIwFVec2(_uvMinX, _uvMaxY);
	s_UVs[2] = CIwFVec2(_uvMaxX, _uvMaxY);
	s_UVs[3] = CIwFVec2(_uvMaxX, _uvMinY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTexture(CIwTexture* tex, RenderData data, CIwSVec2 position, int r, int g, int b, int alpha)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	if(!data.bRotate)
	{
		s_Quad[0] = CIwSVec2(position.x, position.y);
		s_Quad[1] = CIwSVec2(position.x, position.y + data.height);
		s_Quad[2] = CIwSVec2(position.x + data.width, position.y + data.height);
		s_Quad[3] = CIwSVec2(position.x + data.width, position.y);
	}
	else
	{
		int newWidth = data.height;
		int newHeight = data.width;

		s_Quad[3] = CIwSVec2(position.x, position.y);
		s_Quad[0] = CIwSVec2(position.x, position.y + newHeight);
		s_Quad[1] = CIwSVec2(position.x + newWidth, position.y + newHeight);
		s_Quad[2] = CIwSVec2(position.x + newWidth, position.y);
	}

	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	s_Color[0].Set(r, g, b, alpha);
	s_Color[1].Set(r, g, b, alpha);
	s_Color[2].Set(r, g, b, alpha);
	s_Color[3].Set(r, g, b, alpha);

	IwGxSetColStream(s_Color);

	s_UVs[0] = CIwFVec2(data.uvInfo.minX, data.uvInfo.minY);
	s_UVs[1] = CIwFVec2(data.uvInfo.minX, data.uvInfo.maxY);
	s_UVs[2] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.maxY);
	s_UVs[3] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.minY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTexture(bool p, CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, float _uvMinX, float _uvMinY, float _uvMaxX, float _uvMaxY, bool bRotate)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	if(!bRotate)
	{
		s_Quad[0] = CIwSVec2(position.x, position.y);
		s_Quad[1] = CIwSVec2(position.x, position.y + dimension.y);
		s_Quad[2] = CIwSVec2(position.x + dimension.x, position.y + dimension.y);
		s_Quad[3] = CIwSVec2(position.x + dimension.x, position.y);
	}
	else
	{
		int newWidth = dimension.y;
		int newHeight = dimension.x;

		s_Quad[3] = CIwSVec2(position.x, position.y);
		s_Quad[0] = CIwSVec2(position.x, position.y + newHeight);
		s_Quad[1] = CIwSVec2(position.x + newWidth, position.y + newHeight);
		s_Quad[2] = CIwSVec2(position.x + newWidth, position.y);
	}


	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	IwGxSetColStream(NULL);

	s_UVs[0] = CIwFVec2(_uvMinX, _uvMinY);
	s_UVs[1] = CIwFVec2(_uvMinX, _uvMaxY);
	s_UVs[2] = CIwFVec2(_uvMaxX, _uvMaxY);
	s_UVs[3] = CIwFVec2(_uvMaxX, _uvMinY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData data, RotateImage rotate, uint8 alpha)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	if(!data.bRotate)
	{
		int n1 = 0;
		int n2 = 1;
		int n3 = 2;
		int n4 = 3;

		if(rotate == ROTATE_270)
		{
			n1 = 1;
			n2 = 2;
			n3 = 3;
			n4 = 0;
		}
		else if(rotate == ROTATE_180)
		{
			n1 = 2;
			n2 = 3;
			n3 = 0;
			n4 = 1;
		}

		s_Quad[n1] = CIwSVec2(position.x, position.y);
		s_Quad[n2] = CIwSVec2(position.x, position.y + data.height);
		s_Quad[n3] = CIwSVec2(position.x + data.width, position.y + data.height);
		s_Quad[n4] = CIwSVec2(position.x + data.width, position.y);
	}
	else
	{
		int n1 = 3;
		int n2 = 0;
		int n3 = 1;
		int n4 = 2;

		if(rotate == ROTATE_270)
		{
			n1 = 2;
			n2 = 3;
			n3 = 0;
			n4 = 1;
		}
		else if(rotate == ROTATE_180)
		{
			n1 = 3;
			n2 = 0;
			n3 = 1;
			n4 = 2;
		}

		int newWidth = data.height;
		int newHeight = data.width;

		s_Quad[3] = CIwSVec2(position.x, position.y);
		s_Quad[0] = CIwSVec2(position.x, position.y + newHeight);
		s_Quad[1] = CIwSVec2(position.x + newWidth, position.y + newHeight);
		s_Quad[2] = CIwSVec2(position.x + newWidth, position.y);
	}


	IwGxSetVertStreamScreenSpace(s_Quad, 4);

	s_Color[0].Set(255, 255, 255, alpha);
	s_Color[1].Set(255, 255, 255, alpha);
	s_Color[2].Set(255, 255, 255, alpha);
	s_Color[3].Set(255, 255, 255, alpha);
	IwGxSetColStream(s_Color, 4);

	s_UVs[0] = CIwFVec2(data.uvInfo.minX, data.uvInfo.minY);
	s_UVs[1] = CIwFVec2(data.uvInfo.minX, data.uvInfo.maxY);
	s_UVs[2] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.maxY);
	s_UVs[3] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.minY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData* data, int degree, int alpha)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	if(!data->bRotate)
	{
		if(degree == 1)
		{
			s_Quad[3] = CIwSVec2(position.x, position.y);
			s_Quad[0] = CIwSVec2(position.x, position.y + data->height);
			s_Quad[1] = CIwSVec2(position.x + data->width, position.y + data->height);
			s_Quad[2] = CIwSVec2(position.x + data->width, position.y);
		}
		else
		{
			s_Quad[0] = CIwSVec2(position.x, position.y);
			s_Quad[1] = CIwSVec2(position.x, position.y + data->height);
			s_Quad[2] = CIwSVec2(position.x + data->width, position.y + data->height);
			s_Quad[3] = CIwSVec2(position.x + data->width, position.y);
		}
	}
	else
	{
		int newWidth = data->height;
		int newHeight = data->width;

		s_Quad[3] = CIwSVec2(position.x, position.y);
		s_Quad[0] = CIwSVec2(position.x, position.y + newHeight);
		s_Quad[1] = CIwSVec2(position.x + newWidth, position.y + newHeight);
		s_Quad[2] = CIwSVec2(position.x + newWidth, position.y);
	}


	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	s_Color[0].Set(255, 255, 255, alpha);
	s_Color[1].Set(255, 255, 255, alpha);
	s_Color[2].Set(255, 255, 255, alpha);
	s_Color[3].Set(255, 255, 255, alpha);
	IwGxSetColStream(s_Color, 4);

	s_UVs[0] = CIwFVec2(data->uvInfo.minX, data->uvInfo.minY);
	s_UVs[1] = CIwFVec2(data->uvInfo.minX, data->uvInfo.maxY);
	s_UVs[2] = CIwFVec2(data->uvInfo.maxX, data->uvInfo.maxY);
	s_UVs[3] = CIwFVec2(data->uvInfo.maxX, data->uvInfo.minY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTextureWithAlpha(CIwTexture* tex, CIwFVec2 position, RenderData data, int alpha)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwFVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	if(!data.bRotate)
	{
		int n1 = 0;
		int n2 = 1;
		int n3 = 2;
		int n4 = 3;

		s_Quad[n1] = CIwFVec2(position.x, position.y);
		s_Quad[n2] = CIwFVec2(position.x, position.y + data.height);
		s_Quad[n3] = CIwFVec2(position.x + data.width, position.y + data.height);
		s_Quad[n4] = CIwFVec2(position.x + data.width, position.y);
	}
	else
	{
		int n1 = 3;
		int n2 = 0;
		int n3 = 1;
		int n4 = 2;

		int newWidth = data.height;
		int newHeight = data.width;

		s_Quad[3] = CIwFVec2(position.x, position.y);
		s_Quad[0] = CIwFVec2(position.x, position.y + newHeight);
		s_Quad[1] = CIwFVec2(position.x + newWidth, position.y + newHeight);
		s_Quad[2] = CIwFVec2(position.x + newWidth, position.y);
	}

	s_Quad[0] = CIwFVec2((float)((int)s_Quad[0].x), (float)((int)s_Quad[0].y));
	s_Quad[1] = CIwFVec2((float)((int)s_Quad[1].x), (float)((int)s_Quad[1].y));
	s_Quad[2] = CIwFVec2((float)((int)s_Quad[2].x), (float)((int)s_Quad[2].y));
	s_Quad[3] = CIwFVec2((float)((int)s_Quad[3].x), (float)((int)s_Quad[3].y));


	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	s_Color[0].Set(255, 255, 255, alpha);
	s_Color[1].Set(255, 255, 255, alpha);
	s_Color[2].Set(255, 255, 255, alpha);
	s_Color[3].Set(255, 255, 255, alpha);
	IwGxSetColStream(s_Color, 4);

	s_UVs[0] = CIwFVec2(data.uvInfo.minX, data.uvInfo.minY);
	s_UVs[1] = CIwFVec2(data.uvInfo.minX, data.uvInfo.maxY);
	s_UVs[2] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.maxY);
	s_UVs[3] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.minY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTextureFloat(CIwTexture* tex, CIwFVec2 position, RenderData data)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwFVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	if(!data.bRotate)
	{
		s_Quad[0] = CIwFVec2(position.x, position.y);
		s_Quad[1] = CIwFVec2(position.x, position.y + data.height);
		s_Quad[2] = CIwFVec2(position.x + data.width, position.y + data.height);
		s_Quad[3] = CIwFVec2(position.x + data.width, position.y);
	}
	else
	{
		int newWidth = data.height;
		int newHeight = data.width;

		s_Quad[3] = CIwFVec2(position.x, position.y);
		s_Quad[0] = CIwFVec2(position.x, position.y + newHeight);
		s_Quad[1] = CIwFVec2(position.x + newWidth, position.y + newHeight);
		s_Quad[2] = CIwFVec2(position.x + newWidth, position.y);
	}

	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	IwGxSetColStream(NULL);

	s_UVs[0] = CIwFVec2(data.uvInfo.minX, data.uvInfo.minY);
	s_UVs[1] = CIwFVec2(data.uvInfo.minX, data.uvInfo.maxY);
	s_UVs[2] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.maxY);
	s_UVs[3] = CIwFVec2(data.uvInfo.maxX, data.uvInfo.minY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, UVInfo uv, bool bRotate)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	static CIwSVec2 s_Quad[4];
	static CIwFVec2 s_UVs[4];
	static CIwColour s_Color[4];

	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);

	if(!bRotate)
	{
		s_Quad[0] = CIwSVec2(position.x, position.y);
		s_Quad[1] = CIwSVec2(position.x, position.y + dimension.y);
		s_Quad[2] = CIwSVec2(position.x + dimension.x, position.y + dimension.y);
		s_Quad[3] = CIwSVec2(position.x + dimension.x, position.y);
	}
	else
	{
		int newWidth = dimension.y;
		int newHeight = dimension.x;

		s_Quad[3] = CIwSVec2(position.x, position.y);
		s_Quad[0] = CIwSVec2(position.x, position.y + newHeight);
		s_Quad[1] = CIwSVec2(position.x + newWidth, position.y + newHeight);
		s_Quad[2] = CIwSVec2(position.x + newWidth, position.y);
	}


	IwGxSetVertStreamScreenSpace(s_Quad, 4);	

	IwGxSetColStream(NULL);

	s_UVs[0] = CIwFVec2(uv.minX, uv.minY);
	s_UVs[1] = CIwFVec2(uv.minX, uv.maxY);
	s_UVs[2] = CIwFVec2(uv.maxX, uv.maxY);
	s_UVs[3] = CIwFVec2(uv.maxX, uv.minY);
	IwGxSetUVStream(s_UVs);

	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);

	IwGxFlush();
}

void Utils::InitQuadDrawing(CIwTexture* tex, bool bAlphaMode)
{
	CIwMaterial* pMat = NULL;
	pMat = IW_GX_ALLOC_MATERIAL();
	if(bAlphaMode)
		pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(tex);
	IwGxSetMaterial(pMat);
	numImagesBatched_util = 0;
}

void Utils::DrawQuadBatch(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX, float _uvMaxX, float _uvMinY, float _uvMaxY, float _offsetX, float _offsetY, int r, int g, int b, int a)
{
	int index = numImagesBatched_util * 4;

	_posX += _offsetX; 
	_posY += _offsetY;

	s_QuadBatch_util[index].x = _posX;
	s_QuadBatch_util[index].y = _posY;
	s_QuadBatch_util[index].z = _posZ;

	s_QuadBatch_util[index + 1].x = _posX;
	s_QuadBatch_util[index + 1].y = _posY + _height;
	s_QuadBatch_util[index + 1].z = _posZ;

	s_QuadBatch_util[index + 2].x = _posX + _width;
	s_QuadBatch_util[index + 2].y = _posY + _height;
	s_QuadBatch_util[index + 2].z = _posZ;

	s_QuadBatch_util[index + 3].x = _posX + _width;
	s_QuadBatch_util[index + 3].y = _posY;
	s_QuadBatch_util[index + 3].z = _posZ;

	s_UVsBatch_util[index] = CIwFVec2(_uvMinX, _uvMinY);
	s_UVsBatch_util[index + 1] = CIwFVec2(_uvMinX, _uvMaxY);
	s_UVsBatch_util[index + 2] = CIwFVec2(_uvMaxX, _uvMaxY);
	s_UVsBatch_util[index + 3] = CIwFVec2(_uvMaxX, _uvMinY);

	s_ColorsBatch_util[index].Set(r, g, b, a);
	s_ColorsBatch_util[index + 1].Set(r, g, b, a);
	s_ColorsBatch_util[index + 2].Set(r, g, b, a);
	s_ColorsBatch_util[index + 3].Set(r, g, b, a);

	numImagesBatched_util++;
}

void Utils::RenderQuadBatch()
{
	IwGxSetVertStreamModelSpace(s_QuadBatch_util, numImagesBatched_util * 4);
	IwGxSetColStream(s_ColorsBatch_util, numImagesBatched_util * 4);
	IwGxSetUVStream(s_UVsBatch_util);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, numImagesBatched_util * 4);
	IwGxFlush();
}

int Utils::GetSizeArray(int* arr)
{
	int i;
	for(i = 0; arr[i] != -1; i++);
	return i;
}

void Utils::SetWalkAnim(IsoPawnAnim anim, Animation* animations)
{
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Walk, DIR_North);
	animations->AddAnimation("WalkNorth",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Walk, DIR_South);
	animations->AddAnimation("WalkSouth",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Walk, DIR_East);
	animations->AddAnimation("WalkEast",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Walk, DIR_West);
	animations->AddAnimation("WalkWest",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
}

void Utils::SetAttackAnim(IsoPawnAnim anim, Animation* animations)
{
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_North);
	animations->AddAnimation("AttackNorth",      anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_South);
	animations->AddAnimation("AttackSouth",      anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_East);
	animations->AddAnimation("AttackEast",       anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_West);
	animations->AddAnimation("AttackWest",       anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_NorthEast);
	animations->AddAnimation("AttackNorthEast",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_NorthWest);
	animations->AddAnimation("AttackNorthWest",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_SouthWest);
	animations->AddAnimation("AttackSouthWest",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Attack, DIR_SouthEast);
	animations->AddAnimation("AttackSouthEast",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
}

void Utils::SetDeathAnim(IsoPawnAnim anim, Animation* animations)
{
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_North);
	animations->AddAnimation("DieNorth",      anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_South);
	animations->AddAnimation("DieSouth",      anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_East);
	animations->AddAnimation("DieEast",       anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_West);
	animations->AddAnimation("DieWest",       anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_NorthEast);
	animations->AddAnimation("DieNorthEast",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_NorthWest);
	animations->AddAnimation("DieNorthWest",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_SouthWest);
	animations->AddAnimation("DieSouthWest",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list,          anim_time_list, anim, ANIM_Death, DIR_SouthEast);
	animations->AddAnimation("DieSouthEast",  anim_list,      Utils::GetSizeArray(anim_list), anim_time_list);
}

void Utils::SetIdleAnim(IsoPawnAnim anim, Animation* animations)
{
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Idle, DIR_North);
	animations->AddAnimation("IdleNorth",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Idle, DIR_South);
	animations->AddAnimation("IdleSouth",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Idle, DIR_East);
	animations->AddAnimation("IdleEast",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
	AnimHelper::RetrieveAnim(anim_list, anim_time_list, anim, ANIM_Idle, DIR_West);
	animations->AddAnimation("IdleWest",  anim_list, Utils::GetSizeArray(anim_list), anim_time_list);
}

void Utils::SetEnemyAnim(IsoEnemyType enemyType, Animation* animations, IsoPawn* pawn)
{
	CIwTexture* texture = NULL;
	animations->ClearAnimation();
	if(!bUseHDImages)
	//Soldier Case
	{
		if(enemyType == ENEMY_Soldier || enemyType == ENEMY_Captain)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_ENEMY_01_1, info, 0);
			pawn->AddAtlasData(ATLAS_ENEMY_01_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_ENEMY_Soldier, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Soldier, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Soldier, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Soldier, animations);
		}
		else if(enemyType == ENEMY_Convict)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_ENEMY_03_1, info, 0);
			pawn->SetMaxAtlasID(99);

			Utils::SetWalkAnim(ANIM_ENEMY_Convict, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Convict, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Convict, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Convict, animations);
		}
		else if(enemyType == ENEMY_Varon)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_ENEMY_02_1, info, 0);
			pawn->AddAtlasData(ATLAS_ENEMY_02_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_ENEMY_Varon, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Varon, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Varon, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Varon, animations);
		}
		else if(enemyType == ENEMY_Scientist)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_ENEMY_04_1, info, 0);
			pawn->AddAtlasData(ATLAS_ENEMY_04_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_ENEMY_Scientist, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Scientist, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Scientist, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Scientist, animations);
		}
		else if(enemyType == ENEMY_Tank)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_ENEMY_05_1, info, 0);
			pawn->SetMaxAtlasID(99);

			Utils::SetWalkAnim(ANIM_ENEMY_Tank, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Tank, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Tank, animations);
		}
		else if(enemyType == ENEMY_Rabbit)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_ENEMY_06_1, info, 0);
			pawn->SetMaxAtlasID(99);

			Utils::SetWalkAnim(ANIM_ENEMY_Rabbit, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Rabbit, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Rabbit, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Rabbit, animations);

			//Falta Slow Walk...
		}
		else if(enemyType == ENEMY_Pirate)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_ENEMY_07_1, info, 0);
			pawn->AddAtlasData(ATLAS_ENEMY_07_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_ENEMY_Pirate, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Pirate, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Pirate, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Pirate, animations);
		}
	}
	else
	{
		//Soldier Case
		if(enemyType == ENEMY_Soldier || enemyType == ENEMY_Captain)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_1.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_1.height;

			pawn->AddAtlasData(ATLAS_HD_ENEMY_1, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_ENEMY_Soldier, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Soldier, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Soldier, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Soldier, animations);
		}
		else if(enemyType == ENEMY_Varon)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_2.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_2.height;

			pawn->AddAtlasData(ATLAS_HD_ENEMY_2, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_ENEMY_Varon, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Varon, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Varon, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Varon, animations);
		}
		else if(enemyType == ENEMY_Convict)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 5;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_3.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_3.height;

			pawn->AddAtlasData(ATLAS_HD_ENEMY_3, info, 0);
			pawn->SetMaxAtlasID(20);

			Utils::SetWalkAnim(ANIM_ENEMY_Convict, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Convict, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Convict, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Convict, animations);
		}
		else if(enemyType == ENEMY_Scientist)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;

			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_4_01.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_4_01.height;
			pawn->AddAtlasData(ATLAS_HD_ENEMY_4_01, info, 0);

			info.rowSize = 3;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_4_02.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_4_02.height;
			pawn->AddAtlasData(ATLAS_HD_ENEMY_4_02, info, 1);

			pawn->SetMaxAtlasID(70);

			Utils::SetWalkAnim(ANIM_ENEMY_Scientist, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Scientist, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Scientist, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Scientist, animations);
		}
		else if(enemyType == ENEMY_Tank)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_5.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_5.height;

			pawn->AddAtlasData(ATLAS_HD_ENEMY_5, info, 0);
			pawn->SetMaxAtlasID(28);

			Utils::SetWalkAnim(ANIM_ENEMY_Tank, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Tank, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Tank, animations);
		}
		else if(enemyType == ENEMY_Rabbit)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_6.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_6.height;

			pawn->AddAtlasData(ATLAS_HD_ENEMY_6, info, 0);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_ENEMY_Rabbit, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Rabbit, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Rabbit, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Rabbit, animations);

			//Falta Slow Walk...
		}
		else if(enemyType == ENEMY_Pirate)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_enemy_7.width;
			info.uvHeight = 144.0f / atlas_hdsize_enemy_1.height;

			pawn->AddAtlasData(ATLAS_HD_ENEMY_7, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_ENEMY_Pirate, animations);
			Utils::SetAttackAnim(ANIM_ENEMY_Pirate, animations);
			Utils::SetIdleAnim(ANIM_ENEMY_Pirate, animations);
			Utils::SetDeathAnim(ANIM_ENEMY_Pirate, animations);
		}
	}
}

void Utils::SetSelknamAnim(IsoSelknamType SelknamType, Animation* animations, IsoPawn* pawn)
{
	animations->ClearAnimation();
	if(!bUseHDImages)
	{
		if(SelknamType == SELKNAM_Archer_Base)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_ARCHER_01_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_ARCHER_01_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Archer_Base, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Archer_Base, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Archer_Base, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Archer_Base, animations);
		}
		else if(SelknamType == SELKNAM_Warrior_Base)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_MELEE_01_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_MELEE_01_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Warrior_Base, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Warrior_Base, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Warrior_Base, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Warrior_Base, animations);
		}
		else if(SelknamType == SELKNAM_Archer_Poisoner)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_ARCHER_03_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_ARCHER_03_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Archer_Poisoner, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Archer_Poisoner, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Archer_Poisoner, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Archer_Poisoner, animations);

		}
		else if(SelknamType == SELKNAM_Archer_Retarder)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_ARCHER_02_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_ARCHER_02_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Archer_Retarder, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Archer_Retarder, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Archer_Retarder, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Archer_Retarder, animations);
		}
		else if(SelknamType == SELKNAM_Shaman_Healer)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_MAGE_01_1, info, 0);

			info.rowSize = 4;
			info.uvWidth = 64 / 256.0f;
			info.uvHeight = 96 / 256.0f;
			pawn->AddAtlasData(ATLAS_SELKNAM_MAGE_01_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Shaman_Healer, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Shaman_Healer, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Shaman_Healer, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Shaman_Healer, animations);
		}
		else if(SelknamType == SELKNAM_Shaman_Might)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_MAGE_03_1, info, 0);

			info.rowSize = 4;
			info.uvWidth = 64 / 256.0f;
			info.uvHeight = 96 / 256.0f;
			pawn->AddAtlasData(ATLAS_SELKNAM_MAGE_03_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Shaman_Might, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Shaman_Might, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Shaman_Might, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Shaman_Might, animations);
		}
		else if(SelknamType == SELKNAM_Shaman_Haste)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;
			pawn->AddAtlasData(ATLAS_SELKNAM_MAGE_02_1, info, 0);

			info.rowSize = 4;
			info.uvWidth = 64 / 256.0f;
			info.uvHeight = 96 / 256.0f;
			pawn->AddAtlasData(ATLAS_SELKNAM_MAGE_02_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Shaman_Haste, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Shaman_Haste, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Shaman_Haste, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Shaman_Haste, animations);
		}
		else if(SelknamType == SELKNAM_Warrior_Tank)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_MELEE_03_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_MELEE_03_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Warrior_Tank, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Warrior_Tank, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Warrior_Tank, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Warrior_Tank, animations);

		}
		else if(SelknamType == SELKNAM_Warrior_Berserk)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_MELEE_02_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_MELEE_02_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Warrior_Berserk, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Warrior_Berserk, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Warrior_Berserk, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Warrior_Berserk, animations);

		}
		else if(SelknamType == SELKNAM_Bowler_Rooter)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_BOWLER_02_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_BOWLER_02_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
		}
		else if(SelknamType == SELKNAM_Bowler_Confuser)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_BOWLER_03_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_BOWLER_03_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
		}
		else if(SelknamType == SELKNAM_Bowler_Stuner)
		{
			pawn->setWidth(64 * globalScale);
			pawn->setHeight(96 * globalScale);

			RenderInfo info;
			info.rowSize = 8;
			info.uvWidth = 64 / 512.0f;
			info.uvHeight = 96 / 512.0f;

			pawn->AddAtlasData(ATLAS_SELKNAM_BOWLER_01_1, info, 0);
			pawn->AddAtlasData(ATLAS_SELKNAM_BOWLER_01_2, info, 1);
			pawn->SetMaxAtlasID(40);

			Utils::SetWalkAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
		}
	}
	else
	{
		if(SelknamType == SELKNAM_Archer_Base)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_archer.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_archer.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_ARCHER_1, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_SELKNAM_Archer_Base, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Archer_Base, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Archer_Base, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Archer_Base, animations);
		}
		else if(SelknamType == SELKNAM_Archer_Retarder)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_archer.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_archer.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_ARCHER_2, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_SELKNAM_Archer_Retarder, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Archer_Retarder, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Archer_Retarder, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Archer_Retarder, animations);
		}
		else if(SelknamType == SELKNAM_Archer_Poisoner)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_archer.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_archer.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_ARCHER_3, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_SELKNAM_Archer_Poisoner, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Archer_Poisoner, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Archer_Poisoner, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Archer_Poisoner, animations);

		}
		else if(SelknamType == SELKNAM_Bowler_Stuner)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;

			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_bowler_01.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_bowler_01.height;
			pawn->AddAtlasData(ATLAS_HD_SELKNAM_BOWLER_1_01, info, 0);

			info.rowSize = 3;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_bowler_02.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_bowler_02.height;
			pawn->AddAtlasData(ATLAS_HD_SELKNAM_BOWLER_1_02, info, 1);

			pawn->SetMaxAtlasID(70);

			Utils::SetWalkAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Bowler_Stuner, animations);
		}
		else if(SelknamType == SELKNAM_Bowler_Rooter)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;

			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_bowler_01.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_bowler_01.height;
			pawn->AddAtlasData(ATLAS_HD_SELKNAM_BOWLER_2_01, info, 0);

			info.rowSize = 3;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_bowler_02.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_bowler_02.height;
			pawn->AddAtlasData(ATLAS_HD_SELKNAM_BOWLER_2_02, info, 1);

			pawn->SetMaxAtlasID(70);

			Utils::SetWalkAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Bowler_Rooter, animations);
		}
		else if(SelknamType == SELKNAM_Bowler_Confuser)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;

			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_bowler_01.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_bowler_01.height;
			pawn->AddAtlasData(ATLAS_HD_SELKNAM_BOWLER_3_01, info, 0);

			info.rowSize = 3;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_bowler_02.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_bowler_02.height;
			pawn->AddAtlasData(ATLAS_HD_SELKNAM_BOWLER_3_02, info, 1);

			pawn->SetMaxAtlasID(70);

			Utils::SetWalkAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Bowler_Confuser, animations);
		}
		else if(SelknamType == SELKNAM_Warrior_Base)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_melee.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_melee.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_MELEE_1, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_SELKNAM_Warrior_Base, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Warrior_Base, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Warrior_Base, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Warrior_Base, animations);
		}
		else if(SelknamType == SELKNAM_Warrior_Berserk)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_melee.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_melee.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_MELEE_2, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_SELKNAM_Warrior_Berserk, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Warrior_Berserk, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Warrior_Berserk, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Warrior_Berserk, animations);

		}
		else if(SelknamType == SELKNAM_Warrior_Tank)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 10;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_melee.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_melee.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_MELEE_3, info, 0);
			pawn->SetMaxAtlasID(68);

			Utils::SetWalkAnim(ANIM_SELKNAM_Warrior_Tank, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Warrior_Tank, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Warrior_Tank, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Warrior_Tank, animations);

		}
		else if(SelknamType == SELKNAM_Shaman_Healer)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 9;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_mage.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_mage.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_MAGE_1, info, 0);
			pawn->SetMaxAtlasID(48);

			Utils::SetWalkAnim(ANIM_SELKNAM_Shaman_Healer, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Shaman_Healer, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Shaman_Healer, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Shaman_Healer, animations);
		}
		else if(SelknamType == SELKNAM_Shaman_Haste)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 9;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_mage.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_mage.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_MAGE_2, info, 0);
			pawn->SetMaxAtlasID(48);

			Utils::SetWalkAnim(ANIM_SELKNAM_Shaman_Haste, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Shaman_Haste, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Shaman_Haste, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Shaman_Haste, animations);
		}
		else if(SelknamType == SELKNAM_Shaman_Might)
		{
			pawn->setWidth(96.0f);
			pawn->setHeight(144.0f);

			RenderInfo info;
			info.rowSize = 9;
			info.uvWidth = 96.0f / atlas_hdsize_selknam_mage.width;
			info.uvHeight = 144.0f / atlas_hdsize_selknam_mage.height;

			pawn->AddAtlasData(ATLAS_HD_SELKNAM_MAGE_3, info, 0);
			pawn->SetMaxAtlasID(48);

			Utils::SetWalkAnim(ANIM_SELKNAM_Shaman_Might, animations);
			Utils::SetAttackAnim(ANIM_SELKNAM_Shaman_Might, animations);
			Utils::SetIdleAnim(ANIM_SELKNAM_Shaman_Might, animations);
			Utils::SetDeathAnim(ANIM_SELKNAM_Shaman_Might, animations);
		}
	}
}

 void Utils::SetBuffImageUV(Skill_Type type, float* uvMaxX, float* uvMinX, float* uvMaxY, float* uvMinY)
 {
	 float icon_width = 0.143f;

	 if(type == Skill_Type_Might)
	 {
		 *uvMinX = icon_width * 0;
		 *uvMaxX = icon_width * 1;
	 }
	 else if(type == Skill_Type_Haste)
	 {
		 *uvMinX = icon_width * 1;
		 *uvMaxX = icon_width * 2;
	 }
	 else if(type == Skill_Type_Confusion)
	 {
		 *uvMinX = icon_width * 2;
		 *uvMaxX = icon_width * 3;
	 }
	 else if(type == Skill_Type_Slow)
	 {
		 *uvMinX = icon_width * 3;
		 *uvMaxX = icon_width * 4;
	 }
	 else if(type == Skill_Type_Poison)
	 {
		 *uvMinX = icon_width * 4;
		 *uvMaxX = icon_width * 5;
	 }
	 else if(type == Skill_Type_PoisonOnDeath)
	 {
		 *uvMinX = icon_width * 4;
		 *uvMaxX = icon_width * 5;
	 }
	 else if(type == Skill_Type_Root)
	 {
		 *uvMinX = icon_width * 5;
		 *uvMaxX = icon_width * 6;
	 }
	 else if(type == Skill_Type_Stun)
	 {
		 *uvMinX = icon_width * 6;
		 *uvMaxX = icon_width * 7;
	 }

	 *uvMinY = 0.0f;
	 *uvMaxY = 1.0f;

	 *uvMinX    = (((*uvMinX) * 112.0f) + buffBarTextureData.textureXPos) / (float)buffBarTextureData.atlasWidth;
	 *uvMaxY    = (((*uvMaxY) * 16.0f)  + buffBarTextureData.textureYPos) / (float)buffBarTextureData.atlasHeight;
	 *uvMaxX    = (((*uvMaxX) * 112.0f) + buffBarTextureData.textureXPos) / (float)buffBarTextureData.atlasWidth;
	 *uvMinY    = (((*uvMinY) * 16.0f)  + buffBarTextureData.textureYPos) / (float)buffBarTextureData.atlasHeight;
 }

 int Utils::Round(float num)
 {
	 return (int)num;
 }

 void Utils::RenderColor(uint32 color, CIwSVec2 position, CIwSVec2 size)
 {
	 CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	 pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	 IwGxSetMaterial(pMat);
	 static CIwSVec2 xy3[4];
	 int xPos = size.x;
	 int yPos = size.y;
	 xy3[0].x = position.x,	      xy3[0].y = position.y;
	 xy3[1].x = position.x,		  xy3[1].y = position.y + yPos;
	 xy3[2].x = position.x+ xPos, xy3[2].y = position.y + yPos;
	 xy3[3].x = position.x+ xPos, xy3[3].y = position.y + 0;
	 IwGxSetVertStreamScreenSpace(xy3, 4);

	 static CIwColour cols[4];
	 cols[0].Set(color);
	 cols[1].Set(color);
	 cols[2].Set(color);
	 cols[3].Set(color);

	 IwGxSetColStream(cols, 4);
	 IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
 }

 void Utils::RenderColor(uint32 colorA, uint32 colorB, CIwSVec2 position, CIwSVec2 size, bool bVerticalDegrade)
 {
	 CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	 pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	 IwGxSetMaterial(pMat);
	 static CIwSVec2 xy3[4];
	 int xPos = size.x;
	 int yPos = size.y;
	 xy3[0].x = position.x,	      xy3[0].y = position.y;
	 xy3[1].x = position.x,		  xy3[1].y = position.y + yPos;
	 xy3[2].x = position.x+ xPos, xy3[2].y = position.y + yPos;
	 xy3[3].x = position.x+ xPos, xy3[3].y = position.y + 0;
	 IwGxSetVertStreamScreenSpace(xy3, 4);

	 static CIwColour cols[4];

	 cols[0].Set(colorA);
	 cols[1].Set(colorA);
	 cols[2].Set(colorB);
	 cols[3].Set(colorB);

	 if(bVerticalDegrade)
	 {
		 cols[0].Set(colorA);
		 cols[1].Set(colorB);
		 cols[2].Set(colorB);
		 cols[3].Set(colorA);
	 }

	 IwGxSetColStream(cols, 4);
	 IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
 }

 void Utils::RenderColor(int r, int g, int b, int a, CIwSVec2 position, CIwSVec2 size)
 {
	 CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	 pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	 IwGxSetMaterial(pMat);
	 static CIwSVec2 xy3[4];
	 int xPos = size.x;
	 int yPos = size.y;
	 xy3[0].x = position.x,	      xy3[0].y = position.y;
	 xy3[1].x = position.x,		  xy3[1].y = position.y + yPos;
	 xy3[2].x = position.x+ xPos, xy3[2].y = position.y + yPos;
	 xy3[3].x = position.x+ xPos, xy3[3].y = position.y + 0;
	 IwGxSetVertStreamScreenSpace(xy3, 4);

	 static CIwColour cols[4];
	 cols[0].Set(r, g, b , a);
	 cols[1].Set(r, g, b , a);
	 cols[2].Set(r, g, b , a);
	 cols[3].Set(r, g, b , a);

	 IwGxSetColStream(cols, 4);
	 IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
 }

 void Utils::RenderText(const char* text, CIwRect rect, CIwGxFont* pfont, uint32 color, IwGxFontAlignHor alig_h, IwGxFontAlignVer alig_v, bool bShadow, int distShadow)
 {
	 IwGxFontSetEncoding(IW_GX_FONT_UTF_8);

	 int myDistShadow = 1;
	 if(distShadow >= 1)
		 myDistShadow = distShadow;

	 
	 if(myDistShadow >= 1)
	 {
		 IwGxFontSetRect(CIwRect(rect.x - myDistShadow, rect.y + myDistShadow, rect.w, rect.h));
		 IwGxFontSetFont(pfont);
		 IwGxFontSetAlignmentHor(alig_h);
		 IwGxFontSetAlignmentVer(alig_v);
		 IwGxFontSetCol(0xff000000);
		 CIwGxFontPreparedData data;
		 IwGxFontPrepareText(data, text);
		 IwGxFontDrawText(data);
	 }
	 
	 IwGxFontSetRect(rect);
	 IwGxFontSetFont(pfont);
	 IwGxFontSetAlignmentHor(alig_h);
	 IwGxFontSetAlignmentVer(alig_v);
	 IwGxFontSetCol(color);
	 CIwGxFontPreparedData data;
	 IwGxFontPrepareText(data, text);
	 IwGxFontDrawText(data);
 }

 void Utils::RenderText(const char* text, CIwRect rect, CIwGxFont* pfont, CIwColour color, IwGxFontAlignHor alig_h, IwGxFontAlignVer alig_v, bool bShadow, int distShadow)
 {
	 IwGxFontSetEncoding(IW_GX_FONT_UTF_8);

	 int myDistShadow = 1;
	 if(distShadow >= 1)
		 myDistShadow = distShadow;
	
	 CIwColour myShadowFontColor;
	 myShadowFontColor.r = 0;
	 myShadowFontColor.g = 0;
	 myShadowFontColor.b = 0;
	 myShadowFontColor.a = color.a;

	 if(myDistShadow >= 1)
	 {
		 IwGxFontSetRect(CIwRect(rect.x - myDistShadow, rect.y + myDistShadow, rect.w, rect.h));
		 IwGxFontSetFont(pfont);
		 IwGxFontSetAlignmentHor(alig_h);
		 IwGxFontSetAlignmentVer(alig_v);
		 IwGxFontSetCol(myShadowFontColor);
		 CIwGxFontPreparedData data;
		 IwGxFontPrepareText(data, text);
		 IwGxFontDrawText(data);
	 }

	 IwGxFontSetRect(rect);
	 IwGxFontSetFont(pfont);
	 IwGxFontSetAlignmentHor(alig_h);
	 IwGxFontSetAlignmentVer(alig_v);
	 IwGxFontSetCol(color);
	 CIwGxFontPreparedData data;
	 IwGxFontPrepareText(data, text);
	 IwGxFontDrawText(data);
 }

 bool Utils::GameIsPaused()
 {
	 return bPause || bPauseGame;
 }


 bool Utils::CanAttackByAnimation(IsoEnemyType type, int id)
 {
	 if(type == ENEMY_Scientist)
	 {
		 return (id == 3  ||
			     id == 7  ||
				 id == 11 ||
				 id == 15 ||
				 id == 19 ||
				 id == 23 ||
				 id == 27 ||
				 id == 31);
	 }

	 return (id == 2  ||
			 id == 5  ||
			 id == 8  ||
			 id == 11 ||
			 id == 14 ||
			 id == 17 ||
			 id == 20 ||
			 id == 23 );

	 /*
	 return (type == ENEMY_Soldier && (id == 2  ||
		                               id == 5  ||
									   id == 8  ||
									   id == 11 ||
									   id == 14 ||
									   id == 17 ||
									   id == 20 ||
									   id == 23 ));
	 */
 }

 bool Utils::CanAttackByAnimation(IsoSelknamType type, int id)
 {
	 if(type == SELKNAM_Bowler_Confuser || 
		type == SELKNAM_Bowler_Rooter   ||
		type == SELKNAM_Bowler_Stuner)
	 {
		 return (id == 2  ||
			 id == 5  ||
			 id == 10  ||
			 id == 14 ||
			 id == 18 ||
			 id == 22 ||
			 id == 26 ||
			 id == 30 );
	 }


	 return (id == 2  ||
			 id == 5  ||
			 id == 8  ||
			 id == 11 ||
			 id == 14 ||
			 id == 17 ||
			 id == 20 ||
			 id == 23 );
	 /*
	 return (type == SELKNAM_Warrior_Base && ((id == 2)  ||
								              (id == 5)  ||
											  (id == 8)  ||
											  (id == 11) ||
											  (id == 14) ||
											  (id == 17) ||
											  (id == 20) ||
											  (id == 23) ));
											  */
 }

 CIwFVec3 Utils::CrossProduct(CIwFVec3 A, CIwFVec3 B)
 {
	 return CIwFVec3(A.y * B.z - B.y * A.z, -(A.x * B.z - A.z * B.x), A.x * B.y - A.y * B.x);
 }

 float Utils::Norm(CIwFVec3 A)
 {
	 return (float)sqrt(A.x * A.x + A.y * A.y + A.z * A.z);
 }

 CIwFVec3 Utils::Normalize(CIwFVec3 A)
 {
	 float n = Norm(A);
	 return CIwFVec3(A.x / n, A.y / n, A.z / n);
 }

 CIwFVec3 Utils::VectorMultiplyScalar(CIwFVec3 A, float scalar)
 {
	 return CIwFVec3(A.x * scalar, A.y * scalar, A.z * scalar);
 }

 void Utils::PlayBGM(const char* filename, uint32 repeatCount)
 {
#if !defined DESKTOPPLATFORM
	if(s3eIOSBackgroundAudioIsPlaying() == S3E_TRUE)
		return;
#endif
	s3eAudioPlay(filename, repeatCount);
 }

 bool Utils::PlaySoundFX(CIwSoundSpec *soundFX)
 {
	 if(bIsFxPaused)
		 return false;
	 if(IwGetSoundManager()->GetNumInsts() >= 8)
		 return false;

#if !defined DESKTOPPLATFORM
	 if(s3eIOSBackgroundAudioIsPlaying() == S3E_TRUE)
		 return false;
#endif

	 //check sound exceptions for arrows
	 if(soundFX == FX_Arrow_Shooting_01 || soundFX == FX_Arrow_Shooting_02)
	 {
		 if(sound_arrows_milisecstamp == 0)
			 sound_arrows_milisecstamp = (int)lastSystemTime;

		 sound_arrows_numcurrent++;
		 if(sound_arrows_numcurrent > sound_arrows_numallowed)
			 return false;
	 }
	 //check sound exceptions for punchs
	 if(soundFX == FX_Punch_01 || soundFX == FX_Punch_02 || soundFX == FX_Punch_03 
		 || soundFX == FX_Punch_04 || soundFX == FX_Punch_05)
	 {
		 if(sound_punch_milisecstamp == 0)
			 sound_punch_milisecstamp = (int)lastSystemTime;

		 sound_punch_numcurrent++;
		 if(sound_punch_numcurrent > sound_punch_numallowed)
			 return false;
	 }
	 //check sound exceptions for cannon ship
	 if(soundFX == FX_Cannon_ship)
	 {
		 if(sound_cannon_ship_milisecstamp == 0)
			 sound_cannon_ship_milisecstamp = (int)lastSystemTime;

		 sound_cannon_ship_numcurrent++;
		 if(sound_cannon_ship_numcurrent > sound_cannon_ship_numallowed)
			 return false;
	 }
	 //check sound exceptions for explosion
	 if(soundFX == FX_Explosion)
	 {
		 if(sound_explosion_milisecstamp == 0)
			 sound_explosion_milisecstamp = (int)lastSystemTime;

		 sound_explosion_numcurrent++;
		 if(sound_explosion_numcurrent > sound_explosion_numallowed)
			 return false;
	 }

	 //check sound exceptions for loose a live
	 if(soundFX == FX_LooseALive)
	 {
		 if(sound_loosealive_milisecstamp == 0)
			 sound_loosealive_milisecstamp = (int)lastSystemTime;

		 sound_loosealive_numcurrent++;
		 if(sound_loosealive_numcurrent > sound_loosealive_numallowed)
			 return false;
	 }

	 soundFX->Play();
	 return true;
 }

 void Utils::UpdateRefreshFXSoundExceptions()
 {
	 if(sound_arrows_milisectorenew <= ((int)lastSystemTime - sound_arrows_milisecstamp))
	 {
		 sound_arrows_numcurrent = 0;
		 sound_arrows_milisecstamp = 0;
	 }

	 if(sound_punch_milisectorenew <= ((int)lastSystemTime - sound_punch_milisecstamp))
	 {
		 sound_punch_numcurrent = 0;
		 sound_punch_milisecstamp = 0;
	 }

	 if(sound_cannon_ship_milisectorenew <= ((int)lastSystemTime - sound_cannon_ship_milisecstamp))
	 {
		 sound_cannon_ship_numcurrent = 0;
		 sound_cannon_ship_milisecstamp = 0;
	 }

	 if(sound_explosion_milisectorenew <= ((int)lastSystemTime - sound_explosion_milisecstamp))
	 {
		 sound_explosion_numcurrent = 0;
		 sound_explosion_milisecstamp = 0;
	 }

	 if(sound_loosealive_milisectorenew <= ((int)lastSystemTime - sound_loosealive_milisecstamp))
	 {
		 sound_loosealive_numcurrent = 0;
		 sound_loosealive_milisecstamp = 0;
	 }
 }

 //bool Utils::PlayFXSound(FxSound sound)
 //{
	// if(bIsFxPaused)
	//	 return false;

	// //FXSoundManager->Play((int)sound);

	// if(IwGetSoundManager()->GetNumInsts() >= 5)
	//	 return false;
	// 

	// if(bFxSoundMuted)
	//	 return false;

	// bool bDelay = true;

	// if(playfxsound_timer.GetIsEnd() || playfxsound_timer.GetIsStop())
	// {
	//	 playfxsound_timer.Reset();
	//	 playfxsound_timer.Start();
	// }
	// else
	// {
	//	 bDelay = false;
	// }
	// if     (sound == FX_Sound_ARROWSHOOTING && Utils::IsFxSoundAvailable(FX_Sound_ARROWSHOOTING)) Arrow_Shooting_01->Play();
	// else if(sound == FX_Sound_CLICK         && Utils::IsFxSoundAvailable(FX_Sound_CLICK))         FX_Click_01->Play();
	// else if(sound == FX_Sound_EXPLOSION     && Utils::IsFxSoundAvailable(FX_Sound_EXPLOSION))     FX_Explosion->Play();
	// else if(sound == FX_Sound_KNIFESWORD01  && Utils::IsFxSoundAvailable(FX_Sound_KNIFESWORD01))  FX_Knife_Sword_01->Play();
	// else if(sound == FX_Sound_ROLLOVER02    && bDelay && Utils::IsFxSoundAvailable(FX_Sound_ROLLOVER02))    FX_RollOver_02->Play();
	// else if(sound == FX_Sound_SHOT01        && Utils::IsFxSoundAvailable(FX_Sound_SHOT01)) FX_Shot_01->Play();
	// else if(sound == FX_Sound_STARGAINED02  && Utils::IsFxSoundAvailable(FX_Sound_STARGAINED02)) FX_Star_Gained_02->Play();
	// else if(sound == FX_Sound_METALIMPACT01 && Utils::IsFxSoundAvailable(FX_Sound_METALIMPACT01)) FX_MetalImpact_01->Play();
	// else if(sound == FX_Sound_METALIMPACT02 && Utils::IsFxSoundAvailable(FX_Sound_METALIMPACT02)) FX_MetalImpact_02->Play();
	// else if(sound == FX_Sound_COINS         && Utils::IsFxSoundAvailable(FX_Sound_COINS)) FX_Coins->Play();
	// else if(sound == FX_Sound_HEAL          && Utils::IsFxSoundAvailable(FX_Sound_HEAL)) FX_Heal->Play();
	// else if(sound == FX_Sound_BUBBLES       && Utils::IsFxSoundAvailable(FX_Sound_BUBBLES)) FX_Bubbles->Play();
	// return true;
 //}

 void Utils::StopAllFXSounds()
 {
	 IwGetSoundManager()->StopAll();
	 bIsFxPaused = true;
 }

 void Utils::ResumeAllFXSounds()
 {
	 bIsFxPaused = false;
 }

 //bool Utils::IsFxSoundAvailable(FxSound sound)
 //{
	// int idx = (int)sound;
	// //IwTrace(FXSOUNDTRACE, ("asdasd: %d %d", fx_sound_limit[sound][1], idx));
	// if(fx_sound_limit[sound][0] > (fx_sound_limit[sound][1]++))
	// {
	//	 //IwTrace(FXSOUNDTRACE, ("PLAY!"));
	//	 return true;
	// }
	// return false;
 //}

 void Utils::UtilsUpdate()
 {
	 //playfxsound_timer.Update();

	 Utils::UpdateRefreshFXSoundExceptions();
	 Utils::UpdateExplotions();
	 Utils::UpdateCannonExplotions();

	 if(bCameraMovingByUtilMethod)
	 {
		 cameraMovingByUtilMethodTimer.Update();
		 if(cameraMovingByUtilMethodTimer.GetIsEnd())
		 {
			 cameraMovementVar += tIncrement;
			 if(cameraMovementVar > 1)
			 {
				 bCameraMovingByUtilMethod = false;
			 }
			 else
			 {
				 CIwFVec2 currVector = CIwFVec2((1 - cameraMovementVar) * aVector.x + cameraMovementVar * bVector.x, (1 - cameraMovementVar) * aVector.y + cameraMovementVar * bVector.y);
				 xWindowOffset = (int)(xInitialOffset + currVector.x);
				 yWindowOffset = (int)(yInitialOffset + currVector.y);

				 bool bCorrection = false;
				 if(xWindowOffset < xMinOffset) { xWindowOffset = xMinOffset; bCorrection = true; }
				 if(xWindowOffset > xMaxOffset) { xWindowOffset = xMaxOffset; bCorrection = true; }
				 if(yWindowOffset < yMinOffset) { yWindowOffset = yMinOffset; bCorrection = true; }
				 if(yWindowOffset > yMaxOffset) { yWindowOffset = yMaxOffset; bCorrection = true; }
				 
				 if(bCorrection)
				 {
					 bCameraMovingByUtilMethod = false;
				 }
				 else
				 {
					 cameraMovingByUtilMethodTimer.Reset();
					 cameraMovingByUtilMethodTimer.Start();
				 }
			 }
		 }
	 }

	 Utils::ShakeCameraUpdate();
 }

bool Utils::IsCameraMoving()
{
	return bCameraMovingByUtilMethod;
}

void Utils::InitPannel(PannelType type, string center, string corner, string border)
{
	int typeToInt = (int)type;
	pannelCenterRD[typeToInt]            = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(center));
	pannelCenterAN[typeToInt]            = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(center));
	pannelCornerUpLeftRD[typeToInt]      = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(corner));
	pannelCornerUpLeftAN[typeToInt]      = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(corner));
	pannelCornerDownLeftRD[typeToInt]    = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(corner)), MIRROR_HORIZONTAL);
	pannelCornerDownLeftAN[typeToInt]    = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(corner));
	pannelCornerUpRightRD[typeToInt]     = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(corner)), MIRROR_VERTICAL);
	pannelCornerUpRightAN[typeToInt]     = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(corner));
	pannelCornerDownRightRD[typeToInt]   = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(corner)), MIRROR_DIAGONAL);
	pannelCornerDownRightAN[typeToInt]   = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(corner));
	pannelBorderUpRD[typeToInt]          = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(border));
	pannelBorderUpAN[typeToInt]          = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(border));
	pannelBorderDownRD[typeToInt]        = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(border)), MIRROR_HORIZONTAL);
	pannelBorderDownAN[typeToInt]        = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(border));
	pannelBorderLeftRD[typeToInt]        = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(border)), MIRROR_HORIZONTAL);
	pannelBorderLeftAN[typeToInt]        = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(border));
	pannelBorderRightRD[typeToInt]       = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(border));
	pannelBorderRightAN[typeToInt]       = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(border));
}

void Utils::InitBar(BarType type, string leftBorder, string center, string rightBorder)
{
	int typeToInt = (int)type;

	barLeftCornerRD[typeToInt]  = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(leftBorder));
	barLeftCornerAN[typeToInt]  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(leftBorder));
	barCenterRD[typeToInt]      = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(center));
	barCenterAN[typeToInt]      = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(center));
	barRightCornerRD[typeToInt] = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed(rightBorder));
	barRightCornerAN[typeToInt] = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed(rightBorder));
}

void Utils::UtilsInit()
{
	 for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	 {
		 explotionIndex[i] = -1;
		 explotionTimer[i].SetTimeLimit(50);
	 }

	 for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	 {
		 cannonexplotionIndex[i] = -1;
		 cannonexplotionTimer[i].SetTimeLimit(75);
	 }

	 //playfxsound_timer.SetTimeLimit(75);

	 //RenderLabelTitle init
	 label_title_center_renderdata = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_title_center"));
	 label_title_left_renderdata   = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_title_left"));
	 label_title_right_renderdata  = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_title_right"));
	 label_title_center_atlasname  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("label_title_center"));
	 label_title_left_atlasname    = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("label_title_left"));
	 label_title_right_atlasname   = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("label_title_right"));

	 Utils::InitPannel(PannelType_A, "pannel_a_center", "pannel_a_corner", "pannel_a_border");
	 Utils::InitPannel(PannelType_B, "pannel_b_center", "pannel_b_corner", "pannel_b_border");
	 Utils::InitPannel(PannelType_C, "pannel_c_center", "pannel_c_corner", "pannel_c_border");
	 Utils::InitPannel(PannelType_D, "pannel_d_center", "pannel_d_corner", "pannel_d_border");
	 Utils::InitPannel(PannelType_E, "pannel_e_center", "pannel_e_corner", "pannel_e_border");
	 Utils::InitPannel(PannelType_F, "pannel_f_center", "pannel_f_corner", "pannel_f_border");
	 Utils::InitPannel(PannelType_G, "pannel_g_center", "pannel_g_corner", "pannel_g_border");
	 Utils::InitPannel(PannelType_H, "pannel_h_center", "pannel_h_corner", "pannel_h_border");

	 Utils::InitBar(BarType_A, "bar_a1", "bar_a2", "bar_a3");
	 Utils::InitBar(BarType_B, "bar_b1", "bar_b2", "bar_b3");

	 util_explotion_textureData = TextureHelper::GetTextureNamed("fx_explotion");
	 util_explotion_atlasname   = TextureHelper::GetAtlasName(util_explotion_textureData);

	 util_cannonexplotion_textureData = TextureHelper::GetTextureNamed("fire_cannon");
	 util_cannonexplotion_atlasname   = TextureHelper::GetAtlasName(util_cannonexplotion_textureData);

	 //Tube
	 pannel_tube_renderdata = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_tube"));
	 pannel_tube_atlasname  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_tube"));

	 bCameraMovingByUtilMethod = false;
	 cameraMovingByUtilMethodTimer.SetTimeLimit(30);
	 tIncrement = 0;

	 cursorMapTD = TextureHelper::GetTextureNamed("cursor_map_big");;
	 cursorMapAtlasName = TextureHelper::GetAtlasName(cursorMapTD);;

	 yellowCircleTD = TextureHelper::GetTextureNamed("cursor_movement");
	 yellowCircleTD.textureRotated = true;
	 yellowCircleAtlasName = TextureHelper::GetAtlasName(yellowCircleTD);

	 Utils::ShakeCameraInit();
 }

 void Utils::RenderVerticalTube(int xPos, int yPos, int large)
 {
	 if(pannel_tube_renderdata.bRotate)
	 {
		 pannel_tube_renderdata.width = large;
	 }
	 else
	 {
		 pannel_tube_renderdata.height = large;
	 }

	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_tube_atlasname), CIwSVec2(xPos, yPos), pannel_tube_renderdata);
 }

 void Utils::MoveCamera(int t, CIwFVec2 a, CIwFVec2 b)
 {
	 bCameraMovingByUtilMethod = true;
	 cameraMovingByUtilMethodTimer.Reset();
	 cameraMovingByUtilMethodTimer.Start();
	 tIncrement = 1 / (t / 10.0f);
	 aVector = a;
	 bVector = b;
	 cameraMovementVar = 0;
	 xInitialOffset = xWindowOffset;
	 yInitialOffset = yWindowOffset;
 }

 string Utils::TransformToMapFilename(int currentMap, bool extraMode)
 {
	 string out;

	 if(!extraMode)
		out = "maps/";
	 else
		out = "extramaps/extra_";

	 if(currentMap < 10)
	 {
		 out += "map_00";
	 }
	 else if(currentMap < 100)
	 {
		 out += "map_0";
	 }

	 char number[5];
	 sprintf(number, "%d", currentMap);

	 out += number;
	 out += ".sdm";

	 return out;
 }

string Utils::GetUnformattedLoadedGroup(string myLoadedGroup)
 {
	string myWordToReplace = ".group";
	string myWordToFind = "/";
	bool bSeparatorFound = false;

	size_t found = myLoadedGroup.find(myWordToFind);
	if (found!=std::string::npos)
	{
		bSeparatorFound = true;
	}

	myLoadedGroup.replace(myLoadedGroup.find(myWordToReplace), myWordToReplace.length(), "");

	if(bSeparatorFound)
	{
		myLoadedGroup.erase(0, found + 1);
	}

	//IwTrace(NEWLOADING, ("GetUnformattedLoadedGroup: myNewWord: %s, bSeparatorFound %i", myNewWord, bSeparatorFound));

	return myLoadedGroup;
 }

 bool Utils::CheckIfMouseIsEnabled()
 {
	 return bMouseActive;
 }

 //SCREEN FIX
 void Utils::GetIsoSpaceCoord(float XPos, float YPos, float XOffset, float YOffset, float *newIsoXPos, float *newIsoYPos)
 {
	 //http://gamedev.stackexchange.com/questions/34787/how-to-convert-mouse-coordinates-to-isometric-indexes

	 float auxFormula1 = XPos * isoFormulaHValue;
	 float auxFormula2 = YPos * isoFormulaWValue;
	 float auxFormula3 = (XOffset * isoFormulaHValue);
	 float auxFormula4 = (YOffset * isoFormulaWValue);

	 float myIsoXValue = (auxFormula1 + auxFormula2 - auxFormula3 - auxFormula4);
	 myIsoXValue = isoFormulaInvAValue * myIsoXValue;
	 float myIsoYValue = (-auxFormula1 + auxFormula2 + auxFormula3 - auxFormula4);
	 myIsoYValue = isoFormulaInvAValue * myIsoYValue;

	 *newIsoXPos = myIsoXValue;
	 *newIsoYPos = myIsoYValue;

	 //IwTrace(ISOPOSITION, ("GetIsoSpaceCoord() called, XPos %f, YPos %f,  myIsoXValue %f, myIsoYValue %f ", XPos, YPos, myIsoXValue, myIsoYValue));
 }

 void Utils::RenderLabelTitle(int len, int xPos, int yPos, float scale)
 {
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_title_left_atlasname), CIwSVec2(xPos, yPos), label_title_left_renderdata);
	 
	 for(int i = 0; i < len; i++)
	 {
		 int xTmp = xPos + TextureHelper::GetDimension(label_title_left_renderdata).x + TextureHelper::GetDimension(label_title_center_renderdata).x * i;
		 int yTmp = yPos;
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_title_center_atlasname), CIwSVec2(xTmp, yTmp), label_title_center_renderdata);
	 }

	 int xTmp = xPos + TextureHelper::GetDimension(label_title_left_renderdata).x + TextureHelper::GetDimension(label_title_center_renderdata).x * len;
	 int yTmp = yPos;
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_title_right_atlasname), CIwSVec2(xTmp, yTmp), label_title_right_renderdata);
 }

 void Utils::RenderPannel(PannelType type, CIwRect position, int alpha)
 {
	 Utils::RenderPannel(type, position.w, position.h, position.x, position.y, alpha);
 }

 void Utils::RenderPannel(PannelType type, int width, int height, int xPos, int yPos, int alpha)
 {
	 int typeToInt = (int)type;

	 int w = TextureHelper::GetDimension(pannelCenterRD[typeToInt]).x;
	 int h = TextureHelper::GetDimension(pannelCenterRD[typeToInt]).y;
	 int xOffset = TextureHelper::GetDimension(pannelCornerUpLeftRD[typeToInt]).x;
	 int yOffset = TextureHelper::GetDimension(pannelCornerUpLeftRD[typeToInt]).y;

	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelCornerUpLeftAN[typeToInt]), pannelCornerUpLeftRD[typeToInt], CIwSVec2(xPos, yPos), 255, 255, 255, alpha);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelCornerDownLeftAN[typeToInt]), pannelCornerDownLeftRD[typeToInt], CIwSVec2(xPos, yPos + yOffset + h * height), 255, 255, 255, alpha);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelCornerUpRightAN[typeToInt]), pannelCornerUpRightRD[typeToInt], CIwSVec2(xPos + xOffset + w * width, yPos), 255, 255, 255, alpha);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelCornerDownRightAN[typeToInt]), pannelCornerDownRightRD[typeToInt], CIwSVec2(xPos + xOffset + w * width, yPos + yOffset + w * height), 255, 255, 255, alpha);

	 int wBorder = TextureHelper::GetDimension(pannelBorderUpRD[typeToInt]).x;
	 int hBorder = TextureHelper::GetDimension(pannelBorderUpRD[typeToInt]).y;
	 for(int i = 0; i < width; i++)
	 {
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelBorderUpAN[typeToInt]), pannelBorderUpRD[typeToInt], CIwSVec2(xPos + xOffset + wBorder * i, yPos), 255, 255, 255, alpha);
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelBorderDownAN[typeToInt]), pannelBorderDownRD[typeToInt], CIwSVec2(xPos + xOffset + wBorder * i, yPos + yOffset + w * height), 255, 255, 255, alpha);
	 }

	 for(int i = 0; i < height; i++)
	 {
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelBorderRightAN[typeToInt]), CIwSVec2(xPos + xOffset + w * width, yPos + yOffset + hBorder * i), pannelBorderRightRD[typeToInt], ROTATE_270, alpha);
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelBorderLeftAN[typeToInt]), CIwSVec2(xPos, yPos + yOffset + hBorder * i), pannelBorderLeftRD[typeToInt], ROTATE_270, alpha);
	 }

	 for(int i = 0; i < height; i++)
	 {
		 for(int j = 0; j < width; j++)
		 {
			 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannelCenterAN[typeToInt]), pannelCenterRD[typeToInt], CIwSVec2(xPos + w * j + xOffset, yPos + h * i + yOffset), 255, 255, 255, alpha);
		 }
	 }
 }

 void Utils::RenderBar(BarType type, int numCols, int xPos, int yPos)
 {
	 int typeToInt = (int)type;

	 int w = TextureHelper::GetDimension(barCenterRD[typeToInt]).x;
	 int xOffset = TextureHelper::GetDimension(barLeftCornerRD[typeToInt]).x;

	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(barLeftCornerAN[typeToInt]), CIwSVec2(xPos, yPos), barLeftCornerRD[typeToInt]);

	 for(int i = 0; i < numCols; i++)
	 {
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(barCenterAN[typeToInt]), CIwSVec2(xPos + xOffset + w * i, yPos), barCenterRD[typeToInt]);
	 }

	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(barRightCornerAN[typeToInt]), CIwSVec2(xPos + xOffset + numCols * w, yPos), barRightCornerRD[typeToInt]);
 }

 void Utils::RenderPannelA(int width, int height, int xPos, int yPos)
 {
	Utils::RenderPannel(PannelType_A, width, height, xPos, yPos);
 }

 void Utils::RenderPannelB(int width, int height, int xPos, int yPos)
 {
	Utils::RenderPannel(PannelType_B, width, height, xPos, yPos);
 }

 void Utils::SwapFloat(float *float1, float* float2)
 {
	 float fTemp = *float1;
	 *float1 = *float2;
	 *float2 = fTemp;
 }

 string Utils::GetClockFormat(int s)
 {
	 int seconds = s % 60;
	 int minutes = (s / 60) % 60;
	 int hours = (s / 3600);

	 char toRet[100];

	 if(hours + minutes == 0)
	 {
		 sprintf(toRet, "%d", seconds);
	 }
	 else if(hours > 0)
	 {
		 if(minutes < 10)
		 {
			 if(seconds < 10)
			 {
				 sprintf(toRet, "%d:0%d:0%d", hours, minutes, seconds);
			 }
			 else
			 {
				 sprintf(toRet, "%d:0%d:%d", hours, minutes, seconds);
			 }
		 }
		 else
		 {
			 if(seconds < 10)
			 {
				 sprintf(toRet, "%d:%d:0%d", hours, minutes, seconds);
			 }
			 else
			 {
				 sprintf(toRet, "%d:%d:%d", hours, minutes, seconds);
			 }
		 }
	 }
	 else
	 {
		 if(seconds < 10)
		 {
			 sprintf(toRet, "%d:0%d", minutes, seconds);
		 }
		 else
		 {
			 sprintf(toRet, "%d:%d", minutes, seconds);
		 }
	 }

	 string tmp = toRet;
	 return tmp;
 }




 string Utils::GetClockFormatMS(int s)
 {
	 char secondsString[5];
	 char minutesString[5];
	 char milisecString[5];

	 //return "HOLA";
	 int milisecs = s % 1000;
	

	 if(milisecs >= 100) sprintf(milisecString, "%d", milisecs);
	 else if(milisecs >= 10) sprintf(milisecString, "0%d", milisecs);
	 else sprintf(milisecString, "00%d", milisecs);

	 s /= 1000;

	 int seconds = s % 60;
	 int minutes = (s / 60) % 60;


	 if(seconds >= 10) sprintf(secondsString, "%d", seconds);
	 else sprintf(secondsString, "0%d", seconds);

	 if(minutes >= 10) sprintf(minutesString, "%d", minutes);
	 else sprintf(minutesString, "0%d", minutes);
	 
	 char toRet[20];

	 sprintf(toRet, "%s:%s.%s", minutesString, secondsString, milisecString);
	 //sprintf(toRet, "%s:%s.%s", "1", "2", "3");

	 //string tmp = toRet;
	 
	 //free(milisecString);
	 //free(secondsString);
	 //free(minutesString);

	 return toRet;
 }

 bool Utils::MouseOn(int x, int y, int xPos, int yPos, int _width, int _height) 
 {
	 return (x >= xPos && 
		 x <= (xPos + _width) && 
		 y >= yPos && 
		 y <= (yPos + (int)(_height)));
 }

#define MAX_RANDPOINTS 10

CIwFVec2 randPoints[MAX_RANDPOINTS];
CIwFVec2 curPoint;
int nextPointIndex;
int numRandPoints;
int shakeTime;
int numIterations;
bool bCameraShaking;
Timer shakeTimer;

void Utils::ShakeCamera(int _radius, int _numRandPoints, int _duration, int _intensity)
{
	//IwTrace(SHAKECAMERA, ("ShakeCamera method called."));

	srand(time(NULL));

	CIwFVec2 cameraPosition = CIwFVec2((float)xWindowOffset, (float)yWindowOffset);

	//IwTrace(SHAKECAMERA, ("Initial Camera Position: %f, %f.", cameraPosition.x, cameraPosition.y));

	numRandPoints = 0;
	
	float upperLimitX = cameraPosition.x + _radius;
	float lowerLimitX = cameraPosition.x - _radius;
	int randFactorX = (int)(upperLimitX - lowerLimitX);

	float upperLimitY = cameraPosition.y + _radius;
	float lowerLimitY = cameraPosition.y - _radius;
	int randFactorY = (int)(upperLimitY - lowerLimitY);

	//IwTrace(SHAKECAMERA, ("Limit-X: (%f, %f).", lowerLimitX, upperLimitX));

	int xFixed = rand() % (int)(_radius * 0.9f + 1);

	for(int i = 0; i < _numRandPoints; i++)
	{
		float _x = 0.0f;
		float _y = 0.0f;

		if(i % 2 == 0)
		{
			//_x += rand() % (int)(_radius * 0.9f + 1);
			_x += xFixed;
			_y -= rand() % (int)(_radius+ 1);
		}
		else
		{
			//_x -= rand() % (int)(_radius * 0.9f + 1);
			_x -= xFixed;
			_y += rand() % (int)(_radius + 1);
		}

		//float _y = cameraPosition.y;// + (rand() % (randFactorY + 1)) + lowerLimitY;
		randPoints[numRandPoints++] = CIwFVec2(_x, _y);

		//IwTrace(SHAKECAMERA, ("Random Point: (%f, %f).", _x, _y));
	}

	numIterations = _numRandPoints * _intensity;
	shakeTime = (int)(_duration / (float)numIterations);
	bCameraShaking = true;
	curPoint = cameraPosition;
	nextPointIndex = 0;

	//IwTrace(SHAKECAMERA, ("Shake Time: (%d).", shakeTime));
}

void Utils::ShakeCameraUpdate()
{
	if(bCameraShaking)
	{
		if(numIterations > 0)
		{
			if(!Utils::IsCameraMoving())
			{
				CIwFVec2 nextPoint = randPoints[nextPointIndex++ % numRandPoints];
				Utils::MoveCamera(shakeTime, CIwFVec2(0, 0), nextPoint);
				curPoint = nextPoint;
				numIterations--;
			}
		}
		else
		{
			bCameraShaking = false;
		}
	}
}

void Utils::ShakeCameraInit()
{
	bCameraShaking = false;
	shakeTimer.Stop();
}

bool Utils::IsCameraShaking()
{
	return bCameraShaking;
}

void Utils::DrawCannonExplotion(float x, float y, float z)
{
	for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	{
		if(cannonexplotionIndex[i] == -1)
		{
			cannonexplotionInfo[i][0] = x;
			cannonexplotionInfo[i][1] = y;
			cannonexplotionInfo[i][2] = z;
			cannonexplotionIndex[i] = 0;

			cannonexplotionTimer[i].Reset();
			cannonexplotionTimer[i].Start();

			//Utils::PlayFXSound(FX_Sound_EXPLOSION);

			return;
		}
	}
}

void Utils::UpdateCannonExplotions()
{
	for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	{
		cannonexplotionTimer[i].Update();
	}
}

void Utils::RenderCannonExplotions()
{
	for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	{
		if(cannonexplotionIndex[i] != -1)
		{
			int _idx = (int)cannonexplotionIndex[i] % 3;
			int _idy = (int)cannonexplotionIndex[i] / 3;

			float offset_x = -32.0f * globalScale;
			float offset_y = -40.0f * globalScale;
			float uvMinX = 0.33f * _idx;
			float uvMaxX = 0.33f * (_idx + 1);
			float uvMinY = 0.25f * _idy;
			float uvMaxY = 0.25f * (_idy + 1);

			uvMinX = (util_cannonexplotion_textureData.textureXPos + uvMinX * util_cannonexplotion_textureData.textureWidth) / (float)util_cannonexplotion_textureData.atlasWidth;
			uvMaxX = (util_cannonexplotion_textureData.textureXPos + uvMaxX * util_cannonexplotion_textureData.textureWidth) / (float)util_cannonexplotion_textureData.atlasWidth;
			uvMinY = (util_cannonexplotion_textureData.textureYPos + uvMinY * util_cannonexplotion_textureData.textureHeight) / (float)util_cannonexplotion_textureData.atlasHeight;
			uvMaxY = (util_cannonexplotion_textureData.textureYPos + uvMaxY * util_cannonexplotion_textureData.textureHeight) / (float)util_cannonexplotion_textureData.atlasHeight;

			Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(util_cannonexplotion_atlasname));
			Utils::DrawQuadBatch(64.0f * globalScale * 1.0f, 48.0f * globalScale * 1.0f, 
				cannonexplotionInfo[i][0], cannonexplotionInfo[i][1], 10,//explotionInfo[i][0], explotionInfo[i][1], explotionInfo[i][2],
				uvMinX * 1.01f, 
				uvMaxX, 
				uvMinY,
				uvMaxY,
				offset_x, 
				offset_y);
			Utils::RenderQuadBatch();

			if(cannonexplotionTimer[i].GetIsEnd())
			{
				cannonexplotionIndex[i]++;
				cannonexplotionTimer[i].Reset();
				cannonexplotionTimer[i].Start();
			}

			if(cannonexplotionIndex[i] > 11)
			{
				cannonexplotionIndex[i] = -1;
				cannonexplotionTimer[i].Stop();
			}

		}
	}
}

void Utils::DrawExplotion(float x, float y, float z)
{
	for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	{
		if(explotionIndex[i] == -1)
		{
			explotionInfo[i][0] = x;
			explotionInfo[i][1] = y;
			explotionInfo[i][2] = z;
			explotionIndex[i] = 0;

			explotionTimer[i].Reset();
			explotionTimer[i].Start();

			//Utils::PlayFXSound(FX_Sound_EXPLOSION);
			Utils::PlaySoundFX(FX_Explosion);

			return;
		}
	}
}

void Utils::UpdateExplotions()
{
	for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	{
		explotionTimer[i].Update();
	}
}

void Utils::RenderExplotions()
{
	for(int i = 0; i < MAX_UTIL_EXPLOTIONS; i++)
	{
		if(explotionIndex[i] != -1)
		{
			int _idx = (int)explotionIndex[i] % 5;
			int _idy = (int)explotionIndex[i] / 5;

			float offset_x = -32.0f * globalScale;
			float offset_y = -40.0f * globalScale;
			float uvMinX = 0.2f * _idx;
			float uvMaxX = 0.2f * (_idx + 1);
			float uvMinY = 0.1667f * _idy;
			float uvMaxY = 0.1667f * (_idy + 1);

			uvMinX = (util_explotion_textureData.textureXPos + uvMinX * 320.0f) / (float)util_explotion_textureData.atlasWidth;
			uvMaxX = (util_explotion_textureData.textureXPos + uvMaxX * 320.0f) / (float)util_explotion_textureData.atlasWidth;
			uvMinY = (util_explotion_textureData.textureYPos + uvMinY * 480.0f) / (float)util_explotion_textureData.atlasHeight;
			uvMaxY = (util_explotion_textureData.textureYPos + uvMaxY * 480.0f) / (float)util_explotion_textureData.atlasHeight;

			Utils::InitQuadDrawing(TextureHelper::GetCIwTexture(util_explotion_atlasname));
			Utils::DrawQuadBatch(64.0f * globalScale * 1.4f, 80.0f * globalScale * 1.4f, 
				explotionInfo[i][0], explotionInfo[i][1], 10,//explotionInfo[i][0], explotionInfo[i][1], explotionInfo[i][2],
				uvMinX, 
				uvMaxX, 
				uvMinY,
				uvMaxY,
				offset_x, 
				offset_y);
			Utils::RenderQuadBatch();

			if(explotionTimer[i].GetIsEnd())
			{
				explotionIndex[i]++;
				explotionTimer[i].Reset();
				explotionTimer[i].Start();
			}

			if(explotionIndex[i] > 23)
			{
				explotionIndex[i] = -1;
				explotionTimer[i].Stop();
			}

		}
	}
}

uint32 Utils::colorToDecimal(int r, int g, int b, int alpha)
{
	int i = 1;
	uint32 numToRet = 0;

	while(b >= 16)
	{
		numToRet += (b % 16) * i;
		i *= 16;
		b /= 16;
	}
	numToRet += (b % 16) * i;
	i *= 16;

	while(g >= 16)
	{
		numToRet += (g % 16) * i;
		i *= 16;
		g /= 16;
	}
	numToRet += (g % 16) * i;
	i *= 16;

	while(r >= 16)
	{
		numToRet += (r % 16) * i;
		i *= 16;
		r /= 16;
	}
	numToRet += (r % 16) * i;
	i *= 16;

	while(alpha >= 16)
	{
		numToRet += (alpha % 16) * i;
		i *= 16;
		alpha /= 16;
	}
	numToRet += (alpha % 16) * i;

	return numToRet;
}

CIwSVec2 Utils::GetScaledSize(CIwSVec2 originalSize)
{
	CIwSVec2 myNewSize;
	float myWidth = 0;
	float myHeigh = 0;
	float myCurrentGameHeight = (float)currentGameHeight;
	float mySrcGameHeight = (float)srcGameHeight;
	float changePct = myCurrentGameHeight / mySrcGameHeight;

	myWidth = originalSize.x;
	myWidth = myWidth * changePct;
	myHeigh = originalSize.y;
	myHeigh = myHeigh * changePct;

	myNewSize.x = (int16)myWidth;
	myNewSize.y = (int16)myHeigh;

	return myNewSize;
}

float Utils::GetPctChange()
{
	float myCurrentGameHeight = (float)currentGameHeight;
	float mySrcGameHeight = (float)srcGameHeight;
	float changePct = myCurrentGameHeight / mySrcGameHeight;

	return changePct;
}

int Utils::GetScaledInt(int myInt)
{
	float tmpInt = (float)myInt;
	float myCurrentGameHeight = (float)currentGameHeight;
	float mySrcGameHeight = (float)srcGameHeight;
	float changePct = myCurrentGameHeight / mySrcGameHeight;
	tmpInt = tmpInt * changePct;

	return (int)tmpInt;
}

int Utils::GetScaledInt(int myInt, int baseHeight)
{
	float tmpInt = (float)myInt;
	float myCurrentGameHeight = (float)currentGameHeight;
	float myBaseHeight = (float)baseHeight;
	float changePct = myCurrentGameHeight / myBaseHeight;
	tmpInt = tmpInt * changePct;

	return (int)tmpInt;
}

int Utils::GetFixedScaledInt(int myInt)
{
	float tmpInt = (float)myInt;
	float myCurrentGameHeight = (float)currentGameHeight;
	float mySrcGameHeight = (float)srcGameHeight;
	float changePct = myCurrentGameHeight / mySrcGameHeight;
	float screenDiff = ((float)currentGameWidth - (changePct * (float)srcGameWidth)) * 0.5f;

	tmpInt = tmpInt * changePct + screenDiff;

	return (int)tmpInt;
}

int Utils::GetManhattanDistance(IsoTerrain* tile1, IsoTerrain* tile2)
{
	if(tile1 == NULL || tile2 == NULL)
	{
		return INT_MAX;
	}

	return abs(tile1->getXNumTile() - tile2->getXNumTile()) + abs(tile1->getYNumTile() - tile2->getYNumTile());
}

CIwSVec2 Utils::GetAdjustedCenter2DPointForGUI(CIwSVec2 currCenter, int w, int h)
{
	int halfSizeX = w / 2;
	int halfSizeY = h / 2;

	CIwSVec2 vectToRet;
	//vectToRet.x = (int)(currCenter.x * 1.025f);
	//vectToRet.y = (int)(currCenter.y * 1.025f);
	vectToRet.x = currCenter.x;
	vectToRet.y = currCenter.y;

	if(currCenter.x + halfSizeX >= (dynamicGUIWrapper[0].w + dynamicGUIWrapper[0].x))
	{
		vectToRet.x = Utils::GetScreenWidth() - halfSizeX;
	}
	else if(currCenter.x - halfSizeX < dynamicGUIWrapper[0].x)
	{
		vectToRet.x = halfSizeX + dynamicGUIWrapper[0].x;
	}

	if(currCenter.y + halfSizeY >= (dynamicGUIWrapper[0].h + dynamicGUIWrapper[0].y))
	{
		vectToRet.y = (dynamicGUIWrapper[0].h + dynamicGUIWrapper[0].y) - halfSizeY;
	}
	else if(currCenter.y - halfSizeY < dynamicGUIWrapper[0].y)
	{
		vectToRet.y = halfSizeY + dynamicGUIWrapper[0].y;
	}

	return vectToRet;
}

int Utils::GetScreenWidth()
{
	return currentGameWidth;
}

int Utils::GetScreenHeight()
{
	return currentGameHeight;
}

int Utils::MaxInt(int a, int b)
{
	if(a > b)
	{
		return a;
	}
	
	return b;
}

void Utils::FreezeCamera()
{
	bFreezeCamera = true;
}

void Utils::UnfreezeCamera()
{
	bFreezeCamera = false;
}

float Utils::Lerp(int initial, int final, float t)
{
	return t * final + (1 - t) * initial;
}

void Utils::RenderYellowCircleCursorMap(IsoTerrain* terrain, int id, int alpha)
{
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(TextureHelper::GetCIwTexture(yellowCircleAtlasName));
	IwGxSetMaterial(pMat);

	float offsetX = 24.0f;// * globalScale;
	float offsetY = 16.0f;// * globalScale;
	float _height = 40.0f;// * globalScale;
	float _width = 48.0f;// * globalScale;

	static CIwFVec3 s_Quad[4];
	if(!yellowCircleTD.textureRotated)
	{
		s_Quad[0].x = terrain->getPosX() - offsetX;
		s_Quad[0].y = terrain->getPosY() - offsetY;
		s_Quad[0].z = terrain->getPosZ();

		s_Quad[1].x = terrain->getPosX() - offsetX;
		s_Quad[1].y = terrain->getPosY() + _height - offsetY;
		s_Quad[1].z = terrain->getPosZ();

		s_Quad[2].x = terrain->getPosX() + _width - offsetX;
		s_Quad[2].y = terrain->getPosY() + _height - offsetY;
		s_Quad[2].z = terrain->getPosZ();

		s_Quad[3].x = terrain->getPosX() + _width - offsetX;
		s_Quad[3].y = terrain->getPosY() - offsetY;
		s_Quad[3].z = terrain->getPosZ();
	}
	else
	{
		s_Quad[3].x = terrain->getPosX() - offsetX;
		s_Quad[3].y = terrain->getPosY() - offsetY;
		s_Quad[3].z = terrain->getPosZ();

		s_Quad[0].x = terrain->getPosX() - offsetX;
		s_Quad[0].y = terrain->getPosY() + _height - offsetY;
		s_Quad[0].z = terrain->getPosZ();

		s_Quad[1].x = terrain->getPosX() + _width - offsetX;
		s_Quad[1].y = terrain->getPosY() + _height - offsetY;
		s_Quad[1].z = terrain->getPosZ();

		s_Quad[2].x = terrain->getPosX() + _width - offsetX;
		s_Quad[2].y = terrain->getPosY() - offsetY;
		s_Quad[2].z = terrain->getPosZ();
	}

	IwGxSetVertStreamModelSpace(s_Quad, 4);

	alpha = 255;

	static CIwColour tmpColor[4];
	tmpColor[0].Set(255, 255, 255, alpha);
	tmpColor[1].Set(255, 255, 255, alpha);
	tmpColor[2].Set(255, 255, 255, alpha);
	tmpColor[3].Set(255, 255, 255, alpha);
	IwGxSetColStream(tmpColor, 4);

	float uvX = 0.2f;
	float uvY = 0.2f;

	int idX = 4 - id;
	int idY = 4;

	static CIwFVec2 s_UVs[4];
	float uvMinX = ((uvX * idX) * yellowCircleTD.textureHeight + yellowCircleTD.textureYPos) / (float)yellowCircleTD.atlasHeight;
	float uvMinY = ((uvY * idY) * yellowCircleTD.textureWidth  + yellowCircleTD.textureXPos) / (float)yellowCircleTD.atlasWidth;
	float uvMaxX = ((uvX * (idX + 1)) * yellowCircleTD.textureHeight + yellowCircleTD.textureYPos) / (float)yellowCircleTD.atlasHeight;
	float uvMaxY = ((uvY * (idY + 1)) * yellowCircleTD.textureWidth  + yellowCircleTD.textureXPos) / (float)yellowCircleTD.atlasWidth;

	s_UVs[0] = CIwFVec2(uvMinY, uvMinX);
	s_UVs[1] = CIwFVec2(uvMinY, uvMaxX);
	s_UVs[2] = CIwFVec2(uvMaxY, uvMaxX);
	s_UVs[3] = CIwFVec2(uvMaxY, uvMinX);

	IwGxSetUVStream(s_UVs);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
}

void Utils::RenderBlueCursorMap(IsoTerrain* terrain, int id, int alpha)
{	
	CIwMaterial* pMat = IW_GX_ALLOC_MATERIAL();
	pMat->SetAlphaMode(CIwMaterial::ALPHA_BLEND);
	pMat->SetTexture(TextureHelper::GetCIwTexture(cursorMapAtlasName));
	IwGxSetMaterial(pMat);

	float offsetX = 32.0f;
	float offsetY = 16.0f;
	float _h = 32.0f;
	float _width = 64.0f;
	float _height = 32.0f;

	float xPos = terrain->getPosX();
	float yPos = terrain->getPosY();
	float zPos = terrain->getPosZ() - 0.005f;

	static CIwFVec3 s_Quad[4];
	if(!cursorMapTD.textureRotated)
	{
		s_Quad[0].x = xPos - offsetX * globalScale;
		s_Quad[0].y = yPos - offsetY * globalScale;
		s_Quad[0].z = zPos;

		s_Quad[1].x = xPos - offsetX * globalScale;
		s_Quad[1].y = yPos + (_h - offsetY) * globalScale;
		s_Quad[1].z = zPos;

		s_Quad[2].x = xPos + (_width - offsetX) * globalScale;
		s_Quad[2].y = yPos + (_h - offsetY) * globalScale;
		s_Quad[2].z = zPos;

		s_Quad[3].x = xPos + (_width - offsetX) * globalScale;
		s_Quad[3].y = yPos - offsetY * globalScale;
		s_Quad[3].z = zPos;
	}
	else
	{
		s_Quad[3].x = xPos - offsetX * globalScale;
		s_Quad[3].y = yPos - offsetY * globalScale;
		s_Quad[3].z = zPos;

		s_Quad[0].x = xPos - offsetX * globalScale;
		s_Quad[0].y = yPos + (_h - offsetY) * globalScale;
		s_Quad[0].z = zPos;

		s_Quad[1].x = xPos + (_width - offsetX) * globalScale;
		s_Quad[1].y = yPos + (_h - offsetY) * globalScale;
		s_Quad[1].z = zPos;

		s_Quad[2].x = xPos + (_width - offsetX) * globalScale;
		s_Quad[2].y = yPos - offsetY * globalScale;
		s_Quad[2].z = zPos;
	}

	IwGxSetVertStreamModelSpace(s_Quad, 4);

	static CIwColour tmpColor[4];
	tmpColor[0].Set(255, 255, 255, alpha);
	tmpColor[1].Set(255, 255, 255, alpha);
	tmpColor[2].Set(255, 255, 255, alpha);
	tmpColor[3].Set(255, 255, 255, alpha);
	IwGxSetColStream(tmpColor, 4);
	
	static CIwFVec2 s_UVs[4];

	float _uvMinY = 0.033333f;
	float _uvMaxY = 0.1f;
	float _uvMinX = 0.25f * id;
	float _uvMaxX = 0.25f * (id + 1);

	float uvMinX = (_uvMinX * cursorMapTD.textureWidth + cursorMapTD.textureXPos) / (float)cursorMapTD.atlasWidth;
	float uvMinY = (_uvMinY * cursorMapTD.textureHeight  + cursorMapTD.textureYPos) / (float)cursorMapTD.atlasHeight;
	float uvMaxX = (_uvMaxX * cursorMapTD.textureWidth + cursorMapTD.textureXPos) / (float)cursorMapTD.atlasWidth;
	float uvMaxY = (_uvMaxY * cursorMapTD.textureHeight  + cursorMapTD.textureYPos) / (float)cursorMapTD.atlasHeight;

	s_UVs[0] = CIwFVec2(uvMinX, uvMinY);
	s_UVs[1] = CIwFVec2(uvMinX, uvMaxY);
	s_UVs[2] = CIwFVec2(uvMaxX, uvMaxY);
	s_UVs[3] = CIwFVec2(uvMaxX, uvMinY);

	IwGxSetUVStream(s_UVs);
	IwGxDrawPrims(IW_GX_QUAD_LIST, NULL, 4);
}

void Utils::PauseGame()
{
	bPauseGame = true;
}

void Utils::UnpauseGame()
{
	bPauseGame = false;
}

CIwFVec2 Utils::ParabolicInterp(CIwFVec2 initial, CIwFVec2 final, float t)
{
	CIwFVec2 points[4];

	points[0] = initial;
	points[3] = final;

	float length_vector = (float)fabs(EuclideanDistance(initial.x, initial.y, final.x, final.y) / 6);
	//float y_length = length_vector * 0.15f;

	CIwFVec3 vector_a   = CIwFVec3(final.x - initial.x, final.y - initial.y, 0);
	CIwFVec3 aux_vector = CIwFVec3(0, 0, 1);

	CIwFVec3 intermediate_vector = Utils::CrossProduct(vector_a, aux_vector) / Utils::Norm(vector_a); 
	intermediate_vector = Utils::VectorMultiplyScalar(intermediate_vector, length_vector);
	intermediate_vector = CIwFVec3(intermediate_vector.x + initial.x, intermediate_vector.y + initial.y, 0);

	float lx = (final.x - initial.x) / 3;
	float ly = (final.y - initial.y) / 3;
	CIwFVec3 p1 = CIwFVec3(intermediate_vector.x + lx, intermediate_vector.y + ly, 0);
	CIwFVec3 p2 = CIwFVec3(intermediate_vector.x + lx * 2, intermediate_vector.y + ly * 2, 0);

	points[1].x = p1.x; points[1].y = p1.y;
	points[2].x = p2.x; points[2].y = p2.y;

	return GetCastelJausPoint(3, 0, t, points);
}

CIwFVec2 Utils::GetCastelJausPoint(int r, int i, float t, CIwFVec2 points[])
{	
	if(r == 0) return points[i];
	CIwFVec2 p1 = GetCastelJausPoint(r - 1, i, t, points);
	CIwFVec2 p2 = GetCastelJausPoint(r - 1, i + 1, t, points);

	return CIwFVec2(((1 - t) * p1.x + t * p2.x), ((1 - t) * p1.y + t * p2.y));	
}

float Utils::EuclideanDistance(float x1, float y1, float x2, float y2)
{
	return (float)(sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2)));
}

int Utils::GetFadeAlpha(int currTime, int fadeTime, bool bFadeIn)
{
	if(fadeTime == 0) 
		return 0;

	if(bFadeIn)
	{
		return (int)(255 * (currTime / (float)fadeTime));
	}
	else
	{
		return (int)(255 * (1 - currTime / (float)fadeTime));
	}
}

int Utils::GetCompleteAlpha(int currTime, int fadeTime, bool bFromZero)
{
	if(fadeTime == 0) 
		return 0;

	int halfTime = (int)(fadeTime * 0.5f);

	if(!bFromZero)
	{
		if(currTime < halfTime)
			return (int)(255 * (1 - currTime / (float)halfTime));
		else
			return (int)(255 * ((currTime - halfTime) / (float)halfTime));
	}

	return 0;
	/*
	if(bFadeIn)
	{
		return (int)(255 * (currTime / (float)fadeTime));
	}
	else
	{
		return (int)(255 * (1 - currTime / (float)fadeTime));
	}
	*/
}

bool Utils::MoveCamera(int xInitPos, int yInitPos, int xFinalPos, int yFinalPos, float t)
{
	int newXPos = (int)((1 - t) * xInitPos + t * xFinalPos);
	int newYPos = (int)((1 - t) * yInitPos + t * yFinalPos);

	xWindowOffset = Utils::ToCameraXPos(newXPos);
	yWindowOffset = Utils::ToCameraYPos(newYPos);

	return Utils::FixWindowOffset();
}

int Utils::GetCameraX()
{
	return (int)(currentGameWidth * 0.5f) + xWindowOffset;
}

int Utils::GetCameraY()
{
	return (int)(currentGameHeight * 0.5f) + yWindowOffset;
}

int Utils::ToCameraXPos(int normalX)
{
	return -(int)(currentGameWidth * 0.5f) + normalX;
}

int Utils::ToCameraYPos(int normalY)
{
	return -(int)(currentGameHeight * 0.5f) + normalY;
}

bool Utils::IsControllerActive(ControllerType type)
{
	return activeControllerType == type;
}

void Utils::SetCameraPos(int xPos, int yPos)
{
	xWindowOffset = Utils::ToCameraXPos(xPos);
	yWindowOffset = Utils::ToCameraYPos(yPos);

	Utils::FixWindowOffset();
}

bool Utils::FixWindowOffset()
{
	bool bCorrection = false;

	if(xWindowOffset < xMinOffset) { xWindowOffset = xMinOffset; bCorrection = true; }
	if(xWindowOffset > xMaxOffset) { xWindowOffset = xMaxOffset; bCorrection = true; }
	if(yWindowOffset < yMinOffset) { yWindowOffset = yMinOffset; bCorrection = true; }
	if(yWindowOffset > yMaxOffset) { yWindowOffset = yMaxOffset; bCorrection = true; }

	return bCorrection;
}

void Utils::GetMenuTitle(RenderData* data, CIwSVec2* position)
{
	*data = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("menu_title"));
	CIwSVec2 menuTitleDimension = TextureHelper::GetDimension(*data);
	position->x = (int)((currentGameWidth - menuTitleDimension.x) * 0.5f);
	position->y = Utils::GetScaledInt(30);;
}

void Utils::GetLongMenuButton(ButtonRenderData* data)
{
	TextureData tmpData;
	tmpData = TextureHelper::GetTextureNamed("btn_menu_long");
	data->numStates = 1;
	data->state[0].RollOut = TextureHelper::GetRenderData(tmpData, 1, 4, 0);
	data->state[0].RollOn = TextureHelper::GetRenderData(tmpData, 1, 4, 1);
	data->state[0].Inactive = TextureHelper::GetRenderData(tmpData, 1, 4, 3);
	data->state[0].Pressed = TextureHelper::GetRenderData(tmpData, 1, 4, 2);
}

void Utils::RegisterMouseCallbacks(void (clickCallback)(s3ePointerEvent*), void (movementCallback)(s3ePointerMotionEvent*))
{
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)movementCallback, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)clickCallback, NULL);
}

void Utils::UnRegisterMouseCallbacks(void (clickCallback)(s3ePointerEvent*), void (movementCallback)(s3ePointerMotionEvent*))
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)movementCallback);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)clickCallback);
}

int Utils::GetMusicMaxValue()
{
	return 250;
}

void Utils::SetMusicVolume(int newValue)
{
	currentAudioVolume = max(min(GetMusicMaxValue(), newValue), 0);
	s3eAudioSetInt(S3E_AUDIO_VOLUME, currentAudioVolume);
}

float Utils::GetMusicVolume()
{
	int currentMusicValue = s3eAudioGetInt(S3E_AUDIO_VOLUME);
	return currentMusicValue / (float)GetMusicMaxValue();
}

int Utils::GetFXSoundMaxValue()
{
	return 250;
}

void Utils::SetFXSoundVolume(int newValue)
{
	currentSoundVolume = max(min(GetFXSoundMaxValue(), newValue), 0);
	s3eSoundSetInt(S3E_SOUND_VOLUME, currentSoundVolume);
}

float Utils::GetFXSoundVolume()
{
	int currentMusicValue = s3eSoundGetInt(S3E_SOUND_VOLUME);
	return currentMusicValue / (float)GetFXSoundMaxValue();
}

bool Utils::GetIsFullscreen()
{
	s3eBool b = s3eWindowIsFullscreen();
	
	if(b == 1)
		return true;
	else 
		return false;
}

int Utils::MaskLeadeboardScore(LBCampaignScore campaignScore)
{
	int myValue = campaignScore.totalScore + (campaignScore.level << 22);
	//IwTrace(MASK, ("MaskLeadeboardScore() , level %i , totalScore %i , myValue %i",campaignScore.level, campaignScore.totalScore, myValue));
	return myValue;
}

LBCampaignScore Utils::UnMaskLeadeboardScore(int rawValue)
{
	int myLevel = 0;
	int myTotalScore = 0;
	int tmpRawValue = rawValue;
	int tmpMaskLevel = 0;
	LBCampaignScore tmpLbCampaignScore;
	tmpLbCampaignScore.level = 0;
	tmpLbCampaignScore.totalScore = 0;

	myLevel = (tmpRawValue >> 22);
	tmpMaskLevel = (myLevel << 22);
	myTotalScore = tmpRawValue - tmpMaskLevel;

	tmpLbCampaignScore.level = myLevel;
	tmpLbCampaignScore.totalScore = myTotalScore;

	//IwTrace(MASK, ("UnMaskLeadeboardScore() , level %i , totalScore %i , rawValue %i",tmpLbCampaignScore.level, tmpLbCampaignScore.totalScore, rawValue));
	return tmpLbCampaignScore;
}

void Utils::ToWindowedScreen()
{
#if defined DESKTOPPLATFORM
	s3eWindowSetFullscreen(NULL);
#endif
}

void Utils::ToFullScreen()
{
#if defined DESKTOPPLATFORM
	s3eWindowSetFullscreen(&g_FullscreenMode);
#endif
}