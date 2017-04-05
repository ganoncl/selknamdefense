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

#include "common.h"
#include "localization.h"
#include "mm_almanac.h"
#include "utils.h"
#include "texturehelper.h"

Almanac* almanacRef = NULL;

//Mouse Motion
void HandleMotion_Almanac(s3ePointerMotionEvent* event) 
{
	if(almanacRef == NULL)
		return;

	if(popUpState != GSP_None)
		return;

	almanacRef->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClick_Almanac(s3ePointerEvent *event) 
{
	if(almanacRef == NULL)
		return;

	if(popUpState != GSP_None)
		return;

	almanacRef->UpdateClickEvents(event);
}

void btnBackAction__Almanac()
{
	if(!almanacRef->bViewingDetailedInfo)
		curAlmanacState = ALMANAC_MENU;
}

void onCloseAction()
{
	almanacRef->ClosePopup();
}

void SelknamAlmanac_CancelAction()
{
	if(almanacRef->bViewingDetailedInfo)
	{
		onCloseAction();
	}
	else
	{
		btnBackAction__Almanac();
	}
}

void SelknamAlmanac_OpenPopup(int i, int j)
{
	almanacRef->OpenPopup(i, j);
}

#if defined DESKTOPPLATFORM
void SelknamAlmanac_Xbox360_A()
{
	almanacRef->focusManager.ExecuteFocusedObject();
}

void SelknamAlmanac_Xbox360_Up()
{
	if(almanacRef->bViewingDetailedInfo)
		almanacRef->focusManager.ChangeFocus(almanacRef->btnClose);
	else
		almanacRef->focusManager.ChangeFocus(XBOX360_Button_Up);
}

void SelknamAlmanac_Xbox360_Down()
{	
	if(almanacRef->bViewingDetailedInfo)
		almanacRef->focusManager.ChangeFocus(almanacRef->btnClose);
	else
		almanacRef->focusManager.ChangeFocus(XBOX360_Button_Down);
}

void SelknamAlmanac_Xbox360_Left()
{
	if(almanacRef->bViewingDetailedInfo)
		almanacRef->focusManager.ChangeFocus(almanacRef->btnClose);
	else
		almanacRef->focusManager.ChangeFocus(XBOX360_Button_Left);
}

void SelknamAlmanac_Xbox360_Right()
{
	if(almanacRef->bViewingDetailedInfo)
		almanacRef->focusManager.ChangeFocus(almanacRef->btnClose);
	else
		almanacRef->focusManager.ChangeFocus(XBOX360_Button_Right);
}
#endif

Almanac::Almanac()
{
	almanacRef = this;

	//renderPanelNumWidth = 9;
	//renderPanelNumHeight = 10;
	//widthSelknamAlmanacPannel =  (renderPanelNumWidth + 2) * 40;
	//pannelXPos = (int)(currentGameWidth * 0.5f) - (int)(widthSelknamAlmanacPannel * 0.5f);
	//pannelYPos = (int)(currentGameHeight * 0.5f) - (int)(((renderPanelNumHeight + 2) * 40) * 0.5f);
	//buttonsXOffset = 55;
	//buttonsYOffset = 20;

	renderPanelNumWidth = 11;
	renderPanelNumHeight = 7;
	widthSelknamAlmanacPannel =  (renderPanelNumWidth + 2) * 40;
	pannelXPos = (int)(currentGameWidth * 0.5f) - (int)(widthSelknamAlmanacPannel * 0.5f);
	pannelYPos = (int)(currentGameHeight * 0.5f) - (int)(((renderPanelNumHeight + 2) * 40 - 20) * 0.5f);
	buttonsXOffset = 36;
	buttonsYOffset = 13;

	if(currentGameHeight <= 600)
		pannelYPos = pannelYPos - 25;

	xPortrait = pannelXPos + buttonsXOffset;
	yPortrait = pannelYPos + buttonsYOffset;

	TextureData buttonData = TextureHelper::GetTextureNamed("btn_popup_long");
	button = new ButtonRenderData();
	button->numStates = 1;
	button->state[0].RollOut  = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	button->state[0].RollOn   = TextureHelper::GetRenderData(buttonData, 1, 3, 1);
	button->state[0].Pressed  = TextureHelper::GetRenderData(buttonData, 1, 3, 2);
	button->state[0].Inactive = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	int buttonWidth = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).x;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	CIwSVec2 buttonDimension = TextureHelper::GetDimension(buttonRenderData.state[0].Pressed);
	int btnTexWidth = buttonDimension.x;
	int btnTexHeight = buttonDimension.y;

	moreInfoBackground = new Background(0, 0, 0, 170);

	int screenH = currentGameHeight;
	
	//Back button
	btnBackPosX = (int)((currentGameWidth - btnTexWidth) * 0.5f);
	btnBackPosY = currentGameHeight - Utils::GetScaledInt(110);
	
	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnBackPosX, btnBackPosY);
	btnBack->OnClick(&btnBackAction__Almanac);

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


	pannelBLengthX = 36;
	pannelBLengthY = 23;

	pannelBPosX = (int)((currentGameWidth - (32 + pannelBLengthX * 16)) * 0.5f);
	pannelBPosY = (int)((currentGameHeight - (32 + pannelBLengthY * 16)) * 0.5f);
	if(currentGameHeight <= 600)
		pannelBPosY = pannelBPosY - 35;
	w = 32 + pannelBLengthX * 16;
	h = 32 + pannelBLengthY * 16;

	btnClose = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, pannelBPosX + int((w - buttonWidth)* 0.5f), pannelBPosY + h - 10);
	btnClose->OnClick(&onCloseAction);
	btnClose->SetText(loc_ui_close);
	btnClose->SetStyle(font_komikadisplaybold18, 0, -15);

	btnBack->SetText(loc_ui_back);

	bViewingDetailedInfo = false;
	realImageI = realImageJ = -1;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			disabledPortrait[i][j] = 0;
			portraitObject[i][j].i = i;
			portraitObject[i][j].j = j;
			portraitObject[i][j].onSelect = &SelknamAlmanac_OpenPopup;
			portraitObject[i][j].x = (int)(xPortrait + (float)((96 + 20) * i));
			portraitObject[i][j].y = (int)(yPortrait + 15 + 105 * j);
			portraitObject[i][j].w = 96;
			portraitObject[i][j].h = 96;
		}
	}

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(XBOX360_Button_A, &SelknamAlmanac_Xbox360_A);
	xbox360Controller.SetCallback(XBOX360_Button_X, &SelknamAlmanac_Xbox360_A);
	xbox360Controller.SetCallback(XBOX360_Button_B, &SelknamAlmanac_CancelAction);
	xbox360Controller.SetCallback(XBOX360_Button_Y, &SelknamAlmanac_CancelAction);
	xbox360Controller.SetCallback(XBOX360_Button_Left, &SelknamAlmanac_Xbox360_Left);
	xbox360Controller.SetCallback(XBOX360_Button_Right, &SelknamAlmanac_Xbox360_Right);
	xbox360Controller.SetCallback(XBOX360_Button_Up, &SelknamAlmanac_Xbox360_Up);
	xbox360Controller.SetCallback(XBOX360_Button_Down, &SelknamAlmanac_Xbox360_Down);

	focusManager.SetDefaultFocus(btnBack);
	focusManager.BlockUpdateMotionFor(btnClose);
#endif
}

Almanac::~Almanac()
{
	delete btnBack;
	delete moreInfoBackground;
	delete btnClose;
	delete button;
	almanacRef = NULL;
}

void Almanac::ClosePopup()
{
	bViewingDetailedInfo = false;

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(&portraitObject[realImageI][realImageJ]);
#endif

	realImageI = realImageJ = -1;
}

void Almanac::SetImageAndText(int i, int j)
{
	//IwTrace(ALMANACINGAME, ("SetImageAndText."));

	realImageI = i;
	realImageJ = j;

	bViewingDetailedInfo = true;

	/*
	//Archer Base
	else if(i == 0 && j == 0) SetParagraph("", "");
	*/

	//Archer Base
	if(realImageI == 0 && realImageJ == 0) SetParagraph(loc_almanac_baseArcherTitle, loc_almanac_baseArcherInformation);
	//Archer Poisoner
	else if(realImageI == 0 && realImageJ == 2) SetParagraph(loc_almanac_poisonArcherTitle, loc_almanac_poisonArcherInformation);
	//Archer Retarder
	else if(realImageI == 0 && realImageJ == 1) SetParagraph(loc_almanac_unitDelayerTitle, loc_almanac_unitDelayerInformation);
	
	//Bowler Base
	else if(realImageI == 1 && realImageJ == 0) SetParagraph(loc_almanac_bolasThrowerBaseTitle, loc_almanac_bolasThrowerBaseInformation);
	//Bowler Root
	else if(realImageI == 1 && realImageJ == 2) SetParagraph(loc_almanac_bolasThrowerFloorTitle, loc_almanac_bolasThrowerFloorInformation);
	//Bowler Confuser
	else if(realImageI == 1 && realImageJ == 1) SetParagraph(loc_almanac_bolasThrowerConfuseTitle, loc_almanac_bolasThrowerConfuseInformation);

	//Warrior Base
	else if(realImageI == 2 && realImageJ == 0) SetParagraph(loc_almanac_baseWarriorTitle, loc_almanac_baseWarriorInformation);
	//Warrior Tank
	else if(realImageI == 2 && realImageJ == 2) SetParagraph(loc_almanac_resistantWarriorTitle, loc_almanac_resistantWarriorInformation);
	//Warrior Berserk
	else if(realImageI == 2 && realImageJ == 1) SetParagraph(loc_almanac_aggressiveWarriorTitle, loc_almanac_aggressiveWarriorInformation);

	//Mage Base
	else if(realImageI == 3 && realImageJ == 0) SetParagraph(loc_almanac_baseMageTitle, loc_almanac_baseMageInformation);
	//Mage Speeder
	else if(realImageI == 3 && realImageJ == 1) SetParagraph(loc_almanac_speederMageTitle, loc_almanac_speederMageInformation);
	//Mage Mighter
	else if(realImageI == 3 && realImageJ == 2) SetParagraph(loc_almanac_mightierMageTitle, loc_almanac_mightierMageInformation);
}


void Almanac::SetParagraph(const char* title, const char* firstPar)
{
	sprintf(detailedInfoTitle, "%s", title);
	sprintf(detailedInfoText, "%s", firstPar);
}

void Almanac::Render()
{
	Utils::RenderPannelA(renderPanelNumWidth, renderPanelNumHeight, pannelXPos, pannelYPos);

	//if(currentGameHeight >= 768)
	//{
	//	for(int i = 0; i < 4; i++)
	//	{
	//		for(int j = 0; j < 3; j++)
	//		{
	//			int selected = 0;
	//			if(realImageI == i && realImageJ == j || portraitObject[i][j].bIsFocused)
	//			{
	//				selected = 4;
	//			}

	//			//Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[i + selected][j + disabledPortrait[i][j]].atlasName), CIwSVec2((int)(xPortrait + (float)((96 + 20) * j)), (int)yPortrait + 15 + 105 * i), portraitArray[i + selected][j + disabledPortrait[i][j]]);
	//			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[i + selected][j + disabledPortrait[i][j]].atlasName), CIwSVec2(portraitObject[i][j].x, portraitObject[i][j].y), portraitArray[i + selected][j + disabledPortrait[i][j]]);
	//		}
	//	}
	//}
	//else
	//{
		for(int i = 0; i < 4; i++)
		{
			for(int j = 0; j < 3; j++)
			{
				int selected = 0;
				if(realImageI == i && realImageJ == j || portraitObject[i][j].bIsFocused)
				{
					selected = 4;
				}

				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[i + selected][j + disabledPortrait[i][j]].atlasName), CIwSVec2(portraitObject[i][j].x, portraitObject[i][j].y), portraitArray[i + selected][j + disabledPortrait[i][j]]);

				//Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(portraitArray[i + selected][j + disabledPortrait[i][j]].atlasName), CIwSVec2((int)(xPortrait + (float)((96 + 20) * i)), (int)yPortrait + 15 + 105 * j), portraitArray[i + selected][j + disabledPortrait[i][j]]);
			}
		}
	//}


	//Buttons
	btnBack->Render();
	
	//More detailed info popup.
	if(bViewingDetailedInfo)
	{
		//IwTrace(ALMANACINGAME, ("Rendering popup."));
		IwGxFlush();
		moreInfoBackground->Render();
		btnClose->Render();
		Utils::RenderPannelB(pannelBLengthX, pannelBLengthY, pannelBPosX, pannelBPosY);
		
		IwGxLightingOn();
		Utils::RenderText(detailedInfoTitle, 
			CIwRect(pannelBPosX + 52, pannelBPosY + 50, w - 81, 20),
			font_SmartKid24, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true, 2);
		IwGxFlush();
		Utils::RenderText(detailedInfoText, 
			CIwRect(pannelBPosX + 20, pannelBPosY + 120, w - 30, 98),
			font_SmartKid20, 0xffffffff, IW_GX_FONT_ALIGN_PARAGRAPH, IW_GX_FONT_ALIGN_TOP, true, 1);
		IwGxLightingOff();

		IwGxFlush();
		if(!bUseHDImages)
			Utils::RenderSingleTexture(selknamRealImage[realImageI * 3 + realImageJ], CIwSVec2(pannelBPosX + 30, pannelBPosY - 7), CIwSVec2((int)(64 * 1.4f), (int)(96 * 1.4f)), 0.0f, 0.0f, 64 / 512.0f, 96 / 512.0f);
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
			Utils::RenderSingleTexture(selknamRealImage[realImageI * 3 + realImageJ], CIwSVec2(pannelBPosX + 30, pannelBPosY - 15), CIwSVec2(96, 144), 0.0f, 0.0f, 96.0f / tmpAtlasWidth, 144.0f / tmpAtlasHeight);	
		}
	}
}

void Almanac::On()
{
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Almanac, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_Almanac, NULL);

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			disabledPortrait[i][j] = 0;
		}
	}
	
	if(ArcherBaseUpgrades.level == 0)
		disabledPortrait[0][0] = 2;
	if(ArcherRetarderUpgrades.level == 0)
		disabledPortrait[0][1] = 2;
	if(ArcherPoisonerUpgrades.level == 0)
		disabledPortrait[0][2] = 2;
	if(WarriorBaseUpgrades.level == 0)
		disabledPortrait[2][0] = 2;
	if(WarriorBerserkUpgrades.level == 0)
		disabledPortrait[2][1] = 2;
	if(WarriorTankUpgrades.level == 0)
		disabledPortrait[2][2] = 2;
	if(BowlerStunerUpgrades.level == 0)
		disabledPortrait[1][0] = 2;
	if(BowlerConfuserUpgrades.level == 0)
		disabledPortrait[1][1] = 2;
	if(BowlerRooterUpgrades.level == 0)
		disabledPortrait[1][2] = 2;
	if(ShamanHealerUpgrades.level == 0)
		disabledPortrait[3][0] = 2;
	if(ShamanHasteUpgrades.level == 0)
		disabledPortrait[3][1] = 2;
	if(ShamanMightUpgrades.level == 0)
		disabledPortrait[3][2] = 2;
		
	bViewingDetailedInfo = false;
	realImageI = realImageJ = -1;

#if defined DESKTOPPLATFORM
	focusManager.ClearFocusDirections();

	focusManager.UnableAllFocus();
	focusManager.SetInitialFocus(focusManager.GetDefaultFocus());

	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			CreateFocus(i, j);
		}
	}

	focusManager.SetFocusDirection(XBOX360_Button_Up, btnBack, &portraitObject[0][2]);
	for(int j = 0; j < 4; j++)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Down, &portraitObject[j][2], btnBack);
	}

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			portraitObject[i][j].bLocked = (disabledPortrait[i][j] == 2);
		}
	}

	focusManager.SetFocusDirection(XBOX360_Button_Directional, btnClose, btnClose);
#endif
}

#if defined DESKTOPPLATFORM
void Almanac::CreateFocus(int i, int j)
{
	if(ValidPortraitPos(i, j + 1))
		focusManager.SetFocusDirection(XBOX360_Button_Right, &portraitObject[j][i], &portraitObject[j + 1][i]);
	
	if(ValidPortraitPos(i, j - 1))
		focusManager.SetFocusDirection(XBOX360_Button_Left, &portraitObject[j][i], &portraitObject[j - 1][i]);

	if(ValidPortraitPos(i + 1, j))
		focusManager.SetFocusDirection(XBOX360_Button_Down, &portraitObject[j][i], &portraitObject[j][i + 1]);

	if(ValidPortraitPos(i - 1, j))
		focusManager.SetFocusDirection(XBOX360_Button_Up, &portraitObject[j][i], &portraitObject[j][i - 1]);
}

bool Almanac::ValidPortraitPos(int i, int j)
{
	return i >= 0 && j >= 0 && i < 3 && j < 4;// && disabledPortrait[j][i] != 2;
}
#endif

void Almanac::Off()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_Almanac);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_Almanac);
}

bool Almanac::MouseOn(int x, int y, int xPos, int yPos, int _width, int _height) 
{
	return (x >= xPos && 
		x <= (xPos + _width) && 
		y >= yPos && 
		y <= (yPos + (int)(_height)));
}

void Almanac::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bViewingDetailedInfo)
	{
		btnClose->HandleClick(event);
	}

	if (event->m_Pressed)
	{
		if(bViewingDetailedInfo)
		{
			if(!MouseOn(event->m_x, event->m_y, pannelBPosX, pannelBPosY, w, h) 
				&& !btnClose->MouseOn(event->m_x, event->m_y))
			{
				bViewingDetailedInfo = false;
				realImageI = realImageJ = -1;
			}

			return;
		}

		//if(currentGameHeight >= 768)
		//{
		//	for(int i = 0; i < 4; i++)
		//	{
		//		for(int j = 0; j < 3; j++)
		//		{
		//			if(MouseOn(event->m_x, event->m_y, (int)(xPortrait + (float)((96 + 20) * j)), (int)yPortrait + 15 + 105 * i, 96, 96))
		//			{
		//				OpenPopup(i, j);
		//			}
		//		}
		//	}
		//}
		//else
		//{
			for(int i = 0; i < 4; i++)
			{
				for(int j = 0; j < 3; j++)
				{
					if(MouseOn(event->m_x, event->m_y, (int)(xPortrait + (float)((96 + 20) * i)), (int)yPortrait + 15 + 105 * j, 96, 96))
					{
						OpenPopup(i, j);
					}
				}
			}
		//}

		
	}

	btnBack->HandleClick(event);

}

void Almanac::OpenPopup(int i, int j)
{
	Utils::PlaySoundFX(FX_Click_01);



	if(!disabledPortrait[i][j])
	{
		realImageI = i;
		realImageJ = j;

#if defined DESKTOPPLATFORM
		focusManager.ChangeFocus(btnClose);
#endif

		SetImageAndText(i, j);
	}
}

void Almanac::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bViewingDetailedInfo)
	{
		btnClose->HandleMotion(event);
	}
	else
	{
		btnBack->HandleMotion(event);

#if defined DESKTOPPLATFORM
		focusManager.UpdateMotionEvents(event);
#endif
	}
}

void Almanac::Update()
{
	if(popUpState != GSP_None)
		return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}