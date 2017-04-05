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
#include <algorithm>
#include "texturehelper.h"
using namespace std;

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

Timer playfxsound_timer;

//RenderLabelTitle variables.
RenderData label_title_center_renderdata, label_title_left_renderdata, label_title_right_renderdata;
AtlasName label_title_center_atlasname, label_title_left_atlasname, label_title_right_atlasname;

//RenderPanelA variables.
RenderData pannel_a_corner_upleft_renderdata, pannel_a_corner_upright_renderdata, pannel_a_corner_downleft_renderdata, pannel_a_corner_downright_renderdata;
RenderData pannel_a_border_up_renderdata, pannel_a_border_down_renderdata, pannel_a_border_left_renderdata, pannel_a_border_right_renderdata;
RenderData pannel_a_center_renderdata;
AtlasName pannel_a_corner_upleft_atlasname, pannel_a_corner_upright_atlasname, pannel_a_corner_downleft_atlasname, pannel_a_corner_downright_atlasname;
AtlasName pannel_a_border_up_atlasname, pannel_a_border_down_atlasname, pannel_a_border_left_atlasname, pannel_a_border_right_atlasname;
AtlasName pannel_a_center_atlasname;

//RenderPanelB variables.
RenderData pannel_b_corner_upleft_renderdata, pannel_b_corner_upright_renderdata, pannel_b_corner_downleft_renderdata, pannel_b_corner_downright_renderdata;
RenderData pannel_b_border_up_renderdata, pannel_b_border_down_renderdata, pannel_b_border_left_renderdata, pannel_b_border_right_renderdata;
RenderData pannel_b_center_renderdata;
AtlasName pannel_b_corner_upleft_atlasname, pannel_b_corner_upright_atlasname, pannel_b_corner_downleft_atlasname, pannel_b_corner_downright_atlasname;
AtlasName pannel_b_border_up_atlasname, pannel_b_border_down_atlasname, pannel_b_border_left_atlasname, pannel_b_border_right_atlasname;
AtlasName pannel_b_center_atlasname;

RenderData pannel_tube_renderdata;
AtlasName pannel_tube_atlasname;

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

void Utils::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData* data, int degree)
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

	IwGxSetColStream(NULL);

	s_UVs[0] = CIwFVec2(data->uvInfo.minX, data->uvInfo.minY);
	s_UVs[1] = CIwFVec2(data->uvInfo.minX, data->uvInfo.maxY);
	s_UVs[2] = CIwFVec2(data->uvInfo.maxX, data->uvInfo.maxY);
	s_UVs[3] = CIwFVec2(data->uvInfo.maxX, data->uvInfo.minY);
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

	//IwTrace(APPLOG, ("NUM BATCHING: %i", numImagesBatched_util));

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
	animations->ClearAnimation();
	//Soldier Case
	if(enemyType == ENEMY_Soldier)
	{
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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


void Utils::SetSelknamAnim(IsoSelknamType SelknamType, Animation* animations, IsoPawn* pawn)
{
	animations->ClearAnimation();
	if(SelknamType == SELKNAM_Archer_Base)
	{
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
		pawn->setWidth(64);
		pawn->setHeight(96);

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
	 /*
	 int _floor = (int)((num - (int)num) * 10);
	 if(_floor > 5)
		 return (int)num + 1;
	 else
		 return (int)num;
	 */
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

 void Utils::RenderText(const char* text, CIwRect rect, CIwGxFont* pfont, uint32 color, IwGxFontAlignHor alig_h, IwGxFontAlignVer alig_v, bool bShadow, int distShadow)
 {
	 
	 CIwGxFontPreparedData data;
	 //if(bShadow)
	 //{
		 IwGxFontSetRect(CIwRect(rect.x - distShadow, rect.y + distShadow, rect.w, rect.h));
		 IwGxFontSetFont(pfont);
		 IwGxFontSetAlignmentHor(alig_h);
		 IwGxFontSetAlignmentVer(alig_v);
		 IwGxFontSetCol(0xff000000);
		 IwGxFontPrepareText(data, text);
		 IwGxFontDrawText(data);
	 //}
	 
	 IwGxFontSetRect(rect);
	 IwGxFontSetFont(pfont);
	 IwGxFontSetAlignmentHor(alig_h);
	 IwGxFontSetAlignmentVer(alig_v);
	 IwGxFontSetCol(color);
	 IwGxFontPrepareText(data, text);
	 IwGxFontDrawText(data);
 }

 bool Utils::CanAttackByAnimation(IsoEnemyType type, int id)
 {
	 //IwTrace(CANATTACKBUG, ("%d", id));

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
	 //IwTrace(SELKNAM, ("ANIMATION ID: %d", id));

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

 bool Utils::PlayFXSound(FxSound sound)
 {
	 if(bIsFxPaused)
		 return false;

	 //FXSoundManager->Play((int)sound);

	 if(IwGetSoundManager()->GetNumInsts() >= 5)
		 return false;
	 

	 if(bFxSoundMuted)
		 return false;

	 bool bDelay = true;

	 if(playfxsound_timer.GetIsEnd() || playfxsound_timer.GetIsStop())
	 {
		 playfxsound_timer.Reset();
		 playfxsound_timer.Start();
	 }
	 else
	 {
		 bDelay = false;
	 }
	 if     (sound == FX_Sound_ARROWSHOOTING && Utils::IsFxSoundAvailable(FX_Sound_ARROWSHOOTING)) Arrow_Shooting->Play();
	 else if(sound == FX_Sound_CLICK         && Utils::IsFxSoundAvailable(FX_Sound_CLICK))         FX_Click->Play();
	 else if(sound == FX_Sound_EXPLOSION     && Utils::IsFxSoundAvailable(FX_Sound_EXPLOSION))     FX_Explosion->Play();
	 else if(sound == FX_Sound_KNIFESWORD01  && Utils::IsFxSoundAvailable(FX_Sound_KNIFESWORD01))  FX_Knife_Sword_01->Play();
	 else if(sound == FX_Sound_ROLLOVER02    && bDelay && Utils::IsFxSoundAvailable(FX_Sound_ROLLOVER02))    FX_RollOver_02->Play();
	 else if(sound == FX_Sound_SHOT01        && Utils::IsFxSoundAvailable(FX_Sound_SHOT01)) FX_Shot_01->Play();
	 else if(sound == FX_Sound_STARGAINED02  && Utils::IsFxSoundAvailable(FX_Sound_STARGAINED02)) FX_Star_Gained_02->Play();
	 else if(sound == FX_Sound_METALIMPACT01 && Utils::IsFxSoundAvailable(FX_Sound_METALIMPACT01)) FX_MetalImpact->Play();
	 else if(sound == FX_Sound_METALIMPACT02 && Utils::IsFxSoundAvailable(FX_Sound_METALIMPACT02)) FX_MetalImpact2->Play();
	 else if(sound == FX_Sound_COINS         && Utils::IsFxSoundAvailable(FX_Sound_COINS)) FX_Coins->Play();
	 else if(sound == FX_Sound_HEAL          && Utils::IsFxSoundAvailable(FX_Sound_HEAL)) FX_Heal->Play();
	 else if(sound == FX_Sound_BUBBLES       && Utils::IsFxSoundAvailable(FX_Sound_BUBBLES)) FX_Bubbles->Play();
	 return true;
 }

 void Utils::StopAllFXSounds()
 {
	 IwGetSoundManager()->StopAll();
	 bIsFxPaused = true;
 }

 void Utils::ResumeAllFXSounds()
 {
	 bIsFxPaused = false;
 }

 bool Utils::IsFxSoundAvailable(FxSound sound)
 {
	 int idx = (int)sound;
	 //IwTrace(FXSOUNDTRACE, ("asdasd: %d %d", fx_sound_limit[sound][1], idx));
	 if(fx_sound_limit[sound][0] > (fx_sound_limit[sound][1]++))
	 {
		 IwTrace(FXSOUNDTRACE, ("PLAY!"));
		 return true;
	 }
	 return false;
 }

 void Utils::UtilsUpdate()
 {
	 playfxsound_timer.Update();
 }

 void Utils::UtilsInit()
 {
	 playfxsound_timer.SetTimeLimit(50);

	 //RenderLabelTitle init
	 label_title_center_renderdata = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_title_center"));
	 label_title_left_renderdata   = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_title_left"));
	 label_title_right_renderdata  = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_title_right"));
	 label_title_center_atlasname  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("label_title_center"));
	 label_title_left_atlasname    = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("label_title_left"));
	 label_title_right_atlasname   = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("label_title_right"));

	 //RenderPanelA init
	 pannel_a_center_renderdata           = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_center"));
	 pannel_a_center_atlasname            = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_center"));
	 pannel_a_corner_upleft_renderdata    = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_corner"));
	 pannel_a_corner_upleft_atlasname     = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_corner"));
	 pannel_a_corner_downleft_renderdata  = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_corner")), MIRROR_HORIZONTAL);
	 pannel_a_corner_downleft_atlasname   = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_corner"));
	 pannel_a_corner_upright_renderdata   = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_corner")), MIRROR_VERTICAL);
	 pannel_a_corner_upright_atlasname    = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_corner"));
	 pannel_a_corner_downright_renderdata = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_corner")), MIRROR_DIAGONAL);
	 pannel_a_corner_downright_atlasname  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_corner"));
	 pannel_a_border_up_renderdata        = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_border"));
	 pannel_a_border_up_atlasname         = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_border"));
	 pannel_a_border_down_renderdata      = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_border")), MIRROR_HORIZONTAL);
	 pannel_a_border_down_atlasname       = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_border"));
	 pannel_a_border_left_renderdata     = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_border")), MIRROR_HORIZONTAL);
	 pannel_a_border_left_atlasname      = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_border"));
	 pannel_a_border_right_renderdata      = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_a_border"));
	 pannel_a_border_right_atlasname       = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_a_border"));

	 //RenderPanelB init
	 pannel_b_center_renderdata           = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_center"));
	 pannel_b_center_atlasname            = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_center"));
	 pannel_b_corner_upleft_renderdata    = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_corner"));
	 pannel_b_corner_upleft_atlasname     = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_corner"));
	 pannel_b_corner_downleft_renderdata  = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_corner")), MIRROR_HORIZONTAL);
	 pannel_b_corner_downleft_atlasname   = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_corner"));
	 pannel_b_corner_upright_renderdata   = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_corner")), MIRROR_VERTICAL);
	 pannel_b_corner_upright_atlasname    = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_corner"));
	 pannel_b_corner_downright_renderdata = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_corner")), MIRROR_DIAGONAL);
	 pannel_b_corner_downright_atlasname  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_corner"));
	 pannel_b_border_up_renderdata        = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_border"));
	 pannel_b_border_up_atlasname         = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_border"));
	 pannel_b_border_down_renderdata      = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_border")), MIRROR_HORIZONTAL);
	 pannel_b_border_down_atlasname       = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_border"));
	 pannel_b_border_left_renderdata     = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_border")), MIRROR_HORIZONTAL);
	 pannel_b_border_left_atlasname      = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_border"));
	 pannel_b_border_right_renderdata      = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_b_border"));
	 pannel_b_border_right_atlasname       = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_b_border"));

	 //Tube
	 pannel_tube_renderdata = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_tube"));
	 pannel_tube_atlasname  = TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("pannel_tube"));
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

 void Utils::PlayAudio(Music_Name musicName)
 {
	if(bIsAudioPaused)
	{
		LastMusicStacked = musicName;
		return;
	}

	if(musicName == Music_Loose)
	{
		s3eAudioStop();
		s3eAudioPlay("bgm/sd_fanfare_lose_theme.mp3", 1);
	}
	else if(musicName == Music_Win)
	{
		s3eAudioStop();
		s3eAudioPlay("bgm/sd_fanfare_win_theme.mp3", 1);
	}
	else if(musicName == Music_Ingame_01)
	{
		s3eAudioStop();
		if(currentMap == 1 && bShowTutorialLevel1)
			s3eAudioPlay("bgm/sd_ingame_01_theme_loop.mp3", 0);
		else
			s3eAudioPlay("bgm/sd_ingame_01_theme_loop.mp3", 1);
	}
	else if(musicName == Music_Ingame_02)
	{
		s3eAudioStop();
		s3eAudioPlay("bgm/sd_ingame_02_theme_loop.mp3", 0);
	}
	else if(musicName == Music_Store)
	{
		IwTrace(TEXHELP, ("Plagin Music Store theme."));
		s3eAudioStop();
		s3eAudioPlay("bgm/sd_store_theme_loop.mp3", 0);
	}
	else if(musicName == Music_MainMenu)
	{
		s3eAudioStop();
		s3eAudioPlay("bgm/sd_mainmenu_theme_loop.mp3", 0);
	}	

	LastMusicStacked = Music_None;
	CurrentMusicPlaying = musicName;
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

 void Utils::RenderPannelA(int width, int height, int xPos, int yPos)
 {
	 int w = TextureHelper::GetDimension(pannel_a_center_renderdata).x;
	 int h = TextureHelper::GetDimension(pannel_a_center_renderdata).y;
	 int xOffset = TextureHelper::GetDimension(pannel_a_corner_upleft_renderdata).x;
	 int yOffset = TextureHelper::GetDimension(pannel_a_corner_upleft_renderdata).y;

	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_corner_upleft_atlasname), CIwSVec2(xPos, yPos), pannel_a_corner_upleft_renderdata);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_corner_downleft_atlasname), CIwSVec2(xPos, yPos + yOffset + h * height), pannel_a_corner_downleft_renderdata);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_corner_upright_atlasname), CIwSVec2(xPos + xOffset + w * width, yPos), pannel_a_corner_upright_renderdata);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_corner_downright_atlasname), CIwSVec2(xPos + xOffset + w * width, yPos + yOffset + w * height), pannel_a_corner_downright_renderdata);

	 int wBorder = TextureHelper::GetDimension(pannel_a_border_up_renderdata).x;
	 int hBorder = TextureHelper::GetDimension(pannel_a_border_up_renderdata).y;
	 for(int i = 0; i < width; i++)
	 {
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_border_up_atlasname), CIwSVec2(xPos + xOffset + wBorder * i, yPos), pannel_a_border_up_renderdata);
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_border_down_atlasname), CIwSVec2(xPos + xOffset + wBorder * i, yPos + yOffset + w * height), pannel_a_border_down_renderdata);
	 }

	 for(int i = 0; i < height; i++)
	 {
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_border_right_atlasname), CIwSVec2(xPos + xOffset + w * width, yPos + yOffset + hBorder * i), pannel_a_border_right_renderdata, ROTATE_270);
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_border_left_atlasname), CIwSVec2(xPos, yPos + yOffset + hBorder * i), pannel_a_border_left_renderdata, ROTATE_270);
	 }

	 for(int i = 0; i < height; i++)
	 {
		 for(int j = 0; j < width; j++)
		 {
			 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_a_center_atlasname), CIwSVec2(xPos + w * j + xOffset, yPos + h * i + yOffset), pannel_a_center_renderdata);
		 }
	 }
 }

 void Utils::RenderPannelB(int width, int height, int xPos, int yPos)
 {
	 int w = TextureHelper::GetDimension(pannel_b_center_renderdata).x;
	 int h = TextureHelper::GetDimension(pannel_b_center_renderdata).y;
	 int xOffset = TextureHelper::GetDimension(pannel_b_corner_upleft_renderdata).x;
	 int yOffset = TextureHelper::GetDimension(pannel_b_corner_upleft_renderdata).y;

	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_corner_upleft_atlasname), CIwSVec2(xPos, yPos), pannel_b_corner_upleft_renderdata);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_corner_downleft_atlasname), CIwSVec2(xPos, yPos + yOffset + h * height), pannel_b_corner_downleft_renderdata);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_corner_upright_atlasname), CIwSVec2(xPos + xOffset + w * width, yPos), pannel_b_corner_upright_renderdata);
	 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_corner_downright_atlasname), CIwSVec2(xPos + xOffset + w * width, yPos + yOffset + w * height), pannel_b_corner_downright_renderdata);

	 int wBorder = TextureHelper::GetDimension(pannel_b_border_up_renderdata).x;
	 int hBorder = TextureHelper::GetDimension(pannel_b_border_up_renderdata).y;
	 for(int i = 0; i < width; i++)
	 {
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_border_up_atlasname), CIwSVec2(xPos + xOffset + wBorder * i, yPos), pannel_b_border_up_renderdata);
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_border_down_atlasname), CIwSVec2(xPos + xOffset + wBorder * i, yPos + yOffset + w * height), pannel_b_border_down_renderdata);
	 }

	 for(int i = 0; i < height; i++)
	 {
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_border_right_atlasname), CIwSVec2(xPos + xOffset + w * width, yPos + yOffset + hBorder * i), pannel_b_border_right_renderdata, ROTATE_270);
		 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_border_left_atlasname), CIwSVec2(xPos, yPos + yOffset + hBorder * i), pannel_b_border_left_renderdata, ROTATE_270);
	 }

	 for(int i = 0; i < height; i++)
	 {
		 for(int j = 0; j < width; j++)
		 {
			 Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(pannel_b_center_atlasname), CIwSVec2(xPos + w * j + xOffset, yPos + h * i + yOffset), pannel_b_center_renderdata);
		 }
	 }
 }

 void Utils::RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData data, RotateImage rotate)
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

		 int newWidth = data.height;
		 int newHeight = data.width;

		 s_Quad[3] = CIwSVec2(position.x, position.y);
		 s_Quad[0] = CIwSVec2(position.x, position.y + newHeight);
		 s_Quad[1] = CIwSVec2(position.x + newWidth, position.y + newHeight);
		 s_Quad[2] = CIwSVec2(position.x + newWidth, position.y);
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

 void Utils::SwapFloat(float *float1, float* float2)
 {
	 float fTemp = *float1;
	 *float1 = *float2;
	 *float2 = fTemp;
 }