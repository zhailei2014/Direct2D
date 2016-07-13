#pragma once
#include "IShare.h"

class HTimer
{
public:
	HTimer()
	{
		LARGE_INTEGER lInt;
		QueryPerformanceFrequency(&lInt);
		frequency = lInt.QuadPart;
		Reset();
	}
	~HTimer(){};

private:
	long long startTime;
	long long lastCallToUpdate;
	long long currentCallToUpdate;
	long long frequency;
public:
	inline void Reset()
	{
		LARGE_INTEGER lInt;
		QueryPerformanceCounter(&lInt);
		startTime = lInt.QuadPart;
		lastCallToUpdate = lInt.QuadPart;
		currentCallToUpdate = lInt.QuadPart;
	}
	inline void Update()
	{
		lastCallToUpdate = currentCallToUpdate;
		LARGE_INTEGER lInt;
		QueryPerformanceCounter(&lInt);
		currentCallToUpdate = lInt.QuadPart;
	}
	inline double GetTimeTotal()
	{
		double d = currentCallToUpdate - startTime;
		return d / frequency;
	}
	inline double GetTimeDelta()
	{
		double d = currentCallToUpdate - lastCallToUpdate;
		return d / frequency;
	}
};
