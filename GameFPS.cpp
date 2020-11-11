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
}

GameFPSLock::~GameFPSLock()
{
}


void GameFPSLock::SetFrameLock(double fl)
{
	framelock = fl;
}

bool GameFPSLock::TimeToRender()
{
	if (framelock == 0.0)
		return true;
	double renderPerSecond = 1000.0 / framelock;// 16.666;// 1000.0 / 60.0;// +0.0025;
	static double timetowait = 0;//1.0f/60.0f;
	timetowait += rendertimer.Now().MillisecondsElapsed;
	rendertimer.ResetTimer();
	if (timetowait >= (renderPerSecond))// -0.00025))
	{
		
		timetowait = timetowait - (renderPerSecond);  

		return true;
	}
	else 
	{
			return false;
	}
}

