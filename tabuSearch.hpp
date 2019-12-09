//
// Created by hubert on 12/8/19.
//

#ifndef OK_TABUSEARCH_HPP
#define OK_TABUSEARCH_HPP

#include <vector>
#include <list>
#include <algorithm>
#include <random>
#include <set>
#include "task.hpp"
#include "config.hpp"

std::mt19937 gen{std::random_device{}()};
template<typename T>
T random(T min, T max) {
    return std::uniform_int_distribution<T>{min, max}(gen);
}


typedef std::vector<task> solution;
typedef std::pair<solution, int> neighbour;

std::vector<neighbour> getNeighbours(const std::list<int> &tabuList, const solution &parentSolution) {
    auto contains = [](const auto & container, const auto & value){return std::find(std::begin(container), std::end(container), value) == std::end(container); };

    std::vector<neighbour> out;
    std::vector<int> usedIndexes;
    for (int i = 0; i < config::neighboursCount; ++i)
    {
        int a;
        do
        {
            a = random(0, (int)parentSolution.size());
        } while (contains(usedIndexes, a) and contains(tabuList, a));

        usedIndexes.push_back(a);
//        auto task = parentSolution[a];
        auto copy = parentSolution;
        copy.erase(copy.begin() + a);
        //TODO wciśnij gdzieś to
        out.emplace_back(copy, a);
    }
    return out;
}

int getLength(const solution &item) {
    return 1;
}

solution tabuSearch(const solution &solution) {
    auto byLength = [](const auto &item1, const auto &item2) { return getLength(item1.first) > getLength(item2.first); };

    auto best = solution;
    auto candidate = std::make_pair(solution, 0);
    auto failsCount = 0;
    std::list<int> tabuList;

    while (failsCount < config::failsLimit and config::properRunTime()) {
        auto neighbours = getNeighbours(tabuList, best);
        candidate = *std::max_element(neighbours.begin(), neighbours.end(), byLength);

        if (getLength(candidate.first) > getLength(best)) {
            best = candidate.first;
            tabuList.push_back(candidate.second);
        } else
            failsCount++;

        if (tabuList.size() > config::tabuListSizeLimit)
            tabuList.pop_front();
    }

    return best;
}

#endif //OK_TABUSEARCH_HPP
