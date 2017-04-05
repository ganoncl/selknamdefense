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

#ifndef _UNITMANAGER_H
#define _UNITMANAGER_H

#include "s3e.h"
#include "button.h"
#include "common.h"
#include "mappointer.h"
#include "utils.h"

class CGame;

#define CLOSING_TIME 300

typedef enum
{
	UMB_None,
	UMB_UP,
	UMB_RIGHT,
	UMB_DOWN,
	UMB_LEFT,
} UnitManagerButton;

typedef void (*function)();

class UnitManager
{
public:
	UnitManager();
	~UnitManager();
	void Render();
	void RenderMovementCursor();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void KeyboardHandler(s3eKeyboardEvent* event);
	void SetPos(int x, int y);
	int GetX() { return _x; };
	int GetY() { return _y; };
	void SetPadding(int padding);
	int GetPadding() { return _padding; };

	void SetIsActive(bool bIsActive);
	bool GetIsActive() { return bIsActive; };

	void SetActiveOptions();

	void DisableUPButton() { btnUP->Enabled(false); };
	void DisableRIGHTButton() { btnRIGHT->Enabled(false); };
	void DisableDOWNButton()   { btnDOWN->Enabled(false);   };
	void DisableLEFTButton()   { btnLEFT->Enabled(false);   };

	void EnableUPButton()  { btnUP->Enabled(true);  };
	void EnableRIGHTButton()  { btnRIGHT->Enabled(true);  };
	void EnableDOWNButton()    { btnDOWN->Enabled(true);    };
	void EnableLEFTButton()    { btnLEFT->Enabled(true);    };

	void Open(IsoTerrain* terrain, int* coins);
	void Close();
	void CloseWithDelay(int64 delay, UnitManagerButton unitManagerButton);
	void OpenWithKeyboard(IsoTerrain* terrain, int* coins);

	void UpgradeSelknam(); //Level up
	void SetAvailableUpgrades(UnitUpgrades unitUpgrades);

	void SetAStarHelper(AStarHelper* aStarHelper) { this->aStarHelper = aStarHelper; };
	void SetCursorGrid(IsoTerrain *CursorGrid[MAX_TILESROW][MAX_TILESROW]);
	void SetWorldMapPointer(MapPointer* worldMapPointer) { this->worldMapPointer = worldMapPointer; };

	void MoveSelknam();
	void SellSelknam();

	bool bSearchingNewPosition;

	s3eKey lastKeyPressed;

	CGame *gameRef;
private:
	bool ConfirmMovement();

	Button* btnUP;
	Button* btnRIGHT;
	Button* btnDOWN;
	Button* btnLEFT;
	Button* btnOK;
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

	int* coins;

	//Buttons
	Button* btnUpgrade;
	Button* btnBranch_1;
	Button* btnBranch_2;
	Button* btnSell;
	Button* btnMove;

	IsoSelknam* CurrentSelknam;
	int range, xTile, yTile;

	AStarHelper* aStarHelper;

	MapPointer movePointer;
	bool bRenderMenu;
	
	bool bClose;
	bool bFirstOpen;

	MapPointer* worldMapPointer;

	bool bOnUnitManager;

	char move_cost[10];
	char sell_cost[10];

	bool bIsKeyPressed;
	ButtonRenderData sellButtonRD;
	ButtonRenderData moveButtonRD;
	ButtonRenderData cancelButtonRD;

	//Map pointer (the mouse in the game) is not able to move when Unit Manager is open.
	//When the move button is pressed, this variable is set to true. Then when OK or Enter key is released,
	//the move pointer will be set to active if this variable is true.
	bool bActivePointer;
};

#endif