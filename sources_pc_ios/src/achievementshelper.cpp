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

#include "achievementshelper.h"
#include "common.h"
#ifdef STEAMREQUIRED
#include "helpers/steam_achievements.h"
#endif
#ifdef GAMECENTER
#include "s3eIOSGameCenter.h"
#endif

AchievementsHelper::AchievementsHelper()
{
#ifdef STEAMREQUIRED
	g_SteamAchievementsRef = NULL;
#endif
}

AchievementsHelper::~AchievementsHelper()
{

}

void AchievementsHelper::ResetAllAchievements()
{
#ifdef STEAMREQUIRED
	if(g_SteamAchievementsRef == NULL)
		return;
	g_SteamAchievementsRef->ResetAchievements();
#endif
}

#ifdef GAMECENTER
static void ReportAchievementCallback(s3eIOSGameCenterError* error)
{
	if (*error != S3E_IOSGAMECENTER_ERR_NONE)
		IwTrace(GAMECENTER, ("ReportAchievementCallback() called, ERROR"));
	else
		IwTrace(GAMECENTER, ("ReportAchievementCallback() called, SUCCESS"));
}
#endif

void AchievementsHelper::SetAchievementCompletedById(int achievmenetId)
{
#ifdef GAMECENTER
	if(bGameCenterAvailable && bGameCenterAuthenticated)
	{
		const char *myAchievementName = "";
		
		if(achievmenetId == 0){myAchievementName = "achievement_001";}
		else if(achievmenetId == 1){myAchievementName = "achievement_002";}
		else if(achievmenetId == 2){myAchievementName = "achievement_003";}
		else if(achievmenetId == 3){myAchievementName = "achievement_004";}
		else if(achievmenetId == 4){myAchievementName = "achievement_005";}
		else if(achievmenetId == 5){myAchievementName = "achievement_006";}
		else if(achievmenetId == 6){myAchievementName = "achievement_007";}
		else if(achievmenetId == 7){myAchievementName = "achievement_008";}
		else if(achievmenetId == 8){myAchievementName = "achievement_009";}
		else if(achievmenetId == 9){myAchievementName = "achievement_010";}
		else if(achievmenetId == 10){myAchievementName = "achievement_011";}
		else if(achievmenetId == 11){myAchievementName = "achievement_012";}
		else if(achievmenetId == 12){myAchievementName = "achievement_013";}
		else if(achievmenetId == 13){myAchievementName = "achievement_014";}
		else if(achievmenetId == 14){myAchievementName = "achievement_015";}

		if(strcmp(myAchievementName, "") != 0)
			s3eIOSGameCenterReportAchievement(myAchievementName, 100, ReportAchievementCallback);
	}
#endif
#ifdef STEAMREQUIRED
	if(g_SteamAchievementsRef == NULL)
		return;
	if(achievmenetId == 0){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_00");}
	else if(achievmenetId == 1){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_01");}
	else if(achievmenetId == 2){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_02");}
	else if(achievmenetId == 3){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_03");}
	else if(achievmenetId == 4){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_04");}
	else if(achievmenetId == 5){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_05");}
	else if(achievmenetId == 6){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_06");}
	else if(achievmenetId == 7){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_07");}
	else if(achievmenetId == 8){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_08");}
	else if(achievmenetId == 9){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_09");}
	else if(achievmenetId == 10){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_10");}
	else if(achievmenetId == 11){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_11");}
	else if(achievmenetId == 12){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_12");}
	else if(achievmenetId == 13){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_13");}
	else if(achievmenetId == 14){g_SteamAchievementsRef->SetAchievement("SD_ACHIEVEMENT_14");}
#endif
}