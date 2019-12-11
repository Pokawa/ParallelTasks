//
// Created by hubert on 10/25/19.
//

#ifndef OK_INSTANCEFACTORY_HPP
#define OK_INSTANCEFACTORY_HPP

#include <string>
#include <vector>
#include <sstream>
#include <algorithm>
#include "fileReader.hpp"
#include "task.hpp"
#include "fileCommentReader.hpp"
#include "config.hpp"

class instanceFactory {
private:
    std::string line;
    task parsedTask;
    std::vector<task> readyTasks;
    fileReader dataFeed;
    fileCommentReader commentReader;
    int numberOfTasksToRead;

public:
    explicit instanceFactory(const std::string &fileName, const int &maxTasks = -1) : dataFeed(fileName), numberOfTasksToRead(maxTasks), commentReader(fileName){
        parsedTask = {};
    }

    std::vector<task> generate()
    {
        collectTasks();
        return readyTasks;
    }

    int maxProcessors()
    {
        std::string value = commentReader.getValue("MaxProcs:");
        config::processors = std::stoi(value);
        return std::stoi(value);
    }

private:

    void collectTasks()
    {
        initializeFile();
        while(isNext())
        {
            getLine();
            parseLine();
            pushToReadyIfValid();
        }
    }

    void initializeFile()
    {
        dataFeed.open();
    }

    bool isNext()
    {
        return dataFeed.isNotEOF() && isMoreTasksOrNotDefined();
    }

    bool isMoreTasksOrNotDefined()
    {
        return numberOfTasksToRead != 0;
    }

    void getLine()
    {
        line = dataFeed.getLine();
    }

    void parseLine()
    {
        std::istringstream stream{line};
        int index = 0, arrivalTime = 0, dummySeparator = 0, runTime = 0, procUsage = 0;
        stream >> index >> arrivalTime >> dummySeparator >> runTime >> procUsage;
        parsedTask = {index, arrivalTime, runTime, procUsage};
    }

    void pushToReadyIfValid()
    {
        if (parsedTask.isValid())
        {
            readyTasks.push_back(parsedTask);
            numberOfTasksToRead--;
        }
    }
};

template<class T>
int getTheoreticalMinumumTime(const T & tasks)
{
    long long sum = 0;
    std::for_each(tasks.begin(), tasks.end(), [&sum](const task & item){ sum += item.runTime; });
    auto lastTask = std::max_element(tasks.begin(), tasks.end(), [](const task & a, const task & b){ return a.arrivalTime + a.runTime < b.arrivalTime + b.runTime; });
    return std::max(lastTask->arrivalTime + lastTask->runTime, sum / config::processors);
}


#endif //OK_INSTANCEFACTORY_HPP
