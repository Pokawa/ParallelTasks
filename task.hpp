//
// Created by hubert on 10/23/19.
//

#ifndef OK_TASK_HPP
#define OK_TASK_HPP

struct task
{
    int index;
    int arrivalTime;
    int runTime;
    int procUsage;

    bool isValid()
    {
        return index > 0 && arrivalTime >= 0 && runTime > 0 && procUsage > 0;
    }

    bool isFit(const int & freeProcs) const
    {
        return procUsage <= freeProcs;
    }

    bool isReady(const int & currentTime) const
    {
        return arrivalTime <= currentTime;
    }
};

struct taskWorking : public task
{
    int endingTime;
    std::vector<int> processors;

    explicit taskWorking(const task & val, const int & currentTime, std::vector<int> && processors) : task(val), endingTime(currentTime + runTime), processors(processors)
    {}

    bool isRunning(const int & currentTime) const
    {
        return endingTime > currentTime;
    }

    bool isFinished(const int & currentTime) const
    {
        return !isRunning(currentTime);
    }
};


#endif //OK_TASK_HPP
