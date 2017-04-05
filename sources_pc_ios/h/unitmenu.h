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
#include "localization.h"
#include "utils.h"
#include "mappointer.h"
#include "Timer.h"
#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#endif

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
	int GetWidth();
	int GetHeight();
	void SetPadding(int padding);
	int GetPadding() { return _padding; };

	void SetArcherBaseButtonFunction(function func) { archerBaseSpawnFnc = func; };//button_ArcherBase->OnClick(func); };

	void SetArcherPoisonerButtonFunction(function func) { archerPoisonerSpawnFnc = func; };//button_ArcherPoisoner->OnClick(func); };
	void SetArcherRetarderButtonFunction(function func) { archerRetarderSpawnFnc = func; };//button_ArcherRetarder->OnClick(func); };

	void SetWarriorBaseButtonFunction(function func)    { warriorBaseSpawnFnc = func; };//button_WarriorBase->OnClick(func);	};
	void SetWarriorBerserkButtonFunction(function func) { warriorBerserkSpawnFnc = func; };//button_WarriorBerserk->OnClick(func); };
	void SetWarriorTankButtonFunction(function func)    { warriorTankSpawnFnc = func; };//button_WarriorTank->OnClick(func); };

	void SetShamanHealerButtonFunction(function func)   { shamanHealerSpawnFnc = func; }; //button_ShamanHealer->OnClick(func); };
	void SetShamanMightButtonFunction(function func)    { shamanMightSpawnFnc = func; }; //button_ShamanMight->OnClick(func); };
	void SetShamanHasteButtonFunction(function func)    { shamanHasteSpawnFnc = func; }; //button_ShamanHaste->OnClick(func); };

	void SetBowlerStunerButtonFunction(function func)   { bowlerStunerSpawnFnc = func; };//button_BowlerStuner->OnClick(func); };
	void SetBowlerConfuserButtonFunction(function func) { bowlerConfuserSpawnFnc = func; }; //button_BowlerConfuser->OnClick(func); };
	void SetBowlerRooterButtonFunction(function func)   { bowlerRooterSpawnFnc = func; }; //button_BowlerRooter->OnClick(func); };

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

	MapPointer* pointerRef;
	int* gameCoinsRef;
	int currCoins;

	bool isValid(int cost)
	{
		return cost > 0 && cost <= *gameCoinsRef;
	}

	s3eKey lastKeyPressed;

	bool bUnitMenuRecentlyClosedByTap;

	Button* GetLeftButton() { return button_left; };
	Button* GetDownButton() { return button_down; };
	Button* GetUpButton() { return button_up; };
	void AllowOnlyButton(Button* buttonToSelect);
	void Block();
	void Unblock();

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
#endif

	void SpawnArcherBase();
	void SpawnArcherRetarder();
	void SpawnArcherPoisoner();
	void SpawnWarriorBase();
	void SpawnWarriorTank();
	void SpawnWarriorBerserk();
	void SpawnShamanMight();
	void SpawnShamanHealer();
	void SpawnShamanHaste();
	void SpawnBowlerRooter();
	void SpawnBowlerConfuser();
	void SpawnBowlerStuner();

	void LoadArcherMenu();
	void LoadWarriorMenu();
	void LoadShamanMenu();
	void LoadBowlerMenu();

	void SpawnSelknam(void (*func)(), Button* btn);

	bool bOpenArcherSubMenu;
	bool bOpenWarriorSubMenu;
	bool bOpenBowlerSubMenu;
	bool bOpenShamanSubMenu;

private:
	void RenderMiniCoin(int x, int y, bool bGrayColor = false);

	void UpdateArcherMenu();
	void UpdateWarriorMenu();
	void UpdateBowlerMenu();
	void UpdateMageMenu();

	Button* btnArcher;
	Button* btnBowler;
	Button* btnMage;
	Button* btnTank;
	Button* btnOK;
	Button* buttonToSelect;

	Timer onPressDuration;
	function currSpawnFunc;
	function archerBaseSpawnFnc;
	function archerPoisonerSpawnFnc;
	function archerRetarderSpawnFnc;
	function warriorTankSpawnFnc;
	function warriorBerserkSpawnFnc;
	function warriorBaseSpawnFnc;
	function shamanHasteSpawnFnc;
	function shamanMightSpawnFnc;
	function shamanHealerSpawnFnc;
	function bowlerStunerSpawnFnc;
	function bowlerConfuserSpawnFnc;
	function bowlerRooterSpawnFnc;

	ButtonRenderData btnArcherData, btnBowlerData, btnMageData, btnTankData;

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

	ButtonRenderData btnArcherBase, btnArcherPoisoner, btnArcherRetarder;
	ButtonRenderData btnWarriorBase, btnWarriorTank, btnWarriorBerserk;
	ButtonRenderData btnShamanHealer, btnShamanMight, btnShamanHaste;
	ButtonRenderData btnBowlerRooter, btnBowlerConfuser, btnBowerStuner;

	Button* button_up;
	Button* button_left;
	Button* button_down;
	Button* button_right;

	RenderData archerPoisonerBlock, archerRetarderBlock;
	RenderData warriorOffensiveBlock, warriorDefensiveBlock;
	RenderData shamanMightBlock, shamanHasteBlock;
	RenderData bowlerRooterBlock, bowlerConfusionBlock;
	RenderData labelGold;

	int _x;
	int _y;
	int _padding;
	int costTextXMargin;
	int miniCoinXMargin;
	int miniCoinYMargin;
	bool bIsActive;
	bool bIsBlocked;
	int64 delay;
	int64 time;
	int64 currTime;
	int64 lastTime;
	bool bDelay;

	bool bIsClosing;
	bool bFirstOpen;
	int coins;

	bool bOnUnitMenu;
	bool bBlockSpawn;

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

	//Prevent unit menu to be opened immediately after is closed.
	Timer blockOpenTimer;

	bool bArcherBaseCost;
	bool bArcherPoisonerCost;
	bool bArcherRetarderCost;
	bool bBowlerConfuserCost;
	bool bBowlerRooterCost;
	bool bBowlerStunerCost;
	bool bShamanHasteCost;
	bool bShamanHealerCost;
	bool bShamanMightCost;
	bool bWarriorBaseCost;
	bool bWarriorBerserkerCost;
	bool bWarriorTankCost;
};

#endif