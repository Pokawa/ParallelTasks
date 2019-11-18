#include <iostream>
#include "fileReader.hpp"
#include "instanceFactory.hpp"
#include "greedy.hpp"

int main(int argc, char* argv[]) {
    int amount = argc > 2 ? std::stoi(argv[2]) : -1;
    instanceFactory factory{argv[1], amount};

    auto tasks = factory.generate();
    auto procs = factory.maxProcessors();

    greedy algorithm{tasks, procs};
    algorithm.run();
    algorithm.print();
    std::cout << algorithm.getFinalLength();
    return 0;
}
