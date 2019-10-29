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
};


#endif //OK_TASK_HPP
