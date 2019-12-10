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

    [[nodiscard]] bool isFit(const int & freeProcs) const
    {
        return procUsage <= freeProcs;
    }

    [[nodiscard]] bool isReady(const int & currentTime) const
    {
        return arrivalTime <= currentTime;
    }
};

struct taskFinished : public task
{
    int endingTime;

    explicit taskFinished(const task & val, const int & currentTime) : task(val), endingTime(currentTime + runTime)
    {}


    [[nodiscard]] bool isRunning(const int & currentTime) const
    {
        return endingTime > currentTime;
    }

    [[nodiscard]] bool isFinished(const int & currentTime) const
    {
        return !isRunning(currentTime);
    }
};

struct taskFinishedProcessors : public taskFinished
{
    std::vector<int> processors;

    explicit taskFinishedProcessors(const task & val, const int & currentTime, std::vector<int> && processors) : taskFinished(val, currentTime), processors(processors)
    {}
};


#endif //OK_TASK_HPP
