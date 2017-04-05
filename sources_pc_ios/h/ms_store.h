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
#include "IwGxFont.h"
#include "common.h"
#include <string>
#include "utils.h"
using namespace std;

#if defined DESKTOPPLATFORM
#include "xbox360controller.h"
#include "focusmanager.h"
#endif

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

class StoreFocusObject : public FocuseableObject
{
public:
	StoreFocusObject()
	{
		bIsFocused = false;
		x = y = w = h = 0;
		onFocused = NULL;
		onExecute = NULL;
	}

	void EnableFocus()
	{
		bIsFocused = true;
	}

	void UnableFocus()
	{
		bIsFocused = false;
	}

	bool HandleFocusMotion(s3ePointerMotionEvent* event)
	{
		return Utils::MouseOn(event->m_x, event->m_y, x, y, w, h);
	}

	void ExecuteOnFocused()
	{
		if(onFocused != NULL)
			onFocused();
	}

	void ExecuteFunction()
	{
		if(onExecute != NULL)
			onExecute();
	}

	void (*onFocused)();
	void (*onExecute)();

	bool bIsFocused;
	int x, y, w, h;
	int i;
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
	bool bViewingDetailedInfo;

	void UpgradeUnit();

	void SetFocus(Store_Focus focus);
	void ResetFocus();

	void ResetStars();
	void LoadClassInfo();

	void FocusTab(Selknam_Tab t);

	void UnlockCharacterSecondCol();
	void UnlockCharacterThirdCol();

	void ClearFocus();

	//Detailed information popup.
	void SetImageAndText(int i, int j);
	void SetParagraph(const char* title, const char* firstPar);
	void OpenDetailedInfo();

	void ShowInformation(int i, int j)
	{
		bShowInformation = true;
		panel_pos._x = i;
		panel_pos._y = j;
	}

	bool bShowInformation;
	int realImageI;
	int realImageJ;
	int pannelBLengthX, pannelBLengthY;
	int xPosDetailed;
	int yPosDetailed, wDetailed, hDetailed;
	Button* button_panel[3][3];

#if defined DESKTOPPLATFORM
	Xbox360Controller xbox360Controller;
	FocusManager focusManager;
#endif

	StoreFocusObject portraits[3];

private:
	Background* background;
	Background* moreInfoBackground;

	CIwTexture* selknamRealImage[12];
	CIwTexture* currentSelectedImage;

	Button* button_tab[4];
	
	Button* button_back;
	Button* button_reset;

	char detailedInfoText[500];
	char detailedInfoTitle[500];

	CIwTexture* panel_b_tex;
	bool available_panel[3][3];

	bool gray_color[3];
	

	char remaining_stars[100];
	int remaining_stars_position_posX;
	int remaining_stars_position_posY;
	CIwRect remaining_stars_position_txt;
	uint32 remaining_stars_color;
	int num_remaining_stars;

	CIwGxFont* pfont;

	int level0_h;

	bool firstLevelBought[3];

	float portrait_uvX;
	float portrait_uvY;

	bool bDoOnce;

	bool bIsPortraitFocused[3];

	const char* portrait_name;
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
	char skill_name[3][20];
	bool bValuesAreUnknow;

	bool bShowThirdCol;

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

	void RenderSkillExplanation(int w, int h, int x, int y);
	IsoSelknamType GetCurrentSelknamSelected();

	bool bDrawFake_WarriorTab;
	bool bDrawFake_MageTab;
	bool bDrawFake_BowlerTab;
	bool bDrawFake_ArcherTab;
	bool bHasSkill;
	int last_tab_pos;
	int w, h, xPos, yPos;

	ButtonRenderData resetButtonData;
	ButtonRenderData lv2ButtonData;
	ButtonRenderData lv3ButtonData;
	RenderData portraitArray[8][5];
	ButtonRenderData buttonRenderData;
	ButtonRenderData btnStoreArcher, btnStoreBowler, btnStoreMage, btnStoreWarrior;
	RenderData btnStoreArcherSelected, btnStoreBowlerSelected, btnStoreMageSelected, btnStoreWarriorSelected;
	RenderData btnStoreLv1RenderData[2];
	RenderData starCounterData;
	RenderData arrowStore;

	ButtonRenderData selknamInfoRenderData;
	Button* selknamInfoButton;
	ButtonRenderData* button;
	Button* btnClose;

	int titleXPos;
	int titleYPos;
	int pannelLeftXPos;
	int pannelLeftYPos;
	int pannelRightXPos;
	int pannelRightYPos;
	int portraitXPos;
	int portraitYPos;
	int infoXOffset;
	int infoYOffset;

	CIwRect skillNamePosition;
	CIwRect skillExpPopupRect;
	bool bShowSkillExpPopup;

	int xText;
	int yText;
	int secondColDiff;
	int thirdColDiff;
	int padding;

	RenderData icon_att_attributes;
	RenderData icon_att_attacksp;
	RenderData icon_att_castsp;
	RenderData icon_att_cost;
	RenderData icon_att_damage;
	RenderData icon_att_health;
	RenderData icon_att_range;
	RenderData icon_att_speed;

	RenderData icon_skill_heal;
	RenderData icon_skill_haste;
	RenderData icon_skill_might;
	RenderData icon_skill_poison;
	RenderData icon_skill_slow;
	RenderData icon_skill_stun;
	RenderData icon_skill_root;
	RenderData icon_skill_confusion;
};

#endif

