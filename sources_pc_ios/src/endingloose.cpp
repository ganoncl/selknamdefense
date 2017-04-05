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

#include "endingloose.h"
#include "texturehelper.h"
#include "survivalmodeconfig.h"
#if !defined DESKTOPPLATFORM
//#include "LeadboltExt.h"
#include "s3eFlurry.h"
#include "iOSNativeEDK.h"
#include "s3eChartBoost.h"
#include "s3eIOSIAd.h"
#endif
#ifdef GAMECENTER
#include "s3eIOSGameCenter.h"
#endif
#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif

EndingLoose *looseRef = NULL;

//Mouse Motion
void HandleMotion_loose(s3ePointerMotionEvent* event) 
{
	if(looseRef == NULL)
		return;
	
	looseRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_loose(s3ePointerEvent *event) 
{
	if(looseRef == NULL)
		return;

	looseRef->UpdateClickEvents(event);
}

void Loose_OnClick_PlayAgain()
{
	gameState = GS_Retry;
	bPause = false;
}

void Loose_OnClick_MapSelect()
{
	if(!bExtraMapMode)
		gameState = GS_MapSelection;
	else
		gameState = GS_ExtraMapSelection;
	inGameState = GSG_None;
}

void Loose_OnClick_QuitGame()
{
	popUpState = GSP_QuitGameLooseEnding;
}

void SurvivalMode_FacebookOnClick()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpValue = "Place|Survival";
		s3eFlurryLogEventParams("event_FacebookClicked", tmpValue.c_str());
	}
	//Entire Phrase
	string myTmpScore = " " + totalSurvivalScoreTmp + " ";
	string socialMsg = loc_socialSurvival_part0 + myTmpScore + loc_socialSurvival_part1;
	Social_Facebook(socialMsg.c_str());
#endif
}

void SurvivalMode_TwitterOnClick()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpValue = "Place|Survival";
		s3eFlurryLogEventParams("event_TwitterClicked", tmpValue.c_str());
	}
	//Entire Phrase
	string myTmpScore = " " + totalSurvivalScoreTmp + " ";
	string socialMsg = loc_socialSurvival_part0 + myTmpScore + loc_socialSurvival_part1;
	Social_Twitter(socialMsg.c_str());
#endif
}

#if defined DESKTOPPLATFORM
void Loose_Xbox360_Action()
{
	looseRef->focusManager.ExecuteFocusedObject();
}

void Loose_Xbox360_Up()
{
	looseRef->focusManager.ChangeFocus(Xbox360_Up);
}

void Loose_Xbox360_Down()
{
	looseRef->focusManager.ChangeFocus(Xbox360_Down);
}
#endif

EndingLoose::EndingLoose()
{
	looseRef = this;

	//Background color
	cols[0].Set(0xbfb77b49);   
	cols[1].Set(0xbfb77b49);
	cols[2].Set(0xbfb77b49);
	cols[3].Set(0xbfb77b49);

	//Set font
	textColor = 0xffffffff;
	titlePos = CIwRect(0, 100, currentGameWidth, 30);
	title = loc_ui_defeat;

	//Position and size
	_x = 140;
	_y = 60;
	_width = 1000;
	_height = 600;
	pos[0] = CIwSVec2(_x, _y);
	pos[1] = CIwSVec2(_x, _y + _height);
	pos[2] = CIwSVec2(_x + _width, _y + _height);
	pos[3] = CIwSVec2(_x + _width, _y);

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	int btnTexWidth = TextureHelper::GetDimension(buttonRenderData.state[0].RollOut).x;

	buttonX = (int)((currentGameWidth - btnTexWidth) * 0.5f);

	btnRetry = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, buttonX, 200);
	btnRetry->OnClick(&Loose_OnClick_PlayAgain);
	btnMapSelect = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, buttonX, 300);
	btnMapSelect->OnClick(&Loose_OnClick_MapSelect);
	btnQuitGame = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, buttonX, 400);
	btnQuitGame->OnClick(&Loose_OnClick_QuitGame);

	bIsActiveObj = true;

	defeatRenderData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("graphic_defeat"));
	defeatDimension  = TextureHelper::GetDimension(defeatRenderData);

	defeat_position = CIwSVec2((int)((currentGameWidth - defeatDimension.x) * 0.5f),
		                       (int)((currentGameHeight - defeatDimension.y) * 0.5f));

	background = new Background(0,0,0,170);

	panelWidth = 20 * 40;
	panelHeight = 13 * 40;
	panelX = (int)((currentGameWidth - panelWidth) * 0.5f);
	panelY = (int)((currentGameHeight - panelHeight) * 0.5f);

	int socialButtonsX = panelX + panelWidth - 178;
	int socialButtonsY = panelY + 13 * 40 - 40 - 64;

#if !defined DESKTOPPLATFORM
	facebookButtonRD.numStates = 1;
	TextureData facebookData = TextureHelper::GetTextureNamed("button_social");
	facebookButtonRD.state[0].RollOn = TextureHelper::GetRenderData(facebookData, 2, 2, 1);
	facebookButtonRD.state[0].RollOut = TextureHelper::GetRenderData(facebookData, 2, 2, 1);
	facebookButtonRD.state[0].Pressed = TextureHelper::GetRenderData(facebookData, 2, 2, 3);
	facebookButtonRD.state[0].Inactive = TextureHelper::GetRenderData(facebookData, 2, 2, 1);

	facebookButtonRD.state[0].RollOn.width = facebookButtonRD.state[0].RollOn.height = 64;
	facebookButtonRD.state[0].RollOut.width = facebookButtonRD.state[0].RollOut.height = 64;
	facebookButtonRD.state[0].Pressed.width = facebookButtonRD.state[0].Pressed.height = 64;
	facebookButtonRD.state[0].Inactive.width = facebookButtonRD.state[0].Inactive.height = 64;

	facebookButton = new Button(TextureHelper::GetCIwTexture(facebookButtonRD.state[0].RollOn.atlasName), &facebookButtonRD, socialButtonsX, socialButtonsY);
	facebookButton->OnClick(&SurvivalMode_FacebookOnClick);

	twitterButtonRD.numStates = 1;
	TextureData twitterData = TextureHelper::GetTextureNamed("button_social");
	twitterButtonRD.state[0].RollOn = TextureHelper::GetRenderData(twitterData, 2, 2, 0);
	twitterButtonRD.state[0].RollOut = TextureHelper::GetRenderData(twitterData, 2, 2, 0);
	twitterButtonRD.state[0].Pressed = TextureHelper::GetRenderData(twitterData, 2, 2, 2);
	twitterButtonRD.state[0].Inactive = TextureHelper::GetRenderData(twitterData, 2, 2, 0);

	twitterButtonRD.state[0].RollOn.width = twitterButtonRD.state[0].RollOn.height = 64;
	twitterButtonRD.state[0].RollOut.width = twitterButtonRD.state[0].RollOut.height = 64;
	twitterButtonRD.state[0].Pressed.width = twitterButtonRD.state[0].Pressed.height = 64;
	twitterButtonRD.state[0].Inactive.width = twitterButtonRD.state[0].Inactive.height = 64;

	twitterButton = new Button(TextureHelper::GetCIwTexture(twitterButtonRD.state[0].RollOn.atlasName), &twitterButtonRD, socialButtonsX + 64 + 10, socialButtonsY);
	twitterButton->OnClick(&SurvivalMode_TwitterOnClick);
#endif

	btnRetry->SetText(loc_ui_retry);
	btnMapSelect->SetText(loc_ui_worldMap);
	btnQuitGame->SetText(loc_ui_quitGame);

	bShowDefeatPicture = true;

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Up, &Loose_Xbox360_Up);
	xbox360Controller.SetCallback(Xbox360_Down, &Loose_Xbox360_Down);
	xbox360Controller.SetCallback(Xbox360_Action1, &Loose_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Action2, &Loose_Xbox360_Action);
	xbox360Controller.SetCallback(Xbox360_Action3, &Loose_Xbox360_Action);
#endif

	bShowingiAd = false;
}

EndingLoose::~EndingLoose()
{
	delete btnRetry;
	delete btnMapSelect;
	delete btnQuitGame;
	delete background;

#if !defined DESKTOPPLATFORM
	delete facebookButton;
	delete twitterButton;
#endif
}

void EndingLoose::Update()
{
	if(!bIsActiveObj) return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif

	time_defeat_picture.Update();

	if(time_defeat_picture.GetIsEnd())
	{
		bShowDefeatPicture = false;
	}
}

void EndingLoose::Render()
{
	if(!bIsActiveObj) return;

	if(!bShowDefeatPicture)
	{
		if(gameMode == GAMEMODE_CAMPAIGN)	
		{
			background->Render();

			//Text
			IwGxLightingOn();
			Utils::RenderText(title, CIwRect(titlePos.x - 4, titlePos.y + 4, titlePos.w, titlePos.h), font_komikadisplaybold48, 0xff000000);
			Utils::RenderText(title, titlePos, font_komikadisplaybold48, 0xff00e0f7);
			IwGxLightingOff();

			btnRetry->Render();
			btnMapSelect->Render();
			btnQuitGame->Render();
		}
		else if(gameMode == GAMEMODE_SURVIVAL)
		{
			background->Render();
			
			Utils::RenderPannelA(18, 11, panelX, panelY);

			IwGxLightingOn();
			
			if(bNewRecord)
			{
				Utils::RenderText(loc_ui_congratulations, CIwRect(panelX, panelY + 55, panelWidth, 60), font_komikadisplaybold25, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true);
				Utils::RenderText(strNewRecord, CIwRect(panelX, panelY + 200, panelWidth, 60), font_komikadisplaybold25, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true);
			}
			else
			{
				Utils::RenderText(loc_ui_noRecord, CIwRect(panelX, panelY + 55, panelWidth, 60), font_komikadisplaybold25, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true);
				Utils::RenderText(strNewRecord, CIwRect(panelX, panelY + 170, panelWidth, 60), font_komikadisplaybold25, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true);
				Utils::RenderText(strBestRecord, CIwRect(panelX, panelY + 230, panelWidth, 60), font_komikadisplaybold25, 0xffffffff, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_MIDDLE, true);
			}
			
			IwGxLightingOff();
			
			bool bRenderButtons = true;

#ifdef STEAMREQUIRED
			if(g_SteamLeaderboards != NULL)
			{
				bRenderButtons = !g_SteamLeaderboards->GetUploadingScore();
			}
#endif
			if(bRenderButtons)
			{
				btnRetry->Render();
				btnQuitGame->Render();
			}
#if !defined DESKTOPPLATFORM
			facebookButton->Render();
			twitterButton->Render();
#endif
		}
	}
	else
	{
		background->Render();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(defeatRenderData.atlasName), defeat_position, defeatRenderData);
		if(time_defeat_picture.GetIsStop())
		{
			time_defeat_picture.Reset();
			time_defeat_picture.Start();
		}
	}
}

#ifdef GAMECENTER
static void ReportScoreCallback(s3eIOSGameCenterError* error)
{
	if (*error != S3E_IOSGAMECENTER_ERR_NONE)
		IwTrace(GAMECENTER, ("ReportScoreCallback() called, ERROR"));
	else
		IwTrace(GAMECENTER, ("ReportScoreCallback() called, SUCCESS"));
}
#endif

void EndingLoose::InitObject()
{
	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_loose, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_loose, NULL);

#if !defined DESKTOPPLATFORM
	//ADS
	//if(bLeadbolt)
	//{
	//	loadAd(leadboltAdNumber);
	//}

	//Flurry Analytics
	if(bFlurryOn)
	{
		char charTmp[3];
		sprintf(charTmp, "%d", currentMap);
		string tmpMapString = charTmp;
		string tmpValue = "MapNumber|" + tmpMapString;
		s3eFlurryLogEventParams("event_MapCampaign_Defeat", tmpValue.c_str());
	}

	if(!bIsFullVersion && bChartBoostAvailable)
	{
		s3eChartBoostShowInterstitial("Game Over");
	}
	else if(!bIsFullVersion && biAdAvailable)
	{
		//Show iAd
		s3eIOSIAdStart();
		s3eIOSIAdSetInt(S3E_IOSIAD_BANNER_SHOW, 1);
		bShowingiAd = true;
	}

#endif

	s3eAudioStop();
	if(bIsAudioPaused)
	{
		LastMusicStacked = Music_Loose;
	}
	else
	{
		Utils::PlayBGM("bgm/sd_fanfare_lose_theme.mp3", 1);
		LastMusicStacked = Music_None;
	}

	time_defeat_picture.SetTimeLimit(5000);
	time_defeat_picture.Reset();
	bShowDefeatPicture = true;

	IwGetSoundManager()->StopAll();

	if(gameMode == GAMEMODE_SURVIVAL)
	{
		bNewRecord = survivalModeConfig->NewRecord();
		if(bNewRecord) 
		{
			survivalModeConfig->UpdateRecord();
			int myNewRecord = survivalModeConfig->GetBestRecord();
			string myNewRecordStr = Utils::GetClockFormat(myNewRecord / 1000);
			totalSurvivalScoreTmp = myNewRecordStr;
			sprintf(strNewRecord, "%s %s", loc_ui_newRecord, Utils::GetClockFormatMS(myNewRecord).c_str());

#ifdef STEAMREQUIRED
			g_SteamLeaderboards->UploadScore(myNewRecord);
#endif
#ifdef GAMECENTER
			if(bGameCenterAvailable && bGameCenterAuthenticated)
				s3eIOSGameCenterReportScore(myNewRecord, "survivalsd", ReportScoreCallback);
#endif
			//Achievement nº 8.
			//It's about time: Durar más de 7:29 minutos en modo survival.
			if(myNewRecord >= 449000)
				achievementsHelper->SetAchievementCompletedById(7);
		}
		else
		{
			int myRecord = survivalModeConfig->GetTimePlayedAsInt();
			string myRecordStr = Utils::GetClockFormat(myRecord / 1000);
			totalSurvivalScoreTmp = myRecordStr;
			sprintf(strNewRecord, "%s %s", loc_ui_yourRecord, Utils::GetClockFormatMS(myRecord).c_str());

			int myBestRecord = survivalModeConfig->GetBestRecord();
			string myBestRecordStr = Utils::GetClockFormat(myBestRecord / 1000);
			sprintf(strBestRecord, "%s %s", loc_ui_bestRecord, Utils::GetClockFormatMS(myBestRecord).c_str());

			//Achievement nº 8.
			//It's about time: Durar más de 7:29 minutos en modo survival.
			if(myRecord >= 449000)
				achievementsHelper->SetAchievementCompletedById(7);
		}

		btnRetry->SetPos(panelX + (int)(panelWidth * 0.5f) - (int)(btnRetry->GetWidth() * 1.75f), panelY + panelHeight - (int)(btnRetry->GetHeight() * 0.25f) * 2 - 40);
		btnQuitGame->SetPos(panelX + (int)(panelWidth * 0.5f) - (int)(btnQuitGame->GetWidth() * 1.75f), panelY + panelHeight - (int)(btnQuitGame->GetHeight() * 0.25f) - 35);
	}
	else
	{
		btnRetry->SetPos(buttonX, 200);
		btnMapSelect->SetPos(buttonX, 300);
		btnQuitGame->SetPos(buttonX, 400);
	}

#if defined DESKTOPPLATFORM
	focusManager.SetInitialFocus(btnRetry);
	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		focusManager.SetFocusDirection(Xbox360_Down, btnRetry, btnMapSelect);
		focusManager.SetFocusDirection(Xbox360_Down, btnMapSelect, btnQuitGame);

		focusManager.SetFocusDirection(Xbox360_Up, btnMapSelect, btnRetry);
		focusManager.SetFocusDirection(Xbox360_Up, btnQuitGame, btnMapSelect);
	}
	else if(gameMode == GAMEMODE_SURVIVAL)
	{
		focusManager.SetFocusDirection(Xbox360_Down, btnRetry, btnQuitGame);

		focusManager.SetFocusDirection(Xbox360_Up, btnQuitGame, btnRetry);
	}
#endif
}

void EndingLoose::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_loose);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_loose);
	bShowDefeatPicture = true;

#if !defined DESKTOPPLATFORM
	if(bShowingiAd)
	{
		//Show iAd
		s3eIOSIAdSetInt(S3E_IOSIAD_BANNER_SHOW, 0);
		s3eIOSIAdStop();
		bShowingiAd = false;
	}
#endif
}

void EndingLoose::UpdateClickEvents(s3ePointerEvent *event)
{
	if(popUpState != GSP_None) return;

	if(bShowDefeatPicture) return;

	btnRetry->HandleClick(event);
	
	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		btnMapSelect->HandleClick(event);
	}
	else if(gameMode == GAMEMODE_SURVIVAL)
	{
#if !defined DESKTOPPLATFORM
		facebookButton->HandleClick(event);
		twitterButton->HandleClick(event);
#endif
	}

	btnQuitGame->HandleClick(event);
}

void EndingLoose::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bShowDefeatPicture) return;

	btnRetry->HandleMotion(event);

	if(gameMode == GAMEMODE_CAMPAIGN)
	{
		btnMapSelect->HandleMotion(event);
	}
	else if(gameMode == GAMEMODE_SURVIVAL)
	{
#if !defined DESKTOPPLATFORM
		facebookButton->HandleMotion(event);
		twitterButton->HandleMotion(event);
#endif
	}

	btnQuitGame->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

