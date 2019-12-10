#include <iostream>
#include <algorithm>
#include "fileReader.hpp"
#include "instanceFactory.hpp"
#include <chrono>
#include "task.hpp"
#include "greedy.hpp"
#include "config.hpp"
#include "tabuSearch.hpp"

int main(int argv, char *argc[]) {
    config::load();
    int amount = argv > 3 ? std::stoi(argc[3]) : -1;
    instanceFactory factory{argc[2], amount};
    auto tasks = factory.generate();
    auto procs = factory.maxProcessors();
    result out{std::vector<taskFinishedProcessors>()};

    std::string parameters = argc[1];
    if (parameters.find('h') != std::string::npos)
    {
        out = greedy(tasks, procs);
    }

    if (parameters.find('m') != std::string::npos)
    {
        auto res = greedySeparated(tasks, procs);
        auto searchSolution = tabuSearch(toSolution(res.getTail()));
        auto numerated = numerateProcessors(searchSolution);
        auto buff = res.getHead();
        buff.insert(buff.end(), numerated.begin(), numerated.end());
        out = result(buff);
    }

    if (parameters.find('p') != std::string::npos)
    {
        out.print();
    }

    if (parameters.find('l') != std::string::npos)
    {
        std::cout << out.length() << "\n";
    }

    if (parameters.find('t') != std::string::npos)
    {
        std::cout << config::getRunTimeSeconds() << "\n";
    }

    return 0;
}