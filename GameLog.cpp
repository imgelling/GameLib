// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameLog.h"
#include <iostream>



// METHODS /////////////////////////////////////////////////////////////////////

void Log::Line()
{
	//#ifdef _DEBUG
	streamlock.lock();
	std::cout << "--------------------------------------------------------------------------------" << std::endl;
	streamlock.unlock();
	//#endif
	streamlock.lock();
	stream << "<HR>";
	streamlock.unlock();
}

Log::Log(std::string filename)
{
	streamlock.lock();
	stream.open(filename.c_str(), std::ios::out);
	streamlock.unlock();
}

void Log::Write(std::string logline)
{
	Write(NORMAL, logline);
}

void Log::Error(std::string logline)
{
	Write(ERRORS, logline);
}

void Log::Warning(std::string logline)
{
	Write(WARNING, logline);
}

void Log::Write(LogType type, std::string logline)
{
	std::string color;
	std::string temp;
	std::string pre;
	std::string time;

	GetTime();
	time = std::string(buffer);
	time.pop_back();
	pre = "[" + time + "] ";
	color = "FFFFFF";

	if (type == ERRORS)
	{
		pre += "ERROR : ";
		color = "FF0000";
	}
	else if (type == WARNING)
	{
		pre += "WARNING : ";
		color = "FFFF00";
	}
	//#ifdef _DEBUG
	streamlock.lock();
	std::cout << pre << logline << std::endl;
	streamlock.unlock();
	//#endif
	temp += "<FONT style=\"background-color:#" + color + "\">";
	temp += pre;
	temp += logline + "</FONT><BR>";
	streamlock.lock();
	stream << temp << std::endl;
	streamlock.unlock();
}

inline void Log::GetTime()
{
#ifdef __linux__

#else
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	asctime_s(buffer, &timeinfo);
#endif
}

void Log::Header(std::string name, std::string version)
{
	Write(name + ", version " + version);
	Write("Built on " + std::string(__DATE__) + " at " + std::string(__TIME__) + ".");
	Line();
}

Log::~Log()
{
	streamlock.lock();
	stream.close();
	streamlock.unlock();
}
