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

#ifndef _ANIMATION_H
#define _ANIMATION_H

#include "timer.h"

struct SeqList
{
	int value[15];
};

#define MAX_SEQ_SIZE 60

class Animation
{
public:
	Animation();
	~Animation();
	void AddAnimation(const char* name, int* sequence, int seqLength, int* time_seq);
	void SetAnim(const char* name, bool bLoop = false, int delay = 50, int initial_id = 0);
	int GetCurrentAnimID();
	const char* GetCurrenAnimName();
	void NextAnim();
	void ResetAnim();
	bool AnimIsEnded() { return currentID == (GetSeqLength(currentAnim) - 1); };
	void PauseAnim();
	void ResumeAnim();

	//Add factor to each seq_time amount.
	void UpdateAnimExtraTime(const char* anim, int factor);
	void ClearAnimation();

private:
	SeqList GetSeq(const char* str);
	SeqList GetSeqTime(const char* str);
	SeqList* GetSeqExtraTime(const char* str);
	int GetSeqLength(const char* str);

	const char* seq_animName[MAX_SEQ_SIZE];

	int num_seqs;

	SeqList seq_SeqList[MAX_SEQ_SIZE];
	SeqList seq_time_SeqList[MAX_SEQ_SIZE];
	SeqList seq_extra_time_SeqList[MAX_SEQ_SIZE];
	int seqLength_int[MAX_SEQ_SIZE];

	int currentID;
	const char* currentAnim;
	int delay;
	int currDelay;
	bool bLoop;
	GameTimer delay_timer;
	bool bPausedAnimation;
};

#endif