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

#ifndef _ENDINGWIN_H
#define _ENDINGWIN_H

#include "IwGx.h"
#include "IwGxFont.h"
#include "button.h"
#include "timer.h"
#include "background.h"
#include "dynamiccounter.h"
#include "texturehelper.h"

class EndingWin
{
public:
	EndingWin();
	~EndingWin();
	void Update();
	void Render();
	void InitObject();
	void EndObject();
	bool bIsActiveObj;

	void SetTimeBonus(int score);
	void SetStarsBonus(int score);
	void SetLevelBonus(int score);
	void SetCoinsBonus(int score);
	void SetLifes(int lifes) {this->lifes = lifes; };
	void UpdateInfo();

	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	bool bMouseBlocked;
	bool bKeyboardBlocked;
	s3eKey lastKeyPressed;

	Timer time_victory_picture;
	bool bShowVictoryPicture;

private:
	//Background
	CIwColour cols[4];
	CIwSVec2 pos[4];
	int _width;
	int _height;
	int _x;
	int _y;

	CIwGxFont* pfont;
	uint32 textColor;
	CIwRect titlePos;
	CIwRect timePos;
	CIwRect starsPos;
	CIwRect levelPos;
	CIwRect coinsPos;
	CIwRect totalPos;

	CIwRect timePos_n;
	CIwRect starsPos_n;
	CIwRect levelPos_n;
	CIwRect coinsPos_n;
	CIwRect totalPos_n;

	const char* title;
	char timeText[100];
	char starsText[100];
	char levelText[100];
	char coinsText[100];
	char totalText[100];

	char timeText_n[100];
	char starsText_n[100];
	char levelText_n[100];
	char coinsText_n[100];
	char totalText_n[100];

	int timeBonus;
	int starsBonus;
	int levelBonus;
	int coinsBonus;
	int total;
	int lifes;

	int current_stars_value;
	int current_level_value;
	int current_coins_value;

	int stars_speed;
	int level_speed;
	int coins_speed;

	Button* btnContinue;

	CIwSVec2 star_position[3][3];
	bool bRenderStars;
	int num_stars;
	int current_stars;
	Timer star_timer;

	CIwSVec2 panelPosition;
	Background* background;

	bool bBlockContinueButton;

	DynamicCounter heart_counter;
	DynamicCounter coins_counter;
	DynamicCounter wave_counter;
	DynamicCounter total_counter;

	CIwSVec2 VictoryPosition;
	RenderData VictoryRenderData;
	RenderData rdBackground;
	int panelWidth;

	ButtonRenderData buttonRenderData;
	RenderData label_star_big;
};

#endif

