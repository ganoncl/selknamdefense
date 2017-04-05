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
	static XBOX360_Button GetActionButton1() { return XBOX360_Button_A; };
	static XBOX360_Button GetActionButton2() { return XBOX360_Button_X; };

	static void CopyRenderData(RenderData* destination, RenderData source);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData data, RotateImage rotate = NO_ROTATE, uint8 alpha = 255);
	static void RenderSingleTextureFloat(CIwTexture* tex, CIwFVec2 position, RenderData data);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, RenderData* data, int degree = 0, int alpha = 255);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, int r = 255, int g = 255, int b = 255, int alpha = 255);
	static void RenderSingleTexture(CIwTexture* tex, RenderData data, CIwSVec2 position, int r = 255, int g = 255, int b = 255, int alpha = 255);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, float _uvMinX, float _uvMinY, float _uvMaxX, float _uvMaxY);
	static void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, UVInfo uv, bool bRotate = false);
	static void RenderSingleTexture(bool p, CIwTexture* tex, CIwSVec2 position, CIwSVec2 dimension, float _uvMinX, float _uvMinY, float _uvMaxX, float _uvMaxY, bool bRotate = false);
	static void InitQuadDrawing(CIwTexture* tex, bool bAlphaMode = true);
	static void DrawQuadBatch(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX, float _uvMaxX, float _uvMinY, float _uvMaxY, float _offsetX, float _offsetY, int r = 255, int g = 255, int b = 255, int a = 255);
	static void RenderQuadBatch();
	static void SetEnemyAnim(IsoEnemyType type, Animation* animations, IsoPawn* pawn);
	static void SetSelknamAnim(IsoSelknamType type, Animation* animations, IsoPawn* pawn);
	static void SetBuffImageUV(Skill_Type type, float* uvMaxX, float* uvMinX, float* uvMaxY, float* uvMinY);
	static int Round(float num);
	static void RenderColor(uint32 color, CIwSVec2 position, CIwSVec2 size);
	static void RenderColor(uint32 colorA, uint32 colorB, CIwSVec2 position, CIwSVec2 size, bool bVerticalDegrade = false);
	static void RenderColor(int r, int g, int b, int a, CIwSVec2 position, CIwSVec2 size);
	static void RenderText(const char* text, CIwRect rect, CIwGxFont* pfont, uint32 color = 0xffffffff, IwGxFontAlignHor alig_h = IW_GX_FONT_ALIGN_CENTRE, IwGxFontAlignVer alig_v = IW_GX_FONT_ALIGN_MIDDLE, bool bShadow = false, int distShadow = 1);
	static void RenderText(const char* text, CIwRect rect, CIwGxFont* pfont, CIwColour color, IwGxFontAlignHor alig_h = IW_GX_FONT_ALIGN_CENTRE, IwGxFontAlignVer alig_v = IW_GX_FONT_ALIGN_MIDDLE, bool bShadow = false, int distShadow = 1);
	static int GetSizeArray(int* arr);

	static void RenderVerticalTube(int xPos, int yPos, int large);

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
	//static bool PlayFXSound(FxSound sound);
	static bool PlaySoundFX(CIwSoundSpec *soundFX);
	static void PlayBGM(const char* filename, uint32 repeatCount);
	static void StopAllFXSounds();
	static void ResumeAllFXSounds();
	static void UpdateRefreshFXSoundExceptions();
	static void UtilsUpdate();
	static void UtilsInit();
	static bool IsFxSoundAvailable(FxSound sound);
	static string TransformToMapFilename(int currentMap, bool extraMode);
	static bool CheckIfMouseIsEnabled();
	static void GetIsoSpaceCoord(float XPos, float YPos, float XOffset, float YOffset, float *newIsoXPos, float *newIsoYPos);

	static string GetUnformattedLoadedGroup(string myLoadedGroup);

	/* Render three images: "label_title_left", "label_title_right" and "label_title_center". The last one is rendered "len" times. */
	static void RenderLabelTitle(int len, int xPos, int yPos, float scale = 1.0f);

	/* Render three images: pannel_a_border (2 x width + 2 x height times), pannel_a_center (width x height times) and pannel_a_corner (4 times). */
	static void RenderPannelA(int width, int height, int xPos, int yPos);
	static void RenderPannelB(int width, int height, int xPos, int yPos);

	/* Swap two float variables. */
	static void SwapFloat(float *float1, float* float2);

	/* Moves camera with Lerp interpolation between position a and b (t is the time, measured in miliseconds) */
	static void MoveCamera(int t, CIwFVec2 a, CIwFVec2 b);
	static bool IsCameraMoving();

	//t e [0,1]
	static bool MoveCamera(int xInitPos, int yInitPos, int xFinalPos, int yFinalPos, float t);
	static int GetCameraX();
	static int GetCameraY();
	static int ToCameraXPos(int normalX);
	static int ToCameraYPos(int normalY);
	static void SetCameraPos(int xPos, int yPos);
	static bool FixWindowOffset();

	static bool IsControllerActive(ControllerType type);

	/* "numRandPoints" random 2-dimensional points (x, y) will be chosen with values 
	 * between (x in [CameraPosX - r, CameraPosX + r], y in [CameraPosY - r, CameraPosY + r])
	 * using srand(time(NULL)) and rand() methods.
	 * duration is measured in miliseconds.
	 * The camera will move through two random points in a time = (duration / (numRandPoints * intensity))
	 */
	static void ShakeCamera(int radius, int numRandPoints, int duration, int intensity);
	static bool IsCameraShaking();
	static void ShakeCameraUpdate();
	static void ShakeCameraInit();

	static void RenderSingleTextureWithAlpha(CIwTexture* tex, CIwFVec2 position, RenderData data, int alpha);

	static string GetClockFormat(int seconds);
	static string GetClockFormatMS(int seconds);

	static bool MouseOn(int x, int y, int xPos, int yPos, int _width, int _height);

	static void DrawExplotion(float x, float y, float z);
	static void RenderExplotions();
	static void UpdateExplotions();

	static void DrawCannonExplotion(float x, float y, float z);
	static void RenderCannonExplotions();
	static void UpdateCannonExplotions();

	static uint32 colorToDecimal(int r, int g, int b, int alpha);
	static bool GameIsPaused();

	//MULTI RES
	static CIwSVec2 GetScaledSize(CIwSVec2 originalSize);
	static void PauseGame();
	static void UnpauseGame();
	static float GetPctChange();
	static int GetScaledInt(int myInt);
	static int GetScaledInt(int myInt, int baseHeight);
	static int GetFixedScaledInt(int myInt);

	//Autoface units.
	static int GetManhattanDistance(IsoTerrain* tile1, IsoTerrain* tile2);

	//Adjust currCenter 2D vector in order to fit completely rect inside the screen
	static CIwSVec2 GetAdjustedCenter2DPointForGUI(CIwSVec2 currCenter, int w, int h);
	static int GetScreenWidth();
	static int GetScreenHeight();
	static int MaxInt(int a, int b);
	static void InitPannel(PannelType type, string center, string corner, string border);
	static void InitBar(BarType type, string leftBorder, string center, string rightBorder);

	static void RenderPannel(PannelType type, int width, int height, int xPos, int yPos, int alpha = 255);
	static void RenderPannel(PannelType type, CIwRect position, int alpha = 255);
	static void RenderBar(BarType type, int numCols, int xPos, int yPos);

	static void FreezeCamera();
	static void UnfreezeCamera();

	static float Lerp(int initial, int final, float t);
	static void RenderBlueCursorMap(IsoTerrain* terrain, int id = 0, int alpha = 255);
	static void RenderYellowCircleCursorMap(IsoTerrain* terrain, int id = 0, int alpha = 255);

	static CIwFVec2 ParabolicInterp(CIwFVec2 initial, CIwFVec2 final, float t); 
	static CIwFVec2 GetCastelJausPoint(int r, int i, float t, CIwFVec2 points[]);
	static float EuclideanDistance(float x1, float y1, float x2, float y2);

	//Value from 0 to 255 (Fade In) OR 255 to 0 (Fade Out)
	static int GetFadeAlpha(int currTime, int fadeTime, bool bFadeIn);

	//Value from 0 to 255 and then again to 0 (bFromZero = true) OR from 255 to 0 and again to 255 (bFromZero = false)
	static int GetCompleteAlpha(int currTime, int fadeTime, bool bFromZero);

	static void GetMenuTitle(RenderData* data, CIwSVec2* position);
	static void GetLongMenuButton(ButtonRenderData* data);

	static void RegisterMouseCallbacks(void (clickCallback)(s3ePointerEvent*), void (movementCallback)(s3ePointerMotionEvent*));
	static void UnRegisterMouseCallbacks(void (clickCallback)(s3ePointerEvent*), void (movementCallback)(s3ePointerMotionEvent*));

	static int GetMusicMaxValue();
	static void SetMusicVolume(int newValue);
	static float GetMusicVolume();

	static int GetFXSoundMaxValue();
	static void SetFXSoundVolume(int newValue);
	static float GetFXSoundVolume();

	static bool GetIsFullscreen();
	static void ToWindowedScreen();
	static void ToFullScreen();

	//LeaderboardNum
	static int MaskLeadeboardScore(LBCampaignScore campaignScore);
	static LBCampaignScore UnMaskLeadeboardScore(int rawValue);

};

#endif