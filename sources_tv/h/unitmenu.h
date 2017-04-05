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

#ifndef _UNITMENU_H
#define _UNITMENU_H

#include "s3e.h"
#include "button.h"
#include "common.h"
#include "utils.h"
#include "texturehelper.h"

typedef enum
{
	UMB_NONE,
	UMB_ARCHER,
	UMB_BOWLER,
	UMB_MAGE,
	UMB_TANK,
} UnitMenuButton;

typedef enum
{
	MENU_None,
	MENU_Archer,
	MENU_Bowler,
	MENU_Tank,
	MENU_Mage,
} menu_name;

typedef void (*function)();

class UnitMenu
{
public:
	UnitMenu();
	~UnitMenu();
	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void KeyboardHandler(s3eKeyboardEvent* event);
	void SetPos(int x, int y);
	int GetX() { return _x; };
	int GetY() { return _y; };
	void SetPadding(int padding);
	int GetPadding() { return _padding; };

	void SetArcherBaseButtonFunction(function func)     { button_ArcherBase->OnClick(func); };
	void SetArcherPoisonerButtonFunction(function func) { button_ArcherPoisoner->OnClick(func); };
	void SetArcherRetarderButtonFunction(function func) { button_ArcherRetarder->OnClick(func); };

	void SetWarriorBaseButtonFunction(function func)    { button_WarriorBase->OnClick(func); };
	void SetWarriorBerserkButtonFunction(function func) { button_WarriorBerserk->OnClick(func); };
	void SetWarriorTankButtonFunction(function func)    { button_WarriorTank->OnClick(func); };

	void SetShamanHealerButtonFunction(function func)   { button_ShamanHealer->OnClick(func); };
	void SetShamanMightButtonFunction(function func)    { button_ShamanMight->OnClick(func); };
	void SetShamanHasteButtonFunction(function func)    { button_ShamanHaste->OnClick(func); };

	void SetBowlerStunerButtonFunction(function func)   { button_BowlerStuner->OnClick(func); };
	void SetBowlerConfuserButtonFunction(function func) { button_BowlerConfuser->OnClick(func); };
	void SetBowlerRooterButtonFunction(function func)   { button_BowlerRooter->OnClick(func); };

	void SetOKButtonFunction(function func)     { btnOK->OnClick(func);     };

	void SetIsActive(bool bIsActive);
	bool GetIsActive() { return bIsActive; };

	void SetActiveOptions();
	void GoToArcherMenu();
	void GoToWarriorMenu();
	void GoToShamanMenu();
	void GoToBowlerMenu();

	void Open(int coins);
	void Close();
	void CloseWithDelay(int64 delay, UnitMenuButton unitMenuButton = UMB_NONE);
	void OpenWithKeyboard(int coins);

	void ResetButtonStatus();

	void UpdateCoins(int new_coins_value) { this->coins = new_coins_value; };

	s3eKey lastKeyPressed;
private:
	Button* btnArcher;
	Button* btnBowler;
	Button* btnMage;
	Button* btnTank;
	Button* btnOK;

	Button* button_ArcherBase;
	Button* button_ArcherPoisoner;
	Button* button_ArcherRetarder;
	Button* button_WarriorBase;
	Button* button_WarriorTank;
	Button* button_WarriorBerserk;
	Button* button_ShamanHealer;
	Button* button_ShamanMight;
	Button* button_ShamanHaste;
	Button* button_BowlerRooter;
	Button* button_BowlerConfuser;
	Button* button_BowlerStuner;

	Button* button_up;
	Button* button_left;
	Button* button_down;
	Button* button_right;

	RenderData archerPoisonerBlock, archerRetarderBlock;
	RenderData warriorOffensiveBlock, warriorDefensiveBlock;
	RenderData shamanMightBlock, shamanHasteBlock;
	RenderData bowlerRooterBlock, bowlerConfusionBlock;

	ButtonRenderData btnArcherBase, btnArcherPoisoner, btnArcherRetarder;
	ButtonRenderData btnWarriorBase, btnWarriorTank, btnWarriorBerserk;
	ButtonRenderData btnShamanHealer, btnShamanMight, btnShamanHaste;
	ButtonRenderData btnBowlerRooter, btnBowlerConfuser, btnBowerStuner;

	ButtonRenderData btnArcherData, btnBowlerData, btnMageData, btnTankData;

	int _x;
	int _y;
	int _padding;
	bool bIsActive;

	int64 delay;
	int64 time;
	int64 currTime;
	int64 lastTime;
	bool bDelay;

	bool bIsClosing;
	bool bFirstOpen;
	int coins;

	bool bOnUnitMenu;

	menu_name menu;

	char button_up_cost[100];
	char button_down_cost[100];
	char button_right_cost[100];
	char button_left_cost[100];
	char button_up_level[10];
	char button_down_level[10];
	char button_right_level[10];
	char button_left_level[10];
	bool bIsKeyPressed;
	int ButtonPressed; //-1: None, 0: Up, 1: Down, 2: Right, 3:Left
	bool bClickOnce;

	ButtonRenderData okRenderData;
};

#endif