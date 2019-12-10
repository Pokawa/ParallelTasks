//
// Created by hubert on 12/8/19.
//

#ifndef OK_CONFIG_HPP
#define OK_CONFIG_HPP
#include <fstream>
#include <chrono>


namespace config
{
    static int failsLimit;
    static int tabuListSizeLimit;
    static int neighboursCount;
    static int processors;
    static bool verbose = false;
    static int secondsTimeLimit = 60 + 60 + 60 + 60 + 50;
    static int mikrosecondsTimeLimit = secondsTimeLimit * 1000;

    static std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

    void load()
    {
        std::fstream file{"config", std::ios::in};
        file >> failsLimit;
        file.ignore(256, '\n');
        file >> tabuListSizeLimit;
        file.ignore(256, '\n');
        file >> neighboursCount;
        file.close();
    }

    void resetClock()
    {
        startTime = std::chrono::system_clock::now();
    }

    int getRunTime()
    {
        auto currentTime = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - startTime);
        return duration.count();
    }

    bool properRunTime()
    {
        return (getRunTime() <= mikrosecondsTimeLimit);
    }
}

#endif //OK_CONFIG_HPP
