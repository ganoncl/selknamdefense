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

#ifndef _UTILS_H
#define _UTILS_H

#include "IwGx.h"
#include "common.h"
#include "animation.h"
#include "isopawn.h"

class Utils
{
public:
	static void CopyRenderData(RenderData* destination, RenderData source);
	static void RenderSingleTextureFloat(CIwTexture* tex, CIwFVec2 position, RenderData data);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData* data, int degree = 0);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, int r = 255, int g = 255, int b = 255, int alpha = 255);
	static void RenderSingleTexture(CIwTexture* tex, RenderData data, CIwSVec2 position, int r = 255, int g = 255, int b = 255, int alpha = 255);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, float _uvMinX, float _uvMinY, float _uvMaxX, float _uvMaxY);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, UVInfo uv, bool bRotate = false);
	static void RenderSingleTexture(bool p, CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, float _uvMinX, float _uvMinY, float _uvMaxX, float _uvMaxY, bool bRotate = false);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData data, RotateImage rotate = NO_ROTATE);
	static void InitQuadDrawing(CIwTexture* tex, bool bAlphaMode = true);
	static void DrawQuadBatch(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX, float _uvMaxX, float _uvMinY, float _uvMaxY, float _offsetX, float _offsetY, int r = 255, int g = 255, int b = 255, int a = 255);
	static void RenderQuadBatch();
	static void SetEnemyAnim(IsoEnemyType type, Animation* animations, IsoPawn* pawn);
	static void SetSelknamAnim(IsoSelknamType type, Animation* animations, IsoPawn* pawn);
	static void SetBuffImageUV(Skill_Type type, float* uvMaxX, float* uvMinX, float* uvMaxY, float* uvMinY);
	static int Round(float num);
	static void RenderColor(uint32 color, CIwSVec2 position, CIwSVec2 size);
	static void RenderColor(uint32 colorA, uint32 colorB, CIwSVec2 position, CIwSVec2 size, bool bVerticalDegrade = false);
	static void RenderText(const char* text, CIwRect rect, CIwGxFont* pfont, uint32 color = 0xffffffff, IwGxFontAlignHor alig_h = IW_GX_FONT_ALIGN_CENTRE, IwGxFontAlignVer alig_v = IW_GX_FONT_ALIGN_MIDDLE, bool bShadow = false, int distShadow = 1);
	static int GetSizeArray(int* arr);

	static string GetUnformattedLoadedGroup(string myLoadedGroup);

	static void SetWalkAnim(IsoPawnAnim anim, Animation* animations);
	static void SetIdleAnim(IsoPawnAnim anim, Animation* animations);
	static void SetDeathAnim(IsoPawnAnim anim, Animation* animations);
	static void SetAttackAnim(IsoPawnAnim anim, Animation* animations);

	static bool CanAttackByAnimation(IsoEnemyType type, int id);
	static bool CanAttackByAnimation(IsoSelknamType type, int id);

	static CIwFVec3 CrossProduct(CIwFVec3 A, CIwFVec3 B);
	static float Norm(CIwFVec3 A);
	static CIwFVec3 Normalize(CIwFVec3 A);
	static CIwFVec3 VectorMultiplyScalar(CIwFVec3 A, float scalar);
	static bool PlayFXSound(FxSound sound);
	static void StopAllFXSounds();
	static void ResumeAllFXSounds();
	static void UtilsUpdate();
	static void UtilsInit();
	static bool IsFxSoundAvailable(FxSound sound);
	static string TransformToMapFilename(int currentMap, bool extraMode);
	static bool CheckIfMouseIsEnabled();
	static void GetIsoSpaceCoord(float XPos, float YPos, float XOffset, float YOffset, float *newIsoXPos, float *newIsoYPos);
	static void PlayAudio(Music_Name musicName);

	/* Render three images: "label_title_left", "label_title_right" and "label_title_center". The last one is rendered "len" times. */
	static void RenderLabelTitle(int len, int xPos, int yPos, float scale = 1.0f);

	/* Render three images: pannel_a_border (2 x width + 2 x height times), pannel_a_center (width x height times) and pannel_a_corner (4 times). */
	static void RenderPannelA(int width, int height, int xPos, int yPos);
	static void RenderPannelB(int width, int height, int xPos, int yPos);

	static void RenderVerticalTube(int xPos, int yPos, int large);

	/* Swap two float variables. */
	static void SwapFloat(float *float1, float* float2);
};

#endif