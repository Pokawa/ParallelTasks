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
protected:
    std::vector<taskWorking> tasks;
    int finalLength;

public:
    explicit result(std::vector<taskWorking> in) : tasks(std::move(in)), finalLength(tasks.back().endingTime)
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

class resultSeparated : public result
{
    int separatorId;
    std::vector<taskWorking> head;
    std::vector<taskWorking> tail;
public:
    resultSeparated(std::vector<taskWorking> in, const int & separataorId) : result(std::move(in)), separatorId(separataorId)
    {
        if (separatorId == 0)
            initDefault();
        else
            initHeadTail();

        sort();
    }

    std::vector<taskWorking> getHead()
    {
        return head;
    }

    std::vector<taskWorking> getTail()
    {
        return tail;
    }

private:
    void sort()
    {
        auto byStaritngTimeASC = [](const taskWorking & a, const taskWorking & b){ return (a.endingTime - a.runTime) < (b.endingTime - b.runTime); };
        std::sort(head.begin(), head.end(), byStaritngTimeASC);
        std::sort(tail.begin(), tail.end(), byStaritngTimeASC);
    }

    void initHeadTail()
    {
        auto isSeparator = [this](const taskWorking & item){ return item.index == separatorId; };
        auto position = std::find_if(tasks.begin(), tasks.end(), isSeparator);
        head = std::vector<taskWorking>(tasks.begin(), position+1);
        tail = std::vector<taskWorking>(position+1, tasks.end());
    }

    void initDefault()
    {
        tail = tasks;
    }
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

resultSeparated greedySeparated(std::vector<task> tasks, int procs)
{
    std::vector<task> tasksReady;
    std::vector<taskWorking> tasksRunning;
    std::vector<taskWorking> tasksFinished;
    tasksFinished.reserve(tasksFinished.size());
    int separatorId = 0;

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
    auto arrivedNow = [&currentTime](const task & item){ return item.arrivalTime >= currentTime; };


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

        if (std::all_of(tasksRunning.begin(), tasksRunning.end(), arrivedNow) and std::all_of(tasksReady.begin(), tasksReady.end(), arrivedNow) and !tasksFinished.empty())
            separatorId = tasksFinished.back().index;
    }


    std::for_each(tasksRunning.begin(), tasksRunning.end(), freeProcessors);
    tasksFinished.insert(tasksFinished.end(), tasksRunning.begin(), tasksRunning.end());
    tasksRunning.erase(tasksRunning.begin(), tasksRunning.end());

    return resultSeparated(std::move(tasksFinished), separatorId);
}

#endif //OK_GREEDY_HPP

