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

#ifndef _BACKGROUND_H
#define _BACKGROUND_H
#include "IwGx.h"

class RenderData;

class Background {
public:
	Background(int r, int g, int b, int a = 255, int width = -1, int height = -1, float x = 0.0f, float y = 0.0f);
	Background(CIwTexture* tex, int width = -1, int height = -1, float x = 0.0f, float y = 0.0f);
	Background(int numImage);
	~Background();
	void Render();
	void SetAlpha(int _a) { this->a = _a; };
	
private:
	bool onlyColor; //Background sin textura, solo color.
	int screenW;
	int screenH;
	float _x;
	float _y;
	float _z;
	//Background color, only if in color mode
	int r;
	int g;
	int b;
	int a;
	//Mosaic mode (Assumption: Max resolution -> 1280 x 768, Texture min: 128 x 128)
	CIwTexture* tex;
	CIwFVec3 pos[4 * 60];
	CIwFVec2 s_UV[4 * 60];
	int numBlocks;

	int numImage;

	RenderData *backgroundData;

	int xFullScreenPos;
};

#endif