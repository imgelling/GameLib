#pragma once
/*
GameTimer.h

GameTimer handles timing functions on the millisecond and second resolutions.
It counts time elapsed since the last state save and counts the total time
the program has been running.  Both ways have millisecond and second resolution
flavors.
*/

// INCLUDES ////////////////////////////////////////////////////////////////////
#include <chrono>
// DEFINES /////////////////////////////////////////////////////////////////////

// DECLARATIONS ////////////////////////////////////////////////////////////////

struct GameTimeState
{
    float MillisecondsElapsedf;
    double MillisecondsElapsed;
    int MillisecondsElapsedi;
    float SecondsElapsedf;
    double SecondsElapsed;
    int SecondsElapsedi;

    float TotalMillisecondsf;
    double TotalMilliseconds;
    int TotalMillisecondsi;
    float TotalSecondsf;
    double TotalSeconds;
    int TotalSecondsi;
};


class GameTimer
{
    public:
        GameTimer();

        void ResetTimer();
        GameTimeState Now();

    private:
		void SaveState();
		std::chrono::high_resolution_clock::time_point startTime;
		std::chrono::high_resolution_clock::time_point endFrame;
		intmax_t resolution;
		inline std::chrono::microseconds Diff(void);
        //double startTime;
        //double endFrame;
        GameTimeState now;

        void Update();

        float MilliSecondsElapsedf();
        double MilliSecondsElapsed();
        int MilliSecondsElapsedi();

        float SecondsElapsedf();
        double SecondsElapsed();
        int SecondsElapsedi();

        float TotalMilliSecondsf();
        double TotalMilliSeconds();
        int TotalMilliSecondsi();

        float TotalSecondsf();
        double TotalSeconds();
        int TotalSecondsi();
};
