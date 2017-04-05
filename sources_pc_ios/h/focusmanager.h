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

#ifndef _FOCUSMANAGER_H
#define _FOCUSMANAGER_H

#include "IwGx.h"
#include "s3e.h"

#include "common.h"
#include "focuseableobject.h"

#include <vector>
#include <map>
using namespace std;

class FocusManager
{
public:
	FocusManager();
	~FocusManager();
	void ChangeFocus(XBOX360_Button button);
	void ChangeFocus(FocuseableObject* nextFocus);
	void SetFocusDirection(XBOX360_Button button, FocuseableObject* sourceObject, FocuseableObject* destinyObject);
	void SetDefaultFocus(FocuseableObject* object);
	void ExecuteFocusedObject();
	void UnableAllFocus();
	void ClearFocusDirections();
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void FocusBy2DPosition(int x, int y);
	bool IsDefaultFocusSetted() { return defaultFocus != NULL; };
	FocuseableObject* GetDefaultFocus() { return defaultFocus; };
	void SetInitialFocus(FocuseableObject* object);
	void BlockUpdateMotionFor(FocuseableObject* object);
	FocuseableObject* GetLastObjectFocused();

private:
	FocuseableObject* findObjectWithId(int id);
	int lastObjectFocused;

	vector<FocuseableObject*> focusObjects;
	map<int, vector< pair<XBOX360_Button, int> > > focuseableObjectsArr;
	map<int, bool> bIsInsideArr;
	map<int, bool> bIsBlockedForUpdateMotion;

	FocuseableObject* currentFocus;
	FocuseableObject* defaultFocus;

	bool bIsActive;
};

#endif