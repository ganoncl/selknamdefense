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

#ifndef _FOCUSEABLEOBJECT_H
#define _FOCUSEABLEOBJECT_H

#include "s3e.h"

class FocuseableObject
{
public:
	FocuseableObject();
	virtual void EnableFocus();
	virtual void UnableFocus();
	virtual void ExecuteFunction();
	virtual bool HandleFocusMotion(s3ePointerMotionEvent* event);
	virtual void ExecuteOnFocused();

	int focusId;
};

#endif