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

#include "lboardentry.h"
#include "common.h"
#include "utils.h"
#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif

LboardEntry::LboardEntry(string playerName, int score, int rank, int entryID)
{
	this->playerName = playerName;
	this->score = score;
	this->rank = rank;
	this->entryID = entryID;
	bIsFocused = false;
	x = y = w = h = 0;
}

LboardEntry::~LboardEntry()
{

}

void LboardEntry::EnableFocus()
{
	bIsFocused = true;
}

void LboardEntry::UnableFocus()
{
	bIsFocused = false;
}

bool LboardEntry::HandleFocusMotion(s3ePointerMotionEvent* event)
{
	return Utils::MouseOn(event->m_x, event->m_y, x, y, w, h);
}

void LboardEntry::ExecuteFunction()
{
#ifdef STEAMREQUIRED
	if(g_SteamLeaderboards != NULL)
	{
		if(g_SteamLeaderboards->m_nLeaderboardEntries > entryID && entryID != -1)
		{
			IwTrace(XBOX360Debug, ("%d", entryID));
			SteamFriends()->ActivateGameOverlayToUser("steamid", g_SteamLeaderboards->m_leaderboardEntries[entryID].m_steamIDUser);
		}
	}
#endif
}

