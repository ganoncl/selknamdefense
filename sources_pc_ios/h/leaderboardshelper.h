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

#ifndef _LEADERBOARDSHELPER_H
#define _LEADERBOARDSHELPER_H

#ifdef STEAMREQUIRED
class CSteamLeaderboards;
#endif

class LeaderboardsHelper
{
public:
	LeaderboardsHelper();
	~LeaderboardsHelper();
#ifdef STEAMREQUIRED
	void RetrieveSteamLeaderboardsInterfaceRef(CSteamLeaderboards * _g_SteamLeaderboardsRef) {g_SteamLeaderboardsRef = _g_SteamLeaderboardsRef;}
#endif
public:
#ifdef STEAMREQUIRED
	CSteamLeaderboards *g_SteamLeaderboardsRef;
#endif

};

#endif