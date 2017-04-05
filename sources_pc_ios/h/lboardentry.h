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

#ifndef _LEADERBOARDENTRY_H
#define _LEADERBOARDENTRY_H

#include <string>
#include "focuseableobject.h"
using namespace std;

class LboardEntry : public FocuseableObject
{
public:
	LboardEntry(string playerName, int score, int rank = 1, int entryID = -1);
	~LboardEntry();

	void EnableFocus();
	void UnableFocus();
	void ExecuteFunction();
	bool HandleFocusMotion(s3ePointerMotionEvent* event);

	string playerName;
	int score;
	int rank;
	bool bIsFocused;
	int entryID;
	int x, y, w, h;
};

#endif