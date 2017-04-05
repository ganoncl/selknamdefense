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

#include "model3d.h"
#include "common.h"
#include "utils.h"

Model3D::Model3D()
{
	s_Player = new CIwAnimPlayer;

	numModels  = 0;
	numAnims   = 0;
	s_BlendDur = 0.25f;
	
	SetRotation(CIwFVec3(0, PI, 0));
	//SetPosition(CIwFVec3(400, 250, 300));
	SetPosition(CIwFVec3(150, 50, 300));
	
	bStopAnim = true;
}

Model3D::~Model3D()
{
	delete s_Player;
}

bool Model3D::IsAnimating()
{
	return !s_Player->IsCurrentAnimComplete();
}

void Model3D::Update(bool bForceUpdate)
{
	CIwFMat rotX, rotY, rotZ;
	rotX.SetRotX(s_Angles.x);
	rotY.SetRotY(s_Angles.y);
	rotZ.SetRotZ(PI);
	s_ModelMatrix.CopyRot(rotZ * rotX * rotY);

	//Movement
	if(!Utils::GameIsPaused() || dialogController->IsActive() || bForceUpdate)
	{
		if(movementTimer.GetIsStop()) movementTimer.Resume();

		movementTimer.Update();
	}
	else
	{
		movementTimer.Stop();
	}
	
	if(!movementTimer.GetIsStop() && !movementTimer.GetIsEnd())
	{
		float newX = initialPosition.x + (totalDesp.x / (float)movementTimer.GetTimeLimit()) * movementTimer.GetCurrTime();
		float newY = initialPosition.y + (totalDesp.y / (float)movementTimer.GetTimeLimit()) * movementTimer.GetCurrTime();
		float newZ = initialPosition.z + (totalDesp.z / (float)movementTimer.GetTimeLimit()) * movementTimer.GetCurrTime();
	
		s_ModelMatrix.t = CIwFVec3(newX, newY, newZ);
	}

	s_Player->Update(1.0f / 30.0f);
	IwGxTickUpdate();
}

void Model3D::Render()
{
	IwGxSetModelMatrix(&s_ModelMatrix);
	IwAnimSetSkelContext(s_Player->GetSkel());

	for(int i = 0; i < numModels; i++)
	{
		if(!hiddenModel[i])
		{
			IwAnimSetSkinContext(s_Skin[i]);
			s_Model[i]->Render();
		}
	}

	IwAnimSetSkelContext(NULL);
	IwAnimSetSkinContext(NULL);
}

void Model3D::RenderSingleModel(int index)
{
	IwGxSetModelMatrix(&s_ModelMatrix);
	IwAnimSetSkelContext(s_Player->GetSkel());

	IwAnimSetSkinContext(s_Skin[index]);
	s_Model[index]->Render();

	IwAnimSetSkelContext(NULL);
	IwAnimSetSkinContext(NULL);

	//Restoring normal configuration for a 2D Isometric view right after rendering 3D Model.
	static CIwFMat view = CIwFMat::g_Identity;
	view = CIwFMat::g_Identity;
	IwGxSetViewMatrix(&view);
	IwGxSetModelMatrix(&view);
}

void Model3D::HidModel(int index)
{
	hiddenModel[index] = true;
}

void Model3D::UnhidModel(int index)
{
	hiddenModel[index] = false;
}

void Model3D::AddModel(CIwModel* model, CIwAnimSkin* skin)
{
	s_Model[numModels] = model;
	s_Skin[numModels] = skin;
	hiddenModel[numModels] = false;
	s_Model[numModels]->ApplyScale(1.3f);

	numModels++;
}

void Model3D::SetRotation(CIwFVec3 rotation)
{
	s_Angles = rotation;
}

void Model3D::SetPosition(CIwFVec3 position)
{
	s_ModelMatrix.t = position;
}

void Model3D::SetSkel(CIwAnimSkel* skel)
{
	s_Skel = skel;
	s_Player->SetSkel(s_Skel);
}

void Model3D::AddAnim(CIwAnim* anim)
{
	anims[numAnims++] = anim;
}

void Model3D::PlayAnim(int animID, float animSpeed, bool bLoop)
{
	animSpeed = animSpeed * FPSMULTIPLIER;
	if(bLoop)
	{
		s_Player->PlayAnim(anims[animID], animSpeed, CIwAnimBlendSource::LOOPING_F, s_BlendDur);
	}
	else
	{
		s_Player->PlayAnim(anims[animID], animSpeed);
	}
}

void Model3D::Move(CIwFVec3 desp, int time)
{
	totalDesp = desp;
	initialPosition = s_ModelMatrix.t;

	movementTimer.SetTimeLimit(time);
	movementTimer.Reset();
	movementTimer.Start();
}

bool Model3D::IsMoving()
{
	return !movementTimer.GetIsStop() && !movementTimer.GetIsEnd();
}

CIwFVec3 Model3D::GetBonePosition(int index)
{
	return s_Skel->GetBoneFromID(index)->GetPos();
}

CIwFVec3 Model3D::GetPosition()
{
	return s_ModelMatrix.t;
}