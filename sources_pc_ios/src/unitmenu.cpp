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

#include "unitmenu.h"
#include "texturehelper.h"

UnitMenu* UnitMenu_Reference = NULL;

void UnitMenu_GoToArcherMenu()
{
	if(UnitMenu_Reference == NULL)
		return;

	UnitMenu_Reference->GoToArcherMenu();
}

void UnitMenu_GoToWarriorMenu()
{
	if(UnitMenu_Reference == NULL)
		return;

	UnitMenu_Reference->GoToWarriorMenu();
}

void UnitMenu_GoToShamanMenu()
{
	if(UnitMenu_Reference == NULL)
		return;

	UnitMenu_Reference->GoToShamanMenu();
}

void UnitMenu_GoToBowlerMenu()
{
	if(UnitMenu_Reference == NULL)
		return;

	UnitMenu_Reference->GoToBowlerMenu();
}

void UnitMenu_SpawnArcherBase()
{
	UnitMenu_Reference->SpawnArcherBase();
}

void UnitMenu_SpawnArcherPoisoner() { return UnitMenu_Reference->SpawnArcherPoisoner(); }
void UnitMenu_SpawnArcherRetarder() { return UnitMenu_Reference->SpawnArcherRetarder(); }
void UnitMenu_SpawnWarriorTank() { return UnitMenu_Reference->SpawnWarriorTank(); }
void UnitMenu_SpawnWarriorBerserk() { return UnitMenu_Reference->SpawnWarriorBerserk(); }
void UnitMenu_SpawnWarriorBase() { return UnitMenu_Reference->SpawnWarriorBase(); }
void UnitMenu_SpawnShamanMight() { return UnitMenu_Reference->SpawnShamanMight(); }
void UnitMenu_SpawnShamanHaste() { return UnitMenu_Reference->SpawnShamanHaste(); }
void UnitMenu_SpawnShamanHealer() { return UnitMenu_Reference->SpawnShamanHealer(); }
void UnitMenu_SpawnBowlerRooter() { return UnitMenu_Reference->SpawnBowlerRooter(); }
void UnitMenu_SpawnBowlerConfuser() { return UnitMenu_Reference->SpawnBowlerConfuser(); }
void UnitMenu_SpawnBowlerStuner() { return UnitMenu_Reference->SpawnBowlerStuner(); }

UnitMenu::UnitMenu()
{
	bBlockSpawn = false;

	UnitMenu_Reference = this;
	pointerRef = NULL;

	float myAltScale = 1.0f;

	bool bRequirescaleOnIos = true;

#ifdef DESKTOPPLATFORM
	bRequirescaleOnIos = false;
#endif

	if(currentGameHeight <= 640 && bRequirescaleOnIos)
	{
		myAltScale = 1.25f;
	}

	_x = 0;
	_y = 0;
	_padding = (int)(80.0f * myAltScale);


	currSpawnFunc = NULL;
	archerBaseSpawnFnc = NULL;
	archerPoisonerSpawnFnc = NULL;
	archerRetarderSpawnFnc = NULL;
	warriorTankSpawnFnc = NULL;
	warriorBerserkSpawnFnc = NULL;
	warriorBaseSpawnFnc = NULL;
	shamanHasteSpawnFnc = NULL;
	shamanMightSpawnFnc = NULL;
	shamanHealerSpawnFnc = NULL;
	bowlerStunerSpawnFnc = NULL;
	bowlerConfuserSpawnFnc = NULL;
	bowlerRooterSpawnFnc = NULL;
	onPressDuration.SetTimeLimit(150);

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_unitmenu_classes");
	btnArcherData.numStates = 1;
	btnArcherData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 0, myAltScale);
	btnArcherData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 4, myAltScale);
	btnArcherData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 8, myAltScale);
	btnArcherData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 12, myAltScale);

	btnBowlerData.numStates = 1;
	btnBowlerData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 1, myAltScale);
	btnBowlerData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 5, myAltScale);
	btnBowlerData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 9, myAltScale);
	btnBowlerData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 13, myAltScale);

	btnMageData.numStates = 1;
	btnMageData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 2, myAltScale);
	btnMageData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 6, myAltScale);
	btnMageData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 10, myAltScale);
	btnMageData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 14, myAltScale);

	btnTankData.numStates = 1;
	btnTankData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 3, myAltScale);
	btnTankData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 7, myAltScale);
	btnTankData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 11, myAltScale);
	btnTankData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 15, myAltScale);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_archers");

	btnArcherBase.numStates = 1;
	btnArcherBase.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0, myAltScale);
	btnArcherBase.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3, myAltScale);
	btnArcherBase.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6, myAltScale);
	btnArcherBase.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9, myAltScale);

	btnArcherPoisoner.numStates = 1;
	btnArcherPoisoner.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2, myAltScale);
	btnArcherPoisoner.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5, myAltScale);
	btnArcherPoisoner.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8, myAltScale);
	btnArcherPoisoner.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11, myAltScale);
	archerPoisonerBlock = TextureHelper::GetRenderData(data, 3, 5, 14, myAltScale);

	btnArcherRetarder.numStates = 1;
	btnArcherRetarder.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1, myAltScale);
	btnArcherRetarder.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4, myAltScale);
	btnArcherRetarder.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7, myAltScale);
	btnArcherRetarder.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10, myAltScale);
	archerRetarderBlock = TextureHelper::GetRenderData(data, 3, 5, 13, myAltScale);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_melees");

	btnWarriorBase.numStates = 1;
	btnWarriorBase.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0, myAltScale);
	btnWarriorBase.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3, myAltScale);
	btnWarriorBase.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6, myAltScale);
	btnWarriorBase.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9, myAltScale);

	btnWarriorTank.numStates = 1;
	btnWarriorTank.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2, myAltScale);
	btnWarriorTank.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5, myAltScale);
	btnWarriorTank.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8, myAltScale);
	btnWarriorTank.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11, myAltScale);
	warriorDefensiveBlock = TextureHelper::GetRenderData(data, 3, 5, 14, myAltScale);

	btnWarriorBerserk.numStates = 1;
	btnWarriorBerserk.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1, myAltScale);
	btnWarriorBerserk.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4, myAltScale);
	btnWarriorBerserk.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7, myAltScale);
	btnWarriorBerserk.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10, myAltScale);
	warriorOffensiveBlock = TextureHelper::GetRenderData(data, 3, 5, 13, myAltScale);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_mages");

	btnShamanHealer.numStates = 1;
	btnShamanHealer.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0, myAltScale);
	btnShamanHealer.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3, myAltScale);
	btnShamanHealer.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6, myAltScale);
	btnShamanHealer.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9, myAltScale);

	btnShamanMight.numStates = 1;
	btnShamanMight.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2, myAltScale);
	btnShamanMight.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5, myAltScale);
	btnShamanMight.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8, myAltScale);
	btnShamanMight.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11, myAltScale);
	shamanMightBlock = TextureHelper::GetRenderData(data, 3, 5, 14, myAltScale);

	btnShamanHaste.numStates = 1;
	btnShamanHaste.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1, myAltScale);
	btnShamanHaste.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4, myAltScale);
	btnShamanHaste.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7, myAltScale);
	btnShamanHaste.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10, myAltScale);
	shamanHasteBlock = TextureHelper::GetRenderData(data, 3, 5, 13, myAltScale);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_bowlers");

	btnBowerStuner.numStates = 1;
	btnBowerStuner.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0, myAltScale);
	btnBowerStuner.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3, myAltScale);
	btnBowerStuner.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6, myAltScale);
	btnBowerStuner.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9, myAltScale);

	btnBowlerConfuser.numStates = 1;
	btnBowlerConfuser.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2, myAltScale);
	btnBowlerConfuser.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5, myAltScale);
	btnBowlerConfuser.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8, myAltScale);
	btnBowlerConfuser.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11, myAltScale);
	bowlerConfusionBlock = TextureHelper::GetRenderData(data, 3, 5, 14, myAltScale);

	btnBowlerRooter.numStates = 1;
	btnBowlerRooter.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1, myAltScale);
	btnBowlerRooter.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4, myAltScale);
	btnBowlerRooter.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7, myAltScale);
	btnBowlerRooter.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10, myAltScale);
	bowlerRooterBlock = TextureHelper::GetRenderData(data, 3, 5, 13, myAltScale);

	TextureData btnOkData = TextureHelper::GetTextureNamed("btn_unitmenu_cancel");
	okRenderData.numStates = 1;
	okRenderData.state[0].RollOut  = TextureHelper::GetRenderData(btnOkData, 1, 4, 0, myAltScale);
	okRenderData.state[0].RollOn   = TextureHelper::GetRenderData(btnOkData, 1, 4, 1, myAltScale);
	okRenderData.state[0].Pressed  = TextureHelper::GetRenderData(btnOkData, 1, 4, 3, myAltScale);
	okRenderData.state[0].Inactive = TextureHelper::GetRenderData(btnOkData, 1, 4, 1, myAltScale);

	btnArcher = new Button(TextureHelper::GetCIwTexture(btnArcherData.state[0].RollOut.atlasName), &btnArcherData, GetX(), GetY() - GetPadding());
	btnBowler = new Button(TextureHelper::GetCIwTexture(btnBowlerData.state[0].RollOut.atlasName), &btnBowlerData, GetX() + GetPadding(), GetY());
	btnMage   = new Button(TextureHelper::GetCIwTexture(btnMageData.state[0].RollOut.atlasName), &btnMageData,   GetX(), GetY() + GetPadding());
	btnTank   = new Button(TextureHelper::GetCIwTexture(btnTankData.state[0].RollOut.atlasName), &btnTankData,   GetX() - GetPadding(), GetY());
	btnOK     = new Button(TextureHelper::GetCIwTexture(okRenderData.state[0].RollOut.atlasName), &okRenderData,  GetX(), GetY());

	btnArcher->OnClick(&UnitMenu_GoToArcherMenu);
	btnBowler->OnClick(&UnitMenu_GoToBowlerMenu);
	btnTank->OnClick(&UnitMenu_GoToWarriorMenu);
	btnMage->OnClick(&UnitMenu_GoToShamanMenu);

	button_ArcherBase      = new Button(TextureHelper::GetCIwTexture(btnArcherBase.state[0].RollOut.atlasName), &btnArcherBase, GetX(), GetY() - GetPadding());
	button_ArcherBase->OnClick(&UnitMenu_SpawnArcherBase);

	button_ArcherPoisoner  = new Button(TextureHelper::GetCIwTexture(btnArcherPoisoner.state[0].RollOut.atlasName), &btnArcherPoisoner, GetX() + GetPadding(), GetY());
	button_ArcherPoisoner->OnClick(&UnitMenu_SpawnArcherPoisoner);
	
	button_ArcherRetarder  = new Button(TextureHelper::GetCIwTexture(btnArcherRetarder.state[0].RollOut.atlasName), &btnArcherRetarder, GetX(), GetY() + GetPadding());
	button_ArcherRetarder->OnClick(&UnitMenu_SpawnArcherRetarder);

	button_WarriorBase     = new Button(TextureHelper::GetCIwTexture(btnWarriorBase.state[0].RollOut.atlasName), &btnWarriorBase, GetX(), GetY() - GetPadding());
	button_WarriorBase->OnClick(&UnitMenu_SpawnWarriorBase);
	
	button_WarriorTank     = new Button(TextureHelper::GetCIwTexture(btnWarriorTank.state[0].RollOut.atlasName), &btnWarriorTank, GetX() - GetPadding(), GetY());
	button_WarriorTank->OnClick(&UnitMenu_SpawnWarriorTank);
	
	button_WarriorBerserk  = new Button(TextureHelper::GetCIwTexture(btnWarriorBerserk.state[0].RollOut.atlasName), &btnWarriorBerserk, GetX(), GetY() + GetPadding());
	button_WarriorBerserk->OnClick(&UnitMenu_SpawnWarriorBerserk);

	button_ShamanHealer    = new Button(TextureHelper::GetCIwTexture(btnShamanHealer.state[0].RollOut.atlasName), &btnShamanHealer, GetX() - GetPadding(), GetY());
	button_ShamanHealer->OnClick(&UnitMenu_SpawnShamanHealer);

	button_ShamanMight     = new Button(TextureHelper::GetCIwTexture(btnShamanMight.state[0].RollOut.atlasName), &btnShamanMight,  GetX() + GetPadding(), GetY());
	button_ShamanMight->OnClick(&UnitMenu_SpawnShamanMight);

	button_ShamanHaste     = new Button(TextureHelper::GetCIwTexture(btnShamanHaste.state[0].RollOut.atlasName), &btnShamanHaste, GetX(), GetY() + GetPadding());
	button_ShamanHaste->OnClick(&UnitMenu_SpawnShamanHaste);

	button_BowlerConfuser  = new Button(TextureHelper::GetCIwTexture(btnBowlerRooter.state[0].RollOut.atlasName), &btnBowlerRooter, GetX(), GetY() - GetPadding());
	button_BowlerConfuser->OnClick(&UnitMenu_SpawnBowlerConfuser);
	
	button_BowlerRooter    = new Button(TextureHelper::GetCIwTexture(btnBowlerConfuser.state[0].RollOut.atlasName), &btnBowlerConfuser, GetX() - GetPadding(), GetY());
	button_BowlerRooter->OnClick(&UnitMenu_SpawnBowlerRooter);

	button_BowlerStuner    = new Button(TextureHelper::GetCIwTexture(btnBowerStuner.state[0].RollOut.atlasName), &btnBowerStuner, GetX() + GetPadding(), GetY());
	button_BowlerStuner->OnClick(&UnitMenu_SpawnBowlerStuner);

	labelGold = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_gold_small"));
	float labelGoldScale = 1.0f;
	labelGold.width  = (int)(labelGold.width * labelGoldScale);
	labelGold.height = (int)(labelGold.height * labelGoldScale);

	bIsActive = false;
	bIsClosing = false;
	
	time = 0;
	bDelay = false;
	currTime = 0;
	lastTime = 0;
	delay = 0;

	button_up = NULL;
	button_down = NULL;
	button_right = NULL;
	button_left = NULL;
	buttonToSelect = NULL;

	bOnUnitMenu = false;

	menu = MENU_None;
	bIsKeyPressed = false;
	bClickOnce = true;

	bUnitMenuRecentlyClosedByTap = false;

	blockOpenTimer.SetTimeLimit(150);
	blockOpenTimer.Stop();

	costTextXMargin = 7;
	miniCoinXMargin = -9;
	miniCoinYMargin = 2;
	bIsBlocked = false;
}

UnitMenu::~UnitMenu()
{
	delete btnArcher;
	delete btnBowler;
	delete btnMage;
	delete btnTank;
	delete btnOK;
	delete button_ArcherBase;
	delete button_ArcherPoisoner;
	delete button_ArcherRetarder;
	delete button_WarriorBase;
	delete button_WarriorTank;
	delete button_WarriorBerserk;
	delete button_ShamanHealer;
	delete button_ShamanMight;
	delete button_ShamanHaste;
	delete button_BowlerRooter;
	delete button_BowlerConfuser;
	delete button_BowlerStuner;
}

void UnitMenu::RenderMiniCoin(int x, int y, bool bGrayColor)
{
	int r, g, b;
	r = g = b = 255;

	if(bGrayColor)
	{
		r = 66;
		g = 66;
		b = 66;
	}

	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(labelGold.atlasName), labelGold, CIwSVec2(x + miniCoinXMargin, y + miniCoinYMargin), r, g, b);
}

void UnitMenu::Render()
{
	if(!bIsActive)
		return;


	if(menu != MENU_None && bIsClosing)
	{
		if(ButtonPressed == 0) button_up->SetStatus(3);
		else if(ButtonPressed == 1) button_down->SetStatus(3);
		else if(ButtonPressed == 2) button_right->SetStatus(3);
		else if(ButtonPressed == 3) button_left->SetStatus(3);
	}

	bool bNormalRender;
	bool bCost = true;

	if(button_up != NULL)
	{
		bNormalRender = true;

		if(menu == MENU_Tank)
		{
			if(WarriorTankUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(warriorDefensiveBlock.atlasName), CIwSVec2(button_up->GetX(), button_up->GetY()), warriorDefensiveBlock);
				bNormalRender = false;
			}

			bCost = bWarriorTankCost;
		}
		else if(menu == MENU_Bowler)
		{
			if(BowlerRooterUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(bowlerRooterBlock.atlasName), CIwSVec2(button_up->GetX(), button_up->GetY()), bowlerRooterBlock);
				bNormalRender = false;
			}

			bCost = bBowlerRooterCost;
		}
		else if(menu == MENU_Archer)
		{
			bCost = bArcherBaseCost;
		}

		if(bNormalRender)
		{
			button_up->Render();
			if(menu != MENU_None)
			{
				RenderMiniCoin(button_up->GetX() + button_up->GetWidth() - 18 + miniCoinXMargin, button_up->GetY() + button_up->GetHeight() - 18, !bCost);
				
				IwGxLightingOn();
				Utils::RenderText(button_up_cost, CIwRect(button_up->GetX() + button_up->GetWidth() - 18 + costTextXMargin, button_up->GetY() + button_up->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_up_cost, CIwRect(button_up->GetX() + button_up->GetWidth() - 20 + costTextXMargin, button_up->GetY() + button_up->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_up_level, CIwRect(button_up->GetX() - 11, button_up->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_up_level, CIwRect(button_up->GetX() - 13, button_up->GetY() - 13, 200, 200), font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				IwGxLightingOff();
			}
		}
	}

	if(button_down != NULL)
	{
		bNormalRender = true;

		if(menu == MENU_Bowler)
		{
			if(BowlerConfuserUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(bowlerConfusionBlock.atlasName), CIwSVec2(button_down->GetX(), button_down->GetY()), bowlerConfusionBlock);
				bNormalRender = false;
			}

			bCost = bBowlerConfuserCost;
		}
		else if(menu == MENU_Tank)
		{
			if(WarriorBerserkUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(warriorOffensiveBlock.atlasName), CIwSVec2(button_down->GetX(), button_down->GetY()), warriorOffensiveBlock);
				bNormalRender = false;
			}

			bCost = bWarriorBerserkerCost;
		}
		else if(menu == MENU_Mage)
		{
			bCost = bShamanHealerCost;
		}

		if(bNormalRender)
		{
			button_down->Render();
			if(menu != MENU_None)
			{
				RenderMiniCoin(button_down->GetX() + button_down->GetWidth() - 18 + miniCoinXMargin, button_down->GetY() + button_down->GetHeight() - 18, !bCost);

				IwGxLightingOn();
				Utils::RenderText(button_down_cost, CIwRect(button_down->GetX() + button_down->GetWidth() - 18 + costTextXMargin, button_down->GetY() + button_down->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_down_cost, CIwRect(button_down->GetX() + button_down->GetWidth() - 20 + costTextXMargin, button_down->GetY() + button_down->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_down_level, CIwRect(button_down->GetX() - 11, button_down->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_down_level, CIwRect(button_down->GetX() - 13, button_down->GetY() - 13, 200, 200), font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				IwGxLightingOff();
			}
		}
	}

	if(button_right != NULL)
	{
		bNormalRender = true;

		if(menu == MENU_Archer)
		{
			if(ArcherPoisonerUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(archerPoisonerBlock.atlasName), CIwSVec2(button_right->GetX(), button_right->GetY()), archerPoisonerBlock);
				bNormalRender = false;
			}

			bCost = bArcherPoisonerCost;
		}
		else if(menu == MENU_Mage)
		{
			if(ShamanMightUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(shamanMightBlock.atlasName), CIwSVec2(button_right->GetX(), button_right->GetY()), shamanMightBlock);
				bNormalRender = false;
			}

			bCost = bShamanMightCost;
		}
		else if(menu == MENU_Bowler)
		{
			bCost = bBowlerStunerCost;
		}

		if(bNormalRender)
		{
			button_right->Render();
			if(menu != MENU_None)
			{
				RenderMiniCoin(button_right->GetX() + button_right->GetWidth() - 18 + miniCoinXMargin, button_right->GetY() + button_right->GetHeight() - 18, !bCost);

				IwGxLightingOn();
				Utils::RenderText(button_right_cost, CIwRect(button_right->GetX() + button_right->GetWidth() - 18 + costTextXMargin, button_right->GetY() + button_right->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_right_cost, CIwRect(button_right->GetX() + button_right->GetWidth() - 20 + costTextXMargin, button_right->GetY() + button_right->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_right_level, CIwRect(button_right->GetX() - 11, button_right->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_right_level, CIwRect(button_right->GetX() - 13, button_right->GetY() - 13, 200, 200), font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				IwGxLightingOff();
			}
		}
	}

	if(button_left != NULL)
	{
		bNormalRender = true;

		if(menu == MENU_Archer)
		{
			if(ArcherRetarderUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(archerRetarderBlock.atlasName), CIwSVec2(button_left->GetX(), button_left->GetY()), archerRetarderBlock);
				bNormalRender = false;
			}

			bCost = bArcherRetarderCost;
		}
		else if(menu == MENU_Mage)
		{
			if(ShamanHasteUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(shamanHasteBlock.atlasName), CIwSVec2(button_left->GetX(), button_left->GetY()), shamanHasteBlock);
				bNormalRender = false;
			}

			bCost = bShamanHasteCost;
		}
		else if(menu == MENU_Tank)
		{
			bCost = bWarriorBaseCost;
		}

		if(bNormalRender)
		{
			button_left->Render();
			if(menu != MENU_None)
			{
				RenderMiniCoin(button_left->GetX() + button_left->GetWidth() - 18 + miniCoinXMargin, button_left->GetY() + button_left->GetHeight() - 18, !bCost);

				IwGxLightingOn();
				Utils::RenderText(button_left_cost, CIwRect(button_left->GetX() + button_left->GetWidth() - 18 + costTextXMargin, button_left->GetY() + button_left->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_left_cost, CIwRect(button_left->GetX() + button_left->GetWidth() - 20 + costTextXMargin, button_left->GetY() + button_left->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_left_level, CIwRect(button_left->GetX() - 11, button_left->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_left_level, CIwRect(button_left->GetX() - 13, button_left->GetY() - 13, 200, 200), font_komikadisplay14, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				IwGxLightingOff();
			}
		}
	}
	
	btnOK->Render();
}

void UnitMenu::UpdateClickEvents(s3ePointerEvent *event)
{
	if(!bIsActive || bIsClosing)
	{
		return;
	}

	bOnUnitMenu = false;

	if(button_up != NULL)
	{
		if(button_up->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}

	if(button_down != NULL)
	{
		if(button_down->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}

	if(button_left != NULL)
	{
		if(button_left->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}

	if(button_right != NULL)
	{
		if(button_right->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}

	if(btnOK->MouseOn(event->m_x, event->m_y))
	{
		bOnUnitMenu = true;
	}

	if(event->m_Pressed && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE && !bOnUnitMenu && buttonToSelect == NULL)
	{
		bUnitMenuRecentlyClosedByTap = true;
		Close();
	}

	if(button_up != NULL && (buttonToSelect == NULL || buttonToSelect == button_up))
		button_up->HandleClick(event);

	if(button_down != NULL && (buttonToSelect == NULL || buttonToSelect == button_down))
		button_down->HandleClick(event);

	if(button_left != NULL && (buttonToSelect == NULL || buttonToSelect == button_left))
		button_left->HandleClick(event);

	if(button_right != NULL && (buttonToSelect == NULL || buttonToSelect == button_right))
	{
		button_right->HandleClick(event);
	}

	if(buttonToSelect == NULL || buttonToSelect == btnOK)
	{
		btnOK->HandleClick(event);
	}

	//bClickOnce = false;
}

void UnitMenu::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!bIsActive || bIsClosing)
		return;

	bOnUnitMenu = false;

	if(button_up != NULL)
	{
		button_up->HandleMotion(event);
		if(button_up->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}

	if(button_down != NULL)
	{
		button_down->HandleMotion(event);
		if(button_down->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}
	
	if(button_left != NULL)
	{
		button_left->HandleMotion(event);
		if(button_left->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}
	
	if(button_right != NULL)
	{
		button_right->HandleMotion(event);
		if(button_right->MouseOn(event->m_x, event->m_y))
		{
			bOnUnitMenu = true;
		}
	}
	
	btnOK->HandleMotion(event);
	if(btnOK->MouseOn(event->m_x, event->m_y))
	{
		bOnUnitMenu = true;
	}
}

void UnitMenu::SetPos(int x, int y)
{
	_x = x;
	_y = y;
	
	btnArcher->SetPos(GetX(), GetY() - GetPadding());
	btnBowler->SetPos(GetX() + GetPadding(), GetY());
	btnMage->SetPos(GetX(), GetY() + GetPadding());
	btnTank->SetPos(GetX() - GetPadding(), GetY());
	btnOK->SetPos(GetX(), GetY());
	
	button_ArcherBase->SetPos(GetX(), GetY() - GetPadding());
	button_ArcherPoisoner->SetPos(GetX() + GetPadding(), GetY());
	button_ArcherRetarder->SetPos(GetX() - GetPadding(), GetY());
	
	button_WarriorBase->SetPos(GetX() - GetPadding(), GetY());
	button_WarriorTank->SetPos(GetX(), GetY() - GetPadding());
	button_WarriorBerserk->SetPos(GetX(), GetY() + GetPadding());
	
	button_ShamanHealer->SetPos(GetX(), GetY() + GetPadding());
	button_ShamanMight->SetPos(GetX() + GetPadding(), GetY());
	button_ShamanHaste->SetPos(GetX() - GetPadding(), GetY());
	
	button_BowlerRooter->SetPos(GetX(), GetY() - GetPadding());
	button_BowlerConfuser->SetPos(GetX(), GetY() + GetPadding());
	button_BowlerStuner->SetPos(GetX() + GetPadding(), GetY());
}

void UnitMenu::SetPadding(int padding)
{
	_padding = padding;
}

void UnitMenu::SetIsActive(bool bIsActive) 
{
	this->bIsActive = bIsActive;
}

void UnitMenu::Update()
{
	blockOpenTimer.Update();

#if defined DESKTOPPLATFORM
	if(bIsActive)
	{
		xbox360Controller.Update();
	}
#endif

	if(!bIsActive)
		return;

	onPressDuration.Update();
	if(onPressDuration.GetIsEnd())
	{
		if(currSpawnFunc != NULL)
		{
			currSpawnFunc();
			currSpawnFunc = NULL;
		}

		if(bOpenArcherSubMenu) LoadArcherMenu();
		else if(bOpenBowlerSubMenu) LoadBowlerMenu();
		else if(bOpenShamanSubMenu) LoadShamanMenu();
		else if(bOpenWarriorSubMenu) LoadWarriorMenu();

		bOpenArcherSubMenu = bOpenBowlerSubMenu = bOpenShamanSubMenu = bOpenWarriorSubMenu = false;
	}
	else if(currCoins != *gameCoinsRef && !bIsClosing)
	{
		currCoins = *gameCoinsRef;
		if(menu == MENU_Archer)      UpdateArcherMenu();
		else if(menu == MENU_Mage)   UpdateMageMenu();
		else if(menu == MENU_Bowler) UpdateBowlerMenu();
		else if(menu == MENU_Tank)   UpdateWarriorMenu();
	}

	if(!bIsActive || !bDelay)
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

void UnitMenu::Close()
{
	if(buttonToSelect != NULL && buttonToSelect != btnOK)
	{
		return;
	}

	SetIsActive(false);
	menu = MENU_None;
	bBlockMousePointer = true;

	if(pointerRef != NULL)
	{
		pointerRef->ShowCursor(450);
	}

	blockOpenTimer.Reset();
	blockOpenTimer.Start();

}

void UnitMenu::Open(int coins)
{
	if(bIsBlocked) return;

	if(!blockOpenTimer.GetIsEnd() && !blockOpenTimer.GetIsStop())
	{
		return;
	}

	bBlockSpawn = false;

	this->coins = coins;
	SetActiveOptions();

	SetIsActive(true);
	bIsClosing = false;

	button_up    = btnArcher;
	button_down  = btnMage;
	button_right = btnBowler;
	button_left  = btnTank;

	if(button_up != NULL && button_up->IsActive())
		button_up->SetStatus(2);
	if(button_down != NULL && button_down->IsActive())
		button_down->SetStatus(2);
	if(button_right != NULL && button_right->IsActive())
		button_right->SetStatus(2);
	if(button_left != NULL && button_left->IsActive())
		button_left->SetStatus(2);

	if(btnOK->IsActive())
		btnOK->SetStatus(2);

	if(button_ArcherBase->IsActive())
		button_ArcherBase->SetStatus(2);
	if(button_ArcherPoisoner->IsActive())
		button_ArcherPoisoner->SetStatus(2);
	if(button_ArcherRetarder->IsActive())
		button_ArcherRetarder->SetStatus(2);

	if(button_WarriorBase->IsActive())
		button_WarriorBase->SetStatus(2);
	if(button_WarriorBerserk->IsActive())
		button_WarriorBerserk->SetStatus(2);
	if(button_WarriorTank->IsActive())
		button_WarriorTank->SetStatus(2);

	if(button_ShamanHaste->IsActive())
		button_ShamanHaste->SetStatus(2);
	if(button_ShamanHealer->IsActive())
		button_ShamanHealer->SetStatus(2);
	if(button_ShamanMight->IsActive())
		button_ShamanMight->SetStatus(2);

	if(button_BowlerConfuser->IsActive())
		button_BowlerConfuser->SetStatus(2);
	if(button_BowlerRooter->IsActive())
		button_BowlerRooter->SetStatus(2);
	if(button_BowlerStuner->IsActive())
		button_BowlerStuner->SetStatus(2);

	bFirstOpen = true;
	ButtonPressed = -1;
	currCoins = *gameCoinsRef;

	bOpenShamanSubMenu = false;
	bOpenBowlerSubMenu = false;
	bOpenArcherSubMenu = false;
	bOpenWarriorSubMenu = false;

	if(tutorial->currBreakpoint == Tutorial_BreakPoint_OpenUnitMenu)
	{
		tutorial->ExecuteBreakpoint();
	}

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Left,  &UnitMenu_GoToWarriorMenu);
	xbox360Controller.SetCallback(Xbox360_Up,    &UnitMenu_GoToArcherMenu);
	xbox360Controller.SetCallback(Xbox360_Down,  &UnitMenu_GoToShamanMenu);
	xbox360Controller.SetCallback(Xbox360_Right, &UnitMenu_GoToBowlerMenu);
	xbox360Controller.SetCallback(Xbox360_Cancel1, btnOK->GetOnClickFunc());
	xbox360Controller.SetCallback(Xbox360_Cancel2, btnOK->GetOnClickFunc());
#endif
}

void UnitMenu::OpenWithKeyboard(int coins)
{
	if(!blockOpenTimer.GetIsEnd() && !blockOpenTimer.GetIsStop())
	{
		return;
	}

	Open(coins);
	bFirstOpen = false;
}

void UnitMenu::CloseWithDelay(int64 delay, UnitMenuButton unitMenuButton)
{
	this->delay = delay;
	bDelay = true;
	time = 0;
	lastTime = s3eTimerGetMs();
	currTime = s3eTimerGetMs();

	bIsClosing = true;
	
	if(unitMenuButton == UMB_ARCHER && btnArcher->IsActive()) btnArcher->SetStatus(3);
	if(unitMenuButton == UMB_BOWLER && btnBowler->IsActive()) btnBowler->SetStatus(3);
	if(unitMenuButton == UMB_MAGE   && btnMage->IsActive())   btnMage->SetStatus(3);
	if(unitMenuButton == UMB_TANK   && btnTank->IsActive())    btnTank->SetStatus(3);
}

void UnitMenu::SetActiveOptions() 
{
	button_ArcherBase->Enabled(bArcherBaseCost = ArcherBaseUpgrades.level > 0 && SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].cost <= coins);
	button_ArcherPoisoner->Enabled(bArcherPoisonerCost = ArcherPoisonerUpgrades.level > 0 && SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].cost <= coins);
	button_ArcherRetarder->Enabled(bArcherRetarderCost = ArcherRetarderUpgrades.level > 0 && SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].cost <= coins);

	button_BowlerConfuser->Enabled(bBowlerConfuserCost = BowlerConfuserUpgrades.level > 0 && SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].cost <= coins);
	button_BowlerRooter->Enabled(bBowlerRooterCost = BowlerRooterUpgrades.level > 0 && SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].cost <= coins);
	button_BowlerStuner->Enabled(bBowlerStunerCost = BowlerStunerUpgrades.level > 0 && SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].cost <= coins);
	
	button_ShamanHaste->Enabled(bShamanHasteCost = ShamanHasteUpgrades.level > 0 && SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].cost <= coins);
	button_ShamanHealer->Enabled(bShamanHealerCost = ShamanHealerUpgrades.level > 0 && SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].cost <= coins);
	button_ShamanMight->Enabled(bShamanMightCost = ShamanMightUpgrades.level > 0 && SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].cost <= coins);

	button_WarriorBase->Enabled(bWarriorBaseCost = WarriorBaseUpgrades.level > 0 && SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].cost <= coins);
	button_WarriorBerserk->Enabled(bWarriorBerserkerCost = WarriorBerserkUpgrades.level > 0 && SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].cost <= coins);
	button_WarriorTank->Enabled(bWarriorTankCost = WarriorTankUpgrades.level > 0 && SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].cost <= coins);
}

void UnitMenu::GoToArcherMenu()
{
	if(buttonToSelect != NULL && buttonToSelect != button_up)
	{
		return;
	}

	if(activeControllerType == Controller_MOUSE)
	{
		LoadArcherMenu();
	}
	else
	{
		bOpenArcherSubMenu = true;
		onPressDuration.Start();
		button_up->SetStatus(3);
	}
}

void UnitMenu::GoToWarriorMenu()
{
	if(buttonToSelect != NULL && buttonToSelect != button_left)
	{
		return;
	}

	if(activeControllerType == Controller_MOUSE)
	{
		LoadWarriorMenu();
	}
	else
	{
		bOpenWarriorSubMenu = true;
		onPressDuration.Start();
		button_left->SetStatus(3);
	}
}

void UnitMenu::UpdateArcherMenu()
{
	int archerBaseCost = 0;
	int archerRetarderCost = 0;
	int archerPoisonerCost = 0;

	if(ArcherBaseUpgrades.level != 0)
	{
		sprintf(button_up_cost, "%d",    SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].cost);
		sprintf(button_up_level, "Lv.%d",    SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].level);

		archerBaseCost = SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].cost;
	}

	if(ArcherRetarderUpgrades.level != 0)
	{
		sprintf(button_left_cost, "%d",  SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].cost);
		sprintf(button_left_level, "Lv.%d",  SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].level);

		archerRetarderCost = SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].cost;
	}

	sprintf(button_down_cost, "%d",  0);
	sprintf(button_down_level, "%d",  0);

	if(ArcherPoisonerUpgrades.level != 0)
	{
		sprintf(button_right_cost, "%d", SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].cost);
		sprintf(button_right_level, "Lv.%d", SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].level);

		archerPoisonerCost = SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].cost;
	}

#if defined DESKTOPPLATFORM
	if(isValid(archerBaseCost)) 
		xbox360Controller.SetCallback(Xbox360_Up, button_up->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Up, NULL);

	if(isValid(archerPoisonerCost))
		xbox360Controller.SetCallback(Xbox360_Right, button_right->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Right, NULL);

	if(isValid(archerRetarderCost))
		xbox360Controller.SetCallback(Xbox360_Left, button_left->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Left, NULL);

	xbox360Controller.SetCallback(Xbox360_Down, NULL);
#endif
}

void UnitMenu::UpdateWarriorMenu()
{
	int warriorTankCost = 0;
	int warriorBaseCost = 0;
	int warriorBerserkCost = 0;

	if(WarriorTankUpgrades.level != 0)
	{
		sprintf(button_up_cost, "%d",        SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].cost);
		sprintf(button_up_level, "Lv.%d",    SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].level);

		warriorTankCost = SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].cost;
	}

	if(WarriorBaseUpgrades.level != 0)
	{
		sprintf(button_left_cost, "%d",  SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].cost);
		sprintf(button_left_level, "Lv.%d",  SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].level);

		warriorBaseCost =  SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].cost;
	}

	if(WarriorBerserkUpgrades.level != 0)
	{
		sprintf(button_down_cost, "%d",  SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].cost);
		sprintf(button_down_level, "Lv.%d",  SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].level);

		warriorBerserkCost = SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].cost;
	}

	sprintf(button_right_cost, "%d", 0);

#if defined DESKTOPPLATFORM
	if(warriorTankCost > 0 && warriorTankCost <= *gameCoinsRef) 
		xbox360Controller.SetCallback(Xbox360_Up, button_up->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Up, NULL);

	xbox360Controller.SetCallback(Xbox360_Right, NULL);

	if(warriorBaseCost > 0 && warriorBaseCost <= *gameCoinsRef) 
		xbox360Controller.SetCallback(Xbox360_Left, button_left->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Left, NULL);

	if(warriorBerserkCost > 0 && warriorBerserkCost <= *gameCoinsRef) 
		xbox360Controller.SetCallback(Xbox360_Down, button_down->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Down, NULL);
#endif
}

void UnitMenu::UpdateBowlerMenu()
{
	int bowlerRooterCost = 0;
	int bowlerConfuserCost = 0;
	int bowlerStunerCost = 0;

	if(BowlerRooterUpgrades.level != 0)
	{
		sprintf(button_up_cost, "%d",    SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].cost);
		sprintf(button_up_level, "Lv.%d",    SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].level);

		bowlerRooterCost = SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].cost;
	}

	sprintf(button_left_cost, "%d",  0);

	if(BowlerConfuserUpgrades.level != 0)
	{
		sprintf(button_down_cost, "%d",  SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].cost);
		sprintf(button_down_level, "Lv.%d",  SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].level);

		bowlerConfuserCost = SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].cost;
	}

	if(BowlerStunerUpgrades.level != 0)
	{
		sprintf(button_right_cost, "%d", SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].cost);
		sprintf(button_right_level, "Lv.%d", SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].level);

		bowlerStunerCost = SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].cost;
	}

#if defined DESKTOPPLATFORM
	if(isValid(bowlerRooterCost)) 
		xbox360Controller.SetCallback(Xbox360_Up, button_up->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Up, NULL);

	if(isValid(bowlerStunerCost))
		xbox360Controller.SetCallback(Xbox360_Right, button_right->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Right, NULL);

	xbox360Controller.SetCallback(Xbox360_Left, NULL);

	if(isValid(bowlerConfuserCost))
		xbox360Controller.SetCallback(Xbox360_Down, button_down->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Down, NULL);
#endif
}

void UnitMenu::UpdateMageMenu()
{
	int shamanHasteCost = 0;
	int shamanHealerCost = 0;
	int shamanMightCost = 0;

	sprintf(button_up_cost, "%d",    0);

	if(ShamanHasteUpgrades.level != 0)
	{
		sprintf(button_left_cost, "%d",  SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].cost);
		sprintf(button_left_level, "Lv.%d",  SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].level);

		shamanHasteCost = SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].cost;
	}

	if(ShamanHealerUpgrades.level != 0)
	{
		sprintf(button_down_cost, "%d",  SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].cost);
		sprintf(button_down_level, "Lv.%d",  SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].level);

		shamanHealerCost = SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].cost;
	}

	if(ShamanMightUpgrades.level != 0)
	{
		sprintf(button_right_cost, "%d", SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].cost);
		sprintf(button_right_level, "Lv.%d", SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].level);

		shamanMightCost = SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].cost;
	}

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Up, NULL);

	if(isValid(shamanMightCost))
		xbox360Controller.SetCallback(Xbox360_Right, button_right->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Right, NULL);

	if(isValid(shamanHasteCost))
		xbox360Controller.SetCallback(Xbox360_Left, button_left->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Left, NULL);

	if(isValid(shamanHealerCost))
		xbox360Controller.SetCallback(Xbox360_Down, button_down->GetOnClickFunc());
	else
		xbox360Controller.SetCallback(Xbox360_Down, NULL);

#endif
}

void UnitMenu::GoToShamanMenu()
{
	if(buttonToSelect != NULL && buttonToSelect != button_down)
	{
		return;
	}

	if(activeControllerType == Controller_MOUSE)
	{
		LoadShamanMenu();
	}
	else
	{
		bOpenShamanSubMenu = true;
		onPressDuration.Start();
		button_down->SetStatus(3);
	}
}

void UnitMenu::LoadArcherMenu()
{
	menu = MENU_Archer;

	button_up = button_ArcherBase;
	button_left = button_ArcherRetarder;
	button_down = NULL;
	button_right = button_ArcherPoisoner;

	UpdateArcherMenu();

	if(tutorial->currBreakpoint == Tutorial_BreakPoint_SelectArcher)
	{
		tutorial->ExecuteBreakpoint();
	}
}

void UnitMenu::LoadWarriorMenu()
{
	menu = MENU_Tank;

	button_up = button_WarriorTank;
	button_left = button_WarriorBase;
	button_down = button_WarriorBerserk;
	button_right = NULL;

	UpdateWarriorMenu();

	if(tutorial->currBreakpoint == Tutorial_BreakPoint_SelectWarrior)
	{
		tutorial->ExecuteBreakpoint();
	}
}

void UnitMenu::LoadShamanMenu()
{
	menu = MENU_Mage;

	button_up = NULL;
	button_left = button_ShamanHaste;
	button_down = button_ShamanHealer;
	button_right = button_ShamanMight;

	UpdateMageMenu();

	if(tutorial->currBreakpoint == Tutorial_BreakPoint_SelectMage)
	{
		tutorial->ExecuteBreakpoint();
	}
}

void UnitMenu::LoadBowlerMenu()
{
	menu = MENU_Bowler;

	button_up = button_BowlerRooter;
	button_left = NULL;
	button_down = button_BowlerConfuser;
	button_right = button_BowlerStuner;

	UpdateBowlerMenu();
}

void UnitMenu::GoToBowlerMenu()
{
	if(buttonToSelect != NULL && buttonToSelect != button_right)
	{
		return;
	}

	if(activeControllerType == Controller_MOUSE)
	{
		LoadBowlerMenu();
	}
	else
	{
		bOpenBowlerSubMenu = true;
		onPressDuration.Start();
		button_right->SetStatus(3);
	}
}

void UnitMenu::KeyboardHandler(s3eKeyboardEvent* event)
{
	if(!bIsActive || bIsClosing)
		return;

	if(event->m_Pressed)
	{
		bIsKeyPressed = true;
		lastKeyPressed = event->m_Key;
		if(lastKeyPressed == s3eKeyEnter || lastKeyPressed == s3eKeyAbsOk)
		{
			if(btnOK->IsActive()) btnOK->SetStatus(3);
		}
		else if(lastKeyPressed == s3eKeyAbsRight)
		{
			if(button_right != NULL && button_right->IsActive())
				button_right->SetStatus(1);
		}
		else if(lastKeyPressed == s3eKeyAbsLeft)
		{
			if(button_left != NULL && button_left->IsActive())
				button_left->SetStatus(1);
		}
		else if(lastKeyPressed == s3eKeyAbsUp)
		{
			if(button_up != NULL && button_up->IsActive())
				button_up->SetStatus(1);
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			if(button_down != NULL && button_down->IsActive())
				button_down->SetStatus(1);
		}
		else
		{
			if(btnOK->IsActive()) 
				btnOK->SetStatus(2);
			
			if(button_right != NULL && button_right->IsActive()) 
				button_right->SetStatus(2);
			
			if(button_left != NULL && button_left->IsActive()) 
				button_left->SetStatus(2);
			
			if(button_up != NULL && button_up->IsActive()) 
				button_up->SetStatus(2);
			
			if(button_down != NULL && button_down->IsActive())
				button_down->SetStatus(2);
		}
	} 
	else
	{
		bIsKeyPressed = false;
	}

	if(bIsKeyPressed)
	{
		bIsKeyPressed = false;
		if(lastKeyPressed == s3eKeyEnter || lastKeyPressed == s3eKeyAbsOk)
		{
			if(btnOK->IsActive())
			{
				btnOK->Execute();
				bBlockKeyboardOnce = true;
			}
		}
		else if(lastKeyPressed == s3eKeyAbsRight)
		{
			if(button_right != NULL && button_right->IsActive())
			{
				button_right->Execute();
				ButtonPressed = 2;
			}
		}
		else if(lastKeyPressed == s3eKeyAbsLeft)
		{
			if(button_left != NULL && button_left->IsActive())
			{
				button_left->Execute();
				ButtonPressed = 3;
			}
		}
		else if(lastKeyPressed == s3eKeyAbsUp)
		{
			if(button_up != NULL && button_up->IsActive())
			{
				button_up->Execute();
				ButtonPressed = 0;
			}
		}
		else if(lastKeyPressed == s3eKeyAbsDown)
		{
			if(button_down != NULL && button_down->IsActive())
			{
				button_down->Execute();
				ButtonPressed = 1;
			}
		}

		lastKeyPressed = s3eKeyFirst;
	}
}

void UnitMenu::ResetButtonStatus()
{
	if(btnOK->IsActive()) 
		btnOK->SetStatus(2);

	if(button_right != NULL && button_right->IsActive()) 
		button_right->SetStatus(2);

	if(button_left != NULL && button_left->IsActive()) 
		button_left->SetStatus(2);

	if(button_up != NULL && button_up->IsActive()) 
		button_up->SetStatus(2);

	if(button_down != NULL && button_down->IsActive())
		button_down->SetStatus(2);
}

/*
btnArcher->SetPos(GetX(), GetY() - GetPadding());
btnBowler->SetPos(GetX() + GetPadding(), GetY());
btnMage->SetPos(GetX(), GetY() + GetPadding());
btnTank->SetPos(GetX() - GetPadding(), GetY());
btnOK->SetPos(GetX(), GetY());
*/

int UnitMenu::GetWidth()
{
	if(btnArcher == NULL || btnOK == NULL)
	{
		return -1;
	}

	int unitButtonWidth = btnArcher->GetWidth();
	int okButtonWidth = btnOK->GetWidth();

	return okButtonWidth + unitButtonWidth + 2 * GetPadding();
}

int UnitMenu::GetHeight()
{
	if(btnArcher == NULL || btnOK == NULL)
	{
		return -1;
	}

	int unitButtonHeight = btnArcher->GetHeight();
	int okButtonHeight = btnOK->GetHeight();

	return okButtonHeight + unitButtonHeight + GetPadding() * 2;
}

void UnitMenu::AllowOnlyButton(Button* buttonToSelect)
{
	this->buttonToSelect = buttonToSelect;
}

void UnitMenu::Block()
{
	bIsBlocked = true;
}

void UnitMenu::Unblock()
{
	bIsBlocked = false;
}

void UnitMenu::SpawnSelknam(void (*func)(), Button* btn)
{
	if(bBlockSpawn)
		return;

	bBlockSpawn = true;

	if(onPressDuration.GetIsEnd() || onPressDuration.GetIsStop())
	{
		if(activeControllerType == Controller_MOUSE)
		{
			func();
		}
		else
		{
			btn->SetStatus(3);
			currSpawnFunc = func;
			onPressDuration.Start();
		}	
	}
}

void UnitMenu::SpawnArcherBase()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(archerBaseSpawnFnc, button_up);
}

void UnitMenu::SpawnArcherRetarder()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(archerRetarderSpawnFnc, button_left);
}

void UnitMenu::SpawnArcherPoisoner()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(archerPoisonerSpawnFnc, button_right);
}

void UnitMenu::SpawnWarriorBase()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(warriorBaseSpawnFnc, button_left);
}

void UnitMenu::SpawnWarriorTank()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(warriorTankSpawnFnc, button_up);
}

void UnitMenu::SpawnWarriorBerserk()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(warriorBerserkSpawnFnc, button_down);
}

void UnitMenu::SpawnShamanMight()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(shamanMightSpawnFnc, button_right);
}

void UnitMenu::SpawnShamanHealer()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(shamanHealerSpawnFnc, button_down);
}

void UnitMenu::SpawnShamanHaste()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(shamanHasteSpawnFnc, button_left);
}

void UnitMenu::SpawnBowlerRooter()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(bowlerRooterSpawnFnc, button_up);
}

void UnitMenu::SpawnBowlerConfuser()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(bowlerConfuserSpawnFnc, button_down);
}

void UnitMenu::SpawnBowlerStuner()
{
	if(currSpawnFunc != NULL)
		return;

	SpawnSelknam(bowlerStunerSpawnFnc, button_right);
}
