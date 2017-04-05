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
#include "steam_leaderboards.h"

CSteamLeaderboards::CSteamLeaderboards(): 				
m_CurrentLeaderboard( NULL ),
	m_nLeaderboardEntries( 0 )
{
	bUploadingScore = false;
	bDownloadingScore = false;
	numAttemptsIfFails = MAX_ATTEMPTS;

	FindLeaderboard("Campaign_sd_");
}

CSteamLeaderboards::~CSteamLeaderboards()
{
}

void CSteamLeaderboards::FindLeaderboard( const char *pchLeaderboardName )
{
	m_CurrentLeaderboard = NULL;

	SteamAPICall_t hSteamAPICall = SteamUserStats()->FindLeaderboard(pchLeaderboardName);
	m_callResultFindLeaderboard.Set(hSteamAPICall, this, &CSteamLeaderboards::OnFindLeaderboard);
}

void CSteamLeaderboards::OnFindLeaderboard( LeaderboardFindResult_t *pCallback,	bool bIOFailure )
{
	// see if we encountered an error during the call
	if ( !pCallback->m_bLeaderboardFound || bIOFailure )
	{
		IwTrace(STEAMLEADERBOARDS, ("Leaderboard could not be found"));
		return;
	}
	IwTrace(STEAMLEADERBOARDS, ("OnFindLeaderboard() called, Success"));
	m_CurrentLeaderboard = pCallback->m_hSteamLeaderboard;
}

bool CSteamLeaderboards::UploadScore( int score )
{
	if (!m_CurrentLeaderboard)
		return false;

	if(!bUploadingScore)
	{
		bUploadingScore = true;
		numAttemptsIfFails = MAX_ATTEMPTS;
	}
	else
	{
		numAttemptsIfFails--;
	}

	IwTrace(STEAMLEADERBOARDS, ("UploadScore() called, score %i", score));

	SteamAPICall_t hSteamAPICall = SteamUserStats()->UploadLeaderboardScore( m_CurrentLeaderboard, k_ELeaderboardUploadScoreMethodKeepBest, score, NULL, 0 );

	m_callResultUploadScore.Set(hSteamAPICall, this, &CSteamLeaderboards::OnUploadScore);

	return true;
}

void CSteamLeaderboards::OnUploadScore(LeaderboardScoreUploaded_t *pCallback, bool bIOFailure)
{
	if ( !pCallback->m_bSuccess || bIOFailure )
	{
		IwTrace(STEAMLEADERBOARDS, ("Score could not be uploaded to Steam"));
		if(numAttemptsIfFails <= 0)
		{
			bUploadingScore = false;
		}
		else
		{
			UploadScore(pCallback->m_nScore);
		}
	}
	else
	{
		bUploadingScore = false;
		IwTrace(STEAMLEADERBOARDS, ("Score uploaded to Steam"));
	}
}

bool CSteamLeaderboards::LeaderboardEnabled() 
{ 
	if(m_CurrentLeaderboard) 
		return true;
	else
		return false;
}

bool CSteamLeaderboards::DownloadScores(int type, int rangeStart, int rangeFinal)
{
	if (!m_CurrentLeaderboard)
		return false;

	if(!bDownloadingScore)
	{
		bDownloadingScore = true;
		numAttemptsIfFails = MAX_ATTEMPTS;
		scoreType = type;
		scoreRangeStart = rangeStart;
		scoreRangeFinal = rangeFinal;
	}
	else
	{
		numAttemptsIfFails--;
	}
	IwTrace(LEADERBOARDS, ("TYPE: %d", type));
	ELeaderboardDataRequest myDataRequest = (ELeaderboardDataRequest)type;

	// load the specified leaderboard data around the current user
	SteamAPICall_t hSteamAPICall;
	hSteamAPICall = SteamUserStats()->DownloadLeaderboardEntries(m_CurrentLeaderboard, myDataRequest, rangeStart, rangeFinal);
	m_callResultDownloadScore.Set(hSteamAPICall, this, &CSteamLeaderboards::OnDownloadScore);

	return true;
}

void CSteamLeaderboards::OnDownloadScore(LeaderboardScoresDownloaded_t *pCallback, bool bIOFailure)
{
	if (!bIOFailure)
	{
		m_nLeaderboardEntries = min(pCallback->m_cEntryCount, 10);

		for (int index = 0; index < m_nLeaderboardEntries; index++)
		{
			SteamUserStats()->GetDownloadedLeaderboardEntry( pCallback->m_hSteamLeaderboardEntries, index, &m_leaderboardEntries[index], NULL, 0);
		}

		bDownloadingScore = false;
	}
	else
	{
		if(numAttemptsIfFails > 0)
		{
			DownloadScores(scoreType, scoreRangeStart, scoreRangeFinal);
		}
	}
}