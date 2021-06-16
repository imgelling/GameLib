// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameTimer.h"

// METHODS /////////////////////////////////////////////////////////////////////

GameTimer::GameTimer()
{
	now = { 0.0 };
	resolution = std::chrono::high_resolution_clock::period::den;
    ResetTimer();
}

void GameTimer::ResetTimer()
{
	endFrame = std::chrono::high_resolution_clock::now();// SDL_GetTicks();
    startTime = endFrame;
}

GameTimeState GameTimer::Now()
{
	SaveState();
    return now;
}

void GameTimer::Update()
{
	endFrame = std::chrono::high_resolution_clock::now();// SDL_GetTicks();
}

inline std::chrono::microseconds GameTimer::Diff()
{

	return std::chrono::duration_cast<std::chrono::microseconds>((std::chrono::high_resolution_clock::now()/*SDL_GetTicks()*/ - endFrame));
}

float GameTimer::MilliSecondsElapsedf()
{
	
	std::chrono::microseconds ticks = (Diff());
	//std::cout << ticks.count() << std::endl;
	return (float)(ticks.count()/1000.0f);
}

double GameTimer::MilliSecondsElapsed()
{
	return (double)(Diff().count() / 1000.0f);
}

int GameTimer::MilliSecondsElapsedi()
{
	return (int)(Diff().count() / 1000.0f);
}

float GameTimer::SecondsElapsedf()
{
	return (float)(Diff().count() / 1000000.0f);
}

double GameTimer::SecondsElapsed()
{
	return (double)(((Diff().count()) / 1000000.0));
}

int GameTimer::SecondsElapsedi()
{
	return (int)((Diff().count()) / 1000000.0f);
}


float GameTimer::TotalSecondsf()
{
	return (float)(Diff().count() / 1000.0f);
}

double GameTimer::TotalSeconds()
{
	return (Diff().count()) / 1000.0;
}

int GameTimer::TotalSecondsi()
{
	return (int)((Diff().count()) / 1000.0f);
}

float GameTimer::TotalMilliSecondsf()
{
	// this is wrong, needs to use startTime
	return (float)(Diff().count());
}

double GameTimer::TotalMilliSeconds()
{
	return ((double)Diff().count());
}

int GameTimer::TotalMilliSecondsi()
{
	return (int)(Diff().count());
}

void GameTimer::SaveState()
{
    now.MillisecondsElapsed = MilliSecondsElapsed();
    now.MillisecondsElapsedf = MilliSecondsElapsedf();
    now.MillisecondsElapsedi = MilliSecondsElapsedi();

    now.SecondsElapsed = SecondsElapsed();
    now.SecondsElapsedf = SecondsElapsedf();
    now.SecondsElapsedi = SecondsElapsedi();

    now.TotalMilliseconds = TotalMilliSeconds();
    now.TotalMillisecondsf = TotalMilliSecondsf();
    now.TotalMillisecondsi = TotalMilliSecondsi();

    now.TotalSeconds = TotalSeconds();
    now.TotalSecondsf = TotalSecondsf();
    now.TotalSecondsi = TotalSecondsi();

    Update();
}

