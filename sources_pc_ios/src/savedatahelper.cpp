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

#include "common.h"
#include "savedatahelper.h"
#include "stdio.h"
#include "iostream.h"
#include "survivalmodeconfig.h"
#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "steam/ISteamRemoteStorage.h"
#endif
using namespace std;

SaveDataHelper::SaveDataHelper()
{

}

SaveDataHelper::~SaveDataHelper()
{

}

#ifdef STEAMREQUIRED
void SaveDataHelper::RetrieveSteamRemoteStorageRef()
{
	m_pSteamRemoteStorage = SteamRemoteStorage();
}
void SaveDataHelper::RemoveSteamRemoteStorageRef()
{
	m_pSteamRemoteStorage = NULL;
}
#endif


void SaveDataHelper::CheckGameDataFlag()
{
	IwTrace(SAVEDATA, ("CheckGameDataFlag(), called"));
#ifdef STEAMREQUIRED
	CheckSteamCloudGameDataFlag();
	return;
#endif

	int _auxSaveDataVersion = 0;

	if(!s3eFileCheckExists("data_gameflag.sdm"))
	{
		ResetGameDataCampaign();
		ResetGameDataSurvival();
		SaveGameDataFlag(saveDataVersion);
		return;
	}

	s3eFile* file = s3eFileOpen("data_gameflag.sdm", "rb");
	if(file == NULL)
	{
		ResetGameDataCampaign();
		ResetGameDataSurvival();
		SaveGameDataFlag(saveDataVersion);
		return;
	}

	s3eFileRead(&_auxSaveDataVersion, sizeof(_auxSaveDataVersion), 1, file);
	s3eFileClose(file);
	if(saveDataVersion!=_auxSaveDataVersion)
	{
		IwTrace(SAVEDATA, ("NEED TO RESET ALL DATA, savedata version mismatch, saveDataVersion %i != _auxSaveDataVersion %i", saveDataVersion, _auxSaveDataVersion));
		ResetGameDataCampaign();
		ResetGameDataSurvival();
		SaveGameDataFlag(saveDataVersion);
	}
	else
	{
		IwTrace(SAVEDATA, ("NO NEED TO RESET DATA saveDataVersion %i != _auxSaveDataVersion %i", saveDataVersion, _auxSaveDataVersion));
	}
}

void SaveDataHelper::SaveGameDataFlag(int versionToSave)
{
	IwTrace(SAVEDATA, ("SaveGameDataFlag()"));
	//SteamCloud
#ifdef STEAMREQUIRED
	SaveSteamCloudGameDataFlag(versionToSave);
	return;
#endif

	if(s3eFileCheckExists("data_gameflag.sdm"))
		s3eFileDelete("data_gameflag.sdm");

	s3eFile* file = s3eFileOpen("data_gameflag.sdm", "w+b");

	s3eFileWrite(&versionToSave, sizeof(versionToSave), 1, file);
	s3eFileClose(file);
}

#ifdef STEAMREQUIRED
void SaveDataHelper::CheckSteamCloudGameDataFlag()
{
	IwTrace(SAVEDATA, ("CheckSteamCloudGameDataFlag(), called"));
	int _auxSaveDataVersion = 0;

	int32 steamCloudLoadResultBytes = 0;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;

	if(!m_pSteamRemoteStorage)
		return;

	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("CheckSteamCloudGameDataFlag() called, steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));

	if(!m_pSteamRemoteStorage->FileExists("selknam_gameflag.sdm"))
	{
		IwTrace(STEAMCLOUD, ("NEED TO RESET ALL DATA, savedata error 1"));
		ResetGameDataCampaign();
		ResetGameDataSurvival();
		SaveGameDataFlag(saveDataVersion);
		return;
	}

	steamCloudLoadResultBytes = m_pSteamRemoteStorage->FileRead("selknam_gameflag.sdm", &_auxSaveDataVersion, sizeof(_auxSaveDataVersion));
	IwTrace(STEAMCLOUD, ("CheckSteamCloudGameDataFlag() called, steamCloudLoadResultBytes %i", steamCloudLoadResultBytes));
	if(steamCloudLoadResultBytes==0)
	{
		IwTrace(STEAMCLOUD, ("NEED TO RESET ALL DATA, savedata error 2"));
		ResetGameDataCampaign();
		ResetGameDataSurvival();
		SaveGameDataFlag(saveDataVersion);
		return;
	}
	IwTrace(STEAMCLOUD, ("CheckSteamCloudGameDataFlag() called, ALL OK"));
	if(saveDataVersion!=_auxSaveDataVersion)
	{
		IwTrace(STEAMCLOUD, ("NEED TO RESET ALL DATA, savedata version mismatch, saveDataVersion %i != _auxSaveDataVersion %i", saveDataVersion, _auxSaveDataVersion));
		ResetGameDataCampaign();
		ResetGameDataSurvival();
		SaveGameDataFlag(saveDataVersion);
	}
	else
	{
		IwTrace(STEAMCLOUD, ("NO NEED TO RESET DATA saveDataVersion %i != _auxSaveDataVersion %i", saveDataVersion, _auxSaveDataVersion));
	}
}

void SaveDataHelper::SaveSteamCloudGameDataFlag(int versionToSave)
{
	IwTrace(SAVEDATA, ("SaveSteamCloudGameDataFlag(), called"));
	if(!m_pSteamRemoteStorage)
		return;
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataFlag() called, STEP 1"));
	bool bSteamCloudSaveResult = false;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;

	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataFlag() called, BEFORE steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
	bSteamCloudSaveResult = m_pSteamRemoteStorage->FileWrite("selknam_gameflag.sdm", &versionToSave, sizeof(versionToSave));
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataFlag() called, bSteamCloudSaveResult %i", bSteamCloudSaveResult));
	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataFlag() called, AFTER steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
}
#endif

void SaveDataHelper::LoadGameDataCampaign()
{
	IwTrace(STEAMCLOUD, ("LoadGameDataCampaign()"));
//SteamCloud
#ifdef STEAMREQUIRED
	LoadSteamCloudGameDataCampaign();
	return;
#endif
	
	GameDataCampaign _auxGameDataCampaign;
	//IwTrace(PERSISTENTDATA, ("LoadGameDataCampaign() called, STEP 1"));
	if(!s3eFileCheckExists("data_campaign.sdm"))
	{
		ResetGameDataCampaign();
		return;
	}

	s3eFile* file = s3eFileOpen("data_campaign.sdm", "rb");
	if(file == NULL)
	{
		ResetGameDataCampaign();
		return;
	}

	//IwTrace(PERSISTENTDATA, ("LoadGameDataCampaign() called"));

	s3eFileRead(&_auxGameDataCampaign, sizeof(_auxGameDataCampaign), 1, file);
	gameDataCampaign = _auxGameDataCampaign;
	s3eFileClose(file);
	GetGameDataCampaignResult();
}

void SaveDataHelper::SaveGameDataCampaign()
{
	IwTrace(STEAMCLOUD, ("SaveGameDataCampaign()"));
//SteamCloud
#ifdef STEAMREQUIRED
	SaveSteamCloudGameDataCampaign();
	return;
#endif

	//IwTrace(PERSISTENTDATA, ("SavePersistentData() called"));
	SetGameDataCampaignResult();
	
	if(s3eFileCheckExists("data_campaign.sdm"))
		s3eFileDelete("data_campaign.sdm");

	s3eFile* file = s3eFileOpen("data_campaign.sdm", "w+b");

	s3eFileWrite(&gameDataCampaign, sizeof(gameDataCampaign), 1, file);
	s3eFileClose(file);
}

void SaveDataHelper::ResetGameDataCampaign()
{
	IwTrace(STEAMCLOUD, ("ResetGameDataCampaign()"));
#ifdef STEAMREQUIRED
	ResetSteamCloudGameDataCampaign();
	return;
#endif
	//IwTrace(PERSISTENTDATA, ("ResetGameData() called"));
	gameDataCampaign = GameDataCampaign();
	//SteamCloud
	if(s3eFileCheckExists("data_campaign.sdm"))
		s3eFileDelete("data_campaign.sdm");

	s3eFile* file = s3eFileOpen("data_campaign.sdm", "w+b");

	s3eFileWrite(&gameDataCampaign, sizeof(gameDataCampaign), 1, file);
	s3eFileClose(file);

	GetGameDataCampaignResult();
};

#ifdef STEAMREQUIRED
void SaveDataHelper::LoadSteamCloudGameDataCampaign()
{
	GameDataCampaign _auxGameDataCampaign;

	int32 steamCloudLoadResultBytes = 0;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;

	if(!m_pSteamRemoteStorage)
		return;

	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("LoadSteamCloudGameDataCampaign() called, steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));

	if(!m_pSteamRemoteStorage->FileExists("selknam_campaign.sdm"))
	{
		ResetGameDataCampaign();
		return;
	}

	steamCloudLoadResultBytes = m_pSteamRemoteStorage->FileRead("selknam_campaign.sdm", &_auxGameDataCampaign, sizeof(_auxGameDataCampaign));
	IwTrace(STEAMCLOUD, ("LoadSteamCloudGameDataCampaign() called, steamCloudLoadResultBytes %i", steamCloudLoadResultBytes));
	if(steamCloudLoadResultBytes==0)
	{
		ResetGameDataCampaign();
		return;
	}
	IwTrace(STEAMCLOUD, ("LoadPersistentData() called, ALL OK"));
	gameDataCampaign = _auxGameDataCampaign;
	GetGameDataCampaignResult();
}

void SaveDataHelper::SaveSteamCloudGameDataCampaign()
{
	SetGameDataCampaignResult();

	if(!m_pSteamRemoteStorage)
		return;
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataCampaign() called, STEP 1"));
	bool bSteamCloudSaveResult = false;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;

	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataCampaign() called, BEFORE steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
	bSteamCloudSaveResult = m_pSteamRemoteStorage->FileWrite("selknam_campaign.sdm", &gameDataCampaign, sizeof(gameDataCampaign));
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataCampaign() called, bSteamCloudSaveResult %i", bSteamCloudSaveResult));
	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataCampaign() called, AFTER steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
}

void SaveDataHelper::ResetSteamCloudGameDataCampaign()
{
	gameDataCampaign = GameDataCampaign();

	if(!m_pSteamRemoteStorage)
		return;

	bool bSteamCloudSaveResult = false;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;
	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("ResetSteamCloudGameDataCampaign() called, BEFORE steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
	bSteamCloudSaveResult = m_pSteamRemoteStorage->FileWrite("selknam_campaign.sdm", &gameDataCampaign, sizeof(gameDataCampaign));
	IwTrace(STEAMCLOUD, ("ResetSteamCloudGameDataCampaign() called, bSteamCloudSaveResult %i", bSteamCloudSaveResult));
	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("ResetSteamCloudGameDataCampaign() called, AFTER steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));

	GetGameDataCampaignResult();
}
#endif

//From file to internal data
void SaveDataHelper::GetGameDataCampaignResult()
{
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult()"));

	NewGame_State = (NewGame)gameDataCampaign.newGame;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult(), NewGame_State %i", NewGame_State)); //o empty, 1 no empty

	for(int i = 0; i < MAX_MAPS; i++)
	{
		MapStatus_State[i] = (MapStatus)gameDataCampaign.campaignMapsData[i];
		IwTrace(SAVEDATA, ("GetGameDataCampaignResult() MapStatus_State[%i], %i", i, gameDataCampaign.campaignMapsData[i]));
	}

	for(int i = 0; i < MAX_MAPS; i++)
	{
		Stars_By_Level[i] = gameDataCampaign.starsByLevelData[i];
		IwTrace(SAVEDATA, ("GetGameDataCampaignResult() starsByLevelData[%i], %i", i, gameDataCampaign.starsByLevelData[i]));
	}

	ArcherBaseUpgrades.level = gameDataCampaign.archerBaseUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() archerBaseUpgradesData %i", gameDataCampaign.archerBaseUpgradesData));
	ArcherPoisonerUpgrades.level = gameDataCampaign.archerPoisonerUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() archerPoisonerUpgradesData %i", gameDataCampaign.archerPoisonerUpgradesData));
	ArcherRetarderUpgrades.level = gameDataCampaign.archerRetarderUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() archerRetarderUpgradesData %i", gameDataCampaign.archerRetarderUpgradesData));
	WarriorBaseUpgrades.level = gameDataCampaign.warriorBaseUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() warriorBaseUpgradesData %i", gameDataCampaign.warriorBaseUpgradesData));
	WarriorTankUpgrades.level = gameDataCampaign.warriorTankUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() warriorTankUpgradesData %i", gameDataCampaign.warriorTankUpgradesData));
	WarriorBerserkUpgrades.level = gameDataCampaign.warriorBerserkUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() warriorBerserkUpgradesData %i", gameDataCampaign.warriorBerserkUpgradesData));
	ShamanHealerUpgrades.level = gameDataCampaign.shamanHealerUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() shamanHealerUpgradesData %i", gameDataCampaign.shamanHealerUpgradesData));
	ShamanMightUpgrades.level = gameDataCampaign.shamanMightUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() shamanMightUpgradesData %i", gameDataCampaign.shamanMightUpgradesData));
	ShamanHasteUpgrades.level = gameDataCampaign.shamanHasteUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() shamanHasteUpgradesData %i", gameDataCampaign.shamanHasteUpgradesData));
	BowlerStunerUpgrades.level = gameDataCampaign.bowlerStunerUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bowlerStunerUpgradesData %i", gameDataCampaign.bowlerStunerUpgradesData));
	BowlerRooterUpgrades.level = gameDataCampaign.bowlerRooterUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bowlerRooterUpgradesData %i", gameDataCampaign.bowlerRooterUpgradesData));
	BowlerConfuserUpgrades.level = gameDataCampaign.bowlerConfuserUpgradesData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bowlerConfuserUpgradesData %i", gameDataCampaign.bowlerConfuserUpgradesData));

	Used_Stars = gameDataCampaign.usedStarsData;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() usedStarsData %i", gameDataCampaign.usedStarsData));

	for(int i = 0; i < MAX_EXTRA_MAPS; i++)
	{
		Stars_By_ExtraLevel[i] = gameDataCampaign.starsByExtraMapsData[i];
	}

	bFirstMapWasPlayed = gameDataCampaign.bFirstMapWasPlayed;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bFirstMapWasPlayed %i", gameDataCampaign.bFirstMapWasPlayed));
	bShowTutorialLevel1 = gameDataCampaign.bShowTutorialLevel1;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bShowTutorialLevel1 %i", gameDataCampaign.bShowTutorialLevel1));
	bShowTutorialStars = gameDataCampaign.bShowTutorialStars;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bShowTutorialStars %i", gameDataCampaign.bShowTutorialStars));
	bShowTutorialUnlockStore = gameDataCampaign.bShowTutorialUnlockStore;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bshowTutorialUnlockStore %i", gameDataCampaign.bShowTutorialUnlockStore));

	bShowTutorialSurvivalModeUnlock = gameDataCampaign.bShowTutorialSurvivalModeUnlock;
	IwTrace(SAVEDATA, ("GetGameDataCampaignResult() bFirstMapWasPlayed %i", gameDataCampaign.bShowTutorialSurvivalModeUnlock));

	for(int i = 0; i < MAX_ENEMIES; i++)
	{
		isEnemyUnlocked[i] = gameDataCampaign.enemiesUnlocked[i];
		IwTrace(SAVEDATA, ("GetGameDataCampaignResult() enemiesUnlocked[%i], %i", i, gameDataCampaign.enemiesUnlocked[i]));
	}

	for(int i = 0; i < MAX_MAPS; i++)
	{
		bestScoreByLevel[i] = gameDataCampaign.bestScoreByLevel[i];
		IwTrace(SAVEDATA, ("GetGameDataCampaignResult(), bestScoreByLevel[%i]: %i", i, gameDataCampaign.bestScoreByLevel[i]));
	}
}

//From internal data to file
void SaveDataHelper::SetGameDataCampaignResult()
{
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult()"));

	gameDataCampaign.newGame = (int)NewGame_State;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), NewGame_State %i", NewGame_State)); //o empty, 1 no empty

	for(int i = 0; i < MAX_MAPS; i++)
	{
		gameDataCampaign.campaignMapsData[i] = (int)MapStatus_State[i];
		IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), MapStatus_State[%i]: %i", i, MapStatus_State[i]));
	}
	for(int i = 0; i < MAX_MAPS; i++)
	{
		gameDataCampaign.starsByLevelData[i] = Stars_By_Level[i];
		IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), Stars_By_Level[%i]: %i", i,  Stars_By_Level[i]));
	}

	gameDataCampaign.archerBaseUpgradesData = ArcherBaseUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), ArcherBaseUpgrades.level %i", ArcherBaseUpgrades.level));
	gameDataCampaign.archerPoisonerUpgradesData  = ArcherPoisonerUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), ArcherPoisonerUpgrades.level %i", ArcherPoisonerUpgrades.level));
	gameDataCampaign.archerRetarderUpgradesData = ArcherRetarderUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), ArcherRetarderUpgrades.level %i", ArcherRetarderUpgrades.level));
	gameDataCampaign.warriorBaseUpgradesData = WarriorBaseUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), WarriorBaseUpgrades.level %i", WarriorBaseUpgrades.level));
	gameDataCampaign.warriorTankUpgradesData = WarriorTankUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), WarriorTankUpgrades.level %i", WarriorTankUpgrades.level));
	gameDataCampaign.warriorBerserkUpgradesData = WarriorBerserkUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), WarriorBerserkUpgrades.level %i", WarriorBerserkUpgrades.level));
	gameDataCampaign.shamanHealerUpgradesData = ShamanHealerUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), ShamanHealerUpgrades.level %i", ShamanHealerUpgrades.level));
	gameDataCampaign.shamanMightUpgradesData = ShamanMightUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), ShamanMightUpgrades.level %i", ShamanMightUpgrades.level));
	gameDataCampaign.shamanHasteUpgradesData = ShamanHasteUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), ShamanHasteUpgrades.level %i", ShamanHasteUpgrades.level));
	gameDataCampaign.bowlerStunerUpgradesData = BowlerStunerUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), BowlerStunerUpgrades.level %i", BowlerStunerUpgrades.level));
	gameDataCampaign.bowlerRooterUpgradesData = BowlerRooterUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), BowlerRooterUpgrades.level %i", BowlerRooterUpgrades.level));
	gameDataCampaign.bowlerConfuserUpgradesData = BowlerConfuserUpgrades.level;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), BowlerConfuserUpgrades.level %i", BowlerConfuserUpgrades.level));

	gameDataCampaign.usedStarsData = Used_Stars;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), Used_Stars %i", Used_Stars));

	for(int i = 0; i < MAX_EXTRA_MAPS; i++)
	{
		gameDataCampaign.starsByExtraMapsData[i] = Stars_By_ExtraLevel[i];
	}

	gameDataCampaign.bFirstMapWasPlayed = bFirstMapWasPlayed;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), bFirstMapWasPlayed %i", bFirstMapWasPlayed));
	gameDataCampaign.bShowTutorialLevel1 = bShowTutorialLevel1;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), bShowTutorialLevel1 %i", bShowTutorialLevel1));
	gameDataCampaign.bShowTutorialStars = bShowTutorialStars;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), bShowTutorialStars %i", bShowTutorialStars));
	gameDataCampaign.bShowTutorialUnlockStore = bShowTutorialUnlockStore;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), bShowTutorialUnlockStore %i", bShowTutorialUnlockStore));
	gameDataCampaign.bShowTutorialSurvivalModeUnlock = bShowTutorialSurvivalModeUnlock;
	IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), bShowTutorialSurvivalModeUnlock %i", bShowTutorialSurvivalModeUnlock));

	for(int i = 0; i < MAX_ENEMIES; i++)
	{
		gameDataCampaign.enemiesUnlocked[i] = isEnemyUnlocked[i];
		IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), isEnemyUnlocked[%i]: %i", i, isEnemyUnlocked[i]));
	}

	for(int i = 0; i < MAX_MAPS; i++)
	{
		gameDataCampaign.bestScoreByLevel[i] = bestScoreByLevel[i];
		IwTrace(SAVEDATA, ("SetGameDataCampaignResult(), bestScoreByLevel[%i]: %i", i, bestScoreByLevel[i]));
	}
}


void SaveDataHelper::LoadGameDataSurvival()
{
	//SteamCloud
#ifdef STEAMREQUIRED
	LoadSteamCloudGameDataSurvival();
	return;
#endif

	GameDataSurvival _auxGameDataSurvival;
	if(!s3eFileCheckExists("data_survival.sdm"))
	{
		ResetGameDataSurvival();
		return;
	}

	s3eFile* file = s3eFileOpen("data_survival.sdm", "rb");
	if(file == NULL)
	{
		ResetGameDataSurvival();
		return;
	}

	s3eFileRead(&_auxGameDataSurvival, sizeof(_auxGameDataSurvival), 1, file);
	gameDataSurvival = _auxGameDataSurvival;
	s3eFileClose(file);
	GetGameDataSurvivalResult();
}

void SaveDataHelper::SaveGameDataSurvival()
{
	//SteamCloud
#ifdef STEAMREQUIRED
	SaveSteamCloudGameDataSurvival();
	return;
#endif

	SetGameDataSurvivalResult();

	if(s3eFileCheckExists("data_survival.sdm"))
		s3eFileDelete("data_survival.sdm");

	s3eFile* file = s3eFileOpen("data_survival.sdm", "w+b");

	s3eFileWrite(&gameDataSurvival, sizeof(gameDataSurvival), 1, file);
	s3eFileClose(file);
}

void SaveDataHelper::ResetGameDataSurvival()
{
#ifdef STEAMREQUIRED
	ResetSteamCloudGameDataSurvival();
	return;
#endif
	gameDataSurvival = GameDataSurvival();
	//SteamCloud
	if(s3eFileCheckExists("data_survival.sdm"))
		s3eFileDelete("data_survival.sdm");

	s3eFile* file = s3eFileOpen("data_survival.sdm", "w+b");

	s3eFileWrite(&gameDataSurvival, sizeof(gameDataSurvival), 1, file);
	s3eFileClose(file);

	GetGameDataSurvivalResult();
};


#ifdef STEAMREQUIRED
void SaveDataHelper::LoadSteamCloudGameDataSurvival()
{
	GameDataSurvival _auxGameDataSurvival;

	int32 steamCloudLoadResultBytes = 0;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;

	if(!m_pSteamRemoteStorage)
		return;

	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("LoadSteamCloudGameDataSurvival() called, steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));

	if(!m_pSteamRemoteStorage->FileExists("selknam_survival.sdm"))
	{
		ResetGameDataSurvival();
		return;
	}

	steamCloudLoadResultBytes = m_pSteamRemoteStorage->FileRead("selknam_survival.sdm", &_auxGameDataSurvival, sizeof(_auxGameDataSurvival));
	IwTrace(STEAMCLOUD, ("LoadSteamCloudGameDataSurvival() called, steamCloudLoadResultBytes %i", steamCloudLoadResultBytes));
	if(steamCloudLoadResultBytes==0)
	{
		ResetGameDataSurvival();
		return;
	}
	IwTrace(STEAMCLOUD, ("LoadPersistentData() called, ALL OK"));
	gameDataSurvival = _auxGameDataSurvival;
	GetGameDataSurvivalResult();
}

void SaveDataHelper::SaveSteamCloudGameDataSurvival()
{
	SetGameDataSurvivalResult();

	if(!m_pSteamRemoteStorage)
		return;
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataSurvival() called, STEP 1"));
	bool bSteamCloudSaveResult = false;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;

	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataSurvival() called, BEFORE steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
	bSteamCloudSaveResult = m_pSteamRemoteStorage->FileWrite("selknam_survival.sdm", &gameDataSurvival, sizeof(gameDataSurvival));
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataSurvival() called, bSteamCloudSaveResult %i", bSteamCloudSaveResult));
	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("SaveSteamCloudGameDataSurvival() called, AFTER steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
}

void SaveDataHelper::ResetSteamCloudGameDataSurvival()
{
	gameDataSurvival = GameDataSurvival();

	if(!m_pSteamRemoteStorage)
		return;

	bool bSteamCloudSaveResult = false;
	int32 steamTotalQuotaByte = 0;
	int32 steamRemainQuotaByte = 0;
	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("ResetSteamCloudGameDataSurvival() called, BEFORE steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));
	bSteamCloudSaveResult = m_pSteamRemoteStorage->FileWrite("selknam_survival.sdm", &gameDataSurvival, sizeof(gameDataSurvival));
	IwTrace(STEAMCLOUD, ("ResetSteamCloudGameDataSurvival() called, bSteamCloudSaveResult %i", bSteamCloudSaveResult));
	m_pSteamRemoteStorage->GetQuota(&steamTotalQuotaByte, &steamRemainQuotaByte);
	IwTrace(STEAMCLOUD, ("ResetSteamCloudGameDataSurvival() called, AFTER steamTotalQuotaByte %i , steamRemainQuotaByte %i", steamTotalQuotaByte, steamRemainQuotaByte));

	GetGameDataSurvivalResult();
}
#endif

//From file to internal data
void SaveDataHelper::GetGameDataSurvivalResult()
{
	survivalModeConfig->bIsSurvivalAvailable = gameDataSurvival.bSurvivalAvailable;
	survivalModeConfig->SetBestRecord(gameDataSurvival.survivalBestRecord);
}

//From internal data to file
void SaveDataHelper::SetGameDataSurvivalResult()
{
	gameDataSurvival.bSurvivalAvailable = survivalModeConfig->bIsSurvivalAvailable;
	gameDataSurvival.survivalBestRecord = survivalModeConfig->GetBestRecord();
}



GameDataCampaign::GameDataCampaign()
{
	newGame = EMPTY_GAME;

	campaignMapsData[0] = CURRENT_PLAYABLE;
	for(int i = 1; i < MAX_MAPS; i++)
		campaignMapsData[i] = NOT_PLAYABLE;

	for(int i = 0; i < MAX_MAPS; i++)
		starsByLevelData[i] = 0;

	archerBaseUpgradesData = 1;
	archerPoisonerUpgradesData = 0;
	archerRetarderUpgradesData = 0;

	warriorBaseUpgradesData = 1;
	warriorTankUpgradesData = 0;
	warriorBerserkUpgradesData = 0;

	shamanHealerUpgradesData = 1;
	shamanMightUpgradesData = 0;
	shamanHasteUpgradesData = 0;

	bowlerStunerUpgradesData = 1;
	bowlerRooterUpgradesData = 0;
	bowlerConfuserUpgradesData = 0;

	usedStarsData = 0;

	for(int i = 0; i < MAX_EXTRA_MAPS; i++)
		starsByExtraMapsData[i] = 0;

	//tutorial section
	bFirstMapWasPlayed = false;
	bShowTutorialLevel1 = true;
	bShowTutorialStars = true;
	bShowTutorialUnlockStore = true;
	bShowTutorialSurvivalModeUnlock = true;

	for(int i = 0; i < MAX_ENEMIES; i++)
	{
		enemiesUnlocked[i] = false;
	}

	//best Score by Level
	for(int i = 0; i < MAX_MAPS; i++)
		bestScoreByLevel[i] = 0;
}

GameDataCampaign::~GameDataCampaign()
{

}

void GameDataCampaign::SetGameDataCampaignFullProgress()
{
	newGame = NOT_EMPTY_GAME;

	campaignMapsData[0] = CURRENT_PLAYABLE;
	for(int i = 0; i < MAX_MAPS; i++)
		campaignMapsData[i] = PLAYABLE;

	for(int i = 0; i < MAX_MAPS; i++)
		starsByLevelData[i] = 3;

	archerBaseUpgradesData = 3;
	archerPoisonerUpgradesData = 3;
	archerRetarderUpgradesData = 3;

	warriorBaseUpgradesData = 3;
	warriorTankUpgradesData = 3;
	warriorBerserkUpgradesData = 3;

	shamanHealerUpgradesData = 3;
	shamanMightUpgradesData = 3;
	shamanHasteUpgradesData = 3;

	bowlerStunerUpgradesData = 3;
	bowlerRooterUpgradesData = 3;
	bowlerConfuserUpgradesData = 3;

	usedStarsData = 72;

	for(int i = 0; i < MAX_EXTRA_MAPS; i++)
		starsByExtraMapsData[i] = 3;

	//tutorial section
	bFirstMapWasPlayed = true;
	bShowTutorialLevel1 = false;
	bShowTutorialStars = false;
	bShowTutorialUnlockStore = false;
	bShowTutorialSurvivalModeUnlock = false;

	for(int i = 0; i < MAX_ENEMIES; i++)
	{
		enemiesUnlocked[i] = true;
	}

	if(saveDataHelper)
		saveDataHelper->GetGameDataCampaignResult();
}

//Survival classes
GameDataSurvival::GameDataSurvival()
{
	bSurvivalAvailable = false;
	survivalBestRecord = 0;
}

GameDataSurvival::~GameDataSurvival()
{

}

void GameDataSurvival::SetGameDataSurvivalAvailable()
{
	bSurvivalAvailable = true;
	survivalBestRecord = 0;
	if(saveDataHelper)
		saveDataHelper->GetGameDataSurvivalResult();
}


//Local Data
void SaveDataHelper::LoadGameLocalData()
{
	GameLocalData _gameLocalData;
	if(!s3eFileCheckExists("data_local.sdm"))
	{
		ResetGameLocalData();
		return;
	}

	s3eFile* file = s3eFileOpen("data_local.sdm", "rb");
	if(file == NULL)
	{
		ResetGameLocalData();
		return;
	}

	s3eFileRead(&_gameLocalData, sizeof(_gameLocalData), 1, file);
	gameLocalData = _gameLocalData;
	s3eFileClose(file);
	GetGameLocalDataResult();
}

void SaveDataHelper::SaveGameLocalData()
{
	SetGameLocalDataResult();

	if(s3eFileCheckExists("data_local.sdm"))
		s3eFileDelete("data_local.sdm");

	s3eFile* file = s3eFileOpen("data_local.sdm", "w+b");

	s3eFileWrite(&gameLocalData, sizeof(gameLocalData), 1, file);
	s3eFileClose(file);
}

void SaveDataHelper::ResetGameLocalData()
{
	gameLocalData = GameLocalData();
	//SteamCloud
	if(s3eFileCheckExists("data_local.sdm"))
		s3eFileDelete("data_local.sdm");

	s3eFile* file = s3eFileOpen("data_local.sdm", "w+b");

	s3eFileWrite(&gameLocalData, sizeof(gameLocalData), 1, file);
	s3eFileClose(file);

	GetGameLocalDataResult();
};

//From file to internal data
void SaveDataHelper::GetGameLocalDataResult()
{
	currentAudioVolume = gameLocalData.audioVolume;
	currentSoundVolume = gameLocalData.soundVolume;
	bIsFullScreenOption = gameLocalData.bIsFullScreenOption;
	bAllMuted = gameLocalData.extraBool_01;
	lastAudioVolume = gameLocalData.extraInt_01;
	lastSoundVolume = gameLocalData.extraInt_02;

	bIsFullVersion = gameLocalData.extraBool_02;
}

//From internal data to file
void SaveDataHelper::SetGameLocalDataResult()
{
	gameLocalData.audioVolume = currentAudioVolume;
	gameLocalData.soundVolume = currentSoundVolume;
	gameLocalData.bIsFullScreenOption = bIsFullScreenOption;
	gameLocalData.extraBool_01 = bAllMuted;
	gameLocalData.extraInt_01 = lastAudioVolume;
	gameLocalData.extraInt_02 = lastSoundVolume;

	gameLocalData.extraBool_02 = bIsFullVersion;
}

//Survival classes
GameLocalData::GameLocalData()
{
	audioVolume = 160;
	soundVolume = 190;
	bIsFullScreenOption = true;

	//extraInt_01: lastAudioVolume
	extraInt_01 = 160;
	//extraInt_02: lastSoundVolume
	extraInt_02 = 190;
	extraInt_03 = 0;
	extraInt_04 = 0;
	extraInt_05 = 0;
	extraInt_06 = 0;
	extraInt_07 = 0;
	extraInt_08 = 0;
	extraInt_09 = 0;
	extraInt_10 = 0;

	//extra bool 01 : Mute All
	extraBool_01 = false;
	//extra bool 02 : bIsFull version
	extraBool_02 = false;

	extraBool_03 = false;
	extraBool_04 = false;
	extraBool_05 = false;
	extraBool_06 = false;
	extraBool_07 = false;
	extraBool_08 = false;
	extraBool_09 = false;
	extraBool_10 = false;

	extraFloat_01 = 0.0f;
	extraFloat_02 = 0.0f;
	extraFloat_03 = 0.0f;
	extraFloat_04 = 0.0f;
	extraFloat_05 = 0.0f;
	extraFloat_06 = 0.0f;
	extraFloat_07 = 0.0f;
	extraFloat_08 = 0.0f;
	extraFloat_09 = 0.0f;
	extraFloat_10 = 0.0f;
}

GameLocalData::~GameLocalData()
{

}