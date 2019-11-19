#include <iostream>
#include "fileReader.hpp"
#include "instanceFactory.hpp"
#include <chrono>
#include "task.hpp"
#include "greedy.hpp"

int main(int argv, char *argc[]) {
//    auto p1 = std::chrono::steady_clock::now();

    int amount = argv > 2 ? std::stoi(argc[2]) : -1;
    instanceFactory factory{argc[1], amount};
    auto tasks = factory.generate();
    auto procs = factory.maxProcessors();

    auto res = greedy(tasks, procs);
    res.print();
//    auto p2 = std::chrono::steady_clock::now();
//    std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(p2 - p1).count();
    return 0;
}