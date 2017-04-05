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

#ifndef _CHECKBOX_H
#define _CHECKBOX_H

#include "common.h"
#include "focuseableobject.h"

class Checkbox : public FocuseableObject
{
public:
	Checkbox();
	~Checkbox();
	void Render();
	void SetPosition(int _x, int _y);
	void UpdateClickEvents(s3ePointerEvent* event);
	bool GetIsChecked();
	void SetIsChecked(bool b);
	void EnableFocus();
	void UnableFocus();
	void ExecuteFunction();
	bool HandleFocusMotion(s3ePointerMotionEvent* event);

private:
	int x;
	int y;
	int w;
	int h;
	RenderData boxRD;
	RenderData boxhRD;
	RenderData okRD;
	bool bIsChecked;
	bool bIsFocused;
};

#endif