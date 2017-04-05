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

#ifndef _DIALOGCONTROLLER_H
#define _DIALOGCONTROLLER_H

#include "timer.h"
#include "button.h"

#if defined DESKTOPPLATFORM
class Xbox360Controller;
#endif

#define MAX_DIALOGS 60
#define MAX_SCENES 5
#define MAX_IMAGES 10

typedef enum {
	DIALOG_NONE,
	DIALOG_SELKNAM_1,
	DIALOG_ENEMY_1,
	DIALOG_BARON,
	DIALOG_ARCHER,
	DIALOG_SCIENTIST,
	DIALOG_MAGE,
	DIALOG_TANK,
	DIALOG_BOWLER,
	DIALOG_CAPTAIN
} DIALOG_IMAGE;

class ButtonRenderData;
class RenderData;

class DialogObject 
{
public:
	DialogObject()
	{
		numLeftImages  = 0; 
		numRightImages = 0;
	}
	
	void SetContent(string _content)
	{
		content = _content;
	}

	string GetContent()
	{
		return content;
	}

	void AddLeftImage(DIALOG_IMAGE _image)
	{
		leftImages[numLeftImages++] = _image;
	}

	int GetNumLeftImages()
	{
		return numLeftImages;
	}

	DIALOG_IMAGE GetImage(int num)
	{
		return leftImages[num];
	}

	void AddRightImage(DIALOG_IMAGE _image)
	{
		rightImages[numRightImages++] = _image;
	}

	int GetNumRightImages()
	{
		return numRightImages;
	}

	DIALOG_IMAGE GetRightImage(int num)
	{
		return rightImages[num];
	}

private:
	string       content;
	int          numLeftImages;
	int          numRightImages;
	DIALOG_IMAGE leftImages[MAX_IMAGES];
	DIALOG_IMAGE rightImages[MAX_IMAGES];
};

class DialogController
{
public:
	DialogController();
	~DialogController();
	void Update();
	void Render();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	bool DialogExists(int dialogId);
	void StartDialog(int dialogId);
	bool IsActive();
	void GoToNextScene();
	void ResizeTextSpace();
	RenderData GetImageData(DIALOG_IMAGE image);
	void AddLeftImage(int dialogId, int sceneId, DIALOG_IMAGE image);
	void AddRightImage(int dialogId, int sceneId, DIALOG_IMAGE image);

#if defined DESKTOPPLATFORM
	Xbox360Controller* xbox360Controller;
#endif

private:
	void RegisterDialog(int dialogId);
	void RenderMessageBox(string content);
	void CreateMessage(int dialogId, int sceneId, string content);
	bool BossStageSpecialDialogCondition();

	bool         activeDialogs[MAX_DIALOGS];
	bool         bIsActive;
	bool         bClosingAnimation;
	DialogObject dialogObject[MAX_DIALOGS][MAX_SCENES];
	Timer        messageBoxFadeTimer;
	Timer        leftImagesMotionTimer[MAX_IMAGES];
	Timer        rightImagesMotionTimer[MAX_IMAGES];
	Timer        closingTimer;
	Timer        delayAllowTap;
	int          imageYOffset;
	int          imageXOffset;
	int          lastScene[MAX_DIALOGS];
	int          currentActiveDialogId;
	int          currentActiveScene;
	int          messageBoxWidth, messageBoxHeight, messageBoxX, messageBoxY;
	int          textOffset;
	int          leftImageTextOffset, rightImageTextOffset;
	int          leftInitialXOffset, rightInitialXOffset;

	RenderData* dialogSelknam1;
	RenderData* dialogEnemy1;

	int storeLastInGameState;

	bool bShakeCamera;
};

#endif