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

#ifndef _LEADERBOARDLIST_H
#define _LEADERBOARDLIST_H

#include "lboardentry.h"
#include "utils.h"

#include <vector>
using namespace std;

#define MAX_ENTRIES 10

typedef enum
{
	Filter_Normal,
	Filter_Hover,
	Filter_Click
} FilterState;

typedef enum
{
	Filter_Overall,
	Filter_Friends,
	Filter_Me,
	Filter_Count = 3
} FilterType;

typedef enum
{
	FilterLBName_Survival,
	FilterLBName_Campaign,
	FilterLBName_Count = 2
} FilterLBNameType;

class GUIFilter : public FocuseableObject
{
public:
	GUIFilter()
	{
		bIsFocused = false;
		executeFnc = NULL;
		x = y = w = h = 0;
	};
	
	void EnableFocus()
	{
		bIsFocused = true;
	};

	void UnableFocus()
	{
		bIsFocused = false;
	};

	void ExecuteFunction()
	{
		if(executeFnc != NULL)
			executeFnc();
	}

	bool HandleFocusMotion(s3ePointerMotionEvent* event)
	{
		return Utils::MouseOn(event->m_x, event->m_y, x, y, w, h);	
	}

	int x, y, w, h;
	bool bIsFocused;

	void (*executeFnc)();
};

class LboardList
{
public:
	LboardList(int xPos, int yPos);
	~LboardList();

	void AddEntry(LboardEntry newEntry);
	void ClearAllEntries();
	void SetSelectEntryCallback(void (*selectEntryCallback)(LboardEntry));
	void SetOnLoadListCallback(void (*onLoadList)()) { this->onLoadList = onLoadList; };
	void SetRangeToRender(int start, int end);
	void SetPosition(int x, int y);
	void LoadUserList(int page);

	void NextPage();
	void PrevPage();

	void Render();
	void Update();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);

	vector<LboardEntry> GetEntryList() { return entryList; };
	LboardEntry* GetEntryAt(int i) { return &entryList[i]; };
	int GetEntryListSize() { return (int)entryList.size(); };
	FocuseableObject* GetGUIFilter() { return &guiFilter; };
	FocuseableObject* GetGUILBNameFilter() { return &guiLBNameFilter; }
	void NextFilter();
	void NextLBNameFilter();

	bool ExistsNextPage() { return btnNextPage->IsEnabled(); };
	bool ExistsPrevPage() { return btnPrevPage->IsEnabled(); };
	Button* GetNextPageButton() { return btnNextPage; };
	Button* GetPrevPageButton() { return btnPrevPage; };

	bool LeaderboardEnabled();

	void LoadLeaderboard(FilterLBNameType type);

	bool bLoadUserList;

private:
	vector<LboardEntry> entryList;

	void (*selectEntryCallback)(LboardEntry);

	void (*onLoadList)();

	int startEntryIndex;
	int endEntryIndex;
	int entryListXpos;
	int entryListYpos;
	int singleEntryWidth;
	int singleEntryHeight;
	int xPos;
	int yPos;
	int realPannelX;
	int realPannelY;
	int rankColumnWidth;
	int nameColumnWidth;
	int scoreColumnWidth;
	int pannelBorder;
	int width;
	int height;
	int currPage;
	int yFirstRow;

	FilterState currFilterState;
	FilterType currFilterType;

	FilterState currLBNameFilterState;
	FilterLBNameType currLBNameFilterType;

	CIwColour normal, hover, click;

	ButtonRenderData popupBtnRenderData;
	int buttonWidth;
	Button* btnPrevPage;
	Button* btnNextPage;

	string nullName;

	CIwRect highlightedEntry;
	int highlightedEntryIndex;
	CIwSVec2 rankPos;
	CIwSVec2 namePos;
	CIwSVec2 scorePos;

	bool bGetScoreboard;

	GUIFilter guiFilter;
	GUIFilter guiLBNameFilter;

	//Campaign table; ct_ := campaign table
	CIwSVec2 ct_columnPos[4];
	int ct_columnWidth[4];

	void SetEntryListParameters();
	bool IsAValidEntryIndex(int idx);
	void RenderRow(string rank, string name, string score, int i);
	CIwColour GetFilterColour();
	string GetFilterText();
	string GetLeaderboardNameText();
	int GetFilterLeaderboard();
	BarType GetFilterBarType();
	BarType GetLeaderboardNameBarType();

	void RenderSurvivalTable();
	void RenderCampaignTable();
	void RenderSurvivalTableLabel();
	void RenderCampaignTableLabel();

	void RenderCampaignTableRow(string rank, string name, int score, int i);

	int filterTypeYPos;
	int lbNameYPos;
};

#endif