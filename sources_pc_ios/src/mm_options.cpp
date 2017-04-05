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

#include "mm_options.h"
#include "utils.h"
#include "common.h"
#include "localization.h"
#if !defined DESKTOPPLATFORM
#include "IwBilling.h"
#endif

OptionsMenu* optionsMenuRef = NULL;

void OptionsMenu_UpdateClickEvents(s3ePointerEvent* event)
{
	if(optionsMenuRef == NULL)
		return;

	optionsMenuRef->UpdateClickEvents(event);
}

void OptionsMenu_UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(optionsMenuRef == NULL)
		return;

	optionsMenuRef->UpdateMotionEvents(event);
}

void OptionsMenu_Settings()
{
	mainMenuState = GSM_Settings;
}

void OptionsMenu_Credits()
{
	mainMenuState = GSM_Credits;
}

void OptionsMenu_Restore()
{
#if !defined DESKTOPPLATFORM
	IwBilling::RestoreTransactions();
#endif
}

void OptionsMenu_Back()
{
	mainMenuState = GSM_MainMenu;
}

#if defined DESKTOPPLATFORM
void OptionsMenu_Up()
{
	optionsMenuRef->focusManager.ChangeFocus(Xbox360_Up);
}

void OptionsMenu_Down()
{
	optionsMenuRef->focusManager.ChangeFocus(Xbox360_Down);
}

void OptionsMenu_Action()
{
	optionsMenuRef->focusManager.ExecuteFocusedObject();
}
#endif

OptionsMenu::OptionsMenu()
{
	optionsMenuRef = this;

	SetIsActive(false);

	Utils::GetMenuTitle(&menuTitle, &titlePosition);
	Utils::GetLongMenuButton(&buttonRenderData);

	CIwSVec2 buttonDimension = TextureHelper::GetDimension(buttonRenderData.state[0].RollOn);
	int texWidth = buttonDimension.x;
	int texHeight = buttonDimension.y;
	int btnXPos =  (int)((currentGameWidth - texWidth) * 0.5f);
	int btnYPos = 0;
	int btnDistance = 0;

#if defined DESKTOPPLATFORM
	btnYPos = Utils::GetScaledInt(230);
	if (currentGameHeight <= 600)
		btnYPos = Utils::GetScaledInt(240);
	btnDistance = Utils::GetScaledInt(85);
#endif
#if !defined DESKTOPPLATFORM
	btnYPos = Utils::GetScaledInt(245);
	if (currentGameHeight <= 600)
		btnYPos = Utils::GetScaledInt(265);
	btnDistance = Utils::GetScaledInt(98);
#endif

	//Settings
	buttonList[0] = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 0);
	buttonList[0]->OnClick(&OptionsMenu_Settings);
	buttonList[0]->SetText(loc_ui_settings);

	//Credits
	buttonList[1] = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 1);
	buttonList[1]->OnClick(OptionsMenu_Credits);
	buttonList[1]->SetText(loc_ui_credits);

#if defined DESKTOPPLATFORM
	//Back
	buttonList[2] = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 3);
	buttonList[2]->OnClick(OptionsMenu_Back);
	buttonList[2]->SetText(loc_ui_back);
#endif
#if !defined DESKTOPPLATFORM
	//Restore
	buttonList[2] = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 2);
	buttonList[2]->OnClick(OptionsMenu_Restore);
	buttonList[2]->SetText(loc_ui_restore);
	buttonList[2]->SetFont(font_komikadisplaybold18);

	//Back
	buttonList[3] = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnXPos, btnYPos + (btnDistance) * 3);
	buttonList[3]->OnClick(OptionsMenu_Back);
	buttonList[3]->SetText(loc_ui_back);
#endif
	

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, OptionsMenu_Action);
	xbox360Controller.SetCallback(Xbox360_Action2, OptionsMenu_Action);
	xbox360Controller.SetCallback(Xbox360_Action3, OptionsMenu_Action);
	xbox360Controller.SetCallback(Xbox360_Cancel1, OptionsMenu_Back);
	xbox360Controller.SetCallback(Xbox360_Cancel2, OptionsMenu_Back);
	xbox360Controller.SetCallback(Xbox360_Up, OptionsMenu_Up);
	xbox360Controller.SetCallback(Xbox360_Down, OptionsMenu_Down);

	for(int i = 0; i < OPTIONS_NUM_BUTTONS - 1; i++)
		focusManager.SetFocusDirection(Xbox360_Down, buttonList[i], buttonList[i + 1]);

	for(int i = 1; i < OPTIONS_NUM_BUTTONS; i++)
		focusManager.SetFocusDirection(Xbox360_Up, buttonList[i], buttonList[i - 1]);

	focusManager.SetInitialFocus(buttonList[0]);
#endif
}

OptionsMenu::~OptionsMenu()
{
	for(int i = 0; i < OPTIONS_NUM_BUTTONS; i++)
		delete buttonList[i];
}

void OptionsMenu::InitObject()
{
	SetIsActive(true);

	Utils::RegisterMouseCallbacks(OptionsMenu_UpdateClickEvents, OptionsMenu_UpdateMotionEvents);

#if defined DESKTOPPLATFORM
	if(focusManager.GetDefaultFocus() == buttonList[OPTIONS_NUM_BUTTONS - 1])
		focusManager.SetInitialFocus(buttonList[0]);
	else
		focusManager.SetInitialFocus(focusManager.GetDefaultFocus());
#endif
}

void OptionsMenu::EndObject()
{
	SetIsActive(false);

	Utils::UnRegisterMouseCallbacks(OptionsMenu_UpdateClickEvents, OptionsMenu_UpdateMotionEvents);
}

void OptionsMenu::Render()
{
	Utils::RenderSingleTexture(atlas_bkg_menu_others, titlePosition, menuTitle);

	for(int i = 0; i < OPTIONS_NUM_BUTTONS; i++)
		buttonList[i]->Render();
}

void OptionsMenu::Update()
{
	if(!GetIsActive())
		return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif
}

void OptionsMenu::UpdateClickEvents(s3ePointerEvent *event)
{
	if(!GetIsActive())
		return;

	for(int i = 0; i < OPTIONS_NUM_BUTTONS; i++)
		buttonList[i]->HandleClick(event);
}

void OptionsMenu::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!GetIsActive())
		return;

	for(int i = 0; i < OPTIONS_NUM_BUTTONS; i++)
		buttonList[i]->HandleMotion(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}