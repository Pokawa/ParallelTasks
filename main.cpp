#include <iostream>
#include "fileReader.hpp"
#include "instanceFactory.hpp"

int main() {
    instanceFactory factory{"das.swf", 100};
    auto tasks = factory.generate();
    auto procs = factory.maxProcessors();
    auto size = tasks.size();

    std::vector<task> tasksReady;
    std::vector<taskWorking> tasksRunning;
    std::vector<taskWorking> tasksFinished;
    tasksFinished.reserve(size);

    std::vector<bool> processors(procs + 1, false);
    int freeProcs = procs;
    int currentTime = 0;

    while(!tasks.empty() or !tasksReady.empty())
    {
        currentTime = !tasksRunning.empty() and tasksRunning.front().endingTime < tasks.front().arrivalTime ? tasksRunning.front().endingTime : tasks.front().arrivalTime;

        auto iter = tasks.begin();
        while (iter != tasks.end() and iter->arrivalTime <= currentTime)
            ++iter;
        tasksReady.insert(tasksReady.end(), tasks.begin(), iter);
        tasks.erase(tasks.begin(), iter);

        std::sort(tasksReady.begin(), tasksReady.end(), [](const task& a, const task& b){
            if (a.runTime == b.runTime)
            {
                return a.procUsage > b.procUsage;
            }
            else
            {
                return a.runTime > b.runTime;
            }
        });


        auto iter2 = tasksRunning.begin();
        while (iter2 != tasksRunning.end() and iter2->isFinished(currentTime))
            ++iter2;

        std::for_each(tasksRunning.begin(), iter2, [&freeProcs, &processors](const taskWorking &task) {
            freeProcs += task.processors.size();
            for (auto &proc : task.processors)
                processors[proc] = false;
        });

        tasksFinished.insert(tasksFinished.end(), tasksRunning.begin(), iter2);
        tasksRunning.erase(tasksRunning.begin(), iter2);


        while (freeProcs > 0 and std::any_of(tasksReady.begin(), tasksReady.end(), [&freeProcs](const task & task){ return task.isFit(freeProcs); }))
        {

            auto taskPosition = std::find_if(tasksReady.begin(), tasksReady.end(),
                                             [&freeProcs](const task &task) { return task.isFit(freeProcs); });
            std::vector<int> procList;
            auto procAmount = taskPosition->procUsage;
            procList.reserve(procAmount);
            freeProcs -= procAmount;

            auto begin = processors.begin();
            while (procAmount--) {
                begin = std::find_if(begin, processors.end(), [](bool a) { return !a; });
                procList.push_back(begin - processors.begin());
                processors[begin - processors.begin()] = true;
                ++begin;
            }

            tasksRunning.emplace_back(*taskPosition, currentTime, std::move(procList));
            tasksReady.erase(taskPosition);
        }

        std::sort(tasksRunning.begin(), tasksRunning.end(), [](const taskWorking & a, const taskWorking & b){ return a.endingTime < b.endingTime; });
    }

    std::for_each(tasksRunning.begin(), tasksRunning.end(), [&freeProcs, &processors](const taskWorking &task) {
        freeProcs += task.processors.size();
        for (auto &proc : task.processors)
            processors[proc] = false;
    });

    tasksFinished.insert(tasksFinished.end(), tasksRunning.begin(), tasksRunning.end());
    tasksRunning.erase(tasksRunning.begin(), tasksRunning.end());

    auto finalLength = tasksFinished.back().endingTime;
    return 0;
}