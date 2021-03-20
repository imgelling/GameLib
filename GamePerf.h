#pragma once
#include <chrono>
#include <string>
#include <unordered_map>
#include <shared_mutex>


class PerfTimer
{
private:
    // Holds relavent data per function timed
    struct FunctionToTime
    {
        std::string name;
        double time = 0.0;
        unsigned int ticks = 0;
        double cumulative = 0.0;
        std::chrono::high_resolution_clock::time_point start;
    };
    // Think I need this if using threads
    std::shared_mutex mute;
    // Holds a FunctionToTime keyed to a string specified by user
    std::unordered_map <std::string, FunctionToTime> timedfuncs;
public:
    ~PerfTimer()
    {
        timedfuncs.clear();
    }
    // Returns number of times the function was timed, -1 if not found
    unsigned int Ticks(std::string name) 
    { 
        mute.lock();
        unsigned int ret = 0;
        try
        {
            ret = timedfuncs.at(name).ticks;
        }
        catch (...)
        {
            ret = -1;
        }
        mute.unlock();
        return ret;
    }
    // Returns the nanoseconds the function took on the last call, -1 if not found
    double LastRun(std::string name) 
    { 
        mute.lock();
        double ret = 0.0;
        try
        {
            ret = timedfuncs.at(name).time;
        }
        catch (...)
        {
            ret = -1.0;
        }
        mute.unlock();
        return ret;
    }
    // Returns the total nanoseconds the function ran over all the calls, -1 if not found
    double Cumulative(std::string name)
    {
        mute.lock();
        double ret = 0.0;
        try
        {
            ret = timedfuncs.at(name).cumulative;
        }
        catch (...)
        {
            ret = -1.0;
        }
        mute.unlock();
    }
    // Returns the average nanoseconds the function ran over all the calls, -1 if not found
    double Average(std::string name)
    {
        mute.lock();
        double ret = 0.0;
        try
        {
            ret = timedfuncs.at(name).cumulative / timedfuncs.at(name).ticks;
        }
        catch (...)
        {
            ret = -1.0;
        }
        mute.unlock();
        return ret;
    }
    // Returns an unmodifiable container of timed functions
    const std::unordered_map <std::string, FunctionToTime>& TimedFunctions()
    {
        return timedfuncs;
    }
    // Start timing
    void Start(std::string name)
    {
        FunctionToTime t;
        mute.lock();
        timedfuncs.try_emplace(name, t);
        timedfuncs[name].name = name;
        timedfuncs[name].start = std::chrono::high_resolution_clock::now();
        timedfuncs[name].ticks++;
        mute.unlock();
    };
    // Stop timing
    void Stop(std::string name)
    {
#define duration(a) std::chrono::duration_cast<std::chrono::nanoseconds>(a).count()
#define timeNow() std::chrono::high_resolution_clock::now()
        mute.lock();
        try
        {
            timedfuncs.at(name).time = (double)(duration(timeNow() - timedfuncs.at(name).start));
            timedfuncs.at(name).cumulative += timedfuncs[name].time;
        }
        catch (...)
        {
            // Output to a log or something
            //std::cout << __func__ << ": \"" << name << "\" is not found." << "\n";
        }
        mute.unlock();
#undef duration
#undef timeNow
    };

};



