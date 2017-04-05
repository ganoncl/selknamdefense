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

#include "tutorial.h"
#include "common.h"
#include "utils.h"
#include "texturehelper.h"

Tutorial* TutorialReference = NULL;

//Keyboard handler
int32 KeyboardHandlerTutorial(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	//if(!TutorialReference->bKeyboardBlocked)
		TutorialReference->UpdateKeyboardEvents(event);

	return 0;
}

//Mouse Motion
void HandleMotionTutorial(s3ePointerMotionEvent* event) 
{
	if(TutorialReference == NULL)
		return;

	//if(!TutorialReference->bMouseBlocked)
		TutorialReference->UpdateMotionEvents(event);
}


//Mouse handle
void HandleClickTutorial(s3ePointerEvent *event) 
{
	if(TutorialReference == NULL)
		return;

	if(!Utils::CheckIfMouseIsEnabled())
	{
		if(ENABLE_MOUSE_HACK && event->m_Button == S3E_POINTER_BUTTON_LEFTMOUSE)
		{
			if(!event->m_Pressed)
			{
				TutorialReference->MouseHack();
			}
			else
			{
				TutorialReference->ActiveButtonMouseHack();
			}
		}

		return;
	}

	//if(!TutorialReference->bMouseBlocked)
		TutorialReference->UpdateClickEvents(event);
}

void EndTutorialAction()
{
	if(TutorialReference != NULL)
	{
		TutorialReference->EndTutorial();
		TutorialReference->SetSkipped(true);
	}
}

void Finish()
{
	if(TutorialReference != NULL)
	{
		TutorialReference->FinishTutorial();
	}
}

void NextFrame()
{
	if(TutorialReference != NULL)
	{
		TutorialReference->NextTutorialFrame();
	}
}

Tutorial::Tutorial()
{
	TutorialReference = this;

	CurTutorial = Tutorial_None;
	CurTutorialStep = -1;

	TextureData greenButtonData = TextureHelper::GetTextureNamed("btn_tutorial_green");
	greenButton = new ButtonRenderData();
	greenButton->numStates = 1;
	greenButton->state[0].RollOut  = TextureHelper::GetRenderData(greenButtonData, 1, 4, 3);
	greenButton->state[0].RollOn   = TextureHelper::GetRenderData(greenButtonData, 1, 4, 1);
	greenButton->state[0].Pressed  = TextureHelper::GetRenderData(greenButtonData, 1, 4, 3);
	greenButton->state[0].Inactive = TextureHelper::GetRenderData(greenButtonData, 1, 4, 3);

	TextureData redButtonData = TextureHelper::GetTextureNamed("btn_tutorial_red");
	redButton = new ButtonRenderData();
	redButton->numStates = 1;
	redButton->state[0].RollOut  = TextureHelper::GetRenderData(redButtonData, 1, 4, 3);
	redButton->state[0].RollOn   = TextureHelper::GetRenderData(redButtonData, 1, 4, 1);
	redButton->state[0].Pressed  = TextureHelper::GetRenderData(redButtonData, 1, 4, 3);
	redButton->state[0].Inactive = TextureHelper::GetRenderData(redButtonData, 1, 4, 3);

	RenderData tmpData = TextureHelper::GetRenderData(TextureHelper::GetTextureNamed("label_arrowtutorial"));
	arrow = new RenderData();
	arrow->bRotate = tmpData.bRotate;
	arrow->height = tmpData.height;
	arrow->uvInfo = tmpData.uvInfo;
	arrow->width = tmpData.width;
	arrow->atlasName = tmpData.atlasName;

	SkipButton = new Button(TextureHelper::GetCIwTexture(redButton->state[0].RollOut.atlasName), redButton, 900, 540);
	SkipButton->SetText("SKIP");
	SkipButton->SetStyle(font_komikadisplay18, 0, 0);
	SkipButton->OnClick(&EndTutorialAction);
	OkButton   = new Button(TextureHelper::GetCIwTexture(greenButton->state[0].RollOut.atlasName), greenButton, 1081, 540);
	OkButton->SetText("OK");
	OkButton->SetStyle(font_komikadisplay18, 0, 0);
	OkButton->OnClick(&NextFrame);
	FinishButton =new Button(TextureHelper::GetCIwTexture(redButton->state[0].RollOut.atlasName), redButton, 990, 540);
	FinishButton->SetText("FINISH");
	FinishButton->SetStyle(font_komikadisplay18, 0, 0);
	FinishButton->OnClick(&Finish);

	bRenderSkipButton = false;
	bRenderOkButton = false;
	bRenderFinishButton = false;

	bMouseBlocked = false;
	bKeyboardBlocked = false;

	bSkipped = false;
	bFirstOpen = true;
	openTimer.SetTimeLimit(2000);
}

Tutorial::~Tutorial()
{
	delete SkipButton;
	delete OkButton;
	delete FinishButton;
	delete redButton;
	delete arrow;
	delete greenButton;
}

void Tutorial::Update()
{
	openTimer.Update();

	if(CurTutorial == Tutorial_Map1 || CurTutorial == Tutorial_UnitMenu)
	{	
		bPauseGame = true;
	}
}

void Tutorial::Render()
{
	if(CurTutorial == Tutorial_Map1)
	{
		if(CurTutorialStep == 0)
		{
			DrawMessage("WHENEVER AN ENEMY REACHES YOUR BASE YOU LOSE A LIFE.");
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(arrow->atlasName),  CIwSVec2(345, 125), arrow);
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(arrow->atlasName),  CIwSVec2(900, 25), arrow, 1);
			Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(arrow->atlasName),  CIwSVec2(1000, 275), arrow);
			IwGxLightingOn();
			Utils::RenderText("ENEMY SPAWN", CIwRect(280, 90, 500, 200), font_komikadisplay22, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP, true, 2);
			Utils::RenderText("YOUR LIVES", CIwRect(720, 45, 500, 200), font_komikadisplay22, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP, true, 2);
			Utils::RenderText("YOUR BASE", CIwRect(950, 240, 500, 200), font_komikadisplay22, 0xff00edf8, IW_GX_FONT_ALIGN_LEFT, IW_GX_FONT_ALIGN_TOP, true, 2);
			IwGxLightingOff();
			bRenderSkipButton = true;
			bRenderOkButton = true;
			bRenderFinishButton = false;
		}
		else if(CurTutorialStep == 1)
		{
			DrawMessage("TO DEFEND YOUR BASE YOU HAVE TO DEPLOY SELKNAMS\nTO ATTACK THE ENEMIES.");
			bRenderSkipButton = true;
			bRenderOkButton = true;
			bRenderFinishButton = false;
		}
		else if(CurTutorialStep == 2)
		{
			DrawMessage("YOU WILL EARN COINS FOR EACH DESTROYED ENEMY.");
			bRenderSkipButton = true;
			bRenderOkButton = true;
			bRenderFinishButton = false;
		}
		else if(CurTutorialStep == 3)
		{
			DrawMessage("A SELKNAMS ATTACKS ENEMIES AUTOMATICALLY\nIF AN ENEMY IS WITHIN SELKNAM RANGE.");
			bRenderSkipButton = true;
			bRenderOkButton = true;
			bRenderFinishButton = false;
		}
		else if(CurTutorialStep == 4)
		{
			DrawMessage("ENEMIES WILL ATTACK IN WAVES IN A CERTAIN PERIOD OF TIME.");
			bRenderSkipButton = true;
			bRenderOkButton = true;
			bRenderFinishButton = false;
		}
		else if(CurTutorialStep == 5)
		{
			if(!bMouseActive)
			{
				DrawMessage("USE ARROWS TO MOVE CURSOR ON CELLS,\nTHEN PRESS 'OK' TO OPEN UNIT MENU.");
			}
			else
			{
				DrawMessage("USE CURSOR TO MOVE ON GRID, THEN CLICK ON\nAVAILABLE CELL TO OPEN UNIT MENU.");
			}
			bRenderSkipButton = true;
			bRenderOkButton = true;
			bRenderFinishButton = false;
		}
		else if(CurTutorialStep == 6)
		{
			EndTutorial();
			bStopWaveTimer = true;
		}
	}
	else if(CurTutorial == Tutorial_UnitMenu)
	{
		if(CurTutorialStep == 0)
		{
			if(!bMouseActive)
			{
				DrawMessage("USE ARROWS TO SELECT DEFENDERS, THEN USE THE\nARROWS AGAIN TO DEPLOY THE SELECTED UNIT.");
			}
			else
			{
				DrawMessage("USE CURSOR TO SELECT DEFENDERS, THEN USE THE\nCURSOR TO DEPLOY THE SELECTED UNIT.");
			}
			bRenderSkipButton = false;
			bRenderOkButton = false;
			bRenderFinishButton = true;
		}
		else if(CurTutorialStep == 1)
		{
			EndTutorial();
			bStopWaveTimer = false;
			bSkipped = true;
			bUpdateClickEvents = true;
		}
	}
	else if(CurTutorial == Tutorial_Stars)
	{
		if(CurTutorialStep == 0)
		{
			DrawMessage("YOU GET STARS FROM WINNING BATTLES!\nGO TO UPGRADES TO IMPROVE YOUR UNITS.");
			bRenderFinishButton = false;
			bRenderOkButton = true;
			bRenderSkipButton = false;
		}
		else if(CurTutorialStep == 1)
		{
			EndTutorial();
		}
	}

	if(bRenderSkipButton)
		SkipButton->Render();
	
	if(bRenderOkButton)
		OkButton->Render();

	if(bRenderFinishButton)
		FinishButton->Render();
}

void Tutorial::InitTutorial(TutorialName _TutorialName)
{
	if(bSkipped)
		return;

	CurTutorial = _TutorialName;
	CurTutorialStep = 0;
	InitDeviceHandler();
	focus = TutorialFocus_None;
	SetFocus(TutorialFocus_Ok);

	if(CurTutorial == Tutorial_UnitMenu)
	{
		SetFocus(TutorialFocus_Finish);
		openTimer.Reset();
		openTimer.Start();
	}
	else if(CurTutorial == Tutorial_Stars)
	{
		SetFocus(TutorialFocus_Ok);
		bPauseMapSelection = true;
	}
}

void Tutorial::DrawMessage(string message)
{
	Utils::RenderColor(0xa8000000, CIwSVec2(0,420), CIwSVec2(1280, 180));
	IwGxFlush();
	Utils::RenderColor(0xff000000, CIwSVec2(0,420), CIwSVec2(1280, 4));
	IwGxFlush();
	Utils::RenderColor(0xff000000, CIwSVec2(0,596), CIwSVec2(1280, 4));
	IwGxFlush();
	Utils::RenderColor(0xff000000, CIwSVec2(0,420), CIwSVec2(4, 160));
	IwGxFlush();
	Utils::RenderColor(0xff000000, CIwSVec2(1276,420), CIwSVec2(4, 160));
	IwGxFlush();
	IwGxLightingOn();
	Utils::RenderText(message.c_str(), CIwRect(0, 420, 1280, 180), font_komikadisplay22);
	IwGxLightingOff();
	IwGxFlush();
}

void Tutorial::UpdateClickEvents(s3ePointerEvent *event)
{
	/*
	if(bMouseBlocked)
		return;
	*/
	if(event->m_Pressed)
		bKeyboardBlocked = true;
	else
		bKeyboardBlocked = false;

	if(bRenderSkipButton)
		SkipButton->HandleClick(event);

	if(bRenderOkButton)
		OkButton->HandleClick(event);

	if(bRenderFinishButton)
		FinishButton->HandleClick(event);
}

void Tutorial::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	/*
	if(bMouseBlocked)
		return;
	*/
	if(bRenderSkipButton)
	{
		SkipButton->HandleMotion(event);
		if(SkipButton->MouseOn(event->m_x, event->m_y))
		{
			SetFocus(TutorialFocus_Skip);
		}
	}

	if(bRenderOkButton)
	{
		OkButton->HandleMotion(event);
		if(OkButton->MouseOn(event->m_x, event->m_y))
		{
			SetFocus(TutorialFocus_Ok);
		}
	}

	if(bRenderFinishButton)
	{
		FinishButton->HandleMotion(event);
		if(FinishButton->MouseOn(event->m_x, event->m_y))
		{
			SetFocus(TutorialFocus_Finish);
		}
	}
}

void Tutorial::UpdateKeyboardEvents(s3eKeyboardEvent* event)
{
	if(CurTutorial == Tutorial_UnitMenu)
	{
		if(event->m_Pressed)
		{
			if(bFirstOpen)
			{
				return;
			}
		}
	}


	/*
	if(bKeyboardBlocked)
		return;
	*/
	if(event->m_Pressed)
	{
		bMouseBlocked = true;
		if(event->m_Key == s3eKeyAbsRight)
		{
			if(CurTutorial == Tutorial_Map1)			
				SetFocus(TutorialFocus_Ok);
		}
		else if(event->m_Key == s3eKeyAbsLeft)
		{
			if(CurTutorial == Tutorial_Map1)
				SetFocus(TutorialFocus_Skip);
		}
	}
	else
	{
		bMouseBlocked = false;
		if(event->m_Key == s3eKeyEnter || event->m_Key == s3eKeyAbsOk)
		{
			if(focus == TutorialFocus_Skip)
			{
				SkipButton->Execute();
			}
			else if(focus == TutorialFocus_Ok)
			{
				OkButton->Execute();
				OkButton->SetStatus(1);
			}
			else if(focus == TutorialFocus_Finish)
			{
				//if(!bFirstOpen)
				if(openTimer.GetIsEnd())
				{
					if(!bFirstOpen)
					{
						FinishButton->Execute();
					}
				}
			}
		}
		bFirstOpen = false;
	}
}

void Tutorial::InitDeviceHandler()
{
	s3ePointerRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTutorial, NULL);
	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerTutorial, NULL);
	s3ePointerRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTutorial, NULL);
}

void Tutorial::StopDeviceHandler()
{
	s3ePointerUnRegister(S3E_POINTER_MOTION_EVENT, (s3eCallback)HandleMotionTutorial);
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandlerTutorial);
	s3ePointerUnRegister(S3E_POINTER_BUTTON_EVENT, (s3eCallback)HandleClickTutorial);
}

void Tutorial::EndTutorial()
{
	CurTutorial = Tutorial_None;
	CurTutorialStep = -1;
	bPauseGame = false;
	bPauseMapSelection = false;
	bRenderSkipButton = false;
	bRenderOkButton = false;
	bRenderFinishButton = false;
	bFirstOpen = true;
	StopDeviceHandler();
}

void Tutorial::SetFocus(TutorialFocus newFocus)
{
	if(newFocus == focus)
	{
		if(focus != TutorialFocus_Skip)
			SkipButton->IsFocused(false);
		if(focus != TutorialFocus_Ok)
			OkButton->IsFocused(false);
		if(focus != TutorialFocus_Finish)
			FinishButton->IsFocused(false);
		return;
	}
	SkipButton->IsFocused(false);
	OkButton->IsFocused(false);
	FinishButton->IsFocused(false);
	/*
	if(focus == TutorialFocus_Skip)
		SkipButton->IsFocused(false);
	else if(focus == TutorialFocus_Ok)
		OkButton->IsFocused(false);
	else if(focus == TutorialFocus_Finish)
		FinishButton->IsFocused(false);
	*/
	if(newFocus == TutorialFocus_Skip)
		SkipButton->IsFocused(true);
	else if(newFocus == TutorialFocus_Ok)
		OkButton->IsFocused(true);
	else if(newFocus == TutorialFocus_Finish)
		FinishButton->IsFocused(true);

	focus = newFocus;
}

void Tutorial::NextTutorialFrame()
{
	CurTutorialStep++;
}

void Tutorial::FinishTutorial()
{
	if(CurTutorial == Tutorial_UnitMenu)
	{
		CurTutorialStep = 1;
	}
}

void Tutorial::MouseHack()
{
	if(focus == TutorialFocus_Skip)
	{
		SkipButton->Execute();
	}
	else if(focus == TutorialFocus_Ok)
	{
		OkButton->Execute();
		OkButton->SetStatus(1);
	}
	else if(focus == TutorialFocus_Finish)
	{
		//if(!bFirstOpen)
		if(openTimer.GetIsEnd())
		{
			if(!bFirstOpen)
			{
				FinishButton->Execute();
			}
		}
	}
}

void Tutorial::ActiveButtonMouseHack()
{

}