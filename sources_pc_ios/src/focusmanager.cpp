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

#include "focusmanager.h"

FocusManager::FocusManager()
{
	currentFocus = NULL;
	defaultFocus = NULL;
	bIsActive = false;
	lastObjectFocused = NULL;
}

FocusManager::~FocusManager()
{
	focusObjects.clear();
	focuseableObjectsArr.clear();
	bIsInsideArr.clear();
	bIsBlockedForUpdateMotion.clear();

	currentFocus = NULL;
	defaultFocus = NULL;
}

void FocusManager::UnableAllFocus()
{
	if(bIsActive)
	{
		for(int i = 0; i < (int)focusObjects.size(); i++)
		{
			focusObjects[i]->UnableFocus();
		}

		currentFocus = NULL;

		bIsActive = false;
	}
}

void FocusManager::BlockUpdateMotionFor(FocuseableObject* object)
{
	bIsBlockedForUpdateMotion[object->focusId] = true;
}

void FocusManager::ChangeFocus(XBOX360_Button button)
{
	bIsActive = true;

	if(currentFocus == NULL)
	{
		currentFocus = defaultFocus;
		currentFocus->EnableFocus();
	}

	FocuseableObject* tmpFocus = currentFocus;
	
	for(int i = 0; i < (int)focuseableObjectsArr[currentFocus->focusId].size(); i++)
	{
		pair<XBOX360_Button, int> currObj = focuseableObjectsArr[currentFocus->focusId][i];
		
		if(currObj.first == button)
		{
			currentFocus = findObjectWithId(currObj.second);

			if(currentFocus != NULL)
			{
				tmpFocus->UnableFocus();
				lastObjectFocused = currentFocus->focusId;
				currentFocus->EnableFocus();
				currentFocus->ExecuteOnFocused();
				SetDefaultFocus(currentFocus);
			}

			return;
		}
	}
}

FocuseableObject* FocusManager::GetLastObjectFocused()
{
	for(int i = 0; i < (int)focusObjects.size(); i++)
	{
		if(focusObjects[i]->focusId == lastObjectFocused)
			return focusObjects[i];
	}

	return NULL;
}

void FocusManager::ChangeFocus(FocuseableObject* nextFocus)
{
	if(nextFocus == currentFocus)
	{
		return;
	}

	bIsActive = true;

	if(currentFocus != NULL)
	{
		currentFocus->UnableFocus();
	}
	
	currentFocus = nextFocus;
	
	if(currentFocus != NULL)
	{
		currentFocus->EnableFocus();
		currentFocus->ExecuteOnFocused();
		lastObjectFocused = currentFocus->focusId;
	}

	defaultFocus = currentFocus;
}

void FocusManager::SetInitialFocus(FocuseableObject* object)
{
	ChangeFocus(object);
}

void FocusManager::SetDefaultFocus(FocuseableObject* object)
{
	defaultFocus = object;
}

void FocusManager::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	for(int i = 0; i < (int)focusObjects.size(); i++)
	{
		if(focusObjects[i] == NULL)
			continue;

		if(!bIsBlockedForUpdateMotion[focusObjects[i]->focusId] && focusObjects[i]->HandleFocusMotion(event))
		{
			ChangeFocus(focusObjects[i]);
		}
	}
}

void FocusManager::ClearFocusDirections()
{
	//focuseableObjectsArr.clear();
	focusObjects.clear();
	focuseableObjectsArr.clear();
	bIsInsideArr.clear();
	bIsBlockedForUpdateMotion.clear();
}

void FocusManager::ExecuteFocusedObject()
{
	if(currentFocus != NULL)
	{
		currentFocus->ExecuteFunction();
	}
}

FocuseableObject* FocusManager::findObjectWithId(int id)
{
	for(int i = 0; i < (int)focusObjects.size(); i++)
	{
		if(focusObjects[i]->focusId == id)
		{
			return focusObjects[i];
		}
	}

	return NULL;
}

void FocusManager::SetFocusDirection(XBOX360_Button button, FocuseableObject* sourceObject, FocuseableObject* destinyObject)
{
	if(button == XBOX360_Button_Directional)
	{
		SetFocusDirection(XBOX360_Button_Up, sourceObject, destinyObject);
		SetFocusDirection(XBOX360_Button_Down, sourceObject, destinyObject);
		SetFocusDirection(XBOX360_Button_Right, sourceObject, destinyObject);
		SetFocusDirection(XBOX360_Button_Left, sourceObject, destinyObject);

		return;
	}
	
	if(!bIsInsideArr[sourceObject->focusId])
	{
		bIsInsideArr[sourceObject->focusId] = true;
		focusObjects.push_back(sourceObject);
	}

	if(!bIsInsideArr[destinyObject->focusId])
	{
		bIsInsideArr[destinyObject->focusId] = true;
		focusObjects.push_back(destinyObject);
	}
	
	for(int i = 0; i < (int)focuseableObjectsArr[sourceObject->focusId].size(); i++)
	{
		if(focuseableObjectsArr[sourceObject->focusId][i].first == button)
		{
			focuseableObjectsArr[sourceObject->focusId].erase(focuseableObjectsArr[sourceObject->focusId].begin() + i);
		}
	}

	focuseableObjectsArr[sourceObject->focusId].push_back(make_pair(button, destinyObject->focusId));
}

void FocusManager::FocusBy2DPosition(int x, int y)
{
	s3ePointerMotionEvent event;
	
	event.m_x = x;
	event.m_y = y;

	for(int i = 0; i < (int)focusObjects.size(); i++)
	{
		if(focusObjects[i]->HandleFocusMotion(&event))
		{
			ChangeFocus(focusObjects[i]);
		}
	}
}
