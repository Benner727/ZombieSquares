#ifndef TIMER_H
#define TIMER_H

#include <SDL.h>

class Timer
{
private:
	static Timer* sInstance;

	Uint32 mStartTicks;
	Uint32 mEndTicks;

	float mDeltaTime;

	Timer();
	~Timer();

public:
	static Timer* Instance();
	static void Release();

	float DeltaTime();

	void Update();
};

#endif