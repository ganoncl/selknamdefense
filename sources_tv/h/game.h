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

#ifndef _GAME_H_
#define _GAME_H_

#include "IwSound.h"

#include "common.h"
#include "isoobject.h"
#include "isoterrain.h"
#include "isoprop.h"
#include "isopawn.h"
#include "isoenemy.h"
#include "isoselknam.h"
#include "timer.h"
#include "mappointer.h"
#include "unitmenu.h"
#include "unitmanager.h"

#include <algorithm>

//InGame Windows
#include "endingloose.h"
#include "endingwin.h"
#include "mainmenuigbase.h"

#include "parserhelper.h"
#include "astarhelper.h"
#include "isoprojectile.h"
#include "texturehelper.h"

struct ExplotionEffect
{
	int index;
	float _x;
	float _y;
	float _z;
	Timer timer;
	bool bRender;
};

struct HealEffect
{
	int index;
	float _x;
	float _y;
	float _z;
	Timer timer;
};

struct MightEffect
{
	int index;
	float _x;
	float _y;
	float _z;
	Timer timer;
};

struct HasteEffect
{
	int index;
	float _x;
	float _y;
	float _z;
	Timer timer;
};

struct PoisonOnDeathEffect
{
	int index;
	float _x;
	float _y;
	float _z;
	Timer timer;
	bool bRender;
};

struct CoinEffect
{
	int index;
	float _x;
	float _y;
	float _z;
	Timer timer;
};

struct IsoPawnRenderContainer
{
	IsoSelknam*    isoSelknam;
	IsoEnemy*      isoEnemy;
	IsoProp*	   isoProp;
	IsoProjectile* isoProjectile;
	PoisonOnDeathEffect* isoPoison;
	ExplotionEffect* isoExplotion;
	float          _z;
};

class CGame
{
public:
	CGame();
	~CGame();

	bool bIsActiveObj;
	void CleanMapProperties();
	void CleanTerrainObjects();
	void CleanPawns();
	void InitObject();
	void InitResources();
	void EndObject();
	void Update();
	void HandleUpdateGameStates(InGameState _inGameState);
	void HandleUpdatePausedStates();
	void SetAtlasAsTexture(int atlasId);

	//Map Parser Helper
	ParserHelper parserHelper;

	//Map Properties
	IsoTerrain *initNodes[MAX_INITNODES];
	void GetInitNodes();

	//Cursor
	IsoTerrain *isoTerrainCursorGrid[MAX_TILESROW][MAX_TILESROW];
	void CursorFillGrid();

	//A-Star
	AStarHelper aStarHelper;
	void AStarPropagateHelper();
	//walkeable grid
	IsoTerrain *isoTerrainAstarGrid[MAX_TILESROW][MAX_TILESROW];
	//walkeable list without order.
	IsoTerrain *isoTerrainAstarList[ASTAR_LISTSMAX];
	IsoTerrain *objectivesNodes[MAX_ASTAROBJECTIVES];
	void AStarFillGrid();
	void AStarCalcPathsForAllTiles();

	//Render
	void RenderSelknam(IsoSelknamType selknamType);
	void RenderEnemy(IsoEnemyType enemyType);
	void Render();
	void HandleRenderGameStates(InGameState _inGameState);
	void HandleRenderGamePausedStates();
	void DrawQuad(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX = 0, float _uvMaxX = 1, float _uvMinY = 0, float _uvMaxY = 1, float _offsetX = 0, float _offsetY = 0);
	void DrawQuadBatch(float _width, float _height, float _posX, float _posY, float _posZ, float _uvMinX = 0, float _uvMaxX = 1, float _uvMinY = 0, float _uvMaxY = 1, float _offsetX = 0, float _offsetY = 0, int r = 255, int g = 255, int b = 255, int alpha = 255, bool bRotate = false);

	//InGame Windows
	MainMenuIG* mainmenuig;
	EndingWin* endingwin;
	EndingLoose* endingloose;

	//Device handler
	bool bMouseBlocked;
	bool bKeyboardBlocked;
	s3eKey lastKeyPressed;
	s3ePointerButton lastPointerButton;
	void KeyboardHandler(s3eKeyboardEvent* event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateClickEvents(s3ePointerEvent *event);

	//UnitMenu
	MapPointer mapPointer;
	UnitMenu unitMenu;
	UnitManager unitManager;
	void SpawnSelknam(IsoSelknamType type);
	void ForceNextWave();

	void TakeDamage(int damage, IsoEnemy* inflictor);

	//Max numbers of terrain on scene on all game
	IsoTerrain isoTerrainArr[MAX_TERRAIN_TILES];
	//Max numbers of props on scene on all game
	IsoProp isoPropArr[MAX_ISO_PROPS];
	//Max numbers of enemies on scene on all game
	IsoEnemy isoEnemyArr[MAX_ISO_ENEMIES];
	//Max numbers of selknams on scene on all game
	IsoSelknam isoSelknamArr[MAX_ISO_SELKNAM];

	IsoProjectile isoProjectileArr[MAX_ISO_PROJECTILE];

	Timer timer_change_theme;
	Background* defense_stage_back;

	FadeScreen* loadingfade;
	bool bDoFadeOut;
	Timer timeToFadeOut;
	bool bChangeTheme;

	void MouseHack();
	void ActiveButtonMouseHack();

protected:
	//Initial Map Properties
	//lifes
	int mp_lifes;
	//coins
	int mp_coins;
	//cinematic num, 0 = no cinematic (not implemented yet)
	int mp_cinematic;
	//howtoplaypage, 0 = no howtoplay, other, page to show
	int mp_howtoplaypage;
	//mp_howtoplaymaxpages, max pages to show
	int mp_howtoplaymaxpages;
	//mp_timedefense, time to prepare defenses
	int mp_timedefense;
	//mp_timetowaves[9], time to waves, if 0, no time to next wave
	int mp_timetowaves[9];
	//mp_timetowavesspawn[9], real time during enemies spawning
	int mp_timetowavesspawn[9];
	//mp_tiletype, tile map to apply on rendering texture map 
	int mp_tiletype;

	//General objects
	Timer timer;
	Timer auxTimer;
	Timer waveTimer;
	CIwGxFont* pFont;	
	CIwTexture* ingameMessageTex;
	CIwSVec2 ingameMessagePos;

	//GSG_Defending
	bool bRenderDefenseText;
	const char* DefenseText;
	CIwRect DefenseTextPos;
	//GSG_Attack
	bool bRenderAttackText;
	const char* AttackText;
	CIwRect AttackTextPos;
	//Time Text
	const char* TimeText;
	CIwRect TimeTextPos;
	//Wave Text
	const char* Wave_Text;
	CIwRect Wave_Text_Position;
	char Number_Wave[100];
	//Heart
	CIwTexture* heart_texture;
	CIwRect num_hearts_position;
	char num_hearts_text[100];
	//Gold
	CIwTexture* gold_texture;
	CIwRect num_gold_position;
	char num_gold_text[100];
	
	CIwTexture* prop64_64;
	CIwTexture* prop64_96;
	CIwTexture* prop64_128;

	bool bActiveTutorial;
	int curr_wave;

	//Aux functions
	void RenderSingleTexture(CIwTexture* tex, CIwSVec2 position);
	void RenderText(const char* text, CIwRect position, uint32 color);
	void Debug();

	bool bCanOpenUnitMenu;
	//True: TakeDamage function does nothing
	bool bIsInvincible;
	FadeScreen* damageFadeScreen;

	IsoPawnRenderContainer isoPawnRenderContainer[MAX_ISO_ENEMIES + MAX_ISO_SELKNAM * 3+ MAX_ISO_PROPS + MAX_ISO_PROJECTILE];
	void IsoPawnRenderInit();
	void IsoPawnRenderUpdateZ();
	int currentIsoPawn;

	char timer_text[100];

	void CheckGameWinCondition();

	void UpdateTimer();
	void UpdateHUD();
	void UpdateGameplayVariables();
	void UpdateIsoPawnsOnScreen();
	void UpdateGameState_GSG_Attack();
	void UpdateGameState_GSG_Defending();
	void UpdateProjectiles();

	void RenderHUD();
	void RenderMovementCursor();
	void RenderTerrain();
	void RenderIsoPawnsOnScreen();
	void RenderGameState_GSG_Attack();
	void RenderGameState_GSG_Defending();
	void RenderIsoPawnBarLifes();
	void RenderIsoPawnBarBuff();
	void RenderProjectiles();
	void RenderDebuffGraphics();
	void RenderOthers();

	void AttachPawnTextureToMaterial(IsoSelknamType SelknamType);
	void AttachPawnTextureToMaterial(IsoEnemyType EnemyType);

	void CleanMapDeathPawns();

	//Erase enemies with bad configurations.
	void NormalizeMapXML();

	int enemiesPerWave[20];
	int wave_time;

	void DrawColorQuad(float _width, float _height, float _posX, float _posY, float _posZ, uint32 color, float _offsetX = 0, float _offsetY = 0);

	Timer stresstest_timer;
	int stresstest_num_units;
	void StressTest();
	void SpawnRandomUnits(int num_units);

	Button* nextWaveButton; /* ¿Que hago con los enemigos que aun no se spawnean de la oleada anterior? */

	int last_mp_coins;
	//Gameplay variables
	//(Total Wave time - Current Wave Time) is added to extra_time when NextWaveButton is pressed.
	int extra_time;  
	int time_before_fastButton;
	int wasted_coins;

	Background* background;

	bool bPrintOutput;
	bool bIsKeyPressed;
	int num_waves;

public:
	void setMPLifes(int _mp_lifes) { mp_lifes = _mp_lifes; }
	int getMPLifes() const { return mp_lifes; }

	void setMPCoins(int _mp_coins) { mp_coins = _mp_coins; }
	int getMPCoins() const { return mp_coins; }

	void setMPCinematic(int _mp_cinematic) { mp_cinematic = _mp_cinematic; }
	int getMPCinematic() const { return mp_cinematic; }

	void setMPHowToplay(int _mp_howtoplaypage) { mp_howtoplaypage = _mp_howtoplaypage; }
	int getMPHowToplay() const { return mp_howtoplaypage; }

	void setMPHowToplayMaxPages(int _mp_howtoplaymaxpages) { mp_howtoplaymaxpages = _mp_howtoplaymaxpages; }
	int getMPHowToplayMaxPages() const { return mp_howtoplaymaxpages; }

	void setMPTimeDefense(int _mp_timedefense) { mp_timedefense = _mp_timedefense; }
	int getMPTimeDefense() const { return mp_timedefense; }

	void setMPTimeToWaves(int _mp_timetowaves[9]) 
	{
		for (int i = 0; i < 9; i++)
			mp_timetowaves[i] = _mp_timetowaves[i]; 
	}

	void setMPTimeToWavesSpawn(int _mp_timetowavesspawn[9]) 
	{
		for (int i = 0; i < 9; i++)
			mp_timetowavesspawn[i] = _mp_timetowavesspawn[i]; 
	}

	void setMPTileType(int _mp_tiletype) { mp_tiletype = _mp_tiletype; }
	int getMPTileType() const { return mp_tiletype; }

	ExplotionEffect isoExplotion[MAX_ISO_SELKNAM];
	HealEffect      isoHeal[MAX_ISO_SELKNAM];
	MightEffect     isoMight[MAX_ISO_SELKNAM];
	HasteEffect     isoHaste[MAX_ISO_SELKNAM];
	PoisonOnDeathEffect isoPoisonOnDeath[MAX_ISO_SELKNAM];
	CoinEffect isoCoin[MAX_ISO_ENEMIES];
	int movementMarkMap[MAX_TILESROW][MAX_TILESROW];
	int rangeTileMap[MAX_TILESROW][MAX_TILESROW];

	private:
		ButtonRenderData nextWaveData;
		RenderData graphic_defense_stage;
		RenderData label_infocard;
		RenderData label_hud_lives_gold;
		RenderData terrainData;
		RenderData prop64_64_Data;
		RenderData prop64_96_Data;
		RenderData prop64_128_Data;
		RenderData rootRenderData;
		TextureData terrainTextureData;
		TextureData expData;
		AtlasName buffBarAtlasName;
		AtlasName arrowProjectileName, rockProjectileName;
		AtlasName explosionName;
		RenderData redButton;
		AtlasName healName, poisonName, stunName, disorientedName, coinsName;
		TextureData healData, poisonData, stunData, disorientedData, coinsData;

		TextureData movementMarkData;
		AtlasName movementMarkAtlasName;

		TextureData rangeTileMapData;
		AtlasName rangeTileMapAtlasName;
		CIwSVec2 rangeTileMapDimension;

		uint64 lastTime;
		uint64 deltaTime;
		int blinkMapTileTime;

		void RenderIsoPawnTexture(CIwTexture* texture, IsoPawn* pawn);
};

#endif // _GAME_H_
