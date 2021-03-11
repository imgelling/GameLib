// INCLUDES ////////////////////////////////////////////////////////////////////
#include "GameLog.h"
#include <iostream>
#include <string>



// METHODS /////////////////////////////////////////////////////////////////////

void GameLog::Line()
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

GameLog::GameLog(std::string filename)
{
	streamlock.lock();
	stream.open(filename.c_str(), std::ios::out);
	streamlock.unlock();
}

void GameLog::Write(std::string logline)
{
	Write(LogType::NORMAL, logline);
}

void GameLog::Error(std::string logline)
{
	Write(LogType::ERRORS, logline);
}

void GameLog::Warning(std::string logline)
{
	Write(LogType::WARNING, logline);
}

void GameLog::Write(LogType type, std::string logline)
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

	if (type == LogType::ERRORS)
	{
		pre += "ERROR : ";
		color = "FF0000";
	}
	else if (type == LogType::WARNING)
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

inline void GameLog::GetTime()
{
#ifdef __linux__

#else
	time(&rawtime);
	localtime_s(&timeinfo, &rawtime);
	asctime_s(buffer, &timeinfo);
#endif
}

void GameLog::Header(std::string name, std::string version)
{
	Write(name + ", version " + version);
	Write("Built on " + std::string(__DATE__) + " at " + std::string(__TIME__) + ".");
	Line();
}

GameLog::~GameLog()
{
	streamlock.lock();
	stream.close();
	streamlock.unlock();
}
