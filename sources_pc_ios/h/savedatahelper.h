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

#ifndef _SAVEDATAHELPER_H
#define _SAVEDATAHELPER_H

#include "common.h"
#include "utils.h"
#ifdef STEAMREQUIRED
#include "steam/ISteamRemoteStorage.h"
#endif

class CGame;

class SaveDataHelper
{
public:
	SaveDataHelper();
	~SaveDataHelper();

#ifdef STEAMREQUIRED
	// Steam RemoteStorage interface
	ISteamRemoteStorage *m_pSteamRemoteStorage;
	void RetrieveSteamRemoteStorageRef();
	void RemoveSteamRemoteStorageRef();
#endif

	void CheckGameDataFlag();
	void SaveGameDataFlag(int versionToSave);

	void LoadGameDataCampaign();
	void SaveGameDataCampaign();
	void ResetGameDataCampaign();

	void GetGameDataCampaignResult();
	void SetGameDataCampaignResult();

#ifdef STEAMREQUIRED
	void CheckSteamCloudGameDataFlag();
	void SaveSteamCloudGameDataFlag(int versionToSave);

	void LoadSteamCloudGameDataCampaign();
	void SaveSteamCloudGameDataCampaign();
	void ResetSteamCloudGameDataCampaign();
#endif

	//Survival Data
	void LoadGameDataSurvival();
	void SaveGameDataSurvival();
	void ResetGameDataSurvival();

	void GetGameDataSurvivalResult();
	void SetGameDataSurvivalResult();

#ifdef STEAMREQUIRED
	void LoadSteamCloudGameDataSurvival();
	void SaveSteamCloudGameDataSurvival();
	void ResetSteamCloudGameDataSurvival();
#endif

	void LoadGameLocalData();
	void SaveGameLocalData();
	void ResetGameLocalData();

	void GetGameLocalDataResult();
	void SetGameLocalDataResult();
};

#endif