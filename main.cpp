#include <iostream>
#include <algorithm>
#include "fileReader.hpp"
#include "instanceFactory.hpp"
#include <chrono>
#include "task.hpp"
#include "greedy.hpp"

int main(int argv, char *argc[]) {
    auto p1 = std::chrono::steady_clock::now();

    int amount = argv > 3 ? std::stoi(argc[3]) : -1;
    instanceFactory factory{argc[2], amount};
    auto tasks = factory.generate();
    auto procs = factory.maxProcessors();
    auto res = greedy(tasks, procs);

    auto p2 = std::chrono::steady_clock::now();


    switch (argc[1][0])
    {
        case 'p':
            res.print();
            break;
        case 't':
            std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p2 - p1).count();
            break;
        case 'l':
            std::cout << res.length();
            break;
    }

    return 0;
}