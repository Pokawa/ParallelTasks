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
    static int changesCount;

    static std::chrono::time_point<std::chrono::system_clock> startTime = std::chrono::system_clock::now();

    void load()
    {
        std::fstream file{"config", std::ios::in};
        file >> failsLimit;
        file.ignore(256, '\n');
        file >> tabuListSizeLimit;
        file.ignore(256, '\n');
        file >> neighboursCount;
        file.ignore(256, '\n');
        file >> changesCount;
        file.close();
    }

    int getRunTimeSeconds()
    {
        auto currentTime = std::chrono::system_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::seconds>(currentTime - startTime);
        return duration.count();
    }

    bool properRunTime()
    {
        return (getRunTimeSeconds() >= 60 + 60 + 60 + 60 + 50);
    }
}

#endif //OK_CONFIG_HPP
