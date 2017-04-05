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

#include "mm_settings.h"
#include "utils.h"
#include "localization.h"
#include "savedatahelper.h"

SettingsMenu* settingsMenuRef = NULL;

void SettingsMenu_UpdateClickEvents(s3ePointerEvent* event)
{
	if(settingsMenuRef == NULL)
		return;

	settingsMenuRef->UpdateClickEvents(event);
}

void SettingsMenu_UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(settingsMenuRef == NULL)
		return;

	settingsMenuRef->UpdateMotionEvents(event);
}

void SettingsMenu_MusicSlider_OnChangeValue(float f)
{
	Utils::SetMusicVolume((int)(Utils::GetMusicMaxValue() * f));
}

void SettingsMenu_FXSoundSlider_OnChangeValue(float f)
{
	Utils::SetFXSoundVolume((int)(Utils::GetFXSoundMaxValue() * f));
}

int myLastAudioVolume;
int myLastSoundVolume;
bool bIsAllMuted;

void SettingsMenu_Accept()
{
	if(mainMenuIGState == GSMIG_Options)
		mainMenuIGState = GSMIG_MainMenuIG;
	else
		mainMenuState = GSM_Options;

	currentAudioVolume = s3eAudioGetInt(S3E_AUDIO_VOLUME);
	currentSoundVolume = s3eSoundGetInt(S3E_SOUND_VOLUME);

#if !defined DESKTOPPLATFORM
	if(bIsAllMuted)
	{
		bAllMuted = true;
		lastAudioVolume = myLastAudioVolume;
		lastSoundVolume = myLastSoundVolume;
	}
	else
	{
		bAllMuted = false;
	}
#endif

#if defined DESKTOPPLATFORM
	settingsMenuRef->ToggleFullscreen();
	
	if(strcmp(currentDeviceID, "Windows Simulator") != 0)
	{
		bIsFullScreenOption = Utils::GetIsFullscreen();
	}
#endif
	if(saveDataHelper)
		saveDataHelper->SaveGameLocalData();
}

#if defined DESKTOPPLATFORM
void SettingsMenu_Action()
{
	settingsMenuRef->focusManager.ExecuteFocusedObject();
}

void SettingsMenu_Up()
{
	settingsMenuRef->focusManager.ChangeFocus(Xbox360_Up);
}

void SettingsMenu_Down()
{
	settingsMenuRef->focusManager.ChangeFocus(Xbox360_Down);
}

void SettingsMenu_Left()
{
	settingsMenuRef->SetVolumeContDecrease(true);
}

void SettingsMenu_Right()
{
	settingsMenuRef->SetVolumeContIncrease(true);
}

void SettingsMenu_OnRelease_Right()
{
	settingsMenuRef->SetVolumeContIncrease(false);
}

void SettingsMenu_OnRelease_Left()
{
	settingsMenuRef->SetVolumeContDecrease(false);
}
#endif

SettingsMenu::SettingsMenu()
{
	bDecreaseVolume = false;
	bIncreaseVolume = false;

	settingsMenuRef = this;

	SetIsActive(false);

	background = new Background(0, 0, 0, fade_menu);

	RenderPanelNumWidth = 15;
	RenderPanelNumHeight = 7;
	widthSettingsPannel = (RenderPanelNumWidth + 2) * 40;
	heightSettingsPannel = (RenderPanelNumHeight + 2) * 40;
	PannelPosX = (int)(currentGameWidth * 0.5f) - (int)(widthSettingsPannel * 0.5f);
	PannelPosY = (int)(currentGameHeight * 0.5f) - (int)(heightSettingsPannel * 0.5f) - Utils::GetScaledInt(30);
	settingsTitleYPos = PannelPosY + 35;

	Utils::GetLongMenuButton(&buttonRenderData);
	CIwSVec2 buttonDimension = TextureHelper::GetDimension(buttonRenderData.state[0].Pressed);
	int btnTexWidth = buttonDimension.x;
	int btnTexHeight = buttonDimension.y;
	btnBackPosX = (int)((currentGameWidth - btnTexWidth) * 0.5f);
	btnBackPosY = currentGameHeight - Utils::GetScaledInt(110);

	btnAccept = new Button(TextureHelper::GetCIwTexture(buttonRenderData.state[0].RollOut.atlasName), &buttonRenderData, btnBackPosX, btnBackPosY);
	btnAccept->OnClick(SettingsMenu_Accept);
	btnAccept->SetText(loc_ui_accept);

	midWidth = (int)(widthSettingsPannel * 0.5f); 
	yMargin = 60;
	yPadding = 90;
	xPadding = -20;

	musicSlider.SetPosition(PannelPosX + xPadding + midWidth + 30, settingsTitleYPos + yPadding + 5);
	musicSlider.SetOnChangeValueCallback(SettingsMenu_MusicSlider_OnChangeValue);

	fxSoundSlider.SetPosition(PannelPosX + xPadding + midWidth + 30, settingsTitleYPos + yPadding + 5 + yMargin);
	fxSoundSlider.SetOnChangeValueCallback(SettingsMenu_FXSoundSlider_OnChangeValue);

	fullScreenCheckbox.SetPosition(PannelPosX + xPadding + midWidth + 30, settingsTitleYPos + yPadding - 8 + yMargin * 2);
	muteAllCheckbox.SetPosition(PannelPosX + xPadding + midWidth + 30, settingsTitleYPos + yPadding - 8 + yMargin * 2);

#if defined DESKTOPPLATFORM
	xbox360Controller.SetCallback(Xbox360_Action1, SettingsMenu_Action);
	xbox360Controller.SetCallback(Xbox360_Action2, SettingsMenu_Action);
	xbox360Controller.SetCallback(Xbox360_Action3, SettingsMenu_Action);
	xbox360Controller.SetCallback(Xbox360_Cancel1, SettingsMenu_Accept);
	xbox360Controller.SetCallback(Xbox360_Cancel2, SettingsMenu_Accept);
	xbox360Controller.SetCallback(Xbox360_Up, SettingsMenu_Up);
	xbox360Controller.SetCallback(Xbox360_Down, SettingsMenu_Down);
	xbox360Controller.SetCallback(Xbox360_Left, SettingsMenu_Left);
	xbox360Controller.SetCallback(Xbox360_Right, SettingsMenu_Right);
	xbox360Controller.SetCallbackOnRelease(Xbox360_Right, SettingsMenu_OnRelease_Right);
	xbox360Controller.SetCallbackOnRelease(Xbox360_Left, SettingsMenu_OnRelease_Left);

	focusManager.SetFocusDirection(Xbox360_Up, btnAccept, &fullScreenCheckbox);
	focusManager.SetFocusDirection(Xbox360_Down, &fullScreenCheckbox, btnAccept);
	focusManager.SetFocusDirection(Xbox360_Up, &fxSoundSlider, &musicSlider);
	focusManager.SetFocusDirection(Xbox360_Down, &musicSlider, &fxSoundSlider);
	focusManager.SetFocusDirection(Xbox360_Down, &fxSoundSlider, &fullScreenCheckbox);
	focusManager.SetFocusDirection(Xbox360_Up, &fullScreenCheckbox, &fxSoundSlider);
#endif

	myLastAudioVolume = 160;
	myLastSoundVolume = 190;
	bIsAllMuted = false;
}

SettingsMenu::~SettingsMenu()
{
	delete background;
	delete btnAccept;
}

void SettingsMenu::SetVolumeContIncrease(bool bValue)
{
	if(!bValue)
	{
		bIncreaseVolume = bDecreaseVolume = false;
		return;
	}

	bIncreaseVolume = true;
	bDecreaseVolume = false;
	changeVolumeTimer.SetTimeLimit(TIME_FIRST_ITERATION_VOL);
	bChangeVolumeFirstTime = true;
}

void SettingsMenu::SetVolumeContDecrease(bool bValue)
{
	if(!bValue)
	{
		bIncreaseVolume = bDecreaseVolume = false;
		return;
	}

	bIncreaseVolume = false;
	bDecreaseVolume = true;
	changeVolumeTimer.SetTimeLimit(TIME_FIRST_ITERATION_VOL);
	bChangeVolumeFirstTime = true;
}

void SettingsMenu::InitObject()
{
	bIncreaseVolume = false;
	bDecreaseVolume = false;

	SetIsActive(true);
	Utils::RegisterMouseCallbacks(SettingsMenu_UpdateClickEvents, SettingsMenu_UpdateMotionEvents);
	musicSlider.SetSliderProgress(Utils::GetMusicVolume());
	fxSoundSlider.SetSliderProgress(Utils::GetFXSoundVolume());
	fullScreenCheckbox.SetIsChecked(Utils::GetIsFullscreen());

#if defined DESKTOPPLATFORM
	focusManager.SetInitialFocus(btnAccept);
#endif
#if !defined DESKTOPPLATFORM
	muteAllCheckbox.SetIsChecked(bAllMuted);
	myLastAudioVolume = lastAudioVolume;
	myLastSoundVolume = lastSoundVolume;
	bIsAllMuted = bAllMuted;
#endif
}

void SettingsMenu::EndObject()
{
	SetIsActive(false);
	Utils::UnRegisterMouseCallbacks(SettingsMenu_UpdateClickEvents, SettingsMenu_UpdateMotionEvents);
}

void SettingsMenu::Render()
{
	background->Render();

	Utils::RenderPannelA(RenderPanelNumWidth, RenderPanelNumHeight, PannelPosX, PannelPosY);

	IwGxLightingOn();
	Utils::RenderText(loc_ui_settings, CIwRect(PannelPosX, settingsTitleYPos, widthSettingsPannel, 50), font_komikadisplaybold25, 0xff00edf8, IW_GX_FONT_ALIGN_CENTRE, IW_GX_FONT_ALIGN_TOP, true, 2); 
	
	Utils::RenderText(loc_ui_music, CIwRect(PannelPosX + xPadding, settingsTitleYPos + yPadding, midWidth, 50), font_komikadisplaybold18, 0xff00c0ff, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP, true, 1); 
	Utils::RenderText(loc_ui_fxSound, CIwRect(PannelPosX + xPadding, settingsTitleYPos + yPadding + yMargin, midWidth, 50), font_komikadisplaybold18, 0xff00c0ff, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP, true, 1); 
	
#if defined DESKTOPPLATFORM
	Utils::RenderText(loc_ui_fullscreen, CIwRect(PannelPosX + xPadding, settingsTitleYPos + yPadding + yMargin * 2, midWidth, 50), font_komikadisplaybold18, 0xff00c0ff, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP, true, 1); 
#endif
#if !defined DESKTOPPLATFORM
	Utils::RenderText(loc_ui_muteAll, CIwRect(PannelPosX + xPadding, settingsTitleYPos + yPadding + yMargin * 2, midWidth, 50), font_komikadisplaybold18, 0xff00c0ff, IW_GX_FONT_ALIGN_RIGHT, IW_GX_FONT_ALIGN_TOP, true, 1); 
#endif

	IwGxLightingOff();

	musicSlider.Render();
	fxSoundSlider.Render();

#if defined DESKTOPPLATFORM
	fullScreenCheckbox.Render();
#endif

#if !defined DESKTOPPLATFORM
	muteAllCheckbox.Render();
#endif

	btnAccept->Render();
}

void SettingsMenu::Update()
{
	if(!GetIsActive())
		return;

#if defined DESKTOPPLATFORM
	xbox360Controller.Update();
#endif

	changeVolumeTimer.Update();

	if(bIncreaseVolume)
	{
		if(bChangeVolumeFirstTime)
		{
			bChangeVolumeFirstTime = false;
			changeVolumeTimer.Start();
			VolumeUp();
		}
		else if(changeVolumeTimer.GetIsEnd())
		{
			changeVolumeTimer.SetTimeLimit(TIME_ITERATION_VOL);
			changeVolumeTimer.Start();
			VolumeUp();
		}
	}

	if(bDecreaseVolume)
	{
		if(bChangeVolumeFirstTime)
		{
			bChangeVolumeFirstTime = false;
			changeVolumeTimer.Start();
			VolumeDown();
		}
		else if(changeVolumeTimer.GetIsEnd())
		{
			changeVolumeTimer.SetTimeLimit(TIME_ITERATION_VOL);
			changeVolumeTimer.Start();
			VolumeDown();
		}
	}
#if !defined DESKTOPPLATFORM
	if(bIsAllMuted && (s3eAudioGetInt(S3E_AUDIO_VOLUME) > 0 || s3eSoundGetInt(S3E_SOUND_VOLUME) > 0))
	{
		bIsAllMuted = false;
		muteAllCheckbox.SetIsChecked(bAllMuted);
	}

	if(bIsAllMuted != muteAllCheckbox.GetIsChecked())
	{
		bIsAllMuted = muteAllCheckbox.GetIsChecked();
		if(bIsAllMuted)
		{
			myLastAudioVolume = s3eAudioGetInt(S3E_AUDIO_VOLUME);
			myLastSoundVolume = s3eSoundGetInt(S3E_SOUND_VOLUME);
			s3eAudioSetInt(S3E_AUDIO_VOLUME, 0);
			s3eSoundSetInt(S3E_SOUND_VOLUME, 0);
			musicSlider.SetSliderProgress(0.0f);
			musicSlider.ForceOnChangeValue();
			fxSoundSlider.SetSliderProgress(0.0f);
			fxSoundSlider.ForceOnChangeValue();
		}
		else
		{
			s3eAudioSetInt(S3E_AUDIO_VOLUME, myLastAudioVolume);
			s3eSoundSetInt(S3E_SOUND_VOLUME, myLastSoundVolume);
			musicSlider.SetSliderProgress(Utils::GetMusicVolume());
			fxSoundSlider.SetSliderProgress(Utils::GetFXSoundVolume());
		}
	}
#endif
}

void SettingsMenu::UpdateClickEvents(s3ePointerEvent *event)
{
	if(!GetIsActive())
		return;

	btnAccept->HandleClick(event);
	musicSlider.UpdateClickEvents(event);
	fxSoundSlider.UpdateClickEvents(event);

#if defined DESKTOPPLATFORM
	fullScreenCheckbox.UpdateClickEvents(event);
#endif

#if !defined DESKTOPPLATFORM
	muteAllCheckbox.UpdateClickEvents(event);
#endif
}

void SettingsMenu::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(!GetIsActive())
		return;

	btnAccept->HandleMotion(event);
	musicSlider.UpdateMotionsEvents(event);
	fxSoundSlider.UpdateMotionsEvents(event);

#if defined DESKTOPPLATFORM
	focusManager.UpdateMotionEvents(event);
#endif
}

void SettingsMenu::ToggleFullscreen()
{
#if defined DESKTOPPLATFORM
	if(fullScreenCheckbox.GetIsChecked() == Utils::GetIsFullscreen())
		return;

	if(fullScreenCheckbox.GetIsChecked())
		Utils::ToFullScreen();
	else
		Utils::ToWindowedScreen();
#endif
}

void SettingsMenu::VolumeUp()
{
	if(musicSlider.GetIsFocused())
	{	
		int currVol = (int)(Utils::GetMusicVolume() * Utils::GetMusicMaxValue());
		int finalVol = (currVol / 10) * 10 + 10;
		
		if(currVol % 10 > 0) 
			finalVol += 10;

		if(finalVol > Utils::GetMusicMaxValue())
			finalVol = Utils::GetMusicMaxValue();

		musicSlider.SetSliderProgress(finalVol / (float)Utils::GetMusicMaxValue());
		musicSlider.ForceOnChangeValue();
	}
	else if(fxSoundSlider.GetIsFocused())
	{
		int currVol = (int)(Utils::GetFXSoundVolume() * Utils::GetFXSoundMaxValue());
		int finalVol = (currVol / 10) * 10 + 10;

		if(currVol % 10 > 0) 
			finalVol += 10;

		if(finalVol > Utils::GetFXSoundMaxValue())
			finalVol = Utils::GetFXSoundMaxValue();

		fxSoundSlider.SetSliderProgress(finalVol / (float)Utils::GetFXSoundMaxValue());
		fxSoundSlider.ForceOnChangeValue();
	}
}

void SettingsMenu::VolumeDown()
{
	if(musicSlider.GetIsFocused())
	{	
		int currVol = (int)(Utils::GetMusicVolume() * Utils::GetMusicMaxValue());
		int finalVol = (currVol / 10) * 10 - 10;

		if(finalVol < 0)
			finalVol = 0;

		musicSlider.SetSliderProgress(finalVol / (float)Utils::GetMusicMaxValue());
		musicSlider.ForceOnChangeValue();
	}
	else if(fxSoundSlider.GetIsFocused())
	{
		int currVol = (int)(Utils::GetFXSoundVolume() * Utils::GetFXSoundMaxValue());
		int finalVol = (currVol / 10) * 10 - 10;

		if(finalVol < 0)
			finalVol = 0;

		fxSoundSlider.SetSliderProgress(finalVol / (float)Utils::GetFXSoundMaxValue());
		fxSoundSlider.ForceOnChangeValue();
	}
}