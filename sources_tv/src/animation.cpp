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

#include "animation.h"
#include "IwGx.h"
Animation::Animation()
{
	currentAnim = "";
	currentID = 0;
	delay = 100; //Max animation velocity
	currDelay = 0;
	bLoop = false; //Loop when end is reached.
	bPausedAnimation = false;
}

Animation::~Animation()
{

}

void Animation::AddAnimation(string name, int* sequence, int seqLength, int* time_seq)
{
	seq_string[num_seqs] = name;
	seqLength_int[num_seqs] = seqLength;
	for(int i = 0; i < seqLength; i++)
	{
		//seq_string[num_seqs] = name;
		seq_SeqList[num_seqs].value[i] = sequence[i];

		//seq_time_string[num_seqs] = name;
		seq_time_SeqList[num_seqs].value[i] = time_seq[i];

		//seq_extra_time_string[num_seqs] = name;
		seq_extra_time_SeqList[num_seqs].value[i] = 0;
	}
	num_seqs++;
}

void Animation::SetAnim(string name, bool bLoop, int delay, int initial_id)
{
	if(currentAnim == name)
		return;

	this->currentAnim = name;
	this->currentID = 0;
	this->bLoop = bLoop;

	if(GetSeqLength(currentAnim) != 0)
		this->delay = delay / GetSeqLength(currentAnim);
	else
		this->delay = 10000;

	delay_timer.SetTimeLimit((GetSeqTime(currentAnim).value[0] + GetSeqExtraTime(currentAnim).value[0]) * 10);
	delay_timer.Reset();
	delay_timer.Start();
}

int Animation::GetCurrentAnimID()
{                                   
	return GetSeq(currentAnim).value[currentID];
}

void Animation::NextAnim()
{
	if(bPausedAnimation) 
		return;

	delay_timer.Update();

	if(delay_timer.GetIsEnd())
	{
		currentID++;
		if(currentID < GetSeqLength(currentAnim))
		{
			delay_timer.SetTimeLimit((GetSeqTime(currentAnim).value[currentID] + GetSeqExtraTime(currentAnim).value[currentID]) * 10);
			delay_timer.Reset();
			delay_timer.Start();
		}

	}

	if(currentID >= GetSeqLength(currentAnim))
	{
		if(bLoop) {
			currentID = 0;
			delay_timer.SetTimeLimit((GetSeqTime(currentAnim).value[currentID] + GetSeqExtraTime(currentAnim).value[currentID]) * 10);
			delay_timer.Reset();
			delay_timer.Start();
		}
		else currentID = GetSeqLength(currentAnim) - 1;
	}
}

string Animation::GetCurrenAnimName()
{
	return currentAnim;
}

void Animation::ResetAnim()
{
	currentID = 0;
	delay_timer.Reset();
	delay_timer.Start();
	ResumeAnim();
}

int Animation::GetTotalAnimationTime(string anim)
{
	int len = 0;
	for(int i = 0; i < GetSeqLength(currentAnim); i++)
		len += GetSeqTime(currentAnim).value[currentID] + GetSeqExtraTime(currentAnim).value[currentID];
	return len;
}

void Animation::PauseAnim() {
	bPausedAnimation = true;
}

void Animation::ResumeAnim() {
	bPausedAnimation = false;
}

void Animation::UpdateAnimTime(string anim, int factor)
{
	int len = GetSeqLength(anim);
	//IwTrace(ANIMBUG, ("AnimLen: %d", len));
	for(int i = 0; i < len; i++)
	{
		GetSeqExtraTime(anim).value[i] = factor;
		//IwTrace(ANIMBUG, ("seq %d: %d", i, seq_time[anim].value[i]));
	}
}

SeqList Animation::GetSeq(string str)
{
	for(int i = 0; i < num_seqs; i++)
	{
		if(str.compare(seq_string[i]) == 0)
		{
			return seq_SeqList[i];
		}
	}

	return seq_SeqList[0];
}

SeqList Animation::GetSeqTime(string str)
{
	for(int i = 0; i < num_seqs; i++)
	{
		if(str.compare(seq_string[i]) == 0)
		{
			return seq_time_SeqList[i];
		}
	}

	return seq_time_SeqList[0];
}

SeqList Animation::GetSeqExtraTime(string str)
{
	for(int i = 0; i < num_seqs; i++)
	{
		if(str.compare(seq_string[i]) == 0)
		{
			return seq_extra_time_SeqList[i];
		}
	}

	return seq_extra_time_SeqList[0];
}

int Animation::GetSeqLength(string str)
{
	for(int i = 0; i < num_seqs; i++)
	{
		if(str.compare(seq_string[i]) == 0)
		{
			//IwTrace(BOSSBATTLE, ("%s", str.c_str()));
			return seqLength_int[i];
		}
	}

	return seqLength_int[0];
}

void Animation::ClearAnimation()
{
	num_seqs = 0;
}