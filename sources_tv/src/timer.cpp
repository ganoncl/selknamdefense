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

#include "timer.h"

Timer::Timer()
{
	//no countdown mode for default
	time_limit = 0; 
	currTime = 0;
	lastTime = 0;
	time   = 0;
	bStop  = true;
	bEnd   = false;
	bStart = false;
}

Timer::~Timer()
{

}

void Timer::Start()
{
	bStop = false;
	bEnd = false;
	bStart = true;
	time = 0;
	currTime = GetTimer();
	lastTime = GetTimer();
}

void Timer::SetTimeLimit(int time_limit) 
{
	this->time_limit = time_limit;
}

void Timer::Stop()
{
	this->bStop = true;
}

void Timer::Resume()
{
	if(!bStart)
		return;

	this->bStop = false;
	currTime = GetTimer();
	lastTime = GetTimer();
}

void Timer::Reset()
{
	time = 0;
	bStop = true;
	bEnd = false;
}

int Timer::GetCurrTime()
{
	return time;
}

int Timer::GetCurrTime_Reversed()
{
	if(time_limit == 0 || time_limit < time) return 0;
	return time_limit - time;
}

int Timer::GetTimer()
{
	return (int)(s3eTimerGetMs());
}

void Timer::Update()
{
	if(bStop || bEnd)
		return;

	time += currTime - lastTime;
	lastTime = currTime;
	currTime = GetTimer();

	if(time_limit != 0 && time > time_limit) 
	{
		bEnd = true;
	}
}

bool Timer::GetIsEnd()
{
	return bEnd;
}

int Timer::GetTimeLimit()
{
	return time_limit;
}

bool Timer::GetIsStop()
{
	return bStop;
}

void Timer::Clear()
{
	time = 0;
	bStop = true;
	bEnd = false;
	bStart = false;
	time_limit = 0;
	currTime = 0;
	lastTime = 0;
}