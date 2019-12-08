//
// Created by hubert on 12/8/19.
//

#ifndef OK_CONFIG_HPP
#define OK_CONFIG_HPP
#include <fstream>


namespace config
{
    int failsLimit;
    int tabuListSize;
    int neighboursCount;
    int changesCount;

    void load()
    {
        std::fstream file{"config", std::ios::in};
        file >> failsLimit;
        file.ignore(256, '\n');
        file >> tabuListSize;
        file.ignore(256, '\n');
        file >> neighboursCount;
        file.ignore(256, '\n');
        file >> changesCount;
        file.close();
    }
}

#endif //OK_CONFIG_HPP
