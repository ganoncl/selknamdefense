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

#ifndef _LOCALIZATION_H_
#define _LOCALIZATION_H_

#include "IwGxFont.h"
#include "SimpleIni.h"

extern CSimpleIniCaseA simpleIniApi;

extern CIwGxFont* font_komikadisplaybold25;
extern CIwGxFont* font_komikadisplay11;
extern CIwGxFont* font_komikadisplay28;
extern CIwGxFont* font_komikadisplay14;
extern CIwGxFont* font_komikadisplay18;
extern CIwGxFont* font_komikadisplaybold18;
extern CIwGxFont* font_komikadisplay22;
extern CIwGxFont* font_komikadisplaybold22;
extern CIwGxFont* font_komikadisplaybold48;
extern CIwGxFont* font_SmartKid12;
extern CIwGxFont* font_SmartKid16;
extern CIwGxFont* font_SmartKid18;
extern CIwGxFont* font_SmartKid20;
extern CIwGxFont* font_SmartKid22;
extern CIwGxFont* font_SmartKid24;
extern CIwGxFont* font_EpsonJap18;

extern const char* loc_splash_loading;
extern const char* loc_splash_version;
extern const char* loc_splash_fullVersion;
extern const char* loc_splash_freeVersion;
extern const char* loc_ui_back;
extern const char* loc_ui_close;
extern const char* loc_ui_ok;
extern const char* loc_ui_accept;
extern const char* loc_ui_cancel;
extern const char* loc_ui_next;
extern const char* loc_ui_skip;
extern const char* loc_ui_finish;
extern const char* loc_ui_playGame;
extern const char* loc_ui_removeAds;
extern const char* loc_ui_newCampaign;
extern const char* loc_ui_playCampaign;
extern const char* loc_ui_almanac;
extern const char* loc_ui_options;
extern const char* loc_ui_restore;
extern const char* loc_ui_settings;
extern const char* loc_ui_credits;
extern const char* loc_ui_survival;
extern const char* loc_ui_quitGame;
extern const char* loc_ui_exitGame;
extern const char* loc_ui_pause;
extern const char* loc_ui_resume;
extern const char* loc_ui_howToPlay;
extern const char* loc_ui_retry;
extern const char* loc_ui_worldMap;
extern const char* loc_ui_playAgain;
extern const char* loc_ui_continue;
extern const char* loc_ui_upgrades;
extern const char* loc_ui_selknams;
extern const char* loc_ui_enemies;
extern const char* loc_ui_time;
extern const char* loc_ui_wave;
extern const char* loc_ui_congratulations;
extern const char* loc_ui_newRecord;
extern const char* loc_ui_yourRecord;
extern const char* loc_ui_bestRecord;
extern const char* loc_ui_noRecord;
extern const char* loc_ui_victory;
extern const char* loc_ui_defeat;
extern const char* loc_ui_scoreHeartBonus;
extern const char* loc_ui_scoreCoinsBonus;
extern const char* loc_ui_scoreWaveBonus;
extern const char* loc_ui_scoreTotal;
extern const char* loc_ui_onlyFullVersion;
extern const char* loc_ui_leaderboards;
extern const char* loc_ui_gamecenter;
extern const char* loc_ui_music;
extern const char* loc_ui_fxSound;
extern const char* loc_ui_fullscreen;
extern const char* loc_ui_muteAll;

//popups
extern const char* loc_popup_savedData;
extern const char* loc_popup_quitGame;
extern const char* loc_popup_noFeature;
extern const char* loc_popup_retry;
extern const char* loc_popup_returnToMenu;
extern const char* loc_popup_returnToWorldMap;
extern const char* loc_popup_upgrades;
extern const char* loc_popup_survival;
extern const char* loc_popup_trial;
extern const char* loc_popup_thanks;
extern const char* loc_popup_error;
extern const char* loc_popup_leaderboarderror;
extern const char* loc_popup_PopupInternetConnection;

//Selknam Upgrade
extern const char* loc_upgrade_selknamUpgrade;
extern const char* loc_portrait_archerBase;
extern const char* loc_portrait_archerSlower;
extern const char* loc_portrait_archerPoisoner;
extern const char* loc_portrait_bowlerStunner;
extern const char* loc_portrait_bowlerConfuser;
extern const char* loc_portrait_bowlerRooter;
extern const char* loc_portrait_warriorBase;
extern const char* loc_portrait_warriorBerserk;
extern const char* loc_portrait_warriorTank;
extern const char* loc_portrait_shamanHealer;
extern const char* loc_portrait_shamanSpeeder;
extern const char* loc_portrait_shamanMighter;
extern const char* loc_upgrade_attributes;
extern const char* loc_upgrade_health;
extern const char* loc_upgrade_damage;
extern const char* loc_upgrade_speed;
extern const char* loc_upgrade_attackSpeed;
extern const char* loc_upgrade_castSpeed;
extern const char* loc_upgrade_range;
extern const char* loc_upgrade_cost;
extern const char* loc_upgrade_skill;
extern const char* loc_upgrade_skillSlow;
extern const char* loc_upgrade_skillPoison;
extern const char* loc_upgrade_skillStun;
extern const char* loc_upgrade_skillConfusion;
extern const char* loc_upgrade_skillRoot;
extern const char* loc_upgrade_skillHeal;
extern const char* loc_upgrade_skillHaste;
extern const char* loc_upgrade_skillMight;

extern const char* loc_description_skillSlow;
extern const char* loc_description_skillPoison;
extern const char* loc_description_skillStun;
extern const char* loc_description_skillConfusion;
extern const char* loc_description_skillRoot;
extern const char* loc_description_skillHeal;
extern const char* loc_description_skillHaste;
extern const char* loc_description_skillMight;

//Leaderboars
extern const char* loc_leaderboards_title;
extern const char* loc_leaderboards_rank;
extern const char* loc_leaderboards_playername;
extern const char* loc_leaderboards_score;
extern const char* loc_leaderboards_time;
extern const char* loc_leaderboards_prevpage;
extern const char* loc_leaderboards_nextpage;
extern const char* loc_leaderboards_loading;
extern const char* loc_leaderboards_overall;
extern const char* loc_leaderboards_friends;
extern const char* loc_leaderboards_myrank;
extern const char* loc_leaderboards_stage;
extern const char* loc_leaderboards_survival;
extern const char* loc_leaderboards_campaign;
extern const char* loc_leaderboards_notAvailable;

//Selknam Almanac
extern const char* loc_almanac_baseArcherTitle;
extern const char* loc_almanac_baseArcherInformation;
extern const char* loc_almanac_poisonArcherTitle;
extern const char* loc_almanac_poisonArcherInformation;
extern const char* loc_almanac_unitDelayerTitle;
extern const char* loc_almanac_unitDelayerInformation;
extern const char* loc_almanac_bolasThrowerBaseTitle;
extern const char* loc_almanac_bolasThrowerBaseInformation;
extern const char* loc_almanac_bolasThrowerFloorTitle;
extern const char* loc_almanac_bolasThrowerFloorInformation;
extern const char* loc_almanac_bolasThrowerConfuseTitle;
extern const char* loc_almanac_bolasThrowerConfuseInformation;
extern const char* loc_almanac_baseWarriorTitle;
extern const char* loc_almanac_baseWarriorInformation;
extern const char* loc_almanac_resistantWarriorTitle;
extern const char* loc_almanac_resistantWarriorInformation;
extern const char* loc_almanac_aggressiveWarriorTitle;
extern const char* loc_almanac_aggressiveWarriorInformation;
extern const char* loc_almanac_baseMageTitle;
extern const char* loc_almanac_baseMageInformation;
extern const char* loc_almanac_speederMageTitle;
extern const char* loc_almanac_speederMageInformation;
extern const char* loc_almanac_mightierMageTitle;
extern const char* loc_almanac_mightierMageInformation;

//Enemies Almanac
extern const char* loc_almanac_soldierTitle;
extern const char* loc_almanac_soldierInfo;
extern const char* loc_almanac_baronTitle;
extern const char* loc_almanac_baronInfo;
extern const char* loc_almanac_convictTitle;
extern const char* loc_almanac_convictInfo;
extern const char* loc_almanac_chemistTitle;
extern const char* loc_almanac_chemistInfo;
extern const char* loc_almanac_tankTitle;
extern const char* loc_almanac_tankInfo;
extern const char* loc_almanac_rabbitTitle;
extern const char* loc_almanac_rabbitInfo;
extern const char* loc_almanac_pirateTitle;
extern const char* loc_almanac_pirateInfo;
extern const char* loc_almanac_captainTitle;
extern const char* loc_almanac_captainInfo;

//credits
extern const char* loc_credits_title0;
extern const char* loc_credits_title1;
extern const char* loc_credits_title2;
extern const char* loc_credits_title3;
extern const char* loc_credits_title4;
extern const char* loc_credits_title5;
extern const char* loc_credits_name0;
extern const char* loc_credits_name1;
extern const char* loc_credits_name2;
extern const char* loc_credits_name3;
extern const char* loc_credits_name4;
extern const char* loc_credits_name5;
extern const char* loc_credits_name6;
extern const char* loc_credits_name7;
extern const char* loc_credits_contact;
extern const char* loc_credits_marmalade;

//tutorial
extern const char* loc_tutorial_part0;
extern const char* loc_tutorial_part1;
extern const char* loc_tutorial_part2;
extern const char* loc_tutorial_part3;
extern const char* loc_tutorial_part4;
extern const char* loc_tutorial_part5;
extern const char* loc_tutorial_part6;
extern const char* loc_tutorial_part7;
extern const char* loc_tutorial_part8;
extern const char* loc_tutorial_part9;
extern const char* loc_tutorial_part10;
extern const char* loc_tutorial_part11;
extern const char* loc_tutorial_part12;
extern const char* loc_tutorial_part13;
extern const char* loc_tutorial_part14;
extern const char* loc_tutorial_part15;
extern const char* loc_tutorial_part16;
extern const char* loc_tutorial_part17;
extern const char* loc_tutorial_part18;

//world dialogs
extern const char* loc_dialog_tapToContinue;
extern const char* loc_dialog_tapToContinue_alt;
extern const char* loc_dialog_enemy;
extern const char* loc_dialog_selknam;
extern const char* loc_dialog_world_1_0;
extern const char* loc_dialog_world_1_1;
extern const char* loc_dialog_world_1_2;
extern const char* loc_dialog_world_1_3;
extern const char* loc_dialog_world_1_4;
extern const char* loc_dialog_world_2_0;
extern const char* loc_dialog_world_2_1;
extern const char* loc_dialog_world_2_2;
extern const char* loc_dialog_world_2_3;
extern const char* loc_dialog_world_3_0;
extern const char* loc_dialog_world_3_1;
extern const char* loc_dialog_world_3_2;
extern const char* loc_dialog_world_4_0;
extern const char* loc_dialog_world_4_1;
extern const char* loc_dialog_world_4_2;
extern const char* loc_dialog_world_4_3;
extern const char* loc_dialog_world_4_4;
extern const char* loc_dialog_world_4_5;
extern const char* loc_dialog_world_end_0;
extern const char* loc_dialog_world_end_1;
extern const char* loc_dialog_world_end_2;
extern const char* loc_dialog_world_end_3;
extern const char* loc_dialog_world_end_4;
extern const char* loc_dialog_world_end_5;
extern const char* loc_dialog_world_end_6;
extern const char* loc_dialog_world_altend_0;
extern const char* loc_dialog_world_altend_1;

//social
extern const char* loc_socialMenu;
extern const char* loc_socialSurvival_part0;
extern const char* loc_socialSurvival_part1;
extern const char* loc_socialStage_part0;
extern const char* loc_socialStage_part1;
extern const char* loc_socialStage_part2;


class LocalizationSystem
{
public:
	LocalizationSystem();
	~LocalizationSystem();

	static void RetrieveLocalization(GameLang selknamLanguage);
};



#endif	// _LOCALIZATION_H_

