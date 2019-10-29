//
// Created by hubert on 10/25/19.
//

#ifndef OK_INSTANCEFACTORY_HPP
#define OK_INSTANCEFACTORY_HPP

#include <string>
#include <vector>
#include <sstream>
#include "fileReader.hpp"
#include "task.hpp"
#include "fileCommentReader.hpp"

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
        reserveBuffer();
    }

    std::vector<task> generate()
    {
        collectTasks();
        return readyTasks;
    }

    int maxProcessors()
    {
        std::string value = commentReader.getValue("MaxProcs:");
        return std::stoi(value);
    }

private:

    void reserveBuffer()
    {
        if (numberOfTasksToRead > 0)
            readyTasks.reserve(numberOfTasksToRead);
    }

    void collectTasks()
    {
        while(isNext())
        {
            getLine();
            parseLine();
            pushToReadyIfValid();
        }
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
        int index, arrivalTime, dummySeparator, runTime, procUsage;
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


#endif //OK_INSTANCEFACTORY_HPP
