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

#include "endingwin.h"
#include "common.h"
#include "localization.h"
#include "parserhelper.h"
#include "savedatahelper.h"
#include "utils.h"
#include "fadescreen.h"
#include "texturehelper.h"
#include "survivalmodeconfig.h"
#if !defined DESKTOPPLATFORM
#include "iOSNativeEDK.h"
#include "s3eKiip.h"
#include "s3eChartBoost.h"
//#include "LeadboltExt.h"
#include "s3eFlurry.h"
#include "s3eIOSIAd.h"
#endif
#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif
#ifdef GAMECENTER
#include "s3eIOSGameCenter.h"
#endif

EndingWin *winRef = NULL;

//Mouse Motion
void HandleMotion_win(s3ePointerMotionEvent* event) 
{
	if(winRef == NULL)
		return;
	
	winRef->UpdateMotionEvents(event);
}

//Mouse Click
void HandleClick_win(s3ePointerEvent *event) 
{
	if(winRef == NULL)
		return;

	winRef->UpdateClickEvents(event);
}

void EndingWin_FacebookOnClick()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpFlurryValue = "Place|Win";
		s3eFlurryLogEventParams("event_FacebookClicked", tmpFlurryValue.c_str());
	}

	//Obtaining Stage
	char stageTmp[3];
	sprintf(stageTmp, "%d", currentMap);
	string tmpMapString = stageTmp;
	//Obtaining Points
	char pointsTmp[6];
	sprintf(pointsTmp, "%d", totalScoreTmp);
	string tmpScoreString = pointsTmp;
	//Entire Phrase
	string myTmpScore = " " + tmpScoreString + " ";
	string myTmpMapString = " " + tmpMapString + " ";
	string socialMsg = loc_socialStage_part0 + myTmpScore + loc_socialStage_part1 + myTmpMapString + loc_socialStage_part2;
	Social_Facebook(socialMsg.c_str());
#endif
}

void EndingWin_TwitterOnClick()
{
#if !defined DESKTOPPLATFORM
	if(bFlurryOn)
	{
		string tmpValue = "Place|Win";
		s3eFlurryLogEventParams("event_TwitterClicked", tmpValue.c_str());
	}
	//Obtaining Stage
	char stageTmp[3];
	sprintf(stageTmp, "%d", currentMap);
	string tmpMapString = stageTmp;
	//Obtaining Points
	char pointsTmp[6];
	sprintf(pointsTmp, "%d", totalScoreTmp);
	string tmpScoreString = pointsTmp;
	//Entire Phrase
	string myTmpScore = " " + tmpScoreString + " ";
	string myTmpMapString = " " + tmpMapString + " ";
	string socialMsg = loc_socialStage_part0 + myTmpScore + loc_socialStage_part1 + myTmpMapString + loc_socialStage_part2;
	Social_Twitter(socialMsg.c_str());
#endif
}

void Win_OnClick_Continue()
{
	winRef->bShowVictoryPicture = true;

	//if(!bIsFullVersion && currentMap == 7)
	//{
	//	gameState = GS_MainMenu;
	//	mainMenuState = GSM_TrialVersion;
	//	inGameState = GSG_None;
	//	bShowTrialVersionFinal = true;
	//	return;
	//}

	//Game complete.
	if(currentMap == 25)
	{
		gameState = GS_MainMenu;
		mainMenuState = GSM_FinalScreen;
		inGameState = GSG_None;
		return;
	}
	
	//Special final.
	if(currentMap == 24)
	{
		dialogController->StartDialog(48);
		inGameState = GSG_None;

		//Achievement nº 13.
		//What happened? : Descubre la auténtica maldad.(pasar etapa 24)
		achievementsHelper->SetAchievementCompletedById(12);
		return;
	}

	if(!bExtraMapMode)
		gameState = GS_MapSelection;
	else
		gameState = GS_ExtraMapSelection;
	inGameState = GSG_None;
}

#if defined DESKTOPPLATFORM
void EndingWin_Xbox360_Action()
{
	winRef->focusManager.ExecuteFocusedObject();
}
#endif

EndingWin::EndingWin()
{
	winRef = this;

	renderPanelNumWidth = 18;
	renderPanelNumHeight = 11;

	if(currentGameHeight <= 640)
	{
		renderPanelNumWidth = 18;
		renderPanelNumHeight = 11;
	}

	panelWidth = (renderPanelNumWidth + 2)  * 40;
	panelHeight = (renderPanelNumHeight + 2)  * 40;
	panelXPos = (int)((currentGameWidth - panelWidth) * 0.5f);
	panelYPos = (int)((currentGameHeight - panelHeight) * 0.5f);
	panelPosition = CIwSVec2(panelXPos, panelYPos);
	
	//Set font
	pfont = font_komikadisplaybold25;

	label_starBig = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_star_big"));

	TextureData tdBackground = TextureHelper::GetTextureNamed("pannel_credits");

	background = new Background(0,0,0, 170);

	//Text properties
	textColor = 0xffffffff;
	int text1_xPos = Utils::GetScaledInt(panelXPos) + 40;
	int text2_xPos = text1_xPos + 420;
	int text1_w   = (int)(panelWidth * 0.5f);

	titlePos = CIwRect(panelXPos, panelYPos + 50, panelWidth, 30);

	int text1_yPos = panelYPos + 120; 

	heartBonusPos  = CIwRect(text1_xPos, text1_yPos, text1_w, 30);
	coinsBonusPos = CIwRect(text1_xPos, text1_yPos + 50, text1_w, 30);
	waveBonusPos = CIwRect(text1_xPos, text1_yPos + 100, text1_w, 30);
	totalPointsPos = CIwRect(text1_xPos, text1_yPos + 150, text1_w, 30);

	heartBonusPos_n  = CIwRect(text2_xPos, text1_yPos, 250, 30);
	coinsBonusPos_n = CIwRect(text2_xPos, text1_yPos + 50, 250, 30);
	waveBonusPos_n = CIwRect(text2_xPos, text1_yPos + 100, 250, 30);
	totalPointsPos_n = CIwRect(text2_xPos, text1_yPos + 150, 250, 30);

	title = loc_ui_victory;

	TextureData data;
	data = TextureHelper::GetTextureNamed("btn_menu");
	buttonRenderData.numStates = 1;
	buttonRenderData.state[0].RollOut = TextureHelper::GetRenderData(data, 1, 4, 0);
	buttonRenderData.state[0].RollOn = TextureHelper::GetRenderData(data, 1, 4, 1);
	buttonRenderData.state[0].Inactive = TextureHelper::GetRenderData(data, 1, 4, 3);
	buttonRenderData.state[0].Pressed = TextureHelper::GetRenderData(data, 1, 4, 2);

	int btnContinueWidth = TextureHelper::GetDimension(buttonRenderData.state[0].RollOut).x;

	int btnContinueOffset = 0;
	if(currentGameHeight <= 640)
	{
		btnContinueOffset = 20;
	}

	btnContinue = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, (int)((currentGameWidth - btnContinueWidth) * 0.5f), panelYPos + 400);
	btnContinue->OnClick(&Win_OnClick_Continue);
	btnContinue->SetText(loc_ui_continue);

	bIsActiveObj = true;

	heartBonus = 0;
	coinsBonus = 0;
	waveBonus = 0;
	lifesOnLevel = 1;

	float w = panelWidth * 0.33f + 55;
	int h = panelYPos + 315;

	star_position[0][0] = CIwSVec2((int16)((panelXPos + w - 40 + 80)), (int16)h);

	star_position[1][0] = CIwSVec2((int16)((panelXPos + w - 40 + 40)), (int16)h);
	star_position[1][1] = CIwSVec2((int16)((panelXPos + w - 40 + 40 + 80)), (int16)h);

	star_position[2][0] = CIwSVec2((int16)((panelXPos + w - 40)), (int16)h);
	star_position[2][1] = CIwSVec2((int16)((panelXPos + w - 40 + 80)), (int16)h);
	star_position[2][2] = CIwSVec2((int16)((panelXPos + w - 40 + 160)), (int16)h);

	num_stars = 0;
	bBlockContinueButton = false;

	heart_counter.SetTextStyle(heartBonusPos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
	coins_counter.SetTextStyle(coinsBonusPos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
	wave_counter.SetTextStyle(waveBonusPos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);
	total_counter.SetTextStyle(totalPointsPos_n, pfont, 0xffffffff, IW_GX_FONT_ALIGN_LEFT);

	heart_counter.SetTime(750);
	coins_counter.SetTime(1000);
	wave_counter.SetTime(1250);
	total_counter.SetTime(1500);

	TextureData VictoryData = TextureHelper::GetTextureNamed("graphic_victory");
	VictoryRenderData = TextureHelper::GetRenderData(VictoryData);
	CIwSVec2 VictoryDimension = TextureHelper::GetDimension(VictoryRenderData);
	VictoryPosition = CIwSVec2((int)((currentGameWidth - VictoryDimension.x) * 0.5f), (int)((currentGameHeight - VictoryDimension.y) * 0.5f));

	int socialButtonsX = panelXPos + panelWidth - 178;
	int socialButtonsY = panelYPos + 13 * 40 - 40 - 64;

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
	facebookButton->OnClick(&EndingWin_FacebookOnClick);

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
	twitterButton->OnClick(&EndingWin_TwitterOnClick);
#endif

	bShowVictoryPicture = true;

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, &Win_OnClick_Continue);
	xbox360Controller.SetCallback(Xbox360_Action2, &Win_OnClick_Continue);
	xbox360Controller.SetCallback(Xbox360_Action3, &Win_OnClick_Continue);
#endif

	bShowingiAd = false;
}

EndingWin::~EndingWin()
{
	delete btnContinue;
	delete background;

#if !defined DESKTOPPLATFORM
	delete facebookButton;
	delete twitterButton;
#endif
}

void EndingWin::Update()
{
	if(!bIsActiveObj)
		return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif

	heart_counter.Update();
	coins_counter.Update();
	wave_counter.Update();
	total_counter.Update();

	star_timer.Update();
	time_victory_picture.Update();
	if(time_victory_picture.GetIsEnd() && bShowVictoryPicture) {
		bShowVictoryPicture = false;
		heart_counter.InitAnimation();
		coins_counter.InitAnimation();
		wave_counter.InitAnimation();
		total_counter.InitAnimation();
	}

	
	if(!bShowVictoryPicture)
	{
		if(total_counter.AnimationEnded())
		{
			bRenderStars = true;
		}

		if(current_stars == num_stars)
		{
			bBlockContinueButton = false;
		}
	}
	
}

void EndingWin::Render()
{
	if(!bIsActiveObj)
		return;

	if(!bShowVictoryPicture)
	{
		background->Render();
		//Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(rdBackground.atlasName), panelPosition, rdBackground);
		Utils::RenderPannelA(18, 11, panelXPos, panelYPos);
		//Text
		IwGxLightingOn();
		Utils::RenderText(title, CIwRect(titlePos.x - 4, titlePos.y + 4, titlePos.w, titlePos.h), font_komikadisplaybold48, 0xff000000);
		Utils::RenderText(title, titlePos, font_komikadisplaybold48, 0xff00e0f7);
		Utils::RenderText(loc_ui_scoreHeartBonus, heartBonusPos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);
		Utils::RenderText(loc_ui_scoreCoinsBonus, coinsBonusPos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);
		Utils::RenderText(loc_ui_scoreWaveBonus, waveBonusPos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);
		Utils::RenderText(loc_ui_scoreTotal, totalPointsPos, pfont, textColor, IW_GX_FONT_ALIGN_RIGHT);

		heart_counter.Render();
		coins_counter.Render();
		wave_counter.Render();
		total_counter.Render();
		
		IwGxLightingOff();

		if(bRenderStars)
		{
			if(star_timer.GetIsStop() && current_stars < num_stars)
			{
				star_timer.Start();
			}

			if(star_timer.GetIsEnd() && current_stars < num_stars)
			{
				//Utils::PlayFXSound(FX_Sound_STARGAINED02);
				Utils::PlaySoundFX(FX_Star_Gained_02);
				current_stars++;
				if(current_stars < num_stars)
				{
					star_timer.Reset();
					star_timer.Start();
				}
			}

			if(current_stars != 0 && num_stars != 0)
			{
				if(current_stars > 3)
					current_stars = 3;

				for(int i = 0; i < current_stars; i++)
				{
					Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(label_starBig.atlasName), star_position[num_stars - 1][i], label_starBig);
				}
			}
		}

		if(!bBlockContinueButton)
		{
			btnContinue->Render();

#if !defined DESKTOPPLATFORM
			facebookButton->Render();
			twitterButton->Render();
#endif
		}
	}
	else
	{
		background->Render();
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(VictoryRenderData.atlasName), VictoryPosition, VictoryRenderData);
		if(time_victory_picture.GetIsStop())
		{
			time_victory_picture.Reset();
			time_victory_picture.Start();
		}
	}
}

void EndingWin::InitObject()
{
	bShowTutorialLevel1 = false;
	bBlockContinueButton = true;
	bIsActiveObj = true;
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_win, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_win, NULL);
	
#if !defined DESKTOPPLATFORM
	//ADS
	//if(bLeadbolt)
	//{
	//	if(adOnWinLevel == 2)
	//	{
	//		adOnWinLevel = 0;
	//		loadAd(leadboltAdNumber);
	//	}
	//	else
	//	{
	//		adOnWinLevel++;
	//	}
	//}

	if(!bIsFullVersion && bKiipAvailable)
	{
		s3eKiipSaveMoment("kiip_mission_complete");
	}
	else if(!bIsFullVersion && bChartBoostAvailable)
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

	//Flurry Analytics
	if(bFlurryOn)
	{
		char charTmp[3];
		sprintf(charTmp, "%d", currentMap);
		string tmpMapString = charTmp;
		string tmpValue = "MapNumber|" + tmpMapString;
		s3eFlurryLogEventParams("event_MapCampaign_Win", tmpValue.c_str());
	}
#endif

	s3eAudioStop();
	if(bIsAudioPaused)
	{
		LastMusicStacked = Music_Win;
	}
	else
	{
		Utils::PlayBGM("bgm/sd_fanfare_win_theme.mp3", 1);
		LastMusicStacked = Music_None;
	}

	bShowVictoryPicture = true;
	time_victory_picture.Reset();
	time_victory_picture.Stop();
	time_victory_picture.SetTimeLimit(5500);

	heartBonus = lifesOnLevel * 1000;

	waveBonus *= 10;

	if(!bExtraMapMode)
	{
		if(MapStatus_State[currentMap - 1] == CURRENT_PLAYABLE)
		{
			MapStatus_State[currentMap - 1] = PLAYABLE;
			MapStatus_State[currentMap] = CURRENT_PLAYABLE;
		}

		if(lifesOnLevel == 4)
		{
			if(Stars_By_Level[currentMap - 1] < 3)
				Stars_By_Level[currentMap - 1] = 3;
			num_stars = 3;
		}
		else if(lifesOnLevel == 3)
		{
			if(Stars_By_Level[currentMap - 1] < 2)
				Stars_By_Level[currentMap - 1] = 2;
			num_stars = 2;
		}
		else if(lifesOnLevel == 2)
		{
			if(Stars_By_Level[currentMap - 1] < 1)
				Stars_By_Level[currentMap - 1] = 1;
			num_stars = 1;
		}
		else if(lifesOnLevel == 1)
		{
			num_stars = 0;
		}

		//You get three stars in the first level, no matter what.
		if(currentMap == 1)
		{
			Stars_By_Level[0] = 3;
			num_stars = 3;
		}
	}
	else
	{
		if(lifesOnLevel == 4)
		{
			if(Stars_By_ExtraLevel[currentMap - 1] < 3)
				Stars_By_ExtraLevel[currentMap - 1] = 3;
			num_stars = 3;
		}
		else if(lifesOnLevel == 3)
		{
			if(Stars_By_ExtraLevel[currentMap - 1] < 2)
				Stars_By_ExtraLevel[currentMap - 1] = 2;
			num_stars = 2;
		}
		else if(lifesOnLevel == 2)
		{
			if(Stars_By_ExtraLevel[currentMap - 1] < 1)
				Stars_By_ExtraLevel[currentMap - 1] = 1;
			num_stars = 1;
		}
		else if(lifesOnLevel == 1)
		{
			num_stars = 0;
		}
	}

	current_stars = 0;
	bRenderStars = false;

	star_timer.SetTimeLimit(100);
	star_timer.Reset();
	star_timer.Stop();

	heart_counter.SetValue(heartBonus);
	coins_counter.SetValue(coinsBonus);
	wave_counter.SetValue(waveBonus);
	totalScoreTmp = coinsBonus + heartBonus + waveBonus;
	total_counter.SetValue(coinsBonus + heartBonus + waveBonus);

	//set best score by level if neccesary
	int tmpMap = currentMap - 1;
	if(tmpMap >= 0 && tmpMap < 24)
	{
		if(totalScoreTmp > bestScoreByLevel[tmpMap])
		{
			IwTrace(MASK, ("EndingWin::InitObject() , Set Best Score %i on map %i", totalScoreTmp, tmpMap));
			bestScoreByLevel[tmpMap] = totalScoreTmp;

			PerformWriteBestScoreOnLB(currentMap, totalScoreTmp);
		}
	}
	

	IwGetSoundManager()->StopAll();

	//Achievement nº 1 First Approach
	CheckAchievement_FirstApproach();

#if defined DESKTOPPLATFORM
	focusManager.SetInitialFocus(btnContinue);
	focusManager.SetFocusDirection(XBOX360_Button_Directional, btnContinue, btnContinue);
#endif
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

//Calcula el puntaje acumulado hasta la ultima etapa que se jugo y se obtuvo puntaje.
void EndingWin::PerformWriteBestScoreOnLB(int myLevel, int currentScore)
{
	int myAccumulatedScore = 0;
	int myMaxLevelReached = 0;
	LBCampaignScore myLBScore;
	int myRawScore = 0;

	for (int i = 0; i < MAX_MAPS_CAMPAIGN; i++)
	{
		if(bestScoreByLevel[i] != 0)
		{
			myMaxLevelReached++;
		}
		else
			break;
	}
	//IwTrace(MASK, ("PerformWriteBestScoreOnLB() ,myMaxLevelReached %i", myMaxLevelReached));
	if(myMaxLevelReached > 24)
		myMaxLevelReached = 24;

	for (int i = 0; i < myMaxLevelReached; i++)
	{
		myAccumulatedScore = myAccumulatedScore + bestScoreByLevel[i];
		//IwTrace(MASK, ("PerformWriteBestScoreOnLB() ,iteration bestScoreByLevel[%i] = %i , accum = %i", i, bestScoreByLevel[i], myAccumulatedScore));
	}
	//IwTrace(MASK, ("PerformWriteBestScoreOnLB() ,myAccumulatedScore %i at to level %i", myAccumulatedScore, myMaxLevelReached));
	myLBScore.level = myMaxLevelReached;
	myLBScore.totalScore = myAccumulatedScore;
	myRawScore = Utils::MaskLeadeboardScore(myLBScore);

#ifdef STEAMREQUIRED
	g_SteamLeaderboards->UploadScore(myRawScore);
#endif

#ifdef GAMECENTER
	if(bGameCenterAvailable && bGameCenterAuthenticated)
		s3eIOSGameCenterReportScore(myLBScore.totalScore, "campaignsd", ReportScoreCallback);
#endif
}

//Achievement nº 1.
//3 primeras etapas con todas las estrellas
void EndingWin::CheckAchievement_FirstApproach()
{
	if(Stars_By_Level[0]==3 
		&& Stars_By_Level[1]==3 
		&& Stars_By_Level[2]==3)
	{
		IwTrace(ACHIEVEMENT, ("CheckAchievementFirstApproach() called, Achievement nº 1 GAINED"));
		achievementsHelper->SetAchievementCompletedById(0);
	}
}

void EndingWin::EndObject()
{
	bIsActiveObj = false;
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotion_win);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClick_win);

	if(saveDataHelper)
		saveDataHelper->SaveGameDataCampaign();

	if(currentMap == 18)
	{
		survivalModeConfig->bIsSurvivalAvailable = true;
		if(saveDataHelper)
			saveDataHelper->SaveGameDataSurvival();
	}
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

void EndingWin::UpdateClickEvents(s3ePointerEvent *event)
{
	if(bShowVictoryPicture)
		return;

	if(!bBlockContinueButton)
	{
		btnContinue->HandleClick(event);

#if !defined DESKTOPPLATFORM
		facebookButton->HandleClick(event);
		twitterButton->HandleClick(event);
#endif
	}
}

void EndingWin::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(bShowVictoryPicture)
		return;

	if(!bBlockContinueButton)
	{
		btnContinue->HandleMotion(event);

#if !defined DESKTOPPLATFORM
		facebookButton->HandleMotion(event);
		twitterButton->HandleMotion(event);
#endif
	}

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

