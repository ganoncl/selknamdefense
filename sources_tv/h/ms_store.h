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

#ifndef _STORE_H
#define _STORE_H

#include "background.h"
#include "storebutton.h"
#include "IwGxFont.h"
#include "common.h"
#include "texturehelper.h"
#include <string>
using namespace std;

typedef enum
{
	Store_Focus_None,
	Store_Focus_Reset,
	Store_Focus_Tab,
	Store_Focus_Panel,
	Store_Focus_Portrait_1,
	Store_Focus_Portrait_2,
	Store_Focus_Portrait_3,
	Store_Focus_Back,
	Store_Focus_SpecialTab //When mouse is over a tab button but is not the actual focus.
} Store_Focus;

typedef enum
{
	ARCHER_TAB,
	BOWLER_TAB,
	WARRIOR_TAB,
	MAGE_TAB,
} Selknam_Tab;

struct TabPosition
{
	int _x;
};

struct PanelPosition
{
	int _x;
	int _y;
};

class Store
{
public:
	Store();
	~Store();
	void Render();
	void InitObject();
	void EndObject();
	bool bIsActiveObj;

	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	bool bMouseBlocked;
	bool bKeyboardBlocked;
	s3eKey lastKeyPressed;

	Selknam_Tab tab;
	TabPosition tab_pos;
	PanelPosition panel_pos;
	Store_Focus focus;

	void UpgradeUnit();

	void SetFocus(Store_Focus focus);
	void ResetFocus();

	void ResetStars();
	void LoadClassInfo();

	void FocusTab(Selknam_Tab t);

	void MouseHack();
	void ActiveButtonMouseHack();

private:
	Background* background;

	Button* button_tab[4];
	Button* button_panel[3][3];
	Button* button_back;
	Button* button_reset;

	CIwTexture* selknam_img[4][3];
	CIwTexture* star[2];
	CIwTexture* portrait;
	CIwTexture* current_Portrait_Texture;

	CIwTexture* panel_tex;
	CIwTexture* panel_b_tex;
	CIwTexture* starcounter_tex;
	bool available_panel[3][3];

	bool gray_color[3];

	char remaining_stars[100];
	CIwRect remaining_stars_position;
	uint32 remaining_stars_color;
	int num_remaining_stars;

	CIwGxFont* pfont;

	const char* title;
	CIwRect titlePos;
	uint32 titleColor;

	int level0_h;

	CIwTexture* three_stars_texture;
	bool firstLevelBought[3];

	float portrait_uvX;
	float portrait_uvY;

	bool bDoOnce;

	bool bIsPortraitFocused[3];

	string portrait_name;
	//0 - Name, 1 - Curr level stat, 2 - Next level stat 
	char current_level[10];
	char next_level[10];
	char health[3][20];
	char damage[3][20];
	char speed[3][20];
	char attack_speed[3][20];
	char cast_speed[3][20];
	char range[3][20];
	char cost[3][20];
	char move_cost[3][20];
	char skill_name[3][20];
	bool bValuesAreUnknow;

	bool bShowThirdCol;
	bool bShowInformation;

	void RenderStars(UnitUpgrades unitupgrade, int req, int level, int panel);
	void UpdateRemainingStars();
	void UpgradeSpecificUnit(UnitUpgrades* unitupgrade, int req[3], int id = 0);
	void UnlockCharacter(UnitUpgrades* unitupgrade, int cost);
	
	void LoadSpecficicInfo(UnitUpgrades unitupgrade, int idx);
	bool MouseOn(int x, int y, int xPos, int yPos, int _width, int _height);

	void UpdateInformationWindow();
	void SetAttributesToUnknow(int i);
	void SetAttributes(PawnAttributes att[], int level);
	void SetBonusAttributes(PawnAttributes att[], int level);

	CIwTexture* warrior_tab_selected;
	CIwTexture* mage_tab_selected;
	CIwTexture* bowler_tab_selected;
	CIwTexture* archer_tab_selected;

	bool bDrawFake_WarriorTab;
	bool bDrawFake_MageTab;
	bool bDrawFake_BowlerTab;
	bool bDrawFake_ArcherTab;
	bool bHasSkill;
	int last_tab_pos;

	CIwTexture* StoreTitlePannel_Texture;

	ButtonRenderData resetButtonData;
	ButtonRenderData lv2ButtonData;
	ButtonRenderData lv3ButtonData;
	RenderData descriptionBackgroundRD;
	int mainPanelWidth;
	int mainPanelHeight;
	RenderData portraitArray[8][5];
	ButtonRenderData buttonRenderData;
	ButtonRenderData btnStoreArcher, btnStoreBowler, btnStoreMage, btnStoreWarrior;
	RenderData btnStoreArcherSelected, btnStoreBowlerSelected, btnStoreMageSelected, btnStoreWarriorSelected;
	RenderData btnStoreLv1RenderData[2];
	RenderData pannel_store_title;
	RenderData starCounterData;
	RenderData panel;
	CIwSVec2 panelDimension;
	RenderData arrowStore;
};

#endif

