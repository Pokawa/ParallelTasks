//
// Created by hubert on 12/10/19.
//

#ifndef OK_GREEDYRESULTS_HPP
#define OK_GREEDYRESULTS_HPP

#include <vector>
#include <iostream>
#include <algorithm>
#include "task.hpp"

class result
{
protected:
    std::vector<taskFinishedProcessors> tasks;

public:
    explicit result(std::vector<taskFinishedProcessors> in) : tasks(std::move(in))
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
        return std::max_element(tasks.begin(), tasks.end(), [](const taskFinished & a, const taskFinished & b){ return a.endingTime < b.endingTime; })->endingTime;
    };
};

class resultSeparated : public result
{
    int separatorId;
    std::vector<taskFinishedProcessors> head;
    std::vector<taskFinishedProcessors> tail;
public:
    resultSeparated(std::vector<taskFinishedProcessors> in, const int & separataorId) : result(std::move(in)), separatorId(separataorId)
    {
        if (separatorId == 0)
            initDefault();
        else
            initHeadTail();

        sort();
    }

    std::vector<taskFinishedProcessors> getHead()
    {
        return head;
    }

    std::vector<taskFinishedProcessors> getTail()
    {
        return tail;
    }

private:
    void sort()
    {
        auto byStaritngTimeASC = [](const taskFinishedProcessors & a, const taskFinishedProcessors & b){ return (a.endingTime - a.runTime) < (b.endingTime - b.runTime); };
        std::stable_sort(head.begin(), head.end(), byStaritngTimeASC);
        std::stable_sort(tail.begin(), tail.end(), byStaritngTimeASC);
    }

    void initHeadTail()
    {
        auto isSeparator = [this](const taskFinishedProcessors & item){ return item.index == separatorId; };
        auto position = std::find_if(tasks.begin(), tasks.end(), isSeparator);
        head = std::vector<taskFinishedProcessors>(tasks.begin(), position + 1);
        tail = std::vector<taskFinishedProcessors>(position + 1, tasks.end());
    }

    void initDefault()
    {
        tail = tasks;
    }
};


#endif //OK_GREEDYRESULTS_HPP
