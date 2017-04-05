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

#include "MusicHandler.h"
#include "common.h"
#include "utils.h"

MusicHandler* MusicHandlerReference = NULL;

void MusicSound_OnClick_Action()
{
	if(MusicHandlerReference == NULL)
		return;

	MusicHandlerReference->MusicSound_Action();
}

void FxSound_OnClick_Action()
{
	if(MusicHandlerReference == NULL)
		return;

	MusicHandlerReference->FxSound_Action();
}

int32 KeyboardHandler_MusicHandler(void* sys, void*)
{
	s3eKeyboardEvent* event = (s3eKeyboardEvent*)sys;

	if(MusicHandlerReference != NULL)
	{
		MusicHandlerReference->KeyboardHandler(event);
		return 0;
	}
	else
	{
		return -1;
	}
}

MusicHandler::MusicHandler(int x, int y)
{
	MusicHandlerReference = this;
	MusicSoundRenderData.numStates = 2;
	TextureData MusicSoundData = TextureHelper::GetTextureNamed("btn_sound_music");
	MusicSoundRenderData.state[0].RollOut   = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 0);
	MusicSoundRenderData.state[0].RollOn  = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 2);
	MusicSoundRenderData.state[0].Pressed  = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 0);
	MusicSoundRenderData.state[0].Inactive = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 0);

	MusicSoundRenderData.state[1].RollOut   = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 1);
	MusicSoundRenderData.state[1].RollOn  = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 3);
	MusicSoundRenderData.state[1].Pressed  = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 1);
	MusicSoundRenderData.state[1].Inactive = TextureHelper::GetRenderData(MusicSoundData, 2, 4, 1);

	MusicSound = new MusicButton(TextureHelper::GetCIwTexture(MusicSoundRenderData.state[0].RollOut.atlasName), &MusicSoundRenderData, x, y);

	FxSoundRenderData.numStates = 2;
	TextureData FxSoundData = TextureHelper::GetTextureNamed("btn_sound_fx");
	FxSoundRenderData.state[0].RollOut   = TextureHelper::GetRenderData(FxSoundData, 2, 4, 0);
	FxSoundRenderData.state[0].RollOn  = TextureHelper::GetRenderData(FxSoundData, 2, 4, 2);
	FxSoundRenderData.state[0].Pressed  = TextureHelper::GetRenderData(FxSoundData, 2, 4, 0);
	FxSoundRenderData.state[0].Inactive = TextureHelper::GetRenderData(FxSoundData, 2, 4, 0);

	FxSoundRenderData.state[1].RollOut   = TextureHelper::GetRenderData(FxSoundData, 2, 4, 1);
	FxSoundRenderData.state[1].RollOn  = TextureHelper::GetRenderData(FxSoundData, 2, 4, 3);
	FxSoundRenderData.state[1].Pressed  = TextureHelper::GetRenderData(FxSoundData, 2, 4, 1);
	FxSoundRenderData.state[1].Inactive = TextureHelper::GetRenderData(FxSoundData, 2, 4, 1);

	FxSound    = new MusicButton(TextureHelper::GetCIwTexture(FxSoundRenderData.state[0].RollOut.atlasName), &FxSoundRenderData, x + 64 + 10, y);

	MusicSound->OnClick(&MusicSound_OnClick_Action);
	FxSound->OnClick(&FxSound_OnClick_Action);

	MusicSound->SetState(Music_Sound_State);
	FxSound->SetState(FX_Sound_State);

	bIsKeyPressed = false;

	s3eKeyboardRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_MusicHandler, NULL);

	TextureData colorData = TextureHelper::GetTextureNamed("label_tv_colors");
	blueButton = TextureHelper::GetRenderData(colorData, 1, 4, 3);
	yellowButton = TextureHelper::GetRenderData(colorData, 1, 4, 2);
}

MusicHandler::~MusicHandler()
{
	s3eKeyboardUnRegister(S3E_KEYBOARD_KEY_EVENT, KeyboardHandler_MusicHandler);

	delete MusicSound;
	delete FxSound;
}

void MusicHandler::Render()
{
	if(MusicSound != NULL)
		MusicSound->Render();
	
	if(FxSound != NULL)
		FxSound->Render();

	if(bRenderColorButtons)
	{
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(blueButton.atlasName), CIwSVec2(FxSound->GetX() + (int)(FxSound->GetWidth() * 0.5f) - 18, FxSound->GetY() + FxSound->GetHeight() + 5), blueButton);
		Utils::RenderSingleTexture(TextureHelper::GetCIwTexture(yellowButton.atlasName), CIwSVec2(MusicSound->GetX() + (int)(MusicSound->GetWidth() * 0.5f) - 18, MusicSound->GetY() + MusicSound->GetHeight() + 5), yellowButton);
	}
}

void MusicHandler::UpdateMotionEvents(s3ePointerMotionEvent* event)
{
	if(MusicSound != NULL)
		MusicSound->HandleMotion(event);
	
	if(FxSound != NULL)
		FxSound->HandleMotion(event);
}

void MusicHandler::UpdateClickEvents(s3ePointerEvent *event)
{
	if(MusicSound != NULL)
		MusicSound->HandleClick(event);
	
	if(FxSound != NULL)
		FxSound->HandleClick(event);
}

void MusicHandler::KeyboardHandler(s3eKeyboardEvent* event)
{
	if(event->m_Pressed)
	{
		bIsKeyPressed = true;
		//Music
		if(event->m_Key == s3eKeyRS)
		{
			//Set button to pressed status, it's the same imagen as idle so dont do anything.
		}
		//Fx Sound
		else if(event->m_Key == s3eKeyAbsASK)
		{
			//Set button to pressed status, it's the same imagen as idle so dont do anything.
		}
	}
	else
	{
		bIsKeyPressed = false;
	}

	if(bIsKeyPressed)
	{
		bIsKeyPressed = false;
		//Music
		if(event->m_Key == s3eKeyRS)
		{
			MusicSound_Action();
		}
		//Fx Sound
		else if(event->m_Key == s3eKeyAbsASK)
		{
			FxSound_Action();
		}
	}
}

void MusicHandler::MusicSound_Action()
{
	MusicSound->GoToNextState();

	if(bUseOldAudioSystem)
	{
		if(MusicSound->GetState() == Music_Full)
		{
			bIsAudioPaused = false;
			Music_Sound_State = NORMAL;

			if(LastMusicStacked == Music_None)
			{
				IwTrace(MusicHandler_Trace, ("Resumiendo."));
				s3eAudioResume();
			}
			else if(LastMusicStacked == Music_Loose)
			{
				IwTrace(MusicHandler_Trace, ("Loose."));
				s3eAudioStop();
				s3eAudioPlay("bgm/sd_fanfare_lose_theme.mp3", 1);
			}
			else if(LastMusicStacked == Music_Win)
			{
				IwTrace(MusicHandler_Trace, ("Win."));
				s3eAudioStop();
				s3eAudioPlay("bgm/sd_fanfare_win_theme.mp3", 1);
			}
			else if(LastMusicStacked == Music_Ingame_01)
			{
				IwTrace(MusicHandler_Trace, ("Ingame 01."));
				s3eAudioStop();
				if(currentMap == 1 && bShowTutorialLevel1)
					s3eAudioPlay("bgm/sd_ingame_01_theme_loop.mp3", 0);
				else
					s3eAudioPlay("bgm/sd_ingame_01_theme_loop.mp3", 1);
			}
			else if(LastMusicStacked == Music_Ingame_02)
			{
				IwTrace(MusicHandler_Trace, ("Ingame 02."));
				s3eAudioStop();
				s3eAudioPlay("bgm/sd_ingame_02_theme_loop.mp3", 0);
			}
			else if(LastMusicStacked == Music_Store)
			{
				IwTrace(MusicHandler_Trace, ("Store."));
				s3eAudioStop();
				s3eAudioPlay("bgm/sd_store_theme_loop.mp3", 0);
			}
			else if(LastMusicStacked == Music_MainMenu)
			{
				IwTrace(MusicHandler_Trace, ("MainMenu."));
				s3eAudioStop();
				s3eAudioPlay("bgm/sd_mainmenu_theme_loop.mp3", 0);
			}

			LastMusicStacked = Music_None;
		}
		else if(MusicSound->GetState() == Music_Muted)
		{
			bIsAudioPaused = true;
			s3eAudioPause();
		}
	}
	else
	{
		if(MusicSound->GetState() == Music_Full)
		{
			bIsAudioPaused = false;
			Music_Sound_State = NORMAL;

			if(LastMusicStacked == Music_None)
			{
				Utils::PlayAudio(CurrentMusicPlaying);
			}
			else
			{
				LastMusicPosition = 0;
				Utils::PlayAudio(LastMusicStacked);
			}

			s3eAudioSetInt(S3E_AUDIO_POSITION, LastMusicPosition);

			LastMusicStacked = Music_None;
		}
		else if(MusicSound->GetState() == Music_Muted)
		{
			bIsAudioPaused = true;
			if(inGamePausedState != GSGP_MainMenuIG)
			{
				LastMusicPosition = s3eAudioGetInt(S3E_AUDIO_POSITION);
				s3eAudioStop();
			}
		}
	}
}

void MusicHandler::FxSound_Action()
{
	FxSound->GoToNextState();

	if(FxSound->GetState() == Music_Full)
	{
		FX_Sound_State = NORMAL;
		Utils::ResumeAllFXSounds();
	}
	else if(FxSound->GetState() == Music_Muted)
	{
		FX_Sound_State = MUTE;
		Utils::StopAllFXSounds();
	}
}
