#pragma once
#include<windows.h>
namespace simpletime
{
	double time = 0;
	LARGE_INTEGER nFreq;
	LARGE_INTEGER nBeginTime;
	LARGE_INTEGER nEndTime;
	void start()
	{
		QueryPerformanceFrequency(&nFreq);
		QueryPerformanceCounter(&nBeginTime);
	}
	float milliseconds()
	{
		QueryPerformanceCounter(&nEndTime);
		time = (double)(nEndTime.QuadPart - nBeginTime.QuadPart) / (double)nFreq.QuadPart;
		return time * 1000;
	}
}