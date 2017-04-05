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

#include "lboardlist.h"
#include "common.h"
#include "localization.h"
#include "stdio.h"
#ifdef STEAMREQUIRED
#include "steam/steam_api.h"
#include "helpers/steam_achievements.h"
#include "helpers/steam_leaderboards.h"
#endif

LboardList* lboardListRef = NULL;

void LboardList_NextPage()
{
	if(lboardListRef == NULL)
		return;

	lboardListRef->NextPage();
}

void LboardList_PrevPage()
{
	if(lboardListRef == NULL) 
		return;

	lboardListRef->PrevPage();
}

void LboardList_NextFilter()
{
	if(lboardListRef == NULL) 
		return;

	lboardListRef->NextFilter();
}

void LboardList_NextLBNameFilter()
{
	if(lboardListRef == NULL)
		return;

	lboardListRef->NextLBNameFilter();
}

LboardList::LboardList(int xPos, int yPos)
{
	lboardListRef = this;

	ClearAllEntries();

	rankColumnWidth = 180;
	nameColumnWidth = 360;
	scoreColumnWidth = 180;
	pannelBorder = 40;

	width = pannelBorder * 18;
	height = pannelBorder * 9;

	SetEntryListParameters();
	SetPosition(xPos, yPos);

	highlightedEntryIndex = 2;

	TextureData buttonData = TextureHelper::GetTextureNamed("btn_popup_long"); 
	popupBtnRenderData.numStates = 1;
	popupBtnRenderData.state[0].RollOut  = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	popupBtnRenderData.state[0].RollOn   = TextureHelper::GetRenderData(buttonData, 1, 3, 1);
	popupBtnRenderData.state[0].Pressed  = TextureHelper::GetRenderData(buttonData, 1, 3, 2);
	popupBtnRenderData.state[0].Inactive = TextureHelper::GetRenderData(buttonData, 1, 3, 0);
	buttonWidth = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).x;
	int buttonHeight = TextureHelper::GetDimension(TextureHelper::GetRenderData(buttonData, 1, 3, 0)).y;

	btnPrevPage = new Button(TextureHelper::GetCIwTexture(popupBtnRenderData.state[0].RollOut.atlasName), &popupBtnRenderData, realPannelX, entryListYpos + 11 * pannelBorder - 20);
	btnPrevPage->OnClick(&LboardList_PrevPage);
	btnNextPage = new Button(TextureHelper::GetCIwTexture(popupBtnRenderData.state[0].RollOut.atlasName), &popupBtnRenderData, realPannelX + 18 * pannelBorder - buttonWidth + 45, entryListYpos + 11 * pannelBorder - 20);
	btnNextPage->OnClick(&LboardList_NextPage);

	btnPrevPage->SetStyle(font_SmartKid18, 0, -17);
	btnPrevPage->SetTextOffsetValueByState(1);
	btnNextPage->SetStyle(font_SmartKid18, 0, -17);
	btnNextPage->SetTextOffsetValueByState(1);

	btnPrevPage->SetText(loc_leaderboards_prevpage);
	btnNextPage->SetText(loc_leaderboards_nextpage);

	bGetScoreboard = false;

	yFirstRow = realPannelY + singleEntryHeight * 2 + 32;

	rankPos.x = realPannelX;
	rankPos.y = yFirstRow;

	namePos.x = realPannelX + rankColumnWidth;
	namePos.y = yFirstRow;

	scorePos.x = realPannelX + rankColumnWidth + nameColumnWidth;
	scorePos.y = yFirstRow;

	ct_columnWidth[0] = 180;
	ct_columnPos[0] = CIwSVec2(realPannelX, yFirstRow);
	ct_columnWidth[1] = 360;
	ct_columnPos[1] = CIwSVec2(realPannelX + ct_columnWidth[0], yFirstRow);
	ct_columnWidth[2] = 90;
	ct_columnPos[2] = CIwSVec2(realPannelX + ct_columnWidth[0] + ct_columnWidth[1], yFirstRow);
	ct_columnWidth[3] = 90;
	ct_columnPos[3] = CIwSVec2(realPannelX + ct_columnWidth[0] + ct_columnWidth[1] + ct_columnWidth[2], yFirstRow);

	currFilterState = Filter_Normal;
	currFilterType = Filter_Overall;

	currLBNameFilterState = Filter_Normal;
	currLBNameFilterType = FilterLBName_Campaign;

	normal.Set(186, 93, 93);
	hover.Set(93, 93, 93);
	click.Set(93, 93, 0);

	onLoadList = NULL;

	lbNameYPos = realPannelY + 12 + 32;
	filterTypeYPos = realPannelY + 8;

	guiFilter.executeFnc = &LboardList_NextFilter;
	guiFilter.x = realPannelX + 13;
	guiFilter.y = filterTypeYPos;
	guiFilter.w = 21 * 32;
	guiFilter.h = 32;

	guiLBNameFilter.executeFnc = &LboardList_NextLBNameFilter;
	guiLBNameFilter.x = realPannelX + 13;
	guiLBNameFilter.y = lbNameYPos;
	guiLBNameFilter.w = 21 * 32;
	guiLBNameFilter.h = 32;

	bLoadUserList = false;
}

LboardList::~LboardList()
{
	delete btnNextPage;
	delete btnPrevPage;
}

void LboardList::NextPage()
{
	LoadUserList(currPage + 1);
}

void LboardList::PrevPage()
{
	LoadUserList(currPage - 1);
}

void LboardList::AddEntry(LboardEntry newEntry)
{
	entryList.push_back(newEntry);
}

void LboardList::ClearAllEntries()
{
	entryList.clear();
}

CIwColour LboardList::GetFilterColour()
{
	if(currFilterState == Filter_Normal) return normal;
	if(currFilterState == Filter_Hover)  return hover;
	if(currFilterState == Filter_Click)  return click;
	return CIwColour();
}

void LboardList::SetSelectEntryCallback(void (*selectEntryCallback)(LboardEntry))
{
	this->selectEntryCallback = selectEntryCallback;
}

string LboardList::GetFilterText()
{
	if(currFilterType == Filter_Overall) return loc_leaderboards_overall;
	else if(currFilterType == Filter_Friends) return loc_leaderboards_friends;
	else if(currFilterType == Filter_Me)  return loc_leaderboards_myrank;
	return "";
}

void LboardList::LoadLeaderboard(FilterLBNameType type)
{
#if defined STEAMREQUIRED
	if(!g_SteamLeaderboards)
		return;

	if(type == FilterLBName_Survival)
		g_SteamLeaderboards->FindLeaderboard("Survival");
	else if(type == FilterLBName_Campaign)
		g_SteamLeaderboards->FindLeaderboard("Campaign_sd_");

	bLoadUserList = true;
#endif
}

void LboardList::LoadUserList(int page)
{
	currPage = page;

	ClearAllEntries();

#ifdef STEAMREQUIRED
	if(g_SteamLeaderboards != NULL)
	{
		int filter = GetFilterLeaderboard();
		int tableOffset = 0;
		if(filter == (int)k_ELeaderboardDataRequestGlobalAroundUser)
		{
			tableOffset = MAX_ENTRIES / 2;
		}

		g_SteamLeaderboards->DownloadScores(GetFilterLeaderboard(), MAX_ENTRIES * page - tableOffset + 1, MAX_ENTRIES * (page + 1) + 1 - tableOffset);
		bGetScoreboard = true;
	}
#endif

	btnNextPage->Enabled(false);
	btnPrevPage->Enabled(false);
}

bool LboardList::LeaderboardEnabled()
{
#if defined STEAMREQUIRED
	return !bGetScoreboard 
		    && g_SteamLeaderboards != NULL
			&& g_SteamLeaderboards->LeaderboardEnabled();
#endif
	return false;
}

int LboardList::GetFilterLeaderboard()
{
	if(currFilterType == Filter_Overall) return 0;
	if(currFilterType == Filter_Friends) return 2;
	if(currFilterType == Filter_Me) return 1;

	return 0;
}

string LboardList::GetLeaderboardNameText()
{
	if(currLBNameFilterType == FilterLBName_Campaign) 
		return loc_leaderboards_campaign;
	else if(currLBNameFilterType == FilterLBName_Survival)
		return loc_leaderboards_survival;

	return "NO NAME";
}

void LboardList::Render()
{
	btnPrevPage->Render();
	btnNextPage->Render();

	IwGxFlush();

	Utils::RenderPannel(PannelType_C, CIwRect(entryListXpos, entryListYpos, 18, 9));

	IwGxFlush();

	/*
	if(!bGetScoreboard)
	{
		if(IsAValidEntryIndex(highlightedEntryIndex) && entryList[highlightedEntryIndex].bIsFocused)
		{
			Utils::RenderColor(255, 255, 255, 196, CIwSVec2(realPannelX + 20, yFirstRow + 3 + singleEntryHeight * (highlightedEntryIndex + 1)), CIwSVec2(singleEntryWidth, singleEntryHeight));
		}
	}
	*/


	IwGxLightingOn();

	IwGxFlush();

	Utils::RenderBar(GetLeaderboardNameBarType(), 21, realPannelX + 13, lbNameYPos);
	Utils::RenderBar(GetFilterBarType(), 21, realPannelX + 13, filterTypeYPos);

	IwGxFlush();
	//CIwColour tmpColor = GetFilterColour();

	//Utils::RenderColor(tmpColor.r, tmpColor.g, tmpColor.b, 196, CIwSVec2(realPannelX + 20, realPannelY + 12), CIwSVec2(singleEntryWidth, (int)(singleEntryHeight * 1.2f)));
	Utils::RenderText(GetLeaderboardNameText().c_str(),
		CIwRect(realPannelX, realPannelY + 32, width, singleEntryHeight * 2),
		font_SmartKid18,
		0xffffffff);

	Utils::RenderText(GetFilterText().c_str(),
		CIwRect(realPannelX, realPannelY - 4, width, singleEntryHeight * 2),
		font_SmartKid18,
		0xffffffff);

	IwGxFlush();

	if(currLBNameFilterType == FilterLBName_Survival)
		RenderSurvivalTableLabel();
	else if(currLBNameFilterType == FilterLBName_Campaign)
		RenderCampaignTableLabel();

	IwGxFlush();

	if(!LeaderboardEnabled())
	{
		//IwTrace(LEADERBOARDS, ("Loaderboard loading..."));
		Utils::RenderText(loc_leaderboards_notAvailable,
			CIwRect(realPannelX, realPannelY, width, height),
			font_SmartKid18,
			0xffffffff);
	}
	else
	{
		if(currLBNameFilterType == FilterLBName_Survival)
			RenderSurvivalTable();
		else if(currLBNameFilterType == FilterLBName_Campaign)
			RenderCampaignTable();
	}

	IwGxLightingOff();
}

void LboardList::RenderSurvivalTableLabel()
{
	Utils::RenderText(loc_leaderboards_rank,
		CIwRect(rankPos.x, rankPos.y, rankColumnWidth, singleEntryHeight),
		font_SmartKid16,
		0xffffffff);

	Utils::RenderText(loc_leaderboards_playername,
		CIwRect(namePos.x, namePos.y, nameColumnWidth, singleEntryHeight),
		font_SmartKid16,
		0xffffffff);

	Utils::RenderText(loc_leaderboards_time,
		CIwRect(scorePos.x, scorePos.y, scoreColumnWidth, singleEntryHeight),
		font_SmartKid16,
		0xffffffff);
}

void LboardList::RenderCampaignTableLabel()
{
	Utils::RenderText(loc_leaderboards_rank,
		CIwRect(ct_columnPos[0].x, ct_columnPos[0].y, ct_columnWidth[0], singleEntryHeight),
		font_SmartKid16,
		0xffffffff);

	Utils::RenderText(loc_leaderboards_playername,
		CIwRect(ct_columnPos[1].x, ct_columnPos[1].y, ct_columnWidth[1], singleEntryHeight),
		font_SmartKid16,
		0xffffffff);

	Utils::RenderText(loc_leaderboards_stage,
		CIwRect(ct_columnPos[2].x, ct_columnPos[2].y, ct_columnWidth[2], singleEntryHeight),
		font_SmartKid16,
		0xffffffff);

	Utils::RenderText(loc_leaderboards_score,
		CIwRect(ct_columnPos[3].x, ct_columnPos[3].y, ct_columnWidth[3], singleEntryHeight),
		font_SmartKid16,
		0xffffffff);
}

void LboardList::RenderSurvivalTable()
{
	for(int i = startEntryIndex; i < endEntryIndex; i++)
	{
		string nameToShow = nullName;
		//char scoreToShow[50];
		char tempInt[20];

		if(IsAValidEntryIndex(i))
		{
			nameToShow = entryList[i].playerName;
			sprintf(tempInt, "%d", entryList[i].rank);
			//sprintf(scoreToShow, "%d", entryList[i].score);

			RenderRow(tempInt, nameToShow, Utils::GetClockFormatMS(entryList[i].score), i);
		}
		else
		{
			RenderRow(nullName, nullName, nullName, i);
		}
	}
}

void LboardList::RenderCampaignTable()
{
	for(int i = startEntryIndex; i < endEntryIndex; i++)
	{
		string nameToShow = nullName;
		//char scoreToShow[50];
		char tempInt[20];

		if(IsAValidEntryIndex(i))
		{
			nameToShow = entryList[i].playerName;
			sprintf(tempInt, "%d", entryList[i].rank);
			//sprintf(scoreToShow, "%d", entryList[i].score);

			RenderCampaignTableRow(tempInt, nameToShow, entryList[i].score, i);
		}
		else
		{
			RenderCampaignTableRow(nullName, nullName, -1, i);
		}

		IwGxFlush();
	}
}

void LboardList::RenderCampaignTableRow(string rank, string name, int iScore, int i)
{
	int32 color = 0xffffffff;
	/*
	if(highlightedEntryIndex == i && activeControllerType == Controller_MOUSE
	  || (IsAValidEntryIndex(i) && entryList[i].bIsFocused)) color = 0xff0da8d9;
	*/
	//if(IsAValidEntryIndex(i) && entryList[i].bIsFocused) color = 0xff0da8d9;

	string stage = nullName;
	string score = nullName;

	if(iScore > 0)
	{
		LBCampaignScore tmpScore = Utils::UnMaskLeadeboardScore(iScore);

		char toString[100];
		sprintf(toString, "%d", tmpScore.level);
		stage = toString;

		sprintf(toString, "%d", tmpScore.totalScore);
		score = toString;
	}

	if(IsAValidEntryIndex(i) && entryList[i].bIsFocused) 
	{
		color = 0xff0da8d9;
		Utils::RenderColor(255, 255, 255, 196, CIwSVec2(realPannelX + 20, yFirstRow + 3 + singleEntryHeight * (i + 1)), CIwSVec2(singleEntryWidth, singleEntryHeight));
	}

	if(IsAValidEntryIndex(i))
	{
		entryList[i].w = singleEntryWidth;
		entryList[i].h = singleEntryHeight;
		entryList[i].x = realPannelX + 20;
		entryList[i].y = yFirstRow + 3 + singleEntryHeight * (i + 1);
	}

	Utils::RenderText(rank.c_str(),
		CIwRect(ct_columnPos[0].x, ct_columnPos[0].y + (i + 1) * singleEntryHeight, ct_columnWidth[0], singleEntryHeight),
		font_SmartKid16,
		color);

	Utils::RenderText(name.c_str(),
		CIwRect(ct_columnPos[1].x, ct_columnPos[1].y + (i + 1) * singleEntryHeight, ct_columnWidth[1], singleEntryHeight),
		font_SmartKid16,
		color);

	Utils::RenderText(stage.c_str(),
		CIwRect(ct_columnPos[2].x, ct_columnPos[1].y + (i + 1) * singleEntryHeight, ct_columnWidth[2], singleEntryHeight),
		font_SmartKid16,
		color);

	Utils::RenderText(score.c_str(),
		CIwRect(ct_columnPos[3].x, ct_columnPos[1].y + (i + 1) * singleEntryHeight, ct_columnWidth[3], singleEntryHeight),
		font_SmartKid16,
		color);

	/*
	Utils::RenderText(rank.c_str(),
		CIwRect(rankPos.x, rankPos.y + (i + 1) * singleEntryHeight, rankColumnWidth, singleEntryHeight),
		font_SmartKid16,
		color);

	Utils::RenderText(name.c_str(),
		CIwRect(namePos.x, namePos.y + (i + 1) * singleEntryHeight, nameColumnWidth, singleEntryHeight),
		font_SmartKid16,
		color);

	Utils::RenderText(score.c_str(),
		CIwRect(scorePos.x, scorePos.y + (i + 1) * singleEntryHeight, scoreColumnWidth, singleEntryHeight),
		font_SmartKid16,
		color);
		*/
}

void LboardList::RenderRow(string rank, string name, string score, int i)
{
	int32 color = 0xffffffff;
	/*
	if(highlightedEntryIndex == i && activeControllerType == Controller_MOUSE
	  || (IsAValidEntryIndex(i) && entryList[i].bIsFocused)) color = 0xff0da8d9;
	*/
	//if(IsAValidEntryIndex(i) && entryList[i].bIsFocused) color = 0xff0da8d9;

	if(IsAValidEntryIndex(i) && entryList[i].bIsFocused) 
	{
		color = 0xff0da8d9;
		Utils::RenderColor(255, 255, 255, 196, CIwSVec2(realPannelX + 20, yFirstRow + 3 + singleEntryHeight * (i + 1)), CIwSVec2(singleEntryWidth, singleEntryHeight));
	}

	if(IsAValidEntryIndex(i))
	{
		entryList[i].w = singleEntryWidth;
		entryList[i].h = singleEntryHeight;
		entryList[i].x = realPannelX + 20;
		entryList[i].y = yFirstRow + 3 + singleEntryHeight * (i + 1);
	}


	Utils::RenderText(rank.c_str(),
		CIwRect(rankPos.x, rankPos.y + (i + 1) * singleEntryHeight, rankColumnWidth, singleEntryHeight),
		font_SmartKid16,
		color);

	Utils::RenderText(name.c_str(),
		CIwRect(namePos.x, namePos.y + (i + 1) * singleEntryHeight, nameColumnWidth, singleEntryHeight),
		font_SmartKid16,
		color);

	Utils::RenderText(score.c_str(),
		CIwRect(scorePos.x, scorePos.y + (i + 1) * singleEntryHeight, scoreColumnWidth, singleEntryHeight),
		font_SmartKid16,
		color);

	IwGxFlush();
}

bool LboardList::IsAValidEntryIndex(int idx)
{
	return idx >= 0 && idx < (int)entryList.size();
}

void LboardList::SetPosition(int x, int y)
{
	entryListXpos = x;
	entryListYpos = y;

	realPannelX = entryListXpos + 20;
	realPannelY = entryListYpos + 30;
}

void LboardList::SetRangeToRender(int start, int end)
{
	this->startEntryIndex = start;
	this->endEntryIndex = end;
}

void LboardList::SetEntryListParameters()
{
	SetRangeToRender(0, MAX_ENTRIES);

	nullName = "---";

	singleEntryWidth = nameColumnWidth + scoreColumnWidth + rankColumnWidth;
	singleEntryHeight = 25;

	highlightedEntry.w = 400;
	highlightedEntry.h = 25;
	highlightedEntry.x = -1;
	highlightedEntry.y = -1;
	highlightedEntryIndex = -1;
}

void LboardList::NextFilter()
{
	if(!LeaderboardEnabled())
		return;

	currFilterType = (FilterType)(((int)currFilterType + 1) % Filter_Count);
	LoadUserList(0);
}

void LboardList::NextLBNameFilter()
{
	if(!LeaderboardEnabled())
		return;

	currLBNameFilterType = (FilterLBNameType)(((int)currLBNameFilterType + 1) % FilterLBName_Count);
	LoadLeaderboard(currLBNameFilterType);
}

void LboardList::UpdateClickEvents(s3ePointerEvent *event)
{
	if(event->m_Button != S3E_POINTER_BUTTON_LEFTMOUSE)
		return;

	btnPrevPage->HandleClick(event);
	btnNextPage->HandleClick(event);

	if(currFilterState != Filter_Hover) currFilterState = Filter_Normal;
	if(currLBNameFilterState != Filter_Hover) currLBNameFilterState = Filter_Normal;

	if(!event->m_Pressed) return;
	
	if(LeaderboardEnabled())
	{
		if(Utils::MouseOn(event->m_x, event->m_y, realPannelX + 20, lbNameYPos, singleEntryWidth, (int)(singleEntryHeight * 1.2f)))
		{
			currLBNameFilterState = Filter_Click;
			NextLBNameFilter();
		} 
		else if(Utils::MouseOn(event->m_x, event->m_y, realPannelX + 20, filterTypeYPos, singleEntryWidth, (int)(singleEntryHeight * 1.2f)))
		{
			currFilterState = Filter_Click;
			NextFilter();
		}
	}

#ifdef STEAMREQUIRED
	if(g_SteamLeaderboards != NULL)
	{
		for(int i = startEntryIndex; i <= endEntryIndex; i++)
		{
			if(Utils::MouseOn(event->m_x, event->m_y, realPannelX, yFirstRow + (i + 1) * singleEntryHeight, singleEntryWidth, singleEntryHeight))
			{
				int userIndex = i - startEntryIndex;
				if(g_SteamLeaderboards->m_nLeaderboardEntries > userIndex)
				{
					SteamFriends()->ActivateGameOverlayToUser("steamid", g_SteamLeaderboards->m_leaderboardEntries[userIndex].m_steamIDUser);
				}

				break;
			}
		}
	}
#endif
}

void LboardList::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	btnPrevPage->HandleMotion(event);
	btnNextPage->HandleMotion(event);

	//highlightedEntryIndex = -1;
	/*
	for(int i = startEntryIndex; i <= endEntryIndex; i++)
	{
		if(Utils::MouseOn(event->m_x, event->m_y, realPannelX, yFirstRow + (i + 1) * singleEntryHeight, singleEntryWidth, singleEntryHeight))
		{
			highlightedEntryIndex = i;
			break;
		}
	}
	*/
	/*
	if(currFilterState != Filter_Click)
	{
		if(Utils::MouseOn(event->m_x, event->m_y, realPannelX + 20, realPannelY + 12, singleEntryWidth, (int)(singleEntryHeight * 1.2f)))
		{
			currFilterState = Filter_Hover;
		}
		else
		{
			currFilterState = Filter_Normal;
		}
	}
	*/
}

BarType LboardList::GetFilterBarType()
{
	if(currFilterState == Filter_Hover) return BarType_B;
	if(currFilterState == Filter_Click) return BarType_B;

	return BarType_A;
}

BarType LboardList::GetLeaderboardNameBarType()
{
	if(currLBNameFilterState == Filter_Hover) return BarType_B;
	if(currLBNameFilterState == Filter_Click) return BarType_B;

	return BarType_A;
}

void LboardList::Update()
{
	if(guiFilter.bIsFocused)
		currFilterState = Filter_Hover;
	else
		currFilterState = Filter_Normal;
	
	if(guiLBNameFilter.bIsFocused)
		currLBNameFilterState = Filter_Hover;
	else
		currLBNameFilterState = Filter_Normal;

	//Wait for m_CurrentLeaderboard to be not null, then calls LoadUserList to load steam users.
	if(bLoadUserList)
	{
		if(LeaderboardEnabled())
		{
			bLoadUserList = false;
			LoadUserList(0);
		}
	}

	if(bGetScoreboard)
	{
#ifdef STEAMREQUIRED
		if(!g_SteamLeaderboards->GetDownloadingScore())
		{
			bGetScoreboard = false;
			int numEntries = g_SteamLeaderboards->m_nLeaderboardEntries;

			btnNextPage->Enabled(numEntries >= MAX_ENTRIES);
			btnPrevPage->Enabled(currPage > 0);

			for(int i = 0; i < numEntries; i++)
			{
				LeaderboardEntry_t currEntry = g_SteamLeaderboards->m_leaderboardEntries[i];
				int rank = currEntry.m_nGlobalRank;
				string playerName =  SteamFriends()->GetFriendPersonaName(currEntry.m_steamIDUser);
				int score = currEntry.m_nScore;
				AddEntry(LboardEntry(playerName, score, rank, i));
			}

			if(onLoadList != NULL)
				onLoadList();
		}
#endif
	}
}