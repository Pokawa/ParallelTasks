#include <iostream>
#include "fileReader.hpp"
#include "instanceFactory.hpp"
#include "task.hpp"
#include "greedy.hpp"

int main(int argv, char *argc[]) {
    int amount = argv > 2 ? std::stoi(argc[2]) : -1;
    instanceFactory factory{argc[1], amount};
    auto tasks = factory.generate();
    auto procs = factory.maxProcessors();

    auto res = greedy(tasks, procs);
    res.print();
    return 0;
}