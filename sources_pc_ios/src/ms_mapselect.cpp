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

#include "ms_mapselect.h"
#include "background.h"
#include "common.h"
#include "localization.h"
#include "utils.h"
#include "texturehelper.h"
#include "survivalmodeconfig.h"
#if !defined DESKTOPPLATFORM
//#include "LeadboltExt.h"
#include "s3eFlurry.h"
#endif
#ifdef STEAMREQUIRED
#include "helpers/steam_achievements.h"
#endif

MapSelection* mapselRef = NULL;

//Mouse Motion
void HandleMotion_msel(s3ePointerMotionEvent* event) 
{
	if(mapselRef == NULL)
		return;

	mapselRef->UpdateMotionEvents(event);
}


//Mouse handler
void HandleClick_msel(s3ePointerEvent *event) 
{
	if(mapselRef == NULL)
		return;

	mapselRef->UpdateClickEvents(event);
}

//Custom functions
void SelectLevel_OnClick(void* data)
{
	if(tutorial->IsActive()) 
		return;

	MapButton* button = (MapButton*)data;
	currentMap = button->level;
	//currentMap = 25;
	//if((!bIsFullVersion && currentMap <= 7) || bIsFullVersion)
	//{
		gameMode = GAMEMODE_CAMPAIGN;
		gameState = GS_Game;
		bFirstMapWasPlayed = true;
	//}

#if !defined DESKTOPPLATFORM
	//Flurry Analytics
	if(bFlurryOn)
	{
		char charTmp[3];
		sprintf(charTmp, "%d", currentMap);
		string tmpMapString = charTmp;
		string tmpValue = "MapNumber|" + tmpMapString;
		s3eFlurryLogEventParams("event_MapCampaignLevel", tmpValue.c_str());
	}
#endif
}

void MS_Store_OnClick()
{
	mapSelectionState = MS_Store;
#if !defined DESKTOPPLATFORM
	//Flurry Analytics
	if(bFlurryOn)
	{
		s3eFlurryLogEvent("event_UpgradeScreen");
	}
#endif
#ifdef STEAMREQUIRED
	//GAME STATS
	if(g_SteamAchievements)
		g_SteamAchievements->StoreGlobalStats_SteamID("event_UpgradeScreen");
#endif
}

void MS_Back_OnClick()
{
	if(tutorial->IsActive())
		return;

	gameState = GS_MainMenu;
}

#if defined DESKTOPPLATFORM
void MapSelection_Xbox360_A()
{
	mapselRef->focusManager.ExecuteFocusedObject();	
}

void MapSelection_Xbox360_Up()
{
	mapselRef->focusManager.ChangeFocus(XBOX360_Button_Up);
}

void MapSelection_Xbox360_Down()
{
	mapselRef->focusManager.ChangeFocus(XBOX360_Button_Down);
}

void MapSelection_Xbox360_Left()
{
	mapselRef->focusManager.ChangeFocus(XBOX360_Button_Left);
}

void MapSelection_Xbox360_Right()
{
	mapselRef->focusManager.ChangeFocus(XBOX360_Button_Right);
}
#endif

MapSelection::MapSelection()
{
	mapselRef = this;
		
	TextureData btn_mapselect_data = TextureHelper::GetTextureNamed("btn_mapselect");

	font_MapNumber = font_komikadisplaybold22;
	
	if(currentGameHeight <= 640)
	{
		font_MapNumber = font_komikadisplaybold18;
	}

	btn_mapselect.numStates = 2;

	RenderData myRenderData[2];
	myRenderData[0] = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 0);
	myRenderData[1] = TextureHelper::GetRenderData(btn_mapselect_data, 1, 3, 1);

	for(int i = 0; i < 2; i++)
	{
		CIwSVec2 tmpSize = CIwSVec2((int16)myRenderData[i].width, (int16)myRenderData[i].height);
		tmpSize = Utils::GetScaledSize(tmpSize);
		myRenderData[i].width = (int)tmpSize.x;
		myRenderData[i].height = (int)tmpSize.y;
	}

	btn_mapselect.state[0].RollOut  = myRenderData[0];
	btn_mapselect.state[0].RollOn   = myRenderData[1];
	btn_mapselect.state[0].Pressed  = myRenderData[1];
	btn_mapselect.state[0].Inactive = myRenderData[0];

	btn_mapselect.state[1].RollOut  = myRenderData[0];
	btn_mapselect.state[1].RollOn   = myRenderData[1];
	btn_mapselect.state[1].Pressed  = myRenderData[1];
	btn_mapselect.state[1].Inactive = myRenderData[0];

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);


	//Stars collected text
	starCounter = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_infocard_star"));
	if(starCounter.bRotate)
	{
		starCounter.width = 139;
		starCounter.height = 196;
	}
	else
	{
		starCounter.width = 196;
		starCounter.height = 139;
	}

	CIwSVec2 starCounterDimension = TextureHelper::GetDimension(starCounter);
	starsCounter_position = CIwSVec2(currentGameWidth - 300, 0);
	starsCollected_Position = CIwRect(starsCounter_position.x - 15, starsCounter_position.y + 22, starCounterDimension.x, starCounterDimension.y);
	starsCollected_Color = 0xffffffff;

	//Title
	pfont = font_komikadisplaybold25;
	titlePos = CIwRect(500, 20, 300, 30);
	titleColor = 0xffffffff;

	for(int i = 0; i < NUM_LEVELS; i++) 
	{
		map_button[i].map_btn = new Button(TextureHelper::GetCIwTexture(btn_mapselect.state[0].RollOut.atlasName), &btn_mapselect, 0, 0);
		map_button[i].map_btn->OnClick(&SelectLevel_OnClick, (void*)&map_button[i]);
		map_button[i].level = -1;
	}

	//Button Positions

	map_button[0].map_btn->SetPos(Utils::GetFixedScaledInt(196), Utils::GetScaledInt(375));
	map_button[1].map_btn->SetPos(Utils::GetFixedScaledInt(233), Utils::GetScaledInt(242));
	map_button[2].map_btn->SetPos(Utils::GetFixedScaledInt(218), Utils::GetScaledInt(148));
	map_button[3].map_btn->SetPos(Utils::GetFixedScaledInt(111), Utils::GetScaledInt(82));
	map_button[4].map_btn->SetPos(Utils::GetFixedScaledInt(186), Utils::GetScaledInt(25));
	map_button[5].map_btn->SetPos(Utils::GetFixedScaledInt(308), Utils::GetScaledInt(55));
	map_button[6].map_btn->SetPos(Utils::GetFixedScaledInt(430), Utils::GetScaledInt(80));
	map_button[7].map_btn->SetPos(Utils::GetFixedScaledInt(545), Utils::GetScaledInt(60));
	map_button[8].map_btn->SetPos(Utils::GetFixedScaledInt(410), Utils::GetScaledInt(180));
	map_button[9].map_btn->SetPos(Utils::GetFixedScaledInt(380), Utils::GetScaledInt(275));
	map_button[10].map_btn->SetPos(Utils::GetFixedScaledInt(490), Utils::GetScaledInt(336));
	map_button[11].map_btn->SetPos(Utils::GetFixedScaledInt(365), Utils::GetScaledInt(430));
	map_button[12].map_btn->SetPos(Utils::GetFixedScaledInt(490), Utils::GetScaledInt(440));
	map_button[13].map_btn->SetPos(Utils::GetFixedScaledInt(600), Utils::GetScaledInt(500));
	map_button[14].map_btn->SetPos(Utils::GetFixedScaledInt(674), Utils::GetScaledInt(438));
	map_button[15].map_btn->SetPos(Utils::GetFixedScaledInt(766), Utils::GetScaledInt(520));
	map_button[16].map_btn->SetPos(Utils::GetFixedScaledInt(890), Utils::GetScaledInt(481));
	map_button[17].map_btn->SetPos(Utils::GetFixedScaledInt(1007), Utils::GetScaledInt(463));
	map_button[18].map_btn->SetPos(Utils::GetFixedScaledInt(1057), Utils::GetScaledInt(348));
	map_button[19].map_btn->SetPos(Utils::GetFixedScaledInt(938), Utils::GetScaledInt(378));
	map_button[20].map_btn->SetPos(Utils::GetFixedScaledInt(790), Utils::GetScaledInt(391));
	map_button[21].map_btn->SetPos(Utils::GetFixedScaledInt(678), Utils::GetScaledInt(297));
	map_button[22].map_btn->SetPos(Utils::GetFixedScaledInt(706), Utils::GetScaledInt(172));
	map_button[23].map_btn->SetPos(Utils::GetFixedScaledInt(829), Utils::GetScaledInt(278));
	map_button[24].map_btn->SetPos(Utils::GetFixedScaledInt(860), Utils::GetScaledInt(220));


	xModifier = 1024.0f / 1280.0f;
	yModifier = 768.0f / 720.0f;
	
	for(int i = 0; i < 24; i++)
	{
		map_button[i].map_btn->SetPos((int)(map_button[i].map_btn->GetX() * xModifier), (int)(map_button[i].map_btn->GetY() * yModifier));
	}
	
	//levels
	map_button[0].level = 1;
	map_button[1].level = 2;
	map_button[2].level = 3;
	map_button[3].level = 4;
	map_button[4].level = 5;
	map_button[5].level = 6;
	map_button[6].level = 7;
	map_button[7].level = 8;
	map_button[8].level = 9;
	map_button[9].level = 10;
	map_button[10].level = 11;
	map_button[11].level = 12;
	map_button[12].level = 13;
	map_button[13].level = 14;
	map_button[14].level = 15;
	map_button[15].level = 16;
	map_button[16].level = 17;
	map_button[17].level = 18;
	map_button[18].level = 19;
	map_button[19].level = 20;
	map_button[20].level = 21;
	map_button[21].level = 22;
	map_button[22].level = 23;
	map_button[23].level = 24;
	map_button[24].level = 25;

	for(int i = 0; i < NUM_LEVELS; i++)
	{
		int level = map_button[i].level;
		if(MapStatus_State[level - 1] == PLAYABLE)
		{
			//Stage was completed can be played yet.
			map_button[i].map_btn->SetImage(1);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
		else if(MapStatus_State[level - 1] == NOT_PLAYABLE)
		{
			//Stage locked, use column 3 on png image and disable the click function.
			map_button[i].map_btn->SetImage(2);
			map_button[i].map_btn->FunctionOnClickEnabled(false);
		}
		else if(MapStatus_State[level - 1] == CURRENT_PLAYABLE)
		{
			//Stage is not completed yet.
			map_button[i].map_btn->SetImage(0);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
	}

	btnBack = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, Utils::GetFixedScaledInt(140), currentGameHeight - Utils::GetScaledInt(110));
	btnBack->OnClick(&MS_Back_OnClick);

	btnUpgrade = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, Utils::GetFixedScaledInt(540), currentGameHeight - Utils::GetScaledInt(110));
	btnUpgrade->OnClick(&MS_Store_OnClick);

	predefined_position[0][0] = CIwSVec2(Utils::GetScaledInt(40), Utils::GetScaledInt(20));
	predefined_position[1][0] = CIwSVec2(Utils::GetScaledInt(30), Utils::GetScaledInt(20));
	predefined_position[1][1] = CIwSVec2(Utils::GetScaledInt(50), Utils::GetScaledInt(20));
	predefined_position[2][0] = CIwSVec2(Utils::GetScaledInt(25), Utils::GetScaledInt(30));
	predefined_position[2][1] = CIwSVec2(Utils::GetScaledInt(40), Utils::GetScaledInt(15));
	predefined_position[2][2] = CIwSVec2(Utils::GetScaledInt(55), Utils::GetScaledInt(30));

	singleStarRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_star_little"));
	singleStarRenderData.width = Utils::GetScaledInt(singleStarRenderData.width);
	singleStarRenderData.height = Utils::GetScaledInt(singleStarRenderData.height);
	singleStarEasterEggOffsetXPos = Utils::GetScaledInt(71);
	singleStarEasterEggOffsetYPos = Utils::GetScaledInt(25);


	btnUpgrade->SetText(loc_ui_upgrades);
	btnBack->SetText(loc_ui_back);

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, &MapSelection_Xbox360_A);
	xbox360Controller.SetCallback(Xbox360_Action1, &MapSelection_Xbox360_A);
	xbox360Controller.SetCallback(Xbox360_Action3, &MapSelection_Xbox360_A);
	xbox360Controller.SetCallback(Xbox360_Cancel1, &MS_Back_OnClick);
	xbox360Controller.SetCallback(Xbox360_Cancel2, &MS_Back_OnClick);
	xbox360Controller.SetCallback(Xbox360_Up, &MapSelection_Xbox360_Up);
	xbox360Controller.SetCallback(Xbox360_Down, &MapSelection_Xbox360_Down);
	xbox360Controller.SetCallback(Xbox360_Right, &MapSelection_Xbox360_Right);
	xbox360Controller.SetCallback(Xbox360_Left, &MapSelection_Xbox360_Left);

	focusManager.SetDefaultFocus(btnBack);
#endif
}

MapSelection::~MapSelection()
{
	for(int i = 0; i < NUM_LEVELS; i++)
		delete map_button[i].map_btn;

	delete btnBack;
	delete btnUpgrade;
	font_MapNumber = NULL;
}

void MapSelection::Render()
{
	Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(starCounter.atlasName), starsCounter_position, starCounter);
	IwGxFlush();

	//if(bIsFullVersion)
	//{
		for(int i = 0; i < NUM_LEVELS; i++)
		{
			if(map_button[i].map_btn->GetImageID() != 2)
			{
				map_button[i].map_btn->Render();
			}
		}
	//}
	//else
	//{
	//	for(int i = 0; i < 7; i++)
	//	{
	//		if(map_button[i].map_btn->GetImageID() != 2)
	//		{
	//			map_button[i].map_btn->Render();
	//		}
	//	}
	//}

	IwGxFlush();
	

	for(int i = 0; i < NUM_LEVELS; i++)
	{
		for(int j = 0; j < Stars_By_Level[i]; j++)
		{
			if(i == 24 && Stars_By_Level[i] > 0)
			{
				CIwSVec2 tmpPos = CIwSVec2(stars_position[24][0].x + singleStarEasterEggOffsetXPos,  stars_position[24][0].y + singleStarEasterEggOffsetYPos);
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(singleStarRenderData.atlasName), tmpPos, singleStarRenderData);
			}
			else
			{
				Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(singleStarRenderData.atlasName), stars_position[i][j], singleStarRenderData);
			}
		}
	}
	
	IwGxFlush();

	btnUpgrade->Render();
	btnBack->Render();

	IwGxLightingOn();
	Utils::RenderText(starsCollected_Text, starsCollected_Position, font_komikadisplaybold18);
	int size = NUM_LEVELS;
	//if(!bIsFullVersion) size = 7;
	for(int i = 0; i < size; i++)
	{
		if(map_button[i].map_btn->GetImageID() != 2)
		{
			char tmp[10];
			if(map_button[i].level == 25)
			{
				sprintf(tmp, "BOSS");
				Utils::RenderText(tmp, CIwRect(map_button[i].map_btn->GetX(), map_button[i].map_btn->GetY(), map_button[i].map_btn->GetWidth() + 20, map_button[i].map_btn->GetHeight() + 4), font_MapNumber, 0xff00edf8, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_BOTTOM, true, 1);
			}
			else
			{
				sprintf(tmp, "%d", map_button[i].level);
				Utils::RenderText(tmp, CIwRect(map_button[i].map_btn->GetX(), map_button[i].map_btn->GetY(), map_button[i].map_btn->GetWidth() - 12, map_button[i].map_btn->GetHeight() + 4), font_MapNumber, 0xff00edf8, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_BOTTOM, true, 1);
			}
		}
	}
	IwGxLightingOff();
}

void MapSelection::SetMapButtons()
{
	for(int i = 0; i < NUM_LEVELS; i++)
	{
		int level = map_button[i].level;
		if(MapStatus_State[level - 1] == PLAYABLE)
		{
			//Stage was completed can be played yet.
			map_button[i].map_btn->SetImage(1);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
		else if(MapStatus_State[level - 1] == NOT_PLAYABLE)
		{
			//Stage locked, use column 3 on png image and disable the click function.
			map_button[i].map_btn->SetImage(2);
			map_button[i].map_btn->FunctionOnClickEnabled(false);
		}
		else if(MapStatus_State[level - 1] == CURRENT_PLAYABLE)
		{
			//Stage is not completed yet.
			map_button[i].map_btn->SetImage(0);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
		
		if(bCheatModeEnabled)
		{
			map_button[i].map_btn->SetImage(1);
			map_button[i].map_btn->FunctionOnClickEnabled(true);
		}
	}

	int current_stars = 0;
	for(int i = 0; i < MAX_MAPS_CAMPAIGN; i++)
		current_stars += Stars_By_Level[i];
	sprintf(starsCollected_Text, "%d / %d", current_stars, Max_Stars);

	if(bShowTutorialSurvivalModeUnlock && MapStatus_State[17] == PLAYABLE)
	{
		tutorial->SetSkipped(false);
		tutorial->InitTutorial(Tutorial_SurvivalMode);
		bShowTutorialSurvivalModeUnlock = false;
	}
	else if(bShowTutorialStars)
	{
		if(current_stars >= 3)
		{
			tutorial->SetSkipped(false);
			tutorial->InitTutorial(Tutorial_Stars);
			bShowTutorialStars = false;
		}
	}

	//Achievement nº 7.
	//A new challenge: Modo survival desbloqueado.
	if(survivalModeConfig->bIsSurvivalAvailable)
	{
		achievementsHelper->SetAchievementCompletedById(6);
	}

	//stars per level
	for(int i = 0; i < NUM_LEVELS; i++)
	{
		for(int j = 0; j < Stars_By_Level[i]; j++)
		{
			stars_position[i][j] = predefined_position[Stars_By_Level[map_button[i].level - 1] - 1][j];
			stars_position[i][j].x += map_button[i].map_btn->GetX();
			stars_position[i][j].y += map_button[i].map_btn->GetY();
		}
	}

	btnUpgrade->Enabled(current_stars >= 3);
}

void MapSelection::InitObject()
{
	tutorial->SetMapSelectionRef(this);

	xWindowOffset = yWindowOffset = 0;

	bExtraMapMode = false;
	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_msel, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_msel, NULL);

	SetMapButtons();

	for(int i = 0; i < NUM_LEVELS; i++)
	{
		map_button[i].map_btn->SetStatus(2);
	}
	btnBack->SetStatus(2);
	btnUpgrade->SetStatus(2);

#if defined DESKTOPPLATFORM
	focusManager.ClearFocusDirections();

	focusManager.ChangeFocus(NULL);

	bool bMapFound = false;

	for(int i = 0; i < MAX_MAPS_CAMPAIGN; i++)
	{
		int level = map_button[i].level - 1;

		if(MapStatus_State[level] == CURRENT_PLAYABLE)
		{
			focusManager.ChangeFocus(map_button[i].map_btn);
			bMapFound = true;
		}
		else if(MapStatus_State[level] == NOT_PLAYABLE)
		{
			focusManager.BlockUpdateMotionFor(map_button[i].map_btn);
		}
	}

	if(!bMapFound)
	{
		focusManager.ChangeFocus(map_button[24].map_btn);
	}

	focusManager.SetFocusDirection(XBOX360_Button_Up, btnBack, map_button[0].map_btn);
	focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[0].map_btn, btnBack);

	if(btnUpgrade->IsEnabled())
	{
		focusManager.SetFocusDirection(XBOX360_Button_Right, btnBack, btnUpgrade);
		focusManager.SetFocusDirection(XBOX360_Button_Left, btnUpgrade, btnBack);
		focusManager.SetFocusDirection(XBOX360_Button_Up, btnUpgrade, map_button[15].map_btn);
	}

	//UP - DOWN - LEFT - RIGHT
	LinkMapButtons(0, 1, -1, -1, 11);
	LinkMapButtons(1, 2, 0, -1, 9);
	LinkMapButtons(2, 3, 1, 3, 8);
	LinkMapButtons(3, 4, 2, -1, 4);
	LinkMapButtons(4, -1, 3, 3, 5);
	LinkMapButtons(5, -1, -1, 4, 6);
	LinkMapButtons(6, -1, 8, 5, 7);
	LinkMapButtons(7, -1, 6, -1, 22);
	LinkMapButtons(8, 6, 9, 2, -1);
	LinkMapButtons(9, 8, 10, 1, 10);
	LinkMapButtons(10, 9, 11, 9, 8);
	LinkMapButtons(11, 10, -1, 0, 12); focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[11].map_btn, btnBack);
	LinkMapButtons(12, 10, -1, 11, 13); focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[12].map_btn, btnBack);
	LinkMapButtons(13, 13, -1, 12, 14); if(btnUpgrade->IsEnabled()) focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[13].map_btn, btnUpgrade);
	LinkMapButtons(14, 20, -1, 13, 15);
	LinkMapButtons(15, 14, -1, 14, 16); if(btnUpgrade->IsEnabled()) focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[15].map_btn, btnUpgrade);
	LinkMapButtons(16, -1, -1, 15, 17); if(btnUpgrade->IsEnabled()) focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[16].map_btn, btnUpgrade);
	LinkMapButtons(17, 19, -1, 16, -1); if(btnUpgrade->IsEnabled()) focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[17].map_btn, btnUpgrade);
	LinkMapButtons(18, 24, 17, 19, -1);
	LinkMapButtons(19, 23, 17, 20, 18); 
	LinkMapButtons(20, 21, 15, 14, 19);
	LinkMapButtons(21, 22, 20, 10, 23);
	LinkMapButtons(22, -1, 21, 7, 23);
	LinkMapButtons(23, 22, 20, 21, 19);
	LinkMapButtons(24, -1, 18, -1, -1);
#endif

#if !defined DESKTOPPLATFORM

	if(!bIsFullVersion && s3eSocketGetInt(S3E_SOCKET_NETWORK_AVAILABLE) == 0)
	{
		popUpState = GSP_NOINTERNET;
	}

#endif
}

#if defined DESKTOPPLATFORM
void MapSelection::LinkMapButtons(int source, int up, int down, int left, int right)
{
	if(map_button[source].map_btn->GetImageID() == 2)
		return;

	if(up != -1 && map_button[up].map_btn->GetImageID() != 2)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Up, map_button[source].map_btn, map_button[up].map_btn);
	}

	if(down != -1 && map_button[down].map_btn->GetImageID() != 2)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Down, map_button[source].map_btn, map_button[down].map_btn);
	}

	if(left != -1 && map_button[left].map_btn->GetImageID() != 2)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Left, map_button[source].map_btn, map_button[left].map_btn);
	}

	if(right != -1 && map_button[right].map_btn->GetImageID() != 2)
	{
		focusManager.SetFocusDirection(XBOX360_Button_Right, map_button[source].map_btn, map_button[right].map_btn);
	}
}
#endif

void MapSelection::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_msel);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_msel);
}

void MapSelection::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bPauseMapSelection)
		return;

	int size = NUM_LEVELS;
	//if(!bIsFullVersion) size = 7;

	if(tutorial->currBreakpoint != Tutorial_BreakPoint_MapSelection)
	{
		for(int i = 0; i < size; i++)
		{
			if(map_button[i].map_btn->GetImageID() != 2)
				map_button[i].map_btn->HandleClick(event);
		}
	}

	btnUpgrade->HandleClick(event);

	if(tutorial->currBreakpoint != Tutorial_BreakPoint_MapSelection)
	{
		btnBack->HandleClick(event);
	}
}

void MapSelection::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bPauseMapSelection)
		return;

	int size = NUM_LEVELS;
	//if(!bIsFullVersion) size = 7;

	for(int i = 0; i < size; i++)
	{
		if(map_button[i].map_btn->GetImageID() != 2)
		{
			map_button[i].map_btn->HandleMotion(event);
		}
	}

	btnUpgrade->HandleMotion(event);
	btnBack->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

bool MapSelection::isValid(int position)
{
	if(position == 26 || position == 25) return true;
	if(position < 0) return false;
	if(map_button[position].map_btn == NULL) return false;
	//if(!bIsFullVersion && position >= 7) return false;
	return map_button[position].map_btn->GetImageID() != 2;
}

void MapSelection::Update()
{
#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}