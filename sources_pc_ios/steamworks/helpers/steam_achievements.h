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

#ifndef _STEAM_ACHIEVEMENTS_H
#define _STEAM_ACHIEVEMENTS_H

#include "common.h"
#include "steam/steam_api.h"


class CSteamAchievements
{
private:
	int64 m_iAppID; // Our current AppID
	Achievement_t *m_pAchievements; // Achievements data
	int m_iNumAchievements; // The number of Achievements
	bool m_RequestStatReceived; // Have we called Request stats and received the callback?

public:
	CSteamAchievements(Achievement_t *Achievements, int NumAchievements);
	~CSteamAchievements();

	bool RequestStats();

	bool RequestGlobalStats();
	void OnRequestGlobalStats( GlobalStatsReceived_t *pCallback, bool bIOFailure);
	CCallResult<CSteamAchievements, GlobalStatsReceived_t> m_callResultRequestGlobalStats;

	bool SetAchievement(const char* ID);
	void StoreGlobalStats_SteamID(const char* steamEvent);
	void StoreStats_TimePlayed(int myAverageTime);
	void PrintStatsReceived();

	STEAM_CALLBACK( CSteamAchievements, OnUserStatsReceived, UserStatsReceived_t, m_CallbackUserStatsReceived );
	STEAM_CALLBACK( CSteamAchievements, OnUserStatsStored, UserStatsStored_t, m_CallbackUserStatsStored );
	STEAM_CALLBACK( CSteamAchievements, OnAchievementStored, UserAchievementStored_t, m_CallbackAchievementStored );

	void ResetAchievements();
};

#endif