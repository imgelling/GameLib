#pragma once
/*
GameLog.h

GameLog handles logging for the game.  It outputs to a HTML formatted file
and the console window when it is open.  It can handle 3 types of logging.
Normal logging shows as normal text.  Warning logging shows as yellow
background text in the file and is prepended with "WARNING."  Error logging
shows up with a red background and is prepended with "ERROR."
*/


// INCLUDES ////////////////////////////////////////////////////////////////////
#include <fstream>
#include <boost/thread/mutex.hpp>



// DEFINES /////////////////////////////////////////////////////////////////////


// DECLARATIONS ////////////////////////////////////////////////////////////////
class Log {
  private:
    std::ofstream stream;
	char buffer[256];
	time_t rawtime;
	struct tm timeinfo;
    enum LogType {NORMAL, WARNING, ERRORS, NONE};
    void Write(LogType type, std::string logline);
	inline void GetTime();


  public:
    Log(std::string filename);
    ~Log();
    void Header(std::string name, std::string version);
    void Write(std::string logline);
    void Error(std::string logline);
    void Warning(std::string logline);
    void Line();
	boost::mutex streamlock;
};
