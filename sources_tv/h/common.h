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

#define MAX_TERRAIN_TILES 512
#define MAX_ISO_PROPS 128
#define MAX_ISO_ENEMIES 128
#define MAX_ISO_SELKNAM 128
#define MAX_ASTAROBJECTIVES 10
#define MAX_TILESROW 18
#define ASTAR_HDISTANCE 10
#define ASTAR_LISTSMAX 324
#define ASTAR_MOVECOST 100
#define MAX_INITNODES 10
#define MAX_SKILLS 10
#define MAX_MAPS 24
#define MAX_EXTRA_MAPS 999
#define MAX_ANIMATION 15
#define MAX_ISO_PROJECTILE 20
#define SOUND_MUSIC_MUTE false
#define ASTAR_WALKABLETILECOST 60
#define ASTAR_WALKABLEGREENTILECOST 450
#define ASTAR_ENEMYONTILECOST 1000

//Music and FXSounds
#define ENABLE_SOUND_BUTTONS 1
#define MAX_FX_SOUNDS 35

//Hacks
//When mouse is not active (not rendering) and user clicks, enter button is pressed instead.
#define ENABLE_MOUSE_HACK 1

//Debug
//Show a text with the code of the last key pressed.
#define LAST_KEY_PRESSED 0


typedef enum
{
	Music_None,	   //No audio playing.
	Music_Loose,     //sd_fanfare_lose_theme.mp3
	Music_Win,       //sd_fanfare_win_theme.mp3
	Music_Ingame_01, //sd_ingame_01_theme_loop.mp3
	Music_Ingame_02, //sd_ingame_02_theme_loop.mp3
	Music_MainMenu,  //sd_mainmenu_theme_loop.mp3
	Music_Store,     //sd_store_theme_loop.mp3
} Music_Name;


//Version
extern string version_number;
extern string version_date;
extern bool bWaterMark;
extern bool bUseOldAudioSystem;

extern Music_Name LastMusicStacked;
extern Music_Name CurrentMusicPlaying;
extern int LastMusicPosition;

extern bool bMapSelectionAudioPlaying;
extern bool bIsAudioPaused;
extern bool bIsFxPaused;

//Mouse configuration
extern int32 iLastMouseX;
extern int32 iLastMouseY;

//Loading a map: When a particular map is selected, a black screen appears for TIME_BLACKSCREEN miliseconds
//After that, a Fade Out is executed at a speed of FADE_OUT_SPEED (value between 0 - 255)
#define TIME_BLACKSCREEN 250 
#define FADE_OUT_SCREEN 30

//Delay between the button to move the map pointer is pressed and the 
//automatic motion is activated. (miliseconds)
#define DELAY_BEFORE_MOUSE_BEGIN_TO_MOVE 1000 

//Confusion Skill: Move to North, West, East or South and wait there for CONFUSION_STOP_TIME miliseconds. Repeat this process CONFUSION_TIMES times.
#define CONFUSION_STOP_TIME 1000
#define CONFUSION_TIMES 4

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
} RotateImage;

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

typedef enum
{
	GSM_None,
	GSM_MainMenu,
	GSM_HowToPlay,
	GSM_Options,
	GSM_Credits,
	GSM_TrialVersion,
	GSM_FinalScreen //Menu that appears when the game is completed.
} MainMenuState;

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
} PopUpState;

typedef enum
{
	GSG_None,
	GSG_Cinematic,
	GSG_Defending,
	GSG_Attack,
	GSG_Win,
	GSG_Loose,
} InGameState;

typedef enum
{
	GSGP_None,
	GSGP_MainMenuIG,
	GSGP_HowToPlayIG,
} InGamePausedState;

typedef enum
{
	GSMIG_None,
	GSMIG_MainMenuIG,
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
	ATLAS_COMMON_0,
	ATLAS_COMMON_1,
	ATLAS_COMMON_2,
	ATLAS_COMMON_TRIAL,
	ATLAS_MENU_01_0,
	ATLAS_MENU_01_1,
	ATLAS_MENU_02_0,
	ATLAS_GAMEBACKGROUND_0,
	ATLAS_WORLDMAP,
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
	ATLAS_TERRAIN_0,
	ATLAS_PROPS_0,
	ATLAS_PROPS_1,
	ATLAS_PROPS_2,
	ATLAS_FX_0,
	ATLAS_FX_1,
	ATLAS_FX_2,
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

extern FadeScreen *fade;

extern PopUp *popup;

extern bool bPause;

extern int currentMap;

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

//SKILLS
extern Skill_Poison				    Skill_Att_Poison[3];
extern Skill_Slow					Skill_Att_Slow[3];
extern Skill_Stun					Skill_Att_Stun[3];
extern Skill_Root					Skill_Att_Root[3];
extern Skill_Confusion				Skill_Att_Confusion[3];
extern Skill_Explode				Skill_Att_Explode[3];
extern Skill_PoisonOnDeath			Skill_Att_PoisonOnDeath[3];
extern Skill_Heal					Skill_Att_Heal[3];
extern Skill_Might					Skill_Att_Might[3];
extern Skill_Haste					Skill_Att_Haste[3];

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
extern int Stars_By_Level[24];
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

extern CIwGxFont* font_komikadisplaybold25;
extern CIwGxFont* font_komikadisplay11;
extern CIwGxFont* font_komikadisplay28;
extern CIwGxFont* font_komikadisplay14;
extern CIwGxFont* font_komikadisplay18;
extern CIwGxFont* font_komikadisplaybold18;
extern CIwGxFont* font_komikadisplay22;
extern CIwGxFont* font_komikadisplaybold22;
extern CIwGxFont* font_komikadisplaybold48;

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
extern CIwSoundSpec* Arrow_Shooting;
extern CIwSoundSpec* FX_Click;
extern CIwSoundSpec* FX_Explosion;
extern CIwSoundSpec* FX_Knife_Sword_01;
extern CIwSoundSpec* FX_RollOver_02;
extern CIwSoundSpec* FX_Shot_01;
extern CIwSoundSpec* FX_Star_Gained_02;
extern CIwSoundSpec* FX_Silence;
extern CIwSoundSpec* FX_MetalImpact;
extern CIwSoundSpec* FX_MetalImpact2;
extern CIwSoundSpec* FX_Coins;
extern CIwSoundSpec* FX_Heal;
extern CIwSoundSpec* FX_Bubbles;

extern bool bRenderColorButtons;

extern bool bExtraMapMode;
extern bool bFxSoundMuted;

//For fx sound 
extern int8 accumulated_frames;
extern int8 fx_sound_limit[MAX_FX_SOUNDS][2]; //[Limit][Current sounds played]

//Store
extern bool bResetStoreStats;
extern bool bPausedStore;
extern bool bFirstMapWasPlayed;
extern bool bBlockKeyboardOnce;

extern bool bIsFullVersion;
extern bool bShowTrialVersionFinal;

//Tutorial
extern Tutorial *tutorial;
extern bool bShowTutorialLevel1;
extern bool bShowTutorialStars;
extern bool bPauseGame;
extern bool bPauseMapSelection;
extern bool bMouseActive;
extern bool bStopWaveTimer;
extern bool bUpdateClickEvents;

//Others
extern bool bBlockOnQuit;
extern bool bShowTrialScreen;

extern bool bBlockMousePointer;

extern TextureData TextureDataList[256];
extern int currentTextureId;

extern TextureData buffBarTextureData;

//Pointers to each available texture.
extern CIwTexture* atlas_common_0;
extern CIwTexture* atlas_common_1;
extern CIwTexture* atlas_common_2;
extern CIwTexture* atlas_common_trial;
extern CIwTexture* atlas_menu_01_0;
extern CIwTexture* atlas_menu_01_1;
extern CIwTexture* atlas_menu_02_0;
extern CIwTexture* atlas_gamebackground_0;
extern CIwTexture* atlas_worldmap;
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
extern CIwTexture* atlas_terrain_0;
extern CIwTexture* atlas_props_0;
extern CIwTexture* atlas_props_1;
extern CIwTexture* atlas_props_2;
extern CIwTexture* atlas_fx_0;
extern CIwTexture* atlas_fx_1;
extern CIwTexture* atlas_fx_2;

//SCREEN FIX
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


//Array of groups
extern int commonResourcesNum;
extern const char* commonResources[22];

extern int totalResourcesLoadedNum;
extern char totalResourcesLoaded[256][256];

#endif	// _COMMON_H_

