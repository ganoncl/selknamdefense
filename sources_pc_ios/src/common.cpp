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
#include "stdlib.h"
#include "captainboss.h"
#include "almanac_base.h"
//#include "savedatahelper.h"
#if defined DESKTOPPLATFORM
#include "s3eWindow.h"
#endif
#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif

int mouseDetectionSensibility = 5; //5 pixels of different related to last mouse position. Sum of X-axis and Y-axis.
int cameraSpeedConstant = 450; //miliseconds required to move the camera from position A to position B.
s3eKey noKeyAssignment = s3eKeyFirst; //Like NULL pointer.

/******GAME OPTIONS*******/
bool bIsFullVersion = false;
bool bWaterMark = false;
bool bNoMusic = false;
bool bUnitMenuAttachedToMouseWhenOpened = true;
bool bIsXbox360ControllerEnabled = true;
bool bUseHDImages = true;
bool bLoadAllResources = false;

//vendors check
bool bIwBillingIosAvailable = false;
//kiip check
bool bKiipAvailable = false;
//chartboost check
bool bChartBoostAvailable = false;
//iAds check
bool biAdAvailable = false;

int showCharBoostOnMainMenuCount = 1;
//remove ads productId
const char* removeAds_ProductID = "com.meridian4.selknamdefense.removeads";

const char* removeAds_Data_ProductID = "none";
const char* removeAds_Data_Title = "none";
const char* removeAds_Data_Desc = "none";
const char* removeAds_Data_Price = "none";

//Audio Background Available
bool biOSBackgroundAudioAvailable = false;

bool bErrorOnLastPurchase = false;
bool bItemRestoredSuccess = false;
bool bItemPurchasedSuccess = false;
bool bItemRefundSuccess = false;



//Este numero solo puede ir incrementandose, si se aumenta, la data se reseteara, 
//sirve para handlear los casos donde el save data cambia de tamaño
int saveDataVersion = 0;

/******XBOX 360 GAMEPAD ******/
int xbox360CameraSpeed = 20;

XBOX360_Button Xbox360_Left    = XBOX360_Button_Left;
XBOX360_Button Xbox360_Up      = XBOX360_Button_Up;
XBOX360_Button Xbox360_Down    = XBOX360_Button_Down;
XBOX360_Button Xbox360_Right   = XBOX360_Button_Right;
XBOX360_Button Xbox360_Action1 = XBOX360_Button_A;
XBOX360_Button Xbox360_Action2 = XBOX360_Button_X;
XBOX360_Button Xbox360_Action3 = XBOX360_Button_Start;
XBOX360_Button Xbox360_Cancel1 = XBOX360_Button_B;
XBOX360_Button Xbox360_Cancel2 = XBOX360_Button_Y;
XBOX360_Button Xbox360_IncreaseWaveSpeed = XBOX360_Button_RB;
XBOX360_Button Xbox360_DecreaseWaveSpeed = XBOX360_Button_LB;
XBOX360_Button Xbox360_OpenIngameAlmanac = XBOX360_Button_Select;
XBOX360_Button Xbox360_OpenIngameMenu = XBOX360_Button_Start;

extern s3eKey currKeyPressed = s3eKeyFirst;

//Ads
const char* leadboltAdNumber = "274912241";

s3eDeviceOSID currentDeviceOS = S3E_OS_ID_WINDOWS;
s3eDeviceLanguage currentDeviceLanguage = S3E_DEVICE_LANGUAGE_ENGLISH;
const char* currentDeviceID = "";
uint32 currentDisplayWidth = 0;
uint32 currentDisplayHeight = 0;
uint32 currentScreenWidth = 0;
uint32 currentScreenHeight = 0;
uint32 currentDeviceWidth = 0;
uint32 currentDeviceHeight = 0;
uint32 currentSurfaceWidth = 0;
uint32 currentSurfaceHeight = 0;
uint32 currentGameWidth = 0;
uint32 currentGameHeight = 0;
GameHeight gameHeight = HEIGHT_768;
uint32 srcGameWidth = 1024;
uint32 srcGameHeight = 768;
GameLang gameLang = GAMELANG_INT;
/*************************/

//Save data handling
GameDataCampaign gameDataCampaign;
GameDataSurvival gameDataSurvival;
GameLocalData gameLocalData;

//Version
string version_number = "1.3.2";
string version_date = "2016.09.25";

//Analytics
bool bFlurryOn = false;
//Ads
//bool bLeadbolt = false;
int adOnWinLevel = 0;

//Landscape Hack
bool bLandscapeHack = false;

bool bBlockCamera = false;
bool bFreezeCamera = false;

int countDownDeathDebuff_Chance = 50; // 0 - 100.  50 means 50% success.

//--------------------------------------------------FINAL BOSS STAGE SETTINGS--------------------------------------------//
//time measured in miliseconds.
int finalboss_num_waves[3];
IsoEnemyType finalboss_unit_to_spawn[3][10][4]; 

//White Screen at the start of the stage.
int finalBoss_time_before_fadeout = 1000; //100% white duration.
int finalBoss_time_fadeout        = 1800; //Time fading.

//Backward Stage: Boss cannot be hit, constant spawn of enemies, normal cannon shots.
//Forward Stage: Special Attack.

int finalBoss_time_resting  = 5000; //Delay between Backward and Forward transition.
int finalBoss_time_pain_anim_can_be_played = 6000; //When boss is hit, pain animations occurs. 
												   //To limit multiple pain animations, only one anim each 
				                                   //"finalBoss_time_pain_anim_can_be_played" is allowed.
int finalBoss_time_ship_explotion_time = 7000; //Boss destruction scene duration.
int finalboss_time_ship_explotion_single_explotion = 250; //Time between explotions during ending scene.
int finalboss_sp_markspot_duration = 2000; //terrain mark indicating shot place.
int finalboss_forward_time = 10000;
int finalboss_max_health = 9999;
int finalboss_deathdebuff_time = 3000; //Stage 3 debuff.
int finalboss_extracoins_per_enemy = 2; //extra coins earned each time an enemy is killed.
float finalboss_ship_idle_speed = 0.15f; //default speed = 1.0f

//Poison debuff
int finalboss_poison_damage_per_interval = 2;
int finalboss_poison_interval_time = 4;
int finalboss_poison_num_intervals = 5;

//Stage 1
int finalboss_cycle1_backward_time = 35000; //Backward stage duration.
int finalboss_cycle1_wave_delay = 4000; //Time between an enemy spawn and another during a same wave.
int finalboss_cycle1_wave_time = 8500; //Time between waves.
int finalboss_cycle1_time_before_sp = 2000; //Time before perfom Special Attack during Forward Stage.
int finalboss_cycle1_time_between_sp_shots = 200;
int finalBoss_cycle1_time_recharge = 5500; //Time between two cannon shots in Backward Stage.
int finalboss_cycle1_sp_num_shots = 15;
int finalboss_cycle1_cannon_damage = 20;

//Stage 2
int finalboss_cycle2_backward_time = 40000;
int finalboss_cycle2_wave_delay = 4500;
int finalboss_cycle2_wave_time = 7000;
int finalboss_cycle2_time_before_sp = 2000;
int finalboss_cycle2_time_between_sp_shots = 125;
int finalBoss_cycle2_time_recharge = 5000;
int finalboss_cycle2_sp_num_shots = 15;
int finalboss_cycle2_cannon_damage = 15;

//Stage 3
int finalboss_cycle3_backward_time = 40000;
int finalboss_cycle3_wave_delay = 5000;
int finalboss_cycle3_wave_time = 7500;
int finalboss_cycle3_time_before_sp = 1500;
int finalboss_cycle3_time_between_sp_shots = 75;
int finalBoss_cycle3_time_recharge = 5000;
int finalboss_cycle3_sp_num_shots = 20;
int finalboss_cycle3_cannon_damage = 10;
//--------------------------------------------------FINAL BOSS STAGE SETTINGS--------------------------------------------//

//Mouse configuration
int32 iLastMouseX = -10; 
int32 iLastMouseY = -10;
//SoundManager* FXSoundManager = NULL;

//Button Speed Mechanics.
float normalGameSpeed = 1.0f;
float doubleGameSpeed = 1.6f;
float tripleGameSpeed = 2.631f;
float currentGameSpeedValue = 1.0f;
TimerSpeed waveTimerSpeed = WTIMER_NORMAL;

//Touch configuration
//Number of pixels that the finger needs to move trough the window to perform a map reposition. 
//If TouchMotionTolerance >= (Y difference) || TouchMotionTolerance >= (X difference) then move.
int TouchMotionTolerance = 4;

GameState gameState = GS_None;
GameState lastGameState = GS_None;

MainMenuState mainMenuState = GSM_None;
MainMenuState lastMainMenuState = GSM_None;

PopUpState popUpState = GSP_None;
PopUpState lastPopUpState = GSP_None;

InGameState inGameState = GSG_None;
InGameState lastInGameState = GSG_None;

InGamePausedState inGamePausedState = GSGP_None;
InGamePausedState lastInGamePausedState = GSGP_None;

MainMenuIGState mainMenuIGState = GSMIG_None;
MainMenuIGState lastMainMenuIGState = GSMIG_None;

MapSelectionState mapSelectionState = MS_None;
MapSelectionState lastMapSelectionState = MS_None;

XBOX360_Button currentButtonPressed = XBOX360_Button_None;
XBOX360_Button lastButtonPressed = XBOX360_Button_None;

PopUp* popup = NULL;

FadeScreen* fade = NULL;

bool bPause = false;
bool bBlockGUI = false;

int currentMap = 1;

int totalScoreTmp = 0;
string totalSurvivalScoreTmp;

//SCALED
IsoOffset isoTerrainOffset    = {-48.0f, -24.0f};

IsoOffset isoESoldierOffset   = {-50.0f, -102.0f};
IsoOffset isoEVaronOffset     = {-50.0f, -102.0f};
IsoOffset isoESuicideOffset   = {-50.0f, -102.0f};
IsoOffset isoEScientistOffset = {-50.0f, -102.0f};
IsoOffset IsoETankOffset      = {-50.0f, -102.0f};
IsoOffset isoEPirateOffset    = {-50.0f, -102.0f};
IsoOffset isoERabbitOffset    = {-50.0f, -102.0f};

IsoOffset isoSArcherOffset          = {-50.0f, -102.0f};
IsoOffset isoSArcherPoisonerOffset  = {-50.0f, -102.0f};
IsoOffset isoSArcherRetarderOffset  = {-50.0f, -102.0f};
IsoOffset isoSShamanHealerOffset    = {-50.0f, -102.0f};
IsoOffset isoSShamanMightOffset     = {-50.0f, -102.0f};
IsoOffset isoSShamanHasteOffset     = {-50.0f, -102.0f};
IsoOffset isoSWarriorOffset         = {-50.0f, -102.0f};
IsoOffset isoSWarriorBerserkOffset  = {-50.0f, -102.0f};
IsoOffset isoSWarriorTankOffset     = {-50.0f, -102.0f};
IsoOffset isoSBowlerConfuserOffset  = {-50.0f, -102.0f};
IsoOffset isoSBowlerRooterOffset    = {-50.0f, -102.0f};
IsoOffset isoSBowlerStunerOffset    = {-50.0f, -102.0f};

//archers
PawnAttributes SelknamAtt_Archer_Base[3];
PawnAttributes SelknamAtt_Archer_Poisoner[3];
PawnAttributes SelknamAtt_Archer_Retarder[3];
//warriors
PawnAttributes SelknamAtt_Warrior_Base[3];
PawnAttributes SelknamAtt_Warrior_Tank[3];
PawnAttributes SelknamAtt_Warrior_Berserk[3];
//shamans
PawnAttributes SelknamAtt_Shaman_Healer[3];
PawnAttributes SelknamAtt_Shaman_Might[3];
PawnAttributes SelknamAtt_Shaman_Haste[3];
//bowler
PawnAttributes SelknamAtt_Bowler_Stuner[3];
PawnAttributes SelknamAtt_Bowler_Rooter[3];
PawnAttributes SelknamAtt_Bowler_Confuser[3];
//ENEMIES
PawnAttributes Enemy_Att_Soldier[3];
PawnAttributes Enemy_Att_Varon[3];
PawnAttributes Enemy_Att_Convict[3];
PawnAttributes Enemy_Att_Scientist[3];
PawnAttributes Enemy_Att_Tank[3];
PawnAttributes Enemy_Att_Rabbit[3];
PawnAttributes Enemy_Att_Pirate[3];

//SKILLS
Skill_Poison				Skill_Att_Poison[NUM_SKILLS];
Skill_Slow					Skill_Att_Slow[NUM_SKILLS];
Skill_Stun					Skill_Att_Stun[NUM_SKILLS];
Skill_Root					Skill_Att_Root[NUM_SKILLS];
Skill_Confusion				Skill_Att_Confusion[NUM_SKILLS];
Skill_Explode				Skill_Att_Explode[NUM_SKILLS];
Skill_PoisonOnDeath			Skill_Att_PoisonOnDeath[NUM_SKILLS];
Skill_Heal					Skill_Att_Heal[NUM_SKILLS];
Skill_Might					Skill_Att_Might[NUM_SKILLS];
Skill_Haste					Skill_Att_Haste[NUM_SKILLS];

//Selknam Store (0: Not Purchased yet)
UnitUpgrades ArcherBaseUpgrades     = {1};
UnitUpgrades ArcherPoisonerUpgrades = {0};
UnitUpgrades ArcherRetarderUpgrades = {0};
UnitUpgrades BowlerStunerUpgrades   = {1};
UnitUpgrades BowlerRooterUpgrades   = {0};
UnitUpgrades BowlerConfuserUpgrades = {0};
UnitUpgrades WarriorBaseUpgrades    = {1}; 
UnitUpgrades WarriorTankUpgrades    = {0};
UnitUpgrades WarriorBerserkUpgrades = {0};
UnitUpgrades ShamanMightUpgrades    = {0};
UnitUpgrades ShamanHasteUpgrades    = {1};
UnitUpgrades ShamanHealerUpgrades   = {0};

//Max Stars that can be collected through the game
int Max_Stars = 72;
//Stars used in the Selknam Store
int Used_Stars = 0;
//Stars that player currently has for each level.
int Stars_By_Level[MAX_MAPS] =   {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
								  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
								  0, 0, 0, 0, 0};

int Stars_By_ExtraLevel[MAX_EXTRA_MAPS] = {0};

//Skills
Skill_Poison Skill_Poison_Att = {1, 20, 10, 5};
Skill_Slow Skill_Slow_Att = {50, 5};
Skill_Heal Skill_Heal_Att = {1, 5};
Skill_Might Skill_Might_Att = {1, 100};
Skill_Haste Skill_Haste_Att = {1, 10};

NewGame NewGame_State = EMPTY_GAME;

MapStatus MapStatus_State[MAX_MAPS] = {CURRENT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE,
	                                   NOT_PLAYABLE,     NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE,
									   NOT_PLAYABLE,     NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE,
									   NOT_PLAYABLE,     NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE};
/*									   
MapStatus MapStatus_State[MAX_MAPS] = {PLAYABLE, CURRENT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE,
	NOT_PLAYABLE,     NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE,
	NOT_PLAYABLE,     NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE,
	NOT_PLAYABLE,     NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE, NOT_PLAYABLE};
*/
SoundStatus FX_Sound_State = NORMAL;
SoundStatus Music_Sound_State = NORMAL;

int archer_base_cost[3]       = {0, 2, 2};
int archer_poisoner_cost[3]   = {3, 2, 2};
int archer_retarder_cost[3]   = {3, 2, 2};
int warrior_base_cost[3]      = {0, 2, 2};
int warrior_tank_cost[3]      = {3, 2, 2};
int warrior_berserker_cost[3] = {3, 2, 2};
int mage_might_cost[3]             = {3, 2, 2};
int mage_heal_cost[3]              = {0, 2, 2};
int mage_haste_cost[3]             = {3, 2, 2};
int bowler_stuner_cost[3]          = {0, 2, 2};
int bowler_rooter_cost[3]          = {3, 2, 2};
int bowler_confuser_cost[3]        = {3, 2, 2};

int fade_menu = 210;

//ANIMATION CONTROLLER [x][y][z][Max_Animation]
//[x], cada clase.
//[y], cada tipo de animacion, Idle, Walk, Attack, Die
//[z], cada una de las direcciones.
//Orden de las direcciones
//north, northeast, east, southeast, south, southwest, west, northwest

//Pawns
int16 Anim_Pawns[19][4][9][MAX_ANIMATION];

bool bQuittingGame = false;

CIwSoundSpec* FX_Arrow_Shooting_01;
CIwSoundSpec* FX_Arrow_Shooting_02;
CIwSoundSpec* FX_Click_01;
CIwSoundSpec* FX_Click_02;
CIwSoundSpec* FX_Explosion;
CIwSoundSpec* FX_Knife_Sword_01;
CIwSoundSpec* FX_Knife_Sword_02;
CIwSoundSpec* FX_Knife_Sword_03;
CIwSoundSpec* FX_RollOver_02;
CIwSoundSpec* FX_Shot_01;
CIwSoundSpec* FX_Star_Gained_02;
CIwSoundSpec* FX_Silence;
CIwSoundSpec* FX_MetalImpact_01;
CIwSoundSpec* FX_MetalImpact_02;
CIwSoundSpec* FX_Coins;
CIwSoundSpec* FX_Heal;
CIwSoundSpec* FX_BuffOn;
CIwSoundSpec* FX_Bubbles;
CIwSoundSpec* FX_Punch_01;
CIwSoundSpec* FX_Punch_02;
CIwSoundSpec* FX_Punch_03;
CIwSoundSpec* FX_Punch_04;
CIwSoundSpec* FX_Punch_05;
CIwSoundSpec* FX_Sell_Unit;
CIwSoundSpec* FX_LooseALive;
CIwSoundSpec* FX_Dying_Baron;
CIwSoundSpec* FX_Dying_Convict;
CIwSoundSpec* FX_Dying_Pirate;
CIwSoundSpec* FX_Dying_Rabbit;
CIwSoundSpec* FX_Dying_Scientist;
CIwSoundSpec* FX_Dying_Soldier;
CIwSoundSpec* FX_Dying_Tank;
CIwSoundSpec* FX_Selknam_Archer_Deploy_01;
CIwSoundSpec* FX_Selknam_Archer_Deploy_02;
CIwSoundSpec* FX_Selknam_Archer_Select_01;
CIwSoundSpec* FX_Selknam_Archer_Select_02;
CIwSoundSpec* FX_Selknam_Bowler_Deploy_01;
CIwSoundSpec* FX_Selknam_Bowler_Deploy_02;
CIwSoundSpec* FX_Selknam_Bowler_Select_01;
CIwSoundSpec* FX_Selknam_Bowler_Select_02;
CIwSoundSpec* FX_Selknam_Mage_Deploy_01;
CIwSoundSpec* FX_Selknam_Mage_Deploy_02;
CIwSoundSpec* FX_Selknam_Mage_Select_01;
CIwSoundSpec* FX_Selknam_Mage_Select_02;
CIwSoundSpec* FX_Selknam_Warrior_Deploy_01;
CIwSoundSpec* FX_Selknam_Warrior_Deploy_02;
CIwSoundSpec* FX_Selknam_Warrior_Select_01;
CIwSoundSpec* FX_Selknam_Warrior_Select_02;
CIwSoundSpec* FX_Dying_Selknam_Archer;
CIwSoundSpec* FX_Dying_Selknam_Bowler;
CIwSoundSpec* FX_Dying_Selknam_Mage;
CIwSoundSpec* FX_Dying_Selknam_Warrior;
CIwSoundSpec* FX_Boss_angry_after_paincycle;
CIwSoundSpec* FX_boss_comment_cycle01_backward_01;
CIwSoundSpec* FX_Boss_comment_cycle01_moving_forward_01;
CIwSoundSpec* FX_Boss_comment_cycle01_prefire;
CIwSoundSpec* FX_Boss_comment_cycle02_backward_01;
CIwSoundSpec* FX_Boss_comment_cycle02_moving_forward_01;
CIwSoundSpec* FX_Boss_comment_cycle02_prefire;
CIwSoundSpec* FX_Boss_comment_cycle03_backward_01;
CIwSoundSpec* FX_Boss_comment_cycle03_backward_02;
CIwSoundSpec* FX_Boss_comment_cycle03_moving_forward_01;
CIwSoundSpec* FX_Boss_comment_cycle03_prefire;
CIwSoundSpec* FX_Boss_dying;
CIwSoundSpec* FX_Boss_pain_01;
CIwSoundSpec* FX_Boss_pain_02;
CIwSoundSpec* FX_Boss_pain_03;
CIwSoundSpec* FX_Cannon_ship;
CIwSoundSpec* FX_Boss_appear_explosion;
CIwSoundSpec* FX_Boss_laugh;

int sound_arrows_numallowed = 2;
int sound_arrows_numcurrent = 0;
int sound_arrows_milisectorenew = 250;
int sound_arrows_milisecstamp = 0;

int sound_cannon_ship_numallowed = 1;
int sound_cannon_ship_numcurrent = 0;
int sound_cannon_ship_milisectorenew = 250;
int sound_cannon_ship_milisecstamp = 0;

int sound_explosion_numallowed = 1;
int sound_explosion_numcurrent = 0;
int sound_explosion_milisectorenew = 300;
int sound_explosion_milisecstamp = 0;

int sound_loosealive_numallowed = 1;
int sound_loosealive_numcurrent = 0;
int sound_loosealive_milisectorenew = 1000;
int sound_loosealive_milisecstamp = 0;

int sound_punch_numallowed = 2;
int sound_punch_numcurrent = 0;
int sound_punch_milisectorenew = 250;
int sound_punch_milisecstamp = 0;

bool bExtraMapMode = false;
bool bFxSoundMuted = true;

//int8 accumulated_frames = 0;

//int8 fx_sound_limit[MAX_FX_SOUNDS][2];

bool bResetStoreStats;
bool bPausedStore;
bool bFirstMapWasPlayed = false;
bool bBlockKeyboardOnce = false;

bool bBlockOnQuit = false;

/*******TUTORIAL*******/
Tutorial* tutorial = NULL;
bool bShowTutorialLevel1 = true;
bool bShowTutorialStars = true;
bool bShowTutorialUnlockStore = false;
bool bShowTutorialSurvivalModeUnlock = true;
bool bPauseGame = false;
bool bPauseMapSelection = false;
bool bMouseActive = false;
bool bStopWaveTimer = false;
bool bUpdateClickEvents = false;
/*********************/

bool bShowTrialVersionFinal = false;
bool bShowTrialScreen = true;
bool bBlockMousePointer = false;

/******AUDIO SETTINGS*****/
/*
Music_Name CurrentMusicPlaying = Music_None;
Music_Name LastMusicPlaying    = Music_None;
*/
bool bIsAudioPaused            = false;
bool bIsFxPaused               = false;
Music_Name LastMusicStacked = Music_None;
/*************************/

int xWindowOffset = 0;
int yWindowOffset = 0;

int LastTouchX = -1;
int LastTouchY = -1;

int xMouse = 0;
int yMouse = 0;

//Array of groups
int commonResourcesNum = 15;
const char* commonResources[15];

int nonHDImagesResourcesNum = 12;
const char* nonHDImagesResources[12];

int hDImagesResourcesNum = 23;
const char* hDImagesResources[23];

int totalResourcesLoadedNum = 0;
char totalResourcesLoaded[256][256];

TextureData buffBarTextureData;

//Texture Data
TextureData TextureDataList[256];
int currentTextureId = 0;

CIwTexture* atlas_bkg_gameplay	   = NULL;
CIwTexture* atlas_bkg_menu_foreground	= NULL;
CIwTexture* atlas_bkg_menu_others  = NULL;
CIwTexture* atlas_bkg_menu_sky     = NULL;
CIwTexture* atlas_bkg_worldmap	   = NULL;
CIwTexture* atlas_bkg_small_0	   = NULL;
CIwTexture* atlas_bkg_small_1	   = NULL;
CIwTexture* atlas_common_textures_0		   = NULL;
CIwTexture* atlas_common_textures_1		   = NULL;
CIwTexture* atlas_common_textures_2		   = NULL;
CIwTexture* atlas_common_textures_3		   = NULL;
CIwTexture* atlas_common_textures_4		   = NULL;
CIwTexture* atlas_small_textures		   = NULL;
CIwTexture* atlas_common_trial_0   = NULL;
CIwTexture* atlas_fx_0             = NULL;
CIwTexture* atlas_fx_1             = NULL;
CIwTexture* atlas_fx_2             = NULL;
CIwTexture* atlas_props_0          = NULL;
CIwTexture* atlas_props_1          = NULL;
CIwTexture* atlas_props_2          = NULL;
CIwTexture* atlas_terrain_0        = NULL;
CIwTexture* atlas_terrain_1        = NULL;
CIwTexture* atlas_others = NULL;

CIwTexture* atlas_hd_selknam_archer_1			= NULL;
CIwTexture* atlas_hd_selknam_archer_2			= NULL;
CIwTexture* atlas_hd_selknam_archer_3			= NULL;
CIwTexture* atlas_hd_selknam_bowler_1_01		= NULL;
CIwTexture* atlas_hd_selknam_bowler_1_02		= NULL;
CIwTexture* atlas_hd_selknam_bowler_2_01		= NULL;
CIwTexture* atlas_hd_selknam_bowler_2_02		= NULL;
CIwTexture* atlas_hd_selknam_bowler_3_01		= NULL;
CIwTexture* atlas_hd_selknam_bowler_3_02		= NULL;
CIwTexture* atlas_hd_selknam_mage_1				= NULL;
CIwTexture* atlas_hd_selknam_mage_2				= NULL;
CIwTexture* atlas_hd_selknam_mage_3				= NULL;
CIwTexture* atlas_hd_selknam_melee_1			= NULL;
CIwTexture* atlas_hd_selknam_melee_2			= NULL;
CIwTexture* atlas_hd_selknam_melee_3			= NULL;

CIwTexture* atlas_hd_enemy_1					= NULL;
CIwTexture* atlas_hd_enemy_2					= NULL;
CIwTexture* atlas_hd_enemy_3					= NULL;
CIwTexture* atlas_hd_enemy_4_01					= NULL;
CIwTexture* atlas_hd_enemy_4_02					= NULL;
CIwTexture* atlas_hd_enemy_5					= NULL;
CIwTexture* atlas_hd_enemy_6					= NULL;
CIwTexture* atlas_hd_enemy_7					= NULL;

//Atlas sizes
AtlasHDSize atlas_hdsize_selknam_archer = {960.0f, 1008.0f};
AtlasHDSize atlas_hdsize_selknam_bowler_01 = {960.0f, 1008.0f};
AtlasHDSize atlas_hdsize_selknam_bowler_02 = {288.0f, 288.0f};
AtlasHDSize atlas_hdsize_selknam_mage = {864.0f, 864.0f};
AtlasHDSize atlas_hdsize_selknam_melee = {960.0f, 1008.0f};

AtlasHDSize atlas_hdsize_enemy_1 = {960.0f, 1008.0f};
AtlasHDSize atlas_hdsize_enemy_2 = {960.0f, 1008.0f};
AtlasHDSize atlas_hdsize_enemy_3 = {480.0f, 576.0f};
AtlasHDSize atlas_hdsize_enemy_4_01 = {960.0f, 1008.0f};
AtlasHDSize atlas_hdsize_enemy_4_02 = {288.0f, 288.0f};
AtlasHDSize atlas_hdsize_enemy_5 = {960.0f, 432.0f};
AtlasHDSize atlas_hdsize_enemy_6 = {960.0f, 576.0f};
AtlasHDSize atlas_hdsize_enemy_7 = {960.0f, 1008.0f};

CIwTexture* atlas_selknam_archer_01_1 = NULL;
CIwTexture* atlas_selknam_archer_01_2 = NULL;
CIwTexture* atlas_selknam_archer_02_1 = NULL;
CIwTexture* atlas_selknam_archer_02_2 = NULL;
CIwTexture* atlas_selknam_archer_03_1 = NULL;
CIwTexture* atlas_selknam_archer_03_2 = NULL;
CIwTexture* atlas_selknam_melee_01_1  = NULL;
CIwTexture* atlas_selknam_melee_01_2  = NULL;
CIwTexture* atlas_selknam_melee_02_1  = NULL;
CIwTexture* atlas_selknam_melee_02_2  = NULL;
CIwTexture* atlas_selknam_melee_03_1  = NULL;
CIwTexture* atlas_selknam_melee_03_2  = NULL;
CIwTexture* atlas_selknam_mage_01_1   = NULL;
CIwTexture* atlas_selknam_mage_01_2   = NULL;
CIwTexture* atlas_selknam_mage_02_1   = NULL;
CIwTexture* atlas_selknam_mage_02_2   = NULL;
CIwTexture* atlas_selknam_mage_03_1   = NULL;
CIwTexture* atlas_selknam_mage_03_2   = NULL;
CIwTexture* atlas_selknam_bowler_01_1 = NULL;
CIwTexture* atlas_selknam_bowler_01_2 = NULL;
CIwTexture* atlas_selknam_bowler_02_1 = NULL;
CIwTexture* atlas_selknam_bowler_02_2 = NULL;
CIwTexture* atlas_selknam_bowler_03_1 = NULL;
CIwTexture* atlas_selknam_bowler_03_2 = NULL;

CIwTexture* atlas_enemy_01_1	   = NULL;
CIwTexture* atlas_enemy_01_2	   = NULL;
CIwTexture* atlas_enemy_02_1	   = NULL;
CIwTexture* atlas_enemy_02_2       = NULL;
CIwTexture* atlas_enemy_03_1       = NULL;
CIwTexture* atlas_enemy_04_1       = NULL;
CIwTexture* atlas_enemy_04_2       = NULL;
CIwTexture* atlas_enemy_05_1       = NULL;
CIwTexture* atlas_enemy_06_1       = NULL;
CIwTexture* atlas_enemy_07_1       = NULL;
CIwTexture* atlas_enemy_07_2       = NULL;

CIwTexture* atlas_boss                = NULL;



//SCREEN FIX
//SCALED
float globalScale = 1.5f;
float tileMapWidthImage = 96.0f;
float tileMapHeightImage = 72.0f;
float mapXOffset = -50;
float mapYOffset = 90;
float mapTileUpperCornerX = 870;
//Isometric Pos Helpers
//SCALED
float tileMapWidthBase = 96.0f;
float tileMapHeightBase = 48.0f;
float tileDiffLayer = 24.0f;
float isoFormulaWValue = tileMapWidthBase * 0.5f;
float isoFormulaHValue = tileMapHeightBase * 0.5f;
float isoFormulaAValue = isoFormulaWValue * isoFormulaHValue * 2;
float isoFormulaInvAValue = 1.0f / isoFormulaAValue;
//place of first tile
float isoFormulaXOffset;
float isoFormulaYOffset;

int xMinOffset = -140;
int xMaxOffset = 755;
int yMinOffset = -70;
int yMaxOffset = 240;

GameMode gameMode = GAMEMODE_CAMPAIGN;

DialogController* dialogController = NULL;

//Survival Mode
int numSelknamKilledByMinute = 0;
int tempNumSelknamKilledByMinute = 0;
SurvivalModeConfig* survivalModeConfig = NULL;

almanacState curAlmanacState  = ALMANAC_NONE;
almanacState lastAlmanacState = ALMANAC_NONE;

AlmanacBase* almanacBase = NULL;
SettingsMenu* settingsMenu = NULL;

bool isEnemyUnlocked[MAX_ENEMIES];

int bestScoreByLevel[MAX_MAPS];

bool bCheatModeEnabled = false;

CaptainBoss* captainBoss = NULL;

//Time Helper Global Value
uint64 deltaSystemTime = 0;
uint64 lastSystemTime = s3eTimerGetMs();

//savedata
SaveDataHelper *saveDataHelper = NULL;

//shaders
CIwGxShaderTechnique* shader_font_a;
CIwGxShaderTechnique* shader_blur;
CIwGxShaderTechnique* shader_bicubic;

//achievement helper
AchievementsHelper	*achievementsHelper = NULL;
LeaderboardsHelper	*leaderboardsHelper = NULL;

CIwRect dynamicGUIWrapper[GAMEPLAY_NUM_WRAPPER_RECTS];

//Steam vars
bool bSteamAvailable = false;
bool bSteam_LoggedOn = false;
const char* steam_PersonalName = "steamUser";
uint32 steamAppId = 0;
const char* steam_AppAllLanguages = "";
const char* steam_AppCurrentLanguage = "";

//Game Center vars
bool bGameCenterAvailable = false;
bool bGameCenterAuthenticated = false;
char gameCenterUserName[256] = "Unknown User";

#ifdef STEAMREQUIRED
//STEAM
CSteamAchievements *g_SteamAchievements = NULL;
CSteamLeaderboards *g_SteamLeaderboards = NULL;
#endif

int numFocusedObjects = 0;

ControllerType activeControllerType = Controller_MOUSE;

//Screen Handle
#if defined DESKTOPPLATFORM
int displayNumModes = 0;
s3eWindowDisplayMode* displayModes = NULL;
s3eWindowDisplayMode g_FullscreenMode = {0};
int displayWidthSelected = 0;
int displayHeightSelected = 0;
#endif

//FPS MULTIPLIER
int oldFPS = 35;
float FPSMULTIPLIER = (float)oldFPS/(float)UPS;

//Settings
int currentAudioVolume = 0;
int currentSoundVolume = 0;
int lastAudioVolume = 0;
int lastSoundVolume = 0;
//All Muted
bool bAllMuted = false;
bool bIsFullScreenOption = false;
