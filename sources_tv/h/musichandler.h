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

#ifndef _MusicHandler_H
#define _MusicHandler_H

#include "musicbutton.h"
#include "utils.h"

class MusicHandler {
public:
	MusicHandler(int x, int y);
	~MusicHandler();

	void Render();
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateClickEvents(s3ePointerEvent *event);
	void KeyboardHandler(s3eKeyboardEvent* event);
	void MusicSound_Action();
	void FxSound_Action();
private:
	MusicButton* FxSound;
	MusicButton* MusicSound;
	bool bIsKeyPressed;
	ButtonRenderData FxSoundRenderData;
	ButtonRenderData MusicSoundRenderData;
	RenderData yellowButton, blueButton;
};

#endif