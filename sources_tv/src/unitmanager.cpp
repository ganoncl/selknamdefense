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

#include "isoselknam.h"
#include "unitmanager.h"
#include "astarhelper.h"
#include "mappointer.h"
#include "game.h"

UnitManager* UnitManagerReference = NULL;

void UnitManager_OK_Action()
{
	UnitManagerReference->Close();
}

void UnitManager_Upgrade_Action()
{
	UnitManagerReference->UpgradeSelknam();
	UnitManagerReference->CloseWithDelay(CLOSING_TIME, UMB_UP);
}

void UnitManager_Sell_Action()
{
	UnitManagerReference->SellSelknam();
	UnitManagerReference->CloseWithDelay(CLOSING_TIME, UMB_RIGHT);
}

void UnitManager_Move_Action()
{
	UnitManagerReference->MoveSelknam();
}

UnitManager::UnitManager()
{
	UnitManagerReference = this;

	TextureData data = TextureHelper::GetTextureNamed("btn_unitmanager");
	sellButtonRD.numStates = 1;
	sellButtonRD.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 4, 0);
	sellButtonRD.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 4, 3);
	sellButtonRD.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 4, 6);
	sellButtonRD.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 4, 9);

	moveButtonRD.numStates = 1;
	moveButtonRD.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 4, 1);
	moveButtonRD.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 4, 4);
	moveButtonRD.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 4, 7);
	moveButtonRD.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 4, 10);

	cancelButtonRD.numStates = 1;
	cancelButtonRD.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 4, 2);
	cancelButtonRD.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 4, 5);
	cancelButtonRD.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 4, 8);
	cancelButtonRD.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 4, 11);

	_x = 0;
	_y = 0;
	_padding = 80;
	
	btnSell     = new Button(TextureHelper::GetCIwTexture(sellButtonRD.state[0].RollOut.atlasName), &sellButtonRD,     GetX() + GetPadding(), GetY());
	btnMove     = new Button(TextureHelper::GetCIwTexture(moveButtonRD.state[0].RollOut.atlasName), &moveButtonRD,    GetX() - GetPadding(), GetY());
	btnOK       = new Button(TextureHelper::GetCIwTexture(cancelButtonRD.state[0].RollOut.atlasName), &cancelButtonRD,      GetX(), GetY());

	btnSell->OnClick(&UnitManager_Sell_Action);
	btnMove->OnClick(&UnitManager_Move_Action);
	btnOK->OnClick(&UnitManager_OK_Action);

	bIsActive  = false;
	bIsClosing = false;
	
	time     = 0;
	bDelay   = false;
	currTime = 0;
	lastTime = 0;
	delay    = 0;

	btnUP     = NULL;
	btnRIGHT  = btnSell;
	btnDOWN   = NULL;
	btnLEFT   = btnMove;

	movePointer.InitIso();
	movePointer.SetIsActive(false);

	bRenderMenu = true;
	bSearchingNewPosition = false;

	bOnUnitManager = true;

	bIsKeyPressed = false;
	bActivePointer = false;
}

UnitManager::~UnitManager()
{
	btnUP = NULL;
	btnRIGHT = NULL;
	btnLEFT = NULL;
	btnDOWN = NULL;

	delete btnOK;
	delete btnSell;
	delete btnMove;
}

void UnitManager::Render()
{
	if(!bIsActive)
		return;

	if(bRenderMenu)
	{
		btnRIGHT->Render();

		IwGxLightingOn();
		Utils::RenderText(sell_cost, CIwRect(btnRIGHT->GetX() + btnRIGHT->GetWidth() - 18, btnRIGHT->GetY() + btnRIGHT->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		Utils::RenderText(sell_cost, CIwRect(btnRIGHT->GetX() + btnRIGHT->GetWidth() - 20, btnRIGHT->GetY() + btnRIGHT->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xff00b300, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		IwGxLightingOff();

		if(btnDOWN != NULL) btnDOWN->Render();

		btnLEFT->Render();
		IwGxLightingOn();
		Utils::RenderText(move_cost, CIwRect(btnLEFT->GetX() + btnLEFT->GetWidth() - 18, btnLEFT->GetY() + btnLEFT->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		Utils::RenderText(move_cost, CIwRect(btnLEFT->GetX() + btnLEFT->GetWidth() - 20, btnLEFT->GetY() + btnLEFT->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		IwGxLightingOff();

		btnOK->Render();
	}
}

void UnitManager::UpdateClickEvents(s3ePointerEvent *event)
{
	if(!bIsActive || bIsClosing)
	{
		return;
	}

	if(event->m_Pressed && bFirstOpen) {
		return;
	}
	else
	{
		bFirstOpen = false;
	}

	if(bRenderMenu)
	{
		if(event->m_Pressed && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE && !bOnUnitManager)
			Close();

		btnRIGHT->HandleClick(event);
		if(btnDOWN != NULL) btnDOWN->HandleClick(event);
		btnLEFT->HandleClick(event);
		btnOK->HandleClick(event);
	}

	if(event->m_Pressed)
	{
		if(event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE && bSearchingNewPosition)
		{
			IsoTerrain* CurrentGrid = movePointer.GetIsoTerrain();
			CIwVec2 Tile = movePointer.GetTileByMouseCoords((float)event->m_x, (float)event->m_y, CurrentGrid->getLayer());
			
			if(Tile.x == CurrentGrid->getXNumTile() && Tile.y == CurrentGrid->getYNumTile())
				ConfirmMovement();
		}
	}
	else
	{
		if(bClose) 
			Close();
	}
}

void UnitManager::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!bIsActive || bIsClosing)
		return;

	if(bActivePointer)
	{
		bActivePointer = false;
		movePointer.SetIsActive(true);
	}

	if(bRenderMenu)
	{
		bOnUnitManager = false;
		//btnUP->HandleMotion(event);		if(btnUP->MouseOn(event->m_x, event->m_y))    bOnUnitManager = true;
		btnRIGHT->HandleMotion(event);  if(btnRIGHT->MouseOn(event->m_x, event->m_y)) bOnUnitManager = true;
		if(btnDOWN != NULL) 
		{
			btnDOWN->HandleMotion(event);
			if(btnRIGHT->MouseOn(event->m_x, event->m_y)) bOnUnitManager = true;
		}
		btnLEFT->HandleMotion(event);  if(btnLEFT->MouseOn(event->m_x, event->m_y)) bOnUnitManager = true;
		btnOK->HandleMotion(event);    if(btnOK->MouseOn(event->m_x, event->m_y)) bOnUnitManager = true;
	}

	movePointer.UpdateMotionEvents(event);
}

void UnitManager::SetPos(int x, int y)
{
	_x = x;
	_y = y;
	//btnUpgrade->SetPos(GetX(), GetY() - GetPadding());
	btnSell->SetPos(GetX() + GetPadding(), GetY());
	btnMove->SetPos(GetX() - GetPadding(), GetY());
	btnOK->SetPos(GetX(), GetY());
}

void UnitManager::SetPadding(int padding)
{
	_padding = padding;
}

void UnitManager::SetIsActive(bool bIsActive) 
{
	this->bIsActive = bIsActive;
}

void UnitManager::KeyboardHandler(s3eKeyboardEvent* event)
{
	if(!bIsActive || bIsClosing)
		return;

	movePointer.KeyboardHandler(event);
	
	if(event->m_Pressed)
	{
		bIsKeyPressed = true;
		lastKeyPressed = event->m_Key;
		if(lastKeyPressed == s3eKeyEnter || lastKeyPressed == s3eKeyAbsOk)
		{
			if(bRenderMenu && btnOK->IsActive()) btnOK->SetStatus(3);
		}
		else if(lastKeyPressed == s3eKeyAbsRight)
		{
			if(bRenderMenu && btnRIGHT->IsActive()) btnRIGHT->SetStatus(3);
		}
		else if(lastKeyPressed == s3eKeyAbsLeft)
		{
			if(bRenderMenu && btnLEFT->IsActive()) btnLEFT->SetStatus(3);
		}
		else if(lastKeyPressed == s3eKeyAbsUp)
		{
			//if(bRenderMenu && btnUP->IsActive()) btnUP->SetStatus(3);
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			if(bRenderMenu && btnDOWN != NULL && btnDOWN->IsActive()) btnDOWN->SetStatus(3);
		}
		else
		{
			if(bRenderMenu)
			{
				if(btnOK->IsActive()) btnOK->SetStatus(2);
				if(btnRIGHT->IsActive()) btnRIGHT->SetStatus(2);
				if(btnLEFT->IsActive()) btnLEFT->SetStatus(2);
				//if(btnUP->IsActive()) btnUP->SetStatus(2);
				if(btnDOWN != NULL && btnDOWN->IsActive()) btnDOWN->SetStatus(2);
			}
		}
	} 
	else
	{
		bIsKeyPressed = false;

		lastKeyPressed = s3eKeyFirst;
	}

	if(bIsKeyPressed)
	{

		if(lastKeyPressed == s3eKeyAbsLeft ||
		   lastKeyPressed == s3eKeyAbsRight ||
		   lastKeyPressed == s3eKeyAbsUp ||
		   lastKeyPressed == s3eKeyAbsDown)
		{
			if(bActivePointer)
			{
				bActivePointer = false;
				movePointer.SetIsActive(true);
			}
		}

		if(lastKeyPressed == s3eKeyEnter || lastKeyPressed == s3eKeyAbsOk)
		{
			if(movePointer.GetIsActive() && bSearchingNewPosition)
			{
				ConfirmMovement();
				bBlockKeyboardOnce = true;
			}
			else
			{
				if(bRenderMenu && btnOK->IsActive())
				{
					btnOK->Execute();
					btnOK->SetStatus(3);
					bBlockKeyboardOnce = true;
				}
			}
		}
		else if(lastKeyPressed == s3eKeyAbsRight)
		{
			if(bRenderMenu && btnRIGHT->IsActive())
			{
				btnRIGHT->Execute();
				btnRIGHT->SetStatus(3);
			}
		}
		else if(lastKeyPressed == s3eKeyAbsLeft)
		{
			if(bRenderMenu && btnLEFT->IsActive())
			{
				btnLEFT->Execute();
				btnLEFT->SetStatus(3);
			}
		}
		else if(lastKeyPressed == s3eKeyAbsUp)
		{
			if(bRenderMenu && btnUP->IsActive())
			{
				//btnUP->Execute();
				//btnUP->SetStatus(3);
			}
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			if(bRenderMenu && btnDOWN != NULL && btnDOWN->IsActive())
			{
				btnDOWN->Execute();
				btnDOWN->SetStatus(3);
			}
		}
		else if(lastKeyPressed == s3eKeyEsc || lastKeyPressed == s3eKeyBack)
		{
			bSearchingNewPosition = false;
			movePointer.SetIsActive(false);
			bRenderMenu = true;
		}
	}
}

void UnitManager::Update()
{	
	if(!bIsActive)
		return;

	if(CurrentSelknam != NULL)
	{
		if(CurrentSelknam->bMove ||
		   CurrentSelknam->GetIsDead())
		{
			Close();
		}
	}
	else
	{
		Close();
	}

	sprintf(move_cost, "%d", CurrentSelknam->GetMoveCost());
	sprintf(sell_cost, "%d", CurrentSelknam->GetSellPrice());

	//IwTrace(APPLOG, ("UnitManager activo..."));
	movePointer.Update();

	if(!bDelay)
		return;
	
	lastTime = currTime;
	currTime = s3eTimerGetMs();
	time += currTime - lastTime;

	if(time >= delay) 
	{
		bDelay = false;
		Close();
	}
}

void UnitManager::Close()
{
	SetIsActive(false);

	for(int i = yTile - range; i <= yTile + range; i++)
	{
		for(int j = xTile - range; j <= xTile + range; j++)
		{
			if(i > 0 && j > 0 && (i != yTile || j != xTile) && i < MAX_TILESROW && j < MAX_TILESROW)
			{
				gameRef->rangeTileMap[j][i] = 0;
			}
		}
	}
}

void UnitManager::Open(IsoTerrain* terrain, int* coins)
{
	this->coins = coins;
	CurrentSelknam = terrain->CurrentIsoSelknam;
	
	if(CurrentSelknam->GetIsDead())
		return;

	movePointer.SetIsActive(true);
	movePointer.SetGrid(terrain);
	movePointer.SetIsActive(false);

	SetActiveOptions();

	SetIsActive(true);
	bIsClosing = false;
	//if(btnUP->IsActive()) btnUP->SetStatus(2);
	if(btnRIGHT->IsActive()) btnRIGHT->SetStatus(2);
	if(btnDOWN != NULL && btnDOWN->IsActive())   btnDOWN->SetStatus(2);
	if(btnLEFT->IsActive())   btnLEFT->SetStatus(2);
	if(btnOK->IsActive())     btnOK->SetStatus(2);

	bSearchingNewPosition = false;
	movePointer.SetIsActive(false);
	bClose = false;
	bRenderMenu = true;
	bFirstOpen = true;

	if(terrain != NULL && CurrentSelknam != NULL)
	{
		range = CurrentSelknam->GetRange();
		xTile = terrain->getXNumTile();
		yTile = terrain->getYNumTile();
		for(int i = yTile - range; i <= yTile + range; i++)
		{
			for(int j = xTile - range; j <= xTile + range; j++)
			{
				if(i > 0 && j > 0 && (i != yTile || j != xTile) && i < MAX_TILESROW && j < MAX_TILESROW)
				{
					gameRef->rangeTileMap[j][i] = -1;
				}
			}
		}
	}
}

void UnitManager::OpenWithKeyboard(IsoTerrain* terrain, int* coins)
{
	Open(terrain, coins);
	bFirstOpen = false;
}

void UnitManager::CloseWithDelay(int64 delay, UnitManagerButton unitManagerButton)
{
	this->delay = delay;
	bDelay = true;
	time = 0;
	lastTime = s3eTimerGetMs();
	currTime = s3eTimerGetMs();

	bIsClosing = true;
	//if(unitManagerButton == UMB_UP    && btnUP->IsActive()) btnUP->SetStatus(3);
	if(unitManagerButton == UMB_RIGHT && btnRIGHT->IsActive()) btnRIGHT->SetStatus(3);
	if(unitManagerButton == UMB_DOWN  && btnDOWN != NULL && btnDOWN->IsActive())  btnDOWN->SetStatus(3);
	if(unitManagerButton == UMB_LEFT  && btnLEFT->IsActive()) btnLEFT->SetStatus(3);
}

void UnitManager::SetActiveOptions() 
{
	//btnUP = btnUpgrade;
	btnDOWN = NULL;
	
	//Set available only those who where acquired on selknam store
	if(CurrentSelknam->GetSelknamType() == SELKNAM_Archer_Base)
		SetAvailableUpgrades(ArcherBaseUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Archer_Poisoner)
		SetAvailableUpgrades(ArcherPoisonerUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Archer_Retarder)
		SetAvailableUpgrades(ArcherRetarderUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Warrior_Base)
		SetAvailableUpgrades(WarriorBaseUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Warrior_Berserk)
		SetAvailableUpgrades(WarriorBerserkUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Warrior_Tank)
		SetAvailableUpgrades(WarriorTankUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Bowler_Confuser)
		SetAvailableUpgrades(BowlerConfuserUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Bowler_Rooter)
		SetAvailableUpgrades(BowlerRooterUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Bowler_Stuner)
		SetAvailableUpgrades(BowlerStunerUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Shaman_Might)
		SetAvailableUpgrades(ShamanMightUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Shaman_Healer)
		SetAvailableUpgrades(ShamanHealerUpgrades);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Shaman_Haste)
		SetAvailableUpgrades(ShamanHasteUpgrades);
	
	btnLEFT->Enabled(CurrentSelknam->GetMoveCost() <= *coins);
}

void UnitManager::SetAvailableUpgrades(UnitUpgrades unitUpgrades)
{
	if(btnUP != NULL)
		btnUP->Enabled(unitUpgrades.level > CurrentSelknam->GetLevel());
}

void UnitManager::UpgradeSelknam()
{
	if(CurrentSelknam->GetSelknamType() == SELKNAM_Archer_Base)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Archer_Base[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Archer_Poisoner)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Archer_Poisoner[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Archer_Retarder)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Archer_Retarder[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Warrior_Base)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Warrior_Base[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Warrior_Berserk)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Warrior_Berserk[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Warrior_Tank)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Warrior_Tank[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Bowler_Confuser)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Bowler_Confuser[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Bowler_Rooter)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Bowler_Rooter[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Bowler_Stuner)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Bowler_Stuner[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Shaman_Haste)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Shaman_Haste[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Shaman_Healer)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Shaman_Healer[CurrentSelknam->GetLevel()]);
	else if(CurrentSelknam->GetSelknamType() == SELKNAM_Shaman_Might)
		CurrentSelknam->SetPawnAtributes(SelknamAtt_Shaman_Might[CurrentSelknam->GetLevel()]);
}

void UnitManager::MoveSelknam()
{
	bActivePointer = true;
	bRenderMenu = false;
	bSearchingNewPosition = true;
	movePointer.SetMovementCursor(true);
	for(int i = yTile - range; i <= yTile + range; i++)
	{
		for(int j = xTile - range; j <= xTile + range; j++)
		{
			if(i > 0 && j > 0 && (i != yTile || j != xTile) && i < MAX_TILESROW && j < MAX_TILESROW)
			{
				gameRef->rangeTileMap[j][i] = 0;
			}
		}
	}
}

void UnitManager::SellSelknam()
{
	*coins += CurrentSelknam->GetSellPrice();
	CurrentSelknam->GetCurrentTile()->CurrentIsoSelknam = NULL;
	CurrentSelknam->GetCurrentTile()->aStar_aditionalCost = 0;
	CurrentSelknam->CleanSpawnTile();
	CurrentSelknam->CleanIso();
}

void UnitManager::RenderMovementCursor()
{
	if(!bIsActive)
		return;

	if(movePointer.GetIsActive())
		movePointer.Render();
}

bool UnitManager::ConfirmMovement()
{
	//Set tile where the isopawn is going to walk
	IsoTerrain* CurrentIsoTerrain = movePointer.GetIsoTerrain();
	if(CurrentIsoTerrain->getCanPutUnit() && CurrentIsoTerrain->CurrentIsoSelknam == NULL)
	{
		bSearchingNewPosition = false;
		CurrentSelknam->MoveToIsoTerrainRecordPath(CurrentIsoTerrain);
		CurrentSelknam->UnitManagerMoveTile = CurrentIsoTerrain;
		movePointer.SetMovementCursor(false);
		CurrentIsoTerrain->setCanPutUnit(false);
		//MapPointer position in CGame must be changed.
		if(worldMapPointer != NULL)
		{
			worldMapPointer->SetIsActive(true);
			worldMapPointer->SetGrid(CurrentIsoTerrain);
			worldMapPointer->SetIsActive(false);
		}
		*coins -= CurrentSelknam->GetMoveCost();
		movePointer.SetIsActive(false);
		Close();
		return true;
	}
	return false;
}

void UnitManager::SetCursorGrid(IsoTerrain *CursorGrid[MAX_TILESROW][MAX_TILESROW]) 
{
	movePointer.SetCursorGrid(CursorGrid);
};