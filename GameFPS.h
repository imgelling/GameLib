#pragma once
/*
GameFPS.h
GameFPS handles some frame calculations and can lock the frame rate
to any frame rate achieveable by the game code. It can calculate frames
per second (FPS) and milliseconds per frame (MsPF).
*/

// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameTimer.h"
#include <string>
#include <sstream>


// DEFINES /////////////////////////////////////////////////////////////////////
// DECLARATIONS ////////////////////////////////////////////////////////////////
class GameFPS
{
	//friend class Game;
    public:
        GameFPS();
        ~GameFPS();
        void Update();
        std::string ToString();
        int ToInt();
        std::string MsPFs();
        int MsPFi();
        double MsPF();
		void Reset();

    private:
		double time;
        double mspf;
        int fps;
        int frames;
        std::stringstream text;
        GameTimer timer;
        GameTimeState now;
};

class GameFPSLock
{
public:
	GameFPSLock();
	~GameFPSLock();
	void SetFrameLock(double);
	bool TimeToRender();
    void Reset();
private:

	double framelock;
    double timetowait = 0.0;
	GameTimer rendertimer;
};




