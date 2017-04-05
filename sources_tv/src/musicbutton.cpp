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

#include "musicbutton.h"

MusicButton::MusicButton(CIwTexture* tex, ButtonRenderData* data, int x, int y) : Button(tex, data, x, y)
{
	musicButtonState = Music_Full;
}

MusicButton::~MusicButton()
{

}

void MusicButton::SetState(SoundStatus sound)
{/*
	if(sound == HALF)
	{
		musicButtonState = Music_Half;
		SetImage((int)musicButtonState);
	}
	else*/ if(sound == NORMAL)
	{
		musicButtonState = Music_Full;
		SetImage((int)musicButtonState);
	}
	else if(sound == MUTE)
	{
		musicButtonState = Music_Muted;
		SetImage((int)musicButtonState);
	}
}

void MusicButton::Render()
{
	Button::Render();
}

MusicButtonState MusicButton::GoToNextState()
{
	
	int nextState = ((int)musicButtonState + 1) % 2;
	
	this->SetImage(nextState);

	musicButtonState = (MusicButtonState)nextState;
	return musicButtonState;
}

