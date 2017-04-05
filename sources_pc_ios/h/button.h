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

#ifndef _BUTTON_H
#define _BUTTON_H
#include "IwGx.h"
#include "s3e.h"
#include "s3ePointer.h"
#include "IwGxFont.h"
#include "focuseableobject.h"

typedef enum
{
	BTN_NONE,
	BTN_ROLLON,
	BTN_ROLLOUT,
	BTN_PRESSED,
	BTN_INACTIVE
} ButtonState;

typedef void (*function)();
typedef void (*functionWithData)(void* data);

class ButtonRenderData;

class Button : public FocuseableObject 
{
public:
	Button(CIwTexture *tex, int x, int y, int w = 0); //If w is defined, then you can call setImage
	Button(CIwTexture *tex, ButtonRenderData* data, int x, int y); 
	~Button();
	void SetPos(int x, int y);
	CIwSVec2 GetPos();
	void Render(int xOff = 0, int yOff = 0);
	void Enabled(bool bIsEnabled);
	bool IsEnabled() { return btnState != BTN_INACTIVE; };
	void HandleMotion(s3ePointerMotionEvent* event);
	void HandleClick(s3ePointerEvent *event);
	void Execute(bool bChangeStatus = true); //Execute OnClickFunc
	void SetText(const char* title); 
	void OnClick(function func);
	void OnClick(functionWithData func, void* data);
	void OnRollOn(function func);
	void IsFocused(bool bIsFocused, bool playSound = true);
	bool IsActive();
	void SetStatus(int status); //Set btnState manually.
	void SetStyle(CIwGxFont* pfont, int marginX = 10, int marginY = 10);
	void SetFont(CIwGxFont* pfont);
	void SetTextOffsetValueByState(int _btnTextOffsetByState){btnTextOffsetByState = _btnTextOffsetByState;}
	void SetImage(int id); //you can use a png file with distincts images for a same button, each column is a id [0, nº cols].
	bool MouseOn(int x, int y);
	void FunctionOnClickEnabled(bool bEnabled);
	bool bIsOnClickEnabled;

	int GetWidth();
	int GetHeight();
	int GetX();
	int GetY();
	CIwVec2 GetCenter(); //Get center point of the image.


	ButtonState GetState() { return btnState; };
	int GetImageID() { return _id; };

	bool bDebugData;

	virtual void EnableFocus();
	virtual void UnableFocus();
	virtual void ExecuteFunction();
	virtual bool HandleFocusMotion(s3ePointerMotionEvent* event);
	virtual void ExecuteOnFocused();
	bool GetIsFocused() { return bIsFocused; };
	void SetOnFocusedCallback(void (*fnc)()) { onFocused = fnc; };

	void SetAlphaValue(const uint8 _buttonAlphaValue){ buttonAlphaValue = _buttonAlphaValue; }
	uint8 GetAlphaValue() const	{ return buttonAlphaValue; }

	void SetButtonOnAnim(const bool _bIsButtonAnim){ bIsButtonAnim = _bIsButtonAnim; }
	bool GetButtonOnAnim() const	{ return bIsButtonAnim; }

	function GetOnClickFunc() { return OnClickFunc; };

private:	
	void (*onFocused)();

	void SetUVCoords();
	function OnClickFunc;
	function OnRollOnFunc;
	functionWithData OnClickFuncWithData;
	void* OnClickData;

	CIwSVec2 xy3[4];
	CIwFVec2	s_UV[4];
	CIwTexture* tex;
	CIwResGroup* pGroup;
	CIwGxFont* pfont;

	ButtonState btnState;
	
	int _width;
	int _height;
	int xPos;
	int yPos;
	//Text position inside button
	int marginX;
	int marginY;
	const char* title;

	bool bIsFocused;
	bool bIsActive;

	s3ePointerButton lastBtnPressed;

	float uvMinX;
	float uvMaxX;
	float _uvMinX;
	float _uvMaxX;

	int _id;

	bool bPlayRollOverSound;

	ButtonRenderData* buttonRenderData;
	bool bRotatedTexture;

	int btnTextOffsetByState;

	uint8 buttonAlphaValue;

	bool bIsButtonAnim;
};

class ButtonWrapper
{
public:
	ButtonWrapper()
	{
		button = NULL;
		bRender = true;
	}

	~ButtonWrapper()
	{
		if(button != NULL)	delete button;
	}

	void HandleMotion(s3ePointerMotionEvent* event)
	{
		if(bRender && button != NULL) button->HandleMotion(event);
	}

	void HandleClick(s3ePointerEvent *event)
	{
		if(bRender && button != NULL) button->HandleClick(event);
	}

	void Render()
	{
		if(bRender && button != NULL) button->Render();
	}

	bool bRender;
	Button* button;
};

#endif