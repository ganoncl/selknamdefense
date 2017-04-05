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

#ifndef _TRIALVERSION_H
#define _TRIALVERSION_H

#include "IwGx.h"
#include "utils.h"
#include "timer.h"
#include "fadescreen.h"
#include "button.h"

class TrialVersion
{
public:
	TrialVersion();
	~TrialVersion();
	void InitObject();
	void EndObject();
	void Update();
	void Render();
	void UpdateClickEvents(s3ePointerEvent *event);
	void UpdateMotionEvents(s3ePointerMotionEvent* event);
	void UpdateKeyboardEvents(s3eKeyboardEvent* event);
	void ContinueAction();
private:
	CIwTexture* ThanksTexture;
	CIwTexture* FourTexture;
	CIwTexture* FramesTexture;
	FadeScreen* TrialVersionFade;
	Timer _Timer;
	bool bRenderFrames;
	Button* ContinueButton;
	int ImagesFadeAmount;
	RenderData FramesRenderData;
	RenderData ThanksRenderData;
	ButtonRenderData buttonRenderData;
	RenderData menuTitle;
	RenderData four;
};

#endif