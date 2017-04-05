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


/******GAME OPTIONS*******/
#ifdef FULLVERSION
bool bIsFullVersion = true;
#else
bool bIsFullVersion = false;
#endif

bool bRenderColorButtons = true;
/*************************/

//Version
string version_number = "1.2.1";
string version_date = "2014.07.13";
bool bWaterMark = false;
//Old Audio System is based on s3eAudioPause(), the other one is based on S3E_POSITION, s3eSetInt and s3eGetInt methods.
bool bUseOldAudioSystem = true;

//Mouse configuration
int32 iLastMouseX = -10; 
int32 iLastMouseY = -10;
//SoundManager* FXSoundManager = NULL;

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

PopUp* popup = NULL;

FadeScreen* fade = NULL;

bool bPause = false;

int currentMap = 1;

IsoOffset isoTerrainOffset    = {-32.0f, -16.0f};

IsoOffset isoESoldierOffset   = {-32.0f, -64.0f};
IsoOffset isoEVaronOffset     = {-32.0f, -64.0f};
IsoOffset isoESuicideOffset   = {-32.0f, -64.0f};
IsoOffset isoEScientistOffset = {-32.0f, -64.0f};
IsoOffset IsoETankOffset      = {-32.0f, -64.0f};
IsoOffset isoEPirateOffset    = {-32.0f, -64.0f};
IsoOffset isoERabbitOffset    = {-32.0f, -64.0f};

IsoOffset isoSArcherOffset          = {-32.0f, -64.0f};
IsoOffset isoSArcherPoisonerOffset  = {-32.0f, -64.0f};
IsoOffset isoSArcherRetarderOffset  = {-32.0f, -64.0f};
IsoOffset isoSShamanHealerOffset    = {-32.0f, -64.0f};
IsoOffset isoSShamanMightOffset     = {-32.0f, -64.0f};
IsoOffset isoSShamanHasteOffset     = {-32.0f, -64.0f};
IsoOffset isoSWarriorOffset         = {-32.0f, -64.0f};
IsoOffset isoSWarriorBerserkOffset  = {-32.0f, -64.0f};
IsoOffset isoSWarriorTankOffset     = {-32.0f, -64.0f};
IsoOffset isoSBowlerConfuserOffset  = {-32.0f, -64.0f};
IsoOffset isoSBowlerRooterOffset    = {-32.0f, -64.0f};
IsoOffset isoSBowlerStunerOffset    = {-32.0f, -64.0f};

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
Skill_Poison				Skill_Att_Poison[3];
Skill_Slow					Skill_Att_Slow[3];
Skill_Stun					Skill_Att_Stun[3];
Skill_Root					Skill_Att_Root[3];
Skill_Confusion				Skill_Att_Confusion[3];
Skill_Explode				Skill_Att_Explode[3];
Skill_PoisonOnDeath			Skill_Att_PoisonOnDeath[3];
Skill_Heal					Skill_Att_Heal[3];
Skill_Might					Skill_Att_Might[3];
Skill_Haste					Skill_Att_Haste[3];

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

/*
//DEMO
UnitUpgrades ArcherBaseUpgrades     = {1};
UnitUpgrades ArcherPoisonerUpgrades = {1};
UnitUpgrades ArcherRetarderUpgrades = {1};
UnitUpgrades BowlerStunerUpgrades   = {1};
UnitUpgrades BowlerRooterUpgrades   = {1};
UnitUpgrades BowlerConfuserUpgrades = {1};
UnitUpgrades WarriorBaseUpgrades    = {1}; 
UnitUpgrades WarriorTankUpgrades    = {1};
UnitUpgrades WarriorBerserkUpgrades = {1};
UnitUpgrades ShamanMightUpgrades    = {1};
UnitUpgrades ShamanHasteUpgrades    = {1};
UnitUpgrades ShamanHealerUpgrades   = {1};
*/

//Max Stars that can be collected through the game
int Max_Stars = 72;
//Stars used in the Selknam Store
int Used_Stars = 0;
//Stars that player currently has for each level.
int Stars_By_Level[24] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
	                      0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
						  0, 0, 0, 0};

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

CIwGxFont* font_komikadisplaybold25;
CIwGxFont* font_komikadisplay11;
CIwGxFont* font_komikadisplay28;
CIwGxFont* font_komikadisplay14;
CIwGxFont* font_komikadisplay18;
CIwGxFont* font_komikadisplaybold18;
CIwGxFont* font_komikadisplay22;
CIwGxFont* font_komikadisplaybold22;
CIwGxFont* font_komikadisplaybold48;

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

CIwSoundSpec* Arrow_Shooting;
CIwSoundSpec* FX_Click;
CIwSoundSpec* FX_Explosion;
CIwSoundSpec* FX_Knife_Sword_01;
CIwSoundSpec* FX_RollOver_02;
CIwSoundSpec* FX_Shot_01;
CIwSoundSpec* FX_Star_Gained_02;
CIwSoundSpec* FX_Silence;
CIwSoundSpec* FX_MetalImpact;
CIwSoundSpec* FX_MetalImpact2;
CIwSoundSpec* FX_Coins;
CIwSoundSpec* FX_Heal;
CIwSoundSpec* FX_Bubbles;

bool bExtraMapMode = false;
bool bFxSoundMuted = true;

int8 accumulated_frames = 0;

int8 fx_sound_limit[MAX_FX_SOUNDS][2];

bool bResetStoreStats;
bool bPausedStore;
bool bFirstMapWasPlayed = false;
bool bBlockKeyboardOnce = false;

bool bBlockOnQuit = false;

/*******TUTORIAL*******/
Tutorial* tutorial = NULL;
bool bShowTutorialLevel1 = true;
bool bShowTutorialStars = true;
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
bool bMapSelectionAudioPlaying = false;
Music_Name LastMusicStacked = Music_None;
Music_Name CurrentMusicPlaying = Music_None;
int LastMusicPosition = 0;
/*************************/

TextureData buffBarTextureData;

//Texture Data
TextureData TextureDataList[256];
int currentTextureId = 0;

CIwTexture* atlas_common_0 = NULL;
CIwTexture* atlas_common_1 = NULL;
CIwTexture* atlas_common_2 = NULL;
CIwTexture* atlas_common_trial = NULL;
CIwTexture* atlas_menu_01_0   = NULL;
CIwTexture* atlas_menu_01_1   = NULL;
CIwTexture* atlas_menu_02_0   = NULL;
CIwTexture* atlas_gamebackground_0 = NULL;
CIwTexture* atlas_worldmap = NULL;
CIwTexture* atlas_enemy_01_1 = NULL;
CIwTexture* atlas_enemy_01_2 = NULL;
CIwTexture* atlas_enemy_02_1 = NULL;
CIwTexture* atlas_enemy_02_2 = NULL;
CIwTexture* atlas_enemy_03_1 = NULL;
CIwTexture* atlas_enemy_04_1 = NULL;
CIwTexture* atlas_enemy_04_2 = NULL;
CIwTexture* atlas_enemy_05_1 = NULL;
CIwTexture* atlas_enemy_06_1 = NULL;
CIwTexture* atlas_enemy_07_1 = NULL;
CIwTexture* atlas_enemy_07_2 = NULL;
CIwTexture* atlas_selknam_archer_01_1 = NULL;
CIwTexture* atlas_selknam_archer_01_2 = NULL;
CIwTexture* atlas_selknam_archer_02_1 = NULL;
CIwTexture* atlas_selknam_archer_02_2 = NULL;
CIwTexture* atlas_selknam_archer_03_1 = NULL;
CIwTexture* atlas_selknam_archer_03_2 = NULL;
CIwTexture* atlas_selknam_melee_01_1 = NULL;
CIwTexture* atlas_selknam_melee_01_2 = NULL;
CIwTexture* atlas_selknam_melee_02_1 = NULL;
CIwTexture* atlas_selknam_melee_02_2 = NULL;
CIwTexture* atlas_selknam_melee_03_1 = NULL;
CIwTexture* atlas_selknam_melee_03_2 = NULL;
CIwTexture* atlas_selknam_mage_01_1 = NULL;
CIwTexture* atlas_selknam_mage_01_2 = NULL;
CIwTexture* atlas_selknam_mage_02_1 = NULL;
CIwTexture* atlas_selknam_mage_02_2 = NULL;
CIwTexture* atlas_selknam_mage_03_1 = NULL;
CIwTexture* atlas_selknam_mage_03_2 = NULL;
CIwTexture* atlas_selknam_bowler_01_1 = NULL;
CIwTexture* atlas_selknam_bowler_01_2 = NULL;
CIwTexture* atlas_selknam_bowler_02_1 = NULL;
CIwTexture* atlas_selknam_bowler_02_2 = NULL;
CIwTexture* atlas_selknam_bowler_03_1 = NULL;
CIwTexture* atlas_selknam_bowler_03_2 = NULL;
CIwTexture* atlas_terrain_0 = NULL;
CIwTexture* atlas_props_0 = NULL;
CIwTexture* atlas_props_1 = NULL;
CIwTexture* atlas_props_2 = NULL;
CIwTexture* atlas_fx_0 = NULL;
CIwTexture* atlas_fx_1 = NULL;
CIwTexture* atlas_fx_2 = NULL;

//SCREEN FIX
float tileMapWidthImage = 64.0f;
float tileMapHeightImage = 48.0f;
float mapXOffset = 64;
float mapYOffset = 80;
//Isometric Pos Helpers
float tileMapWidthBase = 64.0f;
float tileMapHeightBase = 32.0f;
float tileDiffLayer = 16.0f;
float isoFormulaWValue = tileMapWidthBase * 0.5f;
float isoFormulaHValue = tileMapHeightBase * 0.5f;
float isoFormulaAValue = isoFormulaWValue * isoFormulaHValue * 2;
float isoFormulaInvAValue = 1.0f / isoFormulaAValue;
//place of first tile
float isoFormulaXOffset;
float isoFormulaYOffset;


//Array of groups
int commonResourcesNum = 22;
const char* commonResources[22];

int totalResourcesLoadedNum = 0;
char totalResourcesLoaded[256][256];