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

#include "xbox360controller.h"

Xbox360Controller::Xbox360Controller()
{
	for(int i = 0; i < NUM_XBOX360_BUTTONS; i++)
	{
		fnc[i] = NULL;
		fnc_release[i] = NULL;
	}

	delayTimer.SetTimeLimit(50);
	currentCallback = NULL;

	lastGameKeyPressed = XBOX360_Button_None;
}

Xbox360Controller::~Xbox360Controller()
{

}

float Xbox360Controller::GetXRightAxis()
{
	float norm = 0;
#if defined DESKTOPPLATFORM
	norm = GamePad_GetStick2XAxis();
	if(norm < 0.4f && norm > -0.4f) return 0.0f;
	else return norm;
#endif
	return norm;
}

float Xbox360Controller::GetYRightAxis()
{
	float norm = 0;
#if defined DESKTOPPLATFORM
	norm = GamePad_GetStick2YAxis();
	if(norm < 0.4f && norm > -0.4f) return 0.0f;
	else return norm;
#endif
	return norm;
}

void Xbox360Controller::Update()
{
#if defined DESKTOPPLATFORM
	if(bIsXbox360ControllerEnabled)
	{
		currentButtonPressed = XBOX360_Button_None;
		//XBOX
		if(GamePad_GetButtonA()) currentButtonPressed = XBOX360_Button_A;
		else if(GamePad_GetButtonB()) currentButtonPressed = XBOX360_Button_B;
		else if(GamePad_GetButtonX()) currentButtonPressed = XBOX360_Button_X;
		else if(GamePad_GetButtonY()) currentButtonPressed = XBOX360_Button_Y;
		else if(GamePad_GetButtonDPadDown()) currentButtonPressed = XBOX360_Button_Down;
		else if(GamePad_GetButtonDPadUp()) currentButtonPressed = XBOX360_Button_Up;
		else if(GamePad_GetStick1YAxis() < -0.75f) currentButtonPressed = XBOX360_Button_Up;
		else if(GamePad_GetStick1YAxis() > 0.75f) currentButtonPressed = XBOX360_Button_Down;
		else if(GamePad_GetButtonDPadLeft()) currentButtonPressed = XBOX360_Button_Left;
		else if(GamePad_GetButtonDPadRight()) currentButtonPressed = XBOX360_Button_Right;
		else if(GamePad_GetStick1XAxis() < -0.75f) currentButtonPressed = XBOX360_Button_Left;
		else if(GamePad_GetStick1XAxis() > 0.75f) currentButtonPressed = XBOX360_Button_Right;
		else if(GamePad_GetButtonRShoulderDown()) currentButtonPressed = XBOX360_Button_RB;
		else if(GamePad_GetButtonLShoulderDown()) currentButtonPressed = XBOX360_Button_LB;
		else if(GamePad_GetButtonSelect()) currentButtonPressed = XBOX360_Button_Select;
		else if(GamePad_GetButtonStart()) currentButtonPressed = XBOX360_Button_Start;
		//PC KEYBOARD
		else if(currKeyPressed == s3eKeyW 
			    || currKeyPressed == s3eKeyUp
				|| currKeyPressed == s3eKeyAbsUp) currentButtonPressed = XBOX360_Button_Up;
		else if(currKeyPressed == s3eKeyS 
			    || currKeyPressed == s3eKeyDown 
				|| currKeyPressed == s3eKeyAbsDown) currentButtonPressed = XBOX360_Button_Down;
		else if(currKeyPressed == s3eKeyD 
				|| currKeyPressed == s3eKeyRight
				|| currKeyPressed == s3eKeyAbsRight) currentButtonPressed = XBOX360_Button_Right;
		else if(currKeyPressed == s3eKeyA 
				|| currKeyPressed == s3eKeyLeft 
				|| currKeyPressed == s3eKeyAbsLeft) currentButtonPressed = XBOX360_Button_Left;
		//ingame case
		if(gameState==GS_Game && inGamePausedState == GSGP_None)
		{
			if(currKeyPressed == s3eKeyEnter) currentButtonPressed = XBOX360_Button_A;
			else if(currKeyPressed == s3eKeyEsc) currentButtonPressed = XBOX360_Button_Start;
			else if(currKeyPressed == s3eKeyBackspace) currentButtonPressed = XBOX360_Button_B;
			else if(currKeyPressed == s3eKeyTab) currentButtonPressed = XBOX360_Button_Select;
			else if(currKeyPressed == s3eKeyPageUp || currKeyPressed == s3eKey2) currentButtonPressed = XBOX360_Button_RB;
			else if(currKeyPressed == s3eKeyPageDown || currKeyPressed == s3eKey1) currentButtonPressed = XBOX360_Button_LB;
		}
		else //any menu including ingame.
		{
			if(currKeyPressed == s3eKeyEnter) currentButtonPressed = XBOX360_Button_A;
			else if(currKeyPressed == s3eKeyEsc || currKeyPressed == s3eKeyBackspace) currentButtonPressed = XBOX360_Button_B;
		}
		

		if(currentButtonPressed != XBOX360_Button_None)
			lastGameKeyPressed = currentButtonPressed;

		if(currentButtonPressed != lastButtonPressed)// && currentCallback == NULL)
		{

			
			if(fnc[(int)currentButtonPressed] != NULL)
			{
				if(currKeyPressed != noKeyAssignment)
					activeControllerType = Controller_KEYBOARD;
				else
					activeControllerType = Controller_XBOX360;

				//IwTrace(Xbox360Debug, ("XBOX 2"));
				fnc[(int)currentButtonPressed]();
				//currentCallback = fnc[(int)currentButtonPressed];
				//delayTimer.Start();
			}

			if(fnc_release[(int)lastButtonPressed] != NULL)
			{
				fnc_release[(int)lastButtonPressed]();
			}

			lastButtonPressed = currentButtonPressed;
		}
		/*
		delayTimer.Update();
		if(delayTimer.GetIsEnd() && currentCallback != NULL)
		{
			currentCallback();
			currentCallback = NULL;
		}
		*/
		//clean keys except movement
		if(!((currKeyPressed == s3eKeyW || currKeyPressed == s3eKeyUp || currKeyPressed == s3eKeyAbsUp) || 
			(currKeyPressed == s3eKeyS || currKeyPressed == s3eKeyDown || currKeyPressed == s3eKeyAbsDown) ||
			(currKeyPressed == s3eKeyD || currKeyPressed == s3eKeyRight || currKeyPressed == s3eKeyAbsRight) ||
			(currKeyPressed == s3eKeyA || currKeyPressed == s3eKeyLeft || currKeyPressed == s3eKeyAbsLeft)))
		currKeyPressed = s3eKeyFirst;
	}
#endif
}

void Xbox360Controller::SetCallback(XBOX360_Button buttonEnum, void (*fnc)())
{
	this->fnc[(int)buttonEnum] = fnc;
}

void Xbox360Controller::SetCallbackOnRelease(XBOX360_Button buttonEnum, void (*fnc)())
{
	this->fnc_release[(int)buttonEnum] = fnc;
}