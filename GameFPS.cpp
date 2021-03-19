// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameFPS.h"



// METHODS /////////////////////////////////////////////////////////////////////
GameFPS::GameFPS()
{

	Reset();
}

GameFPS::~GameFPS()
{
    
}

void GameFPS::Reset()
{
	frames = 0;
	fps = 0;
	mspf = 0;
	time = 0;
	text.str("");
}

std::string GameFPS::ToString()
{
    text << fps;
    std::string str = text.str();
    text.str("");
    return str;
}

int GameFPS::ToInt()
{
    return fps;
}

double GameFPS::MsPF()
{
	return mspf;
}

int GameFPS::MsPFi()
{
    return (int) mspf;
}

std::string GameFPS::MsPFs()
{
    text << (int)mspf;
    std::string str = text.str();
    text.str("");
    return str;
}

void GameFPS::Update()
{
    now = timer.Now();
	
    frames++;
    time += now.MillisecondsElapsed;
    if (time >= 1000.0)// - 0.5)
    {
		if (fps != 0) mspf = 1000.0f / (float)fps;
		time = time - (int)time;// 0.0;
        fps = frames;
        frames = 0;
    }

}


///////////////////////////

GameFPSLock::GameFPSLock()
{
	framelock = 0;
}

GameFPSLock::~GameFPSLock()
{
}

void GameFPSLock::Reset()
{
	rendertimer.ResetTimer();
}


void GameFPSLock::SetFrameLock(double fl)
{
	framelock = fl;
	Reset();
}

bool GameFPSLock::TimeToRender()
{
	if (framelock == 0.0)
		return true;
	double renderPerSecond = (1000.0 / framelock);
	double now = rendertimer.Now().MillisecondsElapsed;
	rendertimer.ResetTimer();
	
	timetowait += now;
	if (timetowait >= (renderPerSecond))
	{
		timetowait -= (renderPerSecond);
		return true;
	}
	else 
	{
			return false;
	}
}

