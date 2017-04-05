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

#include "string.h"

#include "ms_store.h"
#include "common.h"
#include "utils.h"
#include "parserhelper.h"
#include "common.h"

Store* storeRef = NULL;

//Keyboard handler
int32 KeyboardHandler_store(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(!storeRef->bKeyboardBlocked)
		storeRef->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotion_store(s3ePointerMotionEvent* event) 
{
	if(storeRef == NULL)
		return;

	if(!storeRef->bMouseBlocked)
		storeRef->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClick_store(s3ePointerEvent *event) 
{
	if(storeRef == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE && popUpState == GSP_None)
		{
			if(!event->m_Pressed)
			{
				storeRef->MouseHack();
			}
			else
			{
				storeRef->ActiveButtonMouseHack();
			}
		}

		return;
	}

	if(!storeRef->bMouseBlocked)
		storeRef->UpdateClickEvents(event);
}

//CUSTOM CLICK/KEY FUNCTIONS
void Store_OnClick_Back()
{
	mapSelectionState = MS_MapSelection;
}

void Store_OnMotion_Back()
{
	if(storeRef->focus != Store_Focus_Back)
	{
		storeRef->ResetFocus();
		storeRef->SetFocus(Store_Focus_Back);
	}
}

void Store_OnMotion_Reset()
{
	if(storeRef->focus != Store_Focus_Reset)
	{
		storeRef->ResetFocus();
		storeRef->SetFocus(Store_Focus_Reset);
	}
}

void Store_OnClick_Reset()
{
	bResetStoreStats = false;
	popUpState = GSP_ResetStore;
	bPausedStore = true;
}

void Store_GOTO_WarriorTab() { storeRef->FocusTab(WARRIOR_TAB); storeRef->LoadClassInfo(); }
void Store_GOTO_MageTab()    { storeRef->FocusTab(MAGE_TAB);    storeRef->LoadClassInfo(); }
void Store_GOTO_BowlerTab()  { storeRef->FocusTab(BOWLER_TAB);  storeRef->LoadClassInfo(); }
void Store_GOTO_ArcherTab()  { storeRef->FocusTab(ARCHER_TAB);  storeRef->LoadClassInfo(); }
void Store_CLICK_Panel()     { storeRef->UpgradeUnit(); storeRef->LoadClassInfo();         }
//END CUSTOM CLICK/KEY FUNCTIONS


Store::Store()
{
	storeRef = this;
	background = new Background(0,0,0, 170);

	descriptionBackgroundRD = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_store_2"));
	panelDimension.x += descriptionBackgroundRD.width + 10;

	TextureData lv1Data = TextureHelper::GetTextureNamed("btn_store_level_1");

	btnStoreLv1RenderData[0] = TextureHelper::GetRenderData(lv1Data, 1, 2, 0);
	btnStoreLv1RenderData[1] = TextureHelper::GetRenderData(lv1Data, 1, 2, 1);

	//Setting textures
	TextureData data1;
	data1 = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data1, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data1, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data1, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data1, 1, 4, 2);

	int btnBackWidth = buttonRenderData.state[0].Pressed.width;
	int btnBackHeight = buttonRenderData.state[0].Pressed.height;

	TextureData data = TextureHelper::GetTextureNamed("btn_store_reset");
	resetButtonData.numStates = 1;
	resetButtonData.state[0].RollOn   = TextureHelper::GetRenderData(data, 1, 4, 1);
	resetButtonData.state[0].RollOut  = TextureHelper::GetRenderData(data, 1, 4, 0);
	resetButtonData.state[0].Pressed  = TextureHelper::GetRenderData(data, 1, 4, 2);
	resetButtonData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 0);

	TextureData lv2TD = TextureHelper::GetTextureNamed("btn_store_level_2");

	lv2ButtonData.numStates = 2;
	lv2ButtonData.state[0].RollOut  = TextureHelper::GetRenderData(lv2TD, 2, 3, 0);
	lv2ButtonData.state[0].RollOn   = TextureHelper::GetRenderData(lv2TD, 2, 3, 2);
	lv2ButtonData.state[0].Pressed  = TextureHelper::GetRenderData(lv2TD, 2, 3, 0);
	lv2ButtonData.state[0].Inactive = TextureHelper::GetRenderData(lv2TD, 2, 3, 0);

	lv2ButtonData.state[1].RollOut  = TextureHelper::GetRenderData(lv2TD, 2, 3, 1);
	lv2ButtonData.state[1].RollOn   = TextureHelper::GetRenderData(lv2TD, 2, 3, 3);
	lv2ButtonData.state[1].Pressed  = TextureHelper::GetRenderData(lv2TD, 2, 3, 1);
	lv2ButtonData.state[1].Inactive = TextureHelper::GetRenderData(lv2TD, 2, 3, 1);

	TextureData lv3TD = TextureHelper::GetTextureNamed("btn_store_level_3");

	lv3ButtonData.numStates = 2;
	lv3ButtonData.state[0].RollOut  = TextureHelper::GetRenderData(lv3TD, 2, 3, 0);
	lv3ButtonData.state[0].RollOn   = TextureHelper::GetRenderData(lv3TD, 2, 3, 2);
	lv3ButtonData.state[0].Pressed  = TextureHelper::GetRenderData(lv3TD, 2, 3, 0);
	lv3ButtonData.state[0].Inactive = TextureHelper::GetRenderData(lv3TD, 2, 3, 0);

	lv3ButtonData.state[1].RollOut  = TextureHelper::GetRenderData(lv3TD, 2, 3, 1);
	lv3ButtonData.state[1].RollOn   = TextureHelper::GetRenderData(lv3TD, 2, 3, 3);
	lv3ButtonData.state[1].Pressed  = TextureHelper::GetRenderData(lv3TD, 2, 3, 1);
	lv3ButtonData.state[1].Inactive = TextureHelper::GetRenderData(lv3TD, 2, 3, 1);

	panel = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_store_1"));
	panelDimension = TextureHelper::GetDimension(panel);

	starCounterData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_infocard_star"));
	starCounterData.width = 196;
	starCounterData.height = 139;
	if(starCounterData.bRotate)
	{
		starCounterData.width = 139;
		starCounterData.height = 196;
	}

	pannel_store_title = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("pannel_store_title"));

	//Setting textures
	TextureData btn_store_class_classes_data = TextureHelper::GetTextureNamed("btn_store_class_classes");
	btnStoreArcher.numStates = 1;
	btnStoreArcher.state[0].RollOut  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 0);
	btnStoreArcher.state[0].RollOn   = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 4);
	btnStoreArcher.state[0].Pressed  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 8);
	btnStoreArcher.state[0].Inactive = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 12);
	btnStoreArcherSelected = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 16);

	btnStoreBowler.numStates = 1;
	btnStoreBowler.state[0].RollOut  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 1);
	btnStoreBowler.state[0].RollOn   = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 5);
	btnStoreBowler.state[0].Pressed  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 9);
	btnStoreBowler.state[0].Inactive = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 13);
	btnStoreBowlerSelected = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 17);

	btnStoreMage.numStates = 1;
	btnStoreMage.state[0].RollOut  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 2);
	btnStoreMage.state[0].RollOn   = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 6);
	btnStoreMage.state[0].Pressed  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 10);
	btnStoreMage.state[0].Inactive = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 14);
	btnStoreMageSelected = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 18);;

	btnStoreWarrior.numStates = 1;
	btnStoreWarrior.state[0].RollOut  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 3);
	btnStoreWarrior.state[0].RollOn   = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 7);
	btnStoreWarrior.state[0].Pressed  = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 11);
	btnStoreWarrior.state[0].Inactive = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 15);
	btnStoreWarriorSelected = TextureHelper::GetRenderData(btn_store_class_classes_data, 4, 5, 19);

	pfont = font_komikadisplaybold25;

	//portrait uv settings
	portrait_uvX = 0.2f;
	portrait_uvY = 0.125f;

	button_reset = new Button(TextureHelper::GetCIwTexture(resetButtonData.state[0].RollOut.atlasName), &resetButtonData, 50, 0);
	button_reset->OnClick(&Store_OnClick_Reset);
	button_reset->OnRollOn(&Store_OnMotion_Reset);

	button_back =  new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, 510, 630);
	button_back->SetText("OK");
	button_back->OnClick(&Store_OnClick_Back);
	button_back->OnRollOn(&Store_OnMotion_Back);

	//Mouse-Keyboard events.
	bKeyboardBlocked = false;
	bMouseBlocked = false;

	//title
	titlePos = CIwRect(450, 20, 450, 70);
	titleColor = 0xffffffff;

	CIwSVec2 starCounterDataDimension = TextureHelper::GetDimension(starCounterData);

	remaining_stars_color = 0xffffffff;
	remaining_stars_position = CIwRect(1000, 20, starCounterDataDimension.x, starCounterDataDimension.y);
	
	
	button_panel[0][0] = new Button(TextureHelper::GetCIwTexture(lv2ButtonData.state[0].RollOut.atlasName), &lv2ButtonData, 350 + (96 + 20) * 0, 390);
	button_panel[0][0]->OnClick(&Store_CLICK_Panel);
	button_panel[1][0] = new Button(TextureHelper::GetCIwTexture(lv3ButtonData.state[0].RollOut.atlasName), &lv3ButtonData, 350 + (96 + 20) * 0, 485);
	button_panel[1][0]->OnClick(&Store_CLICK_Panel);
	button_panel[0][1] = new Button(TextureHelper::GetCIwTexture(lv2ButtonData.state[0].RollOut.atlasName), &lv2ButtonData, 350 + (96 + 20) * 1, 390);
	button_panel[0][1]->OnClick(&Store_CLICK_Panel);
	button_panel[1][1] = new Button(TextureHelper::GetCIwTexture(lv3ButtonData.state[0].RollOut.atlasName), &lv3ButtonData, 350 + (96 + 20) * 1, 485);
	button_panel[1][1]->OnClick(&Store_CLICK_Panel);
	button_panel[0][2] = new Button(TextureHelper::GetCIwTexture(lv2ButtonData.state[0].RollOut.atlasName), &lv2ButtonData, 350 + (96 + 20) * 2, 390);
	button_panel[0][2]->OnClick(&Store_CLICK_Panel);
	button_panel[1][2] = new Button(TextureHelper::GetCIwTexture(lv3ButtonData.state[0].RollOut.atlasName), &lv3ButtonData, 350 + (96 + 20) * 2, 485);
	button_panel[1][2]->OnClick(&Store_CLICK_Panel);

	int tab_margin_x = 1;
	int tab_x = 355;
	int tab_y = 145;
	int btnWidth = btnStoreArcher.state[0].Pressed.width;
	button_tab[0] = new Button(TextureHelper::GetCIwTexture(btnStoreArcher.state[0].RollOut.atlasName), &btnStoreArcher, tab_x + (btnWidth + tab_margin_x) * 0, tab_y);
	button_tab[2] = new Button(TextureHelper::GetCIwTexture(btnStoreWarrior.state[0].RollOut.atlasName), &btnStoreWarrior, tab_x + (btnWidth + tab_margin_x) * 2, tab_y);
	button_tab[1] = new Button(TextureHelper::GetCIwTexture(btnStoreBowler.state[0].RollOut.atlasName), &btnStoreBowler, tab_x + (btnWidth + tab_margin_x) * 1, tab_y);
	button_tab[3] = new Button(TextureHelper::GetCIwTexture(btnStoreMage.state[0].RollOut.atlasName), &btnStoreMage, tab_x + (btnWidth + tab_margin_x) * 3, tab_y);
	button_tab[0]->OnClick(&Store_GOTO_ArcherTab);
	button_tab[2]->OnClick(&Store_GOTO_WarriorTab);
	button_tab[1]->OnClick(&Store_GOTO_BowlerTab);
	button_tab[3]->OnClick(&Store_GOTO_MageTab);
	
	for(int i = 0; i < 3; i++)
		for(int j = 0; j < 3; j++)
			available_panel[i][j] = true;
 
	bIsPortraitFocused[0] = false;
	bIsPortraitFocused[1] = false;
	bIsPortraitFocused[2] = false;

	bDoOnce = false;
	bShowThirdCol = false;
	bShowInformation = false;

	TextureData portraitData = TextureHelper::GetTextureNamed("graphic_store_portraits");

	int index = 7;
	int initialIndex = 7;
	for(int i = 0; i < 8; i++)
	{
		index = 5 * i;
		for(int j = 0; j < 5; j++)
		{
			portraitArray[i][j] = TextureHelper::GetRenderData(portraitData, 5, 8, index + j);
		}
	}
	
	sprintf(health[0],       "%s", "Health");
	sprintf(damage[0],       "%s", "Damage");
	sprintf(speed[0],        "%s", "Speed");
	sprintf(attack_speed[0], "%s", "Attack Sp");
	sprintf(cast_speed[0],   "%s", "Cast Sp");
	sprintf(range[0],        "%s", "Range");
	sprintf(cost[0],         "%s", "Cost");
	sprintf(move_cost[0],    "%s", "Move Cost");
	sprintf(skill_name[0],   "%s", "Skill");
	sprintf(health[1],       "%s", "?");
	sprintf(damage[1],       "%s", "?");
	sprintf(speed[1],        "%s", "?");
	sprintf(attack_speed[1], "%s", "?");
	sprintf(cast_speed[1],   "%s", "?");
	sprintf(range[1],        "%s", "?");
	sprintf(cost[1],         "%s", "?");
	sprintf(move_cost[1],    "%s", "?");
	sprintf(skill_name[1],   "%s", "?");
	sprintf(health[2],       "%s", "?");
	sprintf(damage[2],       "%s", "?");
	sprintf(speed[2],        "%s", "?");
	sprintf(attack_speed[2], "%s", "?");
	sprintf(cast_speed[2],   "%s", "?");
	sprintf(range[2],        "%s", "?");
	sprintf(cost[2],         "%s", "?");
	sprintf(move_cost[2],    "%s", "?");
	sprintf(skill_name[2],   "%s", "?");

	title = "Selknam Store";
	portrait_name = "Archer Base";
	panel_pos._x = 0;
	UpdateInformationWindow();

	tab = ARCHER_TAB;

	last_tab_pos = 0;
	bPausedStore = false;

	bDrawFake_MageTab = bDrawFake_ArcherTab = bDrawFake_WarriorTab = bDrawFake_BowlerTab = true;
	bValuesAreUnknow = false;
	bHasSkill = true;

	arrowStore = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_store_arrow"));
}

Store::~Store()
{
	delete background;
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 3; j++)
			delete button_panel[i][j];
	delete button_back;
	delete button_reset;
	for(int i = 0; i < 4; i++)
		for(int j = 0; j < 3; j++)
			selknam_img[i][j] = NULL;
	star[0] = NULL;
	star[1] = NULL;
	for(int i = 0; i < 4; i++)
		delete button_tab[i];
}

void Store::Render()
{
	background->Render();

	Utils::RenderLabelTitle(10, (int)((IwGxGetScreenWidth() - (128 + 320)) * 0.5f), -50);

	Utils::RenderPannelA(8, 8, (int)((IwGxGetScreenWidth() - 600 - 40) * 0.5f), 210);

	Utils::RenderVerticalTube((int)((IwGxGetScreenWidth() - 600 - 40) * 0.5f) + 130, 230, 360);
	Utils::RenderVerticalTube((int)((IwGxGetScreenWidth() - 600 - 40) * 0.5f) + 130 + 117, 230, 360);

	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(starCounterData.atlasName), CIwSVec2(1020, 0), starCounterData);

	mainPanelWidth  = (int)((IwGxGetScreenWidth() - 600 - 40) * 0.5f);
	mainPanelHeight = 210;

	Utils::RenderPannelA(4, 8, mainPanelWidth + 400 + 5, mainPanelHeight);

	gray_color[0] = gray_color[1] = gray_color[2] = false;

	if(tab == ARCHER_TAB)
	{
		if(ArcherBaseUpgrades.level == 0)
			gray_color[0] = true;
		if(ArcherRetarderUpgrades.level == 0)
			gray_color[1] = true;
		if(ArcherPoisonerUpgrades.level == 0)
			gray_color[2] = true;
	}
	else if(tab == WARRIOR_TAB)
	{
		if(WarriorBaseUpgrades.level == 0)
			gray_color[0] = true;
		if(WarriorBerserkUpgrades.level == 0)
			gray_color[1] = true;
		if(WarriorTankUpgrades.level == 0)
			gray_color[2] = true;
	}
	else if(tab == BOWLER_TAB)
	{
		if(BowlerStunerUpgrades.level == 0)
			gray_color[0] = true;
		if(BowlerConfuserUpgrades.level == 0)
			gray_color[1] = true;
		if(BowlerRooterUpgrades.level == 0)
			gray_color[2] = true;
	}
	else if(tab == MAGE_TAB)
	{
		if(ShamanHealerUpgrades.level == 0)
			gray_color[0] = true;
		if(ShamanHasteUpgrades.level == 0)
			gray_color[1] = true;
		if(ShamanMightUpgrades.level == 0)
			gray_color[2] = true;
	}

	for(int i = 0; i < 3; i++)
	{
		int off = 0;
		if(gray_color[i]) 
		{
			off = 2;
		}

		int focused_margin = 0;
		if(bIsPortraitFocused[i]) focused_margin = 4;

		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[(int)tab + focused_margin][i + off].atlasName), CIwSVec2((int)(350 + (float)((96 + 20) * i)), (int)240), portraitArray[(int)tab + focused_margin][i + off]);
	}

	if(!bIsFullVersion)
	{
		for(int i = 0; i < 3; i++)
		{
			if(gray_color[i])
			{
				Utils::RenderColor(0xaf000000, CIwSVec2(350 + ((96 + 20) * i), 240), CIwSVec2(96, 96));
				IwGxFlush();
				IwGxLightingOn();
				Utils::RenderText("ONLY FULL\nVERSION", CIwRect(350 + ((96 + 20) * i), 240, 96, 96), font_komikadisplay11, 0xff00c0ff);
				IwGxLightingOff();
			}
		}
	}


	IwGxFlush();

	for(int i = 0; i < 3; i++)
	{
		int disable = 0;
		if(gray_color[i]) 
		{
			disable = 1;
		}
		

		if(i == 0) 
			continue;

		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreLv1RenderData[disable].atlasName), CIwSVec2(350 + (96 + 20) * i, 240 + 100), btnStoreLv1RenderData[disable]);
	}
	

	if(tab == MAGE_TAB && bDrawFake_MageTab && focus != Store_Focus_Tab)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreMageSelected.atlasName), CIwSVec2(button_tab[3]->GetX(), button_tab[3]->GetY()), btnStoreMageSelected);
	}
	else
	{
		button_tab[3]->Render();
	}

	if(tab == WARRIOR_TAB && bDrawFake_WarriorTab && focus != Store_Focus_Tab)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreWarriorSelected.atlasName), CIwSVec2(button_tab[2]->GetX(), button_tab[2]->GetY()), btnStoreWarriorSelected);
	}
	else
	{
		button_tab[2]->Render();
	}
	 
	if(tab == BOWLER_TAB && bDrawFake_BowlerTab && focus != Store_Focus_Tab)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreBowlerSelected.atlasName), CIwSVec2(button_tab[1]->GetX(), button_tab[1]->GetY()), btnStoreBowlerSelected);
	}
	else
	{
		button_tab[1]->Render();
	}

	if(tab == ARCHER_TAB && bDrawFake_ArcherTab && focus != Store_Focus_Tab)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreArcherSelected.atlasName), CIwSVec2(button_tab[0]->GetX(), button_tab[0]->GetY()), btnStoreArcherSelected);
	}
	else
	{
		button_tab[0]->Render();
	}

	for(int i = 0; i < 3; i++)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(arrowStore.atlasName), CIwSVec2((int)350 + (int)((96 + 20) * i) + 48 - 16, (int)240 + 96 + 4 + 32), arrowStore);
	}

	for(int i = 0; i < 3; i++)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(arrowStore.atlasName), CIwSVec2((int)350 + (int)((96 + 20) * i) + 48 - 16, (int)240 +96 + 4 + 32 + 96), arrowStore);
	}

	
	for(int i = 0; i < 3; i++) 
	{
		button_panel[0][i]->Render();
		button_panel[1][i]->Render();
	}

	if(bShowInformation)
	{
		int current_p_x = panel_pos._x;
		int current_p_y = 0;
		if(gray_color[panel_pos._x]) current_p_x += 2; //Lock portrait.
		if(tab == ARCHER_TAB)       current_p_y = 0;
		else if(tab == BOWLER_TAB)  current_p_y = 1;
		else if(tab == WARRIOR_TAB) current_p_y = 2;
		else if(tab == MAGE_TAB)    current_p_y = 3;
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[current_p_y][current_p_x].atlasName),
								   CIwSVec2(790, 270),
								   portraitArray[current_p_y][current_p_x]);
	}


	//Titles
	IwGxLightingOn();
	Utils::RenderText("SELKNAM UPGRADE", CIwRect(410, 45, 438, 58), font_komikadisplay28, 0xff00d9fc);
	IwGxFontSetFont(font_komikadisplaybold18);
	IwGxFontSetCol(remaining_stars_color);
	IwGxFontSetRect(remaining_stars_position);
	IwGxFontDrawText(remaining_stars);

	if(bShowInformation)
	{
		int adjust = 0;
		if(tab == MAGE_TAB)
			adjust = 10;

		int informationX = 750;

		Utils::RenderText(portrait_name.c_str(), CIwRect(informationX, 230, 180, 50), font_komikadisplaybold18, 0xff00edf8);

		//Attributes list
		Utils::RenderText("Attributes", CIwRect(informationX + 5,  355 + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(current_level, CIwRect(informationX + 5 + 100,  355 + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(bShowThirdCol &&  !gray_color[panel_pos._x])
			Utils::RenderText(next_level, CIwRect(informationX + 5 + 140, 355 + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

		int padding = 20;
		//Col 1
		Utils::RenderText(health[0],       CIwRect(informationX + 5, 355 + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(tab != MAGE_TAB)
			Utils::RenderText(damage[0],       CIwRect(informationX + 5, 355 + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(speed[0],        CIwRect(informationX + 5, 355 + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(tab != MAGE_TAB)
			Utils::RenderText(attack_speed[0], CIwRect(informationX + 5, 355 + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		else
			Utils::RenderText(cast_speed[0],   CIwRect(informationX + 5, 355 + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(range[0],        CIwRect(informationX + 5, 355 + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(cost[0],         CIwRect(informationX + 5, 355 + 20 + 5 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(move_cost[0],    CIwRect(informationX + 5, 355 + 20 + 6 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(bHasSkill)
			Utils::RenderText(skill_name[0],   CIwRect(informationX + 5, 355 + 20 + 8 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

		//Col 2
		Utils::RenderText(health[1],       CIwRect(informationX + 5 + 100, 355 + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(tab != MAGE_TAB)
			Utils::RenderText(damage[1],       CIwRect(informationX + 5 + 100, 355 + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(speed[1],        CIwRect(informationX + 5 + 100, 355 + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(tab != MAGE_TAB)
			Utils::RenderText(attack_speed[1], CIwRect(informationX + 5 + 100, 355 + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		else
			Utils::RenderText(cast_speed[1],   CIwRect(informationX + 5 + 100, 355 + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(range[1],        CIwRect(informationX + 5 + 100, 355 + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(cost[1],         CIwRect(informationX + 5 + 100, 355 + 20 + 5 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(move_cost[1],    CIwRect(informationX + 5 + 100, 355 + 20 + 6 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(bHasSkill)
			Utils::RenderText(skill_name[1],   CIwRect(informationX + 5 + 100, 355 + 20 + 8 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

		//Col 3 008d00 0xff008d00 (0xffffffff) ? bValuesUnknow : 0xff00edf8
		if(bShowThirdCol && !gray_color[panel_pos._x])
		{
			if(bValuesAreUnknow)
				Utils::RenderText(health[2],       CIwRect(informationX + 5 + 140, 355 + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			else
				Utils::RenderText(health[2],       CIwRect(informationX + 5 + 140, 355 + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);

			if(bValuesAreUnknow)
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(damage[2],       CIwRect(informationX + 5 + 140, 355 + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			}
			else
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(damage[2],       CIwRect(informationX + 5 + 140, 355 + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);
			}

			if(bValuesAreUnknow)			
				Utils::RenderText(speed[2],        CIwRect(informationX + 5 + 140, 355 + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			else
				Utils::RenderText(speed[2],        CIwRect(informationX + 5 + 140, 355 + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);

			if(bValuesAreUnknow)
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(attack_speed[2], CIwRect(informationX + 5 + 140, 355 + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
				else
					Utils::RenderText(cast_speed[2],   CIwRect(informationX + 5 + 140, 355 + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			}
			else
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(attack_speed[2], CIwRect(informationX + 5 + 140, 355 + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);
				else
					Utils::RenderText(cast_speed[2],   CIwRect(informationX + 5 + 140, 355 + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);
			}

			if(bValuesAreUnknow)
				Utils::RenderText(range[2],        CIwRect(informationX + 5 + 140, 355 + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			else
				Utils::RenderText(range[2],        CIwRect(informationX + 5 + 140, 355 + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);

			Utils::RenderText(cost[2],         CIwRect(informationX + 5 + 140, 355 + 20 + 5 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

			Utils::RenderText(move_cost[2],    CIwRect(informationX + 5 + 140, 355 + 20 + 6 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

			if(bHasSkill)
				Utils::RenderText(skill_name[2],   CIwRect(informationX + 5 + 140, 355 + 20 + 8 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		}
	}
	IwGxLightingOff();

	button_reset->Render();
	button_back->Render();
}

void Store::InitObject()
{
	panel_pos._x = 0;
	panel_pos._y = 0;
	tab_pos._x = 0;

	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_store, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_store, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_store, NULL);




	UpdateRemainingStars();
	FocusTab(ARCHER_TAB);
	LoadClassInfo();
	bFxSoundMuted = true;
	ResetFocus();

	//Cleaning panel focus.
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			button_panel[i][j]->IsFocused(false);
		}
	}

	SetFocus(Store_Focus_Back);
	bFxSoundMuted = false;

	if(bUseOldAudioSystem)
	{
		//then start the next
		if(!SOUND_MUSIC_MUTE && !s3eAudioIsPlaying() && !bMapSelectionAudioPlaying)
		{
			if(bIsAudioPaused)
			{
				LastMusicStacked = Music_Store;
			}
			else
			{
				s3eAudioPlay("bgm/sd_store_theme_loop.mp3", 0);
				LastMusicStacked = Music_None;
			}
		}
	}
}

void Store::UpdateRemainingStars()
{
	int num_levels = sizeof(Stars_By_Level) / 4;
	int current_stars = 0;
	for(int i = 0; i < num_levels; i++) current_stars += Stars_By_Level[i];
	num_remaining_stars = current_stars - Used_Stars;
	sprintf(remaining_stars, "%d / %d", current_stars - Used_Stars, current_stars);
}

void Store::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_store);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_store);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_store);

	ParserHelper::SaveGameData();
}

void Store::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bPausedStore)
	{
		return;
	}

	if(event->m_Pressed) bKeyboardBlocked = true;
	else bKeyboardBlocked = false;

	button_reset->HandleClick(event);
	button_back->HandleClick(event);
	
	for(int i = 0; i < 4; i++)
	{
		button_tab[i]->HandleClick(event);
	}

	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 3; j++)
		{
			if(bIsFullVersion || (!bIsFullVersion && j == 0))
				button_panel[i][j]->HandleClick(event);
		}

	if(!event->m_Pressed) 
	{
		bDoOnce = true;
		return;
	}

	if(event->m_Button != S3E_POINTER_BUTTON_LEFTMOUSE) return;

	if(bDoOnce)
		bDoOnce = false;
	else
		return;

	if(MouseOn((int)event->m_x, (int)event->m_y, (int)(350 + (float)((96 + 20) * 0)), 240, 96, 96))
	{
		Utils::PlayFXSound(FX_Sound_CLICK);
	}

	if(MouseOn((int)event->m_x, (int)event->m_y, (int)(350 + (float)((96 + 20) * 1)), 240, 96, 96))
	{
		if(bIsFullVersion)
		{
			if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherRetarderUpgrades, archer_retarder_cost[0]);
			else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorBerserkUpgrades, warrior_berserker_cost[0]);
			else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerConfuserUpgrades, bowler_confuser_cost[0]);
			else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanHasteUpgrades, mage_haste_cost[0]);
		
			Utils::PlayFXSound(FX_Sound_CLICK);
		}
	}

	if(MouseOn((int)event->m_x, (int)event->m_y, (int)(350 + (float)((96 + 20) * 2)), 240, 96, 96))
	{
		if(bIsFullVersion)
		{
			if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherPoisonerUpgrades, archer_poisoner_cost[0]);
			else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorTankUpgrades, warrior_tank_cost[0]);
			else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerRooterUpgrades, bowler_rooter_cost[0]);
			else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanMightUpgrades, mage_might_cost[0]);
			
			Utils::PlayFXSound(FX_Sound_CLICK);		
		}
	}
}

bool Store::MouseOn(int x, int y, int xPos, int yPos, int _width, int _height) 
{
	return (x >= xPos && 
		x <= (xPos + _width) && 
		y >= yPos && 
		y <= (yPos + (int)(_height)));
}

void Store::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bPausedStore)
	{
		return;
	}

	button_reset->HandleMotion(event);
	button_back->HandleMotion(event);
	
	bDrawFake_WarriorTab = true;
	bDrawFake_MageTab = true;
	bDrawFake_BowlerTab = true;
	bDrawFake_ArcherTab = true;

	for(int i = 0; i < 4; i++)
	{
		button_tab[i]->HandleMotion(event);
		if(button_tab[i]->MouseOn(event->m_x, event->m_y))
		{				
			//Remove focus from all buttons.
			button_reset->IsFocused(false);
			button_back->IsFocused(false);
			//button_panel[panel_pos._y][panel_pos._x]->IsFocused(false);
			//button_tab[tab_pos._x]->IsFocused(false);
			bIsPortraitFocused[0] = false;
			bIsPortraitFocused[1] = false;
			bIsPortraitFocused[2] = false;

			if(i == 0) bDrawFake_ArcherTab = false;
			else if(i == 1) bDrawFake_BowlerTab = false;
			else if(i == 2) bDrawFake_WarriorTab = false;
			else if(i == 3) bDrawFake_MageTab = false;

			SetFocus(Store_Focus_SpecialTab);

			if(i != last_tab_pos)
			{
				Utils::PlayFXSound(FX_Sound_ROLLOVER02);
				last_tab_pos = i;
				//bDrawFake_ArcherTab = bDrawFake_WarriorTab = bDrawFake_BowlerTab = bDrawFake_MageTab = true;




				//Set this special focus, this means that's not actual focus on the window.
			}


		}
	}
	
	if(MouseOn((int)event->m_x, (int)event->m_y, (int)(350 + (float)((96 + 20) * 0)), 240, 96, 96))
	{
		if(focus != Store_Focus_Portrait_1)
		{
			ResetFocus();
			panel_pos._x = 0;
			panel_pos._y = -1;
			SetFocus(Store_Focus_Portrait_1);
			Utils::PlayFXSound(FX_Sound_ROLLOVER02);
		}
	}
	
	if(MouseOn((int)event->m_x, (int)event->m_y, 350 + (int)((float)((96 + 20) * 1)), 240, 96, 96))
	{
		if(bIsFullVersion)
		{
			if(focus != Store_Focus_Portrait_2)
			{
				ResetFocus();
				panel_pos._x = 1;
				panel_pos._y = -1;
				SetFocus(Store_Focus_Portrait_2);
				Utils::PlayFXSound(FX_Sound_ROLLOVER02);
			}
		}
	}

	if(MouseOn((int)event->m_x, (int)event->m_y, 350 + (int)((float)((96 + 20) * 2)), 240, 96, 96))
	{
		if(bIsFullVersion)
		{
			if(focus != Store_Focus_Portrait_3)
			{
				ResetFocus();
				panel_pos._x = 2;
				panel_pos._y = -1;
				SetFocus(Store_Focus_Portrait_3);
				Utils::PlayFXSound(FX_Sound_ROLLOVER02);
			}
		}
	}
	
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 3; j++)
		{
			if(bIsFullVersion || (!bIsFullVersion && j == 0))
			{
				button_panel[i][j]->HandleMotion(event);

				if(button_panel[i][j]->MouseOn(event->m_x, event->m_y))
				{
					if(panel_pos._x != j || panel_pos._y != i || focus != Store_Focus_Panel)
					{
						ResetFocus();
						panel_pos._x = j;
						panel_pos._y = i;
						SetFocus(Store_Focus_Panel);
					}
				}
			}
		}
}

void Store::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(bPausedStore)
	{
		return;
	}

	if(event->m_Pressed)
	{
		bMouseBlocked = true;
		
		if(event->m_Key == s3eKeyAbsRight) 
		{
			if(focus == Store_Focus_Tab || focus == Store_Focus_SpecialTab)
			{
				tab_pos._x = (int)tab;
				if(tab_pos._x < 3)
				{
					ResetFocus();
					tab_pos._x++;
					tab = (Selknam_Tab)tab_pos._x;
					storeRef->LoadClassInfo();
					SetFocus(Store_Focus_Tab);
				}
			}
			else if(focus == Store_Focus_Panel)
			{
				if(bIsFullVersion)
				{
					if(panel_pos._x < 2)
					{
						ResetFocus();
						panel_pos._x++;
						SetFocus(Store_Focus_Panel);
					}
				}
			}
			else if(focus == Store_Focus_Portrait_1)
			{
				if(bIsFullVersion)
				{
					ResetFocus();
					panel_pos._x = 1;
					SetFocus(Store_Focus_Portrait_2);
					Utils::PlayFXSound(FX_Sound_ROLLOVER02);
				}
			}
			else if(focus == Store_Focus_Portrait_2)
			{
				if(bIsFullVersion)
				{
					ResetFocus();
					panel_pos._x = 2;
					SetFocus(Store_Focus_Portrait_3);
					Utils::PlayFXSound(FX_Sound_ROLLOVER02);
				}
			}
			else if(focus == Store_Focus_Reset)
			{
				ResetFocus();
				SetFocus(Store_Focus_Tab);
			}
		}
		else if(event->m_Key == s3eKeyAbsLeft)
		{
			if(focus == Store_Focus_Tab || focus == Store_Focus_SpecialTab)
			{
				tab_pos._x = (int)tab;
				if(tab_pos._x > 0)
				{
					ResetFocus();
					tab_pos._x--;
					tab = (Selknam_Tab)tab_pos._x;
					storeRef->LoadClassInfo();
					SetFocus(Store_Focus_Tab);
				}
				else if(tab_pos._x == 0)
				{
					ResetFocus();
					SetFocus(Store_Focus_Reset);
				}
			}
			else if(focus == Store_Focus_Panel)
			{
				if(panel_pos._x > 0 && available_panel[panel_pos._y][panel_pos._x - 1])
				{
					ResetFocus();
					panel_pos._x--;
					SetFocus(Store_Focus_Panel);
				}
			}
			else if(focus == Store_Focus_Portrait_2)
			{
				ResetFocus();
				panel_pos._x--;
				SetFocus(Store_Focus_Portrait_1);
				Utils::PlayFXSound(FX_Sound_ROLLOVER02);
			}
			else if(focus == Store_Focus_Portrait_3)
			{
				ResetFocus();
				panel_pos._x--;
				SetFocus(Store_Focus_Portrait_2);
				Utils::PlayFXSound(FX_Sound_ROLLOVER02);
			}
		}
		else if(event->m_Key == s3eKeyAbsUp)
		{
			if(focus == Store_Focus_Back)
			{
				ResetFocus();
				panel_pos._y = 1;
				
				SetFocus(Store_Focus_Panel);
			}
			else if(focus == Store_Focus_Panel)
			{
				if(panel_pos._y > 0)
				{
						ResetFocus();
						panel_pos._y--;
						SetFocus(Store_Focus_Panel);
				}
				else
				{
					Utils::PlayFXSound(FX_Sound_ROLLOVER02);
					ResetFocus();
					panel_pos._y = -1;
					if(panel_pos._x == 0) SetFocus(Store_Focus_Portrait_1);
					else if(panel_pos._x == 1) SetFocus(Store_Focus_Portrait_2);
					else if(panel_pos._x == 2) SetFocus(Store_Focus_Portrait_3);
				}
			}
			else if(focus == Store_Focus_Tab)
			{
				ResetFocus();
				SetFocus(Store_Focus_Reset);
			}
			else if(focus == Store_Focus_Portrait_1 || focus == Store_Focus_Portrait_2 || focus == Store_Focus_Portrait_3)
			{
				ResetFocus();
				tab_pos._x = (int)tab;
				SetFocus(Store_Focus_Tab);
			}
		}
		else if(event->m_Key == s3eKeyAbsDown)
		{
			if(focus == Store_Focus_Tab || focus == Store_Focus_SpecialTab)
			{
				Utils::PlayFXSound(FX_Sound_ROLLOVER02);
				ResetFocus();
				if(panel_pos._x == 0) SetFocus(Store_Focus_Portrait_1);
				else if(panel_pos._x == 1) SetFocus(Store_Focus_Portrait_2);
				else if(panel_pos._x == 2) SetFocus(Store_Focus_Portrait_3);
			}
			else if(focus == Store_Focus_Panel)
			{
				if(panel_pos._y < 1)
				{
					ResetFocus();
					panel_pos._y++;
					SetFocus(Store_Focus_Panel);
				}
				else
				{
					ResetFocus();
					SetFocus(Store_Focus_Back);
				}
			}
			else if(focus == Store_Focus_Reset)
			{
				ResetFocus();
				SetFocus(Store_Focus_Tab);
			}
			else if(focus == Store_Focus_Portrait_1 || focus == Store_Focus_Portrait_2 || focus == Store_Focus_Portrait_3)
			{
				ResetFocus();
				panel_pos._y = 0;
				if(focus == Store_Focus_Portrait_1) panel_pos._x = 0;
				else if(focus == Store_Focus_Portrait_2) panel_pos._x = 1;
				else if(focus == Store_Focus_Portrait_3) panel_pos._x = 2;
				SetFocus(Store_Focus_Panel);
			}
		}
		else if(event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter || event->m_Key == s3eKeyOk)
		{
			if(focus == Store_Focus_Back)
			{
				button_back->SetStatus(3);
			}
			else if(focus == Store_Focus_Panel)
			{
				button_panel[panel_pos._y][panel_pos._x]->SetStatus(3);
			}
			else if(focus == Store_Focus_Reset)
			{
				button_reset->SetStatus(3);
			}
		}
	}
	else
	{
		bMouseBlocked = false;
		if(event->m_Key == s3eKeyAbsOk || event->m_Key == s3eKeyEnter)
		{
			if(focus == Store_Focus_Back)
			{
				button_back->Execute();
			}
			else if(focus == Store_Focus_Panel)
			{
				button_panel[panel_pos._y][panel_pos._x]->Execute();
				SetFocus(Store_Focus_Panel);
			}
			else if(focus == Store_Focus_Reset)
			{
				button_reset->Execute();
				button_reset->SetStatus(1);
			}
			else if(focus == Store_Focus_Portrait_2)
			{
				Utils::PlayFXSound(FX_Sound_CLICK);
				if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherRetarderUpgrades, archer_retarder_cost[0]);
				else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorBerserkUpgrades, warrior_berserker_cost[0]);
				else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerConfuserUpgrades, bowler_confuser_cost[0]);
				else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanHasteUpgrades, mage_haste_cost[0]);
			}
			else if (focus == Store_Focus_Portrait_3)
			{
				Utils::PlayFXSound(FX_Sound_CLICK);
				if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherPoisonerUpgrades, archer_poisoner_cost[0]);
				else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorTankUpgrades, warrior_tank_cost[0]);
				else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerRooterUpgrades, bowler_rooter_cost[0]);
				else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanMightUpgrades, mage_might_cost[0]);
			}
			else if(focus == Store_Focus_Portrait_1)
			{
				Utils::PlayFXSound(FX_Sound_CLICK);
			}
		} 
		//CHEAT: MUST BE DELETED BEFORE FINAL RELEASE!!!!!!!!
#ifdef IW_DEBUG
		else if(event->m_Key == s3eKey1)
		{
			for(int i = 0; i < 24; i++)
			{
				if(Stars_By_Level[i] < 3)
				{
					Stars_By_Level[i]++;
					UpdateRemainingStars();
					break;
				}
			}
		}
#endif
		else if(event->m_Key == s3eKeyEsc || event->m_Key == s3eKeyBack) 
		{
			mapSelectionState = MS_MapSelection;
		}
	}
}

void Store::RenderStars(UnitUpgrades unitupgrade, int req, int level, int panel)
{
	/*
	int margin_y = 5;
	int star_x = 60;

	int local_h = (int)(level0_h / (float)req);
	if(unitupgrade.level > level)
		for(int j = 0; j < req; j++)
			Utils::RenderSingleTexture(star[0], 
			CIwSVec2(button_panel[panel][level]->GetX() + star_x, button_panel[panel][level]->GetY() + (local_h + margin_y) * j));
	else
		for(int j = 0; j < req; j++)
			Utils::RenderSingleTexture(star[1], 
			CIwSVec2(button_panel[panel][level]->GetX() + star_x, button_panel[panel][level]->GetY() + (local_h + margin_y) * j));
	*/
}

void Store::UpgradeUnit()
{
	//IwTrace(Trace_Store, ("Upgrade unit"));
	if(tab == ARCHER_TAB)
	{
		if(panel_pos._x == 0)      UpgradeSpecificUnit(&ArcherBaseUpgrades, archer_base_cost, 0);
		else if(panel_pos._x == 1) UpgradeSpecificUnit(&ArcherRetarderUpgrades, archer_retarder_cost, 1);
		else if(panel_pos._x == 2) UpgradeSpecificUnit(&ArcherPoisonerUpgrades, archer_poisoner_cost, 2);
		//IwTrace(Trace_Store, ("Archer %d", panel_pos._x));
	}
	else if(tab == BOWLER_TAB)
	{
		if(panel_pos._x == 0)      UpgradeSpecificUnit(&BowlerStunerUpgrades,   bowler_stuner_cost, 0);
		else if(panel_pos._x == 1) UpgradeSpecificUnit(&BowlerConfuserUpgrades, bowler_confuser_cost, 1);
		else if(panel_pos._x == 2) UpgradeSpecificUnit(&BowlerRooterUpgrades,   bowler_rooter_cost, 2);
		//IwTrace(Trace_Store, ("Bowler %d", panel_pos._x));
	}
	else if(tab == WARRIOR_TAB)
	{
		if(panel_pos._x == 0)      UpgradeSpecificUnit(&WarriorBaseUpgrades, warrior_base_cost, 0);
		else if(panel_pos._x == 1) UpgradeSpecificUnit(&WarriorBerserkUpgrades, warrior_berserker_cost, 1);
		else if(panel_pos._x == 2) UpgradeSpecificUnit(&WarriorTankUpgrades, warrior_tank_cost, 2);
		//IwTrace(Trace_Store, ("Warrior %d", panel_pos._x));
	}
	else if(tab == MAGE_TAB)
	{
		if(panel_pos._x == 0)      UpgradeSpecificUnit(&ShamanHealerUpgrades, mage_heal_cost, 0);
		else if(panel_pos._x == 1) UpgradeSpecificUnit(&ShamanHasteUpgrades, mage_haste_cost, 1);
		else if(panel_pos._x == 2) UpgradeSpecificUnit(&ShamanMightUpgrades, mage_might_cost, 2);
		//IwTrace(Trace_Store, ("Mage %d", panel_pos._x));
	}
	UpdateRemainingStars();
}

void Store::UpgradeSpecificUnit(UnitUpgrades* unitupgrade, int req[3], int id)
{
	/*
	IwTrace(Trace_Store, ("%d %d", panel_pos._y, unitupgrade->level));
	IwTrace(Trace_Store, ("%d %d %d", req[0], req[1], req[2]));
	*/
	//IwTrace(TRIALVERSION, ("You cant upgrade this unit."));
	if(!bIsFullVersion && id != 0)
	{
		return;
	}

	if(unitupgrade->level == panel_pos._y + 1)
	{
		//buy
		if(num_remaining_stars >= req[panel_pos._y + 1])
		{
			Used_Stars += req[panel_pos._y + 1];
			unitupgrade->level++;
		}
	}
	else if(unitupgrade->level == (panel_pos._y + 2))
	{
		//sell
		//if(req[panel_pos._y + 2] != 0)
		//{
			Used_Stars -= req[panel_pos._y + 1];
			unitupgrade->level--;
		//}
	}
}

void Store::SetFocus(Store_Focus focus)
{
	if(focus == Store_Focus_Back) {
		button_back->IsFocused(true);
		bShowInformation = false;
	}
	else if(focus == Store_Focus_Reset) {
		button_reset->IsFocused(true);
		bShowInformation = false;
	}
	else if(focus == Store_Focus_Panel) {
		button_panel[panel_pos._y][panel_pos._x]->IsFocused(true);
		bShowInformation = true;
	}
	else if(focus == Store_Focus_Tab) {
		button_tab[tab_pos._x]->IsFocused(true);
		bShowInformation = false;
	}
	else if(focus == Store_Focus_Portrait_1)
	{
		bIsPortraitFocused[0] = true;
		bShowInformation = true;
	}
	else if(focus == Store_Focus_Portrait_2)
	{
		bIsPortraitFocused[1] = true;
		bShowInformation = true;
	}
	else if(focus == Store_Focus_Portrait_3)
	{
		bIsPortraitFocused[2] = true;
		bShowInformation = true;
	}

	this->focus = focus;
}

void Store::ResetFocus()
{
	button_reset->IsFocused(false);
	button_back->IsFocused(false);
	button_panel[panel_pos._y][panel_pos._x]->IsFocused(false);
	button_tab[tab_pos._x]->IsFocused(false);
	bIsPortraitFocused[0] = false;
	bIsPortraitFocused[1] = false;
	bIsPortraitFocused[2] = false;
	for(int i = 0; i < 4; i++)
		button_tab[i]->IsFocused(false);
}

void Store::ResetStars()
{
	ArcherBaseUpgrades.level     = 1;
	ArcherPoisonerUpgrades.level = 0;
	ArcherRetarderUpgrades.level = 0;
	BowlerStunerUpgrades.level   = 1;
	BowlerRooterUpgrades.level   = 0;
	BowlerConfuserUpgrades.level = 0;
	WarriorBaseUpgrades.level    = 1;
	WarriorTankUpgrades.level    = 0;
	WarriorBerserkUpgrades.level = 0;
	ShamanMightUpgrades.level    = 0;
	ShamanHasteUpgrades.level    = 0;
	ShamanHealerUpgrades.level   = 1;
	Used_Stars = 0;
	UpdateRemainingStars();
	LoadClassInfo();
}

void Store::LoadClassInfo()
{
	if(tab == ARCHER_TAB)
	{
		LoadSpecficicInfo(ArcherBaseUpgrades, 0);
		LoadSpecficicInfo(ArcherPoisonerUpgrades, 2);
		LoadSpecficicInfo(ArcherRetarderUpgrades, 1);
	}
	else if(tab == BOWLER_TAB)
	{
		LoadSpecficicInfo(BowlerStunerUpgrades, 0);
		LoadSpecficicInfo(BowlerRooterUpgrades, 2);
		LoadSpecficicInfo(BowlerConfuserUpgrades, 1);
	}
	else if(tab == WARRIOR_TAB)
	{
		LoadSpecficicInfo(WarriorBaseUpgrades, 0);
		LoadSpecficicInfo(WarriorBerserkUpgrades, 1);
		LoadSpecficicInfo(WarriorTankUpgrades, 2);
	}
	else if(tab == MAGE_TAB)
	{
		LoadSpecficicInfo(ShamanHasteUpgrades, 1);
		LoadSpecficicInfo(ShamanHealerUpgrades, 0);
		LoadSpecficicInfo(ShamanMightUpgrades, 2);
	}
}

void Store::LoadSpecficicInfo(UnitUpgrades unitupgrade, int idx)
{
	for(int i = 0; i < 2; i++)
	{
		if(unitupgrade.level > i+1)
		{
			button_panel[i][idx]->SetImage(0);
		}
		else
		{
			button_panel[i][idx]->SetImage(1);
		}
	}
}

void Store::UnlockCharacter(UnitUpgrades* unitupgrade, int cost)
{
	if(!bIsFullVersion)
	{
		return;
	}

	if(unitupgrade->level == 0 && num_remaining_stars >= cost)
	{
		unitupgrade->level++;
		Used_Stars += cost;
	}
	else if(unitupgrade->level == 1)
	{
		unitupgrade->level--;
		Used_Stars -= cost;
		IwTrace(TRACE_STORE, ("Reduciendo nivel"));
	}

	UpdateRemainingStars();
}

void Store::UpdateInformationWindow()
{
	if(tab == ARCHER_TAB)
	{
		if(panel_pos._x == 0)
		{
			portrait_name = "SNIPER";
			sprintf(current_level, "lvl %d", ArcherBaseUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ArcherBaseUpgrades.level >= (panel_pos._y + 2)) bShowThirdCol = false;
			else bShowThirdCol = true;

			bHasSkill = false;
			sprintf(skill_name[1], "%s", "--");
			if(ArcherBaseUpgrades.level != 0) SetAttributes(SelknamAtt_Archer_Base, ArcherBaseUpgrades.level);
			else SetAttributes(SelknamAtt_Archer_Base, 1);

			int level = panel_pos._y + 2;
			if(level == ArcherBaseUpgrades.level + 1)
				SetBonusAttributes(SelknamAtt_Archer_Base, level);
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 1)
		{
			portrait_name = "SLOWER";
			sprintf(current_level, "lvl %d", (ArcherRetarderUpgrades.level == 0) ? 1 : ArcherRetarderUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ArcherRetarderUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Slow");
			sprintf(skill_name[1], "lvl %d", (ArcherRetarderUpgrades.level == 0) ? 1 : ArcherRetarderUpgrades.level);
			if(ArcherRetarderUpgrades.level != 0) 
			{
				SetAttributes(SelknamAtt_Archer_Retarder, ArcherRetarderUpgrades.level);
			}
			else SetAttributes(SelknamAtt_Archer_Retarder, 1);

			int level = panel_pos._y + 2;
			if(level == ArcherRetarderUpgrades.level + 1 && ArcherRetarderUpgrades.level != 0)
			{
				SetBonusAttributes(SelknamAtt_Archer_Retarder, level);
			}
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 2)
		{
			portrait_name = "POISONER";
			sprintf(current_level, "lvl %d", (ArcherPoisonerUpgrades.level == 0) ? 1 : ArcherPoisonerUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ArcherPoisonerUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Poison");
			sprintf(skill_name[1], "lvl %d", (ArcherPoisonerUpgrades.level == 0) ? 1 : ArcherPoisonerUpgrades.level);
			if(ArcherPoisonerUpgrades.level != 0) SetAttributes(SelknamAtt_Archer_Poisoner, ArcherPoisonerUpgrades.level);
			else SetAttributes(SelknamAtt_Archer_Poisoner, 1);

			int level = panel_pos._y + 2;
			if(level == ArcherPoisonerUpgrades.level + 1 && ArcherPoisonerUpgrades.level != 0)
				SetBonusAttributes(SelknamAtt_Archer_Poisoner, level);
			else
				SetAttributesToUnknow(2);
		}
	}
	else if(tab == BOWLER_TAB)
	{
		if(panel_pos._x == 0)
		{
			portrait_name = "STUNNER";
			sprintf(current_level, "lvl %d", BowlerStunerUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(BowlerStunerUpgrades.level >= (panel_pos._y + 2)) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Stun");
			sprintf(skill_name[1], "lvl %d", BowlerStunerUpgrades.level);
			if(BowlerStunerUpgrades.level != 0) SetAttributes(SelknamAtt_Bowler_Stuner, BowlerStunerUpgrades.level);
			else SetAttributesToUnknow(1);

			int level = panel_pos._y + 2;
			if(level == BowlerStunerUpgrades.level + 1)
				SetBonusAttributes(SelknamAtt_Bowler_Stuner, level);
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 1)
		{
			portrait_name = "CONFUSER";
			sprintf(current_level, "lvl %d", (BowlerConfuserUpgrades.level == 0) ? 1 : BowlerConfuserUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(BowlerConfuserUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Confusion");
			sprintf(skill_name[1], "lvl %d", (BowlerConfuserUpgrades.level == 0) ? 1 : BowlerConfuserUpgrades.level);
			if(BowlerConfuserUpgrades.level != 0) SetAttributes(SelknamAtt_Bowler_Confuser, BowlerConfuserUpgrades.level);
			else SetAttributes(SelknamAtt_Bowler_Confuser, 1);

			int level = panel_pos._y + 2;
			if(level == BowlerConfuserUpgrades.level + 1 && BowlerConfuserUpgrades.level != 0)
				SetBonusAttributes(SelknamAtt_Bowler_Confuser, level);
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 2)
		{
			portrait_name = "ROOTER";
			sprintf(current_level, "lvl %d", (BowlerRooterUpgrades.level == 0) ? 1 : BowlerRooterUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(BowlerRooterUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Root");
			sprintf(skill_name[1], "lvl %d", (BowlerRooterUpgrades.level == 0) ? 1 : BowlerRooterUpgrades.level);
			if(BowlerRooterUpgrades.level != 0) SetAttributes(SelknamAtt_Bowler_Rooter, BowlerRooterUpgrades.level);
			else SetAttributes(SelknamAtt_Bowler_Rooter, 1);

			int level = panel_pos._y + 2;
			if(level == BowlerRooterUpgrades.level + 1 && BowlerRooterUpgrades.level != 0)
				SetBonusAttributes(SelknamAtt_Bowler_Rooter, level);
			else
				SetAttributesToUnknow(2);
		}
	}
	else if(tab == WARRIOR_TAB)
	{
		if(panel_pos._x == 0)
		{
			portrait_name = "WARRIOR";
			sprintf(current_level, "lvl %d", WarriorBaseUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(WarriorBaseUpgrades.level >= (panel_pos._y + 2)) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = false;
			sprintf(skill_name[1], "%s", "--");
			if(WarriorBaseUpgrades.level != 0) SetAttributes(SelknamAtt_Warrior_Base, WarriorBaseUpgrades.level);
			else SetAttributesToUnknow(1);

			int level = panel_pos._y + 2;
			if(level == WarriorBaseUpgrades.level + 1)
				SetBonusAttributes(SelknamAtt_Warrior_Base, level);
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 1)
		{
			portrait_name = "BERSERK";
			sprintf(current_level, "lvl %d", (WarriorBerserkUpgrades.level == 0) ? 1 : WarriorBerserkUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(WarriorBerserkUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = false;
			sprintf(skill_name[1], "%s", "--");
			if(WarriorBerserkUpgrades.level != 0) SetAttributes(SelknamAtt_Warrior_Berserk, WarriorBerserkUpgrades.level);
			else SetAttributes(SelknamAtt_Warrior_Berserk, 1);

			int level = panel_pos._y + 2;
			if(level == WarriorBerserkUpgrades.level + 1 && WarriorBerserkUpgrades.level != 0)
				SetBonusAttributes(SelknamAtt_Warrior_Berserk, level);
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 2)
		{
			portrait_name = "TANK";
			sprintf(current_level, "lvl %d", (WarriorTankUpgrades.level == 0) ? 1 : WarriorTankUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(WarriorTankUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = false;
			sprintf(skill_name[1], "%s", "--");
			if(WarriorTankUpgrades.level != 0) SetAttributes(SelknamAtt_Warrior_Tank, WarriorTankUpgrades.level);
			else SetAttributes(SelknamAtt_Warrior_Tank, 1);

			int level = panel_pos._y + 2;
			if(level == WarriorTankUpgrades.level + 1 && WarriorTankUpgrades.level != 0)
				SetBonusAttributes(SelknamAtt_Warrior_Tank, level);
			else
				SetAttributesToUnknow(2);
		}
	}
	else if(tab == MAGE_TAB)
	{
		if(panel_pos._x == 0)
		{
			portrait_name = "HEALER";
			sprintf(current_level, "lvl %d", ShamanHealerUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ShamanHealerUpgrades.level >= (panel_pos._y + 2)) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Heal");
			sprintf(skill_name[1], "lvl %d", ShamanHealerUpgrades.level);
			if(ShamanHealerUpgrades.level != 0) SetAttributes(SelknamAtt_Shaman_Healer, ShamanHealerUpgrades.level);
			else SetAttributesToUnknow(1);

			int level = panel_pos._y + 2;
			if(level == ShamanHealerUpgrades.level + 1)
				SetBonusAttributes(SelknamAtt_Shaman_Healer, level);
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 1)
		{
			portrait_name = "SPEEDER";
			sprintf(current_level, "lvl %d", (ShamanHasteUpgrades.level == 0) ? 1 : ShamanHasteUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ShamanHasteUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Haste");
			sprintf(skill_name[1], "lvl %d", (ShamanHasteUpgrades.level == 0) ? 1 : ShamanHasteUpgrades.level);
			if(ShamanHasteUpgrades.level != 0) SetAttributes(SelknamAtt_Shaman_Haste, ShamanHasteUpgrades.level);
			else SetAttributes(SelknamAtt_Shaman_Haste, 1);

			int level = panel_pos._y + 2;
			if(level == ShamanHasteUpgrades.level + 1 && ShamanHasteUpgrades.level != 0)
				SetBonusAttributes(SelknamAtt_Shaman_Haste, level);
			else
				SetAttributesToUnknow(2);
		}
		else if(panel_pos._x == 2)
		{
			portrait_name = "MIGHTER";
			sprintf(current_level, "lvl %d", (ShamanMightUpgrades.level == 0) ? 1 : ShamanMightUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ShamanMightUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", "Might");
			sprintf(skill_name[1], "lvl %d", (ShamanMightUpgrades.level == 0) ? 1 : ShamanMightUpgrades.level);
			if(ShamanMightUpgrades.level != 0) SetAttributes(SelknamAtt_Shaman_Might, ShamanMightUpgrades.level);
			else SetAttributes(SelknamAtt_Shaman_Might, 1);

			int level = panel_pos._y + 2;
			if(level == ShamanMightUpgrades.level + 1 && ShamanMightUpgrades.level != 0)
				SetBonusAttributes(SelknamAtt_Shaman_Might, level);
			else
				SetAttributesToUnknow(2);
		}
	}
}

void Store::Update()
{
	if(bResetStoreStats)
	{
		ResetStars();
		bResetStoreStats = false;
	}

	button_tab[(int)tab]->SetStatus(1);

	UpdateInformationWindow();
}

void Store::FocusTab(Selknam_Tab t)
{
	if(t == tab) 
		return;

	button_tab[(int)tab]->SetStatus(2);
	tab = t;
}

void Store::SetAttributesToUnknow(int i)
{
	sprintf(health[i],       "%s", "?");
	sprintf(damage[i],       "%s", "?");
	sprintf(speed[i],        "%s", "?");
	sprintf(attack_speed[i], "%s", "?");
	sprintf(cast_speed[i],   "%s", "?");
	sprintf(range[i],        "%s", "?");
	sprintf(cost[i],         "%s", "?");
	sprintf(move_cost[i],    "%s", "?");
	bValuesAreUnknow = true;
}

void Store::SetAttributes(PawnAttributes att[], int level)
{
	sprintf(health[1],       "%d", att[level - 1].health);
	sprintf(damage[1],       "%d", att[level - 1].damage);
	sprintf(speed[1],        "%d", att[level - 1].speed);
	sprintf(attack_speed[1], "%d", att[level - 1].attackspeed);
	sprintf(cast_speed[1],   "%d", att[level - 1].casting_speed);
	sprintf(range[1],        "%d", att[level - 1].range);
	sprintf(cost[1],         "%d", att[level - 1].cost);
	sprintf(move_cost[1],    "%d", att[level - 1].move_cost);
}

void Store::SetBonusAttributes(PawnAttributes att[], int level)
{
	if(att[level - 1].health - att[level - 2].health == 0) sprintf(health[2],       "%s", "--");
	else sprintf(health[2],  "+%d", att[level - 1].health - att[level - 2].health);

	if(att[level - 1].damage - att[level - 2].damage == 0) sprintf(damage[2],       "%s", "--");
	else sprintf(damage[2],       "+%d", att[level - 1].damage - att[level - 2].damage);

	if(att[level - 1].speed - att[level - 2].speed == 0) sprintf(speed[2],       "%s", "--");
	else sprintf(speed[2],        "+%d", att[level - 1].speed - att[level - 2].speed);

	if(att[level - 1].attackspeed - att[level - 2].attackspeed == 0) sprintf(attack_speed[2],       "%s", "--");
	else sprintf(attack_speed[2], "+%d", att[level - 1].attackspeed - att[level - 2].attackspeed);

	if(att[level - 1].casting_speed - att[level - 2].casting_speed == 0) sprintf(cast_speed[2],       "%s", "--");
	else sprintf(cast_speed[2],   "+%d", att[level - 1].casting_speed - att[level - 2].casting_speed);

	if(att[level - 1].range - att[level - 2].range == 0) sprintf(range[2],       "%s", "--");
	else sprintf(range[2],        "+%d", att[level - 1].range - att[level - 2].range);

	sprintf(cost[2],         "%d",  att[level - 1].cost);

	sprintf(move_cost[2],    "%d",  att[level - 1].move_cost);

	sprintf(skill_name[2], "lvl %d", level);
	bValuesAreUnknow = false;
}

void Store::MouseHack()
{
	if(focus == Store_Focus_Back)
	{
		button_back->Execute();
	}
	else if(focus == Store_Focus_Panel)
	{
		button_panel[panel_pos._y][panel_pos._x]->Execute();
		SetFocus(Store_Focus_Panel);
	}
	else if(focus == Store_Focus_Reset)
	{
		button_reset->Execute();
		button_reset->SetStatus(1);
	}
	else if(focus == Store_Focus_Portrait_2)
	{
		Utils::PlayFXSound(FX_Sound_CLICK);
		if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherRetarderUpgrades, archer_retarder_cost[0]);
		else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorBerserkUpgrades, warrior_berserker_cost[0]);
		else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerConfuserUpgrades, bowler_confuser_cost[0]);
		else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanHasteUpgrades, mage_haste_cost[0]);
	}
	else if (focus == Store_Focus_Portrait_3)
	{
		Utils::PlayFXSound(FX_Sound_CLICK);
		if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherPoisonerUpgrades, archer_poisoner_cost[0]);
		else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorTankUpgrades, warrior_tank_cost[0]);
		else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerRooterUpgrades, bowler_rooter_cost[0]);
		else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanMightUpgrades, mage_might_cost[0]);
	}
	else if(focus == Store_Focus_Portrait_1)
	{
		Utils::PlayFXSound(FX_Sound_CLICK);
	}
}

void Store::ActiveButtonMouseHack()
{
	if(focus == Store_Focus_Back)
	{
		button_back->SetStatus(3);
	}
	else if(focus == Store_Focus_Panel)
	{
		button_panel[panel_pos._y][panel_pos._x]->SetStatus(3);
	}
	else if(focus == Store_Focus_Reset)
	{
		button_reset->SetStatus(3);
	}
}