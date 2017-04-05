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

#ifndef _SETTINGS_MENU_H
#define _SETTINGS_MENU_H

#include "game_window.h"
#include "background.h"
#include "button.h"
#include "common.h"
#include "slider.h"
#include "checkbox.h"
#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

#define TIME_FIRST_ITERATION_VOL 150
#define TIME_ITERATION_VOL 20

class SettingsMenu : public GameWindow
{
public:
	SettingsMenu();
	~SettingsMenu();

	void InitObject();
	void EndObject();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void ToggleFullscreen();
	void VolumeUp();
	void VolumeDown();

	void SetVolumeContIncrease(bool bValue);
	void SetVolumeContDecrease(bool bValue);

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

private:
	bool bIncreaseVolume;
	bool bDecreaseVolume;
	bool bChangeVolumeFirstTime;
	Timer changeVolumeTimer;

	Background* background;

	Button* btnAccept;
	ButtonRenderData buttonRenderData;
	int PannelPosX;
	int PannelPosY;
	int btnBackPosX;
	int btnBackPosY;
	int widthSettingsPannel;
	int heightSettingsPannel;
	int RenderPanelNumWidth;
	int RenderPanelNumHeight;
	int settingsTitleYPos;
	Slider musicSlider;
	Slider fxSoundSlider;
	Checkbox fullScreenCheckbox;
	Checkbox muteAllCheckbox;
	int midWidth; 
	int yMargin;
	int yPadding;
	int xPadding;
};

#endif