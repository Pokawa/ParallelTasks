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
    for (auto p : parameters)
    {
        switch (p)
        {
            case 'h':
                config::resetClock();
                out = greedy(tasks, procs);
                break;
            case 'm':
                out = getTabuSearchResult(tasks, procs);
                break;
            case 'v':
                out = getTabuSearchResult(tasks, procs, true);
                break;
            case 'p':
                out.print();
                break;
            case 'l':
                std::cout << out.length() << "\n";
                break;
            case 't':
                std::cout << config::getRunTime() << "\n";
                break;
            case 'd':
                std::cout << getTheoreticalMinumumTime(tasks) << "\n";
                break;
            default:
                std::cout << "Nieznany parametr: " << p << "\n";
                break;
        }
    }
    return 0;
}