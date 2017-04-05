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

#ifndef _SPLASHSCREEN_H
#define _SPLASHSCREEN_H

#include "timer.h"
#include "texturehelper.h"

class SplashScreen
{
public:
	SplashScreen(uint8 delay = 4, uint8 skip = 2, int fadeSpeed = 10);
	~SplashScreen();
	bool bIsActiveObj;
	void InitObject();
	void EndObject();
	void Update();
	void Render();
	void AddRes(const char* resName);
	bool SplashOver(); //Termina la animación.
	void SkipImage();
private:
	CIwResGroup* sGroup;
	CIwFVec2	s_UV[4];

	uint8 initialTime;
	uint8 nextImageTime;
	uint8 skipTime;

	TextureData textures[3]; 
	int numTextures;
	int currText;
	bool bSplashOver; //Verifica si termina o no la transicion de imagenes.

	int _width; //width de la imagen actual.
	int _height; //height de la imagen actual.

	int fSpeed; //Fade Speed [0, 255]

	bool bSkip;

	Timer AuxTimer;
	bool bPlaySilence;
	UVInfo uvInfo;
};

#endif //_SPLASHSCREEN_H