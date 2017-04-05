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

#include "game.h"
#include "steam_achievements.h"

CSteamAchievements::CSteamAchievements(Achievement_t *Achievements, int NumAchievements): 				
	m_CallbackUserStatsReceived( this, &CSteamAchievements::OnUserStatsReceived ),
	m_CallbackUserStatsStored( this, &CSteamAchievements::OnUserStatsStored ),
	m_CallbackAchievementStored( this, &CSteamAchievements::OnAchievementStored )
{
	m_iAppID = SteamUtils()->GetAppID();
	m_RequestStatReceived = false;
	m_pAchievements = Achievements;
	m_iNumAchievements = NumAchievements;

	IwTrace(STEAMACHIEVEMENTS, ("CSteamAchievements() called, NumAchievements %i , Then RequestStats()", NumAchievements));

	RequestStats();
}

CSteamAchievements::~CSteamAchievements()
{

}

void CSteamAchievements::ResetAchievements()
{
	SteamUserStats()->ResetAllStats(true);
}

bool CSteamAchievements::RequestStats()
{
	IwTrace(STEAMACHIEVEMENTS, ("RequestStats() called, NumAchievements step 1"));
	// Is Steam loaded? If not we can't get stats.
	if ( NULL == SteamUserStats() || NULL == SteamUser() )
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if ( !SteamUser()->BLoggedOn() )
	{
		return false;
	}
	// Request user stats.
	IwTrace(STEAMACHIEVEMENTS, ("RequestStats() called, NumAchievements step 2, all ok, then RequestCurrentStats()"));
	return SteamUserStats()->RequestCurrentStats();
}

bool CSteamAchievements::RequestGlobalStats()
{
	IwTrace(GAMESTATS, ("RequestGlobalStats() called,"));
	// Is Steam loaded? If not we can't get stats.
	if ( NULL == SteamUserStats() || NULL == SteamUser() )
	{
		return false;
	}
	// Is the user logged on?  If not we can't get stats.
	if ( !SteamUser()->BLoggedOn() )
	{
		return false;
	}
	// Request user stats.
	IwTrace(GAMESTATS, ("RequestGlobalStats() called, RequestCurrentStats()"));
	SteamAPICall_t hSteamAPICall = SteamUserStats()->RequestGlobalStats(30);
	m_callResultRequestGlobalStats.Set(hSteamAPICall, this, &CSteamAchievements::OnRequestGlobalStats);
	return true;
}

void CSteamAchievements::OnRequestGlobalStats( GlobalStatsReceived_t *pCallback, bool bIOFailure )
{
	// see if we encountered an error during the call
	if ( bIOFailure )
	{
		IwTrace(GAMESTATS, ("OnRequestGlobalStats could not be found"));
		return;
	}
	IwTrace(GAMESTATS, ("OnRequestGlobalStats() called, Success"));
	IwTrace(GAMESTATS, ("OnRequestGlobalStats, m_nGameID %i , m_eResult %i", pCallback->m_nGameID, pCallback->m_eResult));

	int64 totalLoggedIn = 0;
	SteamUserStats()->GetGlobalStat("event_UserLoggedIn", &totalLoggedIn);
	IwTrace(GAMESTATS, ("GameStats, Total Users Logged In : %i", totalLoggedIn));
}

bool CSteamAchievements::SetAchievement(const char* ID)
{
	IwTrace(STEAMACHIEVEMENTS, ("SetAchievement() called, by ID: %s m_RequestStatReceived Value : %i", ID, m_RequestStatReceived));

	// Have we received a call back from Steam yet?
	if (m_RequestStatReceived == true)
	{
		IwTrace(STEAMACHIEVEMENTS, ("SetAchievement() called, by ID: %s success", ID));
		SteamUserStats()->SetAchievement(ID);
		bool bResultStoreStats = false;
		bResultStoreStats = SteamUserStats()->StoreStats();
		return bResultStoreStats;
	}
	// If not then we can't set achievements yet
	return false;
}

void CSteamAchievements::OnUserStatsReceived( UserStatsReceived_t *pCallback )
{
	// IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsReceived() called, Callback received app Id %i", pCallback->m_nGameID));
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )
	{
		if ( k_EResultOK == pCallback->m_eResult )
		{
			IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsReceived() called, Received stats and achievements from Steam success"));
			//OutputDebugString("Received stats and achievements from Steam\n");
			m_RequestStatReceived = true;
			IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsReceived() called, m_RequestStatReceived Value : %i", m_RequestStatReceived));

			// load achievements
			for ( int iAch = 0; iAch < m_iNumAchievements; ++iAch )
			{
				Achievement_t &ach = m_pAchievements[iAch];

				SteamUserStats()->GetAchievement(ach.m_pchAchievementID, &ach.m_bAchieved);
				IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsReceived() called, achievement name : %i %s", iAch, SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "name")));
				IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsReceived() called, achievement descripcion :%i %s", iAch, SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "desc")));
				//_snprintf( ach.m_rgchName, sizeof(ach.m_rgchName), "%s", SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "name"));
				//_snprintf( ach.m_rgchDescription, sizeof(ach.m_rgchDescription), "%s", SteamUserStats()->GetAchievementDisplayAttribute(ach.m_pchAchievementID, "desc"));			
			}
		}
		else
		{
			//char buffer[128];
			//_snprintf( buffer, 128, "RequestStats - failed, %d\n", pCallback->m_eResult );
			//OutputDebugString( buffer );
			IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsReceived() called, Received stats and achievements from Steam failed"));
		}
	}
}

void CSteamAchievements::PrintStatsReceived()
{
	// load stats
	int64 totalLoggedIn = 0;
	SteamUserStats()->GetGlobalStat("event_UserLoggedIn", &totalLoggedIn);
	IwTrace(GAMESTATS, ("GameStats, Total Users Logged In : %i", totalLoggedIn));
}

void CSteamAchievements::OnUserStatsStored( UserStatsStored_t *pCallback )
{
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )	
	{
		if ( k_EResultOK == pCallback->m_eResult )
		{
			IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsStored() called, Stored stats for Steam success"));
			//OutputDebugString( "Stored stats for Steam\n" );
		}
		else
		{
			//char buffer[128];
			//_snprintf( buffer, 128, "StatsStored - failed, %d\n", pCallback->m_eResult );
			//OutputDebugString( buffer );
			IwTrace(STEAMACHIEVEMENTS, ("OnUserStatsStored() called, Stored stats for Steam, failed"));
		}
	}
}

void CSteamAchievements::OnAchievementStored( UserAchievementStored_t *pCallback )
{
	// we may get callbacks for other games' stats arriving, ignore them
	if ( m_iAppID == pCallback->m_nGameID )	
	{
		IwTrace(STEAMACHIEVEMENTS, ("OnAchievementStored() called, Stored Achievement for Steam"));
		//OutputDebugString( "Stored Achievement for Steam\n" );
	}
}


void CSteamAchievements::StoreGlobalStats_SteamID(const char* steamEvent)
{
	//Set Stat
	SteamUserStats()->SetStat(steamEvent, 1);
	bool bSuccess = SteamUserStats()->StoreStats();
	IwTrace(GAMESTATS, ("Store GameStats, %s, result %i", steamEvent, bSuccess));
}