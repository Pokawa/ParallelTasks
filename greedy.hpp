//
// Created by hubert on 11/19/19.
//

#ifndef OK_GREEDY_HPP
#define OK_GREEDY_HPP

#include "task.hpp"
#include <vector>
#include <iostream>
#include <algorithm>

class result
{
    std::vector<taskWorking> tasks;
    int finalLength;

public:
    result(std::vector<taskWorking> in) : tasks(std::move(in)), finalLength(tasks.back().endingTime)
    {}

    void print()
    {
        for (auto & a : tasks)
        {
            std::cout << a.index << " " << a.endingTime - a.runTime << " " << a.endingTime << " ";
            for (auto & p: a.processors)
                std::cout << p << " ";
            std::cout << "\n";
        }
    }

    int length()
    {
        return finalLength;
    };
};

result greedy(std::vector<task> tasks, int procs)
{
    std::vector<task> tasksReady;
    std::vector<taskWorking> tasksRunning;
    std::vector<taskWorking> tasksFinished;
    tasksFinished.reserve(tasksFinished.size());

    std::vector<bool> processors(procs, false);
    int freeProcs = procs;
    int currentTime = 0;

    auto isFinished = [&currentTime](const taskWorking & task){ return task.isFinished(currentTime); };
    auto isReady = [&currentTime](const task & item){ return item.isReady(currentTime); };
    auto fits = [&freeProcs](const task & task){ return task.isFit(freeProcs); };
    auto freeProcessors = [&freeProcs, &processors](const taskWorking &task) {
        freeProcs += task.processors.size();
        for (auto &proc : task.processors)
            processors[proc] = false;
    };
    auto byRuntimeDescThenProcUsageDesc = [](const task& a, const task& b){
        if (a.runTime == b.runTime)
            return a.procUsage > b.procUsage;
        else
            return a.runTime > b.runTime;
    };
    auto byEndingTimeAsc = [](const taskWorking & a, const taskWorking & b){ return a.endingTime < b.endingTime; };
    auto byArrivalTime = [](const task & a, const task & b){ return a.arrivalTime < b.arrivalTime; };


    std::sort(tasks.begin(), tasks.end(), byArrivalTime);

    while(!tasks.empty() or !tasksReady.empty())
    {
        currentTime = ((!tasksRunning.empty() and tasksRunning.front().endingTime < tasks.front().arrivalTime) or tasks.empty()) ? tasksRunning.front().endingTime : tasks.front().arrivalTime;


        auto iter = std::find_if_not(tasks.begin(), tasks.end(), isReady);
        tasksReady.insert(tasksReady.end(), tasks.begin(), iter);
        tasks.erase(tasks.begin(), iter);


        std::sort(tasksReady.begin(), tasksReady.end(), byRuntimeDescThenProcUsageDesc);


        auto iter2 = std::find_if_not(tasksRunning.begin(), tasksRunning.end(), isFinished);
        std::for_each(tasksRunning.begin(), iter2, freeProcessors);
        tasksFinished.insert(tasksFinished.end(), tasksRunning.begin(), iter2);
        tasksRunning.erase(tasksRunning.begin(), iter2);


        while (freeProcs > 0 and std::any_of(tasksReady.begin(), tasksReady.end(), fits))
        {

            auto taskPosition = std::find_if(tasksReady.begin(), tasksReady.end(), fits);

            std::vector<int> procList;
            auto procAmount = taskPosition->procUsage;
            procList.reserve(procAmount);
            freeProcs -= procAmount;

            auto begin = processors.begin();
            while (procAmount--) {
                begin = std::find(begin, processors.end(), false);
                procList.push_back(begin - processors.begin());
                processors[begin - processors.begin()] = true;
                ++begin;
            }

            tasksRunning.emplace_back(*taskPosition, currentTime, std::move(procList));
            tasksReady.erase(taskPosition);
        }


        std::sort(tasksRunning.begin(), tasksRunning.end(), byEndingTimeAsc);
    }


    std::for_each(tasksRunning.begin(), tasksRunning.end(), freeProcessors);
    tasksFinished.insert(tasksFinished.end(), tasksRunning.begin(), tasksRunning.end());
    tasksRunning.erase(tasksRunning.begin(), tasksRunning.end());

    return result(std::move(tasksFinished));
}



#endif //OK_GREEDY_HPP

