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

#ifndef _FADESCREEN_H
#define _FADESCREEN_H

class FadeScreen {
public:
	FadeScreen(int r = 0, int g = 0, int b = 0);
	~FadeScreen();
	void Update();
	void Render();
	int GetAlpha();
	void SetAlpha(int alpha);
	void FadeIn(int speed = 30, int maxAlpha = 255);
	void FadeOut(int speed = 30, int minAlpha = 0);
	void Fade(int speed = 30, int minAlpha = 0, int maxAlpha = 255);

private:
	int _fadeAlpha; //Alpha Channel, Transition between images.
	bool bFade;   //True: Fade Active, False: Fade Inactive.
	bool bFadeIn; //True: FadeIn, False: FadeOut
	bool bCompleteFade; 
	int fSpeed; //Fade Speed [0, 255]
	int maxAlpha; //Max alpha when FadeIn is called.
	int minAlpha; //Min alpha when FadeOut is called.
	int r;
	int g;
	int b;
};

#endif