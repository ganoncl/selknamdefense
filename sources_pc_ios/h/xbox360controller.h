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

#ifndef _XBOX360CONTROLLER_H
#define _XBOX360CONTROLLER_H

/*
 * Name: Xbox 360 Controller handler.
 * Description: Using "Marmalade 360 Xbox Controller framework" this class allows to link each xbox 360 gamepad's buttons to a specific custom function
 *              that will trigger when that button is pressed or when a change in movement happens (stick and the buttons at the back of the controller).
 * How to use: For each window's game you have to create a instance of this class to handle input. A window's game is for example, the menu or the gameplay.
 *             Update() method must be called in each frame.
 */

#if defined DESKTOPPLATFORM
#include "h/GamePad.h"
#endif
#include "common.h"
#include "timer.h"

class Xbox360Controller
{
public:
	Xbox360Controller();
	~Xbox360Controller();
	void Update();
	void SetCallback(XBOX360_Button buttonEnum, void (*fnc)());
	void SetCallbackOnRelease(XBOX360_Button buttonEnum, void (*fnc)());
	float GetXRightAxis();
	float GetYRightAxis();

	XBOX360_Button GetLastGameKeyPressed() { return lastGameKeyPressed; };

private:
	void (*fnc[NUM_XBOX360_BUTTONS])();
	void (*fnc_release[NUM_XBOX360_BUTTONS])();
	Timer delayTimer;
	void (*currentCallback)();

	XBOX360_Button lastGameKeyPressed;
};

#endif