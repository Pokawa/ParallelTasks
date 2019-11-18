//
// Created by hubert on 11/18/19.
//

#ifndef OK_GREEDY_HPP
#define OK_GREEDY_HPP


#include <iostream>
#include "instanceFactory.hpp"

class greedy {
    std::vector<task> tasks;
    std::vector<task> tasksReady;
    std::vector<taskWorking> tasksRunning;
    std::vector<taskWorking> tasksFinished;
    std::vector<bool> processors;
    int freeProcs;
    int currentTime;
    int finalLength;

public:
    greedy(std::vector<task> instance, const int & procs) : tasks(std::move(instance)), processors(tasks.size(), false), freeProcs(procs), currentTime(0), finalLength(0)
    {
        tasksFinished.reserve(tasks.size());
    }

    std::vector<taskWorking> getResult()
    {
        return tasksFinished;
    }

    int getFinalLength()
    {
        return finalLength;
    }

    void run()
    {
        while (isTasksWaiting()) {
            findNextTimeValue();
            transferArrivedToReady();
            sortReadyTasksDesc();
            transferRunningToFinished();
            transferReadyToFinished();
            sortRunningTasksAsc();
        }
        transferRemainingRunningTasksToFinished();
        calculateFinalLength();
    }

    void print() {
        for (auto &item : tasksFinished) {
            std::cout << item.index << " " << item.endingTime - item.runTime << " " << item.endingTime << " ";
            if (item.processors.empty())
                throw;
            for (auto & proc : item.processors)
                std::cout << proc << " ";
            std::cout << "\n";
        }
    }


private:
    bool isTasksWaiting() const { return !tasks.empty() or !tasksReady.empty(); }

    void findNextTimeValue() {
        currentTime = endingTimeSooner() ? tasksRunning.front().endingTime : tasks.front().arrivalTime;
    }

    bool endingTimeSooner()
    {
        return !tasksRunning.empty() and tasksRunning.front().endingTime < tasks.front().arrivalTime;
    }

    void transferArrivedToReady() {
        auto arrived = [&](const task & task){ return task.arrivalTime <= currentTime; };

        auto iter = std::find_if_not(tasks.begin(), tasks.end(), arrived);
        tasksReady.insert(tasksReady.end(), tasks.begin(), iter);
        tasks.erase(tasks.begin(), iter);
    }

    void sortReadyTasksDesc() {
        auto runtimeDescProcUsageDesc = [](const task &a, const task &b) {
            if (a.runTime == b.runTime) {
                return a.procUsage > b.procUsage;
            } else {
                return a.runTime > b.runTime;
            }
        };

        std::sort(tasksReady.begin(), tasksReady.end(), runtimeDescProcUsageDesc);
    }

    void transferRemainingRunningTasksToFinished() {
        auto clearProcessors = [&](const taskWorking &task) {
            freeProcs += task.processors.size();
            for (auto &proc : task.processors)
                processors[proc] = false;
        };

        std::for_each(tasksRunning.begin(), tasksRunning.end(), clearProcessors);

        tasksFinished.insert(tasksFinished.end(), tasksRunning.begin(), tasksRunning.end());
        tasksRunning.erase(tasksRunning.begin(), tasksRunning.end());
    }

    void sortRunningTasksAsc(){
        auto endingTimeAsc = [](const taskWorking &a, const taskWorking &b) { return a.endingTime < b.endingTime; };

        std::sort(tasksRunning.begin(), tasksRunning.end(), endingTimeAsc);
    }

    void transferReadyToFinished() {
        while (isProcessorsFreeAndAnyTaskWillFit()) {

            auto taskPosition = std::find_if(tasksReady.begin(), tasksReady.end(),
                                             [&](const task &task) { return task.isFit(freeProcs); });
            std::vector<int> procList;
            auto procAmount = taskPosition->procUsage;
            procList.reserve(procAmount);
            freeProcs -= procAmount;

            auto begin = processors.begin();
            while (procAmount--) {
                begin = std::find(begin, processors.end(), 0);
                procList.push_back(begin - processors.begin());
                processors[begin - processors.begin()] = true;
                ++begin;
            }

            tasksRunning.emplace_back(*taskPosition, currentTime, std::move(procList));
            tasksReady.erase(taskPosition);
        }
    }

    bool isProcessorsFreeAndAnyTaskWillFit() const {
        auto willFit = [&](const task &task) { return task.isFit(freeProcs); };

        return freeProcs > 0 and std::any_of(tasksReady.begin(), tasksReady.end(), willFit);
    }

    void transferRunningToFinished() {
        auto isFinished = [&](const taskWorking & task){ return task.isFinished(currentTime); };
        auto clearProcessors = [&](const taskWorking &task) {
            freeProcs += task.processors.size();
            for (auto &proc : task.processors)
                processors[proc] = false;
        };

        auto iter = std::find_if_not(tasksRunning.begin(), tasksRunning.end(), isFinished);

        std::for_each(tasksRunning.begin(), iter, clearProcessors);

        tasksFinished.insert(tasksFinished.end(), tasksRunning.begin(), iter);
        tasksRunning.erase(tasksRunning.begin(), iter);
    }

    void calculateFinalLength() {
        finalLength = tasksFinished.back().endingTime;
    }


};


#endif //OK_GREEDY_HPP
