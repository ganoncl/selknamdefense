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

#ifndef _MODEL3D_H
#define _MODEL3D_H

#define MAX_MODELS 10
#define MAX_ANIMATIONS 10

#include "IwGx.h"
#include "IwAnim.h"
#include "IwAnimCore.h"
#include "IwAnimSkel.h"
#include "IwGraphics.h"
#include "IwAnimPlayer.h"
#include "timer.h"

class Model3D
{
public:
	Model3D();
	~Model3D();
	void Update(bool bForceUpdate = false);
	void Render();
	void SetRotation(CIwFVec3 rotation);
	void SetPosition(CIwFVec3 position);
	void AddModel(CIwModel* model, CIwAnimSkin* skin);
	void SetSkel(CIwAnimSkel* skel);
	void AddAnim(CIwAnim* anim);
	void PlayAnim(int animID, float animSpeed = 1.0f, bool bLoop = true);
	void Move(CIwFVec3 desp, int time);
	bool IsMoving();
	CIwFVec3 GetBonePosition(int index);
	CIwFVec3 GetPosition();
	bool IsAnimating();
	void HidModel(int index);
	void UnhidModel(int index);
	void RenderSingleModel(int index);
private:
	CIwFVec3 s_Angles; //Rotation
	CIwFVec3 totalDesp;
	CIwFVec3 initialPosition;
	CIwFMat s_ModelMatrix; //Position
	CIwModel*    s_Model[MAX_MODELS]; //Models - rendering order determined by array index.
	CIwAnimSkin* s_Skin[MAX_MODELS];
	CIwAnimSkel* s_Skel;
	CIwAnimPlayer*  s_Player;
	CIwAnim* anims[MAX_ANIMATIONS];
	int numModels;
	int numAnims;
	float   s_BlendDur;
	Timer movementTimer;
	bool hiddenModel[MAX_MODELS];
	bool bStopAnim;
};

#endif