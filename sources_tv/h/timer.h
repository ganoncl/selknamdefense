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

#ifndef _TIMER_DEFENSEX_H
#define _TIMER_DEFENSEX_H

#include "s3e.h"

//Simulates a clock

class Timer
{
public:
	Timer();
	~Timer();
	//Must be called to start the timer
	void Start(); 
	//Must be called if you want to use GetCurrTime_Reversed(), time_limit must be greater than or equal to 0
	void SetTimeLimit(int time_limit); 
	//variable time doesn't change
	void Stop(); 
	//timer starts again
	void Resume(); 
	//time = 0
	void Reset();
	int GetCurrTime(); 
	int GetCurrTime_Reversed();
	int GetTimeLimit();
	bool GetIsStop();
	bool GetIsEnd();
	void Update();
	void Clear();
private:
	//Internal function, transform miliseconds to seconds.
	int GetTimer(); //return s3eGetTimerMs() * 0.001.
	//Used when timer is on countdown mode
	int time_limit;
	//Variables used to know the time difference between frames.
	int currTime;
	int lastTime;
	//time of the timer
	int time;
	//Timer stats
	//bStop is true when Stop() is called
	bool bStop; 
	//bEnd is true when time >= time_limit
	bool bEnd;
	//bStart is true when Start() is called
	bool bStart;
};

#endif