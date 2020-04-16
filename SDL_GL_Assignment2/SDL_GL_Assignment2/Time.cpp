#include "Time.h"

Time* Time::sInstance = NULL;

Time::Time()
{
	startTime = std::chrono::high_resolution_clock::now();
}

Time* Time::Instance()
{
	if (sInstance == NULL)
	{
		sInstance = new Time();
	}
	return sInstance;
}

void Time::NewFrame()
{
	start = std::chrono::high_resolution_clock::now();
}

float Time::EndFrame()
{
	end = std::chrono::high_resolution_clock::now();
	duration = end - start;
	deltaTime = duration.count();
	deltaTimes[index] = deltaTime;
	index++;
	if (index >= 100)
	{
		index = 0;
	}
	return deltaTime;
}

float Time::GetTimeSinceStart()
{
	std::chrono::duration<double> diff = std::chrono::high_resolution_clock::now() - startTime;
	return diff.count();
}