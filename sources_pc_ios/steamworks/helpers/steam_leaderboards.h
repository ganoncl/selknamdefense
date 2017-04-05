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

#ifndef _STEAM_LEADERBOARDS_H
#define _STEAM_LEADERBOARDS_H

#include "common.h"
#include "steam/steam_api.h"

#define MAX_ATTEMPTS 5

class CSteamLeaderboards
{
private:
	SteamLeaderboard_t m_CurrentLeaderboard; // Handle to leaderboard

	bool bUploadingScore;
	bool bDownloadingScore;
	int numAttemptsIfFails;
	int scoreType, scoreRangeStart, scoreRangeFinal;

public:
	int m_nLeaderboardEntries; // How many entries do we have?
	LeaderboardEntry_t m_leaderboardEntries[11]; // The entries

	CSteamLeaderboards();
	~CSteamLeaderboards();

	//Tells if there is a leaderboard loaded, if this function returns false then
	//UploadScore and DownloadScores won't work.
	bool LeaderboardEnabled();

	void FindLeaderboard( const char *pchLeaderboardName );
	bool UploadScore( int score );
	bool DownloadScores(int type, int rangeStart, int rangeFinal);

	void OnFindLeaderboard( LeaderboardFindResult_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardFindResult_t> m_callResultFindLeaderboard;
	//CCallResult m_callResultFindLeaderboard;
	void OnUploadScore( LeaderboardScoreUploaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoreUploaded_t> m_callResultUploadScore;
	//CCallResult m_callResultUploadScore;
	void OnDownloadScore( LeaderboardScoresDownloaded_t *pResult, bool bIOFailure);
	CCallResult<CSteamLeaderboards, LeaderboardScoresDownloaded_t> m_callResultDownloadScore;
	//CCallResult m_callResultDownloadScore;

	bool GetUploadingScore() { return bUploadingScore; };
	bool GetDownloadingScore() { return bDownloadingScore; };
};

#endif