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

#ifndef _COMMON_H_
#define _COMMON_H_

#include "tutorial.h"
#include "IwSound.h"
#include "fadescreen.h"
#include "mm_popup.h"
#include "dialogcontroller.h"
#include "achievementshelper.h"
#include "leaderboardshelper.h"
#if defined DESKTOPPLATFORM
#include "s3eWindow.h"
#endif

class SaveDataHelper;
class AlmanacBase;
class SurvivalModeConfig;
class CaptainBoss;
class SettingsMenu;

#ifdef DESKTOPPLATFORM
#ifdef STEAMREQUIRED
//STEAM
class CSteamAchievements;
class CSteamLeaderboards;
#endif
#endif

// updates per second
#define UPS 60

#define MAX_TERRAIN_TILES 512
#define MAX_ISO_PROPS 128
#define MAX_ISO_ENEMIES 192
#define MAX_ISO_SELKNAM 192
#define MAX_ASTAROBJECTIVES 10
#define MAX_TILESROW 18
#define ASTAR_HDISTANCE 10
#define ASTAR_LISTSMAX 324
#define ASTAR_MOVECOST 100
#define MAX_INITNODES 10
#define MAX_SKILLS 11
#define MAX_MAPS 25
#define MAX_MAPS_CAMPAIGN 24
#define MAX_EXTRA_MAPS 999
#define MAX_ANIMATION 15
#define MAX_ISO_PROJECTILE 128
#define ASTAR_WALKABLETILECOST 60
#define ASTAR_WALKABLEGREENTILECOST 450
#define ASTAR_ENEMYONTILECOST 1000
#define MAX_ENEMIES 9
#define GAMEPLAY_NUM_WRAPPER_RECTS 2

//Music and FXSounds
#define ENABLE_SOUND_BUTTONS 1
#define MAX_FX_SOUNDS 35

//Hacks
//When mouse is not active (not rendering) and user clicks, enter button is pressed instead.
#define ENABLE_MOUSE_HACK 1

//Debug
//Show a text with the code of the last key pressed.
#define LAST_KEY_PRESSED 0

//Languages
typedef enum
{
	GAMELANG_INT,
	GAMELANG_ESN
} GameLang;

typedef enum
{
	Music_None,	   //No audio playing.
	Music_Loose,     //sd_fanfare_lose_theme.mp3
	Music_Win,       //sd_fanfare_win_theme.mp3
	Music_Forest_Ingame_01, //sd_ingame_forest_01_theme_loop.mp3
	Music_Forest_Ingame_02, //sd_ingame_forest_02_theme_loop.mp3
	Music_Mountain_Ingame_01, //sd_ingame_mountain_01_theme_loop.mp3
	Music_Mountain_Ingame_02, //sd_ingame_mountain_02_theme_loop.mp3
	Music_IceLand_Ingame_01, //sd_ingame_iceland_01_theme_loop.mp3
	Music_IceLand_Ingame_02, //sd_ingame_iceland_02_theme_loop.mp3
	Music_Fireland_Ingame_01, //sd_ingame_fireland_01_theme_loop.mp3
	Music_Fireland_Ingame_02, //sd_ingame_fireland_02_theme_loop.mp3
	Music_MainMenu,  //sd_mainmenu_theme_loop.mp3
	Music_Store,     //sd_store_theme_loop.mp3
	Music_Boss,     //sd_boss_base.mp3
	Music_Ending,     //sd_ending_theme.mp3
} Music_Name;

//Game Heights Supported
typedef enum
{
	HEIGHT_768, //iOS iPad
	HEIGHT_540,
	HEIGHT_600,
	HEIGHT_640, //iOS iPhone
	HEIGHT_720,
	HEIGHT_800
} GameHeight;

//Game Obj Positions
typedef enum
{
	POS_Left,
	POS_Center,
	POS_Right
} GameObjPos;

/******GAME OPTIONS*******/
extern bool bIsFullVersion;
extern bool bNoMusic;
extern bool bSteamAvailable;
extern bool bUnitMenuAttachedToMouseWhenOpened;
extern bool bIsXbox360ControllerEnabled;
extern bool bUseHDImages;
extern bool bLoadAllResources;

//vendors check
extern bool bIwBillingIosAvailable;
//kiip check
extern bool bKiipAvailable;
//chartboost check
extern bool bChartBoostAvailable;
//iAds check
extern bool biAdAvailable;
extern int showCharBoostOnMainMenuCount;
//remove ads productId
extern const char* removeAds_ProductID;

extern const char* removeAds_Data_ProductID;
extern const char* removeAds_Data_Title;
extern const char* removeAds_Data_Desc;
extern const char* removeAds_Data_Price;

//Audio Background Available
extern bool biOSBackgroundAudioAvailable;

extern bool bErrorOnLastPurchase;
extern bool bItemRestoredSuccess;
extern bool bItemPurchasedSuccess;
extern bool bItemRefundSuccess;


//gamecenter vars
extern bool bGameCenterAvailable;
extern bool bGameCenterAuthenticated;
extern char gameCenterUserName[256];

extern s3eDeviceOSID currentDeviceOS;
extern s3eDeviceLanguage currentDeviceLanguage;
extern const char* currentDeviceID;
extern uint32 currentDisplayWidth;
extern uint32 currentDisplayHeight;
extern uint32 currentScreenWidth;
extern uint32 currentScreenHeight;
extern uint32 currentDeviceWidth;
extern uint32 currentDeviceHeight;
extern uint32 currentSurfaceWidth;
extern uint32 currentSurfaceHeight;
extern uint32 currentGameWidth;
extern uint32 currentGameHeight;
extern GameHeight gameHeight;
extern uint32 srcGameWidth;
extern uint32 srcGameHeight;
extern GameLang gameLang;

extern int saveDataVersion;
/*************************/

extern bool bShowTrialVersionFinal;

//Version
extern string version_number;
extern string version_date;
extern bool bWaterMark;

extern Music_Name LastMusicStacked;

extern bool bIsAudioPaused;
extern bool bIsFxPaused;

//Mouse configuration
extern int32 iLastMouseX;
extern int32 iLastMouseY;

extern int xMouse;
extern int yMouse;

//Button Speed Mechanics.
extern float normalGameSpeed;
extern float doubleGameSpeed;
extern float tripleGameSpeed;
extern float currentGameSpeedValue;
//enums
typedef enum
{
	WTIMER_NORMAL,
	WTIMER_X2,
	WTIMER_X3,
} TimerSpeed;
extern TimerSpeed waveTimerSpeed;

//Loading a map: When a particular map is selected, a black screen appears for TIME_BLACKSCREEN miliseconds
//After that, a Fade Out is executed at a speed of FADE_OUT_SPEED (value between 0 - 255)
#define TIME_BLACKSCREEN 250 
#define FADE_OUT_SCREEN 30

//Delay between the button to move the map pointer is pressed and the 
//automatic motion is activated. (miliseconds)
#define DELAY_BEFORE_MOUSE_BEGIN_TO_MOVE 200 

//Confusion Skill: Move to North, West, East or South and wait there for CONFUSION_STOP_TIME miliseconds. Repeat this process CONFUSION_TIMES times.
#define CONFUSION_STOP_TIME 1000
#define CONFUSION_TIMES 4

#define NUM_PANELS 8

typedef enum
{
	PannelType_A,
	PannelType_B,
	PannelType_C,
	PannelType_D,
	PannelType_E,
	PannelType_F,
	PannelType_G,
	PannelType_H
} PannelType;

#define NUM_BARS 2

typedef enum
{
	BarType_A,
	BarType_B
} BarType;

#define NUM_XBOX360_BUTTONS 14

typedef enum 
{
	XBOX360_Button_None,
	XBOX360_Button_A,
	XBOX360_Button_B,
	XBOX360_Button_X,
	XBOX360_Button_Y,
	XBOX360_Button_Down,
	XBOX360_Button_Up,
	XBOX360_Button_Left,
	XBOX360_Button_Right,
	XBOX360_Button_Directional,
	XBOX360_Button_RB,
	XBOX360_Button_LB,
	XBOX360_Button_Select,
	XBOX360_Button_Start,
} XBOX360_Button;

typedef enum
{
	Controller_XBOX360,
	Controller_MOUSE,
	Controller_KEYBOARD
} ControllerType;

//enums
typedef enum
{
	MIRROR_HORIZONTAL,
	MIRROR_VERTICAL,
	MIRROR_DIAGONAL
} MirrorImage;

typedef enum
{
	NO_ROTATE,
	ROTATE_270,
	ROTATE_180
} RotateImage;

typedef enum 
{
	ALMANAC_NONE,
	ALMANAC_MENU,
	ALMANAC_SELKNAM,
	ALMANAC_ENEMY
} almanacState;

typedef enum
{
	GS_None,
	GS_SplashScreen,
	GS_MainMenu,
	GS_MapSelection,
	GS_Game,
	GS_Retry,
	GS_ExtraMapSelection,
} GameState;

#define MAINMENUSTATE_ENUM_LENGTH 9

typedef enum
{
	GSM_None,
	GSM_MainMenu,
	GSM_HowToPlay,
	GSM_Options,
	GSM_Credits,
	GSM_TrialVersion,
	GSM_FinalScreen, //Menu that appears when the game is completed.
	GSM_Almanac,
	GSM_Play,
	GSM_Leaderboards,
	GSM_Settings
} MainMenuState;

typedef enum
{
	GAMEMODE_CAMPAIGN,
	GAMEMODE_SURVIVAL,
	GAMEMODE_VSEUROPE
} GameMode;

typedef enum
{
	GSP_None,
	GSP_NewGame,
	GSP_QuitGame,
	GSP_QuitGameLooseEnding,
	GSP_Retry,
	GSP_ReturnToMainMenu,
	GSP_NewGame_Error,
	GSP_NewGame_SaveData,
	GSP_MapSelection,
	GSP_ResetStore,
	GSP_SURVIVALDISABLED,
	GSP_SOCIALNETWORKPUBLISH,
	GSP_SOCIALNETWORKPUBLISHFAIL,
	GSP_LEADERBOARD_ERROR,
	GSP_NOINTERNET
} PopUpState;

typedef enum
{
	GSG_None,
	GSG_Cinematic,
	GSG_Defending,
	GSG_Attack,
	GSG_Win,
	GSG_Loose,
	GSG_DynamicCredits
} InGameState;

typedef enum
{
	GSGP_None,
	GSGP_MainMenuIG,
	GSGP_AlmanacIG,
	GSGP_HowToPlayIG,
} InGamePausedState;

typedef enum
{
	GSMIG_None,
	GSMIG_MainMenuIG,
	GSMIG_AlmanacIG,
	GSMIG_Options,
} MainMenuIGState;

typedef enum
{
	MS_None,
	MS_MapSelection,
	MS_Store,
} MapSelectionState;

typedef enum
{
	ENEMY_None,
	ENEMY_Soldier,
	ENEMY_Varon,
	ENEMY_Convict,
	ENEMY_Scientist,
	ENEMY_Tank,
	ENEMY_Rabbit,
	ENEMY_Pirate,
	ENEMY_Captain
} IsoEnemyType;

typedef enum
{
	SELKNAM_None,
	SELKNAM_Archer_Base,
	SELKNAM_Archer_Poisoner,
	SELKNAM_Archer_Retarder,
	SELKNAM_Warrior_Base,
	SELKNAM_Warrior_Tank,
	SELKNAM_Warrior_Berserk,
	SELKNAM_Shaman_Healer,
	SELKNAM_Shaman_Might,
	SELKNAM_Shaman_Haste,
	SELKNAM_Bowler_Stuner,
	SELKNAM_Bowler_Rooter,
	SELKNAM_Bowler_Confuser,
} IsoSelknamType;

typedef enum
{
	DIR_None,
	DIR_North,
	DIR_NorthEast,
	DIR_East,
	DIR_SouthEast,
	DIR_South,
	DIR_SouthWest,
	DIR_West,
	DIR_NorthWest,
} IsoDirection;

typedef enum
{
	ANIM_None,
	ANIM_Idle,
	ANIM_Walk,
	ANIM_Attack,
	ANIM_Death,
} IsoAnim;

typedef enum
{
	Skill_Type_None,
	Skill_Type_Heal, //skill curacion
	Skill_Type_Might, //buff fuerza
	Skill_Type_Haste, //buff velocidad ataque
	Skill_Type_Stun, //debuff stun
	Skill_Type_Poison, //debuff veneno
	Skill_Type_Slow, //debuff alentar
	Skill_Type_Root, //debuff dejar parado al enemigo
	Skill_Type_Confusion, //debuff hacer que el enemigo valla a cualquier parte
	Skill_Type_PoisonOnDeath, //debuff veneno al morir
	Skill_Type_Explode, //explotar al morir
	Skill_Type_CountdownDeath
} Skill_Type;

typedef enum
{
	BHS_None,
	BHS_Range_SingleFocus,
	BHS_Range_NearFocus,
	BHS_Melee_Defensive,
	BHS_Melee_Ofensive_Defensive,
	BHS_SupportActive,
	BHS_SupportPasive,
} Pawn_SelknamBehavior;

typedef enum
{
	BHE_None,
	BHE_Range_SingleFocus,
	BHE_Melee_Ofensive_Defensive,
	BHE_Suicidal,
	BHE_Objective,
} Pawn_EnemyBehavior;

typedef enum
{
	EMPTY_GAME,
	NOT_EMPTY_GAME,
} NewGame;

typedef enum
{
	Projectile_None,
	Projectile_Arrow,
	Projectile_Rock,
	Projectile_Bullet,
	Projectile_Cannon,
} IsoProjectileType;

typedef enum
{
	NOT_PLAYABLE,
	PLAYABLE,
	CURRENT_PLAYABLE,
} MapStatus;

typedef enum
{
	NORMAL,
	//HALF,
	MUTE,
} SoundStatus;

typedef enum
{
	ANIM_SELKNAM_Archer_Base,
	ANIM_SELKNAM_Archer_Poisoner,
	ANIM_SELKNAM_Archer_Retarder,
	ANIM_SELKNAM_Warrior_Base,
	ANIM_SELKNAM_Warrior_Tank,
	ANIM_SELKNAM_Warrior_Berserk,
	ANIM_SELKNAM_Shaman_Healer,
	ANIM_SELKNAM_Shaman_Might,
	ANIM_SELKNAM_Shaman_Haste,
	ANIM_SELKNAM_Bowler_Stuner,
	ANIM_SELKNAM_Bowler_Rooter,
	ANIM_SELKNAM_Bowler_Confuser,
	ANIM_ENEMY_Soldier,
	ANIM_ENEMY_Varon,
	ANIM_ENEMY_Convict,
	ANIM_ENEMY_Scientist,
	ANIM_ENEMY_Tank,
	ANIM_ENEMY_Rabbit,
	ANIM_ENEMY_Pirate,
} IsoPawnAnim;

typedef enum
{
	FX_Sound_ARROWSHOOTING,
	FX_Sound_CLICK,
	FX_Sound_EXPLOSION,
	FX_Sound_KNIFESWORD01,
	FX_Sound_ROLLOVER02,
	FX_Sound_SHOT01,
	FX_Sound_STARGAINED02,
	FX_Sound_METALIMPACT01,
	FX_Sound_METALIMPACT02,
	FX_Sound_COINS,
	FX_Sound_HEAL,
	FX_Sound_BUBBLES
} FxSound;

typedef enum
{
	ATLAS_NONE,
	ATLAS_BKG_GAMEPLAY,
	ATLAS_BKG_MENU_FOREGROUND,
	ATLAS_BKG_MENU_OTHERS,
	ATLAS_BKG_MENU_SKY,
	ATLAS_BKG_WORLDMAP,
	ATLAS_BKG_SMALL_0,
	ATLAS_BKG_SMALL_1,
	ATLAS_COMMON_TEXTURES_0,
	ATLAS_COMMON_TEXTURES_1,
	ATLAS_COMMON_TEXTURES_2,
	ATLAS_COMMON_TEXTURES_3,
	ATLAS_COMMON_TEXTURES_4,
	ATLAS_SMALL_TEXTURES,
	ATLAS_COMMON_TRIAL_0,
	ATLAS_FX_0,
	ATLAS_FX_1,
	ATLAS_FX_2,
	ATLAS_PROPS_0,
	ATLAS_PROPS_1,
	ATLAS_PROPS_2,
	ATLAS_TERRAIN_0,
	ATLAS_TERRAIN_1,
	ATLAS_HD_SELKNAM_ARCHER_1,
	ATLAS_HD_SELKNAM_ARCHER_2,
	ATLAS_HD_SELKNAM_ARCHER_3,
	ATLAS_HD_SELKNAM_BOWLER_1_01,
	ATLAS_HD_SELKNAM_BOWLER_1_02,
	ATLAS_HD_SELKNAM_BOWLER_2_01,
	ATLAS_HD_SELKNAM_BOWLER_2_02,
	ATLAS_HD_SELKNAM_BOWLER_3_01,
	ATLAS_HD_SELKNAM_BOWLER_3_02,
	ATLAS_HD_SELKNAM_MAGE_1,
	ATLAS_HD_SELKNAM_MAGE_2,
	ATLAS_HD_SELKNAM_MAGE_3,
	ATLAS_HD_SELKNAM_MELEE_1,
	ATLAS_HD_SELKNAM_MELEE_2,
	ATLAS_HD_SELKNAM_MELEE_3,
	ATLAS_HD_ENEMY_1,
	ATLAS_HD_ENEMY_2,
	ATLAS_HD_ENEMY_3,
	ATLAS_HD_ENEMY_4_01,
	ATLAS_HD_ENEMY_4_02,
	ATLAS_HD_ENEMY_5,
	ATLAS_HD_ENEMY_6,
	ATLAS_HD_ENEMY_7,
	ATLAS_SELKNAM_ARCHER_01_1,
	ATLAS_SELKNAM_ARCHER_01_2,
	ATLAS_SELKNAM_ARCHER_02_1,
	ATLAS_SELKNAM_ARCHER_02_2,
	ATLAS_SELKNAM_ARCHER_03_1,
	ATLAS_SELKNAM_ARCHER_03_2,
	ATLAS_SELKNAM_MELEE_01_1,
	ATLAS_SELKNAM_MELEE_01_2,
	ATLAS_SELKNAM_MELEE_02_1,
	ATLAS_SELKNAM_MELEE_02_2,
	ATLAS_SELKNAM_MELEE_03_1,
	ATLAS_SELKNAM_MELEE_03_2,
	ATLAS_SELKNAM_BOWLER_01_1,
	ATLAS_SELKNAM_BOWLER_01_2,
	ATLAS_SELKNAM_BOWLER_02_1,
	ATLAS_SELKNAM_BOWLER_02_2,
	ATLAS_SELKNAM_BOWLER_03_1,
	ATLAS_SELKNAM_BOWLER_03_2,
	ATLAS_SELKNAM_MAGE_01_1,
	ATLAS_SELKNAM_MAGE_01_2,
	ATLAS_SELKNAM_MAGE_02_1,
	ATLAS_SELKNAM_MAGE_02_2,
	ATLAS_SELKNAM_MAGE_03_1,
	ATLAS_SELKNAM_MAGE_03_2,
	ATLAS_ENEMY_01_1,
	ATLAS_ENEMY_01_2,
	ATLAS_ENEMY_02_1,
	ATLAS_ENEMY_02_2,
	ATLAS_ENEMY_03_1,
	ATLAS_ENEMY_04_1,
	ATLAS_ENEMY_04_2,
	ATLAS_ENEMY_05_1,
	ATLAS_ENEMY_06_1,
	ATLAS_ENEMY_07_1,
	ATLAS_ENEMY_07_2,
	ATLAS_BOSS,
	ATLAS_OTHERS,
} AtlasName;

//extern SoundManager* FXSoundManager;

//predefined pawns atributes
typedef struct
{
	//Selknam Behavior (none on enemies)
	Pawn_SelknamBehavior sBehavior;
	//Enemy Behavior (none on selknam)
	Pawn_EnemyBehavior eBehavior;
	//current level
	int level;
	//current health
	int health;
	//damage to inflict
	int damage;
	//speed multiplier (useful to add as multiplier on velocity), normal_value:10
	int speed;
	//delay multiplier to apply on attack animation, en decimas de segundos.
	int attackspeed;
	//delay cooldown between two consecutive attacks, en decimas de segundos.
	int attack_cooldown;
	//range of unit. (attack on melees)
	int range;
	//range alt of unit. (used on melees, movement)
	int rangealt;
	//cost of unit.
	int cost;
	//cost to move a unit
	int move_cost;
	//when a unit is sold, mp_coins increase in sold_value
	int sold_value;
	//coins gained per kill enemy
	int coins_perkill;
	//delay multiplier to apply on magic skills (heal), en decimas de segundos.
	int casting_speed;
	//delay cooldown between two consecutive magic skills (heal), en decimas de segundos.
	int cast_cooldown;

	Skill_Type skill;

}PawnAttributes;
//current pawns atributes, need to be setted by predefined atributes 
//on isoenemy and isoselknam

///SKILLS
typedef struct  
{
	int level;
	int damage_per_interval;
	int interval_time; //en decimas de segundos
	int num_intervals;
} Skill_Poison;

typedef struct
{
	int level;
	int debuff_time; //en decimas de segundos
	int debuff_slow_amount;
} Skill_Slow;

typedef struct
{
	int level;
	int debuff_time; //en decimas de segundos
} Skill_Stun;

typedef struct
{
	int level;
	int debuff_time; //en decimas de segundos
} Skill_Root;

typedef struct
{
	int level;
	int debuff_time; //en decimas de segundos
} Skill_Confusion;

typedef struct  
{
	int level;
	int range;
	int damage;
} Skill_Explode;

typedef struct  
{
	int level;
	int range;
	int damage_per_interval;
	int interval_time; //en decimas de segundos
	int num_intervals;
} Skill_PoisonOnDeath;

typedef struct
{
	int level;
	int health_amount;
} Skill_Heal;

typedef struct  
{
	int level;
	int buff_might_amount;
} Skill_Might;

typedef struct
{
	int level;
	int buff_haste_amount;
} Skill_Haste;

///END SKILLS

//offset to render iso objects, useful to retrieve real centers.
typedef struct
{
	float offsetX;
	float offsetY;
}IsoOffset;

typedef struct  
{
	int level;
} UnitUpgrades;

//Texture helper
typedef struct  
{
	char textureFileName[64];
	char atlasFileName[64];
	int atlasWidth;
	int atlasHeight;
	int textureXPos;
	int textureYPos;
	int textureWidth;
	int textureHeight;
	bool textureRotated;
} TextureData;

typedef struct {
	float minX, minY, maxX, maxY;
} UVInfo;

class RenderData {
public:
	int width, height;
	UVInfo uvInfo;
	bool bRotate;
	AtlasName atlasName;
};

typedef struct {
	RenderData RollOn;
	RenderData RollOut;
	RenderData Pressed;
	RenderData Inactive;
} ButtonRenderState;

class ButtonRenderData {
public:
	int numStates;
	ButtonRenderState state[4];
};

typedef struct  
{
	int level;
	int totalScore;
} LBCampaignScore;

extern GameState gameState;
extern GameState lastGameState;

extern MainMenuState mainMenuState;
extern MainMenuState lastMainMenuState;

extern PopUpState popUpState;
extern PopUpState lastPopUpState;

extern InGameState inGameState;
extern InGameState lastInGameState;

extern InGamePausedState inGamePausedState;
extern InGamePausedState lastInGamePausedState;

extern MainMenuIGState mainMenuIGState;
extern MainMenuIGState lastMainMenuIGState;

extern MapSelectionState mapSelectionState;
extern MapSelectionState lastMapSelectionState;

extern XBOX360_Button currentButtonPressed;
extern XBOX360_Button lastButtonPressed;

extern FadeScreen *fade;

extern PopUp *popup;

extern bool bPause;
extern bool bBlockGUI;
extern int currentMap;

extern int totalScoreTmp;
extern string totalSurvivalScoreTmp;

extern IsoOffset isoTerrainOffset;
extern IsoOffset isoESoldierOffset;
extern IsoOffset isoEScientistOffset;
extern IsoOffset IsoETankOffset;
extern IsoOffset isoEVaronOffset;
extern IsoOffset isoESuicideOffset;
extern IsoOffset isoEPirateOffset;
extern IsoOffset isoERabbitOffset;
extern IsoOffset isoSArcherOffset;
extern IsoOffset isoSArcherPoisonerOffset;
extern IsoOffset isoSArcherRetarderOffset;
extern IsoOffset isoSShamanHealerOffset;
extern IsoOffset isoSShamanMightOffset;
extern IsoOffset isoSShamanHasteOffset;
extern IsoOffset isoSWarriorOffset;
extern IsoOffset isoSWarriorBerserkOffset;
extern IsoOffset isoSWarriorTankOffset;
extern IsoOffset isoSBowlerConfuserOffset;
extern IsoOffset isoSBowlerRooterOffset;
extern IsoOffset isoSBowlerStunerOffset;

//SELKNAMS
//archers
extern PawnAttributes SelknamAtt_Archer_Base[3];
extern PawnAttributes SelknamAtt_Archer_Poisoner[3];
extern PawnAttributes SelknamAtt_Archer_Retarder[3];
//warriors
extern PawnAttributes SelknamAtt_Warrior_Base[3];
extern PawnAttributes SelknamAtt_Warrior_Tank[3];
extern PawnAttributes SelknamAtt_Warrior_Berserk[3];
//shamans
extern PawnAttributes SelknamAtt_Shaman_Healer[3];
extern PawnAttributes SelknamAtt_Shaman_Might[3];
extern PawnAttributes SelknamAtt_Shaman_Haste[3];
//bowler
extern PawnAttributes SelknamAtt_Bowler_Stuner[3];
extern PawnAttributes SelknamAtt_Bowler_Rooter[3];
extern PawnAttributes SelknamAtt_Bowler_Confuser[3];

//ENEMIES
extern PawnAttributes Enemy_Att_Soldier[3];
extern PawnAttributes Enemy_Att_Varon[3];
extern PawnAttributes Enemy_Att_Convict[3];
extern PawnAttributes Enemy_Att_Scientist[3];
extern PawnAttributes Enemy_Att_Tank[3];
extern PawnAttributes Enemy_Att_Rabbit[3];
extern PawnAttributes Enemy_Att_Pirate[3];

#define NUM_SKILLS 4

//SKILLS
extern Skill_Poison				    Skill_Att_Poison[NUM_SKILLS];
extern Skill_Slow					Skill_Att_Slow[NUM_SKILLS];
extern Skill_Stun					Skill_Att_Stun[NUM_SKILLS];
extern Skill_Root					Skill_Att_Root[NUM_SKILLS];
extern Skill_Confusion				Skill_Att_Confusion[NUM_SKILLS];
extern Skill_Explode				Skill_Att_Explode[NUM_SKILLS];
extern Skill_PoisonOnDeath			Skill_Att_PoisonOnDeath[NUM_SKILLS];
extern Skill_Heal					Skill_Att_Heal[NUM_SKILLS];
extern Skill_Might					Skill_Att_Might[NUM_SKILLS];
extern Skill_Haste					Skill_Att_Haste[NUM_SKILLS];

//Selknam Store (0: Not Purchased yet)
extern UnitUpgrades ArcherBaseUpgrades;
extern UnitUpgrades ArcherPoisonerUpgrades;
extern UnitUpgrades ArcherRetarderUpgrades;
extern UnitUpgrades BowlerStunerUpgrades;
extern UnitUpgrades BowlerRooterUpgrades;
extern UnitUpgrades BowlerConfuserUpgrades;
extern UnitUpgrades WarriorBaseUpgrades;
extern UnitUpgrades WarriorTankUpgrades;
extern UnitUpgrades WarriorBerserkUpgrades;
extern UnitUpgrades ShamanMightUpgrades;
extern UnitUpgrades ShamanHasteUpgrades;
extern UnitUpgrades ShamanHealerUpgrades;

extern int Max_Stars;
extern int Used_Stars;
extern int Stars_By_Level[MAX_MAPS];
extern int Stars_By_ExtraLevel[MAX_EXTRA_MAPS];

extern int archer_base_cost[3];
extern int archer_poisoner_cost[3];
extern int archer_retarder_cost[3];
extern int warrior_base_cost[3];
extern int warrior_tank_cost[3];
extern int warrior_berserker_cost[3];
extern int mage_might_cost[3];
extern int mage_heal_cost[3];
extern int mage_haste_cost[3];
extern int bowler_stuner_cost[3];
extern int bowler_rooter_cost[3];
extern int bowler_confuser_cost[3];

//Skills
extern Skill_Poison Skill_Poison_Att;
extern Skill_Slow Skill_Slow_Att;
extern Skill_Heal Skill_Heal_Att;
extern Skill_Might Skill_Might_Att;
extern Skill_Haste Skill_Haste_Att;

extern NewGame NewGame_State;
extern MapStatus MapStatus_State[MAX_MAPS];

extern SoundStatus FX_Sound_State;
extern SoundStatus Music_Sound_State;

extern int fade_menu;

//ANIMATION CONTROLLER [x][y][z][Max_Animation]
//[x], cada clase.
//[y], cada tipo de animacion, Idle, Walk, Attack, Die
//[z], cada una de las direcciones.
//Orden de las direcciones
//north, northeast, east, southeast, south, southwest, west, northwest

//Pawns
extern int16 Anim_Pawns[19][4][9][MAX_ANIMATION];

//This fixes a bug when the user press the quit game more than once.
extern bool bQuittingGame;

// Get the sound spec for our silly_sound sound effect
extern CIwSoundSpec* FX_Arrow_Shooting_01;
extern CIwSoundSpec* FX_Arrow_Shooting_02;
extern CIwSoundSpec* FX_Click_01;
extern CIwSoundSpec* FX_Click_02;
extern CIwSoundSpec* FX_Explosion;
extern CIwSoundSpec* FX_Knife_Sword_01;
extern CIwSoundSpec* FX_Knife_Sword_02;
extern CIwSoundSpec* FX_Knife_Sword_03;
extern CIwSoundSpec* FX_RollOver_02;
extern CIwSoundSpec* FX_Shot_01;
extern CIwSoundSpec* FX_Star_Gained_02;
extern CIwSoundSpec* FX_Silence;
extern CIwSoundSpec* FX_MetalImpact_01;
extern CIwSoundSpec* FX_MetalImpact_02;
extern CIwSoundSpec* FX_Coins;
extern CIwSoundSpec* FX_BuffOn;
extern CIwSoundSpec* FX_Heal;
extern CIwSoundSpec* FX_Bubbles;
extern CIwSoundSpec* FX_Punch_01;
extern CIwSoundSpec* FX_Punch_02;
extern CIwSoundSpec* FX_Punch_03;
extern CIwSoundSpec* FX_Punch_04;
extern CIwSoundSpec* FX_Punch_05;
extern CIwSoundSpec* FX_Sell_Unit;
extern CIwSoundSpec* FX_LooseALive;
extern CIwSoundSpec* FX_Dying_Baron;
extern CIwSoundSpec* FX_Dying_Convict;
extern CIwSoundSpec* FX_Dying_Pirate;
extern CIwSoundSpec* FX_Dying_Rabbit;
extern CIwSoundSpec* FX_Dying_Scientist;
extern CIwSoundSpec* FX_Dying_Soldier;
extern CIwSoundSpec* FX_Dying_Tank;
extern CIwSoundSpec* FX_Selknam_Archer_Deploy_01;
extern CIwSoundSpec* FX_Selknam_Archer_Deploy_02;
extern CIwSoundSpec* FX_Selknam_Archer_Select_01;
extern CIwSoundSpec* FX_Selknam_Archer_Select_02;
extern CIwSoundSpec* FX_Selknam_Bowler_Deploy_01;
extern CIwSoundSpec* FX_Selknam_Bowler_Deploy_02;
extern CIwSoundSpec* FX_Selknam_Bowler_Select_01;
extern CIwSoundSpec* FX_Selknam_Bowler_Select_02;
extern CIwSoundSpec* FX_Selknam_Mage_Deploy_01;
extern CIwSoundSpec* FX_Selknam_Mage_Deploy_02;
extern CIwSoundSpec* FX_Selknam_Mage_Select_01;
extern CIwSoundSpec* FX_Selknam_Mage_Select_02;
extern CIwSoundSpec* FX_Selknam_Warrior_Deploy_01;
extern CIwSoundSpec* FX_Selknam_Warrior_Deploy_02;
extern CIwSoundSpec* FX_Selknam_Warrior_Select_01;
extern CIwSoundSpec* FX_Selknam_Warrior_Select_02;
extern CIwSoundSpec* FX_Dying_Selknam_Archer;
extern CIwSoundSpec* FX_Dying_Selknam_Bowler;
extern CIwSoundSpec* FX_Dying_Selknam_Mage;
extern CIwSoundSpec* FX_Dying_Selknam_Warrior;
extern CIwSoundSpec* FX_Boss_angry_after_paincycle;
extern CIwSoundSpec* FX_boss_comment_cycle01_backward_01;
extern CIwSoundSpec* FX_Boss_comment_cycle01_moving_forward_01;
extern CIwSoundSpec* FX_Boss_comment_cycle01_prefire;
extern CIwSoundSpec* FX_Boss_comment_cycle02_backward_01;
extern CIwSoundSpec* FX_Boss_comment_cycle02_moving_forward_01;
extern CIwSoundSpec* FX_Boss_comment_cycle02_prefire;
extern CIwSoundSpec* FX_Boss_comment_cycle03_backward_01;
extern CIwSoundSpec* FX_Boss_comment_cycle03_backward_02;
extern CIwSoundSpec* FX_Boss_comment_cycle03_moving_forward_01;
extern CIwSoundSpec* FX_Boss_comment_cycle03_prefire;
extern CIwSoundSpec* FX_Boss_dying;
extern CIwSoundSpec* FX_Boss_pain_01;
extern CIwSoundSpec* FX_Boss_pain_02;
extern CIwSoundSpec* FX_Boss_pain_03;
extern CIwSoundSpec* FX_Cannon_ship;
extern CIwSoundSpec* FX_Boss_appear_explosion;
extern CIwSoundSpec* FX_Boss_laugh;

extern int sound_arrows_numallowed;
extern int sound_arrows_numcurrent;
extern int sound_arrows_milisectorenew;
extern int sound_arrows_milisecstamp;

extern int sound_cannon_ship_numallowed;
extern int sound_cannon_ship_numcurrent;
extern int sound_cannon_ship_milisectorenew;
extern int sound_cannon_ship_milisecstamp;

extern int sound_explosion_numallowed;
extern int sound_explosion_numcurrent;
extern int sound_explosion_milisectorenew;
extern int sound_explosion_milisecstamp;

extern int sound_loosealive_numallowed;
extern int sound_loosealive_numcurrent;
extern int sound_loosealive_milisectorenew;
extern int sound_loosealive_milisecstamp;

extern int sound_punch_numallowed;
extern int sound_punch_numcurrent;
extern int sound_punch_milisectorenew;
extern int sound_punch_milisecstamp;


extern bool bExtraMapMode;
extern bool bFxSoundMuted;

//For fx sound 
//extern int8 accumulated_frames;
//extern int8 fx_sound_limit[MAX_FX_SOUNDS][2]; //[Limit][Current sounds played]

//Store
extern bool bResetStoreStats;
extern bool bPausedStore;
extern bool bFirstMapWasPlayed;
extern bool bBlockKeyboardOnce;

//Tutorial
extern Tutorial *tutorial;
extern bool bShowTutorialLevel1;
extern bool bShowTutorialStars;
extern bool bShowTutorialUnlockStore;
extern bool bShowTutorialSurvivalModeUnlock;
extern bool bPauseGame;
extern bool bPauseMapSelection;
extern bool bMouseActive;
extern bool bStopWaveTimer;
extern bool bUpdateClickEvents;

//Others
extern bool bBlockOnQuit;
extern bool bShowTrialScreen;

extern bool bBlockMousePointer;

//Gameplay window has a size bigger than 1024x720. 
//To see the whole window, a xWindowOffset and a yWindowOffset are defined.
extern int xWindowOffset;
extern int yWindowOffset;

extern int LastTouchX;
extern int LastTouchY;

extern int TouchMotionTolerance;

extern TextureData TextureDataList[256];
extern int currentTextureId;

extern TextureData buffBarTextureData;

//Array of groups
extern int commonResourcesNum;
extern const char* commonResources[15];

extern int nonHDImagesResourcesNum;
extern const char* nonHDImagesResources[12];

extern int hDImagesResourcesNum;
extern const char* hDImagesResources[23];

extern int totalResourcesLoadedNum;
extern char totalResourcesLoaded[256][256];

//Pointers to each available texture.
extern CIwTexture* atlas_bkg_gameplay;
extern CIwTexture* atlas_bkg_menu_foreground;
extern CIwTexture* atlas_bkg_menu_others;
extern CIwTexture* atlas_bkg_menu_sky;
extern CIwTexture* atlas_bkg_worldmap;
extern CIwTexture* atlas_bkg_small_0;
extern CIwTexture* atlas_bkg_small_1;
extern CIwTexture* atlas_common_textures_0;
extern CIwTexture* atlas_common_textures_1;
extern CIwTexture* atlas_common_textures_2;
extern CIwTexture* atlas_common_textures_3;
extern CIwTexture* atlas_common_textures_4;
extern CIwTexture* atlas_small_textures;
extern CIwTexture* atlas_common_trial_0;
extern CIwTexture* atlas_fx_0;
extern CIwTexture* atlas_fx_1;
extern CIwTexture* atlas_fx_2;
extern CIwTexture* atlas_props_0;
extern CIwTexture* atlas_props_1;
extern CIwTexture* atlas_props_2;
extern CIwTexture* atlas_terrain_0;
extern CIwTexture* atlas_terrain_1;

extern CIwTexture* atlas_others;

typedef struct  
{
	float width;
	float height;
} AtlasHDSize;

//HD Chars
extern CIwTexture* atlas_hd_selknam_archer_1;
extern CIwTexture* atlas_hd_selknam_archer_2;
extern CIwTexture* atlas_hd_selknam_archer_3;
extern CIwTexture* atlas_hd_selknam_bowler_1_01;
extern CIwTexture* atlas_hd_selknam_bowler_1_02;
extern CIwTexture* atlas_hd_selknam_bowler_2_01;
extern CIwTexture* atlas_hd_selknam_bowler_2_02;
extern CIwTexture* atlas_hd_selknam_bowler_3_01;
extern CIwTexture* atlas_hd_selknam_bowler_3_02;
extern CIwTexture* atlas_hd_selknam_mage_1;
extern CIwTexture* atlas_hd_selknam_mage_2;
extern CIwTexture* atlas_hd_selknam_mage_3;
extern CIwTexture* atlas_hd_selknam_melee_1;
extern CIwTexture* atlas_hd_selknam_melee_2;
extern CIwTexture* atlas_hd_selknam_melee_3;

extern CIwTexture* atlas_hd_enemy_1;
extern CIwTexture* atlas_hd_enemy_2;
extern CIwTexture* atlas_hd_enemy_3;
extern CIwTexture* atlas_hd_enemy_4_01;
extern CIwTexture* atlas_hd_enemy_4_02;
extern CIwTexture* atlas_hd_enemy_5;
extern CIwTexture* atlas_hd_enemy_6;
extern CIwTexture* atlas_hd_enemy_7;

//Atlas sizes
extern AtlasHDSize atlas_hdsize_selknam_archer;
extern AtlasHDSize atlas_hdsize_selknam_bowler_01;
extern AtlasHDSize atlas_hdsize_selknam_bowler_02;
extern AtlasHDSize atlas_hdsize_selknam_mage;
extern AtlasHDSize atlas_hdsize_selknam_melee;

extern AtlasHDSize atlas_hdsize_enemy_1;
extern AtlasHDSize atlas_hdsize_enemy_2;
extern AtlasHDSize atlas_hdsize_enemy_3;
extern AtlasHDSize atlas_hdsize_enemy_4_01;
extern AtlasHDSize atlas_hdsize_enemy_4_02;
extern AtlasHDSize atlas_hdsize_enemy_5;
extern AtlasHDSize atlas_hdsize_enemy_6;
extern AtlasHDSize atlas_hdsize_enemy_7;


//Normal Chars
extern CIwTexture* atlas_selknam_archer_01_1;
extern CIwTexture* atlas_selknam_archer_01_2;
extern CIwTexture* atlas_selknam_archer_02_1;
extern CIwTexture* atlas_selknam_archer_02_2;
extern CIwTexture* atlas_selknam_archer_03_1;
extern CIwTexture* atlas_selknam_archer_03_2;
extern CIwTexture* atlas_selknam_melee_01_1;
extern CIwTexture* atlas_selknam_melee_01_2;
extern CIwTexture* atlas_selknam_melee_02_1;
extern CIwTexture* atlas_selknam_melee_02_2;
extern CIwTexture* atlas_selknam_melee_03_1;
extern CIwTexture* atlas_selknam_melee_03_2;
extern CIwTexture* atlas_selknam_mage_01_1;
extern CIwTexture* atlas_selknam_mage_01_2;
extern CIwTexture* atlas_selknam_mage_02_1;
extern CIwTexture* atlas_selknam_mage_02_2;
extern CIwTexture* atlas_selknam_mage_03_1;
extern CIwTexture* atlas_selknam_mage_03_2;
extern CIwTexture* atlas_selknam_bowler_01_1;
extern CIwTexture* atlas_selknam_bowler_01_2;
extern CIwTexture* atlas_selknam_bowler_02_1;
extern CIwTexture* atlas_selknam_bowler_02_2;
extern CIwTexture* atlas_selknam_bowler_03_1;
extern CIwTexture* atlas_selknam_bowler_03_2;

extern CIwTexture* atlas_enemy_01_1;
extern CIwTexture* atlas_enemy_01_2;
extern CIwTexture* atlas_enemy_02_1;
extern CIwTexture* atlas_enemy_02_2;
extern CIwTexture* atlas_enemy_03_1;
extern CIwTexture* atlas_enemy_04_1;
extern CIwTexture* atlas_enemy_04_2;
extern CIwTexture* atlas_enemy_05_1;
extern CIwTexture* atlas_enemy_06_1;
extern CIwTexture* atlas_enemy_07_1;
extern CIwTexture* atlas_enemy_07_2;
extern CIwTexture* atlas_boss;


//SCREEN FIX
extern float globalScale;
extern float mapTileUpperCornerX;
extern float tileMapWidthImage;
extern float tileMapHeightImage;
extern float mapXOffset;
extern float mapYOffset;
//Isometric Pos Helpers
extern float tileMapWidthBase;
extern float tileMapHeightBase;
extern float tileDiffLayer;
extern float isoFormulaWValue;
extern float isoFormulaHValue;
extern float isoFormulaAValue;
extern float isoFormulaInvAValue;
extern float isoFormulaXOffset;
extern float isoFormulaYOffset;

extern int xMinOffset;
extern int xMaxOffset;
extern int yMinOffset;
extern int yMaxOffset;

extern GameMode gameMode;

extern DialogController* dialogController;

extern SurvivalModeConfig* survivalModeConfig;

extern int numSelknamKilledByMinute;
extern int tempNumSelknamKilledByMinute;

extern almanacState curAlmanacState;
extern almanacState lastAlmanacState;

extern AlmanacBase* almanacBase;
extern SettingsMenu* settingsMenu;

extern bool isEnemyUnlocked[MAX_ENEMIES];

extern int bestScoreByLevel[MAX_MAPS];

extern bool bCheatModeEnabled;

//Save data handling
class GameDataCampaign 
{
public:
	int newGame;
	int campaignMapsData[MAX_MAPS];
	int starsByLevelData[MAX_MAPS];

	int archerBaseUpgradesData;
	int archerPoisonerUpgradesData;
	int archerRetarderUpgradesData;

	int warriorBaseUpgradesData;
	int warriorTankUpgradesData;
	int warriorBerserkUpgradesData;

	int shamanHealerUpgradesData;
	int shamanMightUpgradesData;
	int shamanHasteUpgradesData;

	int bowlerStunerUpgradesData;
	int bowlerRooterUpgradesData;
	int bowlerConfuserUpgradesData;

	int usedStarsData;

	int8 starsByExtraMapsData[MAX_EXTRA_MAPS];

	bool bFirstMapWasPlayed;
	bool bShowTutorialLevel1;
	bool bShowTutorialStars;
	bool bShowTutorialUnlockStore;
	bool bShowTutorialSurvivalModeUnlock;

	bool enemiesUnlocked[MAX_ENEMIES];

	int bestScoreByLevel[MAX_MAPS];

	GameDataCampaign();
	~GameDataCampaign();
	void SetGameDataCampaignFullProgress();
};

extern GameDataCampaign gameDataCampaign;


class GameDataSurvival 
{
public:
	bool bSurvivalAvailable;
	int survivalBestRecord;
	GameDataSurvival();
	~GameDataSurvival();
	void SetGameDataSurvivalAvailable();
};

extern GameDataSurvival gameDataSurvival;

class GameLocalData
{
public:
	int audioVolume;
	int soundVolume;
	bool bIsFullScreenOption;

	//extraInt_01: lastAudioVolume
	int extraInt_01;
	//extraInt_02: lastSoundVolume
	int extraInt_02;
	int extraInt_03;
	int extraInt_04;
	int extraInt_05;
	int extraInt_06;
	int extraInt_07;
	int extraInt_08;
	int extraInt_09;
	int extraInt_10;

	//extra bool 01 : Mute All
	bool extraBool_01;
	//extra bool 02 : bIsFull version
	bool extraBool_02;
	bool extraBool_03;
	bool extraBool_04;
	bool extraBool_05;
	bool extraBool_06;
	bool extraBool_07;
	bool extraBool_08;
	bool extraBool_09;
	bool extraBool_10;

	float extraFloat_01;
	float extraFloat_02;
	float extraFloat_03;
	float extraFloat_04;
	float extraFloat_05;
	float extraFloat_06;
	float extraFloat_07;
	float extraFloat_08;
	float extraFloat_09;
	float extraFloat_10;

	GameLocalData();
	~GameLocalData();
};

extern GameLocalData gameLocalData;

//Analytics
extern bool bFlurryOn;
//Ads
//extern bool bLeadbolt;
extern int adOnWinLevel;

//Landscape Hack
extern bool bLandscapeHack;

//End Game Boss
extern CaptainBoss* captainBoss;

extern bool bBlockCamera;
extern bool bFreezeCamera;

extern int numFocusedObjects;

extern int finalBoss_time_before_fadeout;
extern int finalBoss_time_fadeout;
extern int finalBoss_cycle1_time_recharge;
extern int finalBoss_cycle2_time_recharge;
extern int finalBoss_cycle3_time_recharge;
extern int finalBoss_time_resting;
extern int finalBoss_time_pain_anim_can_be_played;
extern int finalBoss_time_ship_explotion_time;
extern int finalboss_time_ship_explotion_single_explotion;
extern int finalboss_cycle1_backward_time;
extern int finalboss_cycle2_backward_time;
extern int finalboss_cycle3_backward_time;
extern int finalboss_cycle1_wave_delay;
extern int finalboss_cycle2_wave_delay;
extern int finalboss_cycle3_wave_delay;
extern int finalboss_cycle1_wave_time;
extern int finalboss_cycle2_wave_time;
extern int finalboss_cycle3_wave_time;
extern int finalboss_cycle1_time_before_sp;
extern int finalboss_cycle2_time_before_sp;
extern int finalboss_cycle3_time_before_sp;
extern int finalboss_cycle1_time_between_sp_shots;
extern int finalboss_cycle2_time_between_sp_shots;
extern int finalboss_cycle3_time_between_sp_shots;
extern int finalboss_sp_markspot_duration;
extern int finalboss_forward_time;
extern int finalboss_max_health;
extern int finalboss_deathdebuff_time;
extern int finalboss_extracoins_per_enemy;
extern int finalboss_poison_damage_per_interval;
extern int finalboss_poison_interval_time;
extern int finalboss_poison_num_intervals;
extern float finalboss_ship_idle_speed;
extern int finalboss_cycle1_cannon_damage;
extern int finalboss_cycle2_cannon_damage;
extern int finalboss_cycle3_cannon_damage;
extern int finalboss_num_waves[3];
extern int finalboss_cycle1_sp_num_shots;
extern int finalboss_cycle2_sp_num_shots;
extern int finalboss_cycle3_sp_num_shots;
extern IsoEnemyType finalboss_unit_to_spawn[3][10][4];
extern int countDownDeathDebuff_Chance;

extern ControllerType activeControllerType;

//Time Helper Global Value
extern uint64 deltaSystemTime;
extern uint64 lastSystemTime;

//savedatahelper
extern SaveDataHelper *saveDataHelper;

//shaders
extern CIwGxShaderTechnique* shader_font_a;
extern CIwGxShaderTechnique* shader_blur;
extern CIwGxShaderTechnique* shader_bicubic;

//Ads
extern const char* leadboltAdNumber;

//Achievements helper
extern AchievementsHelper	*achievementsHelper;

//Leaderboards helper
extern LeaderboardsHelper	*leaderboardsHelper;

//GUI placement.
//Limited place where GUI can be place in runtime. Example: UnitManager.
extern CIwRect dynamicGUIWrapper[GAMEPLAY_NUM_WRAPPER_RECTS];

//Steam vars
extern bool bSteam_LoggedOn;
extern const char* steam_PersonalName;
extern uint32 steamAppId;
extern const char* steam_AppAllLanguages;
extern const char* steam_AppCurrentLanguage;

#ifdef STEAMREQUIRED
//STEAM
extern CSteamAchievements *g_SteamAchievements;
extern CSteamLeaderboards *g_SteamLeaderboards;
#endif

//ACHIEVEMENTS
#define _ACH_ID( id, name ) { id, #id, name, "", 0, 0 }

struct Achievement_t
{
	int m_eAchievementID;
	const char *m_pchAchievementID;
	char m_rgchName[128];
	char m_rgchDescription[256];
	bool m_bAchieved;
	int m_iIconImage;
};

enum EAchievements
{
	SD_ACHIEVEMENT_00 = 0,
	SD_ACHIEVEMENT_01 = 1,
	SD_ACHIEVEMENT_02 = 2,
	SD_ACHIEVEMENT_03 = 3,
	SD_ACHIEVEMENT_04 = 4,
	SD_ACHIEVEMENT_05 = 5,
	SD_ACHIEVEMENT_06 = 6,
	SD_ACHIEVEMENT_07 = 7,
	SD_ACHIEVEMENT_08 = 8,
	SD_ACHIEVEMENT_09 = 9,
	SD_ACHIEVEMENT_10 = 10,
	SD_ACHIEVEMENT_11 = 11,
	SD_ACHIEVEMENT_12 = 12,
	SD_ACHIEVEMENT_13 = 13,
	SD_ACHIEVEMENT_14 = 14,
};

extern XBOX360_Button Xbox360_Left;
extern XBOX360_Button Xbox360_Up;
extern XBOX360_Button Xbox360_Down;
extern XBOX360_Button Xbox360_Right;
extern XBOX360_Button Xbox360_Action1;
extern XBOX360_Button Xbox360_Action2;
extern XBOX360_Button Xbox360_Action3;
extern XBOX360_Button Xbox360_Cancel1;
extern XBOX360_Button Xbox360_Cancel2;
extern XBOX360_Button Xbox360_IncreaseWaveSpeed;
extern XBOX360_Button Xbox360_DecreaseWaveSpeed;
extern XBOX360_Button Xbox360_OpenIngameAlmanac;
extern XBOX360_Button Xbox360_OpenIngameMenu;
extern int xbox360CameraSpeed;

//Screen Handle
#if defined DESKTOPPLATFORM
extern int displayNumModes;
extern s3eWindowDisplayMode* displayModes;
extern s3eWindowDisplayMode g_FullscreenMode;
extern int displayWidthSelected;
extern int displayHeightSelected;
#endif

extern s3eKey currKeyPressed;

//FPS MULTIPLIER
extern int oldFPS;
extern float FPSMULTIPLIER;

//Settings
extern int currentAudioVolume;
extern int currentSoundVolume;
extern int lastAudioVolume;
extern int lastSoundVolume;
//All Muted
extern bool bAllMuted;
extern bool bIsFullScreenOption;

extern int mouseDetectionSensibility;
extern int cameraSpeedConstant;
extern s3eKey noKeyAssignment;

#endif	// _COMMON_H_

