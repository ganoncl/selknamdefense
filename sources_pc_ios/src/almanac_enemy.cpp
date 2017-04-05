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

#include "almanac_enemy.h"

AlmanacEnemy* almanacEnemyRef = NULL;

//Mouse Motion
void HandleMotion_AlmanacEnemy(s3ePointerMotionEvent* event) 
{
	if(almanacEnemyRef == NULL)
		return;

	almanacEnemyRef->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClick_AlmanacEnemy(s3ePointerEvent *event) 
{
	if(almanacEnemyRef == NULL)
		return;

	almanacEnemyRef->UpdateClickEvents(event);
}


void btnBackAction_AlmanacEnemy()
{
	curAlmanacState = ALMANAC_MENU;
}

void onCloseActionAlmanacEnemy()
{
	almanacEnemyRef->ClosePopup();
}

#if defined DESKTOPPLATFORM
void EnemyAlmanac_Xbox360_Action()
{
	almanacEnemyRef->focusManager.ExecuteFocusedObject();
}

void EnemyAlmanac_Xbox360_Left()
{
	almanacEnemyRef->focusManager.ChangeFocus(Xbox360_Left);
}

void EnemyAlmanac_Xbox360_Right()
{
	almanacEnemyRef->focusManager.ChangeFocus(Xbox360_Right);
}

void EnemyAlmanac_Xbox360_Up()
{
	almanacEnemyRef->focusManager.ChangeFocus(Xbox360_Up);
}

void EnemyAlmanac_Xbox360_Down()
{
	almanacEnemyRef->focusManager.ChangeFocus(Xbox360_Down);
}

void EnemyAlmanac_Xbox360_BackAction()
{
	if(almanacEnemyRef->bViewingDetailedInfo)
	{
		onCloseActionAlmanacEnemy();
	}
	else
	{
		btnBackAction_AlmanacEnemy();
	}
}
#endif

void EnemyAlmanac_OpenPopup(int i, int j)
{
	almanacEnemyRef->SetImageAndText(i, j);
}

AlmanacEnemy::AlmanacEnemy()
{
	bIsActive = false;
	
	almanacEnemyRef = this;

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

	btnBackPosX = (int)((currentGameWidth - btnTexWidth) * 0.5f);
	btnBackPosY = currentGameHeight - Utils::GetScaledInt(110);

	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnBackPosX, btnBackPosY);
	btnBack->OnClick(&btnBackAction_AlmanacEnemy);
	btnBack->SetText(loc_ui_back);

	buttonEnemyPortraitXOffset = 0;
	buttonEnemyPortraitYOffset = 0;

	pannelUnit = 40;
	pannelLengthX = 21;
	pannelLengthY = 11;
	buttonEnemyPortraitWidth = 200;
	buttonEnemyPortraitHeight = 200;

	if(currentGameHeight < 768)
	{
		pannelUnit = 40;
		pannelLengthX = 16;
		pannelLengthY = 7;
		buttonEnemyPortraitWidth = 140;
		buttonEnemyPortraitHeight = 140;
		buttonEnemyPortraitXOffset = 15;
		buttonEnemyPortraitYOffset = -13;
	}

	pannelWidth = pannelLengthX * pannelUnit + 2 * pannelUnit;
	pannelHeight = pannelLengthY * pannelUnit + 2 * pannelUnit;
	pannelXPos = (int)((currentGameWidth - pannelWidth) * 0.5f);
	pannelYPos = (int)((currentGameHeight - pannelHeight) * 0.5f) + 10;

	if(currentGameHeight <= 600)
		pannelYPos = pannelYPos - 25;

	TextureData backgroundButtonTemp = TextureHelper::GetTextureNamed("background_enemies");
	backgroundButtonData = TextureHelper::GetRenderData(backgroundButtonTemp);
	backgroundButtonData.width  = buttonEnemyPortraitWidth;
	backgroundButtonData.height = buttonEnemyPortraitHeight;
	backgroundButtonDimension = TextureHelper::GetDimension(backgroundButtonData);
	backgroundButtonAtlasName = TextureHelper::GetAtlasName(backgroundButtonTemp);

	TextureData backgroundFocusButtonTemp = TextureHelper::GetTextureNamed("background_enemies_highlighted");
	backgroundFocusButtonData = TextureHelper::GetRenderData(backgroundFocusButtonTemp);
	backgroundFocusButtonData.width  = buttonEnemyPortraitWidth;
	backgroundFocusButtonData.height = buttonEnemyPortraitHeight;
	backgroundFocusButtonDimension = TextureHelper::GetDimension(backgroundFocusButtonData);
	backgroundFocusButtonAtlasName = TextureHelper::GetAtlasName(backgroundFocusButtonTemp);

	TextureData buttonData = TextureHelper::GetTextureNamed("btn_popup_long");
	button = new ButtonRenderData();
	button->numStates = 1;
	button->state[0].RollOut  = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	button->state[0].RollOn   = TextureHelper::GetRenderData(buttonData, 1, 3, 1);
	button->state[0].Pressed  = TextureHelper::GetRenderData(buttonData, 1, 3, 2);
	button->state[0].Inactive = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	int buttonWidth = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).x;

	moreInfoBackground = new Background(0, 0, 0, 170);

	pannelBLengthX = 36;
	pannelBLengthY = 23;

	pannelBPosX = (int)((currentGameWidth - (32 + pannelBLengthX * 16)) * 0.5f);
	pannelBPosY = (int)((currentGameHeight - (32 + pannelBLengthY * 16)) * 0.5f);
	if(currentGameHeight <= 600)
		pannelBPosY = pannelBPosY - 35;
	w = 32 + pannelBLengthX * 16;
	h = 32 + pannelBLengthY * 16;

	btnClose = new Button(TextureHelper::GetCIwTexture(button->state[0].RollOut.atlasName), button, pannelBPosX + int((w - buttonWidth)* 0.5f), pannelBPosY + h - 10);
	btnClose->OnClick(&onCloseActionAlmanacEnemy);
	btnClose->SetText(loc_ui_close);
	btnClose->SetStyle(font_komikadisplaybold18, 0, -15);

	SetPortraitEnemy(0, "enemy_soldier");
	SetPortraitEnemy(1, "enemy_tank");
	SetPortraitEnemy(2, "enemy_varon");
	SetPortraitEnemy(3, "enemy_rabbit");
	SetPortraitEnemy(4, "enemy_scientist");
	SetPortraitEnemy(5, "enemy_suicida");
	SetPortraitEnemy(6, "enemy_pirate");
	SetPortraitEnemy(7, "enemy_boss");

	SetIndexToEnemyType(ENEMY_Soldier  , 0);
	SetIndexToEnemyType(ENEMY_Tank     , 1);
	SetIndexToEnemyType(ENEMY_Varon    , 2);
	SetIndexToEnemyType(ENEMY_Rabbit   , 3);
	SetIndexToEnemyType(ENEMY_Scientist, 4);
	SetIndexToEnemyType(ENEMY_Convict  , 5);
	SetIndexToEnemyType(ENEMY_Pirate   , 6);
	SetIndexToEnemyType(ENEMY_Captain  , 7); 


	if(!bUseHDImages)
	{
		enemyRealImage[0] = atlas_enemy_01_1; //Soldier
		enemyRealImage[1] = atlas_enemy_05_1; //Tank
		enemyRealImage[2] = atlas_enemy_02_1; //Baron
		enemyRealImage[3] = atlas_enemy_06_1; //Rabbit
		enemyRealImage[4] = atlas_enemy_04_1; //Chemist
		enemyRealImage[5] = atlas_enemy_03_1; //Convict
		enemyRealImage[6] = atlas_enemy_07_1; //Pirate
	}
	else
	{
		enemyRealImage[0] = atlas_hd_enemy_1; //Soldier
		enemyRealImage[1] = atlas_hd_enemy_5; //Tank
		enemyRealImage[2] = atlas_hd_enemy_2; //Baron
		enemyRealImage[3] = atlas_hd_enemy_6; //Rabbit
		enemyRealImage[4] = atlas_hd_enemy_4_01; //Chemist
		enemyRealImage[5] = atlas_hd_enemy_3; //Convict
		enemyRealImage[6] = atlas_hd_enemy_7; //Pirate
	}
	enemyRealImage[7] = TextureHelper::GetCIwTexture(TextureHelper::GetAtlasName(TextureHelper::GetTextureNamed("attack0002"))); //Captain

	bViewingDetailedInfo = false;

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			portraits[i][j].i = i;
			portraits[i][j].j = j;
			portraits[i][j].onClick = &EnemyAlmanac_OpenPopup;
		}
	}

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, &EnemyAlmanac_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Action2, &EnemyAlmanac_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Up, &EnemyAlmanac_Xbox360_Up);
	xbox360Controller.SetCallback(Xbox360_Down, &EnemyAlmanac_Xbox360_Down);
	xbox360Controller.SetCallback(Xbox360_Left, &EnemyAlmanac_Xbox360_Left);
	xbox360Controller.SetCallback(Xbox360_Right, &EnemyAlmanac_Xbox360_Right);
	xbox360Controller.SetCallback(Xbox360_Cancel1, &EnemyAlmanac_Xbox360_BackAction);
	xbox360Controller.SetCallback(Xbox360_Cancel2, &EnemyAlmanac_Xbox360_BackAction);
	focusManager.BlockUpdateMotionFor(btnClose);

	for(int i = 0; i < 2; i++)
	{
		for(int j = 3; j >= 0; j--)
		{
			int textureIndex = 4 * i + j;

			if(i > 0)
				focusManager.SetFocusDirection(Xbox360_Up, &portraits[i][j], &portraits[i - 1][j]);

			if(i + 1 < 2)
				focusManager.SetFocusDirection(Xbox360_Down, &portraits[i][j], &portraits[i + 1][j]);

			if(j + 1 < 4)
				focusManager.SetFocusDirection(Xbox360_Right, &portraits[i][j], &portraits[i][j + 1]);

			if(j > 0)
				focusManager.SetFocusDirection(Xbox360_Left, &portraits[i][j], &portraits[i][j - 1]);
		}
	}

	focusManager.SetFocusDirection(Xbox360_Up, btnBack, &portraits[1][0]);
	for(int i = 0; i < 4; i++)
	{
		focusManager.SetFocusDirection(Xbox360_Down, &portraits[1][i], btnBack);
	}
#endif
}

AlmanacEnemy::~AlmanacEnemy()
{
	delete btnBack;
	delete button;
	delete btnClose;
	delete moreInfoBackground;
}

void AlmanacEnemy::SetIndexToEnemyType(IsoEnemyType type, int index)
{
	indexToEnemyType[index] = (int)type;
}

int AlmanacEnemy::GetIndexOfPortrait(int portraitIndex)
{
	return indexToEnemyType[portraitIndex] - 1;
}

void AlmanacEnemy::SetParagraph(const char* title, const char* firstPar)
{
	sprintf(detailedInfoTitle, "%s", title);
	sprintf(detailedInfoText, "%s", firstPar);
}

void AlmanacEnemy::SetPortraitEnemy(int index, string name)
{
	TextureData tmp = TextureHelper::GetTextureNamed(name.c_str());
	
	if(name != "enemy_soldier")
	{
		enemyPortrait[index] = TextureHelper::GetMirrorRenderData(TextureHelper::GetRenderData(tmp), MIRROR_VERTICAL);
	}
	else
	{
		enemyPortrait[index] = TextureHelper::GetRenderData(tmp);
	}

	enemyPortrait[index].width = buttonEnemyPortraitWidth;
	enemyPortrait[index].height = buttonEnemyPortraitHeight;
	enemyPortraitAtlasName[index] = TextureHelper::GetAtlasName(tmp);
}

void AlmanacEnemy::InitObject()
{
	bIsActive = true;

	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_AlmanacEnemy, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_AlmanacEnemy, NULL);

	bViewingDetailedInfo = false;

#if defined DESKTOPPLATFORM
	focusManager.SetInitialFocus(btnBack);
#endif
}

void AlmanacEnemy::EndObject()
{
	bIsActive = false;

	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_AlmanacEnemy);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_AlmanacEnemy);

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(NULL);
#endif
}

void AlmanacEnemy::Render()
{
	if(!bIsActive)
		return;

	Utils::RenderPannelA(pannelLengthX, pannelLengthY, pannelXPos, pannelYPos);

	for(int i = 0; i < 2; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			if(portraits[i][j].bIsFocused)
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundFocusButtonAtlasName), GetPortraitPosition(i, j), backgroundFocusButtonData);
			else
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(backgroundButtonAtlasName), GetPortraitPosition(i, j), backgroundButtonData);

			int textureIndex = 4 * i + j;
			if(isEnemyUnlocked[GetIndexOfPortrait(textureIndex)])
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(enemyPortraitAtlasName[textureIndex]), GetPortraitPosition(i, j), enemyPortrait[textureIndex]);
			}
			else
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(enemyPortraitAtlasName[textureIndex]),  enemyPortrait[textureIndex], GetPortraitPosition(i, j), 0, 0, 0);
			}
		}
	}

	btnBack->Render();
	
	//More detailed info popup.
	if(bViewingDetailedInfo)
	{
		moreInfoBackground->Render();
		btnClose->Render();
		Utils::RenderPannelB(pannelBLengthX, pannelBLengthY, pannelBPosX, pannelBPosY);

		IwGxLightingOn();
		Utils::RenderText(detailedInfoTitle, 
			CIwRect(pannelBPosX + 52, pannelBPosY + 50, w - 81, 20),
			font_SmartKid24, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true, 2);
		Utils::RenderText(detailedInfoText, 
			CIwRect(pannelBPosX + 20, pannelBPosY + 120, w - 30, 98),
			font_SmartKid20, 0xffffffff, IW_GX_FONT_ALIGN_PARAGRAPH, IW_GX_FONT_ALIGN_TOP, true, 1);
		IwGxLightingOff();

		IwGxFlush();

		int tmpIndex = enemyIindex * 4 + enemyJIndex;
		if(tmpIndex == 7)
		{
			Utils::RenderSingleTexture(enemyRealImage[tmpIndex], CIwSVec2(pannelBPosX + 30, pannelBPosY - 7), CIwSVec2((int)(96 * 1.4f), (int)(96 * 1.4f)), 0.0f, 0.0f, 140 / 1024.0f, 140 / 1024.0f);
		}
		else
		{
			if(!bUseHDImages)
				Utils::RenderSingleTexture(enemyRealImage[tmpIndex], CIwSVec2(pannelBPosX + 30, pannelBPosY - 7), CIwSVec2((int)(64 * 1.4f), (int)(96 * 1.4f)), 0.0f, 0.0f, 64 / 512.0f, 96 / 512.0f);
			else
			{
				//Archers, Bowler and Melee
				float tmpAtlasWidth = 960.0f;
				float tmpAtlasHeight = 1008.0f;

				if(tmpIndex == 0 || tmpIndex == 2 || tmpIndex == 4 || tmpIndex == 6)
				{
					tmpAtlasWidth = atlas_hdsize_enemy_1.width;
					tmpAtlasHeight = atlas_hdsize_enemy_1.height;
				}
				//tank
				else if(tmpIndex == 1)
				{
					tmpAtlasWidth = atlas_hdsize_enemy_5.width;
					tmpAtlasHeight = atlas_hdsize_enemy_5.height;
				}
				//rabbit
				else if(tmpIndex == 3)
				{
					tmpAtlasWidth = atlas_hdsize_enemy_6.width;
					tmpAtlasHeight = atlas_hdsize_enemy_6.height;
				}
				//convict
				else if(tmpIndex == 5)
				{
					tmpAtlasWidth = atlas_hdsize_enemy_3.width;
					tmpAtlasHeight = atlas_hdsize_enemy_3.height;
				}
				Utils::RenderSingleTexture(enemyRealImage[tmpIndex], CIwSVec2(pannelBPosX + 30, pannelBPosY - 15), CIwSVec2(96, 144), 0.0f, 0.0f, 96.0f / tmpAtlasWidth, 144.0f / tmpAtlasHeight);
			}
		}
	}
}

void AlmanacEnemy::SetImageAndText(int i, int j)
{
	if(!isEnemyUnlocked[GetIndexOfPortrait(4 * i + j)])
		return;

	bViewingDetailedInfo = true;

	enemyIindex = i;
	enemyJIndex = j;

	if(i == 0 && j == 0) SetParagraph(loc_almanac_soldierTitle, loc_almanac_soldierInfo);
	else if(i == 0 && j == 2) SetParagraph(loc_almanac_baronTitle, loc_almanac_baronInfo);
	else if(i == 1 && j == 1) SetParagraph(loc_almanac_convictTitle, loc_almanac_convictInfo);
	else if(i == 1 && j == 0) SetParagraph(loc_almanac_chemistTitle, loc_almanac_chemistInfo);
	else if(i == 0 && j == 1) SetParagraph(loc_almanac_tankTitle, loc_almanac_tankInfo);
	else if(i == 0 && j == 3) SetParagraph(loc_almanac_rabbitTitle, loc_almanac_rabbitInfo);
	else if(i == 1 && j == 2) SetParagraph(loc_almanac_pirateTitle, loc_almanac_pirateInfo);
	else if(i == 1 && j == 3) SetParagraph(loc_almanac_captainTitle, loc_almanac_captainInfo);
	else SetParagraph("ENEMY TITLE", "ENEMY TEXT 1 ENEMY TEXT 2");

#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(btnClose);
#endif
}

CIwSVec2 AlmanacEnemy::GetPortraitPosition(int i, int j)
{
	int margin = 15;

	portraits[i][j].x = pannelXPos + pannelUnit + backgroundButtonDimension.x * j + margin * j + buttonEnemyPortraitXOffset;
	portraits[i][j].y = pannelYPos + pannelUnit + backgroundButtonDimension.y * i + margin * i + 10 + buttonEnemyPortraitYOffset;
	portraits[i][j].w = buttonEnemyPortraitWidth;
	portraits[i][j].h = buttonEnemyPortraitHeight;

	return CIwSVec2(portraits[i][j].x, portraits[i][j].y);
}

void AlmanacEnemy::ClosePopup()
{
	bViewingDetailedInfo = false;
#if defined DESKTOPPLATFORM
	focusManager.ChangeFocus(&portraits[enemyIindex][enemyJIndex]);
#endif
}

void AlmanacEnemy::UpdateClickEvents(s3ePointerEvent *event)
{
	if(!bViewingDetailedInfo)
	{
		btnBack->HandleClick(event);
	}

	if(bViewingDetailedInfo)
	{
		btnClose->HandleClick(event);
	}

	if (event->m_Pressed)
	{
		if(bViewingDetailedInfo)
		{
			if(!Utils::MouseOn(event->m_x, event->m_y, pannelBPosX, pannelBPosY, w, h) 
				&& !btnClose->MouseOn(event->m_x, event->m_y))
			{
				bViewingDetailedInfo = false;
			}

			return;
		}

		for(int i = 0; i < 2; i++)
		{
			for(int j = 0; j < 4; j++)
			{
				CIwSVec2 position = GetPortraitPosition(i, j);
				if(Utils::MouseOn(event->m_x, event->m_y, position.x, position.y, backgroundButtonDimension.x, backgroundButtonDimension.y))
				{
					
					//Utils::PlayFXSound(FX_Sound_CLICK);
					Utils::PlaySoundFX(FX_Click_01);

					if(isEnemyUnlocked[GetIndexOfPortrait(4 * i + j)])
					{
						SetImageAndText(i, j);
					}
				}
			}
		}
	}
}

void AlmanacEnemy::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!bViewingDetailedInfo)
	{
		btnBack->HandleMotion(event);
	
#if defined DESKTOPPLATFORM
		focusManager.UpdateMotionEvents(event);
#endif
	}
}


void AlmanacEnemy::Update()
{
	if(!bIsActive)
		return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}