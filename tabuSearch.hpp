//
// Created by hubert on 12/8/19.
//

#ifndef OK_TABUSEARCH_HPP
#define OK_TABUSEARCH_HPP

#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include "task.hpp"
#include "config.hpp"

std::mt19937 gen{std::random_device{}()};
template<typename T>
T random(T min, T max) {
    return std::uniform_int_distribution<T>{min, max}(gen);
}

typedef std::vector<taskFinished> solution;
typedef std::pair<solution, int> neighbour;


std::vector<taskFinishedProcessors> numerateProcessors(const solution & value)
{
    std::vector<taskFinishedProcessors> out;
    std::vector<taskFinishedProcessors> tasksRunning;
    int freeProcs = config::processors;
    int currentTime = 0, destinationTime = 0;
    auto tasksIter = value.begin();
    std::vector<bool> processors(config::processors, false);

    auto freeProcessors = [&freeProcs, &processors](const taskFinishedProcessors & task){
        freeProcs += task.procUsage;
        for (auto &proc : task.processors)
            processors[proc] = false;
    };
    auto isFinished = [&currentTime](const taskFinished & task){ return task.isFinished(currentTime); };
    auto byEndingTimeAsc = [](const taskFinished & a, const taskFinished & b){ return a.endingTime < b.endingTime; };

    while(tasksIter != value.end())
    {
        if (!tasksRunning.empty()) {
            if (tasksRunning.front().endingTime < tasksIter->arrivalTime)
                destinationTime = tasksRunning.front().endingTime;
            else
            if (tasksIter->isFit(freeProcs))
                destinationTime = tasksIter->arrivalTime;
            else
                destinationTime = tasksRunning.front().endingTime;
        } else
            destinationTime = tasksIter->arrivalTime;
        currentTime = std::max(destinationTime, currentTime);


        auto iter = std::find_if_not(tasksRunning.begin(), tasksRunning.end(), isFinished);
        std::for_each(tasksRunning.begin(), iter, freeProcessors);
        tasksRunning.erase(tasksRunning.begin(), iter);

        while (tasksIter->isFit(freeProcs) and tasksIter->isReady(currentTime) and tasksIter != value.end())
        {
            std::vector<int> procList;
            auto procAmount = tasksIter->procUsage;
            freeProcs -= procAmount;
            auto begin = processors.begin();
            while (procAmount--) {
                begin = std::find(begin, processors.end(), false);
                procList.push_back(begin - processors.begin());
                processors[begin - processors.begin()] = true;
                ++begin;
            }
            auto procList2 = procList;

            tasksRunning.emplace_back(*tasksIter, currentTime, std::move(procList));
            out.emplace_back(*tasksIter, currentTime, std::move(procList2));
            tasksIter++;
        }

        std::sort(tasksRunning.begin(), tasksRunning.end(), byEndingTimeAsc);
    }

    return out;
}

std::pair<int, int> findFreeSpace(const taskFinished & removedTask, const std::vector<std::pair<int,int>> & quant)
{
    auto removedFits = [&removedTask](const std::pair<int, int> & item){ return removedTask.isFit(item.second); };
    auto removedReady = [&removedTask](const std::pair<int, int> & item){ return removedTask.isReady(item.first); };

    auto firstArrivalPosition = std::find_if(quant.begin(), quant.end(), removedReady);
    auto beginPosition = firstArrivalPosition, endPosition = firstArrivalPosition;
    int timeSpan;
    while(beginPosition != quant.end()){

        beginPosition = std::find_if(beginPosition, quant.end(), removedFits);
        endPosition = std::find_if_not(beginPosition, quant.end(), removedFits);

        if (endPosition == quant.end() and beginPosition != endPosition)
            break;

        timeSpan = endPosition->first - beginPosition->first;

        if (timeSpan >= removedTask.runTime)
            break;

        beginPosition = endPosition;
    }

    if (beginPosition == quant.end())
        return quant.back();

    return *beginPosition;
}

solution evaluateNeighbour(solution value, taskFinished removedTask)
{
    std::vector<taskFinished> tasksRunning;
    std::vector<std::pair<int, int>> quant;
    int freeProcs = config::processors;
    int currentTime = 0, destinationTime = 0;
    auto tasksIter = value.begin();

    auto freeProcessors = [&freeProcs](const taskFinished & task){ freeProcs += task.procUsage; };
    auto isFinished = [&currentTime](const taskFinished & task){ return task.isFinished(currentTime); };
    auto byEndingTimeAsc = [](const taskFinished & a, const taskFinished & b){ return a.endingTime < b.endingTime; };
    auto byStaritngTimeASC = [](const taskFinished & a, const taskFinished & b){ return (a.endingTime - a.runTime) < (b.endingTime - b.runTime); };

    while(tasksIter != value.end())
    {
        if (!tasksRunning.empty()) {
            if (tasksRunning.front().endingTime < tasksIter->arrivalTime)
                destinationTime = tasksRunning.front().endingTime;
            else
                if (tasksIter->isFit(freeProcs))
                    destinationTime = tasksIter->arrivalTime;
                else
                    destinationTime = tasksRunning.front().endingTime;
        } else
            destinationTime = tasksIter->arrivalTime;

        currentTime = std::max(destinationTime, currentTime);

        auto iter = std::find_if_not(tasksRunning.begin(), tasksRunning.end(), isFinished);
        std::for_each(tasksRunning.begin(), iter, freeProcessors);
        tasksRunning.erase(tasksRunning.begin(), iter);

        while (tasksIter->isFit(freeProcs) and tasksIter->isReady(currentTime) and tasksIter != value.end())
        {
            tasksIter->endingTime = currentTime + tasksIter->runTime;
            tasksRunning.push_back(*tasksIter);
            freeProcs -= tasksIter->procUsage;
            tasksIter++;
        }

        std::sort(tasksRunning.begin(), tasksRunning.end(), byEndingTimeAsc);
        quant.emplace_back(currentTime, freeProcs);
    }

    while(!tasksRunning.empty())
    {
        currentTime = tasksRunning.front().endingTime;
        auto iter = std::find_if_not(tasksRunning.begin(), tasksRunning.end(), isFinished);
        std::for_each(tasksRunning.begin(), iter, freeProcessors);
        tasksRunning.erase(tasksRunning.begin(), iter);
        quant.emplace_back(currentTime, freeProcs);
    }

    auto freeSpace = findFreeSpace(removedTask, quant);
    removedTask.endingTime = freeSpace.first + removedTask.runTime;
    value.push_back(removedTask);
    std::stable_sort(value.begin(), value.end(), byStaritngTimeASC);

    return value;
}

std::vector<neighbour> getNeighbours(const std::list<int> &tabuList, const solution &parentSolution) {
    auto contains = [](const auto & container, const auto & value){return std::find(std::begin(container), std::end(container), value) != std::end(container); };

    std::vector<neighbour> out;
    std::vector<int> usedIndexes;
    for (int i = 0; i < config::neighboursCount; ++i)
    {
        int a;
        do
        {
            a = random(0, (int)parentSolution.size() - 1);
        } while (contains(usedIndexes, a) and contains(tabuList, parentSolution[a].index));

        usedIndexes.push_back(a);
        auto task = parentSolution[a];
        auto copy = parentSolution;
        copy.erase(copy.begin() + a);
        copy = evaluateNeighbour(std::move(copy), task);
        out.emplace_back(copy, task.index);
    }
    return out;
}

int getLength(const solution &item) {
    return std::max_element(item.begin(), item.end(), [](const taskFinished & a, const taskFinished & b){ return a.endingTime < b.endingTime; })->endingTime;
}

solution tabuSearch(const solution &solution) {
    if (solution.size() == 1)
        return solution;

    auto byLength = [](const auto &item1, const auto &item2) { return getLength(item1.first) < getLength(item2.first); };

    auto best = solution;
    auto candidate = std::make_pair(solution, 0);
    auto failsCount = 0;
    std::list<int> tabuList;

    while (failsCount < config::failsLimit and config::properRunTime()) {
        auto neighbours = getNeighbours(tabuList, best);
        candidate = *std::min_element(neighbours.begin(), neighbours.end(), byLength);

        if (getLength(candidate.first) < getLength(best)) {
            best = candidate.first;
            tabuList.push_back(candidate.second);
        } else
            failsCount++;

        if (tabuList.size() > config::tabuListSizeLimit)
            tabuList.pop_front();
    }

    return best;
}

solution toSolution(const std::vector<taskFinishedProcessors> & item)
{
    return solution{item.begin(), item.end()};
}

#endif //OK_TABUSEARCH_HPP
