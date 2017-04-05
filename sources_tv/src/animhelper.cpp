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

#include "IwGx.h"
#include "common.h"
#include "animhelper.h"
#include "stdio.h"
#include "isoterrain.h"
#include "game.h"

AnimHelper::AnimHelper() { }
AnimHelper::~AnimHelper() { }

void AnimHelper::RetrieveAnim(int *animList, int *animListTimer, IsoPawnAnim myAnimPawn, IsoAnim myAnimType, IsoDirection myAnimDirection)
{
	int myAnimPawnNum = (int)myAnimPawn;
	int myAnimTypeNum = (int)myAnimType - 1;
	int myAnimDirectionNum = (int)myAnimDirection;
	//retrieve time
	for (int i = 0; i < MAX_ANIMATION; i++)
	{
		animListTimer[i] = Anim_Pawns[myAnimPawnNum][myAnimTypeNum][0][i];
		//IwTrace(APPLOG, ("ANIMLISTTIMER: %d = %d", i, animListTimer[i]));
	}
	//retrieve anim data
	for (int i = 0; i < MAX_ANIMATION; i++)
	{
		animList[i] = Anim_Pawns[myAnimPawnNum][myAnimTypeNum][myAnimDirection][i];
	}
}

