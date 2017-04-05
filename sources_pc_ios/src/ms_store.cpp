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
#include "localization.h"
#include "utils.h"
#include "parserhelper.h"
#include "savedatahelper.h"
#include "common.h"
#include "texturehelper.h"

Store* storeRef = NULL;

//Keyboard handler
int32 KeyboardHandler_store(void* sys, void*)
{
	return 0;
}

//Mouse Motion
void HandleMotion_store(s3ePointerMotionEvent* event) 
{
	if(storeRef == NULL)
		return;

	storeRef->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClick_store(s3ePointerEvent *event) 
{
	if(storeRef == NULL)
		return;

	storeRef->UpdateClickEvents(event);
}

//CUSTOM CLICK/KEY FUNCTIONS
void Store_OnClick_Back()
{
	if(tutorial->IsActive()) return;

	mapSelectionState = MS_MapSelection;

	//Achievement nº 3.
	//Selknam Authority: Sube una clase completa de selknams a nivel 3.(arqueros, boleadores, guerreros o magos)
	if((ArcherBaseUpgrades.level==3 && ArcherPoisonerUpgrades.level==3 && ArcherRetarderUpgrades.level==3) ||
		(BowlerStunerUpgrades.level==3 && BowlerRooterUpgrades.level==3 && BowlerConfuserUpgrades.level==3) ||
		(WarriorBaseUpgrades.level==3 && WarriorTankUpgrades.level==3 && WarriorBerserkUpgrades.level==3) ||
		(ShamanMightUpgrades.level==3 && ShamanHasteUpgrades.level==3 && ShamanHealerUpgrades.level==3))
	{
		achievementsHelper->SetAchievementCompletedById(2);
	}

	//Achievement nº 12.
	//Selknam Mastery: Sube todas las unidades a nivel 3.(arqueros, boleadores, guerreros y magos)
	if((ArcherBaseUpgrades.level==3 && ArcherPoisonerUpgrades.level==3 && ArcherRetarderUpgrades.level==3) &&
		(BowlerStunerUpgrades.level==3 && BowlerRooterUpgrades.level==3 && BowlerConfuserUpgrades.level==3) &&
		(WarriorBaseUpgrades.level==3 && WarriorTankUpgrades.level==3 && WarriorBerserkUpgrades.level==3) &&
		(ShamanMightUpgrades.level==3 && ShamanHasteUpgrades.level==3 && ShamanHealerUpgrades.level==3))
	{
		achievementsHelper->SetAchievementCompletedById(11);
	}
}

void Store_OnMotion_Back()
{
	if(storeRef->focus != Store_Focus_Back)
	{
		storeRef->ResetFocus();
		storeRef->SetFocus(Store_Focus_Back);
		storeRef->ClearFocus();
	}
}

void Store_OnMotion_Reset()
{
	if(storeRef->focus != Store_Focus_Reset)
	{
		storeRef->ResetFocus();
		storeRef->SetFocus(Store_Focus_Reset);
		storeRef->ClearFocus();
	}
}

void Store_OnClick_Reset()
{
	if(tutorial->IsActive()) return;

	bResetStoreStats = false;
	popUpState = GSP_ResetStore;
	bPausedStore = true;
}

void Store_OnClick_SelknamInfo()
{
	storeRef->bViewingDetailedInfo = true;

	storeRef->OpenDetailedInfo();
}

void Store_GOTO_WarriorTab() { if(tutorial->IsActive()) return; storeRef->FocusTab(WARRIOR_TAB); storeRef->LoadClassInfo(); storeRef->ClearFocus();}
void Store_GOTO_MageTab()    { if(tutorial->IsActive()) return; storeRef->FocusTab(MAGE_TAB);    storeRef->LoadClassInfo(); storeRef->ClearFocus();}
void Store_GOTO_BowlerTab()  { if(tutorial->IsActive()) return; storeRef->FocusTab(BOWLER_TAB);  storeRef->LoadClassInfo(); storeRef->ClearFocus();}
void Store_GOTO_ArcherTab()  { if(tutorial->IsActive()) return; storeRef->FocusTab(ARCHER_TAB);  storeRef->LoadClassInfo(); storeRef->ClearFocus();}

void Store_CLICK_Panel()     
{ 
      
	if(tutorial->currBreakpoint == Tutorial_BreakPoint_UpgradeArcher)
	{
		if(storeRef->panel_pos._x == 0 && storeRef->panel_pos._y == 0)
		{
			storeRef->UpgradeUnit(); 
			storeRef->LoadClassInfo();  
			
			IwTrace(ASDASRC, ("%d", ArcherBaseUpgrades.level));

			if(ArcherBaseUpgrades.level >= 2)
				tutorial->ExecuteBreakpoint();
		}
	}
	else
	{
		storeRef->UpgradeUnit(); 
		storeRef->LoadClassInfo();  
	}
}
//END CUSTOM CLICK/KEY FUNCTIONS

void onCloseAction_Store()
{
	storeRef->bViewingDetailedInfo = false;
}

#if defined DESKTOPPLATFORM
void Store_Xbox360_Left()
{
	storeRef->focusManager.ChangeFocus(Xbox360_Left);
}

void Store_Xbox360_Right()
{
	storeRef->focusManager.ChangeFocus(Xbox360_Right);
}

void Store_Xbox360_Up()
{
	storeRef->focusManager.ChangeFocus(Xbox360_Up);
}

void Store_Xbox360_Down()
{
	storeRef->focusManager.ChangeFocus(Xbox360_Down);
}

void Store_Xbox360_Action()
{
	storeRef->focusManager.ExecuteFocusedObject();
}
#endif

void Store_C1R1() { storeRef->ShowInformation(0, -1); };
void Store_C1R2() { storeRef->ShowInformation(0, 0); };
void Store_C1R3() { storeRef->ShowInformation(0, 1); };
void Store_C2R1() { storeRef->ShowInformation(1, -1); };
void Store_C2R2() { storeRef->ShowInformation(1, 0); };
void Store_C2R3() { storeRef->ShowInformation(1, 1); };
void Store_C3R1() { storeRef->ShowInformation(2, -1); };
void Store_C3R2() { storeRef->ShowInformation(2, 0); };
void Store_C3R3() { storeRef->ShowInformation(2, 1); };
void Store_UnlockCharacerC2() { storeRef->UnlockCharacterSecondCol(); };
void Store_UnlockCharacerC3() { storeRef->UnlockCharacterThirdCol(); };

Store::Store()
{
	bIsActiveObj = false;

	storeRef = this;
	background = new Background(0,0,0, 170);
	moreInfoBackground = new Background(0, 0, 0, 170);

	titleXPos = Utils::GetFixedScaledInt(260);
	titleYPos = Utils::GetScaledInt(-50);

	pannelLeftXPos = Utils::GetFixedScaledInt(160);
	pannelLeftYPos = Utils::GetScaledInt(230);

	if(currentGameHeight <= 640)
	{
		titleXPos = Utils::GetFixedScaledInt(220);
		titleYPos = Utils::GetScaledInt(-70);
		pannelLeftXPos = Utils::GetFixedScaledInt(110);
		pannelLeftYPos = Utils::GetScaledInt(170);
	}
	if(currentGameHeight <= 600)
	{
		pannelLeftXPos = Utils::GetFixedScaledInt(110);
		pannelLeftYPos = Utils::GetScaledInt(140);
	}
	if(currentGameHeight <= 540)
	{
		pannelLeftXPos = Utils::GetFixedScaledInt(50);
		pannelLeftYPos = Utils::GetScaledInt(100);
	}

	pannelRightXPos = pannelLeftXPos + 410;
	pannelRightYPos = pannelLeftYPos;

	infoXOffset = 20;
	infoYOffset = 0;

	int xButtonPanel = pannelLeftXPos + 35;
	int yButtonPanel = pannelLeftYPos + 185;

	portraitXPos = pannelLeftXPos + 35;
	portraitYPos = pannelLeftYPos + 10;

	TextureData data1;
	data1 = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data1, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data1, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data1, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data1, 1, 4, 2);

	int btnBackWidth = TextureHelper::GetDimension(buttonRenderData.state[0].Pressed).x;
	int btnBackHeight = TextureHelper::GetDimension(buttonRenderData.state[0].Pressed).y;

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

	bViewingDetailedInfo = false;

	selknamInfoRenderData.numStates = 2;
	TextureData _selknamInfoRenderData = TextureHelper::GetTextureNamed("btn_info");
	selknamInfoRenderData.state[0].RollOn   = TextureHelper::GetRenderData(_selknamInfoRenderData, 2, 3, 0);
	selknamInfoRenderData.state[0].RollOut  = TextureHelper::GetRenderData(_selknamInfoRenderData, 2, 3, 0);
	selknamInfoRenderData.state[0].Pressed  = TextureHelper::GetRenderData(_selknamInfoRenderData, 2, 3, 1);
	selknamInfoRenderData.state[0].Inactive = TextureHelper::GetRenderData(_selknamInfoRenderData, 2, 3, 0);

	//This code is only for testing issues and it has to be deleted once the real image is available.
	int size = 32;
	selknamInfoRenderData.state[0].RollOn.width = size;
	selknamInfoRenderData.state[0].RollOn.height = size;
	selknamInfoRenderData.state[0].Pressed.width = size;
	selknamInfoRenderData.state[0].Pressed.height = size;
	selknamInfoRenderData.state[0].Inactive.width = size;
	selknamInfoRenderData.state[0].Inactive.height = size;
	selknamInfoRenderData.state[0].RollOut.width = size;
	selknamInfoRenderData.state[0].RollOut.height = size;
	
	selknamInfoButton = new Button(TextureHelper::GetCIwTexture(selknamInfoRenderData.state[0].RollOut.atlasName), &selknamInfoRenderData, pannelRightXPos + 170 + infoXOffset, pannelRightYPos + 130 + infoYOffset);
	selknamInfoButton->OnClick(&Store_OnClick_SelknamInfo);
	
	TextureData btnStoreLv1TextureData = TextureHelper::GetTextureNamed("btn_store_level_1");
	btnStoreLv1RenderData[0] = TextureHelper::GetRenderData(btnStoreLv1TextureData, 1, 2, 0);
	btnStoreLv1RenderData[1] = TextureHelper::GetRenderData(btnStoreLv1TextureData, 1, 2, 1);

	starCounterData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_infocard_star"));
	
	if(starCounterData.bRotate)
	{
		starCounterData.width = 139;
		starCounterData.height = 196;
	}
	else
	{
		starCounterData.width = 196;
		starCounterData.height = 139;
	}



	remaining_stars_color = 0xffffffff;
	remaining_stars_position_posX = currentGameWidth - 250;
	remaining_stars_position_posY = 0;
	if(currentGameHeight < 640)
	{
		remaining_stars_position_posX = currentGameWidth - 245;
		remaining_stars_position_posY = -38;
	}

	remaining_stars_position_txt = CIwRect(remaining_stars_position_posX - 20, remaining_stars_position_posY + 22, starCounterData.width, starCounterData.height);

	if(currentGameHeight < 600)
	{
		starCounterData.width = (int)((float)starCounterData.width * 0.75f);
		starCounterData.height = (int)((float)starCounterData.height * 0.75f);
		remaining_stars_position_posX = currentGameWidth - 160;
		remaining_stars_position_posY = -20;
		remaining_stars_position_txt = CIwRect(remaining_stars_position_posX - 20, remaining_stars_position_posY + 22, starCounterData.width, starCounterData.height);
	}


	//portrait uv settings
	portrait_uvX = 0.2f;
	portrait_uvY = 0.125f;

	TextureData data = TextureHelper::GetTextureNamed("btn_store_reset");
	resetButtonData.numStates = 1;
	resetButtonData.state[0].RollOn   = TextureHelper::GetRenderData(data, 1, 4, 1);
	resetButtonData.state[0].RollOut  = TextureHelper::GetRenderData(data, 1, 4, 0);
	resetButtonData.state[0].Pressed  = TextureHelper::GetRenderData(data, 1, 4, 2);
	resetButtonData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 0);

	int button_resetYPos = 0;
	if(currentGameHeight < 640)
		button_resetYPos = -30;

	button_reset = new Button(TextureHelper::GetCIwTexture(resetButtonData.state[0].RollOut.atlasName), &resetButtonData, 50, button_resetYPos);
	button_reset->OnClick(&Store_OnClick_Reset);
	button_reset->OnRollOn(&Store_OnMotion_Reset);

	int bntBackDiff = 0;
	if(currentGameHeight <= 640)
	{
		bntBackDiff = 15;
	}

	button_back = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, (int)((currentGameWidth - btnBackWidth) * 0.5f), currentGameHeight - Utils::GetScaledInt(110));
	button_back->SetText(loc_ui_accept);
	button_back->OnClick(&Store_OnClick_Back);
	button_back->OnRollOn(&Store_OnMotion_Back);

	//Mouse-Keyboard events.
	bKeyboardBlocked = false;
	bMouseBlocked = false;

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

	button_panel[0][0] = new Button(TextureHelper::GetCIwTexture(lv2ButtonData.state[0].RollOut.atlasName), &lv2ButtonData, xButtonPanel  + (96 + 20) * 0, yButtonPanel);
	button_panel[0][0]->OnClick(&Store_CLICK_Panel);
	button_panel[0][0]->SetOnFocusedCallback(&Store_C1R2);

	button_panel[1][0] = new Button(TextureHelper::GetCIwTexture(lv3ButtonData.state[0].RollOut.atlasName), &lv3ButtonData, xButtonPanel + (96 + 20) * 0, 95 + yButtonPanel);
	button_panel[1][0]->OnClick(&Store_CLICK_Panel);
	button_panel[1][0]->SetOnFocusedCallback(&Store_C1R3);

	button_panel[0][1] = new Button(TextureHelper::GetCIwTexture(lv2ButtonData.state[0].RollOut.atlasName), &lv2ButtonData, xButtonPanel + (96 + 20) * 1, yButtonPanel);
	button_panel[0][1]->OnClick(&Store_CLICK_Panel);
	button_panel[0][1]->SetOnFocusedCallback(&Store_C2R2);

	button_panel[1][1] = new Button(TextureHelper::GetCIwTexture(lv3ButtonData.state[0].RollOut.atlasName), &lv3ButtonData, xButtonPanel + (96 + 20) * 1, 95 + yButtonPanel);
	button_panel[1][1]->OnClick(&Store_CLICK_Panel);
	button_panel[1][1]->SetOnFocusedCallback(&Store_C2R3);

	button_panel[0][2] = new Button(TextureHelper::GetCIwTexture(lv2ButtonData.state[0].RollOut.atlasName), &lv2ButtonData, xButtonPanel + (96 + 20) * 2, yButtonPanel);
	button_panel[0][2]->OnClick(&Store_CLICK_Panel);
	button_panel[0][2]->SetOnFocusedCallback(&Store_C3R2);

	button_panel[1][2] = new Button(TextureHelper::GetCIwTexture(lv3ButtonData.state[0].RollOut.atlasName), &lv3ButtonData, xButtonPanel + (96 + 20) * 2, 95 + yButtonPanel);
	button_panel[1][2]->OnClick(&Store_CLICK_Panel);
	button_panel[1][2]->SetOnFocusedCallback(&Store_C3R3);

	portraits[0].onFocused = &Store_C1R1;
	portraits[0].onExecute = NULL;

	portraits[1].onFocused = &Store_C2R1;
	portraits[1].onExecute = &Store_UnlockCharacerC2;

	portraits[2].onFocused = &Store_C3R1;
	portraits[2].onExecute = &Store_UnlockCharacerC3;

	int tab_margin_x = 1;
	int tab_x = pannelLeftXPos + 35;
	int tab_y = pannelLeftYPos - 65;
	int btnWidth = TextureHelper::GetDimension(btnStoreArcher.state[0].Pressed).x;

	int tabBtnXPos[4];
	int tabBtnYPos[4];

	tabBtnXPos[0] = tab_x + (btnWidth + tab_margin_x) * 0;
	tabBtnXPos[1] = tab_x + (btnWidth + tab_margin_x) * 1;
	tabBtnXPos[2] = tab_x + (btnWidth + tab_margin_x) * 2;
	tabBtnXPos[3] = tab_x + (btnWidth + tab_margin_x) * 3;

	tabBtnYPos[0] = tab_y;
	tabBtnYPos[1] = tab_y;
	tabBtnYPos[2] = tab_y;
	tabBtnYPos[3] = tab_y;

	if(currentGameHeight <= 640)
	{
		tab_x = pannelLeftXPos - 65;
		tab_y = pannelLeftYPos + 40;

		tabBtnXPos[0] = tab_x;
		tabBtnXPos[1] = tab_x;
		tabBtnXPos[2] = tab_x;
		tabBtnXPos[3] = tab_x;

		tabBtnYPos[0] = tab_y + (btnWidth + tab_margin_x) * 0;
		tabBtnYPos[1] = tab_y + (btnWidth + tab_margin_x) * 1;
		tabBtnYPos[2] = tab_y + (btnWidth + tab_margin_x) * 2;
		tabBtnYPos[3] = tab_y + (btnWidth + tab_margin_x) * 3;
	}
	
	button_tab[0] = new Button(TextureHelper::GetCIwTexture(btnStoreArcher.state[0].RollOut.atlasName), &btnStoreArcher, tabBtnXPos[0], tabBtnYPos[0]);
	button_tab[1] = new Button(TextureHelper::GetCIwTexture(btnStoreBowler.state[0].RollOut.atlasName), &btnStoreBowler, tabBtnXPos[1], tabBtnYPos[1]);
	button_tab[2] = new Button(TextureHelper::GetCIwTexture(btnStoreWarrior.state[0].RollOut.atlasName), &btnStoreWarrior, tabBtnXPos[2], tabBtnYPos[2]);
	button_tab[3] = new Button(TextureHelper::GetCIwTexture(btnStoreMage.state[0].RollOut.atlasName), &btnStoreMage, tabBtnXPos[3], tabBtnYPos[3]);

	button_tab[0]->OnClick(&Store_GOTO_ArcherTab);
	button_tab[0]->SetOnFocusedCallback(&Store_GOTO_ArcherTab);

	button_tab[1]->OnClick(&Store_GOTO_BowlerTab);
	button_tab[1]->SetOnFocusedCallback(&Store_GOTO_BowlerTab);
	
	button_tab[2]->OnClick(&Store_GOTO_WarriorTab);
	button_tab[2]->SetOnFocusedCallback(&Store_GOTO_WarriorTab);

	button_tab[3]->OnClick(&Store_GOTO_MageTab);
	button_tab[3]->SetOnFocusedCallback(&Store_GOTO_MageTab);

	TextureData buttonData = TextureHelper::GetTextureNamed("btn_popup_long");
	button = new ButtonRenderData();
	button->numStates = 1;
	button->state[0].RollOut  = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	button->state[0].RollOn   = TextureHelper::GetRenderData(buttonData, 1, 3, 1);
	button->state[0].Pressed  = TextureHelper::GetRenderData(buttonData, 1, 3, 2);
	button->state[0].Inactive = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	int buttonWidth = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).x;

	xPos = (int)((currentGameWidth - (32 + 24 * 16)) * 0.5f);
	yPos = (int)((currentGameHeight - (32 + 8 * 16)) * 0.5f);
	w = 32 + 24 * 16;
	h = 32 + 8 * 16;

	pannelBLengthX = 36;
	pannelBLengthY = 23;

	xPosDetailed = (int)((currentGameWidth - (32 + pannelBLengthX * 16)) * 0.5f);
	yPosDetailed = (int)((currentGameHeight - (32 + pannelBLengthY * 16)) * 0.5f);
	if(currentGameHeight <= 600)
		yPosDetailed = yPosDetailed - 25;
	wDetailed = 32 + pannelBLengthX * 16;
	hDetailed = 32 + pannelBLengthY * 16;

	btnClose = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, xPosDetailed + (int)((wDetailed - buttonWidth) * 0.5f), yPosDetailed + hDetailed - 10);
	btnClose->OnClick(&onCloseAction_Store);
	btnClose->SetText(loc_ui_close);
	btnClose->SetStyle(font_komikadisplaybold18, 0, -15);
	
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


	sprintf(health[0],       "%s", loc_upgrade_health);
	sprintf(damage[0],       "%s", loc_upgrade_damage);
	sprintf(speed[0],        "%s", loc_upgrade_speed);
	sprintf(attack_speed[0], "%s", loc_upgrade_attackSpeed);
	sprintf(cast_speed[0],   "%s", loc_upgrade_castSpeed);
	sprintf(range[0],        "%s", loc_upgrade_range);
	sprintf(cost[0],         "%s", loc_upgrade_cost);
	sprintf(skill_name[0],   "%s", loc_upgrade_skill);
	sprintf(health[1],       "%s", "?");
	sprintf(damage[1],       "%s", "?");
	sprintf(speed[1],        "%s", "?");
	sprintf(attack_speed[1], "%s", "?");
	sprintf(cast_speed[1],   "%s", "?");
	sprintf(range[1],        "%s", "?");
	sprintf(cost[1],         "%s", "?");
	sprintf(skill_name[1],   "%s", "?");
	sprintf(health[2],       "%s", "?");
	sprintf(damage[2],       "%s", "?");
	sprintf(speed[2],        "%s", "?");
	sprintf(attack_speed[2], "%s", "?");
	sprintf(cast_speed[2],   "%s", "?");
	sprintf(range[2],        "%s", "?");
	sprintf(cost[2],         "%s", "?");
	sprintf(skill_name[2],   "%s", "?");

	portrait_name = "";
	panel_pos._x = 0;
	UpdateInformationWindow();

	tab = ARCHER_TAB;

	last_tab_pos = 0;
	bPausedStore = false;

	bDrawFake_MageTab = bDrawFake_ArcherTab = bDrawFake_WarriorTab = bDrawFake_BowlerTab = true;
	bValuesAreUnknow = false;
	bHasSkill = true;

	arrowStore = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_store_arrow"));

	if(!bUseHDImages)
	{
		selknamRealImage[0] = atlas_selknam_archer_01_1;
		selknamRealImage[1] = atlas_selknam_archer_02_1;
		selknamRealImage[2] = atlas_selknam_archer_03_1;

		selknamRealImage[3] = atlas_selknam_bowler_01_1;
		selknamRealImage[4] = atlas_selknam_bowler_03_1;
		selknamRealImage[5] = atlas_selknam_bowler_02_1;

		selknamRealImage[6] = atlas_selknam_melee_01_1;
		selknamRealImage[7] = atlas_selknam_melee_02_1;
		selknamRealImage[8] = atlas_selknam_melee_03_1;

		selknamRealImage[9] = atlas_selknam_mage_01_1;
		selknamRealImage[10] = atlas_selknam_mage_02_1;
		selknamRealImage[11] = atlas_selknam_mage_03_1;
	}
	else
	{
		selknamRealImage[0] = atlas_hd_selknam_archer_1;
		selknamRealImage[1] = atlas_hd_selknam_archer_2;
		selknamRealImage[2] = atlas_hd_selknam_archer_3;

		selknamRealImage[3] = atlas_hd_selknam_bowler_1_01;
		selknamRealImage[4] = atlas_hd_selknam_bowler_3_01;
		selknamRealImage[5] = atlas_hd_selknam_bowler_2_01;

		selknamRealImage[6] = atlas_hd_selknam_melee_1;
		selknamRealImage[7] = atlas_hd_selknam_melee_2;
		selknamRealImage[8] = atlas_hd_selknam_melee_3;

		selknamRealImage[9] = atlas_hd_selknam_mage_1;
		selknamRealImage[10] = atlas_hd_selknam_mage_2;
		selknamRealImage[11] = atlas_hd_selknam_mage_3;
	}

	currentSelectedImage = NULL;

	xText = pannelRightXPos + 32 + infoXOffset;
	yText = pannelRightYPos + 150 + infoYOffset;
	secondColDiff = 115;
	thirdColDiff = 161;
	padding = 22;

	skillExpPopupRect.w = 13;
	skillExpPopupRect.h = 3;
	skillExpPopupRect.x = 0; //xOffset 
	skillExpPopupRect.y = 0; //yOffset
	bShowSkillExpPopup = true;

	skillNamePosition.x = xText - 25;
	skillNamePosition.y = yText + 20 + 7 * padding + 10;
	skillNamePosition.w = 35;
	skillNamePosition.h = 35;

	icon_att_attributes = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_attributes"));
	icon_att_attacksp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_attacksp"));
	icon_att_castsp = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_castsp"));
	icon_att_cost = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_cost"));
	icon_att_damage = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_damage"));
	icon_att_health = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_health"));
	icon_att_range = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_range"));
	icon_att_speed = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_att_speed"));

	icon_skill_heal = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_heal"));
	icon_skill_haste = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_haste"));
	icon_skill_might = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_might"));
	icon_skill_poison = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_poison"));
	icon_skill_slow = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_slow"));
	icon_skill_stun = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_stun"));
	icon_skill_root = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_root"));
	icon_skill_confusion = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("icon_skill_confusion"));

	for(int i = 0; i < 3; i++)
	{
		portraits[i].x = portraitXPos + ((96 + 20) * i);
		portraits[i].y = portraitYPos + 15;
		portraits[i].w = 96;
		portraits[i].h = 96;
	}

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Left, &Store_Xbox360_Left);
	xbox360Controller.SetCallback(Xbox360_Right, &Store_Xbox360_Right);
	xbox360Controller.SetCallback(Xbox360_Up, &Store_Xbox360_Up);
	xbox360Controller.SetCallback(Xbox360_Down, &Store_Xbox360_Down);
	xbox360Controller.SetCallback(Xbox360_Action1, &Store_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Action2, &Store_Xbox360_Action);	
	xbox360Controller.SetCallback(Xbox360_Action2, &Store_Xbox360_Action);	
	xbox360Controller.SetCallback(Xbox360_Cancel1, &Store_OnClick_Back);	
	xbox360Controller.SetCallback(Xbox360_Cancel1, &Store_OnClick_Back);	
	
	focusManager.SetFocusDirection(Xbox360_Down, button_reset, button_tab[0]);

	for(int i = 0; i < 4; i++)
	{
		focusManager.BlockUpdateMotionFor(button_tab[i]);

		focusManager.SetFocusDirection(Xbox360_Up, button_tab[i], button_reset);

		if(i + 1 < 4)
			focusManager.SetFocusDirection(Xbox360_Right, button_tab[i], button_tab[i + 1]);

		if(i > 0)
			focusManager.SetFocusDirection(Xbox360_Left, button_tab[i], button_tab[i - 1]);

		focusManager.SetFocusDirection(Xbox360_Down, button_tab[i], &portraits[0]);
	}

	for(int i = 0; i < 3; i++)
	{
		focusManager.BlockUpdateMotionFor(&portraits[i]);
		focusManager.SetFocusDirection(Xbox360_Up, &portraits[i], button_tab[0]);

		if(i + 1 < 3)
			focusManager.SetFocusDirection(Xbox360_Right, &portraits[i], &portraits[i + 1]);

		if(i > 0)
			focusManager.SetFocusDirection(Xbox360_Left, &portraits[i], &portraits[i - 1]);

		focusManager.SetFocusDirection(Xbox360_Down, &portraits[i], button_panel[0][i]);
		focusManager.SetFocusDirection(Xbox360_Up, button_panel[0][i], &portraits[i]);
	}

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			focusManager.BlockUpdateMotionFor(button_panel[i][j]);

			if(i + 1 < 2)
				focusManager.SetFocusDirection(Xbox360_Down, button_panel[i][j], button_panel[i + 1][j]);

			if(j + 1 < 3)
				focusManager.SetFocusDirection(Xbox360_Right, button_panel[i][j], button_panel[i][j + 1]);

			if(i > 0)
				focusManager.SetFocusDirection(Xbox360_Up, button_panel[i][j], button_panel[i - 1][j]);

			if(j > 0)
				focusManager.SetFocusDirection(Xbox360_Left, button_panel[i][j], button_panel[i][j - 1]);
		}
	}
	
	focusManager.SetFocusDirection(Xbox360_Up, button_back, button_panel[1][2]);
	focusManager.SetFocusDirection(Xbox360_Down, button_panel[1][0], button_back);
	focusManager.SetFocusDirection(Xbox360_Down, button_panel[1][1], button_back);
	focusManager.SetFocusDirection(Xbox360_Down, button_panel[1][2], button_back);
	
	
#endif
}

Store::~Store()
{
	delete background;
	delete moreInfoBackground;
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 3; j++)
			delete button_panel[i][j];
	delete button_back;
	delete button_reset;
	for(int i = 0; i < 4; i++)
		delete button_tab[i];
	delete selknamInfoButton;
	for(int i = 0; i < 12; i++) selknamRealImage[i] = NULL;
	currentSelectedImage = NULL;
	delete btnClose;
	delete button;
}

void Store::OpenDetailedInfo()
{
	int i = (int)tab;
	int j = panel_pos._x;
	SetImageAndText(i, j);
}

void Store::SetImageAndText(int i, int j)
{
	bViewingDetailedInfo = true;

	realImageI = i;
	realImageJ = j;

	//Archer Base
	if(i == 0 && j == 0) SetParagraph(loc_almanac_baseArcherTitle, loc_almanac_baseArcherInformation);
	//Archer Poisoner
	else if(i == 0 && j == 2) SetParagraph(loc_almanac_poisonArcherTitle, loc_almanac_poisonArcherInformation);
	//Archer Retarder
	else if(i == 0 && j == 1) SetParagraph(loc_almanac_unitDelayerTitle, loc_almanac_unitDelayerInformation);
	
	//Archer Base
	else if(i == 1 && j == 0) SetParagraph(loc_almanac_bolasThrowerBaseTitle, loc_almanac_bolasThrowerBaseInformation);
	//Archer Base
	else if(i == 1 && j == 2) SetParagraph(loc_almanac_bolasThrowerFloorTitle, loc_almanac_bolasThrowerFloorInformation);
	//Archer Base
	else if(i == 1 && j == 1) SetParagraph(loc_almanac_bolasThrowerConfuseTitle, loc_almanac_bolasThrowerConfuseInformation);

	//Archer Base
	else if(i == 2 && j == 0) SetParagraph(loc_almanac_baseWarriorTitle, loc_almanac_baseWarriorInformation);
	//Archer Base
	else if(i == 2 && j == 2) SetParagraph(loc_almanac_resistantWarriorTitle, loc_almanac_resistantWarriorInformation);
	//Archer Base
	else if(i == 2 && j == 1) SetParagraph(loc_almanac_aggressiveWarriorTitle, loc_almanac_aggressiveWarriorInformation);

	//Archer Base
	else if(i == 3 && j == 0) SetParagraph(loc_almanac_baseMageTitle, loc_almanac_baseMageInformation);
	//Archer Base
	else if(i == 3 && j == 1) SetParagraph(loc_almanac_speederMageTitle, loc_almanac_speederMageInformation);
	//Archer Base
	else if(i == 3 && j == 2) SetParagraph(loc_almanac_mightierMageTitle, loc_almanac_mightierMageInformation);
}

void Store::SetParagraph(const char* title, const char* firstPar)
{
	sprintf(detailedInfoTitle, "%s", title);
	sprintf(detailedInfoText, "%s", firstPar);
}

void Store::Render()
{
	if(!bIsActiveObj)
		return;

	background->Render();

	if(currentGameHeight >= 640)
		Utils::RenderLabelTitle(10, titleXPos, titleYPos);
	Utils::RenderPannelA(8, 8, pannelLeftXPos, pannelLeftYPos);
	Utils::RenderVerticalTube(pannelLeftXPos + 135, pannelLeftYPos + 20, 360);
	Utils::RenderVerticalTube(pannelLeftXPos + 135 + 117, pannelLeftYPos + 20, 360);
	Utils::RenderPannelA(5, 8, pannelRightXPos, pannelRightYPos);

	int hardcodedFontPosYOffset = -23;
	if(currentGameHeight < 600)
		hardcodedFontPosYOffset = -17;
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(starCounterData.atlasName), CIwSVec2(remaining_stars_position_posX, remaining_stars_position_posY), starCounterData);
	
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

#if defined DESKTOPPLATFORM
		if (portraits[i].bIsFocused)
			focused_margin = 4;
#endif
#if !defined DESKTOPPLATFORM
		if (bIsPortraitFocused[i])
			focused_margin = 4;
#endif
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[(int)tab + focused_margin][i + off].atlasName), CIwSVec2((portraitXPos + ((96 + 20) * i)), portraitYPos + 15), portraitArray[(int)tab + focused_margin][i + off]);
	}

	
	//if(!bIsFullVersion)
	//{
	//	for(int i = 0; i < 3; i++)
	//	{
	//		if(gray_color[i])
	//		{
	//			Utils::RenderColor(0xaf000000, CIwSVec2(portraitXPos + ((96 + 20) * i), portraitYPos + 15), CIwSVec2(96, 96));
	//			IwGxFlush();
	//			IwGxLightingOn();
	//			Utils::RenderText(loc_ui_onlyFullVersion, CIwRect(portraitXPos + ((96 + 20) * i), portraitYPos + 15, 96, 96), font_komikadisplay11, 0xff00c0ff);
	//			IwGxLightingOff();
	//		}
	//	}
	//}


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

		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreLv1RenderData[disable].atlasName), btnStoreLv1RenderData[disable], CIwSVec2(portraitXPos + (96 + 20) * i, portraitYPos + 120));

		IwGxFlush();
	}

	if(tab == MAGE_TAB && !button_tab[3]->GetIsFocused())
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreMageSelected.atlasName), CIwSVec2(button_tab[3]->GetX(), button_tab[3]->GetY()), btnStoreMageSelected);
	}
	else
	{
		button_tab[3]->Render();
	}

	if(tab == WARRIOR_TAB && !button_tab[2]->GetIsFocused())
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreWarriorSelected.atlasName), CIwSVec2(button_tab[2]->GetX(), button_tab[2]->GetY()), btnStoreWarriorSelected);
	}
	else
	{
		button_tab[2]->Render();
	}
	 
	if(tab == BOWLER_TAB && !button_tab[1]->GetIsFocused())
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreBowlerSelected.atlasName), CIwSVec2(button_tab[1]->GetX(), button_tab[1]->GetY()), btnStoreBowlerSelected);
	}
	else
	{
		button_tab[1]->Render();
	}

	if(tab == ARCHER_TAB && !button_tab[0]->GetIsFocused())
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(btnStoreArcherSelected.atlasName), CIwSVec2(button_tab[0]->GetX(), button_tab[0]->GetY()), btnStoreArcherSelected);
	}
	else
	{
		button_tab[0]->Render();
	}

	for(int i = 0; i < 3; i++)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(arrowStore.atlasName), CIwSVec2(portraitXPos + (96 + 20) * i + 32, portraitYPos + 96 + 32 + 28), arrowStore);
	}

	for(int i = 0; i < 3; i++)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(arrowStore.atlasName), CIwSVec2(portraitXPos + (96 + 20) * i + 32, portraitYPos + 96 + 32 + 30 + 96), arrowStore);
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

		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[(int)tab][current_p_x].atlasName), CIwSVec2(pannelRightXPos + 70 + infoXOffset, pannelRightYPos + 60 + infoYOffset), portraitArray[(int)tab][current_p_x]);
		
		if(!gray_color[panel_pos._x])
		{
			selknamInfoButton->Render();
		}
	}


	//Titles
	IwGxLightingOn();
	if(currentGameHeight >= 640)
		Utils::RenderText(loc_upgrade_selknamUpgrade, CIwRect(titleXPos + 5, titleYPos + 95, 438, 66), font_komikadisplay28, 0xff00d9fc);

	Utils::RenderText(remaining_stars, remaining_stars_position_txt, font_komikadisplaybold18, remaining_stars_color);

	int adjust = 0;

	if(bShowInformation)
	{
		if(tab == MAGE_TAB)
			adjust = 10;


		int _icon_xOffset = -19;
		int _icon_yOffset = 18;


		Utils::RenderText(portrait_name, CIwRect(pannelRightXPos + infoXOffset, pannelRightYPos + 13 + infoYOffset, 240, 50), font_komikadisplaybold18, 0xff00edf8);

		//Attributes list
		Utils::RenderText(loc_upgrade_attributes, CIwRect(xText,  yText + adjust - 2, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_attributes.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + adjust - 2 + _icon_yOffset), icon_att_attributes);
		Utils::RenderText(current_level, CIwRect(xText + secondColDiff,  yText + adjust - 2, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(bShowThirdCol &&  !gray_color[panel_pos._x])
			Utils::RenderText(next_level, CIwRect(xText + thirdColDiff, yText + adjust - 2, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

		//Col 1
		Utils::RenderText(health[0],       CIwRect(xText, yText + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_health.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + 20 + 0 * padding + adjust + _icon_yOffset), icon_att_health);
		if(tab != MAGE_TAB)
		{
			Utils::RenderText(damage[0],       CIwRect(xText, yText + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_damage.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + 20 + 1 * padding + _icon_yOffset), icon_att_damage);
		}
		Utils::RenderText(speed[0],        CIwRect(xText, yText + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_speed.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + 20 + 2 * padding - adjust + _icon_yOffset), icon_att_speed);
		if(tab != MAGE_TAB)
		{
			Utils::RenderText(attack_speed[0], CIwRect(xText, yText + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_attacksp.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + 20 + 3 * padding + _icon_yOffset), icon_att_attacksp);
		}
		else
		{
			Utils::RenderText(cast_speed[0],   CIwRect(xText, yText + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_castsp.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + 20 + 3 * padding - adjust + _icon_yOffset), icon_att_castsp);
		}
		Utils::RenderText(range[0],        CIwRect(xText, yText + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_range.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + 20 + 4 * padding - adjust + _icon_yOffset), icon_att_range);

		Utils::RenderText(cost[0],         CIwRect(xText, yText + 20 + 5 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(icon_att_cost.atlasName), CIwSVec2(xText + _icon_xOffset,  yText + 20 + 5 * padding - adjust + _icon_yOffset), icon_att_cost);
		
		if(bHasSkill)
		{
			Utils::RenderText(skill_name[0],   CIwRect(xText, yText + 20 + 7 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			RenderData renderdataTmp;
			renderdataTmp = icon_skill_heal;


			if(tab == ARCHER_TAB )
			{
				if(panel_pos._x == 1)
					renderdataTmp = icon_skill_slow;
				else if(panel_pos._x == 2)
					renderdataTmp = icon_skill_poison;
			}
			else if(tab == BOWLER_TAB )
			{
				if(panel_pos._x == 0)
					renderdataTmp = icon_skill_stun;
				else if(panel_pos._x == 1)
					renderdataTmp = icon_skill_confusion;
				else if(panel_pos._x == 2)
					renderdataTmp = icon_skill_root;
			}
			else if(tab == MAGE_TAB )
			{
				if(panel_pos._x == 0)
					renderdataTmp = icon_skill_heal;
				else if(panel_pos._x == 1)
					renderdataTmp = icon_skill_haste;
				else if(panel_pos._x == 2)
					renderdataTmp = icon_skill_might;
			}

			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(renderdataTmp.atlasName), CIwSVec2(xText + _icon_xOffset, yText + 20 + 7 * padding - adjust + _icon_yOffset), renderdataTmp);
		}

		//Col 2

		Utils::RenderText(health[1],       CIwRect(xText + secondColDiff, yText + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(tab != MAGE_TAB)
			Utils::RenderText(damage[1],       CIwRect(xText + secondColDiff, yText + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(speed[1],        CIwRect(xText + secondColDiff, yText + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		if(tab != MAGE_TAB)
			Utils::RenderText(attack_speed[1], CIwRect(xText + secondColDiff, yText + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		else
			Utils::RenderText(cast_speed[1],   CIwRect(xText + secondColDiff, yText + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(range[1],        CIwRect(xText + secondColDiff, yText + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		Utils::RenderText(cost[1],         CIwRect(xText + secondColDiff, yText + 20 + 5 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		
		if(bHasSkill)
		{
			Utils::RenderText(skill_name[1],   CIwRect(xText + secondColDiff, yText + 20 + 7 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
		}
		
		if(bShowThirdCol && !gray_color[panel_pos._x])
		{
			if(bValuesAreUnknow)
				Utils::RenderText(health[2],       CIwRect(xText + thirdColDiff, yText + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			else
				Utils::RenderText(health[2],       CIwRect(xText + thirdColDiff, yText + 20 + 0 * padding + adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);

			if(bValuesAreUnknow)
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(damage[2],       CIwRect(xText + thirdColDiff, yText + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			}
			else
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(damage[2],       CIwRect(xText + thirdColDiff, yText + 20 + 1 * padding, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);
			}

			if(bValuesAreUnknow)			
				Utils::RenderText(speed[2],        CIwRect(xText + thirdColDiff, yText + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			else
				Utils::RenderText(speed[2],        CIwRect(xText + thirdColDiff, yText + 20 + 2 * padding - adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);

			if(bValuesAreUnknow)
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(attack_speed[2], CIwRect(xText + thirdColDiff, yText + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
				else
					Utils::RenderText(cast_speed[2],   CIwRect(xText + thirdColDiff, yText + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			}
			else
			{
				if(tab != MAGE_TAB)
					Utils::RenderText(attack_speed[2], CIwRect(xText + thirdColDiff, yText + 20 + 3 * padding, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);
				else
					Utils::RenderText(cast_speed[2],   CIwRect(xText + thirdColDiff, yText + 20 + 3 * padding - adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);
			}

			if(bValuesAreUnknow)
				Utils::RenderText(range[2],        CIwRect(xText + thirdColDiff, yText + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			else
				Utils::RenderText(range[2],        CIwRect(xText + thirdColDiff, yText + 20 + 4 * padding - adjust, 180, 50), font_komikadisplay11, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT);

			Utils::RenderText(cost[2],         CIwRect(xText + thirdColDiff, yText + 20 + 5 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

			if(bHasSkill)
			{
				int xSkillNameText = xText + thirdColDiff;
				int ySkillNameText = yText + 20 + 7 * padding - adjust;

				Utils::RenderText(skill_name[2],   CIwRect(xText + thirdColDiff, yText + 20 + 7 * padding - adjust, 180, 50), font_komikadisplay11, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
			}
		}
	}
	IwGxLightingOff();

	button_reset->Render();
	button_back->Render();

	//More detailed info popup.
	if(bViewingDetailedInfo)
	{
		//IwTrace(ALMANACINGAME, ("Rendering popup."));
		IwGxFlush();
		moreInfoBackground->Render();
		btnClose->Render();
		Utils::RenderPannelB(pannelBLengthX, pannelBLengthY, xPosDetailed, yPosDetailed);

		IwGxLightingOn();
		Utils::RenderText(detailedInfoTitle, 
			CIwRect(xPosDetailed + 72, yPosDetailed + 50, wDetailed - 81, 20),
			font_SmartKid20, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true, 3);
		IwGxFlush();
		Utils::RenderText(detailedInfoText, 
			CIwRect(xPosDetailed + 20, yPosDetailed + 120, wDetailed - 30, 98),
			font_SmartKid20, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
		IwGxLightingOff();

		IwGxFlush();
		if(!bUseHDImages)
			Utils::RenderSingleTexture(selknamRealImage[realImageI * 3 + realImageJ], CIwSVec2(xPosDetailed + 30, yPosDetailed - 7), CIwSVec2((int)(64 * 1.4f), (int)(96 * 1.4f)), 0.0f, 0.0f, 64 / 512.0f, 96 / 512.0f);
		else
		{
			//Archers, Bowler and Melee
			float tmpAtlasWidth = 960.0f;
			float tmpAtlasHeight = 1008.0f;
			if(realImageI == 0 || realImageI == 1 || realImageI == 2)
			{
				tmpAtlasWidth = atlas_hdsize_selknam_archer.width;
				tmpAtlasHeight = atlas_hdsize_selknam_archer.height;
			}
			//Mage
			else if(realImageI == 3)
			{
				tmpAtlasWidth = atlas_hdsize_selknam_mage.width;
				tmpAtlasHeight = atlas_hdsize_selknam_mage.height;
			}
			Utils::RenderSingleTexture(selknamRealImage[realImageI * 3 + realImageJ], CIwSVec2(xPosDetailed + 30, yPosDetailed - 15), CIwSVec2(96, 144), 0.0f, 0.0f, 96.0f / tmpAtlasWidth, 144.0f / tmpAtlasHeight);
		}	
	}

	if(bShowSkillExpPopup)
	{
		Utils::RenderPannel(PannelType_F, skillExpPopupRect.w, skillExpPopupRect.h, skillExpPopupRect.x + skillNamePosition.x + 15, skillExpPopupRect.y + skillNamePosition.y - adjust - 90);
		RenderSkillExplanation(skillExpPopupRect.w, skillExpPopupRect.h, skillExpPopupRect.x + skillNamePosition.x + 15, skillExpPopupRect.y + skillNamePosition.y - adjust - 90);
	}
}

void Store::RenderSkillExplanation(int w, int h, int x, int y)
{
	IsoSelknamType type = GetCurrentSelknamSelected();

	int thisX = x + 10;
	int thisY = y + 10;
	int thisW = w * 20;
	int thisH = h * 20;
	
	CIwGxFont* fontToUse = font_SmartKid12;
	const char* stringToUse = "DEFAULT";

	if(type == SELKNAM_Shaman_Haste) stringToUse = loc_description_skillHaste;
	else if(type == SELKNAM_Shaman_Healer) stringToUse = loc_description_skillHeal;
	else if(type == SELKNAM_Shaman_Might) stringToUse = loc_description_skillMight;
	else if(type == SELKNAM_Bowler_Confuser) stringToUse = loc_description_skillConfusion;
	else if(type == SELKNAM_Bowler_Rooter) stringToUse = loc_description_skillRoot;
	else if(type == SELKNAM_Bowler_Stuner) stringToUse = loc_description_skillStun;
	else if(type == SELKNAM_Archer_Poisoner) stringToUse = loc_description_skillPoison;
	else if(type == SELKNAM_Archer_Retarder) stringToUse = loc_description_skillSlow;

	IwGxLightingOn();
	Utils::RenderText(stringToUse, CIwRect(thisX + 5, thisY, thisW, thisH), fontToUse, 0xffffffff, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP);
	IwGxLightingOff();
}

void Store::InitObject()
{
	tutorial->SetStoreRef(this);

	panel_pos._x = -1;
	panel_pos._y = -1;
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
	/*
	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			button_panel[i][j]->IsFocused(false);
		}
	}
	*/

	button_back->SetStatus(2);

	//SetFocus(Store_Focus_Back);
	bFxSoundMuted = false;
	bShowSkillExpPopup = false;

	bShowInformation = false;

	if(tutorial->currBreakpoint == Tutorial_BreakPoint_MapSelection)
	{
		tutorial->ExecuteBreakpoint();
	}

#if defined DESKTOPPLATFORM
	focusManager.SetInitialFocus(button_back);
#endif
}

void Store::UpdateRemainingStars()
{
	int current_stars = 0;
	for(int i = 0; i < MAX_MAPS_CAMPAIGN; i++)
		current_stars += Stars_By_Level[i];
	num_remaining_stars = current_stars - Used_Stars;
	sprintf(remaining_stars, "%d / %d", current_stars - Used_Stars, current_stars);
}

void Store::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_store);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_store);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_store);

	if(saveDataHelper)
		saveDataHelper->SaveGameDataCampaign();

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(NULL);
#endif
}

void Store::ClearFocus()
{
	bIsPortraitFocused[0] = bIsPortraitFocused[1] = bIsPortraitFocused[2] = false;
	for(int i = 0; i < 2; i++) for(int j = 0; j < 3; j++) button_panel[i][j]->SetStatus(2);
	bShowInformation = false;
}

void Store::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bPausedStore)
	{
		return;
	}

	if(tutorial->currBreakpoint == Tutorial_BreakPoint_UpgradeArcher)
	{
		if(button_panel[0][0]->MouseOn(event->m_x, event->m_y))
		{
			if(event->m_Pressed)
			{
				if((0 != panel_pos._x || 0 != panel_pos._y))
				{
					bShowInformation = true;
					bIsPortraitFocused[0] = bIsPortraitFocused[1] = bIsPortraitFocused[2] = false;
					if(panel_pos._x >= 0 && panel_pos._y >= 0)
					{
						button_panel[panel_pos._y][panel_pos._x]->SetStatus(2);
					}
					button_panel[0][0]->SetStatus(1);
				}
				else
				{
					panel_pos._x = 0;
					panel_pos._y = 0;
					button_panel[0][0]->Execute();
					button_panel[0][0]->SetStatus(1);
				}
				Utils::PlaySoundFX(FX_Click_01);

				//tutorial->ExecuteBreakpoint();
			}

			panel_pos._x = 0;
			panel_pos._y = 0;
		}

		return;
	}

	if(bViewingDetailedInfo)
	{
		btnClose->HandleClick(event);

		if(event->m_Pressed 
			&& !MouseOn(event->m_x, event->m_y, (int)((currentGameWidth - (32 + 24 * 16)) * 0.5f), (int)((currentGameHeight - (32 + 8 * 16)) * 0.5f), 32 + 24 * 16, 32 + 8 * 16)
			&& !btnClose->MouseOn(event->m_x, event->m_y))
		{
			bViewingDetailedInfo = false;
		}

		return;
	}

	//if(event->m_Pressed) bKeyboardBlocked = true;
	//else bKeyboardBlocked = false;

	button_reset->HandleClick(event);
	button_back->HandleClick(event);

	
	if(bShowInformation && panel_pos._x >= 0 && !gray_color[panel_pos._x])
	{
		selknamInfoButton->HandleClick(event);
	}
	
	
	for(int i = 0; i < 4; i++)
	{
		button_tab[i]->HandleClick(event);
	}
	
	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 3; j++)
		{
			//if(bIsFullVersion || (!bIsFullVersion && j == 0))
			//{
				if(button_panel[i][j]->MouseOn(event->m_x, event->m_y))
				{
					if(event->m_Pressed)
					{
						if((j != panel_pos._x || i != panel_pos._y))
						{
							bShowInformation = true;
							bIsPortraitFocused[0] = bIsPortraitFocused[1] = bIsPortraitFocused[2] = false;
							if(panel_pos._x >= 0 && panel_pos._y >= 0)
							{
								button_panel[panel_pos._y][panel_pos._x]->SetStatus(2);
							}
							button_panel[i][j]->SetStatus(1);
						}
						else
						{
							panel_pos._x = j;
							panel_pos._y = i;
							button_panel[i][j]->Execute();
							button_panel[i][j]->SetStatus(1);
						}
						Utils::PlaySoundFX(FX_Click_01);
					}

					panel_pos._x = j;
					panel_pos._y = i;
				}
			//}
		}

	if(!event->m_Pressed)
	{
		bDoOnce = true;
		return;
	}

	if(event->m_Button != S3E_POINTER_BUTTON_LEFTMOUSE) return;

	if(MouseOn((int)event->m_x, (int)event->m_y, portraitXPos, portraitYPos + 15, 96, 96))
	{
		if(!bIsPortraitFocused[0])
		{
			bShowInformation = true;
			if(panel_pos._x >= 0 && panel_pos._y >= 0)
			{
				button_panel[panel_pos._y][panel_pos._x]->SetStatus(2);
			}
			bIsPortraitFocused[1] = bIsPortraitFocused[2] = false;
			bIsPortraitFocused[0] = true;
			panel_pos._x = 0;
			panel_pos._y = -1;
		}

		//Utils::PlayFXSound(FX_Sound_CLICK);
		Utils::PlaySoundFX(FX_Click_01);
	}

	if(MouseOn((int)event->m_x, (int)event->m_y, (portraitXPos + ((96 + 20) * 1)), portraitYPos + 15, 96, 96))
	{
		//if(bIsFullVersion)
		//{
			if(!bIsPortraitFocused[1])
			{
				bShowInformation = true;
				if(panel_pos._x >= 0 && panel_pos._y >= 0)
				{
					button_panel[panel_pos._y][panel_pos._x]->SetStatus(2);
				}
				bIsPortraitFocused[0] = bIsPortraitFocused[2] = false;
				bIsPortraitFocused[1] = true;
				panel_pos._x = 1;
				panel_pos._y = -1;
			}
			else
			{
				if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherRetarderUpgrades, archer_retarder_cost[0]);
				else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorBerserkUpgrades, warrior_berserker_cost[0]);
				else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerConfuserUpgrades, bowler_confuser_cost[0]);
				else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanHasteUpgrades, mage_haste_cost[0]);
			}
		
			//Utils::PlayFXSound(FX_Sound_CLICK);
			Utils::PlaySoundFX(FX_Click_01);
		//}
	}

	if(MouseOn((int)event->m_x, (int)event->m_y, (portraitXPos + ((96 + 20) * 2)), portraitYPos + 15, 96, 96))
	{
		//if(bIsFullVersion)
		//{
			bShowInformation = true;
			if(!bIsPortraitFocused[2])
			{
				if(panel_pos._x >= 0 && panel_pos._y >= 0)
				{
					button_panel[panel_pos._y][panel_pos._x]->SetStatus(2);
				}
				bIsPortraitFocused[0] = bIsPortraitFocused[1] = false;
				bIsPortraitFocused[2] = true;
				panel_pos._x = 2;
				panel_pos._y = -1;
			}
			else
			{
				if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherPoisonerUpgrades, archer_poisoner_cost[0]);
				else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorTankUpgrades, warrior_tank_cost[0]);
				else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerRooterUpgrades, bowler_rooter_cost[0]);
				else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanMightUpgrades, mage_might_cost[0]);
			}
			
			//Utils::PlayFXSound(FX_Sound_CLICK);
			Utils::PlaySoundFX(FX_Click_01);
		//}
	}


#if defined DESKTOPPLATFORM
	if(event->m_Pressed)
		focusManager.FocusBy2DPosition(event->m_x, event->m_y);
#endif
}

void Store::UnlockCharacterSecondCol()
{
	if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherRetarderUpgrades, archer_retarder_cost[0]);
	else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorBerserkUpgrades, warrior_berserker_cost[0]);
	else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerConfuserUpgrades, bowler_confuser_cost[0]);
	else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanHasteUpgrades, mage_haste_cost[0]);
}

void Store::UnlockCharacterThirdCol()
{
	if     (tab == ARCHER_TAB) UnlockCharacter(&ArcherPoisonerUpgrades, archer_poisoner_cost[0]);
	else if(tab == WARRIOR_TAB) UnlockCharacter(&WarriorTankUpgrades, warrior_tank_cost[0]);
	else if(tab == BOWLER_TAB) UnlockCharacter(&BowlerRooterUpgrades, bowler_rooter_cost[0]);
	else if(tab == MAGE_TAB)   UnlockCharacter(&ShamanMightUpgrades, mage_might_cost[0]);
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

#if defined DESKTOPPLATFORM
	bShowSkillExpPopup = (Utils::MouseOn(event->m_x, event->m_y, skillNamePosition.x, skillNamePosition.y, skillNamePosition.w, skillNamePosition.h)
		&& bShowInformation && bHasSkill);
#endif
	if(bViewingDetailedInfo)
	{
		btnClose->HandleMotion(event);
	}

	button_reset->HandleMotion(event);
	button_back->HandleMotion(event);

	
	if(bShowInformation && panel_pos._x >= 0 && !gray_color[panel_pos._x])
	{
		selknamInfoButton->HandleMotion(event);
	}
	

	for(int i = 0; i < 4; i++)
	{
		button_tab[i]->HandleMotion(event);
	}

	for(int i = 0; i < 2; i++)
		for(int j = 0; j < 3; j++)
		{
			//if(bIsFullVersion || (!bIsFullVersion && j == 0))
			//{
				button_panel[i][j]->HandleMotion(event);
			//}
		}

#if defined DESKTOPPLATFORM
		focusManager.UpdateMotionEvents(event);
#endif
}

void Store::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	/*
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
	*/
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
	//if(!bIsFullVersion && id != 0)
	//{
	//	return;
	//}

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

}

void Store::ResetFocus()
{
	
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
	//if(!bIsFullVersion)
	//{
	//	return;
	//}

	if(unitupgrade->level == 0 && num_remaining_stars >= cost)
	{
		unitupgrade->level++;
		Used_Stars += cost;
	}
	else if(unitupgrade->level == 1)
	{
		unitupgrade->level--;
		Used_Stars -= cost;
		//IwTrace(TRACE_STORE, ("Reduciendo nivel"));
	}

	UpdateRemainingStars();
}

void Store::UpdateInformationWindow()
{
	if(tab == ARCHER_TAB)
	{
		if(panel_pos._x == 0)
		{
			portrait_name = loc_portrait_archerBase;
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
			portrait_name = loc_portrait_archerSlower;
			sprintf(current_level, "lvl %d", (ArcherRetarderUpgrades.level == 0) ? 1 : ArcherRetarderUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ArcherRetarderUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillSlow);
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
			portrait_name = loc_portrait_archerPoisoner;
			sprintf(current_level, "lvl %d", (ArcherPoisonerUpgrades.level == 0) ? 1 : ArcherPoisonerUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ArcherPoisonerUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillPoison);
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
			portrait_name = loc_portrait_bowlerStunner;
			sprintf(current_level, "lvl %d", BowlerStunerUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(BowlerStunerUpgrades.level >= (panel_pos._y + 2)) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillStun);
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
			portrait_name = loc_portrait_bowlerConfuser;
			sprintf(current_level, "lvl %d", (BowlerConfuserUpgrades.level == 0) ? 1 : BowlerConfuserUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(BowlerConfuserUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillConfusion);
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
			portrait_name = loc_portrait_bowlerRooter;
			sprintf(current_level, "lvl %d", (BowlerRooterUpgrades.level == 0) ? 1 : BowlerRooterUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(BowlerRooterUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillRoot);
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
			portrait_name = loc_portrait_warriorBase;
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
			portrait_name = loc_portrait_warriorBerserk;
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
			portrait_name = loc_portrait_warriorTank;
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
			portrait_name = loc_portrait_shamanHealer;
			sprintf(current_level, "lvl %d", ShamanHealerUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ShamanHealerUpgrades.level >= (panel_pos._y + 2)) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillHeal);
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
			portrait_name = loc_portrait_shamanSpeeder;
			sprintf(current_level, "lvl %d", (ShamanHasteUpgrades.level == 0) ? 1 : ShamanHasteUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ShamanHasteUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillHaste);
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
			portrait_name = loc_portrait_shamanMighter;
			sprintf(current_level, "lvl %d", (ShamanMightUpgrades.level == 0) ? 1 : ShamanMightUpgrades.level);
			sprintf(next_level,    "lvl %d",  (panel_pos._y + 2));
			if(ShamanMightUpgrades.level >= (panel_pos._y + 2) || panel_pos._y == -1) bShowThirdCol = false;
			else bShowThirdCol = true;
			bHasSkill = true;
			sprintf(skill_name[0], "%s", loc_upgrade_skillMight);
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

IsoSelknamType Store::GetCurrentSelknamSelected()
{
	if(tab == WARRIOR_TAB)
	{
		if(panel_pos._x == 0) return SELKNAM_Warrior_Base;
		if(panel_pos._x == 1) return SELKNAM_Warrior_Berserk;
		if(panel_pos._x == 2) return SELKNAM_Warrior_Tank;
	}

	if(tab == MAGE_TAB)
	{
		if(panel_pos._x == 0) return SELKNAM_Shaman_Healer;
		if(panel_pos._x == 1) return SELKNAM_Shaman_Haste;
		if(panel_pos._x == 2) return SELKNAM_Shaman_Might;
	}

	if(tab == BOWLER_TAB)
	{
		if(panel_pos._x == 0) return SELKNAM_Bowler_Stuner;
		if(panel_pos._x == 1) return SELKNAM_Bowler_Confuser;
		if(panel_pos._x == 2) return SELKNAM_Bowler_Rooter;
	}

	if(tab == ARCHER_TAB)
	{
		if(panel_pos._x == 0) return SELKNAM_Archer_Base;
		if(panel_pos._x == 1) return SELKNAM_Archer_Retarder;
		if(panel_pos._x == 2) return SELKNAM_Archer_Poisoner;
	}

	return SELKNAM_None;
}

void Store::Update()
{
	if(!bIsActiveObj)
		return;

#if defined DESKTOPPLATFORM
	if((!tutorial->IsActive() || (tutorial->IsActive() && !tutorial->ClickeableFrame())) && popUpState == GSP_None)
		xbox360Controller.Update();
#endif

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

#if defined DESKTOPPLATFORM
	for(int i = 0; i < 3; i++)
	{
		focusManager.SetFocusDirection(Xbox360_Up, &portraits[i], button_tab[(int)t]);
	}
#endif
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

	sprintf(skill_name[2], "lvl %d", level);

	bValuesAreUnknow = false;
}
