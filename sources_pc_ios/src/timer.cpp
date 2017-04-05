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
#include "common.h"

Timer::Timer()
{
	//no countdown mode for default
	maxTimeMilisec = 0;
	currentTimeMilisec = 0;
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
	currentTimeMilisec = 0;
}

void Timer::SetTimeLimit(int _maxTimeMilisec)
{
	this->maxTimeMilisec = _maxTimeMilisec;
}

void Timer::Stop()
{
	this->bStop = true;
	bEnd = false;
}

void Timer::Resume()
{
	if(!bStart)
		return;

	this->bStop = false;
}

void Timer::Reset()
{
	currentTimeMilisec = 0;
	bStop = true;
	bEnd = false;
}

int Timer::GetCurrTime()
{
	return currentTimeMilisec;
}

int Timer::GetCurrTime_Reversed()
{
	if(maxTimeMilisec == 0 || maxTimeMilisec < currentTimeMilisec)
		return 0;
	return maxTimeMilisec - currentTimeMilisec;
}

void Timer::Update()
{
	if(bStop || bEnd)
		return;

	currentTimeMilisec += (int)deltaSystemTime;

	if(maxTimeMilisec != 0 && currentTimeMilisec > maxTimeMilisec) 
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
	return maxTimeMilisec;
}

bool Timer::GetIsStop()
{
	return bStop;
}

void Timer::Clear()
{
	currentTimeMilisec = 0;
	bStop = true;
	bEnd = false;
	bStart = false;
	maxTimeMilisec = 0;
}

//////////////////////////GAME TIMER///////////////////////////////

GameTimer::GameTimer()
{
	//no countdown mode for default
	maxTimeMilisec = 0;
	currentTimeMilisec = 0;
	bStop  = true;
	bEnd   = false;
	bStart = false;
}

GameTimer::~GameTimer()
{

}

void GameTimer::Start()
{
	bStop = false;
	bEnd = false;
	bStart = true;
	currentTimeMilisec = 0;
}

void GameTimer::SetTimeLimit(int _maxTimeMilisec)
{
	this->maxTimeMilisec = _maxTimeMilisec;
}

void GameTimer::Stop()
{
	this->bStop = true;
	bEnd = false;
}

void GameTimer::Resume()
{
	if(!bStart)
		return;

	this->bStop = false;
}

void GameTimer::Reset()
{
	currentTimeMilisec = 0;
	bStop = true;
	bEnd = false;
}

int GameTimer::GetCurrTime()
{
	return currentTimeMilisec;
}

int GameTimer::GetCurrTime_Reversed()
{
	if(maxTimeMilisec == 0 || maxTimeMilisec < currentTimeMilisec)
		return 0;
	return maxTimeMilisec - currentTimeMilisec;
}

void GameTimer::Update()
{
	if(bStop || bEnd)
		return;

	float myDeltaGameTime = (float)deltaSystemTime;
	myDeltaGameTime = myDeltaGameTime * currentGameSpeedValue;
	currentTimeMilisec += (int)myDeltaGameTime;

	if(maxTimeMilisec != 0 && currentTimeMilisec > maxTimeMilisec) 
	{
		bEnd = true;
	}
}

bool GameTimer::GetIsEnd()
{
	return bEnd;
}

int GameTimer::GetTimeLimit()
{
	return maxTimeMilisec;
}

bool GameTimer::GetIsStop()
{
	return bStop;
}

void GameTimer::Clear()
{
	currentTimeMilisec = 0;
	bStop = true;
	bEnd = false;
	bStart = false;
	maxTimeMilisec = 0;
}

//////////////////////////WAVE TIMER///////////////////////////////

WaveTimer::WaveTimer()
{
	//no countdown mode for default
	maxTimeSecond = 0;
	currentTimeSecond = 0;
	fpsCounter = 0;
	bStop  = true;
	bEnd   = false;
	bStart = false;
	fpsParam = UPS;
	currentGameSpeedCheck = 0;
}

WaveTimer::~WaveTimer()
{

}

void WaveTimer::SetMaxTimeSecond(int time_limit)
{
	this->maxTimeSecond = time_limit;
}

void WaveTimer::Start()
{
	currentTimeSecond = 0;
	fpsCounter = 0;
	bStop = false;
	bEnd = false;
	bStart = true;
}

void WaveTimer::Stop()
{
	this->bStop = true;
	bEnd = false;
}

void WaveTimer::Reset()
{
	currentTimeSecond = 0;
	bStop = true;
	bEnd = false;
}

void WaveTimer::Resume()
{
	if(!bStart)
		return;

	this->bStop = false;
}

int WaveTimer::GetCurrTime()
{
	return currentTimeSecond;
}

int WaveTimer::GetCurrTime_Reversed()
{
	if(maxTimeSecond == 0 || maxTimeSecond < currentTimeSecond)
		return 0;
	return maxTimeSecond - currentTimeSecond;
}

void WaveTimer::Update()
{
	if(currentGameSpeedCheck != waveTimerSpeed)
	{
		currentGameSpeedCheck = waveTimerSpeed;
		UpdateGameTimerMultiplier();
	}

	if(bStop || bEnd)
		return;

	fpsCounter++;
	if(fpsCounter >= fpsParam)
	{
		currentTimeSecond++;
		fpsCounter = 0;
	}

	if(maxTimeSecond != 0 && currentTimeSecond > maxTimeSecond) 
	{
		bEnd = true;
	}
}

void WaveTimer::UpdateGameTimerMultiplier()
{
	int tmpTimerCounterValue = (int)(UPS * (1.0f / currentGameSpeedValue));
	fpsParam = tmpTimerCounterValue;
}

bool WaveTimer::GetIsEnd()
{
	return bEnd;
}

int WaveTimer::GetTimeLimit()
{
	return maxTimeSecond;
}

bool WaveTimer::GetIsStop()
{
	return bStop;
}

void WaveTimer::Clear()
{
	maxTimeSecond = 0;
	currentTimeSecond = 0;
	fpsCounter = 0;
	bStop = true;
	bEnd = false;
	bStart = false;
}

LerpTimer::LerpTimer()
{
	bActive = false;
}

LerpTimer::~LerpTimer()
{

}

void LerpTimer::Start(int milisecs)
{
	maxTime = milisecs;
	currTime = 0;
	bActive = true;
}

float LerpTimer::GetLerpConstant()
{
	if(!bActive)
		return 1;

	float f = currTime / (float)maxTime; 
	if(f > 1) return 1;
	else return f;
}

void LerpTimer::Finish()
{
	bActive = false;
}

void LerpTimer::Update()
{
	if(bActive)
	{
		currTime += deltaSystemTime;

		if(currTime > maxTime)
		{
			bActive = false;
		}
	}
}