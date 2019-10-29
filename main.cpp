#include <iostream>
#include "fileReader.hpp"
#include "instanceFactory.hpp"

int main() {
    instanceFactory factory{"das.swf", 100000};
    auto tasks = factory.generate();
    auto procs = factory.maxProcessors();
    auto size = tasks.size();
    return 0;
}