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

UnitMenu::UnitMenu()
{
	UnitMenu_Reference = this;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_unitmenu_classes");
	btnArcherData.numStates = 1;
	btnArcherData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 0);
	btnArcherData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 4);
	btnArcherData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 8);
	btnArcherData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 12);

	btnBowlerData.numStates = 1;
	btnBowlerData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 1);
	btnBowlerData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 5);
	btnBowlerData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 9);
	btnBowlerData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 13);

	btnMageData.numStates = 1;
	btnMageData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 2);
	btnMageData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 6);
	btnMageData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 10);
	btnMageData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 14);

	btnTankData.numStates = 1;
	btnTankData.state[0].RollOut  = TextureHelper::GetRenderData(data, 4, 4, 3);
	btnTankData.state[0].RollOn   = TextureHelper::GetRenderData(data, 4, 4, 7);
	btnTankData.state[0].Pressed  = TextureHelper::GetRenderData(data, 4, 4, 11);
	btnTankData.state[0].Inactive = TextureHelper::GetRenderData(data, 4, 4, 15);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_archers");

	btnArcherBase.numStates = 1;
	btnArcherBase.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0);
	btnArcherBase.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3);
	btnArcherBase.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6);
	btnArcherBase.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9);

	btnArcherPoisoner.numStates = 1;
	btnArcherPoisoner.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2);
	btnArcherPoisoner.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5);
	btnArcherPoisoner.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8);
	btnArcherPoisoner.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11);
	archerPoisonerBlock = TextureHelper::GetRenderData(data, 3, 5, 14);

	btnArcherRetarder.numStates = 1;
	btnArcherRetarder.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1);
	btnArcherRetarder.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4);
	btnArcherRetarder.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7);
	btnArcherRetarder.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10);
	archerRetarderBlock = TextureHelper::GetRenderData(data, 3, 5, 13);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_melees");

	btnWarriorBase.numStates = 1;
	btnWarriorBase.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0);
	btnWarriorBase.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3);
	btnWarriorBase.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6);
	btnWarriorBase.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9);

	btnWarriorTank.numStates = 1;
	btnWarriorTank.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2);
	btnWarriorTank.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5);
	btnWarriorTank.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8);
	btnWarriorTank.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11);
	warriorDefensiveBlock = TextureHelper::GetRenderData(data, 3, 5, 14);

	btnWarriorBerserk.numStates = 1;
	btnWarriorBerserk.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1);
	btnWarriorBerserk.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4);
	btnWarriorBerserk.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7);
	btnWarriorBerserk.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10);
	warriorOffensiveBlock = TextureHelper::GetRenderData(data, 3, 5, 13);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_mages");

	btnShamanHealer.numStates = 1;
	btnShamanHealer.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0);
	btnShamanHealer.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3);
	btnShamanHealer.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6);
	btnShamanHealer.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9);

	btnShamanMight.numStates = 1;
	btnShamanMight.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2);
	btnShamanMight.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5);
	btnShamanMight.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8);
	btnShamanMight.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11);
	shamanMightBlock = TextureHelper::GetRenderData(data, 3, 5, 13);

	btnShamanHaste.numStates = 1;
	btnShamanHaste.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1);
	btnShamanHaste.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4);
	btnShamanHaste.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7);
	btnShamanHaste.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10);
	shamanHasteBlock = TextureHelper::GetRenderData(data, 3, 5, 13);

	data = TextureHelper::GetTextureNamed("btn_unitmenu_class_bowlers");

	btnBowerStuner.numStates = 1;
	btnBowerStuner.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 0);
	btnBowerStuner.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 3);
	btnBowerStuner.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 6);
	btnBowerStuner.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 9);

	btnBowlerConfuser.numStates = 1;
	btnBowlerConfuser.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 2);
	btnBowlerConfuser.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 5);
	btnBowlerConfuser.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 8);
	btnBowlerConfuser.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 11);
	bowlerConfusionBlock = TextureHelper::GetRenderData(data, 3, 5, 13);

	btnBowlerRooter.numStates = 1;
	btnBowlerRooter.state[0].RollOut  = TextureHelper::GetRenderData(data, 3, 5, 1);
	btnBowlerRooter.state[0].RollOn   = TextureHelper::GetRenderData(data, 3, 5, 4);
	btnBowlerRooter.state[0].Pressed  = TextureHelper::GetRenderData(data, 3, 5, 7);
	btnBowlerRooter.state[0].Inactive = TextureHelper::GetRenderData(data, 3, 5, 10);
	bowlerRooterBlock = TextureHelper::GetRenderData(data, 3, 5, 13);

	TextureData btnOkData = TextureHelper::GetTextureNamed("btn_unitmenu_cancel");
	okRenderData.numStates = 1;
	okRenderData.state[0].RollOut  = TextureHelper::GetRenderData(btnOkData, 1, 4, 0);
	okRenderData.state[0].RollOn   = TextureHelper::GetRenderData(btnOkData, 1, 4, 1);
	okRenderData.state[0].Pressed  = TextureHelper::GetRenderData(btnOkData, 1, 4, 3);
	okRenderData.state[0].Inactive = TextureHelper::GetRenderData(btnOkData, 1, 4, 1);

	_x = 0;
	_y = 0;
	_padding = 80;
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
	button_ArcherPoisoner  = new Button(TextureHelper::GetCIwTexture(btnArcherPoisoner.state[0].RollOut.atlasName), &btnArcherPoisoner, GetX() + GetPadding(), GetY());
	button_ArcherRetarder  = new Button(TextureHelper::GetCIwTexture(btnArcherRetarder.state[0].RollOut.atlasName), &btnArcherRetarder, GetX(), GetY() + GetPadding());

	button_WarriorBase     = new Button(TextureHelper::GetCIwTexture(btnWarriorBase.state[0].RollOut.atlasName), &btnWarriorBase, GetX(), GetY() - GetPadding());
	button_WarriorTank     = new Button(TextureHelper::GetCIwTexture(btnWarriorTank.state[0].RollOut.atlasName), &btnWarriorTank, GetX() - GetPadding(), GetY());
	button_WarriorBerserk  = new Button(TextureHelper::GetCIwTexture(btnWarriorBerserk.state[0].RollOut.atlasName), &btnWarriorBerserk, GetX(), GetY() + GetPadding());

	button_ShamanHealer    = new Button(TextureHelper::GetCIwTexture(btnShamanHealer.state[0].RollOut.atlasName), &btnShamanHealer, GetX() - GetPadding(), GetY());
	button_ShamanMight     = new Button(TextureHelper::GetCIwTexture(btnShamanMight.state[0].RollOut.atlasName), &btnShamanMight,  GetX() + GetPadding(), GetY());
	button_ShamanHaste     = new Button(TextureHelper::GetCIwTexture(btnShamanHaste.state[0].RollOut.atlasName), &btnShamanHaste, GetX(), GetY() + GetPadding());

	button_BowlerConfuser  = new Button(TextureHelper::GetCIwTexture(btnBowlerRooter.state[0].RollOut.atlasName), &btnBowlerRooter, GetX(), GetY() - GetPadding());
	button_BowlerRooter    = new Button(TextureHelper::GetCIwTexture(btnBowlerConfuser.state[0].RollOut.atlasName), &btnBowlerConfuser, GetX() - GetPadding(), GetY());
	button_BowlerStuner    = new Button(TextureHelper::GetCIwTexture(btnBowerStuner.state[0].RollOut.atlasName), &btnBowerStuner, GetX() + GetPadding(), GetY());

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

	bOnUnitMenu = false;

	menu = MENU_None;
	bIsKeyPressed = false;
	bClickOnce = true;
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
		}
		else if(menu == MENU_Bowler)
		{
			if(BowlerRooterUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(bowlerRooterBlock.atlasName), CIwSVec2(button_up->GetX(), button_up->GetY()), bowlerRooterBlock);
				bNormalRender = false;
			}
		}

		if(bNormalRender)
		{
			button_up->Render();
			if(menu != MENU_None)
			{
				IwGxLightingOn();
				//Utils::RenderText(button_up_cost, CIwRect(button_up->GetX() + button_up->GetWidth() - 18, button_up->GetY() + button_up->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_up_cost, CIwRect(button_up->GetX() + button_up->GetWidth() - 20, button_up->GetY() + button_up->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				//Utils::RenderText(button_up_level, CIwRect(button_up->GetX() - 11, button_up->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
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
		}
		else if(menu == MENU_Tank)
		{
			if(WarriorBerserkUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(warriorOffensiveBlock.atlasName), CIwSVec2(button_down->GetX(), button_down->GetY()), warriorOffensiveBlock);
				bNormalRender = false;
			}
		}

		if(bNormalRender)
		{
			button_down->Render();
			if(menu != MENU_None)
			{
				IwGxLightingOn();
				//Utils::RenderText(button_down_cost, CIwRect(button_down->GetX() + button_down->GetWidth() - 18, button_down->GetY() + button_down->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_down_cost, CIwRect(button_down->GetX() + button_down->GetWidth() - 20, button_down->GetY() + button_down->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				//Utils::RenderText(button_down_level, CIwRect(button_down->GetX() - 11, button_down->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
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
		}
		else if(menu == MENU_Mage)
		{
			if(ShamanMightUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(shamanMightBlock.atlasName), CIwSVec2(button_right->GetX(), button_right->GetY()), shamanMightBlock);
				bNormalRender = false;
			}
		}

		if(bNormalRender)
		{
			button_right->Render();
			if(menu != MENU_None)
			{
				IwGxLightingOn();
				//Utils::RenderText(button_right_cost, CIwRect(button_right->GetX() + button_right->GetWidth() - 18, button_right->GetY() + button_right->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_right_cost, CIwRect(button_right->GetX() + button_right->GetWidth() - 20, button_right->GetY() + button_right->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				//Utils::RenderText(button_right_level, CIwRect(button_right->GetX() - 11, button_right->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
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
		}
		else if(menu == MENU_Mage)
		{
			if(ShamanHasteUpgrades.level == 0)
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(shamanHasteBlock.atlasName), CIwSVec2(button_left->GetX(), button_left->GetY()), shamanHasteBlock);
				bNormalRender = false;
			}
		}

		if(bNormalRender)
		{
			button_left->Render();
			if(menu != MENU_None)
			{
				IwGxLightingOn();
				//Utils::RenderText(button_left_cost, CIwRect(button_left->GetX() + button_left->GetWidth() - 18, button_left->GetY() + button_left->GetHeight() - 18, 200, 200), font_komikadisplaybold18, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				Utils::RenderText(button_left_cost, CIwRect(button_left->GetX() + button_left->GetWidth() - 20, button_left->GetY() + button_left->GetHeight() - 20, 200, 200), font_komikadisplaybold18, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
				//Utils::RenderText(button_left_level, CIwRect(button_left->GetX() - 11, button_left->GetY() - 11, 200, 200), font_komikadisplay14, 0xff000000, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
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

	if(event->m_Pressed && bFirstOpen)
		return;
	else bFirstOpen = false;
	
	if(event->m_Pressed && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE && !bOnUnitMenu)
		Close();


	if(button_up != NULL)
		button_up->HandleClick(event);

	if(button_down != NULL)
		button_down->HandleClick(event);

	if(button_left != NULL)
		button_left->HandleClick(event);

	if(button_right != NULL)
	{
		button_right->HandleClick(event);
	}

	btnOK->HandleClick(event);


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
	SetIsActive(false);
	menu = MENU_None;
	bBlockMousePointer = true;
}

void UnitMenu::Open(int coins)
{
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

	if(bShowTutorialLevel1)
	{
		tutorial->InitTutorial(Tutorial_UnitMenu);
	}
}

void UnitMenu::OpenWithKeyboard(int coins)
{
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
	button_ArcherBase->Enabled(ArcherBaseUpgrades.level > 0 && SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].cost <= coins);
	button_ArcherPoisoner->Enabled(ArcherPoisonerUpgrades.level > 0 && SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].cost <= coins);
	button_ArcherRetarder->Enabled(ArcherRetarderUpgrades.level > 0 && SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].cost <= coins);

	button_BowlerConfuser->Enabled(BowlerConfuserUpgrades.level > 0 && SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].cost <= coins);
	button_BowlerRooter->Enabled(BowlerRooterUpgrades.level > 0 && SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].cost <= coins);
	button_BowlerStuner->Enabled(BowlerStunerUpgrades.level > 0 && SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].cost <= coins);
	
	button_ShamanHaste->Enabled(ShamanHasteUpgrades.level > 0 && SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].cost <= coins);
	button_ShamanHealer->Enabled(ShamanHealerUpgrades.level > 0 && SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].cost <= coins);
	button_ShamanMight->Enabled(ShamanMightUpgrades.level > 0 && SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].cost <= coins);

	button_WarriorBase->Enabled(WarriorBaseUpgrades.level > 0 && SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].cost <= coins);
	button_WarriorBerserk->Enabled(WarriorBerserkUpgrades.level > 0 && SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].cost <= coins);
	button_WarriorTank->Enabled(WarriorTankUpgrades.level > 0 && SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].cost <= coins);
}

void UnitMenu::GoToArcherMenu()
{
	menu = MENU_Archer;

	button_up = button_ArcherBase;
	button_left = button_ArcherRetarder;
	button_down = NULL;
	button_right = button_ArcherPoisoner;

	if(ArcherBaseUpgrades.level != 0)
	{
		sprintf(button_up_cost, "%d",    SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].cost);
		sprintf(button_up_level, "Lv.%d",    SelknamAtt_Archer_Base[ArcherBaseUpgrades.level - 1].level);
	}
	
	if(ArcherRetarderUpgrades.level != 0)
	{
		sprintf(button_left_cost, "%d",  SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].cost);
		sprintf(button_left_level, "Lv.%d",  SelknamAtt_Archer_Retarder[ArcherRetarderUpgrades.level - 1].level);
	}
	
	sprintf(button_down_cost, "%d",  0);
	sprintf(button_down_level, "%d",  0);

	if(ArcherPoisonerUpgrades.level != 0)
	{
		sprintf(button_right_cost, "%d", SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].cost);
		sprintf(button_right_level, "Lv.%d", SelknamAtt_Archer_Poisoner[ArcherPoisonerUpgrades.level - 1].level);
	}
}

void UnitMenu::GoToWarriorMenu()
{
	menu = MENU_Tank;

	button_up = button_WarriorTank;
	button_left = button_WarriorBase;
	button_down = button_WarriorBerserk;
	button_right = NULL;

	if(WarriorTankUpgrades.level != 0)
	{
		sprintf(button_up_cost, "%d",        SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].cost);
		sprintf(button_up_level, "Lv.%d",    SelknamAtt_Warrior_Tank[WarriorTankUpgrades.level - 1].level);
	}
	
	if(WarriorBaseUpgrades.level != 0)
	{
		sprintf(button_left_cost, "%d",  SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].cost);
		sprintf(button_left_level, "Lv.%d",  SelknamAtt_Warrior_Base[WarriorBaseUpgrades.level - 1].level);
	}

	if(WarriorBerserkUpgrades.level != 0)
	{
		sprintf(button_down_cost, "%d",  SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].cost);
		sprintf(button_down_level, "Lv.%d",  SelknamAtt_Warrior_Berserk[WarriorBerserkUpgrades.level - 1].level);
	}
	
	sprintf(button_right_cost, "%d", 0);
}

void UnitMenu::GoToShamanMenu()
{
	menu = MENU_Mage;

	button_up = NULL;
	button_left = button_ShamanHaste;
	button_down = button_ShamanHealer;
	button_right = button_ShamanMight;

	sprintf(button_up_cost, "%d",    0);

	if(ShamanHasteUpgrades.level != 0)
	{
		sprintf(button_left_cost, "%d",  SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].cost);
		sprintf(button_left_level, "Lv.%d",  SelknamAtt_Shaman_Haste[ShamanHasteUpgrades.level - 1].level);
	}
	
	if(ShamanHealerUpgrades.level != 0)
	{
		sprintf(button_down_cost, "%d",  SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].cost);
		sprintf(button_down_level, "Lv.%d",  SelknamAtt_Shaman_Healer[ShamanHealerUpgrades.level - 1].level);
	}
	
	if(ShamanMightUpgrades.level != 0)
	{
		sprintf(button_right_cost, "%d", SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].cost);
		sprintf(button_right_level, "Lv.%d", SelknamAtt_Shaman_Might[ShamanMightUpgrades.level - 1].level);
	}
	
}

void UnitMenu::GoToBowlerMenu()
{
	menu = MENU_Bowler;

	button_up = button_BowlerRooter;
	button_left = NULL;
	button_down = button_BowlerConfuser;
	button_right = button_BowlerStuner;

	if(BowlerRooterUpgrades.level != 0)
	{
		sprintf(button_up_cost, "%d",    SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].cost);
		sprintf(button_up_level, "Lv.%d",    SelknamAtt_Bowler_Rooter[BowlerRooterUpgrades.level - 1].level);
	}
	
	sprintf(button_left_cost, "%d",  0);

	if(BowlerConfuserUpgrades.level != 0)
	{
		sprintf(button_down_cost, "%d",  SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].cost);
		sprintf(button_down_level, "Lv.%d",  SelknamAtt_Bowler_Confuser[BowlerConfuserUpgrades.level - 1].level);
	}
	
	if(BowlerStunerUpgrades.level != 0)
	{
		sprintf(button_right_cost, "%d", SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].cost);
		sprintf(button_right_level, "Lv.%d", SelknamAtt_Bowler_Stuner[BowlerStunerUpgrades.level - 1].level);
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